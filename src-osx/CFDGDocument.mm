// CFDGDocument.mm
// this file is part of Context Free
// ---------------------
// Copyright (C) 2005-2007 Mark Lentczner - markl@glyphic.com
// Copyright (C) 2008 John Horigan - john@glyphic.com
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
// 
// John Horigan can be contacted at john@glyphic.com or at
// John Horigan, 1209 Villa St., Mountain View, CA 94041-1123, USA
//
// Mark Lentczner can be contacted at markl@glyphic.com or at
// Mark Lentczner, 1209 Villa St., Mountain View, CA 94041-1123, USA
//
//

#import "CFDGDocument.h"
#import "GView.h"
#import "GalleryUploader.h"

#include <fstream>
#include <sstream>

#include "cfdg.h"


namespace {
    using namespace std;
    
    class DocSystem : public AbstractSystem
    {
    public:
        DocSystem(CFDGDocument* doc) : mDoc(doc) { }
        
        virtual void message(bool error, const char* fmt, ...);
        virtual void syntaxError(const string& path, int line);
        
        virtual istream* openFileForRead(const string& path);
        virtual istream* tempFileForRead(const string& path);
        virtual ostream* tempFileForWrite(string& prefixInNameOut);
            
        virtual string relativeFilePath(const string& base, const string& rel);
        
        virtual void stats(const Stats& s);
        
    private:
        CFDGDocument* mDoc;
        
        CFDGDocument* findDocFor(const string& path);
    };
    
    CFDGDocument*
    DocSystem::findDocFor(const string& path)
    {
        if (path.empty())
            return mDoc;

        NSString* pathStr =
            [NSString stringWithUTF8String: path.c_str()];
            
        return [[NSDocumentController sharedDocumentController]
                    documentForFileName: pathStr];
    }
    
    
    void
    DocSystem::message(bool, const char* fmt, ...)
    {
        char buf[256];
        {
            va_list args;
            va_start(args, fmt);
            vsnprintf(buf, sizeof(buf), fmt, args);
            buf[sizeof(buf)-1] = '\0';
            va_end(args);
        }
        
        [mDoc performSelectorOnMainThread: @selector(noteStatus:)
            withObject: [NSString stringWithUTF8String: buf]
            waitUntilDone: NO];
    }

    void
    DocSystem::stats(const Stats& s)
    {
        NSValue* value = [NSValue value: &s withObjCType: @encode(Stats)];
        
        [mDoc performSelectorOnMainThread: @selector(noteStats:)
            withObject: value waitUntilDone: NO];
    }
    
    
    void
    DocSystem::syntaxError(const string& path, int line)
    {
        // note: this only happens during Parse, 
        // and that only happens in the main thread
        
        CFDGDocument* doc = findDocFor(path);
        if (doc)
            [doc highlightLine: line];
    }
    
            
    istream*
    DocSystem::openFileForRead(const string& path)
    {
        CFDGDocument* doc = findDocFor(path);
                
        if (doc) {
            NSData* data = [doc getContent];
            basic_stringstream<char>* s = new stringstream;
            s->write((const char*)[data bytes], [data length]);
            s->seekp(0, ios::beg);
            return s;
        }

        return new ifstream(path.c_str(), ios::in);
    }
    
    istream*
    DocSystem::tempFileForRead(const string& path)
    {
        return new ifstream(path.c_str(), ios::in | ios::binary);
    }

    ostream*
    DocSystem::tempFileForWrite(string& prefixInNameOut)
    {
        string t = prefixInNameOut + "XXXXXX";

        NSString* prefix =
            [NSString stringWithUTF8String: t.c_str()];
        NSString* path =
            [NSTemporaryDirectory() stringByAppendingPathComponent: prefix];
                
        ofstream* f = 0;
        
        char* b = strdup([path cString]);
        if (mktemp(b)) {
            f = new ofstream;
            f->open(b, ios::binary | ios::trunc | ios::out);
            prefixInNameOut = b;
        }
        delete b;
        
        return f;
    }
            
    string
    DocSystem::relativeFilePath(const string& base, const string& rel)
    {
        NSString* baseStr =
            [NSString stringWithUTF8String: base.c_str()];
        NSString* relStr =
            [NSString stringWithUTF8String: rel.c_str()];
        
        NSString* newStr =
            [[baseStr stringByDeletingLastPathComponent]
                stringByAppendingPathComponent: relStr];
        
        NSFileManager* fm = [NSFileManager defaultManager];
        if (![fm fileExistsAtPath: newStr]) {
            NSString* libStr =
                [[[[NSBundle mainBundle] resourcePath]
                    stringByAppendingPathComponent: @"Examples"]
                        stringByAppendingPathComponent: relStr];
            if ([fm fileExistsAtPath: libStr])
                newStr = libStr;
        }

        return string([newStr cString]);
    }
};


NSString* CFDGDocumentType = @"ContextFree Design Grammar";

@implementation CFDGDocument

+ (NSString*) documentType
{
    return CFDGDocumentType;
}

- (id)init
{
    self = [super init];
    if (self) {
        mSystem = new DocSystem(self);
        mContent = nil;
		mUploader = nil;
        mDisplayName = nil;
    }
    return self;
}

- (void)dealloc
{
    delete mSystem;			mSystem = 0;
    [mContent release];		mContent = nil;
	[mUploader release];	mUploader = nil;
    [super dealloc];
}

- (NSString *)windowNibName {
    return @"CFDGDocument";
}


- (void)showWindows
{
    [super showWindows];

    if ([[mEditor string] length] == 0)
        return;
        
    BOOL render = [[NSUserDefaults standardUserDefaults]
                        boolForKey: @"RenderOnDocumentOpen"];
    if (render)
        [self startRender: self];
}

- (void)close
{
    mStatus = nil;
    mGView = nil;
    [self stopRender: self];
    [super close];
}

- (CFDG*)buildEngine
{
    [mEditor setTextColor: nil];
    
    const char* fileName = [[[self fileURL] path] UTF8String];
    if (!fileName) fileName = "";
    
    static NSLock* parseLock = [[NSLock alloc] init];
    
    CFDG* result = 0;
    if ([parseLock lockBeforeDate: [NSDate dateWithTimeIntervalSinceNow: 2.0]]) {
        result = CFDG::ParseFile(fileName, mSystem, [mGView variation]);
        [parseLock unlock];
    }
    return result;
}

- (void)noteStatus:(NSString*)s
{
    if (mStatus != nil) {
        [mStatus setStringValue: s];
        [mStatus setToolTip: s];
    }
}

- (void)noteStats:(NSValue*)v
{
    if (mGView != nil) {
        [mGView noteStats: v];
    }
}

- (void)highlightLine:(int)targetLine
{
    NSString* text = [mEditor string];
    
    NSRange lineRange;
    unsigned int lineCount;
    unsigned int endOfLine;
    
    lineRange.location = 0;
    lineRange.length = 0;
    lineCount = 1;          // lineRange(0,0) is the start of line 1!
    
    while ((int)lineCount < targetLine) {
        // find start of next line
        [text getLineStart: nil end: &endOfLine contentsEnd: nil
            forRange: lineRange];
        lineRange.location = endOfLine;
        lineCount += 1;
    }

    // now find the range of the whole line
    [text getLineStart: nil end: &endOfLine contentsEnd: nil
        forRange: lineRange];
    lineRange.length = endOfLine - lineRange.location;
    
    [mEditor setTextColor: [NSColor redColor] range: lineRange];
    [mEditor setSelectedRange: lineRange];
    [mEditor scrollRangeToVisible: lineRange];
}


- (NSData *)dataRepresentationOfType:(NSString *)type {
    NSAssert([type isEqualToString: CFDGDocumentType], @"Unknown type");

    return [self getContent];
}

- (BOOL)loadDataRepresentation:(NSData *)data ofType:(NSString *)type
{
    NSAssert([type isEqualToString: CFDGDocumentType], @"Unknown type");

    mContent = [data retain];
    [self showContent];
    
    return YES;
}

- (void)windowControllerDidLoadNib:(NSWindowController *)windowController
{
    [self showContent];
}



- (IBAction)toggleRenderView:(id)sender
{
}

- (IBAction) startRender:(id)sender { [mGView startRender: sender]; }
- (IBAction) repeatRender:(id)sender{ [mGView repeatRender: sender]; }
- (IBAction) finishRender:(id)sender{ [mGView finishRender: sender]; }
- (IBAction) stopRender:(id)sender  { [mGView stopRender: sender]; }
- (IBAction) saveImage:(id)sender   { [mGView saveImage: sender]; }
- (IBAction) saveAsSVG:(id)sender   { [mGView saveAsSVG: sender]; }
- (IBAction) saveAsMovie:(id)sender { [mGView saveAsMovie: sender]; }
- (IBAction) toggleLegacyVariations:(id)sender
                                    { [mGView toggleLegacyVariations: sender]; }


- (BOOL)validateMenuItem:(NSMenuItem *)anItem;
{
    SEL action = [anItem action];
   
    if (action == @selector(startRender:)
    ||  action == @selector(repeatRender:)
    ||  action == @selector(finishRender:)
    ||  action == @selector(stopRender:)
    ||  action == @selector(saveImage:)
    ||  action == @selector(saveAsSVG:)
    ||  action == @selector(saveAsMovie:)
    ||  action == @selector(showHiresRenderSheet:)
    ||  action == @selector(uploadToGallery:)
    ||  action == @selector(toggleLegacyVariations:)
    ||  action == @selector(showSvgRenderSheet:))
    {
        return [mGView validateMenuItem: anItem];
    }
    
    return [super validateMenuItem: anItem];
}

- (IBAction) showHiresRenderSheet:(id)sender
{
    [NSApp beginSheet: mHiresSheet
        modalForWindow: [[[self windowControllers] lastObject] window]
        modalDelegate: nil didEndSelector: nil contextInfo: nil];
}

- (IBAction) startHiresRender:(id)sender
{
	if (![mHiresSheet makeFirstResponder: nil])
		return;
	
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    float hiresWidth = [defaults floatForKey: @"HiresWidth"];
    float hiresHeight = [defaults floatForKey: @"HiresHeight"];
    float hiresMinSize = [defaults floatForKey: @"HiresMinimumSize"];

    [mGView startHiresRender: NSMakeSize(hiresWidth, hiresHeight)
                minimum: hiresMinSize];
    [self cancelHiresRender:sender];
}

- (IBAction) cancelHiresRender:(id)sender
{
    [NSApp endSheet: mHiresSheet];
    [mHiresSheet orderOut: sender];
}

- (IBAction) uploadToGallery:(id)sender
{
	if (!mUploader) {
		mUploader= [[GalleryUploader alloc]
							initForDocument: self andView: mGView];
	}
	[mUploader show: self];
}

- (void)showContent
{
    if (mEditor) {
        NSString* s = [[NSString alloc]
                        initWithData: mContent encoding: NSUTF8StringEncoding];
        [mEditor setString: s];
        [s release];
    }
}

- (NSData*)getContent
{
    [mContent release];
    
    mContent = [[[mEditor string]
                    dataUsingEncoding: NSUTF8StringEncoding] retain];
    return mContent;
}


- (void)readFromExample:(NSString*)path
{
    [self loadDataRepresentation: [NSData dataWithContentsOfFile: path]
        ofType:CFDGDocumentType];

    [mDisplayName release];
    mDisplayName = [[[path lastPathComponent]
                            stringByDeletingPathExtension] retain];
}

- (NSString*)displayName
{
    if (mDisplayName) return mDisplayName;
    return [super displayName];
}

- (void)setFileName:(NSString*)path
{
    [mDisplayName release];
    mDisplayName = nil;
    [super setFileName: path];
}

- (void)setFileURL:(NSURL*)path
{
    [mDisplayName release];
    mDisplayName = nil;
    [super setFileURL: path];
}


// Delegate methods from the editor
- (void) textDidChange: (NSNotification *) notification
{
    [self stopRender: self];
    [mGView reuseVariation];
}

@end


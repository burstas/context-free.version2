// ImageCanvas.mm
// this file is part of Context Free
// ---------------------
// Copyright (C) 2006 Mark Lentczner - markl@glyphic.com
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
// Mark Lentczner can be contacted at markl@glyphic.com or at
// Mark Lentczner, 1209 Villa St., Mountain View, CA 94041-1123, USA
//
//

#import "imagecanvas.h"

#import <Cocoa/Cocoa.h>

#import "GView.h"


ImageCanvas::ImageCanvas(GView* view, NSBitmapImageRep* bitmap,
		PixelFormat format)
    : aggCanvas(format), mView(view)
{
    NSSize size = [bitmap size];
    int pixelWidth = (int)size.width;
    int pixelHeight = (int)size.height;
        
    attach([bitmap bitmapData], pixelWidth, pixelHeight,
        [bitmap bytesPerRow]);
}

ImageCanvas::~ImageCanvas()
    { }

void
ImageCanvas::start(bool clear,  const agg::rgba& background,
    int width, int height)
{
    aggCanvas::start(clear, background, width, height);
}

void
ImageCanvas::end()
{
    aggCanvas::end();

    [mView performSelectorOnMainThread: @selector(redisplayImage:)
        withObject: [NSValue valueWithRect:
            NSMakeRect(cropX(), cropY(), cropWidth(), cropHeight())]
        waitUntilDone: YES];
}

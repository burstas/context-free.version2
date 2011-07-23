// qtCanvas.cpp
// this file is part of Context Free
// ---------------------
// Copyright (C) 2006-2008 Mark Lentczner - markl@glyphic.com
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


#include "qtCanvas.h"

#ifdef _WIN32

#include <QTML.h>
#include <QuickTimeComponents.h>
#include <GXMath.h>
#include <MediaHandlers.h>

#define GetPortPixMap(port) (port->portPixMap)

#else
#include <QuickTime/QuickTime.h>
#endif
#include <iostream>

static bool
CheckError(OSErr err, const char* name)
{
    if (err != noErr)
        std::cerr << "error " << std::dec << err << " in " << name << std::endl;
    return err == noErr;
}

static bool
CheckMoviesError(const char* name)
{
	return CheckError(GetMoviesError(), name);
}

bool qtCanvas::systemInited = false;

bool
qtCanvas::systemInit()
{
    if (!qtCanvas::systemInited) {
        OSErr err = noErr;
#ifdef _WIN32
        err = InitializeQTML(kInitializeQTMLNoSoundFlag);
        CheckError (err, "InitializeQTML error" );
#endif
        qtCanvas::systemInited = err == noErr;
    }
    return qtCanvas::systemInited;
} 

void
qtCanvas::systemExit()
{
    if (qtCanvas::systemInited) {
#ifdef _WIN32
        TerminateQTML();
#endif
        qtCanvas::systemInited = false;
    }
}



class qtCanvas::Impl
{
public:
	Impl(short width, short height);
	~Impl();

	bool prepareMovie(CFStringRef);
	
	void addFrame();
	void showSettingsDialog();
	void setPreview(PixelFormat format);
	
private:
	void setupSCComponent();	void finishSCComponent();
	bool setupMovie();			void finishMovie();
	bool setupTrack();			void finishTrack();
	void setupImage();			void finishImage();
	bool setupCompression();	void finishCompression();

private:
    CFStringRef		mPath;
	double			mFrameRate;
    short             mWidth;
    short             mHeight;

	ComponentInstance
					mSCComponent;
	static Handle	sSCSettings;

    DataHandler		mDataHandler;
	Movie			mMovie;

	Track			mTrack;
	Media			mMedia;

    Rect			mFrame;
    unsigned char*	mImageData;
	unsigned int	mImageStride;
	GWorldPtr		mGWorld;
	
	ImageDescriptionHandle
					mImageDesc;
	
	friend class qtCanvas;
};

Handle qtCanvas::Impl::sSCSettings = nil;


void
qtCanvas::Impl::setupSCComponent()
{
	OSErr err;
	
    err = EnterMovies();
    CheckError (err, "EnterMovies error" );

	mSCComponent = OpenDefaultComponent(
						StandardCompressionType,
						StandardCompressionSubType);
	CheckMoviesError("OpenDefaultComponent");

	if (sSCSettings) {
		SCSetInfo(mSCComponent, scSettingsStateType, &sSCSettings);
	}
	else {
		SCSpatialSettings	spatial;
		SCTemporalSettings	temporal;
		long				preference;
		CodecFlags			codecFlags;
		
		spatial.codecType = kAnimationCodecType;
		spatial.codec = NULL;
		spatial.depth = 32; // reset when the preview is set up
		spatial.spatialQuality = codecNormalQuality;
		
		temporal.temporalQuality = codecNormalQuality;
		temporal.frameRate = FloatToFixed(15.0);
		temporal.keyFrameRate = FixedToInt(temporal.frameRate) * 2;
		
		preference = scListEveryCodec;
		//preference |= scShowBestDepth;
		//preference |= scUseMovableModal;
		
		codecFlags = codecFlagUpdatePreviousComp;
		
		SCSetInfo(mSCComponent, scSpatialSettingsType, &spatial);
		SCSetInfo(mSCComponent, scTemporalSettingsType, &temporal);
		SCSetInfo(mSCComponent, scPreferenceFlagsType, &preference);
		SCSetInfo(mSCComponent, scCodecFlagsType, &codecFlags);
	}
}

void
qtCanvas::Impl::finishSCComponent()
{
	if (sSCSettings)
		DisposeHandle(sSCSettings);
	SCGetInfo(mSCComponent, scSettingsStateType, &sSCSettings);

	CloseComponent(mSCComponent);
	CheckMoviesError("CloseComponent");
}

void
qtCanvas::Impl::setPreview(PixelFormat format)
{
	SCSpatialSettings	spatial;
	SCGetInfo(mSCComponent, scSpatialSettingsType, &spatial);
    
    switch (format) {
        case RGB8_Blend:
            spatial.depth = 24;
            break;
        case Gray8_Blend:
            spatial.depth = 40;
            break;
        default:
            spatial.depth = 32;
            break;
    }
    
	SCSetInfo(mSCComponent, scSpatialSettingsType, &spatial);

	SCSetTestImagePixMap(mSCComponent,
		GetPortPixMap(mGWorld), &mFrame,
		scPreferScaling
		);
    CheckMoviesError("SCSetTestImagePixMap");
}

void
qtCanvas::Impl::showSettingsDialog()
{
	SCRequestSequenceSettings(mSCComponent);
}


bool
qtCanvas::Impl::setupMovie()
{
	OSErr err;

	Handle dataRefH = nil;
	OSType dataRefType = 0;
	
	err = QTNewDataReferenceFromFullPathCFString(
			mPath, (UInt32)kQTNativeDefaultPathStyle, 0,
			&dataRefH, &dataRefType);
    CheckError(err, "QTNewDataReferenceFromFullPathCFString");

	CreateMovieStorage(dataRefH, dataRefType,
			'TVOD', smSystemScript,
			createMovieFileDeleteCurFile |
				createMovieFileDontCreateResFile |
				newMovieActive,
			&mDataHandler, &mMovie);
    CheckMoviesError("CreateMovieStorage");
    CheckError((OSErr)((nil != mMovie) ? noErr : -1), "CreateMovieStorage movie");
    
    return (err == noErr) && (GetMoviesError() == noErr) && (nil != mMovie);
}

void
qtCanvas::Impl::finishMovie()
{
	if (mMovie) {
		UpdateMovieInStorage(mMovie, mDataHandler);
		CheckMoviesError("UpdateMovieInStorage");
		
		DisposeMovie(mMovie);
	}
	
    if (mDataHandler) {
        CloseMovieStorage(mDataHandler);
        CheckMoviesError("CloseMovieStorage");
    }
}

bool
qtCanvas::Impl::setupTrack()
{
    mTrack = NewMovieTrack(mMovie,
				FixRatio(mWidth,1), FixRatio(mHeight,1),
				(short)kNoVolume);
	if (!CheckMoviesError("NewMovieTrack")) return false;


	SCTemporalSettings	temporal;
	SCGetInfo(mSCComponent, scTemporalSettingsType, &temporal);

    mMedia = NewTrackMedia(mTrack,
				VideoMediaType, (TimeScale)temporal.frameRate,
				nil, 0);
    if (!CheckMoviesError("NewTrackMedia" )) return false;
    
    MediaHandler mh = GetMediaHandler(mMedia);
    if (!CheckMoviesError("GetMediaHandler")) return false;
    
    ComponentResult err;
    err = MediaSetGraphicsMode(mh, graphicsModePreMulColorAlpha, NULL);
    if (!CheckError(err, "MediaSetGraphicsMode")) return false;

    BeginMediaEdits(mMedia);
    if (!CheckMoviesError("BeginMediaEdits" )) return false;
    return true;
}

void
qtCanvas::Impl::finishTrack()
{
	if (mMedia) {
		EndMediaEdits(mMedia);
		CheckMoviesError("EndMediaEdits");

		if (mTrack) {
			InsertMediaIntoTrack(mTrack,
						0, 0, GetMediaDuration(mMedia), fixed1);
			CheckMoviesError("InsertMediaIntoTrack" );
		}
	}
}

void
qtCanvas::Impl::setupImage()
{
    MacSetRect(&mFrame, 0, 0, mWidth, mHeight);
    
    mImageStride = mWidth * 4;
    mImageData = new unsigned char[mImageStride * mHeight];

    OSErr err;
	err = QTNewGWorldFromPtr(&mGWorld,
				k32ARGBPixelFormat, &mFrame,
				nil, nil, (GWorldFlags)0,
				mImageData, mImageStride);
    CheckError(err, "QTNewGWorldFromPtr");
}

void
qtCanvas::Impl::finishImage()
{
    if (mGWorld) {
        DisposeGWorld (mGWorld);
    }

    delete[] mImageData;
}

bool
qtCanvas::Impl::setupCompression()
{
	SCCompressSequenceBegin(mSCComponent,
		GetPortPixMap(mGWorld), &mFrame,
		&mImageDesc);
	return CheckMoviesError("SCCompressSequenceBegin");
}

void
qtCanvas::Impl::finishCompression()
{
	SCCompressSequenceEnd(mSCComponent);
	CheckMoviesError("SCCompressSequenceEnd");
}

void
qtCanvas::Impl::addFrame()
{
	if (!mImageData) return;
	
	Handle	compressedData;
	long	compressedSize;

	short notSyncFlag;
	SCCompressSequenceFrame(mSCComponent,
			GetPortPixMap(mGWorld), &mFrame,
			&compressedData, &compressedSize,
			&notSyncFlag);
    CheckMoviesError("SCCompressSequenceFrame");

    AddMediaSample(mMedia,
			compressedData, 0, compressedSize,
			(TimeValue)fixed1,
			(SampleDescriptionHandle)mImageDesc,
			1, notSyncFlag, nil);
    CheckMoviesError("AddMediaSample");
}


qtCanvas::Impl::Impl(short width, short height)
	: mPath(nil), mFrameRate(12.0),
	mWidth(width), mHeight(height),
	mSCComponent(nil),
	mDataHandler(nil), mMovie(nil),
	mTrack(nil), mMedia(nil),
	mImageData(0), mGWorld(nil),
	mImageDesc(nil)
{
    ClearMoviesStickyError();
	setupSCComponent();
	setupImage();
}

bool
qtCanvas::Impl::prepareMovie(CFStringRef path)
{
    mPath = path;
	CFRetain(mPath);
	
    return (GetMoviesStickyError() == noErr) && 
        setupMovie() && setupTrack() && setupCompression();
}

qtCanvas::Impl::~Impl()
{
	finishCompression();
	finishTrack();
	finishMovie();
	
	if (mPath) CFRelease(mPath);
	
	finishImage();
	finishSCComponent();
}



qtCanvas::qtCanvas(int width, int height)
    : aggCanvas(QT_Blend), impl(* new Impl((short)width, (short)height))
{
    attach(impl.mImageData, impl.mWidth, impl.mHeight, impl.mImageStride);
}

qtCanvas::~qtCanvas()
{
    delete &impl;
}

void
qtCanvas::end()
{
    aggCanvas::end();
	
	impl.addFrame();
}

bool
qtCanvas::prepareMovie(CFStringRef path)
{
	return impl.prepareMovie(path);
}
void
qtCanvas::setPreview(void* data, unsigned width, unsigned height,
	int stride, PixelFormat format)
{
	copy(data, width, height, stride, format);
	impl.setPreview(format);
}

void
qtCanvas::showSettingsDialog()
{
	impl.showSettingsDialog();
}

int
qtCanvas::convertToFrames(float seconds)
{
	SCTemporalSettings	temporal;
	
	SCGetInfo(impl.mSCComponent, scTemporalSettingsType, &temporal);
	
	return (int)(FixedToFloat(temporal.frameRate) * seconds);
}


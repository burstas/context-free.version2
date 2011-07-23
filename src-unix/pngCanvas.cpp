// pngCanvas.cpp
// Context Free
// ---------------------
// Copyright (C) 2005-2007 Mark Lentczner - markl@glyphic.com
// Copyright (C) 2007-2008 John Horigan - john@glyphic.com
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
// John Horigan can be contacted at john@glyphic.com or at
// John Horigan, 1209 Villa St., Mountain View, CA 94041-1123, USA
//
//

#include "pngCanvas.h"
#include "png.h"
#include <stdlib.h>
#include <iostream>
#include "makeCFfilename.h"

using namespace std;

pngCanvas::pngCanvas(const char* outfilename, bool quiet, int width, int height, 
                     aggCanvas::PixelFormat pixfmt, bool crop, int frameCount,
                     int variation)
    : aggCanvas(pixfmt), mData(0), mOutputFileName(outfilename),  
      mFrameCount(frameCount), mCurrentFrame(0), 
      mPixelFormat(pixfmt), mCrop(crop), mQuiet(quiet), mVariation(variation)
{
	mWidth = width;
	mHeight = height;
	if (quiet) return;
    cout << width << "w x " << height << "h pixel image." << endl;
    cout << "Generating..." << endl;
}

pngCanvas::~pngCanvas()
{
    delete[] mData;
}

void pngCanvas::start(bool clear, const agg::rgba& bk, int width, int height)
{
	if (!mFrameCount && !mQuiet)
        cout << endl << "Rendering..." << endl;

    if (mCrop) {
        mWidth = width;
        mHeight = height;
    }

    mStride = mWidth * aggCanvas::BytesPerPixel[mPixelFormat];
    mData = new unsigned char[mStride * mHeight];
    attach(mData, mWidth, mHeight, mStride);

    aggCanvas::start(clear, bk, width, height);
}

void pngCanvas::end()
{
	aggCanvas::end();
	
	string name = makeCFfilename(mOutputFileName, mCurrentFrame, mFrameCount,
	                             mVariation);
	
	if (mFrameCount) {
		output(name.c_str(), mCurrentFrame++);
	} else {
		output(name.c_str());
	}
}

namespace {
    void
    pngWriteError(png_structp png_ptr, png_const_charp message)
    {
        cerr << message << endl;
        throw false;
    }

    void
    pngWriteWarning(png_structp png_ptr, png_const_charp message)
    {
        cerr << message << endl;
    }
}


void pngCanvas::output(const char* outfilename, int frame)
{
	if (frame == -1 && !mQuiet) {
        cout << endl << "Writing "
             << mWidth << "w x " << mHeight << "h pixel image..." << endl;
	} 

    FILE *out = 0;
    png_structp png_ptr = 0;
    png_infop info_ptr = 0;

    png_bytep row = new png_byte[mStride];
    
    try {
        png_ptr = png_create_write_struct(
            PNG_LIBPNG_VER_STRING,
            0, pngWriteError, pngWriteWarning);
        if (!png_ptr) throw "couldn't create png write struct";

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) throw "couldn't create png info struct";

        out = fopen(outfilename, "wb");
        if (!out) {
            cerr << "Couldn't open " << outfilename << "\n";
            throw false;
        }

        png_init_io(png_ptr, out);
        
        int pngFormat;
        switch (mPixelFormat) {
            case aggCanvas::RGBA8_Blend:
                pngFormat = PNG_COLOR_TYPE_RGB_ALPHA;
                break;
            case aggCanvas::RGB8_Blend:
                pngFormat = PNG_COLOR_TYPE_RGB;
                break;
            case aggCanvas::Gray8_Blend:
                pngFormat = PNG_COLOR_TYPE_GRAY;
                break;
            default:
                throw "Unknown pixel format";
        }

        png_set_IHDR(png_ptr, info_ptr,
            mWidth, mHeight, 8, pngFormat,
            PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT);

        png_text comments[1];
        memset(comments, 0, sizeof(comments));
        comments[0].compression = PNG_TEXT_COMPRESSION_NONE;
        comments[0].key = (char*)"Software";
        comments[0].text = (char*)"Context Free";
        comments[0].text_length = strlen(comments[0].text);
        png_set_text(png_ptr, info_ptr,
            comments, sizeof(comments)/sizeof(comments[0]));

        png_write_info(png_ptr, info_ptr);

        png_bytep rowPtr = mData;
        for (int r = 0; r < mHeight; ++r) {
            if (mPixelFormat == aggCanvas::RGBA8_Blend) {
                // Convert each row to non-premultiplied alpha as per PNG spec
                // This is done in a separate array instead of in-situ because
                // for animations the main buffer might be drawn into again
                for (int c = 0; c < mStride; c += 4) {
                    row[c + 0] = rowPtr[c + 0]; row[c + 1] = rowPtr[c + 1];
                    row[c + 2] = rowPtr[c + 2]; row[c + 3] = rowPtr[c + 3];
                    agg::rgba8* pix = (agg::rgba8*)(row + c);
                    pix->demultiply();
                }
            }
            png_write_row(png_ptr, (mPixelFormat == aggCanvas::RGBA8_Blend) ?
                row : rowPtr);
            rowPtr += mStride;
        }

        png_write_end(png_ptr, 0);

        if (fclose(out) != 0) throw "File I/O error!?!?!";
        out = 0;
    }
    catch (const char* msg) {
        cerr << "***" << msg << endl;
    }
    catch (bool) { }
    
    if (row)        delete[] row;

    if (out)        fclose(out), out = 0;
    if (png_ptr)    png_destroy_write_struct(&png_ptr, &info_ptr);
}



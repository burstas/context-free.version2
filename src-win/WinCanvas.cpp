// WinCanvas.cpp
// this file is part of Context Free
// ---------------------
// Copyright (C) 2005-2008 John Horigan - john@glyphic.com
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
//

#include "WinCanvas.h"
#include "WinSystem.h"
#include <windows.h>

void WinCanvas::start(bool clear, const agg::rgba& bk, int actualWidth, int actualHeight)
{
    aggCanvas::start(clear, bk, actualWidth, actualHeight);
}

void WinCanvas::end()
{
    aggCanvas::end();

    // This message tells the Document view to update the window
    mSystem->statusUpdate();
}

WinCanvas::WinCanvas(WinSystem* sys, aggCanvas::PixelFormat pixfmt, 
                     int width, int height, agg::rgba bkgrnd)
:   aggCanvas(pixfmt),
    mPixelFormat(pixfmt),
    mBM(0),
    mBackground(bkgrnd),
    mSystem(sys)
{
    mWidth = width;
    mHeight = height;
    mStride = mWidth * aggCanvas::BytesPerPixel[mPixelFormat];

    // pad row stride to DWORD boundary
    mStride += ((-mStride) & 3);

    mBM = (char*)(new DWORD[mStride * mHeight / 4]);
    aggCanvas::attach((void*)mBM, mWidth, mHeight, mStride);

    // clear the bitmap
    aggCanvas::start(true, bkgrnd, width, height);
    aggCanvas::end();
}

WinCanvas::~WinCanvas()
{
    delete[] mBM;
}


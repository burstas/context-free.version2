// aggCanvas.h
// this file is part of Context Free
// ---------------------
// Copyright (C) 2005-2008 Mark Lentczner - markl@glyphic.com
// Copyright (C) 2006-2008 John Horigan - john@glyphic.com
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


#ifndef INCLUDE_AGGCANVAS_H
#define INCLUDE_AGGCANVAS_H

#include "cfdg.h"
#include "agg_trans_affine.h"
#include "agg_path_storage.h"

class aggCanvas : public Canvas {
    public:
        enum PixelFormat {Gray8_Blend = 0, RGBA8_Blend = 1, QT_Blend = 2, RGB8_Blend = 3};
        static int BytesPerPixel[4];
        void start(bool clear, const agg::rgba& bk, int width, int height);
        void end();

        void circle(RGBA8 c, agg::trans_affine tr);
        void square(RGBA8 c, agg::trans_affine tr);
        void triangle(RGBA8 c, agg::trans_affine tr);
        void path(RGBA8 c, agg::trans_affine tr, agg::path_storage* path, 
                  pathAttr* attr);
        
		bool colorCount256();
			// return whether the aggCanvas can fit in byte pixels
        
        static PixelFormat SuggestPixelFormat(CFDG* engine);
        
    protected:
        aggCanvas(PixelFormat);
        virtual ~aggCanvas();
        
        void attach(void* data, unsigned width, unsigned height, int stride);
            // data is int8u grayscale pixels or int32u pixels
        
		void copy(void* data, unsigned width, unsigned height,
							int stride, PixelFormat format);
			// copy the image from different pixel buffer
			
    public:
        int cropX();
        int cropY();
        int cropWidth();
        int cropHeight();
    
    //private:
    public:
        class impl;
        impl* m;
};

#endif // INCLUDE_AGGCANVAS_H



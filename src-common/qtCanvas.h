// qtCanvas.h
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


#ifndef INCLUDE_QTCANVAS_H
#define INCLUDE_QTCANVAS_H

// forward decls and ugly typedef from CFBase.h -- pointer types are bad!
struct __CFString;
typedef const struct __CFString * CFStringRef;

#include "aggCanvas.h"

class qtCanvas : public aggCanvas {
    public:
        qtCanvas(int width, int height);
        ~qtCanvas();
        
        void end();

		void setPreview(void* data, unsigned width, unsigned height,
							int stride, PixelFormat format);
		void showSettingsDialog();
		bool prepareMovie(CFStringRef);
		
		int convertToFrames(float seconds);

        static bool systemInit();
        static void systemExit();
        
    private:
        static bool systemInited;
        class Impl;
        Impl& impl;
        qtCanvas& operator=(qtCanvas& c) { return c; }
};

#endif // INCLUDE_QTCANVAS_H


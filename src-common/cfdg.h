// cfdg.h
// this file is part of Context Free
// ---------------------
// Copyright (C) 2005-2008 Mark Lentczner - markl@glyphic.com
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
// Mark Lentczner can be contacted at markl@glyphic.com or at
// Mark Lentczner, 1209 Villa St., Mountain View, CA 94041-1123, USA
//
//


#ifndef INCLUDE_CFDG_H
#define INCLUDE_CFDG_H

#include <iosfwd>
#include <string>
#include <time.h>
#include "agg_trans_affine.h"
#include "agg_color_rgba.h"
#include "agg_path_storage.h"

typedef agg::rgba8 RGBA8;

struct pathAttr;

class AbstractSystem {
    public:
        virtual void message(bool error, const char* fmt, ...) = 0;
        virtual void syntaxError(const std::string& filepath, int line) = 0;
        
        virtual std::istream* openFileForRead(const std::string& path) = 0;
        virtual std::istream* tempFileForRead(const std::string& path) = 0;
        virtual std::ostream* tempFileForWrite(std::string& prefixInNameOut) = 0;
            // caller must delete returned streams when done
            
        virtual std::string relativeFilePath(
            const std::string& base, const std::string& rel) = 0;
        
        struct Stats {
            int     shapeCount;     // finished shapes in image
            int     toDoCount;      // unfinished shapes still to expand
            
            bool    inOutput;       // true if we are in the output loop
            bool    fullOutput;     // not an incremental output
            bool    finalOutput;    // the last output
            int     outputCount;    // number to be output
            int     outputDone;     // number output so far
            clock_t outputTime;
			
			bool	animating;		// inside the animation loop
        };

        virtual void stats(const Stats&);
        
        virtual ~AbstractSystem();
};

class Canvas {
    public:
        virtual void start(bool , const agg::rgba& , int , int ) 
        { mTime = clock(); }
        virtual void end()
        { mTime = clock() - mTime; }

        virtual void circle(RGBA8 , agg::trans_affine ) = 0;
        virtual void square(RGBA8 , agg::trans_affine ) = 0;
        virtual void triangle(RGBA8 , agg::trans_affine ) = 0;
        virtual void path(RGBA8 , agg::trans_affine, agg::path_storage*, pathAttr* ) = 0;
        virtual void scale(double ) {}

		Canvas(int width, int height) 
			: mWidth(width), mHeight(height) {}
        virtual ~Canvas();
		
		int mWidth;
		int mHeight;
        clock_t mTime;
};

class Renderer;

class CFDG {
    public:
        static CFDG* ParseFile(const char* fname, AbstractSystem*, int variation);
        virtual ~CFDG();

        virtual Renderer* renderer(
                int width, int height, float minSize,
                int variation, double border = 2.0
            ) = 0;
            // caller must delete returned object

        bool usesColor;
        bool usesAlpha;
        virtual bool isTiled(agg::trans_affine* tr = 0, double* x = 0, double* y = 0) const = 0;
        virtual bool isSized(double* x = 0, double* y = 0) const = 0;
		
		agg::rgba backgroundColor;

    protected:
        CFDG()
        : usesColor(false), usesAlpha(false), backgroundColor(1, 1, 1, 1)
        { }
};


class Renderer {
    public:
        virtual ~Renderer();
        
        virtual void setMaxShapes(int n) = 0;        
        virtual void resetBounds() = 0;

        virtual double run(Canvas* canvas, bool partialDraw) = 0;
        virtual void draw(Canvas* canvas) = 0;
        virtual void animate(Canvas* canvas, int frames, bool zoom) = 0;

        volatile bool requestStop;     // stop ASAP
        volatile bool requestFinishUp; // stop expanding, and do final output
        volatile bool requestUpdate;   // call stats() soon
        
        int m_width;
        int m_height;
    
    protected:
        Renderer() 
        : requestStop(false),
          requestFinishUp(false),
          requestUpdate(false)
     { }
};

#endif

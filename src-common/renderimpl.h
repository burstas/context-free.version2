// renderimpl.h
// this file is part of Context Free
// ---------------------
// Copyright (C) 2006-2008 Mark Lentczner - markl@glyphic.com
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


#ifndef INCLUDE_RENDERIMPL_H
#define INCLUDE_RENDERIMPL_H

#ifdef _WIN32
#pragma warning( disable : 4786 )
#endif

#include <deque>
#include <set>

#include "agg_trans_affine.h"
#include "bounds.h"
#include "cfdg.h"
#include "cfdgimpl.h"
#include "shape.h"
#include "tempfile.h"
#include "shape.h"

class ShapeOp;


class RendererImpl : public Renderer {
    public:
        RendererImpl(CFDGImpl* cfdg,
                        int width, int height, float minSize,
                        int variation, double border);
        ~RendererImpl();

        void setMaxShapes(int n);
        void resetBounds();
        
        double run(Canvas* canvas, bool partialDraw);
        void draw(Canvas* canvas);
        void animate(Canvas* canvas, int frames, bool zoom);
        
    private:
        void outputPrep(Canvas*);
        void rescaleOutput(int& curr_width, int& curr_height, bool final);
		void forEachShape(bool final, ShapeOp& op);

        void output(bool final);
        void outputPartial() { output(false); }
        void outputFinal() { output(true); }
        void outputStats();
		
        friend class OutputDraw;
        friend class OutputMerge;
		friend class OutputBounds;
        
        bool isDone();
        void fileIfNecessary();
        void moveFinishedToFile();
        void moveUnfinishedToTwoFiles();
        void getUnfinishedFromFile();
        AbstractSystem* system() { return m_cfdg->system(); }
        void processShape(const Shape& s);
        void executeShape(Shape& s);

    private:
        CFDGImpl*   m_cfdg;
        Canvas*     m_canvas;

        int m_maxShapes;
        bool m_tiled;
        bool m_sized;

        double mNextFramePoint;

        std::deque<FinishedShape> mFinishedShapes;
        std::multiset<Shape> mUnfinishedShapes;

        std::deque< ref_ptr<TempFile> > m_finishedFiles;
        std::deque< ref_ptr<TempFile> > m_unfinishedFiles;
        int mFinishedFileCount;
        int mUnfinishedFileCount;

        int mVariation;
        
        double mScaleArea;
        double mScale;
        double mFixedBorder;
        double mShapeBorder;
        double mTotalArea;

        double m_currScale;
        double m_minArea;
        Bounds mBounds;
        agg::trans_affine m_currTrans;
        unsigned int m_outputSoFar;

        AbstractSystem::Stats m_stats;
        int m_unfinishedInFilesCount;
};


#endif // INCLUDE_RENDERIMPL_H

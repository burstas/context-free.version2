// tiledCanvas.h
// this file is part of Context Free
// ---------------------
// Copyright (C) 2006 John Horigan - john@glyphic.com
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

#ifndef INCLUDE_TILEDCANVAS_H
#define INCLUDE_TILEDCANVAS_H

#include "agg_trans_affine.h"
#include "agg_color_rgba.h"
#include "cfdg.h"
#include "bounds.h"
#include "builder.h"
#include "shape.h"
#include <vector>

typedef std::vector<agg::point_i> tileList;

class tiledCanvas : public Canvas {
public:
	void start(bool clear, const agg::rgba& bk, int width, int height);
	void end();
	
	void circle(RGBA8 c, agg::trans_affine tr);
	void square(RGBA8 c, agg::trans_affine tr);
	void triangle(RGBA8 c, agg::trans_affine tr);
    void path(RGBA8 c, agg::trans_affine tr, agg::path_storage* path, 
              pathAttr* attr);
	
	tiledCanvas(Canvas* tile, const agg::trans_affine& tr); 
    ~tiledCanvas() {};
    
    void scale(double scaleFactor);
    
    tileList getTesselation(int width, int height, int x, int y, bool flipY = false);
    bool isRectangular(int* x_factor = 0, int* y_factor = 0);
    
private:
    enum tileType { inside = 0, simple = 1, complex = 2};
	Canvas* mTile;
	tileType tileTransform(agg::trans_affine& tr, agg::path_storage* path,
                           pathAttr* = &Builder::DefaultPathAttributes);
    void getTesselation(Bounds b);
	agg::trans_affine mOffset;
    agg::trans_affine mInvert;
    std::vector<agg::point_d> mTileList;
};

#endif  // INCLUDE_TILEDCANVAS_H

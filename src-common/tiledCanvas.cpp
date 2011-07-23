// tiledCanvas.cpp
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

#include "tiledCanvas.h"
#include <math.h>
#include "primShape.h"
#include "bounds.h"
#include <cstdlib>
#include <stdlib.h>

void tiledCanvas::start(bool clear, const agg::rgba& bk, int , int )
{
	mTile->start(clear, bk, mTile->mWidth, mTile->mHeight);
}

void tiledCanvas::end() 
{
	mTile->end();
}

void tiledCanvas::circle(RGBA8 c, agg::trans_affine tr)
{
    switch (tileTransform(tr, primShape::shapeMap[primShape::circleType])) {
        case inside:
            mTile->circle(c, tr);
            return;
        case simple:
        case complex:
            for (unsigned int i = 0; i < mTileList.size(); ++i) {
                agg::trans_affine t(tr);
                t.tx += mTileList[i].x;
                t.ty += mTileList[i].y;
                mTile->circle(c, t);
            }
            return;
    }
    return;
}

void tiledCanvas::square(RGBA8 c, agg::trans_affine tr)
{
    switch (tileTransform(tr, primShape::shapeMap[primShape::squareType])) {
        case inside:
            mTile->square(c, tr);
            return;
        case simple:
        case complex:
            for (unsigned int i = 0; i < mTileList.size(); ++i) {
                agg::trans_affine t(tr);
                t.tx += mTileList[i].x;
                t.ty += mTileList[i].y;
                mTile->square(c, t);
            }
            return;
    }
    return;
}

void tiledCanvas::triangle(RGBA8 c, agg::trans_affine tr)
{
    switch (tileTransform(tr, primShape::shapeMap[primShape::triangleType])) {
        case inside:
            mTile->triangle(c, tr);
            return;
        case simple:
        case complex:
            for (unsigned int i = 0; i < mTileList.size(); ++i) {
                agg::trans_affine t(tr);
                t.tx += mTileList[i].x;
                t.ty += mTileList[i].y;
                mTile->triangle(c, t);
            }
            return;
    }
    return;
}

void tiledCanvas::path(RGBA8 c, agg::trans_affine tr, agg::path_storage* path, 
                       pathAttr* attr)
{
    switch (tileTransform(tr, path, attr)) {
        case inside:
            mTile->path(c, tr, path, attr);
            return;
        case simple:
        case complex:
            for (unsigned int i = 0; i < mTileList.size(); ++i) {
                agg::trans_affine t(tr);
                t.tx += mTileList[i].x;
                t.ty += mTileList[i].y;
                mTile->path(c, t, path, attr);
            }
            return;
    }
    return;
}

static const double tileBuffer = 1.05;

tiledCanvas::tileType tiledCanvas::tileTransform(agg::trans_affine& tr, 
                                                 agg::path_storage* path,
                                                 pathAttr* attr)
// Adjust the translation part of the transform so that it falls within the 
// tile parallelogram at the origin. 
//
// Returns whether the shape is close to the edge of the canvas 
// (true=not close, false=close/overlapping).
{
	double dummy;
    mInvert.transform(&(tr.tx), &(tr.ty));  // transform to unit square tesselation
	tr.tx = modf(tr.tx, &dummy);            // translate to unit square at the origin
	tr.ty = modf(tr.ty, &dummy);
    if (tr.tx < 0.0) tr.tx += 1.0;
    if (tr.ty < 0.0) tr.ty += 1.0;
    mOffset.transform(&(tr.tx), &(tr.ty));  // transform back to specified tesselation
	
    Bounds b(tr, path, attr, tileBuffer);
    if (b.mMin_X > 0 && b.mMax_X < mWidth && b.mMin_Y > 0 && b.mMax_Y < mHeight)
        return inside;
    //if ((b.mMax_X - b.mMin_X) < mWidth && (b.mMax_Y - b.mMin_Y) < mHeight)
    //    return simple;
    
    getTesselation(b);
    return complex;
}

tiledCanvas::tiledCanvas(Canvas* tile, const agg::trans_affine& tr) 
:   Canvas(tile->mWidth, tile->mHeight), 
    mTile(tile), 
    mOffset(tr)
{
}

void tiledCanvas::scale(double scaleFactor)
{
    agg::trans_affine_scaling scale(scaleFactor);
    
    // Generate the tiling transform in pixel units
    mOffset *= scale;
    
    // The mInvert transform can transform coordinates from the pixel unit tiling
    // to the unit square tiling.
    mInvert = mOffset;
    mInvert.invert();
}

tileList tiledCanvas::getTesselation(int w, int h, int x, int y, bool flipY)
{
    // Produce an integer version of mOffset that is centered in the w x h screen
    agg::trans_affine tess(mWidth, floor(mOffset.shy + 0.5), floor(mOffset.shx + 0.5),
        flipY ? -mHeight : mHeight, x, y);
    agg::rect_i screen(0, 0, w - 1, h - 1);
    
    tileList tessPoints;
    tessPoints.push_back(agg::point_i(x, y));   // always include the center tile
    
    // examine rings of tile units around the center unit until you encounter a
    // ring that doesn't have any tile units that intersect the screen. Then stop.
    for (int ring = 1; ; ring++) {
        bool hit = false;
        for (int y = -ring; y <= ring; y++) {
            for (int x = -ring; x <= ring; x++) {
                // These loops enumerate all tile units on and within the ring.
                // Skip tile units that are within (not on) the ring.
                if (abs(x) < ring && abs(y) < ring) continue;
                
                // Find where this tile is on the screen
                double dx = x;
                double dy = y;
                tess.transform(&dx, &dy);
                int px = (int)floor(dx + 0.5);
                int py = (int)floor(dy + 0.5);
                
                // If the tile is visible then record it
                agg::rect_i tile(px, py, px + mWidth - 1, py + mHeight - 1);
                if (tile.overlaps(screen)) {
                    hit = true;
                    tessPoints.push_back(agg::point_i(px, py));
                }
            }
        }
        
        if (!hit) break;
    }
    
    return tessPoints;
}

void tiledCanvas::getTesselation(Bounds b) 
// use the same algorithm as getTesselation(int ...) , but purely in the 
// floating point domain, to see what tesselation points the large shape
// needs to be drawn at such that all parts of it that overlap the canvas are 
// drawn. 
{
    mTileList.clear();
    mTileList.push_back(agg::point_d(0, 0));
    agg::rect_d canvas(0, 0, (double)(mWidth - 1), (double)(mHeight - 1));
    
    for (int ring = 1; ; ring++) {
        bool hit = false;
        for (int y = -ring; y <= ring; y++) {
            for (int x = -ring; x <= ring; x++) {
                // These loops enumerate all tile units on and within the ring.
                // Skip tile units that are within (not on) the ring.
                if (abs(x) < ring && abs(y) < ring) continue;
                
                // Find where this tile is on the canvas
                double dx = x;
                double dy = y;
                mOffset.transform(&dx, &dy);
                
                // If the tile might touch the canvas then record it
                agg::rect_d shape(b.mMin_X + dx, b.mMin_Y + dy, b.mMax_X + dx, b.mMax_Y + dy);
                if (shape.overlaps(canvas)) {
                    hit = true;
                    mTileList.push_back(agg::point_d(dx, dy));
                }
            }
        }
        
        if (!hit) break;
    }
}

bool tiledCanvas::isRectangular(int* x_factor, int* y_factor)
{
    int shx = (int)floor(mOffset.shx + 0.5);
    int shy = (int)floor(mOffset.shy + 0.5);
    
    if (shx == 0 && shy == 0) {
        if (x_factor && y_factor) *x_factor = *y_factor = 1;
        return true;
    }
    
    if (shx && mWidth % abs(shx) == 0) {
        if (x_factor && y_factor) {
            *x_factor = 1;
            *y_factor = mWidth / abs(shx);
        }
        return true;
    }
    
    if (shx && mWidth % (mWidth - abs(shx)) == 0) {
        if (x_factor && y_factor) {
            *x_factor = 1;
            *y_factor = mWidth / (mWidth - abs(shx));
        }
        return true;
    }
    
    if (shy && mHeight % shy == 0) {
        if (x_factor && y_factor) {
            *x_factor = mHeight / abs(shy);
            *y_factor = 1;
        }
        return true;
    }
    
    if (shy && mHeight % (mHeight - abs(shy)) == 0) {
        if (x_factor && y_factor) {
            *x_factor = mHeight / (mHeight - abs(shy));
            *y_factor = 1;
        }
        return true;
    }
    
    return false;
}

// shape.cpp
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


#include "shape.h"
#include <stdlib.h>
#include "primShape.h"
#include "agg_bounding_rect.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_conv_stroke.h"
#include "agg_conv_curve.h"

FinishedShape::FinishedShape(const Shape& s, double cumulativeArea, 
                             agg::path_storage* path, pathAttr* attr)
:   mCumulativeArea(cumulativeArea),
    mPath(path),
    mAttributes(attr)
{
    mPrimitiveShapeType = (unsigned char)(s.mShapeType);

    m_MiniTransform[0] = (float)s.mWorldState.m_transform.sx;
    m_MiniTransform[1] = (float)s.mWorldState.m_transform.shy;
    m_MiniTransform[2] = (float)s.mWorldState.m_transform.shx;
    m_MiniTransform[3] = (float)s.mWorldState.m_transform.sy;
    m_MiniTransform[4] = (float)s.mWorldState.m_transform.tx;
    m_MiniTransform[5] = (float)s.mWorldState.m_transform.ty;

    agg::rgba c;
    s.mWorldState.m_Color.getRGBA(c);
    mColor = RGBA8(c);

    m_Z = (float)s.mWorldState.m_Z;
	
	mCumulativeArea = cumulativeArea;    
}


void FinishedShape::GetTransform(agg::trans_affine& tr) const
{
    tr.sx  = (double)m_MiniTransform[0];
    tr.shy = (double)m_MiniTransform[1];
    tr.shx = (double)m_MiniTransform[2];
    tr.sy  = (double)m_MiniTransform[3];
    tr.tx  = (double)m_MiniTransform[4];
    tr.ty  = (double)m_MiniTransform[5];
}


//-------------------------------------------------------------------------////
//-------------------------------------------------------------------------////

void Rule::addReplacement(const Replacement& r)
{
    mReplacements.push_back(r);
}

typedef agg::conv_curve<agg::path_storage>              Curved;
typedef agg::conv_stroke<Curved>                        CurvedStroked;
typedef agg::conv_transform<CurvedStroked>              CurvedStrokedTrans;
typedef agg::conv_transform<agg::path_storage>          Trans;
typedef agg::conv_curve<Trans>                          TransCurved;
typedef agg::conv_stroke<TransCurved>                   TransCurvedStroked;

static agg::path_storage    dummyPath;
static agg::trans_affine    dummyTrans;
static Trans                trans(dummyPath, dummyTrans);
static TransCurved          transCurved(trans);
static TransCurvedStroked   transCurvedStroked(transCurved);
static Curved               curved(dummyPath);
static CurvedStroked        curvedStroked(curved);
static CurvedStrokedTrans   curvedStrokedTrans(curvedStroked, dummyTrans);

static void initAverage(unsigned index, Curved& path, agg::point_d& centroid);

void pathAttr::apply(agg::path_storage* path, const agg::trans_affine& tr, double accuracy)
{
    double scale = sqrt(fabs(tr.determinant()));
    
    if (mCommand == stroke) {
        if (mIsoWidthFlag) {
            trans.attach(*path);
            trans.transformer(tr);
            transCurvedStroked.width(mStrokeWidth * scale);
            transCurvedStroked.line_join(mLineJoin);
            transCurvedStroked.line_cap(mLineCap);
            transCurvedStroked.miter_limit(mMiterLimit);
            transCurvedStroked.inner_join(agg::inner_round);
            transCurvedStroked.approximation_scale(accuracy);
            transCurved.approximation_scale(accuracy);
            
            // If the *visual* line width is considerable we 
            // turn on processing of curve cusps.
            //---------------------
            if (mStrokeWidth * scale > 1.0) {
                transCurved.angle_tolerance(0.2);
            } else {
                transCurved.angle_tolerance(0.0);
            }
        } else {
            curved.attach(*path);
            curvedStrokedTrans.transformer(tr);
            curvedStroked.width(mStrokeWidth);
            curvedStroked.line_join(mLineJoin);
            curvedStroked.line_cap(mLineCap);
            curvedStroked.miter_limit(mMiterLimit);
            curvedStroked.inner_join(agg::inner_round);
            curvedStroked.approximation_scale(accuracy * scale);
            curved.approximation_scale(accuracy * scale);
            
            // If the *visual* line width is considerable we 
            // turn on processing of curve cusps.
            //---------------------
            if (mStrokeWidth * scale > 1.0) {
                curved.angle_tolerance(0.2);
            } else {
                curved.angle_tolerance(0.0);
            }
        }
    } else {
        trans.attach(*path);
        trans.transformer(tr);
        transCurved.approximation_scale(accuracy);
    }
}

bool pathAttr::boundingRect(agg::path_storage* path, const agg::trans_affine& tr,
                            double& minx, double& miny, double& maxx, double& maxy,
                            double scale)
{
    apply(path, tr, scale * 0.1);
    
    if (mCommand == stroke) {
        if (mIsoWidthFlag) {
            return agg::bounding_rect_single(transCurvedStroked, mIndex, &minx, &miny, &maxx, &maxy);
        } else {
            return agg::bounding_rect_single(curvedStrokedTrans, mIndex, &minx, &miny, &maxx, &maxy);
        }
    } else {
        return agg::bounding_rect_single(transCurved, mIndex, &minx, &miny, &maxx, &maxy);
    }
}

template <>
void pathAttr::addPath<agg::rasterizer_scanline_aa<> >(agg::rasterizer_scanline_aa<>& ras,
                                                       agg::path_storage* path, 
                                                       const agg::trans_affine& tr)
{
    apply(path, tr, 1.0);
    
    if (mCommand == stroke) {
        if (mIsoWidthFlag) {
            ras.add_path(transCurvedStroked, mIndex);
        } else {
            ras.add_path(curvedStrokedTrans, mIndex);
        }
    } else {
        ras.add_path(transCurved, mIndex);
    }
}

pathAttr::pathAttr(unsigned index, agg::path_storage* path)
:   mIndex(index), mNext(0), mCommand(fill), mCount(0), mEvenOddFlag(false),
    mIsoWidthFlag(false), mLineJoin(agg::miter_join), mLineCap(agg::butt_cap), 
    mMiterLimit(4.0), mStrokeWidth(0.1), mCentroid(0.0, 0.0), mArea(0.0)
{
    Curved curve(*path);
    bool first = true;
    agg::point_d p0, plast;
    p0.x = p0.y = plast.x = plast.y = 0.0;
    
    curve.approximation_scale(0.05);
    curve.rewind(index);
    unsigned cmd;
    double x = 0.0, y = 0.0, area = 0.0, centx = 0.0, centy = 0.0;
    while(!agg::is_stop(cmd = curve.vertex(&x, &y))) {
        if(agg::is_vertex(cmd)) {
            if(first) {
                first = false;
                p0.x = plast.x = x;
                p0.y = plast.y = y;
                area = centx = centy = 0.0;
            } else {
                double term = y * plast.x - x * plast.y;
                area += 0.5 * term;
                centx += (x + plast.x) * term;
                centy += (y + plast.y) * term;
                plast.x = x; plast.y = y;
            }
        } else if (agg::is_end_poly(cmd)) {
            first = true;
            double term = p0.y * plast.x - p0.x * plast.y;
            area += 0.5 * term;
            centx += (p0.x + plast.x) * term;
            centy += (p0.y + plast.y) * term;
            if (fabs(area) < 0.001) {
                initAverage(index, curve, mCentroid);
                return;
            }
            centx /= 6.0 * area;
            centy /= 6.0 * area;
            mArea += area;
            mCentroid.x += area * centx;
            mCentroid.y += area * centy;
        }
    }
    mArea = fabs(mArea);
    if (mArea < 0.001) {
        mArea = 1.0;
        initAverage(index, curve, mCentroid);
        return;
    }
    mCentroid.x /= mArea;
    mCentroid.y /= mArea;
}

static void initAverage(unsigned index, Curved& path, agg::point_d& centroid)
{
    centroid.x = centroid.y = 0.0;
    int c = 0;
    path.rewind(index);
    unsigned cmd;
    double x, y;
    while(!agg::is_stop(cmd = path.vertex(&x, &y))) {
        if(agg::is_vertex(cmd)) {
            centroid.x += x;
            centroid.y += y;
            ++c;
        }
    }
    if (c) {
        centroid.x /= (double)c;
        centroid.y /= (double)c;
    }
}

Shape& Shape::operator*=(const Replacement& r) {
    mShapeType = r.mShapeType;
    mWorldState *= r.mChildChange;
    mAreaCache = mWorldState.area();
    return *this;
}


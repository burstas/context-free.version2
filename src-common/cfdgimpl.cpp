// cfdgimpl.cpp
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


#include "cfdgimpl.h"
#include "renderimpl.h"
#include "primShape.h"
#include <assert.h>
#include <algorithm>

#ifdef _WIN32
#pragma warning( disable : 4800 4189 )
#endif

using namespace std;


CFDGImpl::CFDGImpl(AbstractSystem* m) 
: mBackgroundSet(false), m_system(m), m_Parameters(0), mTiled(false), 
  mSized(false), mTileOffset(0, 0)
{ 
    // These have to be encoded first so that their type number will fit
    // within an unsigned char
    string circle_name = "CIRCLE";
    string square_name = "SQUARE";
    string triangle_name = "TRIANGLE";
    string loopstart_name = "~~LOOPSTART~~";
    string loopend_name = "~~LOOPEND~~";
    
    // ensure these four get the first four shape numbers
    int circle_num = encodeShapeName(circle_name);
    int square_num = encodeShapeName(square_name);
    int triangle_num = encodeShapeName(triangle_name);
    int loopstart_num = encodeShapeName(loopstart_name);
    int loopend_num = encodeShapeName(loopend_name);

    // Make sure that the shape numbers correspond to the order of primShape::shapeMap[]
    assert(circle_num == primShape::circleType);
    assert(square_num == primShape::squareType);
    assert(triangle_num == primShape::triangleType);
    assert(loopstart_num == primShape::loopStartType);
    assert(loopend_num == primShape::loopEndType);
}
CFDGImpl::~CFDGImpl()                               { }

void
CFDGImpl::setInitialShape(const Shape& s)
{ 
    // set the startshape if it hasn't already been set
    if (m_initialShape.mShapeType == -1)
        m_initialShape = s; 
}

const Shape&
CFDGImpl::getInitialShape()
{ 
    m_initialShape.mWorldState.m_transform.tx = mTileOffset.x;
    m_initialShape.mWorldState.m_transform.ty = mTileOffset.y;
    return m_initialShape; 
}

namespace {
    // We need a light-weight version of the Rule class to give to the lower_bound
    // template function. Using a Rule object incurs the cost of allocating a
    // Replacements vector that is never used.
    struct RuleSpecifier
    {
        int mInitialShapeType;
        double mWeight;
        RuleSpecifier(int type, double weight) : mInitialShapeType(type), mWeight(weight) {};
    };
    inline bool operator<(const Rule& l, const RuleSpecifier& r) { 
        return (l.mInitialShapeType == r.mInitialShapeType) ? 
        (l.mWeight < r.mWeight) : 
        (l.mInitialShapeType < r.mInitialShapeType); 
    }
    // This is a pointless comparator that is needed to be specified for the debug
    // version of STL under VC2005. 
    inline bool operator<(const RuleSpecifier& l, const Rule& r) { 
        return (l.mInitialShapeType == r.mInitialShapeType) ? 
        (l.mWeight < r.mWeight) : 
        (l.mInitialShapeType < r.mInitialShapeType); 
    }
}

Rule *
CFDGImpl::findRule(int shapetype, double r)
{
    RuleSpecifier temp(shapetype, r);
    
    // Technically it is illegal for temp to not be a Rule, but we seem to be able
    // to get away with it. 
    vector<Rule>::iterator first = lower_bound(mRules.begin(), mRules.end(), temp);
    return &*first;
}

void
CFDGImpl::addRule(const Rule& r, bool isShape)
{
    mRules.push_back(r);
    if (r.mInitialShapeType < int(m_shapeTypes.size()))
        m_shapeTypes[r.mInitialShapeType].hasRules = true;
    m_shapeTypes[r.mInitialShapeType].shapeType = isShape ? pathType : ruleType;
}

void 
CFDGImpl::addParameter(Parameter p)
{
    m_Parameters |= p;
    usesColor = m_Parameters & Color;
    usesAlpha = m_Parameters & Alpha;
}

void 
CFDGImpl::setBackgroundColor(const agg::rgba& bk) 
{
    if (!mBackgroundSet) {
        backgroundColor = bk;
        mBackgroundSet = true;
    }
}

bool
CFDGImpl::isTiled(agg::trans_affine* tr, double* x, double* y) const
{
    if (!mTiled) return false;
    if (tr) *tr = mTileTransform;
    if (x) *x = mTileX;
    if (y) *y = mTileY;
    return true;
}

bool
CFDGImpl::isSized(double* x, double* y) const
{
    if (!mSized) return false;
    if (x) *x = mTileX;
    if (y) *y = mTileY;
    return true;
}

void
CFDGImpl::setTiled(const agg::trans_affine& tr, const double& x, const double& y)
{
    if (mTiled) return;     // only accept the first tile directive
    mTileTransform = tr;
    mTileOffset.x = tr.tx; mTileTransform.tx = 0;
    mTileOffset.y = tr.ty; mTileTransform.ty = 0;
    mTileX = x;
    mTileY = y;
    mTiled = true;
}

void
CFDGImpl::setSized(const agg::trans_affine& tr)
{
    mTileTransform = tr;
    mTileOffset.x = tr.tx; mTileTransform.tx = 0;
    mTileOffset.y = tr.ty; mTileTransform.ty = 0;
    mTileX = tr.sx;
    mTileY = tr.sy;
    mSized = true;
}

void
CFDGImpl::rulesLoaded()
{
    // thanks to by Brent Yorgey
    vector<double> weightsums( m_shapeTypes.size(), 0.0 );
    vector<double> unitweightsums( m_shapeTypes.size(), 0.0 );
    vector<int> rulecounts( m_shapeTypes.size(), 0 );
    unsigned int i;

    // first pass: sum all the weights for each shape type
    for ( i = 0; i < mRules.size(); i++ ) {
        Rule& r = mRules[i];
        weightsums[ r.mInitialShapeType ] += r.mWeight;
        rulecounts[ r.mInitialShapeType ]++;
    }

    // second pass: normalize each weight by dividing by the
    // total weight for that shape type
    for ( i = 0; i < mRules.size(); i++ ) {
        Rule& r = mRules[i];
        double weight = r.mWeight / weightsums[ r.mInitialShapeType ];
        unitweightsums[ r.mInitialShapeType ] += weight;
        if (--rulecounts[ r.mInitialShapeType ]) {
            r.mWeight = unitweightsums[ r.mInitialShapeType ];
        } else {
            // make sure that last rule of a type has a weightsum > 1.0
            r.mWeight = 1.1;
        }
    }
    
    // third pass: sort the rules by shape type, preserving the rule order
    // with respect to rules of the same shape type
    sort(mRules.begin(), mRules.end());
}

int
CFDGImpl::numRules()
{
    return mRules.size();
}

string
CFDGImpl::decodeShapeName(int shapetype)
{
    if (shapetype < int(m_shapeTypes.size()))
        return m_shapeTypes[shapetype].name;
    else
        return ("**unnamed shape**");
}

int
CFDGImpl::encodeShapeName(const string& s)
{
    for (unsigned int i = 0; i < m_shapeTypes.size(); i++) {
        if (s == m_shapeTypes[i].name) {
            return i;
        }
    }

    m_shapeTypes.push_back(ShapeType(s));
    return m_shapeTypes.size() - 1;
}

int
CFDGImpl::getShapeType(const string& s)
{
    for (unsigned int i = 0; i < m_shapeTypes.size(); i++) {
        if (s == m_shapeTypes[i].name) {
            return m_shapeTypes[i].shapeType;
        }
    }
    
    return newShape;
}

int
CFDGImpl::getShapeType(int shapetype)
{
    return m_shapeTypes[shapetype].shapeType;
}

bool
CFDGImpl::shapeHasRules(int shapetype)
{
    if (shapetype < int(m_shapeTypes.size()))
        return m_shapeTypes[shapetype].hasRules;
    else
        return false;
}


Renderer*
CFDGImpl::renderer(int width, int height, float minSize,
                    int variation, double border)
{
    return new RendererImpl(this, width, height, minSize, variation, border);
}


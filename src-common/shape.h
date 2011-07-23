// shape.h
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


#ifndef INCLUDE_SHAPE_H
#define INCLUDE_SHAPE_H

#include <vector>
#include <string>
#include <iostream>

#include "agg_math_stroke.h"
#include "agg_trans_affine.h"
#include "agg_color_rgba.h"
#include "HSBColor.h"
#include "cfdg.h"
#include "Rand48.h"
#include "agg_path_storage.h"

class Replacement;
struct pathAttr;

// Contains all of the information about a change to a shape 
class Modification {
    public: 
        agg::trans_affine m_transform;
        double m_Z;
        double m_SizeZ;
        
        HSBColor m_Color;
        HSBColor m_ColorTarget;

        Rand48 mRand48Seed;

        Modification() : m_Z(0.0), m_SizeZ(1.0)
        { }
        
        double area() const { return fabs(m_transform.determinant()); }
        
        Modification operator*(const Modification& m) const
        {
            Modification n = *this;
            n *= m;
            return n;
        }
        Modification& operator*=(const Modification& m)
        {
            m_transform.premultiply(m.m_transform);
            
            m_Z += m.m_Z * m_SizeZ;
            m_SizeZ *= m.m_SizeZ;
            
            
            m_Color.adjustWith(m.m_Color, m_ColorTarget);
            m_ColorTarget.adjustWith(m.m_ColorTarget, m_ColorTarget);
            
            mRand48Seed ^= m.mRand48Seed;
            return *this;
        }
};

// Contains all of the information about a shape that is used during parsing
// and executing a cfdg file
class Shape {
    public: 
        int mShapeType;
        Modification mWorldState;
        double mAreaCache;

        Shape() : mShapeType(-1) { mAreaCache = mWorldState.area(); }

        Shape operator*(const Modification& m) const {
            Shape s = *this;
            s.mWorldState *= m;
            s.mAreaCache = s.mWorldState.area();
            return s;
        }
        
        
        Shape& operator*=(const Replacement& r);
        
        Shape& operator*=(const Modification& m) {
            mWorldState *= m;
            mAreaCache = mWorldState.area();
            return *this;
        }

        double area() const { return mAreaCache; }
        bool operator<(const Shape& b) const { return mAreaCache < b.mAreaCache; }

        void write(std::ostream& os) const { os.write((char*)(this), sizeof(Shape)); }
        void read(std::istream& is) { is.read((char *)(this), sizeof(Shape)); }
};

// Contains all of the information about a shape replacement that is used during parsing
// and executing a cfdg file
class Replacement {
    public: 
        int mShapeType;
        Modification mChildChange;
        int mLoopCount;

        Replacement() : mShapeType(-1), mLoopCount(0) {}
};

// Contains the subset of class Shape that is needed for drawing squares,
// circles, and triangles onto a canvas
class FinishedShape {
    public: 
        unsigned char mPrimitiveShapeType;
        float m_MiniTransform[6];
        float m_Z;
        RGBA8 mColor;
		double mCumulativeArea;
        agg::path_storage* mPath;
        pathAttr* mAttributes;
		
        FinishedShape() : mPath(0) {};
        FinishedShape(const Shape&, double, agg::path_storage* mPath, 
                      pathAttr* attr);

        void GetTransform(agg::trans_affine& tr) const;

        bool operator<(const FinishedShape& b) const { 
            return m_Z == b.m_Z ? mCumulativeArea < b.mCumulativeArea : m_Z < b.m_Z; 
        };
 
        void write(std::ostream& os) const { os.write((char*)(this), sizeof(FinishedShape)); }
        void read(std::istream& is) { is.read((char *)(this), sizeof(FinishedShape)); }
};

inline std::ostream& operator<<(std::ostream& os, const FinishedShape& fs) { fs.write(os); return os; }
inline std::istream& operator>>(std::istream& is, FinishedShape& fs) { fs.read(is); return is; }
inline std::ostream& operator<<(std::ostream& os, const Shape& s) { s.write(os); return os; }
inline std::istream& operator>>(std::istream& is, Shape& s) { s.read(is); return is; }

class Rule {
    public:
        int mInitialShapeType;
        std::vector<Replacement> mReplacements;
        double mWeight;
        agg::path_storage* mPath;
        pathAttr* mAttributes;
        Rule() : mPath(0), mAttributes(0) {};
    public:
        void addReplacement(const Replacement& r);
        bool operator<(const Rule& r) const { 
            return (mInitialShapeType == r.mInitialShapeType) ? 
                (mWeight < r.mWeight) : 
                (mInitialShapeType < r.mInitialShapeType); 
        }
};

struct pathAttr
{
    enum Command {stroke, fill, loopStart, loopEnd };
    
    unsigned            mIndex;
    pathAttr*           mNext;
    Command             mCommand;
    int                 mCount;
    bool                mEvenOddFlag;
    bool                mIsoWidthFlag;
    agg::line_join_e    mLineJoin;
    agg::line_cap_e     mLineCap;
    double              mMiterLimit;
    double              mStrokeWidth;
    Modification        mWorldState;
    agg::point_d        mCentroid;
    double              mArea;
    
    // Empty constructor
    pathAttr() :
    mIndex(0),
    mNext(0),
    mCommand(fill),
    mCount(0),
    mEvenOddFlag(false),
    mIsoWidthFlag(false),
    mLineJoin(agg::miter_join),
    mLineCap(agg::butt_cap),
    mMiterLimit(4.0),
    mStrokeWidth(0.1),
    mCentroid(0.0, 0.0),
    mArea(1.0)
    {
    }
        
    pathAttr(unsigned index, agg::path_storage* path);
    
    bool boundingRect(agg::path_storage* path, const agg::trans_affine& tr,
                      double& minx, double& miny, double& maxx, double& maxy,
                      double scale);
    
    template <class Rasterizer>
    void addPath(Rasterizer& ras, agg::path_storage* path, 
                 const agg::trans_affine& tr);
    
private:    
    void apply(agg::path_storage* path, const agg::trans_affine& tr, double accuracy);
};


const double MY_PI =  3.14159265358979323846;

#endif // INCLUDE_SHAPE_H

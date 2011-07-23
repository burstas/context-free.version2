// builder.h
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


#ifndef INCLUDE_BUILDER_H
#define INCLUDE_BUILDER_H

#ifdef _WIN32
#pragma warning( disable : 4786 )
#endif

#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include "agg_basics.h"
#include "Rand48.h"

#include "shape.h"

//#define DEBUG_ENTROPY

class CFDGImpl;

class Builder {
public:
    enum LoopType { noLoop, unknownLoop, opLoop, cmdLoop };
    CFDGImpl*       m_CFDG;
    std::queue<std::string>   m_filesToLoad;
    std::string          m_currentPath;
    
    std::istream*        m_input;
    
    int             mShapeType;
    Modification    mCurrentModification;
    std::stack<Modification>    mSavedModifications;
    std::stack<unsigned> mSavedPathIndices;
    double          mOrientation;
    double          mReflection;
    bool            mReflected;
    double          mSizeX;
    double          mSizeY;
    double          mSizeZ;
    double          mSkewX;
    double          mSkewY;
    char*           mParameters;
    double          mXLocation;
    double          mYLocation;
    double          mZLocation;
    int             mMask;
    Rule            m_currRule;
    agg::point_d    mPathPoints[5];
    agg::point_d    mLastPoint;
    bool            mStop;
    bool            mClosed;
    bool            mWantMoveTo;
    unsigned        mIndex;
    unsigned        mNextIndex;
    double          mStrokeWidth;
    Rand48          mSeed;
    
    void addEntropy(const char* txt, bool isNumber = false);
    void negEntropy();
    static unsigned ModSeedIndex;
    static bool WasNumber;
    static Rand48 SavedSeed;
    static char SavedText[32];
    static unsigned SavedSeedIndex;
#ifdef DEBUG_ENTROPY
    std::string mEntropy;
    size_t mNumberPos;
#endif

    static pathAttr DefaultPathAttributes;
    
    void LoopStart();
    void LoopEnd(int loopCount);
    void PathLoopStart();
    const char* PathLoopEnd(int loopCount);
    LoopType        mLoopState;
    int             mPendingLoops;
    unsigned        mLoopDepth;
    
    
    Builder(CFDGImpl* cfdg, int variation)
    : m_CFDG(cfdg), m_input(0), mOrientation(0.0), mReflection(0.0), 
    mReflected(false), mSizeX(1.0), mSizeY(1.0), mSizeZ(1.0), 
    mSkewX(0.0), mSkewY(0.0), mParameters(0), mXLocation(0.0), 
    mYLocation(0.0), mZLocation(0.0), mMask(0), mStrokeWidth(0.1), 
#ifdef DEBUG_ENTROPY
    mNumberPos(std::string::npos),
#endif
    mLoopState(noLoop), mPendingLoops(0), mLoopDepth(0)
    { 
        mPathPoints[0].x = mPathPoints[0].y = 
        mPathPoints[1].x = mPathPoints[1].y = 
        mPathPoints[2].x = mPathPoints[2].y = 
        mPathPoints[3].x = mPathPoints[3].y = 
        mPathPoints[4].x = mPathPoints[4].y = 0.0;
        mLastPoint.x = mLastPoint.y = 0.0;
        mSeed.seed(0x330E, (unsigned short)((variation >> 16) & 0xffff), 
                           (unsigned short)(variation & 0xffff)); 
    }
    ~Builder();
    
    void            Orientation(double);
    void            Reflection(double);
    void            Size(double, double, double);
    void            Skew(double, double);
    void            Parameters(char*);
    void            XLocation(double);
    void            YLocation(double);
    void            ZLocation(double);
    void            PointX(double, int);
    void            PointY(double, int);
    const char*     pathOp(const char*);
    const char*     pathCmd(const char*);
    void            StrokeWidth(double);
    
    void            buildDefaultTransform();
    const char*     buildTileTransform();
    const char*     buildSizeTransform();
    void            clearModification();
    void            startPath();
    void            finishPath(bool);
private:
    enum OpType { moveto = 0, moverel = 1, lineto = 2, linerel = 3, 
                  arcto = 4, arcrel = 5, curveto = 6, curverel = 7, 
                  closepoly = 8 };
    void addAttribute(pathAttr* newAttr);
    void unwindLoops(LoopType t, bool gotMoveTo);
    void CmdLoopStart();
    void OpLoopStart(bool gotMoveTo);
    void CmdLoopEnd(int loopCount);
    const char* OpLoopEnd(int loopCount);
};

#endif // INCLUDE_BUILDER_H

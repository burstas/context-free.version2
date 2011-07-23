// builder.cpp
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


#include "builder.h"

#include "agg_trans_affine.h"
#include "agg_path_storage.h"
#include "agg_basics.h"
#include "cfdgimpl.h"
#include "yglue.h"
#include "primShape.h"
#include <string.h>

#ifdef WIN32
#include "rand48.h"
#endif

unsigned Builder::ModSeedIndex = 0;
pathAttr Builder::DefaultPathAttributes;
bool Builder::WasNumber = false;
Rand48 Builder::SavedSeed;
char Builder::SavedText[32];
unsigned Builder::SavedSeedIndex;


Builder::~Builder()
{
    delete m_CFDG;
    delete m_input;
}


void
Builder::buildDefaultTransform()
{
    agg::trans_affine_translation tr(mXLocation, mYLocation);
    agg::trans_affine_scaling sc(mSizeX, mSizeY);
    agg::trans_affine_skewing sk(mSkewX * MY_PI / 180.0, mSkewY * MY_PI / 180.0);
    agg::trans_affine_rotation rot(mOrientation * MY_PI / 180.0);

    mCurrentModification.m_transform.reset();

    if (mReflected) {
        agg::trans_affine_reflection ref(mReflection * MY_PI / 180.0);
        mCurrentModification.m_transform *= ref;
    }

    mCurrentModification.m_transform *= sk;
    mCurrentModification.m_transform *= sc;
    mCurrentModification.m_transform *= rot;
    mCurrentModification.m_transform *= tr;

    mCurrentModification.m_Z = mZLocation;
    mCurrentModification.m_SizeZ = mSizeZ;
}

const char*
Builder::buildTileTransform()
{
    if (mSizeX > 0 && mSizeY > 0) {
        double o_x = 0.0;
        double o_y = 0.0;
        double u_x = 1.0;
        double u_y = 0.0;
        double v_x = 0.0;
        double v_y = 1.0;
        
        mCurrentModification.m_transform.transform(&o_x, &o_y);
        mCurrentModification.m_transform.transform(&u_x, &u_y);
        mCurrentModification.m_transform.transform(&v_x, &v_y);
        
        if (fabs(u_y - o_y) >= 0.0001 && fabs(v_x - o_x) >= 0.0001) 
            return "Tile must be aligned with the X or Y axis.";
        if ((u_x - o_x) < 0.0 || (v_y - o_y) < 0.0)
            return "Tile must be in the positive X/Y quadrant.";
        
        m_CFDG->setTiled(mCurrentModification.m_transform, u_x - o_x, v_y - o_y);
    }
    
    return 0;
}

const char*
Builder::buildSizeTransform()
{
    if (mCurrentModification.m_transform.shx != 0.0 || 
        mCurrentModification.m_transform.shy != 0.0)
        return "Only size transforms may be used in the size directive.";
    
    m_CFDG->setSized(mCurrentModification.m_transform);
    return 0;
}

void
Builder::LoopStart()
{
    Replacement r;
    r.mShapeType = primShape::loopStartType;
    r.mLoopCount = 0;
    m_currRule.addReplacement(r);
}

void 
Builder::LoopEnd(int loopCount)
{
    Replacement r;
    r.mShapeType = primShape::loopEndType;
    r.mChildChange = mSavedModifications.top();
    r.mLoopCount = loopCount;
    m_currRule.addReplacement(r);
    mSavedModifications.pop();
}

// Path op loops fully expanded out in the path storage object. For example, if 
// a path op loop contains two path ops and a loop count of seven then this will 
// expand to 14 path ops in the path storeage.
void
Builder::OpLoopStart(bool gotMoveTo)
{
    mWantMoveTo = mWantMoveTo && !gotMoveTo;
    
    // If an implicit Moveto is needed then insert it before starting the loop
    if (mWantMoveTo) {
        mWantMoveTo = false;
        m_currRule.mPath->move_to(mLastPoint.x, mLastPoint.y);
    }
    
    // Save the path index of the first path op in the loop
    mSavedPathIndices.push(m_currRule.mPath->total_vertices());
}

// This function takes a subsection of a path and copies it to the end of the
// path, transformed by the transform tr
static void 
addPath(unsigned startAt, unsigned endAt,
        agg::path_storage* path, const agg::trans_affine& tr)
{
    for (unsigned i = startAt; i < endAt; ++i) {
        double x, y;
        unsigned cmd;
        cmd = path->vertex(i, &x, &y);
        if (agg::is_vertex(cmd & agg::path_cmd_mask)) tr.transform(&x, &y);
        path->vertices().add_vertex(x, y, cmd);
    }
}

const char*
Builder::OpLoopEnd(int loopCount)
{
    static const HSBColor defColor;
    
    if (mSavedModifications.top().m_Color != defColor || 
        mSavedModifications.top().m_ColorTarget != defColor) 
    {
        return "Color changes are not allowed in path operation loops";
    }
    
    // Take the subsection of the path from the start of the loop to the end of
    // the path and duplicate it loopCount - 1 times. Each copy is tranformed
    // by the loop transform with respect to the previous copy.
    unsigned lastIndex = mSavedPathIndices.top();
    mSavedPathIndices.pop();
    unsigned end = m_currRule.mPath->total_vertices();
    agg::trans_affine tr;
    for (int i = 1; i < loopCount; ++i) {
        tr.premultiply(mSavedModifications.top().m_transform);
        addPath(lastIndex, end, m_currRule.mPath, tr);
    }
    mSavedModifications.pop();
    return 0;
}

// Command loop starts and ends are inserted into the path attribute list as
// instructions that are executed when the path is drawn by the renderer.
// The reason that path command loops are stored as instructions instead of 
// expanded out as path op loops are is because path command loops can include
// color transforms and color transform are not associative the way that affine
// transforms are. What this means is that given two color transforms one can
// apply one and then the other to a color. But one cannot take the two color 
// transforms and compute a single color transform that has the same result
// as the original two color transforms. So the loop transform of path command
// loops must be done at render time because the loop transform can only be
// applied to a real world state. Loops in rules are implemented in exactly the
// same way.
void
Builder::CmdLoopStart()
{
    pathAttr* loop = new pathAttr();
    loop->mIndex = mIndex;
    loop->mCommand = pathAttr::loopStart;
    addAttribute(loop);
}

void
Builder::CmdLoopEnd(int loopCount)
{
    pathAttr* loop = new pathAttr();
    loop->mIndex = mIndex;
    loop->mCommand = pathAttr::loopEnd;
    loop->mCount = loopCount;
    loop->mWorldState = mSavedModifications.top();
    addAttribute(loop);
    mSavedModifications.pop();
}

// Entry point for start of path op and path command loops
void
Builder::PathLoopStart()
{
    // All loop starts are deferred until an actual path op or path command is
    // parsed. This is done for two reasons: initially the parser does not know
    // if a loop is a path op loop or a path command loop, and for path op loops
    // we want to know if the first op in the loop is a MOVETO so that we know to
    // suppress an implicit MOVETO
    if (mLoopState == noLoop)
        mLoopState = unknownLoop;
    ++mPendingLoops;
    ++mLoopDepth;
}

// Entry point for end of path op and path command loops
const char*
Builder::PathLoopEnd(int count)
{
    const char* ret = 0;

    if (mPendingLoops) {
        // If there are any pending loop starts then just swallow one of them
        --mPendingLoops;
        mSavedModifications.pop();
    } else {
        switch (mLoopState) {
            case noLoop:
            case unknownLoop:
                ret = "Error parsing path loop";
                break;
            case opLoop:
                ret = OpLoopEnd(count);
                break;
            case cmdLoop:
                CmdLoopEnd(count);
                break;
        }
    }
    
    --mLoopDepth;
    if (mLoopDepth == 0) mLoopState = noLoop;
    return ret;
}

void
Builder::unwindLoops(LoopType t, bool gotMoveTo)
{
    // When a path op or path command is received and there are pending loop 
    // starts then we need to instantiate the loops before doing the path op
    // or path command
    if (mLoopState == unknownLoop) 
        mLoopState = t;
    
    for (; mPendingLoops; --mPendingLoops) {
        if (t == opLoop) OpLoopStart(gotMoveTo);
        else CmdLoopStart();
    }
}

void 
Builder::Orientation(double a)
{
    agg::trans_affine_rotation rot(a * MY_PI / 180.0);
    mCurrentModification.m_transform.premultiply(rot);

    mOrientation = a;
    addEntropy("Orientation");
}

void 
Builder::Reflection(double a)
{
    agg::trans_affine_reflection ref(a * MY_PI / 180.0);
    mCurrentModification.m_transform.premultiply(ref);
    
    mReflection = a;
    mReflected = true;
    addEntropy("Reflection");
}

void 
Builder::Size(double sx, double sy, double sz)
{
    agg::trans_affine_scaling sc(sx, sy);
    mCurrentModification.m_transform.premultiply(sc);

    mCurrentModification.m_SizeZ *= sz;

    mSizeX = sx; mSizeY = sy; mSizeZ = sz;
    addEntropy("Size");
}

void 
Builder::Skew(double sx, double sy)
{
    agg::trans_affine_skewing sk(sx * MY_PI / 180.0, sy * MY_PI / 180.0);
    mCurrentModification.m_transform.premultiply(sk);

    mSkewX = sx; mSkewY = sy;
    addEntropy("Skew");
}

void 
Builder::Parameters(char* p)
{
    if (mParameters) yg_FreeString(mParameters);
    mParameters = p;
    addEntropy(p);
    addEntropy("Parameters");
}

void 
Builder::XLocation(double x)
{
    agg::trans_affine_translation tr(x, 0.0);
    mCurrentModification.m_transform.premultiply(tr);

    mXLocation = x;
    addEntropy("XLocation");
}

void 
Builder::YLocation(double y)
{
    agg::trans_affine_translation tr(0.0, y);
    mCurrentModification.m_transform.premultiply(tr);

    mYLocation = y;
    addEntropy("YLocation");
}

void 
Builder::ZLocation(double z)
{
    mCurrentModification.m_Z += mCurrentModification.m_SizeZ * z;

    mZLocation = z;
    addEntropy("ZLocation");
}

void
Builder::PointX(double x, int index)
{
    mMask |= (1 << index);
    mPathPoints[index].x = x;
}

void
Builder::PointY(double y, int index)
{
    mMask |= (1 << index);
    mPathPoints[index].y = y;
}

void 
Builder::clearModification()
{
    // Restore builder current shape info to initial state for next replacement
    mCurrentModification = Modification();
    mOrientation = mXLocation = mYLocation = mZLocation = 0.0;
    mReflected = false;
    mSizeX = mSizeY = mSizeZ = 1.0;
    mSkewX = mSkewY = 0.0;
    if (mParameters) yg_FreeString(mParameters);
    mParameters = 0;
    ModSeedIndex = 0;
    mMask = 0;
    mPathPoints[0].x = mPathPoints[0].y = 
    mPathPoints[1].x = mPathPoints[1].y = 
    mPathPoints[2].x = mPathPoints[2].y = 
    mPathPoints[4].x = mPathPoints[4].y = 0.0;
    mPathPoints[3].x = mPathPoints[3].y = 1.0;
    mStrokeWidth = 0.1;
#ifdef DEBUG_ENTROPY
    fprintf(stderr, "%s\n", mEntropy.c_str());
    mEntropy.clear();
    mNumberPos = std::string::npos;
#endif
}

void
Builder::addEntropy(const char* txt, bool isNumber)
// Randomizes the rand48 seed using text from the cfdg file
{
    if (isNumber) {
#ifdef DEBUG_ENTROPY
        mNumberPos = mEntropy.length();
#endif
        SavedSeedIndex = ModSeedIndex;
        SavedSeed = mCurrentModification.mRand48Seed;
        strncpy(SavedText + 1, txt, 30);
        SavedText[31] = '\0';
#ifdef DEBUG_ENTROPY
    } else {
        mNumberPos = std::string::npos;
#endif
    }
#ifdef DEBUG_ENTROPY
    mEntropy.append(txt);
#endif
    WasNumber = isNumber;
    unsigned len = strlen(txt);
    for (unsigned i = 0; i < len; i++) {
        mCurrentModification.mRand48Seed.xorChar(txt[i], ModSeedIndex);
        mCurrentModification.mRand48Seed.bump();
        ModSeedIndex = (ModSeedIndex + 1) % 6;
    }
}

void
Builder::negEntropy()
{
#ifdef DEBUG_ENTROPY
    if (mNumberPos != std::string::npos) {
        mEntropy.erase(mNumberPos);
        mNumberPos = std::string::npos;
    }
#endif
    if (WasNumber) {
        mCurrentModification.mRand48Seed = SavedSeed;
        ModSeedIndex = SavedSeedIndex;
        SavedText[0] = '-';
        addEntropy(SavedText, false);
    } else {
        addEntropy("neg", false);
    }
}
    

const char*
Builder::pathOp(const char* opName)
{
    if (mLoopState == cmdLoop)
        return "Cannot place path operations inside of path command loops.";

    // Figure out which path operation opName is
    const char* ops = "MOVETO    |MOVEREL   |LINETO    |LINEREL   |ARCTO     |ARCREL    |CURVETO   |CURVEREL  |CLOSEPOLY ";
    const char* opPos = strstr(ops, opName);
    if (opPos == 0 || (opPos - ops) % 11 != 0 || opPos[strlen(opName)] != ' ')
        return "Unknown path operation";
    int opIndex = (opPos - ops) / 10;
    
    // Before inserting a new pathOp, instantiate any pending path loops
    unwindLoops(opLoop, opIndex < lineto);
    
    // Process the parameters for ARCTO/ARCREL
    bool largeArc = false, sweep = false;
    double radius_x = 0.0, radius_y = 0.0, angle = 0.0;
    if (opIndex == arcto || opIndex == arcrel) {
        largeArc = mParameters && strstr(mParameters, "large");
        sweep = !(mParameters && strstr(mParameters, "cw"));
        if (mMask & 8) {
            // If the radii are specified then use the ellipse ARCxx form
            radius_x = mPathPoints[3].x;
            radius_y = mPathPoints[3].y;
            angle = mPathPoints[4].x * 0.0174532925199;
        } else {
            // Otherwise use the circle ARCxx form
            if (mMask & 16)
                radius_x = radius_y = mPathPoints[4].x;
            else
                radius_x = radius_y = 1.0;
            angle = 0.0;
        }
        if (radius_x < 0.0 || radius_y < 0.0) {
            radius_x = fabs(radius_x);
            radius_y = fabs(radius_y);
            sweep = !sweep;
        }
    }
    
    // If this is the first path operation following a path command then set the 
    // path index used by subsequent path commands to the path sequence that the
    // current path operation is part of. 
    // If this is not the first path operation following a path command then this
    // line does nothing.
    mIndex = mNextIndex;
    
    // If the op is anything other than a CLOSEPOLY then we are opening up a 
    // new path sequence.
    mClosed = false;
    mStop = false;
    
    if (opIndex == closepoly) {
        bool align = mParameters && strstr(mParameters, "align");
        if (m_currRule.mPath->total_vertices() > 1 && 
            agg::is_drawing(m_currRule.mPath->vertices().last_command()))
        {
            // Find the MOVETO/MOVEREL that is the start of the current path sequence
            // and reset LastPoint to that.
            unsigned last = m_currRule.mPath->total_vertices() - 1;
            unsigned cmd = agg::path_cmd_stop;
            for (unsigned i = last - 1; 
                 i < last && agg::is_vertex(cmd = m_currRule.mPath->command(i)); 
                 --i) 
            {
                if (agg::is_move_to(cmd)) {
                    m_currRule.mPath->vertex(i, &mLastPoint.x, &mLastPoint.y);
                    break;
                }
            }
            
            if (!agg::is_move_to(cmd))
                return "CLOSEPOLY: Unable to find a MOVETO/MOVEREL for start of path.";
            
            // If this is an aliging CLOSEPOLY then change the last vertex to
            // exactly match the first vertex in the path sequence
            if (align) {
                m_currRule.mPath->modify_vertex(last, mLastPoint.x, mLastPoint.y);
            }
        } else if (align) {
            return "Nothing to align to.";
        }
        m_currRule.mPath->close_polygon();
        mClosed = true;
        mWantMoveTo = true;
        return 0;
    }
    
    // Insert an implicit MOVETO unless the pathOp is a MOVETO/MOVEREL
    if (mWantMoveTo && opIndex > moverel) {
        mWantMoveTo = false;
        m_currRule.mPath->move_to(mLastPoint.x, mLastPoint.y);
    }
    
    switch (opIndex) {
        case moveto:
            m_currRule.mPath->move_to(mPathPoints[0].x, mPathPoints[0].y);
            mWantMoveTo = false;
            break;
        case moverel:
            m_currRule.mPath->move_rel(mPathPoints[0].x, mPathPoints[0].y);
            mWantMoveTo = false;
            break;
        case lineto:
            m_currRule.mPath->line_to(mPathPoints[0].x, mPathPoints[0].y);
            break;
        case linerel:
            m_currRule.mPath->line_rel(mPathPoints[0].x, mPathPoints[0].y);
            break;
        case arcto:
            m_currRule.mPath->arc_to(radius_x, radius_y, angle, largeArc, sweep, 
                                     mPathPoints[0].x, mPathPoints[0].y);
            break;
        case arcrel:
            m_currRule.mPath->arc_rel(radius_x, radius_y, angle, largeArc, sweep, 
                                      mPathPoints[0].x, mPathPoints[0].y);
            break;
        case curveto:
            if (mMask & 2) {
                if (mMask & 4) {
                    m_currRule.mPath->curve4(mPathPoints[1].x, mPathPoints[1].y, 
                                             mPathPoints[2].x, mPathPoints[2].y, 
                                             mPathPoints[0].x, mPathPoints[0].y);
                } else {
                    m_currRule.mPath->curve3(mPathPoints[1].x, mPathPoints[1].y, 
                                             mPathPoints[0].x, mPathPoints[0].y);
                }
            } else {
                double dx = 0.0, dy = 0.0;
                if (!agg::is_curve(m_currRule.mPath->last_vertex(&dx, &dy)))
                    return "Smooth curve operations must be preceded by another curve operation.";
                
                if (mMask & 4) {
                    m_currRule.mPath->curve4(mPathPoints[2].x, mPathPoints[2].y,
                                             mPathPoints[0].x, mPathPoints[0].y);
                } else {
                    m_currRule.mPath->curve3(mPathPoints[0].x, mPathPoints[0].y);
                }
            }
            break;
        case curverel:
            if (mMask & 2) {
                if (mMask & 4) {
                    m_currRule.mPath->curve4_rel(mPathPoints[1].x, mPathPoints[1].y, 
                                                 mPathPoints[2].x, mPathPoints[2].y, 
                                                 mPathPoints[0].x, mPathPoints[0].y);
                } else {
                    m_currRule.mPath->curve3_rel(mPathPoints[1].x, mPathPoints[1].y, 
                                                 mPathPoints[0].x, mPathPoints[0].y);
                }
            } else {
                double dx = 0.0, dy = 0.0;
                if (!agg::is_curve(m_currRule.mPath->last_vertex(&dx, &dy)))
                    return "Smooth curve operations must be preceded by another curve operation.";
                
                if (mMask & 4) {
                    m_currRule.mPath->curve4_rel(mPathPoints[2].x, mPathPoints[2].y,
                                                 mPathPoints[0].x, mPathPoints[0].y);
                } else {
                    m_currRule.mPath->curve3_rel(mPathPoints[0].x, mPathPoints[0].y);
                }
            }
            break;
    }
    
    m_currRule.mPath->last_vertex(&mLastPoint.x, &mLastPoint.y);
    return 0;
}

const char*
Builder::pathCmd(const char* opName)
{
    if (mLoopState == opLoop)
        return "Cannot place path commands inside of path operation loops.";
    unwindLoops(cmdLoop, false);
    
    finishPath(false);
    pathAttr* newAttr = new pathAttr(mIndex, m_currRule.mPath);

    if (strcmp(opName, "STROKE") == 0) {
        newAttr->mCommand = pathAttr::stroke;
    } else if (strcmp(opName, "FILL") == 0) {
        newAttr->mCommand = pathAttr::fill;
    } else {
        return "Unknown path operation/command";
    }
    newAttr->mCommand = strcmp(opName, "STROKE") ? 
        pathAttr::fill : pathAttr::stroke;
    if (mParameters) {
        newAttr->mEvenOddFlag = strstr(mParameters, "evenodd") != 0;
        newAttr->mIsoWidthFlag = strstr(mParameters, "iso") != 0 &&
                                 newAttr->mCommand == pathAttr::stroke; 
        if (strstr(mParameters, "miterjoin"))
            newAttr->mLineJoin = agg::miter_join;
        if (strstr(mParameters, "roundjoin"))
            newAttr->mLineJoin = agg::round_join;
        if (strstr(mParameters, "beveljoin"))
            newAttr->mLineJoin = agg::bevel_join;
        if (strstr(mParameters, "buttcap"))
            newAttr->mLineCap = agg::butt_cap;
        if (strstr(mParameters, "squarecap"))
            newAttr->mLineCap = agg::square_cap;
        if (strstr(mParameters, "roundcap"))
            newAttr->mLineCap = agg::round_cap;
    }
    newAttr->mStrokeWidth = mStrokeWidth;
    newAttr->mWorldState = mCurrentModification;
    newAttr->mNext = 0;

    addAttribute(newAttr);
    return 0;
}

void
Builder::addAttribute(pathAttr* newAttr)
{
    if (!m_currRule.mAttributes) {
        m_currRule.mAttributes = newAttr;
    } else {
        pathAttr* p = m_currRule.mAttributes;
        for (; p->mNext; p = p->mNext);
        p->mNext = newAttr;
    }
}

void
Builder::startPath()
{
    m_currRule.mPath = new agg::path_storage;
    mStop = false;
    mClosed = false;
    mWantMoveTo = true;
    mIndex = mNextIndex = 0;
    mLastPoint.x = mLastPoint.y = 0.0;
}

void
Builder::finishPath(bool setAttr)
{
    // Close and end the last path sequence if it wasn't already closed and ended
    if (!mClosed) {
        m_currRule.mPath->end_poly(0);
        mClosed = true;
    }
    
    if (!mStop) {
        m_currRule.mPath->start_new_path();
        mStop = true;
    }
    
    mWantMoveTo = true;
    mNextIndex = m_currRule.mPath->total_vertices();
    
    // If setAttr is true then make sure that the last path sequence has a path
    // attribute associated with it. 
    if (setAttr) {
        if (m_currRule.mAttributes) {
            pathAttr* last;
            for (last = m_currRule.mAttributes; last->mNext; last = last->mNext);
            if (last->mIndex != mIndex) 
                last->mNext = new pathAttr(mIndex, m_currRule.mPath);
        } else {
            m_currRule.mAttributes = new pathAttr(mIndex, m_currRule.mPath);
        }
    }
}

void
Builder::StrokeWidth(double w)
{
    mStrokeWidth = w;
}


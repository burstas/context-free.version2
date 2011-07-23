// yglue.cpp
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


#include "yglue.h"

#include <string.h>

#include "builder.h"
#include "cfdg.h"
#include "cfdgimpl.h"
#include "shape.h"
#include "primShape.h"
#include <cstdlib>
#include <stdlib.h>

#ifdef _WIN32
#pragma warning( disable : 4786 )

static double acosh(double x) 
{
    return log(x + sqrt(x * x - 1));
}

static double asinh(double x) 
{
    return log(x + sqrt(x * x + 1));
}

static double atanh(double x) 
{
    return log((1 + x) / (1 - x)) / 2;
}

static double fmin(const double& x, const double& y)
{
    return x < y ? x : y;
}
#endif

extern Builder * builder;

extern "C" {
    extern int yylineno;
    void yyrestart(FILE *new_file);
}

void yyerror(const char *msg) {
    builder->m_CFDG->system()->syntaxError(builder->m_currentPath, yylineno);
    builder->m_CFDG->system()->message(true, 
        "Line %d: Parse error - %s", yylineno, (char*)msg);
}


yg_real     yg_BuildReal(const char* token)    
{ 
    builder->addEntropy(token, true); 
    return atof(token); 
}
yg_string   yg_BuildString(const char* token)  
{ 
    if (*token == '\"') {
        char* ret = strdup(token + 1);
        ret[strlen(ret) - 1] = '\0';
        return ret;
    } else {
        return strdup(token); 
    }
}

void        yg_FreeString(yg_string st)        { free(st); }

using namespace std;

// Store parsed contents of include lines
void
yg_IncludeFile(yg_string fname)
{
    string path =
        builder->m_CFDG->system()->relativeFilePath(builder->m_currentPath, fname);
    builder->m_filesToLoad.push(path);
    yg_FreeString(fname);
}


static int
yg_StringToShape(yg_string st)
{
    string name = st;
    yg_FreeString(st);

    return builder->m_CFDG->encodeShapeName(name);
}

// Store parsed contents of startshape lines
void
yg_Initialize(yg_string name)
{
    Shape si;
    si.mShapeType = yg_StringToShape(name);
    
    builder->m_CFDG->setInitialShape(si);
}

// Store parsed contents of background lines
void
yg_Background()
{
    HSBColor solidWhite(0, 0, 1, 1);
    solidWhite.adjustWith(builder->mCurrentModification.m_Color);
    agg::rgba bk;
    solidWhite.getRGBA(bk);
    builder->m_CFDG->setBackgroundColor(bk);

    builder->clearModification();
}

//Store parsed contents of tile lines
const char*
yg_Tile()
{
    const char* errmsg = builder->buildTileTransform();
    builder->clearModification();
    return errmsg;
}

//Store parsed contents of size lines
const char*
yg_SizeLine()
{
    const char* errmsg = builder->buildSizeTransform();
    builder->clearModification();
    return errmsg;
}

const char*
yg_CheckRule(yg_string name, int isPath)
{
    int type = builder->m_CFDG->getShapeType(name);
    if ((isPath &&  type == CFDGImpl::ruleType) ||
        (!isPath && type == CFDGImpl::pathType))
            return "Cannot mix rules and shapes with the same name.";
    if (isPath && type != CFDGImpl::newShape)
        return "A given path can only be defined once";
    
    if (isPath) 
        builder->startPath();
    
    return 0;
}

static void
yg_RulePath(yg_string name, yg_real weight, bool isPath)
{
    if (weight <= 0.0) weight = 1.0;

    // Store the current rule and its replacements in the rule list
    builder->m_currRule.mInitialShapeType = yg_StringToShape(name);
    builder->m_currRule.mWeight = weight;
    if (isPath) {
        builder->finishPath(true);
    } else {
        builder->m_currRule.mAttributes = &Builder::DefaultPathAttributes;
    }
    builder->m_CFDG->addRule(builder->m_currRule, isPath);

    // Restore builder current rule to initial state for next rule
    builder->m_currRule = Rule();
}

void
yg_Rule(yg_string name, yg_real weight)
{
    yg_RulePath(name, weight, false);
}

void
yg_Path(yg_string name, yg_real weight)
{
    yg_RulePath(name, weight, true);
    builder->mLastPoint.x = builder->mLastPoint.y = 0.0;
}

void
yg_LoopStart()
{
    builder->LoopStart();
}

void 
yg_LoopEnd(int loopCount)
{
    builder->LoopEnd(loopCount);
}

void
yg_Replacement(yg_string name)
{
    // Store replacement shape in the rules replacement list
    Replacement r;
    builder->addEntropy(name);
    r.mShapeType = yg_StringToShape(name);
    r.mChildChange = builder->mCurrentModification;
    r.mLoopCount = 1;
    builder->m_currRule.addReplacement(r);
    builder->clearModification();
}

const char*
yg_PathOp(yg_string name)
{
    const char* ret = builder->pathOp(name);
    builder->clearModification();
    yg_FreeString(name);
    return ret;
}

void
yg_PathLoopStart()
{
    builder->PathLoopStart();
}

const char* 
yg_PathLoopEnd(int loopCount)
{
    return builder->PathLoopEnd(loopCount);
}

const char*
yg_PathCmd(yg_string name)
{
    const char* ret = builder->pathCmd(name);
    builder->clearModification();
    yg_FreeString(name);
    return ret;
}

void
yg_ClearTransform()
{
    builder->clearModification();
}

const char*
yg_SaveTransform()
{
    if (builder->mParameters) 
        return "Parameter strings are not allowed in loop adjustments";
    if (builder->mStrokeWidth != 0.1)
        return "Setting the stroke width is not allowed in loop adjustments";
    
    builder->mSavedModifications.push(builder->mCurrentModification);
    builder->clearModification();
    return 0;
}

// Calls the builder routine to discard the built-up transform that is used
// in the shape [ ... ] syntax and build a new transform use the standard
// transform order for the shape { ... } syntax.
void 
yg_DefTransform() 
{
    builder->buildDefaultTransform();
}

// These routines a) store that the transform was parsed b) multiply the 
// transform into the current transform
void yg_Orientation(yg_real v)          { builder->Orientation(v); }
void yg_Reflection(yg_real v)           { builder->Reflection(v); }
void yg_XLocation(yg_real v)            { builder->XLocation(v); }
void yg_YLocation(yg_real v)            { builder->YLocation(v); }
void yg_ZLocation(yg_real v)            { builder->ZLocation(v); }
void yg_Size(yg_real vx, yg_real vy, yg_real vz)    
                                        { builder->Size(vx, vy, vz); }
void yg_Skew(yg_real vx, yg_real vy)    { builder->Skew(vx, vy); }
void yg_Parameters(yg_string p)         { builder->Parameters(p); }
void yg_PointX(yg_real x, int index)    { builder->PointX(x, index); }
void yg_PointY(yg_real y, int index)    { builder->PointY(y, index); }
void yg_StrokeWidth(yg_real w)          { builder->StrokeWidth(w); }

void yg_Hue(yg_real v, int useTarget)
{
    builder->addEntropy("Hue");
    builder->m_CFDG->addParameter(CFDGImpl::Color);
    if (useTarget) {
        if (v < -1) v = -1;
        if (v >  1) v =  1;
    }
    builder->mCurrentModification.m_Color.h = v;
    if (useTarget) {
        builder->mCurrentModification.m_Color.mUseTarget |= HSBColor::HueTarget;
        builder->addEntropy("target");
    }
}

void yg_Saturation(yg_real v, int useTarget)
{
    builder->addEntropy("Saturation");
    builder->m_CFDG->addParameter(CFDGImpl::Color);
    if (v < -1) v = -1;
    if (v >  1) v =  1;
    builder->mCurrentModification.m_Color.s = v;
    if (useTarget) {
        builder->mCurrentModification.m_Color.mUseTarget |= HSBColor::SaturationTarget;
        builder->addEntropy("target");
    }
}

void yg_Brightness(yg_real v, int useTarget)
{
    builder->addEntropy("Brightness");
    if (v < -1) v = -1;
    if (v >  1) v =  1;
    builder->mCurrentModification.m_Color.b = v;
    if (useTarget) {
        builder->mCurrentModification.m_Color.mUseTarget |= HSBColor::BrightnessTarget;
        builder->addEntropy("target");
    }
}

void yg_Alpha(yg_real v, int useTarget)
{
    builder->addEntropy("Alpha");
    builder->m_CFDG->addParameter(CFDGImpl::Alpha);
    if (v < -1) v = -1;
    if (v >  1) v =  1;
    builder->mCurrentModification.m_Color.a = v;
    if (useTarget) {
        builder->mCurrentModification.m_Color.mUseTarget |= HSBColor::AlphaTarget;
        builder->addEntropy("target");
    }
}

void yg_HueTarget(yg_real v, int )
{
    builder->addEntropy("HueTarget");
    builder->m_CFDG->addParameter(CFDGImpl::Color);
    builder->mCurrentModification.m_ColorTarget.h = v;
}

void yg_SaturationTarget(yg_real v, int )
{
    builder->addEntropy("SaturationTarget");
    builder->m_CFDG->addParameter(CFDGImpl::Color);
    if (v < -1) v = -1;
    if (v >  1) v =  1;
    builder->mCurrentModification.m_ColorTarget.s = v;
}

void yg_BrightnessTarget(yg_real v, int )
{
    builder->addEntropy("BrightnessTarget");
    if (v < -1) v = -1;
    if (v >  1) v =  1;
    builder->mCurrentModification.m_ColorTarget.b = v;
}

void yg_AlphaTarget(yg_real v, int )
{
    builder->addEntropy("AlphaTarget");
    builder->m_CFDG->addParameter(CFDGImpl::Alpha);
    if (v < -1) v = -1;
    if (v >  1) v =  1;
    builder->mCurrentModification.m_ColorTarget.a = v;
}

yg_real yg_function(int func, yg_real arg1, yg_real arg2)
{
    if (func & 1) {
        switch (func >> 1) {
            case 0: return atan2(arg1, arg2) * 57.29577951308;
            case 1: return fmod(arg1, arg2);
        }
    } else {
        switch (func >> 1) {
            case  0: return cos(arg1 * 0.0174532925199);
            case  1: return sin(arg1 * 0.0174532925199);
            case  2: return tan(arg1 * 0.0174532925199);
            case  3: return 1.0 / tan(arg1 * 0.0174532925199);
            case  4: return acos(arg1) * 57.29577951308;
            case  5: return asin(arg1) * 57.29577951308;
            case  6: return atan(arg1) * 57.29577951308;
            case  7: return atan(1.0 / arg1) * 57.29577951308;
            case  8: return cosh(arg1);
            case  9: return sinh(arg1);
            case 10: return tanh(arg1);
            case 11: return acosh(arg1);
            case 12: return asinh(arg1);
            case 13: return atanh(arg1);
            case 14: return log(arg1);
            case 15: return log10(arg1);
            case 16: return sqrt(arg1);
            case 17: return exp(arg1);
            case 18: return fabs(arg1);
            case 1000: 
                arg1 = 1.0;
            case 1001: 
                arg2 = 0.0;
            case 1002: 
                return builder->mSeed.getDouble() * fabs(arg2 - arg1) + 
                                                    fmin(arg1, arg2);
        }
    }
    return 0.0;
}

int
yg_functionType(yg_string func, int argcount)
{
    static const char* monadics = "cos   sin   tan   cot   acos  asin  atan  acot  cosh  sinh  tanh acosh asinh atanh log   log10  sqrt  exp   abs   ";
    static const char* dyadics  = "atan2  mod   ";
    
    if (func == NULL || *func == '\0') {
        builder->m_CFDG->system()->syntaxError(builder->m_currentPath, yylineno);
        builder->m_CFDG->system()->message(true, 
                                           "Line %d: Parse error - bad function call", yylineno);
        return -1;
    }
    
    if (strcmp(func, "rand_static") == 0) return (1000 + argcount) * 2;
    
    builder->addEntropy(func);
    const char* w = strstr(monadics, func);
    if (w && (w - monadics) % 6 == 0) {
        if (argcount != 1) {
            builder->m_CFDG->system()->syntaxError(builder->m_currentPath, yylineno);
            builder->m_CFDG->system()->message(true, 
                                               "Line %d: Function %s() takes one argument", yylineno, func);
            return -1;
        }
        return (w - monadics) / 3;
    }
    
    w = strstr(dyadics, func);
    if (w && (w - dyadics) % 6 == 0) {
        if (argcount != 2) {
            builder->m_CFDG->system()->syntaxError(builder->m_currentPath, yylineno);
            builder->m_CFDG->system()->message(true, 
                                               "Line %d: Function %s() takes two arguments", yylineno, func);
            return -1;
        }
        return (w - dyadics) / 3 | 1;
    }
    
    builder->m_CFDG->system()->syntaxError(builder->m_currentPath, yylineno);
    builder->m_CFDG->system()->message(true, 
                                       "Line %d: Unknown function - %s", yylineno, func);
    return -1;
}

void 
yg_Entropy(yg_string s)
{
    builder->addEntropy(s);
}

void 
yg_NegEntropy()
{
    builder->negEntropy();
}

void
yg_Reset()
{
    yylineno = 1;
    yyrestart(0);
}

void
yg_ResetForNextFile()
{
    yylineno = 1;
}

int
yg_Input(char* buf, int max)
{
    if (!builder->m_input  ||  builder->m_input->eof())
        return 0;
    
    builder->m_input->read(buf, max);
	
	char* r = buf;
	char* w = r;
	char* e = r + builder->m_input->gcount();
    
    // Tack on a line-feed at the end of the cfdg file so that line comments
    // at the end of the file are tokenized correctly even if they have no
    // terminating line feed.
    if ((e - r) < max) *e++ = '\n';
    
    // Convert "\r\n" to " \n" and "\r" to "\n"
	while (r != e) {
		char c = *r++;
		if (c == '\r') {
			*w++ = (*r == '\n') ? ' ' : '\n';
		} else {
            *w++ = c;
		}
	}
	
    return w - buf;
}


// yglue.h
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


#ifdef __cplusplus
extern "C" {
#endif

#include "math.h"
#include "stdio.h"
    
typedef double yg_real;
typedef char * yg_string;

typedef union {
    yg_real     real;
    yg_string   string;
} yystype;

#define YY_NO_UNISTD_H
#define YYSTYPE yystype

#define YY_INPUT(buf,result,max_size) \
    { result = yg_Input(buf, max_size); if (result == 0) result = YY_NULL; }

int yyget_lineno(void);
FILE *yyget_in  (void);
FILE *yyget_out  (void);
size_t yyget_leng  (void);
char *yyget_text  (void);
void yyset_lineno (int  line_number );
void yyset_in (FILE *  in_str );
void yyset_out (FILE *  out_str );
int yyget_debug  (void);
void yyset_debug (int  bdebug );
int yylex_destroy  (void);

yg_real     yg_BuildReal(const char*);
yg_string   yg_BuildString(const char*);
void        yg_FreeString(yg_string);

void yg_IncludeFile(yg_string);

void yg_Initialize(yg_string);
void yg_Background();
const char* yg_Tile();
const char* yg_SizeLine();
const char* yg_CheckRule(yg_string, int);
void yg_Rule(yg_string, yg_real weight);
void yg_Path(yg_string, yg_real weight);
void yg_Replacement(yg_string);
void yg_LoopStart();
void yg_LoopEnd(int);
const char* yg_PathOp(yg_string);
void yg_PathLoopStart();
const char* yg_PathLoopEnd(int);
const char* yg_PathCmd(yg_string);
void yg_CmdLoopStart();
void yg_CmdLoopEnd(int);
void yg_StrokeWidth(yg_real);
void yg_DefTransform();
const char* yg_SaveTransform();
void yg_ClearTransform();
yg_real yg_function(int, yg_real, yg_real);
int yg_functionType(yg_string func, int argcount);
void yg_Entropy(yg_string);
void yg_NegEntropy();

void yg_Orientation(yg_real);
void yg_Reflection(yg_real);
void yg_XLocation(yg_real);
void yg_YLocation(yg_real);
void yg_PointX(yg_real, int);
void yg_PointY(yg_real, int);
void yg_ZLocation(yg_real);
void yg_Size(yg_real, yg_real, yg_real);
void yg_Skew(yg_real, yg_real);
void yg_Parameters(yg_string);
void yg_Hue(yg_real, int);
void yg_Saturation(yg_real, int);
void yg_Brightness(yg_real, int);
void yg_Alpha(yg_real, int);
void yg_HueTarget(yg_real, int);
void yg_SaturationTarget(yg_real, int);
void yg_BrightnessTarget(yg_real, int);
void yg_AlphaTarget(yg_real, int);

void yg_Reset();
void yg_ResetForNextFile();
int yg_Input(char*, int);

void yyerror(const char *);
int yylex(void);

#ifdef __cplusplus
}
#endif

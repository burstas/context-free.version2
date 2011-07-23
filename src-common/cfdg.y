// cfdg.y
// this file is part of Context Free
// ---------------------
// Copyright (C) 2005-2008 Mark Lentczner - markl@glyphic.com
// Copyright (C) 2005-2008 John Horigan - john@glyphic.com
// Copyright (C) 2005 Chris Coyne - ccoyne77@gmail.com
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


%{
    #include "yglue.h"
    #include "math.h"
%}

%token STARTSHAPE
%token RULE
%token PATH
%token ROTATE
%token FLIP
%token XSHIFT
%token YSHIFT
%token XCTRL1
%token YCTRL1
%token XCTRL2
%token YCTRL2
%token XRADIUS
%token YRADIUS
%token ARC
%token ZSHIFT
%token SIZE
%token SKEW
%token HUE
%token SATURATION
%token BRIGHTNESS
%token ALPHA
%token TARGETHUE
%token TARGETSATURATION
%token TARGETBRIGHTNESS
%token TARGETALPHA
%token BACKGROUND
%token <string> USER_STRING
%token <real> USER_INTEGER
%token <real> USER_RATIONAL
%token <string> USER_FILENAME
%token INCLUDE
%token TILE
%token PARAMETERS
%token <string> USER_PATHOP
%token STROKEWIDTH
%left '-' '+'
%left '*' '/'
%left NEG POS    /* negation--unary minus, unary identity */
%right '^'    /* exponentiation */
%type <real> exp exp2

%%


cfdg:
        cfdg statement
        |
        ;
        
statement:
          initialization
        | background  
        | inclusion
        | tile
        | size
        | rule
        | path
        ;
        
inclusion:
        INCLUDE USER_STRING  {
            yg_IncludeFile($2);
        }
        |
        INCLUDE USER_FILENAME  {
            yg_IncludeFile($2);
        } 
        ;

initialization:
        STARTSHAPE USER_STRING  {
            yg_Initialize($2);
        }

background:
        BACKGROUND '{' buncha_color_adjustments '}' {
            yg_Background();
        }
        ;

tile:
        TILE modification {
            const char *errmsg = yg_Tile();
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
        }
        ;

size:
        SIZE modification {
            const char *errmsg = yg_SizeLine();
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
        }
        ; 

rule:
        RULE USER_STRING {
            const char *errmsg = yg_CheckRule($2, 0);
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
        } '{' buncha_replacements '}' {
            yg_Rule($2, 1.0);
        }
        |
        RULE USER_STRING USER_RATIONAL {
            const char *errmsg = yg_CheckRule($2, 0);
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
            if ($3 <= 0.0) {
                yyerror("Rule weights must be positive.");
                YYABORT;
            }
            yg_ClearTransform(); 
        } '{' buncha_replacements '}' {
            yg_Rule($2, $3);
        }
        ;

path:
        PATH USER_STRING {
            const char *errmsg = yg_CheckRule($2, 1);
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
        } '{' buncha_pathOps '}' {
            yg_Path($2, 1.0);
        }
        ;

buncha_pathOps: 
        buncha_pathOps pathOp
        |
        ;

pathOp:
        USER_PATHOP '{' points '}' {
            const char *errmsg = yg_PathOp($1);
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
        }
        |
        USER_RATIONAL '*' path_modification { 
            const char *errmsg = yg_SaveTransform();
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
            yg_PathLoopStart();
        } USER_PATHOP '{' points '}' {
            const char *errmsg = yg_PathOp($5);
            if (!errmsg) errmsg = yg_PathLoopEnd((int)$1);
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
        }
        |
        USER_RATIONAL '*' path_modification { 
            const char *errmsg = yg_SaveTransform();
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
            yg_PathLoopStart();
        } '{' buncha_pathOps '}' {
            const char *errmsg = yg_PathLoopEnd((int)$1);
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
        }
        |
        USER_STRING path_modification {
            const char *errmsg = yg_PathCmd($1);
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
        }
        |
        USER_RATIONAL '*' path_modification { 
            const char *errmsg = yg_SaveTransform();
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
            yg_PathLoopStart();
        } USER_STRING path_modification {
            const char *errmsg = yg_PathCmd($5);
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
            yg_PathLoopEnd((int)$1);
        }
        ;

points:
        points point
        |
        ;

point:
        XSHIFT exp  {
            yg_PointX($2, 0);
        }
        |
        XCTRL1 exp  {
            yg_PointX($2, 1);
        }
        |
        XCTRL2 exp  {
            yg_PointX($2, 2);
        }
        |
        YSHIFT exp  {
            yg_PointY($2, 0);
        }
        |
        YCTRL1 exp  {
            yg_PointY($2, 1);
        }
        |
        YCTRL2 exp  {
            yg_PointY($2, 2);
        }
        |
        XRADIUS exp  {
            yg_PointX($2, 3);
        }
        |
        YRADIUS exp  {
            yg_PointY($2, 3);
        }
        |
        ROTATE exp {
            yg_PointX($2, 4);
        }
        |
        PARAMETERS USER_STRING {
            yg_Parameters($2);
        }
        ;

buncha_replacements: 
        buncha_replacements replacement
        |
        ;

replacement:
        USER_STRING modification {
            yg_Replacement($1);
        }
        |
        USER_RATIONAL '*' modification { 
            yg_SaveTransform(); 
            yg_LoopStart();
        } USER_STRING modification {
            yg_Replacement($5);
            yg_LoopEnd((int)$1);
        }
        |
        USER_RATIONAL '*' modification { 
            yg_SaveTransform(); 
            yg_LoopStart();
        } '{' buncha_replacements '}' {
            yg_LoopEnd((int)$1);
        }
        ;

modification:
        '{' buncha_adjustments '}' {
            yg_DefTransform();
        }
        |
        '[' buncha_adjustments ']' {
        }
        ;

path_modification:
        '{' buncha_path_adjustments '}' {
            yg_DefTransform();
        }
        |
        '[' buncha_path_adjustments ']' {
        }
        ;

buncha_path_adjustments:
        buncha_path_adjustments path_adjustment
        |
        ;

path_adjustment:
        geom_adjustment
        |
        color_adjustment
        |
        PARAMETERS USER_STRING {
            yg_Parameters($2);
        }
        |
        STROKEWIDTH exp {
            yg_StrokeWidth($2);
        }
        |
        ZSHIFT exp {
            yyerror("Z changes are not allowed in paths");
            YYABORT;
        }
        |
        SIZE exp exp exp {
            yyerror("Z changes are not allowed in paths");
            YYABORT;
        }
        ;

buncha_adjustments:
        buncha_adjustments adjustment
        |
        ;

buncha_color_adjustments:
        buncha_color_adjustments color_adjustment
        |
        ;

adjustment:
        geom_adjustment
        |
        color_adjustment
        |
        ZSHIFT exp {
            yg_ZLocation($2);
        }
        |
        SIZE exp exp exp {
            yg_Size($2, $3, $4);
        }
        |
        PARAMETERS USER_STRING {
            yyerror("Parameters are not allowed in rules");
            YYABORT;
        }
        |
        STROKEWIDTH exp {
            yyerror("Stroke widths are not allowed in rules");
            YYABORT;
        }
        ;

geom_adjustment:
        ROTATE exp {
            yg_Orientation($2);
        }
        |
        FLIP exp {
            yg_Reflection($2);
        }
        |
        XSHIFT exp {
            yg_XLocation($2);
        }
        |
        YSHIFT exp {
            yg_YLocation($2);
        }
        |
        SIZE exp {
            yg_Size($2, $2, 1.0);
        }
        |
        SIZE exp exp {
            yg_Size($2, $3, 1.0);
        }
        |
        SKEW exp exp {
            yg_Skew($2, $3);
        }
        ;

color_adjustment:
        HUE exp {
            yg_Hue($2, 0);
        }
        |
        SATURATION exp {
            yg_Saturation($2, 0);
        }
        |
        BRIGHTNESS exp {
            yg_Brightness($2, 0);
        }
        |
        ALPHA exp {
            yg_Alpha($2, 0);
        }
        |
        HUE exp '|' {
            yg_Entropy("|");
            yg_Hue($2, -1);
        }
        |
        SATURATION exp '|' {
            yg_Entropy("|");
            yg_Saturation($2, -1);
        }
        |
        BRIGHTNESS exp '|' {
            yg_Entropy("|");
            yg_Brightness($2, -1);
        }
        |
        ALPHA exp '|' {
            yg_Entropy("|");
            yg_Alpha($2, -1);
        }
        |
        TARGETHUE exp {
            yg_HueTarget($2, 0);
        }
        |
        TARGETSATURATION exp {
            yg_SaturationTarget($2, 0);
        }
        |
        TARGETBRIGHTNESS exp {
            yg_BrightnessTarget($2, 0);
        }
        |
        TARGETALPHA exp {
            yg_AlphaTarget($2, 0);
        } 
        ;

exp:
        USER_RATIONAL       { $$ = $1; }
        |
        '-' USER_RATIONAL   { $$ = -$2; yg_NegEntropy(); }
        |
        '+' USER_RATIONAL   { $$ = $2; }
        |
        '(' exp2 ')'        { $$ = $2; yg_Entropy("()"); }
        | 
        USER_STRING '(' ')'   { 
            int ftype = yg_functionType($1, 0);
            yg_FreeString($1); 
            if (ftype == -1) {
                YYABORT;
            }
            $$ = yg_function(ftype, 0.0, 0.0); 
        }
        | 
        USER_STRING '(' exp2 ')'   { 
            int ftype = yg_functionType($1, 1);
            yg_FreeString($1); 
            if (ftype == -1) {
                YYABORT;
            }
            $$ = yg_function(ftype, $3, 0.0); 
        }
        | 
        USER_STRING '(' exp2 ',' exp2 ')'   {
            int ftype = yg_functionType($1, 2);
            yg_FreeString($1); 
            if (ftype == -1) {
                YYABORT;
            }
            $$ = yg_function(ftype, $3, $5); 
        }
        ;

exp2: 
        USER_RATIONAL      { $$ = $1;         }
        | 
        USER_STRING '(' ')'   { 
            int ftype = yg_functionType($1, 0);
            yg_FreeString($1); 
            if (ftype == -1) {
                YYABORT;
            }
            $$ = yg_function(ftype, 0.0, 0.0); 
        }
        | 
        USER_STRING '(' exp2 ')'   { 
            int ftype = yg_functionType($1, 1);
            yg_FreeString($1); 
            if (ftype == -1) {
                YYABORT;
            }
            $$ = yg_function(ftype, $3, 0.0); 
        }
        | 
        USER_STRING '(' exp2 ',' exp2 ')'   {
            int ftype = yg_functionType($1, 2);
            yg_FreeString($1); 
            if (ftype == -1) {
                YYABORT;
            }
            $$ = yg_function(ftype, $3, $5); 
        }
        | 
        exp2 '+' exp2        { $$ = $1 + $3; yg_Entropy("+");   }
        | 
        exp2 '-' exp2        { $$ = $1 - $3; yg_Entropy("-"); }
        | 
        exp2 '*' exp2        { $$ = $1 * $3; yg_Entropy("*"); }
        | 
        exp2 '/' exp2        { $$ = $1 / $3; yg_Entropy("/"); }
        | 
        '-' exp2  %prec NEG { $$ = -$2; yg_Entropy("NEG"); }
        | 
        '+' exp2  %prec POS { $$ = $2; yg_Entropy("POS"); }
        | 
        exp2 '^' exp2        { $$ = pow ($1, $3); yg_Entropy("^"); }
        | 
        '(' exp2 ')'        { $$ = $2; yg_Entropy("()"); }
        ;

%%



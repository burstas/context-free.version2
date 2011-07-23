/* cfdg.l
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
*/


%{
    #include "yglue.h"
    #include "y.tab.h"
    
    extern YYSTYPE yylval;
%}


INTEGER     [0-9]+
RATIONAL    ([0-9]+\.[0-9]*)|(\.[0-9]+)
PATHOP      MOVETO|LINETO|ARCTO|CURVETO|MOVEREL|LINEREL|ARCREL|CURVEREL|CLOSEPOLY
FILENAME   ([a-zA-Z\200-\377][a-zA-Z\200-\377_\-0-9\.]*\.cfdg)|(\"[^\n\r\f\"]*\")
STRING     [a-zA-Z\200-\377_]+[a-zA-Z_0-9\200-\377]*
WHITESPACE  [ \n\t\r\f]*

/* calling this externally    */
/* don't want yywrap to exist.*/

%option noyywrap
%option yylineno
%%

\#[^\n]*\n  /* pound comment line */
\/\/[^\n]*\n  /* c++ style comment line */
\/\*([^\*]|[\r\n]|(\*+([^\*\/]|[\r\n])))*\*+\/ /* c style comments */
\#[^\r\n]*\r\n*  /* pound comment line */
\/\/[^\r\n]*\r\n*  /* c++ style comment line */


"startshape"    {return STARTSHAPE;}
"background"    {return BACKGROUND;}
"include"       {return INCLUDE;}
"tile"          {return TILE;}
"rule"          {return RULE;}
"path"          {return PATH;}
"{"             {return '{';}
"}"             {return '}';}
"["             {return '[';}
"]"             {return ']';}
"^"             {return '^';}
"*"             {return '*';}
"/"             {return '/';}
"+"             {return '+';}
"-"             {return '-';}
"("             {return '(';}
")"             {return ')';}
","             {return ',';}
"rotate"        {return ROTATE;}
"r"             {return ROTATE;}
"flip"	        {return FLIP;}
"f"             {return FLIP;}
"hue"           {return HUE;}
"h"             {return HUE;}
"saturation"    {return SATURATION;}
"sat"           {return SATURATION;}
"brightness"    {return BRIGHTNESS;}
"b"             {return BRIGHTNESS;}
"alpha"         {return ALPHA;}
"a"             {return ALPHA;}
"x"             {return XSHIFT;}
"y"             {return YSHIFT;}
"x1"            {return XCTRL1;}
"y1"            {return YCTRL1;}
"rx"            {return XRADIUS;}
"ry"            {return YRADIUS;}
"x2"            {return XCTRL2;}
"y2"            {return YCTRL2;}
"z"             {return ZSHIFT;}
"size"          {return SIZE;}
"s"             {return SIZE;}
"skew"          {return SKEW;}
"|hue"          {return TARGETHUE;}
"|h"            {return TARGETHUE;}
"|saturation"   {return TARGETSATURATION;}
"|sat"          {return TARGETSATURATION;}
"|brightness"   {return TARGETBRIGHTNESS;}
"|b"            {return TARGETBRIGHTNESS;}
"|alpha"        {return TARGETALPHA;}
"|a"            {return TARGETALPHA;}
"|"             {return '|';}
"param"         {return PARAMETERS;}
"p"             {return PARAMETERS;}
"width"         {return STROKEWIDTH;}
                                                        

{PATHOP}    {yylval.string=yg_BuildString(yytext); return USER_PATHOP;}
{STRING}    {yylval.string=yg_BuildString(yytext); return USER_STRING;}
{RATIONAL}  {yylval.real=yg_BuildReal(yytext); return USER_RATIONAL;}
{INTEGER}   {yylval.real=yg_BuildReal(yytext); return USER_RATIONAL;}
{FILENAME}    {yylval.string=yg_BuildString(yytext); return USER_FILENAME;}

.|\n|\r      /* eat up any unmatched character */

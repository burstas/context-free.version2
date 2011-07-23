/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     STARTSHAPE = 258,
     RULE = 259,
     PATH = 260,
     ROTATE = 261,
     FLIP = 262,
     XSHIFT = 263,
     YSHIFT = 264,
     XCTRL1 = 265,
     YCTRL1 = 266,
     XCTRL2 = 267,
     YCTRL2 = 268,
     XRADIUS = 269,
     YRADIUS = 270,
     ARC = 271,
     ZSHIFT = 272,
     SIZE = 273,
     SKEW = 274,
     HUE = 275,
     SATURATION = 276,
     BRIGHTNESS = 277,
     ALPHA = 278,
     TARGETHUE = 279,
     TARGETSATURATION = 280,
     TARGETBRIGHTNESS = 281,
     TARGETALPHA = 282,
     BACKGROUND = 283,
     USER_STRING = 284,
     USER_INTEGER = 285,
     USER_RATIONAL = 286,
     USER_FILENAME = 287,
     INCLUDE = 288,
     TILE = 289,
     PARAMETERS = 290,
     USER_PATHOP = 291,
     STROKEWIDTH = 292,
     POS = 293,
     NEG = 294
   };
#endif
/* Tokens.  */
#define STARTSHAPE 258
#define RULE 259
#define PATH 260
#define ROTATE 261
#define FLIP 262
#define XSHIFT 263
#define YSHIFT 264
#define XCTRL1 265
#define YCTRL1 266
#define XCTRL2 267
#define YCTRL2 268
#define XRADIUS 269
#define YRADIUS 270
#define ARC 271
#define ZSHIFT 272
#define SIZE 273
#define SKEW 274
#define HUE 275
#define SATURATION 276
#define BRIGHTNESS 277
#define ALPHA 278
#define TARGETHUE 279
#define TARGETSATURATION 280
#define TARGETBRIGHTNESS 281
#define TARGETALPHA 282
#define BACKGROUND 283
#define USER_STRING 284
#define USER_INTEGER 285
#define USER_RATIONAL 286
#define USER_FILENAME 287
#define INCLUDE 288
#define TILE 289
#define PARAMETERS 290
#define USER_PATHOP 291
#define STROKEWIDTH 292
#define POS 293
#define NEG 294




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;


/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 31 "../../src-common/cfdg.y"

    #include "yglue.h"
    #include "math.h"


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 189 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   328

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  53
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  35
/* YYNRULES -- Number of rules.  */
#define YYNRULES  111
/* YYNRULES -- Number of states.  */
#define YYNSTATES  209

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   294

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      50,    51,    40,    39,    52,    38,     2,    41,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    47,     2,    48,    44,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    45,    49,    46,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    42,    43
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     7,     9,    11,    13,    15,    17,
      19,    21,    24,    27,    30,    35,    38,    41,    42,    49,
      50,    58,    59,    66,    69,    70,    75,    76,    85,    86,
      94,    97,    98,   105,   108,   109,   112,   115,   118,   121,
     124,   127,   130,   133,   136,   139,   142,   143,   146,   147,
     154,   155,   163,   167,   171,   175,   179,   182,   183,   185,
     187,   190,   193,   196,   201,   204,   205,   208,   209,   211,
     213,   216,   221,   224,   227,   230,   233,   236,   239,   242,
     246,   250,   253,   256,   259,   262,   266,   270,   274,   278,
     281,   284,   287,   290,   292,   295,   298,   302,   306,   311,
     318,   320,   324,   329,   336,   340,   344,   348,   352,   355,
     358,   362
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      54,     0,    -1,    54,    55,    -1,    -1,    57,    -1,    58,
      -1,    56,    -1,    59,    -1,    60,    -1,    61,    -1,    64,
      -1,    33,    29,    -1,    33,    32,    -1,     3,    29,    -1,
      28,    45,    82,    46,    -1,    34,    77,    -1,    18,    77,
      -1,    -1,     4,    29,    62,    45,    73,    46,    -1,    -1,
       4,    29,    31,    63,    45,    73,    46,    -1,    -1,     5,
      29,    65,    45,    66,    46,    -1,    66,    67,    -1,    -1,
      36,    45,    71,    46,    -1,    -1,    31,    40,    78,    68,
      36,    45,    71,    46,    -1,    -1,    31,    40,    78,    69,
      45,    66,    46,    -1,    29,    78,    -1,    -1,    31,    40,
      78,    70,    29,    78,    -1,    71,    72,    -1,    -1,     8,
      86,    -1,    10,    86,    -1,    12,    86,    -1,     9,    86,
      -1,    11,    86,    -1,    13,    86,    -1,    14,    86,    -1,
      15,    86,    -1,     6,    86,    -1,    35,    29,    -1,    73,
      74,    -1,    -1,    29,    77,    -1,    -1,    31,    40,    77,
      75,    29,    77,    -1,    -1,    31,    40,    77,    76,    45,
      73,    46,    -1,    45,    81,    46,    -1,    47,    81,    48,
      -1,    45,    79,    46,    -1,    47,    79,    48,    -1,    79,
      80,    -1,    -1,    84,    -1,    85,    -1,    35,    29,    -1,
      37,    86,    -1,    17,    86,    -1,    18,    86,    86,    86,
      -1,    81,    83,    -1,    -1,    82,    85,    -1,    -1,    84,
      -1,    85,    -1,    17,    86,    -1,    18,    86,    86,    86,
      -1,    35,    29,    -1,    37,    86,    -1,     6,    86,    -1,
       7,    86,    -1,     8,    86,    -1,     9,    86,    -1,    18,
      86,    -1,    18,    86,    86,    -1,    19,    86,    86,    -1,
      20,    86,    -1,    21,    86,    -1,    22,    86,    -1,    23,
      86,    -1,    20,    86,    49,    -1,    21,    86,    49,    -1,
      22,    86,    49,    -1,    23,    86,    49,    -1,    24,    86,
      -1,    25,    86,    -1,    26,    86,    -1,    27,    86,    -1,
      31,    -1,    38,    31,    -1,    39,    31,    -1,    50,    87,
      51,    -1,    29,    50,    51,    -1,    29,    50,    87,    51,
      -1,    29,    50,    87,    52,    87,    51,    -1,    31,    -1,
      29,    50,    51,    -1,    29,    50,    87,    51,    -1,    29,
      50,    87,    52,    87,    51,    -1,    87,    39,    87,    -1,
      87,    38,    87,    -1,    87,    40,    87,    -1,    87,    41,
      87,    -1,    38,    87,    -1,    39,    87,    -1,    87,    44,
      87,    -1,    50,    87,    51,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    81,    81,    82,    86,    87,    88,    89,    90,    91,
      92,    96,   100,   106,   111,   117,   127,   137,   137,   147,
     147,   164,   164,   176,   177,   181,   189,   189,   205,   205,
     220,   228,   228,   246,   247,   251,   255,   259,   263,   267,
     271,   275,   279,   283,   287,   293,   294,   298,   302,   302,
     310,   310,   319,   323,   328,   332,   337,   338,   342,   344,
     346,   350,   354,   359,   366,   367,   371,   372,   376,   378,
     380,   384,   388,   393,   400,   404,   408,   412,   416,   420,
     424,   430,   434,   438,   442,   446,   451,   456,   461,   466,
     470,   474,   478,   484,   486,   488,   490,   492,   501,   510,
     521,   523,   532,   541,   550,   552,   554,   556,   558,   560,
     562,   564
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "STARTSHAPE", "RULE", "PATH", "ROTATE",
  "FLIP", "XSHIFT", "YSHIFT", "XCTRL1", "YCTRL1", "XCTRL2", "YCTRL2",
  "XRADIUS", "YRADIUS", "ARC", "ZSHIFT", "SIZE", "SKEW", "HUE",
  "SATURATION", "BRIGHTNESS", "ALPHA", "TARGETHUE", "TARGETSATURATION",
  "TARGETBRIGHTNESS", "TARGETALPHA", "BACKGROUND", "USER_STRING",
  "USER_INTEGER", "USER_RATIONAL", "USER_FILENAME", "INCLUDE", "TILE",
  "PARAMETERS", "USER_PATHOP", "STROKEWIDTH", "'-'", "'+'", "'*'", "'/'",
  "POS", "NEG", "'^'", "'{'", "'}'", "'['", "']'", "'|'", "'('", "')'",
  "','", "$accept", "cfdg", "statement", "inclusion", "initialization",
  "background", "tile", "size", "rule", "@1", "@2", "path", "@3",
  "buncha_pathOps", "pathOp", "@4", "@5", "@6", "points", "point",
  "buncha_replacements", "replacement", "@7", "@8", "modification",
  "path_modification", "buncha_path_adjustments", "path_adjustment",
  "buncha_adjustments", "buncha_color_adjustments", "adjustment",
  "geom_adjustment", "color_adjustment", "exp", "exp2", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,    45,    43,
      42,    47,   293,   294,    94,   123,   125,    91,    93,   124,
      40,    41,    44
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    53,    54,    54,    55,    55,    55,    55,    55,    55,
      55,    56,    56,    57,    58,    59,    60,    62,    61,    63,
      61,    65,    64,    66,    66,    67,    68,    67,    69,    67,
      67,    70,    67,    71,    71,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    73,    73,    74,    75,    74,
      76,    74,    77,    77,    78,    78,    79,    79,    80,    80,
      80,    80,    80,    80,    81,    81,    82,    82,    83,    83,
      83,    83,    83,    83,    84,    84,    84,    84,    84,    84,
      84,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    86,    86,    86,    86,    86,    86,    86,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     0,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     4,     2,     2,     0,     6,     0,
       7,     0,     6,     2,     0,     4,     0,     8,     0,     7,
       2,     0,     6,     2,     0,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     0,     2,     0,     6,
       0,     7,     3,     3,     3,     3,     2,     0,     1,     1,
       2,     2,     2,     4,     2,     0,     2,     0,     1,     1,
       2,     4,     2,     2,     2,     2,     2,     2,     2,     3,
       3,     2,     2,     2,     2,     3,     3,     3,     3,     2,
       2,     2,     2,     1,     2,     2,     3,     3,     4,     6,
       1,     3,     4,     6,     3,     3,     3,     3,     2,     2,
       3,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       2,     6,     4,     5,     7,     8,     9,    10,    13,    17,
      21,    65,    65,    16,    67,    11,    12,    15,    19,     0,
       0,     0,     0,     0,     0,    46,    24,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,    64,    68,    69,    53,    14,
      66,    46,     0,     0,     0,    93,     0,     0,     0,    74,
      75,    76,    77,    70,    78,     0,    81,    82,    83,    84,
      89,    90,    91,    92,    72,    73,     0,     0,     0,    18,
      45,     0,     0,     0,    22,    23,     0,    94,    95,     0,
     100,     0,     0,     0,     0,    79,    80,    85,    86,    87,
      88,    20,    47,     0,    57,    57,    30,     0,    34,    97,
       0,     0,   108,   109,     0,     0,     0,     0,     0,     0,
      96,    71,    48,     0,     0,    26,     0,    98,     0,   101,
       0,   111,   105,   104,   106,   107,   110,     0,     0,     0,
       0,     0,     0,    54,    56,    58,    59,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    25,    33,     0,   102,     0,     0,    46,    62,    78,
      60,    61,     0,    24,     0,    43,    35,    38,    36,    39,
      37,    40,    41,    42,    44,    99,     0,    49,     0,    79,
      34,     0,    32,   103,    51,    63,     0,    29,    27
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    10,    11,    12,    13,    14,    15,    16,    29,
      34,    17,    30,    63,    95,   158,   159,   160,   136,   172,
      62,    90,   147,   148,    23,   116,   133,   154,    31,    33,
      55,    56,    57,    69,   104
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -99
static const yytype_int16 yypact[] =
{
     -99,    77,   -99,    -5,    10,    31,    11,    39,    21,    11,
     -99,   -99,   -99,   -99,   -99,   -99,   -99,   -99,   -99,    35,
     -99,   -99,   -99,   -99,   -99,   -99,   -99,   -99,   -99,    47,
      48,   194,   128,   244,    52,   -99,   -99,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,    69,   245,   -99,   -99,   -99,   -99,   -99,   -99,
     -99,   -99,    33,     5,    53,   -99,    71,    83,   248,   -99,
     -99,   -99,   -99,   -99,   245,   245,    67,    68,    73,    74,
     -99,   -99,   -99,   -99,   -99,   -99,    60,    11,    98,   -99,
     -99,    14,   100,    99,   -99,   -99,     4,   -99,   -99,    93,
     -99,   248,   248,   248,   253,   245,   -99,   -99,   -99,   -99,
     -99,   -99,   -99,    11,   -99,   -99,   -99,    14,   -99,   -99,
     154,    70,   115,   115,   261,   248,   248,   248,   248,   248,
     -99,   -99,   119,   226,   162,    20,    17,   -99,   248,   -99,
     186,   -99,   117,   117,   115,   115,   115,   131,   121,   245,
     245,   143,   245,   -99,   -99,   -99,   -99,   -99,   137,   129,
     146,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     148,   -99,   -99,   269,   -99,   248,    11,   -99,   -99,   245,
     -99,   -99,   133,   -99,    14,   -99,   -99,   -99,   -99,   -99,
     -99,   -99,   -99,   -99,   -99,   -99,   277,   -99,   110,   245,
     -99,    54,   -99,   -99,   -99,   -99,    61,   -99,   -99
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -99,   -99,   -99,   -99,   -99,   -99,   -99,   -99,   -99,   -99,
     -99,   -99,   -99,     7,   -99,   -99,   -99,   -99,    -4,   -99,
     -44,   -99,   -99,   -99,    -9,   -98,    76,   -99,   182,   -99,
     -99,   -46,   -15,   -37,   -81
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -51
static const yytype_int16 yytable[] =
{
      27,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,   120,    85,    86,    60,   135,
     122,   123,   124,   161,    18,   162,   163,   164,   165,   166,
     167,   168,   169,    99,    91,   100,    92,   105,   106,    19,
     140,    93,   101,   102,   142,   143,   144,   145,   146,   -31,
      25,    94,   170,    26,   103,   119,    21,   173,    22,   114,
      20,   115,    87,   171,    88,   -28,    28,   161,   131,   162,
     163,   164,   165,   166,   167,   168,   169,     2,   112,    89,
       3,     4,     5,    91,    24,    92,   202,   155,   155,    87,
      93,    88,    35,    36,   196,     6,   170,    61,    84,    99,
     207,   100,    97,    96,   132,     7,   111,   208,   101,   102,
       8,     9,   178,   179,    98,   181,   107,   108,   156,   156,
     103,   139,   109,   110,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   198,    37,    38,    39,    40,   113,    87,
     117,    88,   199,   121,   118,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,   204,   127,   128,   129,
     176,   129,   205,    52,   -50,    53,   177,   197,    37,    38,
      39,    40,   180,   182,   183,   184,    58,   194,   200,   149,
     150,    43,    44,    45,    46,    47,    48,    49,    50,    51,
     201,   134,   125,   126,   127,   128,   206,   151,   129,   152,
      37,    38,    39,    40,    32,   137,   138,     0,     0,     0,
     157,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,     0,     0,   125,   126,   127,   128,     0,    52,
     129,    53,    37,    38,    39,    40,     0,   174,   175,     0,
      54,     0,     0,   149,   150,    43,    44,    45,    46,    47,
      48,    49,    50,    51,     0,     0,     0,     0,     0,     0,
       0,   151,     0,   152,    44,    45,    46,    47,    48,    49,
      50,    51,   153,     0,    64,     0,    65,    99,     0,   100,
       0,     0,     0,    66,    67,     0,   101,   102,     0,     0,
      59,   125,   126,   127,   128,    68,     0,   129,   103,   125,
     126,   127,   128,     0,   130,   129,     0,   125,   126,   127,
     128,     0,   141,   129,     0,   125,   126,   127,   128,     0,
     195,   129,     0,     0,     0,     0,     0,     0,   203
};

static const yytype_int16 yycheck[] =
{
       9,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    96,    53,    61,    33,   117,
     101,   102,   103,     6,    29,     8,     9,    10,    11,    12,
      13,    14,    15,    29,    29,    31,    31,    74,    75,    29,
     121,    36,    38,    39,   125,   126,   127,   128,   129,    29,
      29,    46,    35,    32,    50,    51,    45,   138,    47,    45,
      29,    47,    29,    46,    31,    45,    31,     6,   105,     8,
       9,    10,    11,    12,    13,    14,    15,     0,    87,    46,
       3,     4,     5,    29,    45,    31,   184,   133,   134,    29,
      36,    31,    45,    45,   175,    18,    35,    45,    29,    29,
      46,    31,    31,    50,   113,    28,    46,    46,    38,    39,
      33,    34,   149,   150,    31,   152,    49,    49,   133,   134,
      50,    51,    49,    49,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   177,     6,     7,     8,     9,    40,    29,
      40,    31,   179,    50,    45,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    46,    40,    41,    44,
      29,    44,   199,    35,    45,    37,    45,   176,     6,     7,
       8,     9,    29,    36,    45,    29,    48,    29,    45,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
     183,   115,    38,    39,    40,    41,   200,    35,    44,    37,
       6,     7,     8,     9,    22,    51,    52,    -1,    -1,    -1,
      48,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    -1,    38,    39,    40,    41,    -1,    35,
      44,    37,     6,     7,     8,     9,    -1,    51,    52,    -1,
      46,    -1,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    37,    20,    21,    22,    23,    24,    25,
      26,    27,    46,    -1,    29,    -1,    31,    29,    -1,    31,
      -1,    -1,    -1,    38,    39,    -1,    38,    39,    -1,    -1,
      46,    38,    39,    40,    41,    50,    -1,    44,    50,    38,
      39,    40,    41,    -1,    51,    44,    -1,    38,    39,    40,
      41,    -1,    51,    44,    -1,    38,    39,    40,    41,    -1,
      51,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    54,     0,     3,     4,     5,    18,    28,    33,    34,
      55,    56,    57,    58,    59,    60,    61,    64,    29,    29,
      29,    45,    47,    77,    45,    29,    32,    77,    31,    62,
      65,    81,    81,    82,    63,    45,    45,     6,     7,     8,
       9,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    35,    37,    46,    83,    84,    85,    48,    46,
      85,    45,    73,    66,    29,    31,    38,    39,    50,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    29,    86,    73,    29,    31,    46,
      74,    29,    31,    36,    46,    67,    50,    31,    31,    29,
      31,    38,    39,    50,    87,    86,    86,    49,    49,    49,
      49,    46,    77,    40,    45,    47,    78,    40,    45,    51,
      87,    50,    87,    87,    87,    38,    39,    40,    41,    44,
      51,    86,    77,    79,    79,    78,    71,    51,    52,    51,
      87,    51,    87,    87,    87,    87,    87,    75,    76,    17,
      18,    35,    37,    46,    80,    84,    85,    48,    68,    69,
      70,     6,     8,     9,    10,    11,    12,    13,    14,    15,
      35,    46,    72,    87,    51,    52,    29,    45,    86,    86,
      29,    86,    36,    45,    29,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    29,    51,    87,    77,    73,    86,
      45,    66,    78,    51,    46,    86,    71,    46,    46
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 11:
#line 96 "../../src-common/cfdg.y"
    {
            yg_IncludeFile((yyvsp[(2) - (2)].string));
        }
    break;

  case 12:
#line 100 "../../src-common/cfdg.y"
    {
            yg_IncludeFile((yyvsp[(2) - (2)].string));
        }
    break;

  case 13:
#line 106 "../../src-common/cfdg.y"
    {
            yg_Initialize((yyvsp[(2) - (2)].string));
        }
    break;

  case 14:
#line 111 "../../src-common/cfdg.y"
    {
            yg_Background();
        }
    break;

  case 15:
#line 117 "../../src-common/cfdg.y"
    {
            const char *errmsg = yg_Tile();
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
        }
    break;

  case 16:
#line 127 "../../src-common/cfdg.y"
    {
            const char *errmsg = yg_SizeLine();
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
        }
    break;

  case 17:
#line 137 "../../src-common/cfdg.y"
    {
            const char *errmsg = yg_CheckRule((yyvsp[(2) - (2)].string), 0);
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
        }
    break;

  case 18:
#line 143 "../../src-common/cfdg.y"
    {
            yg_Rule((yyvsp[(2) - (6)].string), 1.0);
        }
    break;

  case 19:
#line 147 "../../src-common/cfdg.y"
    {
            const char *errmsg = yg_CheckRule((yyvsp[(2) - (3)].string), 0);
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
            if ((yyvsp[(3) - (3)].real) <= 0.0) {
                yyerror("Rule weights must be positive.");
                YYABORT;
            }
            yg_ClearTransform(); 
        }
    break;

  case 20:
#line 158 "../../src-common/cfdg.y"
    {
            yg_Rule((yyvsp[(2) - (7)].string), (yyvsp[(3) - (7)].real));
        }
    break;

  case 21:
#line 164 "../../src-common/cfdg.y"
    {
            const char *errmsg = yg_CheckRule((yyvsp[(2) - (2)].string), 1);
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
        }
    break;

  case 22:
#line 170 "../../src-common/cfdg.y"
    {
            yg_Path((yyvsp[(2) - (6)].string), 1.0);
        }
    break;

  case 25:
#line 181 "../../src-common/cfdg.y"
    {
            const char *errmsg = yg_PathOp((yyvsp[(1) - (4)].string));
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
        }
    break;

  case 26:
#line 189 "../../src-common/cfdg.y"
    { 
            const char *errmsg = yg_SaveTransform();
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
            yg_PathLoopStart();
        }
    break;

  case 27:
#line 196 "../../src-common/cfdg.y"
    {
            const char *errmsg = yg_PathOp((yyvsp[(5) - (8)].string));
            if (!errmsg) errmsg = yg_PathLoopEnd((int)(yyvsp[(1) - (8)].real));
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
        }
    break;

  case 28:
#line 205 "../../src-common/cfdg.y"
    { 
            const char *errmsg = yg_SaveTransform();
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
            yg_PathLoopStart();
        }
    break;

  case 29:
#line 212 "../../src-common/cfdg.y"
    {
            const char *errmsg = yg_PathLoopEnd((int)(yyvsp[(1) - (7)].real));
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
        }
    break;

  case 30:
#line 220 "../../src-common/cfdg.y"
    {
            const char *errmsg = yg_PathCmd((yyvsp[(1) - (2)].string));
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
        }
    break;

  case 31:
#line 228 "../../src-common/cfdg.y"
    { 
            const char *errmsg = yg_SaveTransform();
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
            yg_PathLoopStart();
        }
    break;

  case 32:
#line 235 "../../src-common/cfdg.y"
    {
            const char *errmsg = yg_PathCmd((yyvsp[(5) - (6)].string));
            if (errmsg) {
                yyerror(errmsg);
                YYABORT;
            }
            yg_PathLoopEnd((int)(yyvsp[(1) - (6)].real));
        }
    break;

  case 35:
#line 251 "../../src-common/cfdg.y"
    {
            yg_PointX((yyvsp[(2) - (2)].real), 0);
        }
    break;

  case 36:
#line 255 "../../src-common/cfdg.y"
    {
            yg_PointX((yyvsp[(2) - (2)].real), 1);
        }
    break;

  case 37:
#line 259 "../../src-common/cfdg.y"
    {
            yg_PointX((yyvsp[(2) - (2)].real), 2);
        }
    break;

  case 38:
#line 263 "../../src-common/cfdg.y"
    {
            yg_PointY((yyvsp[(2) - (2)].real), 0);
        }
    break;

  case 39:
#line 267 "../../src-common/cfdg.y"
    {
            yg_PointY((yyvsp[(2) - (2)].real), 1);
        }
    break;

  case 40:
#line 271 "../../src-common/cfdg.y"
    {
            yg_PointY((yyvsp[(2) - (2)].real), 2);
        }
    break;

  case 41:
#line 275 "../../src-common/cfdg.y"
    {
            yg_PointX((yyvsp[(2) - (2)].real), 3);
        }
    break;

  case 42:
#line 279 "../../src-common/cfdg.y"
    {
            yg_PointY((yyvsp[(2) - (2)].real), 3);
        }
    break;

  case 43:
#line 283 "../../src-common/cfdg.y"
    {
            yg_PointX((yyvsp[(2) - (2)].real), 4);
        }
    break;

  case 44:
#line 287 "../../src-common/cfdg.y"
    {
            yg_Parameters((yyvsp[(2) - (2)].string));
        }
    break;

  case 47:
#line 298 "../../src-common/cfdg.y"
    {
            yg_Replacement((yyvsp[(1) - (2)].string));
        }
    break;

  case 48:
#line 302 "../../src-common/cfdg.y"
    { 
            yg_SaveTransform(); 
            yg_LoopStart();
        }
    break;

  case 49:
#line 305 "../../src-common/cfdg.y"
    {
            yg_Replacement((yyvsp[(5) - (6)].string));
            yg_LoopEnd((int)(yyvsp[(1) - (6)].real));
        }
    break;

  case 50:
#line 310 "../../src-common/cfdg.y"
    { 
            yg_SaveTransform(); 
            yg_LoopStart();
        }
    break;

  case 51:
#line 313 "../../src-common/cfdg.y"
    {
            yg_LoopEnd((int)(yyvsp[(1) - (7)].real));
        }
    break;

  case 52:
#line 319 "../../src-common/cfdg.y"
    {
            yg_DefTransform();
        }
    break;

  case 53:
#line 323 "../../src-common/cfdg.y"
    {
        }
    break;

  case 54:
#line 328 "../../src-common/cfdg.y"
    {
            yg_DefTransform();
        }
    break;

  case 55:
#line 332 "../../src-common/cfdg.y"
    {
        }
    break;

  case 60:
#line 346 "../../src-common/cfdg.y"
    {
            yg_Parameters((yyvsp[(2) - (2)].string));
        }
    break;

  case 61:
#line 350 "../../src-common/cfdg.y"
    {
            yg_StrokeWidth((yyvsp[(2) - (2)].real));
        }
    break;

  case 62:
#line 354 "../../src-common/cfdg.y"
    {
            yyerror("Z changes are not allowed in paths");
            YYABORT;
        }
    break;

  case 63:
#line 359 "../../src-common/cfdg.y"
    {
            yyerror("Z changes are not allowed in paths");
            YYABORT;
        }
    break;

  case 70:
#line 380 "../../src-common/cfdg.y"
    {
            yg_ZLocation((yyvsp[(2) - (2)].real));
        }
    break;

  case 71:
#line 384 "../../src-common/cfdg.y"
    {
            yg_Size((yyvsp[(2) - (4)].real), (yyvsp[(3) - (4)].real), (yyvsp[(4) - (4)].real));
        }
    break;

  case 72:
#line 388 "../../src-common/cfdg.y"
    {
            yyerror("Parameters are not allowed in rules");
            YYABORT;
        }
    break;

  case 73:
#line 393 "../../src-common/cfdg.y"
    {
            yyerror("Stroke widths are not allowed in rules");
            YYABORT;
        }
    break;

  case 74:
#line 400 "../../src-common/cfdg.y"
    {
            yg_Orientation((yyvsp[(2) - (2)].real));
        }
    break;

  case 75:
#line 404 "../../src-common/cfdg.y"
    {
            yg_Reflection((yyvsp[(2) - (2)].real));
        }
    break;

  case 76:
#line 408 "../../src-common/cfdg.y"
    {
            yg_XLocation((yyvsp[(2) - (2)].real));
        }
    break;

  case 77:
#line 412 "../../src-common/cfdg.y"
    {
            yg_YLocation((yyvsp[(2) - (2)].real));
        }
    break;

  case 78:
#line 416 "../../src-common/cfdg.y"
    {
            yg_Size((yyvsp[(2) - (2)].real), (yyvsp[(2) - (2)].real), 1.0);
        }
    break;

  case 79:
#line 420 "../../src-common/cfdg.y"
    {
            yg_Size((yyvsp[(2) - (3)].real), (yyvsp[(3) - (3)].real), 1.0);
        }
    break;

  case 80:
#line 424 "../../src-common/cfdg.y"
    {
            yg_Skew((yyvsp[(2) - (3)].real), (yyvsp[(3) - (3)].real));
        }
    break;

  case 81:
#line 430 "../../src-common/cfdg.y"
    {
            yg_Hue((yyvsp[(2) - (2)].real), 0);
        }
    break;

  case 82:
#line 434 "../../src-common/cfdg.y"
    {
            yg_Saturation((yyvsp[(2) - (2)].real), 0);
        }
    break;

  case 83:
#line 438 "../../src-common/cfdg.y"
    {
            yg_Brightness((yyvsp[(2) - (2)].real), 0);
        }
    break;

  case 84:
#line 442 "../../src-common/cfdg.y"
    {
            yg_Alpha((yyvsp[(2) - (2)].real), 0);
        }
    break;

  case 85:
#line 446 "../../src-common/cfdg.y"
    {
            yg_Entropy("|");
            yg_Hue((yyvsp[(2) - (3)].real), -1);
        }
    break;

  case 86:
#line 451 "../../src-common/cfdg.y"
    {
            yg_Entropy("|");
            yg_Saturation((yyvsp[(2) - (3)].real), -1);
        }
    break;

  case 87:
#line 456 "../../src-common/cfdg.y"
    {
            yg_Entropy("|");
            yg_Brightness((yyvsp[(2) - (3)].real), -1);
        }
    break;

  case 88:
#line 461 "../../src-common/cfdg.y"
    {
            yg_Entropy("|");
            yg_Alpha((yyvsp[(2) - (3)].real), -1);
        }
    break;

  case 89:
#line 466 "../../src-common/cfdg.y"
    {
            yg_HueTarget((yyvsp[(2) - (2)].real), 0);
        }
    break;

  case 90:
#line 470 "../../src-common/cfdg.y"
    {
            yg_SaturationTarget((yyvsp[(2) - (2)].real), 0);
        }
    break;

  case 91:
#line 474 "../../src-common/cfdg.y"
    {
            yg_BrightnessTarget((yyvsp[(2) - (2)].real), 0);
        }
    break;

  case 92:
#line 478 "../../src-common/cfdg.y"
    {
            yg_AlphaTarget((yyvsp[(2) - (2)].real), 0);
        }
    break;

  case 93:
#line 484 "../../src-common/cfdg.y"
    { (yyval.real) = (yyvsp[(1) - (1)].real); }
    break;

  case 94:
#line 486 "../../src-common/cfdg.y"
    { (yyval.real) = -(yyvsp[(2) - (2)].real); yg_NegEntropy(); }
    break;

  case 95:
#line 488 "../../src-common/cfdg.y"
    { (yyval.real) = (yyvsp[(2) - (2)].real); }
    break;

  case 96:
#line 490 "../../src-common/cfdg.y"
    { (yyval.real) = (yyvsp[(2) - (3)].real); yg_Entropy("()"); }
    break;

  case 97:
#line 492 "../../src-common/cfdg.y"
    { 
            int ftype = yg_functionType((yyvsp[(1) - (3)].string), 0);
            yg_FreeString((yyvsp[(1) - (3)].string)); 
            if (ftype == -1) {
                YYABORT;
            }
            (yyval.real) = yg_function(ftype, 0.0, 0.0); 
        }
    break;

  case 98:
#line 501 "../../src-common/cfdg.y"
    { 
            int ftype = yg_functionType((yyvsp[(1) - (4)].string), 1);
            yg_FreeString((yyvsp[(1) - (4)].string)); 
            if (ftype == -1) {
                YYABORT;
            }
            (yyval.real) = yg_function(ftype, (yyvsp[(3) - (4)].real), 0.0); 
        }
    break;

  case 99:
#line 510 "../../src-common/cfdg.y"
    {
            int ftype = yg_functionType((yyvsp[(1) - (6)].string), 2);
            yg_FreeString((yyvsp[(1) - (6)].string)); 
            if (ftype == -1) {
                YYABORT;
            }
            (yyval.real) = yg_function(ftype, (yyvsp[(3) - (6)].real), (yyvsp[(5) - (6)].real)); 
        }
    break;

  case 100:
#line 521 "../../src-common/cfdg.y"
    { (yyval.real) = (yyvsp[(1) - (1)].real);         }
    break;

  case 101:
#line 523 "../../src-common/cfdg.y"
    { 
            int ftype = yg_functionType((yyvsp[(1) - (3)].string), 0);
            yg_FreeString((yyvsp[(1) - (3)].string)); 
            if (ftype == -1) {
                YYABORT;
            }
            (yyval.real) = yg_function(ftype, 0.0, 0.0); 
        }
    break;

  case 102:
#line 532 "../../src-common/cfdg.y"
    { 
            int ftype = yg_functionType((yyvsp[(1) - (4)].string), 1);
            yg_FreeString((yyvsp[(1) - (4)].string)); 
            if (ftype == -1) {
                YYABORT;
            }
            (yyval.real) = yg_function(ftype, (yyvsp[(3) - (4)].real), 0.0); 
        }
    break;

  case 103:
#line 541 "../../src-common/cfdg.y"
    {
            int ftype = yg_functionType((yyvsp[(1) - (6)].string), 2);
            yg_FreeString((yyvsp[(1) - (6)].string)); 
            if (ftype == -1) {
                YYABORT;
            }
            (yyval.real) = yg_function(ftype, (yyvsp[(3) - (6)].real), (yyvsp[(5) - (6)].real)); 
        }
    break;

  case 104:
#line 550 "../../src-common/cfdg.y"
    { (yyval.real) = (yyvsp[(1) - (3)].real) + (yyvsp[(3) - (3)].real); yg_Entropy("+");   }
    break;

  case 105:
#line 552 "../../src-common/cfdg.y"
    { (yyval.real) = (yyvsp[(1) - (3)].real) - (yyvsp[(3) - (3)].real); yg_Entropy("-"); }
    break;

  case 106:
#line 554 "../../src-common/cfdg.y"
    { (yyval.real) = (yyvsp[(1) - (3)].real) * (yyvsp[(3) - (3)].real); yg_Entropy("*"); }
    break;

  case 107:
#line 556 "../../src-common/cfdg.y"
    { (yyval.real) = (yyvsp[(1) - (3)].real) / (yyvsp[(3) - (3)].real); yg_Entropy("/"); }
    break;

  case 108:
#line 558 "../../src-common/cfdg.y"
    { (yyval.real) = -(yyvsp[(2) - (2)].real); yg_Entropy("NEG"); }
    break;

  case 109:
#line 560 "../../src-common/cfdg.y"
    { (yyval.real) = (yyvsp[(2) - (2)].real); yg_Entropy("POS"); }
    break;

  case 110:
#line 562 "../../src-common/cfdg.y"
    { (yyval.real) = pow ((yyvsp[(1) - (3)].real), (yyvsp[(3) - (3)].real)); yg_Entropy("^"); }
    break;

  case 111:
#line 564 "../../src-common/cfdg.y"
    { (yyval.real) = (yyvsp[(2) - (3)].real); yg_Entropy("()"); }
    break;


/* Line 1267 of yacc.c.  */
#line 2270 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 567 "../../src-common/cfdg.y"





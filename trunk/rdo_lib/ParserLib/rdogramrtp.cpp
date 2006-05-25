/* A Bison parser, made from .\rdortp.y
   by GNU bison 1.34.  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse rtpparse
#define yylex rtplex
#define yyerror rtperror
#define yylval rtplval
#define yychar rtpchar
#define yydebug rtpdebug
#define yynerrs rtpnerrs
#define YYLSP_NEEDED 1

# define	Resource_type	257
# define	permanent	258
# define	Parameters	259
# define	integer	260
# define	real	261
# define	End	262
# define	temporary	263
# define	IDENTIF	264
# define	INT_CONST	265
# define	REAL_CONST	266
# define	such_as	267
# define	dblpoint	268
# define	Resources	269
# define	trace_keyword	270
# define	no_trace	271
# define	IDENTIF_COLON	272
# define	Constant	273
# define	Body	274
# define	Function_keyword	275
# define	Type_keyword	276
# define	algorithmic	277
# define	table_keyword	278
# define	list_keyword	279
# define	Calculate_if	289
# define	Exist	281
# define	Not_Exist	282
# define	For_All	283
# define	Not_For_All	284
# define	neq	285
# define	leq	286
# define	geq	287
# define	NoCheck	288
# define	or_keyword	290
# define	and_keyword	291
# define	Sequence	292
# define	uniform	293
# define	exponential	294
# define	normal_keyword	295
# define	by_hist	296
# define	enumerative	297
# define	Pattern	298
# define	operation	299
# define	irregular_event	300
# define	rule_keyword	301
# define	keyboard	302
# define	Relevant_resources	303
# define	Keep	304
# define	Create_keyword	305
# define	Erase	306
# define	NonExist	307
# define	IDENTIF_NoChange	308
# define	Time	309
# define	Choice	310
# define	from_keyword	311
# define	first_keyword	312
# define	Convert_begin	313
# define	Convert_end	314
# define	Convert_rule	315
# define	Convert_event	316
# define	with_max	317
# define	with_min	318
# define	IDENTIF_set	319
# define	IDENTIF_NoChange_NoChange	320
# define	Operations	321
# define	Results	322
# define	watch_par	323
# define	watch_state	324
# define	watch_quant	325
# define	watch_value	326
# define	get_value	327
# define	Model_name	328
# define	Resource_file	329
# define	OprIev_file	330
# define	Frame_file	331
# define	Statistic_file	332
# define	Results_file	333
# define	Trace_file	334
# define	Show_mode	335
# define	Frame_number	336
# define	Show_rate	337
# define	Run_StartTime	338
# define	Trace_StartTime	339
# define	Trace_EndTime	340
# define	Terminate_if	341
# define	Break_point	342
# define	Seed	343
# define	NoShow	344
# define	Monitor	345
# define	Animation	346
# define	NoChange	347
# define	Decision_point	348
# define	search_keyword	349
# define	trace_stat	350
# define	trace_tops	351
# define	trace_all	352
# define	Condition_keyword	353
# define	Term_condition	354
# define	Evaluate_by	355
# define	Compare_tops	356
# define	NO	357
# define	YES	358
# define	Activities	359
# define	value_before	360
# define	value_after	361
# define	some	362
# define	Process	363
# define	SEIZE	364
# define	GENERATE	365
# define	Frame	400
# define	Show_if	401
# define	Back_picture	402
# define	Show	403
# define	frm_cell	404
# define	text	405
# define	transparent	406
# define	bitmap	407
# define	s_bmp	408
# define	rect_keyword	409
# define	r_rect	410
# define	line	411
# define	ellipse	412
# define	triang	413
# define	active	414
# define	QUOTED_IDENTIF	415
# define	QUOTED_IDENTIF_BAD	416
# define	IDENTIF_BAD	417

#line 1 ".\\rdortp.y"

#define YYPARSE_PARAM lexer
#define YYLEX_PARAM lexer
#line 141 ".\\rdortp.y"

#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoparser.h"
#include "rdoparser_rdo.h"
#include "rdortp.h"

namespace rdoParse
{
#ifndef YYSTYPE
#define YYSTYPE int
#endif

#ifndef YYLTYPE
typedef struct yyltype
{
  int first_line;
  int first_column;

  int last_line;
  int last_column;
} yyltype;

# define YYLTYPE yyltype
#endif

#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		106
#define	YYFLAG		-32768
#define	YYNTBASE	136

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 417 ? yytranslate[x] : 151)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     132,   133,     2,     2,   134,     2,   135,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   129,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   130,     2,   131,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
       2,    27,    28,    29,    30,    31,    32,    33,    34,    26,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     1,     4,     6,    11,    15,    18,    22,    25,
      29,    31,    33,    34,    37,    40,    43,    45,    49,    53,
      56,    58,    62,    66,    70,    74,    77,    80,    83,    86,
      87,    93,    98,   103,   108,   114,   119,   122,   123,   129,
     135,   141,   147,   153,   159,   165,   171,   176,   181,   184,
     185,   188,   191,   194,   195,   198,   201,   204,   208,   211,
     213,   217,   220,   223,   227,   231,   235,   237,   239,   241,
     243,   244,   247,   250,   255,   259,   264,   268
};
static const short yyrhs[] =
{
      -1,   136,   137,     0,     1,     0,   138,     5,   140,     8,
       0,   138,     5,   140,     0,   138,     1,     0,     3,    18,
     139,     0,     3,     1,     0,     3,    18,     1,     0,     4,
       0,     9,     0,     0,   140,   141,     0,    18,   142,     0,
      18,     1,     0,     1,     0,     6,   143,   145,     0,     7,
     144,   146,     0,   147,   149,     0,   150,     0,   150,   129,
      11,     0,   150,   129,    12,     0,   150,   129,    10,     0,
     150,   129,     1,     0,   150,     1,     0,     6,     1,     0,
       7,     1,     0,   147,     1,     0,     0,   130,    11,    14,
      11,   131,     0,   130,    12,    14,    12,     0,   130,    12,
      14,    11,     0,   130,    11,    14,    12,     0,   130,    11,
      14,    11,     1,     0,   130,    11,    14,     1,     0,   130,
       1,     0,     0,   130,    12,    14,    12,   131,     0,   130,
      12,    14,    11,   131,     0,   130,    11,    14,    12,   131,
       0,   130,    11,    14,    11,   131,     0,   130,    12,    14,
      12,     1,     0,   130,    12,    14,    11,     1,     0,   130,
      11,    14,    12,     1,     0,   130,    11,    14,    11,     1,
       0,   130,    12,    14,     1,     0,   130,    11,    14,     1,
       0,   130,     1,     0,     0,   129,    11,     0,   129,    12,
       0,   129,     1,     0,     0,   129,    12,     0,   129,    11,
       0,   129,     1,     0,   132,   148,   133,     0,   132,   148,
       0,    10,     0,   148,   134,    10,     0,   148,    10,     0,
     148,     1,     0,   148,   134,    11,     0,   148,   134,    12,
       0,   148,   134,   128,     0,    11,     0,    12,     0,   128,
       0,     1,     0,     0,   129,    10,     0,   129,     1,     0,
      13,    10,   135,    10,     0,    13,    10,   135,     0,    13,
      10,   135,     1,     0,    13,    10,     1,     0,    13,     1,
       0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   161,   162,   163,   168,   174,   178,   182,   192,   197,
     202,   203,   205,   208,   214,   221,   231,   236,   242,   248,
     257,   261,   265,   269,   273,   276,   279,   282,   285,   289,
     293,   297,   301,   305,   309,   313,   317,   323,   327,   333,
     339,   345,   351,   355,   359,   363,   367,   371,   375,   381,
     384,   387,   392,   408,   411,   414,   417,   433,   436,   441,
     446,   458,   465,   475,   479,   483,   487,   491,   495,   499,
     504,   507,   511,   527,   542,   553,   564,   575
};
#endif


#if (YYDEBUG) || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const yytname[] =
{
  "$", "error", "$undefined.", "Resource_type", "permanent", "Parameters", 
  "integer", "real", "End", "temporary", "IDENTIF", "INT_CONST", 
  "REAL_CONST", "such_as", "dblpoint", "Resources", "trace_keyword", 
  "no_trace", "IDENTIF_COLON", "Constant", "Body", "Function_keyword", 
  "Type_keyword", "algorithmic", "table_keyword", "list_keyword", 
  "Calculate_if", "Exist", "Not_Exist", "For_All", "Not_For_All", "neq", 
  "leq", "geq", "NoCheck", "or_keyword", "and_keyword", "Sequence", 
  "uniform", "exponential", "normal_keyword", "by_hist", "enumerative", 
  "Pattern", "operation", "irregular_event", "rule_keyword", "keyboard", 
  "Relevant_resources", "Keep", "Create_keyword", "Erase", "NonExist", 
  "IDENTIF_NoChange", "Time", "Choice", "from_keyword", "first_keyword", 
  "Convert_begin", "Convert_end", "Convert_rule", "Convert_event", 
  "with_max", "with_min", "IDENTIF_set", "IDENTIF_NoChange_NoChange", 
  "Operations", "Results", "watch_par", "watch_state", "watch_quant", 
  "watch_value", "get_value", "Model_name", "Resource_file", 
  "OprIev_file", "Frame_file", "Statistic_file", "Results_file", 
  "Trace_file", "Show_mode", "Frame_number", "Show_rate", "Run_StartTime", 
  "Trace_StartTime", "Trace_EndTime", "Terminate_if", "Break_point", 
  "Seed", "NoShow", "Monitor", "Animation", "NoChange", "Decision_point", 
  "search_keyword", "trace_stat", "trace_tops", "trace_all", 
  "Condition_keyword", "Term_condition", "Evaluate_by", "Compare_tops", 
  "NO", "YES", "Activities", "value_before", "value_after", "some", 
  "Process", "SEIZE", "GENERATE", "Frame", "Show_if", "Back_picture", 
  "Show", "frm_cell", "text", "transparent", "bitmap", "s_bmp", 
  "rect_keyword", "r_rect", "line", "ellipse", "triang", "active", 
  "QUOTED_IDENTIF", "QUOTED_IDENTIF_BAD", "IDENTIF_BAD", "'='", "'['", 
  "']'", "'('", "')'", "','", "'.'", "rtp_list", "rtp_res_type", 
  "rtp_res_type_hdr", "rtp_vid_res", "rtp_body", "rtp_param_desc", 
  "rtp_param_type", "rtp_int_diap", "rtp_real_diap", 
  "rtp_int_default_val", "rtp_real_default_val", "rtp_enum", 
  "rtp_enum_list", "rtp_enum_default_val", "rtp_such_as", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,   136,   136,   136,   137,   137,   137,   138,   138,   138,
     139,   139,   140,   140,   141,   141,   141,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   143,
     143,   143,   143,   143,   143,   143,   143,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   145,
     145,   145,   145,   146,   146,   146,   146,   147,   147,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     149,   149,   149,   150,   150,   150,   150,   150
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     0,     2,     1,     4,     3,     2,     3,     2,     3,
       1,     1,     0,     2,     2,     2,     1,     3,     3,     2,
       1,     3,     3,     3,     3,     2,     2,     2,     2,     0,
       5,     4,     4,     4,     5,     4,     2,     0,     5,     5,
       5,     5,     5,     5,     5,     5,     4,     4,     2,     0,
       2,     2,     2,     0,     2,     2,     2,     3,     2,     1,
       3,     2,     2,     3,     3,     3,     1,     1,     1,     1,
       0,     2,     2,     4,     3,     4,     3,     2
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       0,     3,     0,     0,     2,     0,     8,     0,     6,    12,
       9,    10,    11,     7,     0,    16,     4,     0,    13,    15,
       0,     0,     0,     0,    14,     0,     0,    26,     0,    49,
      27,     0,    53,    77,     0,    69,    59,    66,    67,    68,
       0,    28,     0,    19,    25,     0,    36,     0,     0,     0,
      17,    48,     0,     0,     0,    18,    76,     0,    62,    61,
      57,     0,    72,    71,    24,    23,    21,    22,     0,     0,
      52,    50,    51,     0,     0,    56,    55,    54,    75,    73,
      60,    63,    64,    65,    35,     0,    33,    32,    31,    47,
       0,     0,    46,     0,     0,    34,    30,    45,    41,    44,
      40,    43,    39,    42,    38,     0,     0
};

static const short yydefgoto[] =
{
       2,     4,     5,    13,    14,    18,    24,    29,    32,    50,
      55,    25,    40,    43,    26
};

static const short yypact[] =
{
     103,-32768,    68,    32,-32768,    30,-32768,   101,-32768,-32768,
  -32768,-32768,-32768,-32768,    61,-32768,-32768,     6,-32768,-32768,
      14,    20,    90,    48,-32768,    39,    45,-32768,    40,  -125,
  -32768,    66,  -123,-32768,     1,-32768,-32768,-32768,-32768,-32768,
       0,-32768,    91,-32768,-32768,    64,-32768,    11,    16,    69,
  -32768,-32768,    29,    52,    71,-32768,-32768,    26,-32768,-32768,
  -32768,    44,-32768,-32768,-32768,-32768,-32768,-32768,    83,    78,
  -32768,-32768,-32768,    85,    87,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,     4,-32768,-32768,-32768,-32768,
       8,    10,-32768,    15,    23,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,    37,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768
};


#define	YYLAST		176


static const short yytable[] =
{
     -58,    58,    56,   -58,    49,    95,    54,    19,   -58,    97,
      59,    99,    20,    21,   -29,    27,   101,   -29,   -58,    22,
     -37,    30,   -29,   -37,   103,    68,   -74,    78,   -37,   -74,
      69,     8,   -29,     6,   -74,     9,    79,   106,   -37,   -70,
      41,    46,   -70,    73,   -74,   -20,    44,   -70,   -20,    35,
       7,    47,    48,   -20,    80,    81,    82,   -70,    36,    37,
      38,    -5,    15,   -20,    -5,    64,    74,    51,   105,    16,
      70,     3,    75,     0,    65,    66,    67,    52,    53,    17,
      71,    72,    76,    77,    84,     0,    89,     0,    92,    87,
      88,    33,    62,     0,    85,    86,    90,    91,    93,    94,
      34,    63,    10,    -1,     1,    11,    -1,     0,     0,     0,
      12,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   -58,
       0,     0,     0,    60,    61,    96,    57,     0,    23,    98,
       0,   100,     0,   -29,    28,     0,   102,     0,     0,   -37,
      31,     0,     0,     0,   104,   -74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    42,     0,
       0,     0,    83,     0,    45,     0,    39
};

static const short yycheck[] =
{
       0,     1,     1,     3,   129,     1,   129,     1,     8,     1,
      10,     1,     6,     7,     0,     1,     1,     3,    18,    13,
       0,     1,     8,     3,     1,    14,     0,     1,     8,     3,
      14,     1,    18,     1,     8,     5,    10,     0,    18,     0,
       1,     1,     3,    14,    18,     0,     1,     8,     3,     1,
      18,    11,    12,     8,    10,    11,    12,    18,    10,    11,
      12,     0,     1,    18,     3,     1,    14,     1,     0,     8,
       1,     3,     1,    -1,    10,    11,    12,    11,    12,    18,
      11,    12,    11,    12,     1,    -1,     1,    -1,     1,    11,
      12,     1,     1,    -1,    11,    12,    11,    12,    11,    12,
      10,    10,     1,     0,     1,     4,     3,    -1,    -1,    -1,
       9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
      -1,    -1,    -1,   133,   134,   131,   135,    -1,   132,   131,
      -1,   131,    -1,   129,   130,    -1,   131,    -1,    -1,   129,
     130,    -1,    -1,    -1,   131,   129,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    -1,   128,    -1,   129,    -1,   128
};
#define YYPURE 1

/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "..\\System\\bison.simple"

/* Skeleton output parser for bison,

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software
   Foundation, Inc.

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
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser when
   the %semantic_parser declaration is not specified in the grammar.
   It was written by Richard Stallman by simplifying the hairy parser
   used when %semantic_parser is specified.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

#if ! defined (yyoverflow) || defined (YYERROR_VERBOSE)

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
# if YYLSP_NEEDED
  YYLTYPE yyls;
# endif
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# if YYLSP_NEEDED
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAX)
# else
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)
# endif

/* Relocate the TYPE STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Type, Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	yymemcpy ((char *) yyptr, (char *) (Stack),			\
		  yysize * (YYSIZE_T) sizeof (Type));			\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (Type) + YYSTACK_GAP_MAX;	\
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif /* ! defined (yyoverflow) || defined (YYERROR_VERBOSE) */


#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
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
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).

   When YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#if YYPURE
# if YYLSP_NEEDED
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, &yylloc, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval, &yylloc)
#  endif
# else /* !YYLSP_NEEDED */
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval)
#  endif
# endif /* !YYLSP_NEEDED */
#else /* !YYPURE */
# define YYLEX			yylex ()
#endif /* !YYPURE */


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
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

#if ! defined (yyoverflow) && ! defined (yymemcpy)
# if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#  define yymemcpy __builtin_memcpy
# else				/* not GNU C or C++ */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
#  if defined (__STDC__) || defined (__cplusplus)
yymemcpy (char *yyto, const char *yyfrom, YYSIZE_T yycount)
#  else
yymemcpy (yyto, yyfrom, yycount)
     char *yyto;
     const char *yyfrom;
     YYSIZE_T yycount;
#  endif
{
  register const char *yyf = yyfrom;
  register char *yyt = yyto;
  register YYSIZE_T yyi = yycount;

  while (yyi-- != 0)
    *yyt++ = *yyf++;
}
# endif
#endif

#ifdef YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif
#endif

#line 319 "..\\System\\bison.simple"


/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif

/* YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to YYPARSE.  */

#define YY_DECL_NON_LSP_VARIABLES			\
/* The lookahead symbol.  */				\
int yychar;						\
							\
/* The semantic value of the lookahead symbol. */	\
YYSTYPE yylval;						\
							\
/* Number of parse errors so far.  */			\
int yynerrs;

#if YYLSP_NEEDED
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES			\
						\
/* Location data for the lookahead symbol.  */	\
YYLTYPE yylloc;
#else
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !YYPURE
YY_DECL_VARIABLES
#endif  /* !YYPURE */

int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if YYPURE
  YY_DECL_VARIABLES
#endif  /* !YYPURE */

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yychar1 = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack. */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

#if YYLSP_NEEDED
  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
#endif

#if YYLSP_NEEDED
# define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
# define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  YYSIZE_T yystacksize = YYINITDEPTH;


  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
#if YYLSP_NEEDED
  YYLTYPE yyloc;
#endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
  int yylen;

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
#if YYLSP_NEEDED
  yylsp = yyls;
#endif
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  */
# if YYLSP_NEEDED
	YYLTYPE *yyls1 = yyls;
	/* This used to be a conditional around just the two extra args,
	   but that might be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
# else
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);
# endif
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (short, yyss);
	YYSTACK_RELOCATE (YYSTYPE, yyvs);
# if YYLSP_NEEDED
	YYSTACK_RELOCATE (YYLTYPE, yyls);
# endif
# undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
#if YYLSP_NEEDED
      yylsp = yyls + yysize - 1;
#endif

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

#if YYDEBUG
     /* We have to keep this `#if YYDEBUG', since we use variables
	which are defined only if `YYDEBUG' is set.  */
      if (yydebug)
	{
	  YYFPRINTF (stderr, "Next token is %d (%s",
		     yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
# endif
	  YYFPRINTF (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      yychar, yytname[yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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

     Otherwise, the following line sets YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

#if YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  yyloc = yylsp[1-yylen];
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
#endif

#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] > 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif

  switch (yyn) {

case 3:
#line 163 ".\\rdortp.y"
{
				currParser->lexer_loc_set( &(yylsp[0]) );
				currParser->error( rdosim::RDOSyntaxError::UNKNOWN );
			;
    break;}
case 4:
#line 168 ".\\rdortp.y"
{
					if ( yyvsp[-1] == 0 ) {
						currParser->lexer_loc_set( &(yylsp[-2]) );
						currParser->warning( rdo::format( "Тип ресурса '%s' не содежит параметров", ((RDORTPResType*)yyvsp[-3])->getName()->c_str() ) );
					}
				;
    break;}
case 5:
#line 174 ".\\rdortp.y"
{
					currParser->lexer_loc_set( yylsp[0].last_line, yylsp[0].last_column );
					currParser->error( "Не найдено ключевое слово $End" );
				;
    break;}
case 6:
#line 178 ".\\rdortp.y"
{
					currParser->error( "Не найдено ключевое слово $Parameters" );
				;
    break;}
case 7:
#line 182 ".\\rdortp.y"
{
						reinterpret_cast<RDOLexerRTP*>(lexer)->enum_param_cnt = 0;
						std::string *name = (std::string *)yyvsp[-1];
						if( currParser->findRTPResType(name) ) {
							currParser->lexer_loc_set( yylsp[-1].first_line, yylsp[-1].first_column + name->length() );
							currParser->error( rdosim::RDOSyntaxError::RTP_SECOND_RES_TYPE, name->c_str() );
						}
						RDORTPResType *res = new RDORTPResType( name, yyvsp[0] != 0 );
						yyval = (int)res;
					;
    break;}
case 8:
#line 192 ".\\rdortp.y"
{
						std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
						currParser->lexer_loc_set( &(yylsp[0]) );
						currParser->error( rdo::format("Ошибка в описании имени типа ресурса: %s", str.c_str()) );
					;
    break;}
case 9:
#line 197 ".\\rdortp.y"
{
						currParser->lexer_loc_set( &(yylsp[-1]) );
						currParser->error( "Не указан вид ресурса" );
					;
    break;}
case 10:
#line 202 ".\\rdortp.y"
{ yyval = 1; ;
    break;}
case 11:
#line 203 ".\\rdortp.y"
{ yyval = 0; ;
    break;}
case 12:
#line 205 ".\\rdortp.y"
{
				yyval = 0; // warning
			;
    break;}
case 13:
#line 208 ".\\rdortp.y"
{
				RDORTPParamDesc *param = (RDORTPParamDesc*)yyvsp[0];
				currParser->getLastRTPResType()->add( param );
				yyval = 1; // no warning
			;
    break;}
case 14:
#line 214 ".\\rdortp.y"
{
					currParser->lexer_loc_backup( &(yylsp[-1]) );
					std::string *name = (std::string *)yyvsp[-1];
					RDORTPResParam *parType = (RDORTPResParam *)yyvsp[0];
					RDORTPParamDesc *param = new RDORTPParamDesc(name, parType);
					yyval = (int)param;
				;
    break;}
case 15:
#line 221 ".\\rdortp.y"
{
					if ( currParser->lexer_loc_line() == yylsp[-1].last_line ) {
						std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
						currParser->lexer_loc_set( &(yylsp[0]) );
						currParser->error( rdo::format( "Неверный тип параметра: %s", str.c_str() ) );
					} else {
						currParser->lexer_loc_set( &(yylsp[-1]) );
						currParser->error( "Ожидается тип параметра" );
					}
				;
    break;}
case 16:
#line 231 ".\\rdortp.y"
{
					currParser->lexer_loc_set( &(yylsp[0]) );
					currParser->error( "Неправильное описание параметра" );
				;
    break;}
case 17:
#line 236 ".\\rdortp.y"
{
					RDORTPIntDiap *diap = (RDORTPIntDiap *)yyvsp[-1];
					RDORTPIntDefVal *dv = (RDORTPIntDefVal *)yyvsp[0];
					RDORTPIntResParam *rp = new RDORTPIntResParam(diap, dv);
					yyval = (int)rp;
				;
    break;}
case 18:
#line 242 ".\\rdortp.y"
{
					RDORTPRealDiap *diap = (RDORTPRealDiap *)yyvsp[-1];
					RDORTPRealDefVal *dv = (RDORTPRealDefVal *)yyvsp[0];
					RDORTPRealResParam *rp = new RDORTPRealResParam(diap, dv);
					yyval = (int)rp;
				;
    break;}
case 19:
#line 248 ".\\rdortp.y"
{
					reinterpret_cast<RDOLexerRTP*>(lexer)->enum_param_cnt = 0;
					RDORTPEnum *enu = (RDORTPEnum *)yyvsp[-1];
					RDORTPEnumDefVal *dv = (RDORTPEnumDefVal *)yyvsp[0];
					if(dv->exist)
						enu->findValue(dv->value);	 // if no value - Syntax exception will be thrown
					RDORTPEnumResParam *rp = new RDORTPEnumResParam(enu, dv);
					yyval = (int)rp;
				;
    break;}
case 20:
#line 257 ".\\rdortp.y"
{
					RDORTPParamDesc *desc = (RDORTPParamDesc *)yyvsp[0];
					yyval = (int)desc->getType()->constructSuchAs();
				;
    break;}
case 21:
#line 261 ".\\rdortp.y"
{
					RDORTPParamDesc *desc = (RDORTPParamDesc *)yyvsp[-2];
					yyval = (int)desc->getType()->constructSuchAs((int)yyvsp[0]);
				;
    break;}
case 22:
#line 265 ".\\rdortp.y"
{
					RDORTPParamDesc *desc = (RDORTPParamDesc *)yyvsp[-2];
					yyval = (int)desc->getType()->constructSuchAs((double *)yyvsp[0]);
				;
    break;}
case 23:
#line 269 ".\\rdortp.y"
{
					RDORTPParamDesc *desc = (RDORTPParamDesc *)yyvsp[-2];
					yyval = (int)desc->getType()->constructSuchAs((std::string *)yyvsp[0]);
				;
    break;}
case 24:
#line 273 ".\\rdortp.y"
{
					currParser->error( "Ожидается зачение по-умолчанию" );
				;
    break;}
case 25:
#line 276 ".\\rdortp.y"
{
					currParser->error( "Ожидается окончание описания параметра-ссылки, например, зачение по-умолчанию" );
				;
    break;}
case 26:
#line 279 ".\\rdortp.y"
{
					currParser->error( rdosim::RDOSyntaxError::RTP_WAITING_FOR_INT_PARAM_END );
				;
    break;}
case 27:
#line 282 ".\\rdortp.y"
{
					currParser->error( rdosim::RDOSyntaxError::RTP_WAITING_FOR_REAL_PARAM_END );
				;
    break;}
case 28:
#line 285 ".\\rdortp.y"
{
					currParser->error( rdosim::RDOSyntaxError::RTP_WAITING_FOR_ENUM_PARAM_END );
				;
    break;}
case 29:
#line 289 ".\\rdortp.y"
{
					RDORTPIntDiap *diap = new RDORTPIntDiap();
					yyval = (int)diap;
				;
    break;}
case 30:
#line 293 ".\\rdortp.y"
{
					RDORTPIntDiap *diap = new RDORTPIntDiap(yyvsp[-3], yyvsp[-1]);
					yyval = (int)diap;
				;
    break;}
case 31:
#line 297 ".\\rdortp.y"
{
					currParser->lexer_loc_set( &(yylsp[-2]) );
					currParser->error( rdosim::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				;
    break;}
case 32:
#line 301 ".\\rdortp.y"
{
					currParser->lexer_loc_set( &(yylsp[-2]) );
					currParser->error( rdosim::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				;
    break;}
case 33:
#line 305 ".\\rdortp.y"
{
					currParser->lexer_loc_set( &(yylsp[0]) );
					currParser->error( rdosim::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				;
    break;}
case 34:
#line 309 ".\\rdortp.y"
{
					currParser->lexer_loc_set( &(yylsp[-1]) );
					currParser->error( "Диапазон задан неверно" );
				;
    break;}
case 35:
#line 313 ".\\rdortp.y"
{
					currParser->lexer_loc_set( &(yylsp[-1]) );
					currParser->error( "Диапазон задан неверно" );
				;
    break;}
case 36:
#line 317 ".\\rdortp.y"
{
					currParser->lexer_loc_set( &(yylsp[0]) );
					currParser->error( "Диапазон задан неверно" );
//					currParser->error( rdosim::RDOSyntaxError::RTP_INVALID_RANGE );
				;
    break;}
case 37:
#line 323 ".\\rdortp.y"
{
					RDORTPRealDiap *diap = new RDORTPRealDiap();
					yyval = (int)diap;
				;
    break;}
case 38:
#line 327 ".\\rdortp.y"
{
					double min = *((double *)yyvsp[-3]);
					double max = *((double *)yyvsp[-1]);
					RDORTPRealDiap *diap = new RDORTPRealDiap(min, max);
					yyval = (int)diap;
				;
    break;}
case 39:
#line 333 ".\\rdortp.y"
{
					double min = *((double *)yyvsp[-3]);
					double max = yyvsp[-1];
					RDORTPRealDiap *diap = new RDORTPRealDiap(min, max);
					yyval = (int)diap;
				;
    break;}
case 40:
#line 339 ".\\rdortp.y"
{
					double min = yyvsp[-3];
					double max = *((double *)yyvsp[-1]);
					RDORTPRealDiap *diap = new RDORTPRealDiap(min, max);
					yyval = (int)diap;
				;
    break;}
case 41:
#line 345 ".\\rdortp.y"
{
					double min = yyvsp[-3];
					double max = yyvsp[-1];
					RDORTPRealDiap *diap = new RDORTPRealDiap(min, max);
					yyval = (int)diap;
				;
    break;}
case 42:
#line 351 ".\\rdortp.y"
{
					currParser->lexer_loc_set( &(yylsp[-1]) );
					currParser->error( "Диапазон задан неверно" );
				;
    break;}
case 43:
#line 355 ".\\rdortp.y"
{
					currParser->lexer_loc_set( &(yylsp[-1]) );
					currParser->error( "Диапазон задан неверно" );
				;
    break;}
case 44:
#line 359 ".\\rdortp.y"
{
					currParser->lexer_loc_set( &(yylsp[-1]) );
					currParser->error( "Диапазон задан неверно" );
				;
    break;}
case 45:
#line 363 ".\\rdortp.y"
{
					currParser->lexer_loc_set( &(yylsp[-1]) );
					currParser->error( "Диапазон задан неверно" );
				;
    break;}
case 46:
#line 367 ".\\rdortp.y"
{
					currParser->lexer_loc_set( &(yylsp[-1]) );
					currParser->error( "Диапазон задан неверно" );
				;
    break;}
case 47:
#line 371 ".\\rdortp.y"
{
					currParser->lexer_loc_set( &(yylsp[-1]) );
					currParser->error( "Диапазон задан неверно" );
				;
    break;}
case 48:
#line 375 ".\\rdortp.y"
{
					currParser->lexer_loc_set( &(yylsp[0]) );
					currParser->error( "Диапазон задан неверно" );
//					currParser->error( rdosim::RDOSyntaxError::RTP_INVALID_RANGE );
				;
    break;}
case 49:
#line 381 ".\\rdortp.y"
{
							yyval = (int)(new RDORTPIntDefVal());
						;
    break;}
case 50:
#line 384 ".\\rdortp.y"
{
							yyval = (int)(new RDORTPIntDefVal(yyvsp[0]));
						;
    break;}
case 51:
#line 387 ".\\rdortp.y"
{
							// Целое число инициализируется вещественным: %f
							currParser->lexer_loc_set( &(yylsp[0]) );
							currParser->error( rdosim::RDOSyntaxError::RTP_INVALID_DEFVAULT_INT_AS_REAL, *(double*)yyvsp[0] );
						;
    break;}
case 52:
#line 392 ".\\rdortp.y"
{
							if ( currParser->lexer_loc_line() == yylsp[-1].first_line ) {
								std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
								if ( str.empty() ) {
									currParser->lexer_loc_set( &(yylsp[-1]) );
									currParser->error( "Ожидается значение по-ум. для integer" );
								} else {
									currParser->lexer_loc_set( &(yylsp[0]) );
									currParser->error( rdo::format( "Неверное значение по-ум. для integer: %s", str.c_str() ) );
								}
							} else {
								currParser->lexer_loc_set( &(yylsp[-1]) );
								currParser->error( "Ожидается значение по-ум. для integer" );
							}
						;
    break;}
case 53:
#line 408 ".\\rdortp.y"
{
							yyval = (int)(new RDORTPRealDefVal());
						;
    break;}
case 54:
#line 411 ".\\rdortp.y"
{
							yyval = (int)(new RDORTPRealDefVal(*((double *)yyvsp[0])));
						;
    break;}
case 55:
#line 414 ".\\rdortp.y"
{
							yyval = (int)(new RDORTPRealDefVal(yyvsp[0]));
						;
    break;}
case 56:
#line 417 ".\\rdortp.y"
{
							if ( currParser->lexer_loc_line() == yylsp[-1].first_line ) {
								std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
								if ( str.empty() ) {
									currParser->lexer_loc_set( &(yylsp[-1]) );
									currParser->error( "Ожидается значение по-ум. для real" );
								} else {
									currParser->lexer_loc_set( &(yylsp[0]) );
									currParser->error( rdo::format( "Неверное значение по-ум. для real: %s", str.c_str() ) );
								}
							} else {
								currParser->lexer_loc_set( &(yylsp[-1]) );
								currParser->error( "Ожидается значение по-ум. для real" );
							}
						;
    break;}
case 57:
#line 433 ".\\rdortp.y"
{
				yyval = yyvsp[-1];
			;
    break;}
case 58:
#line 436 ".\\rdortp.y"
{
				currParser->lexer_loc_set( &(yylsp[0]) );
				currParser->error( "Перечисление должно заканчиваться скобкой" );
			;
    break;}
case 59:
#line 441 ".\\rdortp.y"
{
					RDORTPEnum *enu = new RDORTPEnum((std::string *)yyvsp[0]);
					yyval = (int)enu;
					reinterpret_cast<RDOLexerRTP*>(lexer)->enum_param_cnt = 1;
				;
    break;}
case 60:
#line 446 ".\\rdortp.y"
{
					if ( reinterpret_cast<RDOLexerRTP*>(lexer)->enum_param_cnt >= 1 ) {
						currParser->lexer_loc_backup();
						currParser->lexer_loc_set( &(yylsp[0]) );
						RDORTPEnum *enu = (RDORTPEnum *)yyvsp[-2];
						enu->add((std::string *)yyvsp[0]);
						currParser->lexer_loc_restore();
						yyval = (int)enu;
					} else {
						currParser->error( "Ошибка в описании значений перечислимого типа" );
					}
				;
    break;}
case 61:
#line 458 ".\\rdortp.y"
{
					if ( reinterpret_cast<RDOLexerRTP*>(lexer)->enum_param_cnt >= 1 ) {
						currParser->error( rdo::format("Пропущена запятая перед: %s", ((std::string*)yyvsp[0])->c_str()) );
					} else {
						currParser->error( "Ошибка в описании значений перечислимого типа" );
					}
				;
    break;}
case 62:
#line 465 ".\\rdortp.y"
{
					std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
					if ( str.empty() ) {
						currParser->lexer_loc_set( &(yylsp[-1]) );
						currParser->error( "Ошибка в описании значений перечислимого типа" );
					} else {
						currParser->lexer_loc_set( &(yylsp[0]) );
						currParser->error( rdo::format( "Неверное значение перечислимого типа: %s", str.c_str() ) );
					}
				;
    break;}
case 63:
#line 475 ".\\rdortp.y"
{
					currParser->lexer_loc_set( &(yylsp[0]) );
					currParser->error( "Значение перечислимого типа не может начинаться с цифры" );
				;
    break;}
case 64:
#line 479 ".\\rdortp.y"
{
					currParser->lexer_loc_set( &(yylsp[0]) );
					currParser->error( "Значение перечислимого типа не может начинаться с цифры" );
				;
    break;}
case 65:
#line 483 ".\\rdortp.y"
{
					currParser->lexer_loc_set( &(yylsp[0]) );
					currParser->error( "Значение перечислимого типа не может начинаться с цифры" );
				;
    break;}
case 66:
#line 487 ".\\rdortp.y"
{
					currParser->lexer_loc_set( &(yylsp[0]) );
					currParser->error( "Значение перечислимого типа не может начинаться с цифры" );
				;
    break;}
case 67:
#line 491 ".\\rdortp.y"
{
					currParser->lexer_loc_set( &(yylsp[0]) );
					currParser->error( "Значение перечислимого типа не может начинаться с цифры" );
				;
    break;}
case 68:
#line 495 ".\\rdortp.y"
{
					currParser->lexer_loc_set( &(yylsp[0]) );
					currParser->error( "Значение перечислимого типа не может начинаться с цифры" );
				;
    break;}
case 69:
#line 499 ".\\rdortp.y"
{
					currParser->lexer_loc_set( &(yylsp[0]) );
					currParser->error( "Ошибка в описании значений перечислимого типа" );
				;
    break;}
case 70:
#line 504 ".\\rdortp.y"
{
							yyval = (int)(new RDORTPEnumDefVal());
						;
    break;}
case 71:
#line 507 ".\\rdortp.y"
{
							currParser->lexer_loc_set( &(yylsp[0]) );
							yyval = (int)(new RDORTPEnumDefVal((std::string *)yyvsp[0]));
						;
    break;}
case 72:
#line 511 ".\\rdortp.y"
{
							if ( currParser->lexer_loc_line() == yylsp[-1].first_line ) {
								std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
								if ( str.empty() ) {
									currParser->lexer_loc_set( &(yylsp[-1]) );
									currParser->error( "Ожидается значение по-ум. для enum" );
								} else {
									currParser->lexer_loc_set( &(yylsp[0]) );
									currParser->error( rdo::format( "Неверное значение по-ум. для enum: %s", str.c_str() ) );
								}
							} else {
								currParser->lexer_loc_set( &(yylsp[-1]) );
								currParser->error( "Ожидается значение по-ум. для enum" );
							}
						;
    break;}
case 73:
#line 527 ".\\rdortp.y"
{
					std::string* type = (std::string *)yyvsp[-2];
					std::string* param = (std::string *)yyvsp[0];
					const RDORTPResType *const rt = currParser->findRTPResType( type );
					if ( !rt ) {
						currParser->lexer_loc_set( &(yylsp[-2]) );
						currParser->error( rdosim::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type->c_str() );
					}
					const RDORTPParamDesc *const rp = rt->findRTPParam( param );
					if ( !rp ) {
						currParser->lexer_loc_set( &(yylsp[0]) );
						currParser->error( rdosim::RDOSyntaxError::RTP_INVALID_SUCHAS_PARAM, type->c_str(), param->c_str() );
					}
					yyval = (int)rp;
				;
    break;}
case 74:
#line 542 ".\\rdortp.y"
{
					std::string* type = (std::string *)yyvsp[-1];
					const RDORTPResType *const rt = currParser->findRTPResType( type );
					if ( !rt ) {
						currParser->lexer_loc_set( &(yylsp[-1]) );
						currParser->error( rdosim::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type->c_str() );
					} else {
						currParser->lexer_loc_set( &(yylsp[0]) );
						currParser->error( "Не указан параметр" );
					}
				;
    break;}
case 75:
#line 553 ".\\rdortp.y"
{
					std::string* type = (std::string *)yyvsp[-2];
					const RDORTPResType *const rt = currParser->findRTPResType( type );
					if ( !rt ) {
						currParser->lexer_loc_set( &(yylsp[-2]) );
						currParser->error( rdosim::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type->c_str() );
					} else {
						currParser->lexer_loc_set( &(yylsp[0]) );
						currParser->error( "Ошибка при указании параметра" );
					}
				;
    break;}
case 76:
#line 564 ".\\rdortp.y"
{
					std::string* type = (std::string *)yyvsp[-1];
					const RDORTPResType *const rt = currParser->findRTPResType( type );
					if ( !rt ) {
						currParser->lexer_loc_set( &(yylsp[-1]) );
						currParser->error( rdosim::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type->c_str() );
					} else {
						currParser->lexer_loc_set( &(yylsp[-1]) );
						currParser->error( "После имени типа должен быть указан параметр через точку" );
					}
				;
    break;}
case 77:
#line 575 ".\\rdortp.y"
{
					if ( yylsp[-1].last_line == yylsp[0].first_line ) {
						currParser->lexer_loc_set( yylsp[0].first_line, yylsp[0].first_column );
					} else {
						currParser->lexer_loc_set( &(yylsp[-1]) );
					}
					currParser->error( "После ключевого слова such_as необходимо указать тип и парамтер ресурса для ссылки" );
				;
    break;}
}

#line 705 "..\\System\\bison.simple"


  yyvsp -= yylen;
  yyssp -= yylen;
#if YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;
#if YYLSP_NEEDED
  *++yylsp = yyloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[YYTRANSLATE (yychar)]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[YYTRANSLATE (yychar)]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* defined (YYERROR_VERBOSE) */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*--------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;


/*-------------------------------------------------------------------.
| yyerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
yyerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  yyn = yydefact[yystate];
  if (yyn)
    goto yydefault;
#endif


/*---------------------------------------------------------------.
| yyerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
yyerrpop:
  if (yyssp == yyss)
    YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#if YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "Error: state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

/*--------------.
| yyerrhandle.  |
`--------------*/
yyerrhandle:
  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

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

/*---------------------------------------------.
| yyoverflowab -- parser overflow comes here.  |
`---------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}
#line 584 ".\\rdortp.y"


}

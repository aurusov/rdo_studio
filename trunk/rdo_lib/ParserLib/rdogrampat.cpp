/* A Bison parser, made from .\rdopat.y
   by GNU bison 1.34.  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse patparse
#define yylex patlex
#define yyerror paterror
#define yylval patlval
#define yychar patchar
#define yydebug patdebug
#define yynerrs patnerrs
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
# define	SIEZE	364
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

#line 1 ".\\rdopat.y"

#define YYPARSE_PARAM lexer
#define YYLEX_PARAM lexer
#line 138 ".\\rdopat.y"

#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoparser.h"
#include "rdoparselex.h"
#include "rdopat.h"
#include "rdortp.h"
#include "rdofun.h"

namespace rdoParse 
{
#ifndef YYSTYPE
#define YYSTYPE int
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		191
#define	YYFLAG		-32768
#define	YYNTBASE	139

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 415 ? yytranslate[x] : 172)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     133,   134,   128,   126,   135,   127,   136,   129,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     137,   130,   138,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   131,     2,   132,     2,     2,     2,     2,     2,     2,
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
     105,   106,   107,   108,   109,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     1,     4,     9,    14,    19,    24,    25,    27,
      29,    32,    36,    40,    43,    46,    52,    58,    63,    68,
      73,    78,    82,    86,    90,    94,    99,   104,   106,   108,
     110,   112,   114,   117,   121,   125,   128,   134,   137,   140,
     144,   145,   148,   152,   153,   155,   158,   161,   163,   168,
     173,   181,   186,   191,   192,   196,   199,   202,   205,   209,
     212,   216,   219,   222,   224,   226,   229,   232,   235,   238,
     241,   244,   245,   251,   252,   258,   262,   264,   268,   273,
     276,   280,   284,   288,   292,   296,   300,   304,   308,   312,
     314,   318,   322,   326,   330,   334,   336,   340,   342,   344,
     346,   351,   352,   355,   359,   361,   363,   365,   367,   371,
     375
};
static const short yyrhs[] =
{
      -1,   139,   155,     0,    43,    18,    44,   141,     0,    43,
      18,    45,   141,     0,    43,    18,    46,   141,     0,    43,
      18,    47,   141,     0,     0,    16,     0,    17,     0,   140,
       5,     0,   142,    18,   156,     0,   143,    18,   156,     0,
     143,    48,     0,   140,    48,     0,   144,    18,    10,   146,
     146,     0,   145,    18,    10,   146,   146,     0,   144,    18,
      10,   146,     0,   145,    18,    10,   146,     0,   144,    18,
      53,   146,     0,   145,    18,    53,   146,     0,   144,    18,
      65,     0,   145,    18,    65,     0,   144,    18,    53,     0,
     145,    18,    53,     0,   144,    18,    10,    53,     0,   145,
      18,    10,    53,     0,    49,     0,    50,     0,    51,     0,
      52,     0,   145,     0,   145,    57,     0,   145,    63,   166,
       0,   145,    62,   166,     0,   147,    20,     0,   147,    54,
     130,   166,    20,     0,   148,    10,     0,   153,    10,     0,
     149,   151,   152,     0,     0,    55,    34,     0,    55,    56,
     165,     0,     0,    57,     0,    63,   166,     0,    62,   166,
       0,   150,     0,   150,    58,   141,   154,     0,   150,    59,
     141,   154,     0,   150,    58,   141,   154,    59,   141,   154,
       0,   150,    60,   141,   154,     0,   150,    61,   141,   154,
       0,     0,   154,    64,   166,     0,   154,    53,     0,   153,
       8,     0,   148,     8,     0,     6,   160,   159,     0,     6,
     160,     0,     7,   161,   158,     0,     7,   161,     0,   162,
     157,     0,   162,     0,   164,     0,   164,   159,     0,   164,
     158,     0,   164,   157,     0,   130,    10,     0,   130,    12,
       0,   130,    11,     0,     0,   131,    11,    14,    11,   132,
       0,     0,   131,    12,    14,    12,   132,     0,   133,   163,
     134,     0,    10,     0,   163,   135,    10,     0,    13,    10,
     136,    10,     0,    13,    10,     0,   166,   130,   166,     0,
     166,    31,   166,     0,   166,   137,   166,     0,   166,   138,
     166,     0,   166,    32,   166,     0,   166,    33,   166,     0,
     165,    36,   165,     0,   165,    35,   165,     0,   131,   165,
     132,     0,   171,     0,   166,   126,   166,     0,   166,   127,
     166,     0,   166,   128,   166,     0,   166,   129,   166,     0,
     133,   166,   134,     0,   167,     0,    10,   136,    10,     0,
      11,     0,    12,     0,    10,     0,    10,   133,   168,   134,
       0,     0,   168,   166,     0,   168,   135,   166,     0,    27,
       0,    28,     0,    29,     0,    30,     0,   169,   133,    18,
       0,   170,   165,   134,     0,   170,    34,   134,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   163,   164,   167,   168,   169,   170,   172,   173,   174,
     177,   179,   180,   182,   183,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   199,   200,   201,
     202,   204,   205,   206,   207,   209,   210,   213,   214,   216,
     218,   219,   220,   222,   223,   224,   225,   227,   228,   230,
     232,   234,   236,   239,   240,   241,   243,   244,   250,   256,
     262,   268,   274,   281,   288,   292,   297,   305,   315,   322,
     328,   334,   338,   344,   348,   357,   359,   363,   370,   383,
     394,   395,   396,   397,   398,   399,   400,   401,   402,   403,
     406,   407,   408,   409,   410,   411,   412,   413,   414,   415,
     417,   419,   420,   421,   424,   425,   426,   427,   429,   431,
     432
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
  "Process", "SIEZE", "Frame", "Show_if", "Back_picture", "Show", 
  "frm_cell", "text", "transparent", "bitmap", "s_bmp", "rect_keyword", 
  "r_rect", "line", "ellipse", "triang", "active", "QUOTED_IDENTIF", 
  "'+'", "'-'", "'*'", "'/'", "'='", "'['", "']'", "'('", "')'", "','", 
  "'.'", "'<'", "'>'", "pat_list", "pat_header", "pat_trace", 
  "pat_params_begin", "pat_params", "pat_params_end", "pat_rel_res", 
  "pat_conv", "pat_common_choice", "pat_time", "pat_body", 
  "pat_res_usage", "pat_choice", "pat_first", "pat_convert", 
  "pat_params_set", "pat_pattern", "pat_type", 
  "fun_const_enum_default_val", "fun_const_real_default_val", 
  "fun_const_int_default_val", "fun_const_int_diap", 
  "fun_const_real_diap", "fun_const_enum", "fun_const_enum_list", 
  "fun_const_such_as", "pat_logic", "pat_arithm", "pat_arithm_func_call", 
  "pat_arithm_func_call_pars", "fun_group_keyword", "fun_group_header", 
  "fun_group", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,   139,   139,   140,   140,   140,   140,   141,   141,   141,
     142,   143,   143,   144,   144,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   146,   146,   146,
     146,   147,   147,   147,   147,   148,   148,   149,   149,   150,
     151,   151,   151,   152,   152,   152,   152,   153,   153,   153,
     153,   153,   153,   154,   154,   154,   155,   155,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   157,   158,
     159,   160,   160,   161,   161,   162,   163,   163,   164,   164,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     167,   168,   168,   168,   169,   169,   169,   169,   170,   171,
     171
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     0,     2,     4,     4,     4,     4,     0,     1,     1,
       2,     3,     3,     2,     2,     5,     5,     4,     4,     4,
       4,     3,     3,     3,     3,     4,     4,     1,     1,     1,
       1,     1,     2,     3,     3,     2,     5,     2,     2,     3,
       0,     2,     3,     0,     1,     2,     2,     1,     4,     4,
       7,     4,     4,     0,     3,     2,     2,     2,     3,     2,
       3,     2,     2,     1,     1,     2,     2,     2,     2,     2,
       2,     0,     5,     0,     5,     3,     1,     3,     4,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       3,     3,     3,     3,     3,     1,     3,     1,     1,     1,
       4,     0,     2,     3,     1,     1,     1,     1,     3,     3,
       3
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       1,     0,     0,     0,     0,     0,     0,    31,     0,     0,
      40,    47,     0,     2,     0,    10,    14,     0,     0,    13,
       0,     0,    32,     0,     0,    35,     0,    57,    37,     0,
      43,     7,     7,     7,     7,    56,    38,     7,     7,     7,
       7,    71,    73,     0,     0,    11,    63,    64,    12,     0,
      23,    21,     0,    24,    22,    99,    97,    98,     0,    34,
      95,    33,     0,    41,     0,    44,     0,     0,    39,     8,
       9,    53,    53,    53,    53,     3,     4,     5,     6,     0,
      59,     0,    61,    79,    76,     0,     0,    62,     0,    67,
      66,    65,    27,    28,    29,    30,    25,    17,    19,    26,
      18,    20,   101,     0,     0,     0,     0,     0,     0,     0,
     104,   105,   106,   107,     0,    42,     0,     0,     0,    89,
      46,    45,    48,    49,    51,    52,     0,     0,    58,     0,
       0,    60,     0,    75,     0,    68,    70,    69,    15,    16,
       0,    96,    94,    90,    91,    92,    93,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     7,     0,     0,     0,    78,    77,   100,     0,   102,
      88,    87,    86,    81,    84,    85,    80,    82,    83,   108,
     110,   109,    53,    54,     0,     0,   103,    50,    72,    74,
       0,     0
};

static const short yydefgoto[] =
{
       1,     3,    71,     4,     5,     6,     7,    97,     8,     9,
      10,    11,    30,    68,    12,   122,    13,    45,    87,    90,
      91,    80,    82,    46,    85,    47,   115,   116,    60,   140,
     117,   118,   119
};

static const short yypact[] =
{
  -32768,    27,    45,    21,    84,    24,    86,    16,     3,    14,
      52,   115,    79,-32768,   133,-32768,-32768,     2,     2,-32768,
       6,    15,-32768,     7,     7,-32768,   -19,-32768,-32768,    30,
      38,    33,    33,    33,    33,-32768,-32768,    33,    33,    33,
      33,   -13,    -7,   106,   126,-32768,    12,    13,-32768,   103,
     132,-32768,   116,   132,-32768,   -30,-32768,-32768,     7,    59,
  -32768,    59,     7,-32768,    26,-32768,     7,     7,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   127,
      17,   134,    20,    22,-32768,   -38,   138,-32768,    64,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,   132,-32768,-32768,
     132,-32768,-32768,   160,   -36,     7,     7,     7,     7,    -6,
  -32768,-32768,-32768,-32768,    26,    74,    34,    11,    18,-32768,
      59,    59,    35,    -2,    -2,    -2,   175,   179,-32768,   177,
     180,-32768,   183,-32768,   184,-32768,-32768,-32768,-32768,-32768,
      -8,-32768,-32768,   -14,   -14,-32768,-32768,-32768,     5,    26,
      26,     7,     7,     7,     7,     7,     7,   178,    61,   -15,
  -32768,    33,     7,   186,   187,-32768,-32768,-32768,     7,    59,
  -32768,   162,-32768,    59,    59,    59,    59,    59,    59,-32768,
  -32768,-32768,-32768,    59,    68,    69,    59,    -2,-32768,-32768,
     202,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,   -27,-32768,-32768,-32768,-32768,     8,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   -41,-32768,   185,   157,   123,
     128,-32768,-32768,-32768,-32768,-32768,   -37,   -23,-32768,-32768,
  -32768,-32768,-32768
};


#define	YYLAST		208


static const short yytable[] =
{
      59,    61,    55,    56,    57,    72,    73,    74,    41,    42,
      75,    76,    77,    78,   147,    43,    49,    55,    56,    57,
     149,   150,    27,    25,    28,    52,    15,   190,    55,    56,
      57,   123,   124,   125,    21,   104,    55,    56,    57,   109,
     149,   150,    18,   120,   121,   110,   111,   112,   113,    69,
      70,   160,   158,   110,   111,   112,   113,    26,    98,    50,
     100,   101,   162,    14,    63,   151,   152,   153,    53,    16,
       2,    51,    19,    22,   135,   136,   137,   148,    23,    24,
      54,   159,   143,   144,   145,   146,    64,    35,   160,    36,
     105,   106,   107,   108,   161,    65,   133,   134,   142,   162,
      66,    67,    17,   102,    20,   138,   103,    29,   139,   149,
     150,    62,   171,   172,   107,   108,    83,   169,    79,   181,
     105,   106,   107,   108,    81,    58,   167,   168,   173,   174,
     175,   176,   177,   178,   182,    44,    84,   170,   126,   183,
      58,   187,    86,    88,   157,   186,   129,   127,   135,   114,
     130,    58,    92,    93,    94,    95,    96,   114,   132,    58,
     105,   106,   107,   108,   154,    92,    93,    94,    95,    99,
     141,   155,   156,    31,    32,    33,    34,    37,    38,    39,
      40,    92,    93,    94,    95,   105,   106,   107,   108,   163,
     136,   164,   137,   165,   166,   180,   179,   184,   150,   185,
     188,   189,   191,    48,    89,   131,     0,     0,   128
};

static const short yycheck[] =
{
      23,    24,    10,    11,    12,    32,    33,    34,     6,     7,
      37,    38,    39,    40,    20,    13,    10,    10,    11,    12,
      35,    36,     8,    20,    10,    10,     5,     0,    10,    11,
      12,    72,    73,    74,    18,    58,    10,    11,    12,    62,
      35,    36,    18,    66,    67,    27,    28,    29,    30,    16,
      17,    53,    34,    27,    28,    29,    30,    54,    50,    53,
      52,    53,    64,    18,    34,    31,    32,    33,    53,    48,
      43,    65,    48,    57,    10,    11,    12,   114,    62,    63,
      65,   118,   105,   106,   107,   108,    56,     8,    53,    10,
     126,   127,   128,   129,    59,    57,   134,   135,   134,    64,
      62,    63,    18,   133,    18,    97,   136,    55,   100,    35,
      36,   130,   149,   150,   128,   129,    10,   140,   131,   134,
     126,   127,   128,   129,   131,   133,   134,   135,   151,   152,
     153,   154,   155,   156,   161,   133,    10,   132,    11,   162,
     133,   182,   130,   130,   133,   168,    12,   130,    10,   131,
     130,   133,    49,    50,    51,    52,    53,   131,   136,   133,
     126,   127,   128,   129,   130,    49,    50,    51,    52,    53,
      10,   137,   138,    58,    59,    60,    61,    44,    45,    46,
      47,    49,    50,    51,    52,   126,   127,   128,   129,    14,
      11,    14,    12,    10,    10,   134,    18,    11,    36,    12,
     132,   132,     0,    18,    47,    82,    -1,    -1,    80
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
#line 167 ".\\rdopat.y"
{ yyval = (int)(new RDOPATPatternOperation(	(std::string *)yyvsp[-2], yyvsp[0] != 0, currParser->patternCounter++)); ;
    break;}
case 4:
#line 168 ".\\rdopat.y"
{ yyval = (int)(new RDOPATPatternEvent(		(std::string *)yyvsp[-2], yyvsp[0] != 0, currParser->patternCounter++)); ;
    break;}
case 5:
#line 169 ".\\rdopat.y"
{ yyval = (int)(new RDOPATPatternRule(			(std::string *)yyvsp[-2], yyvsp[0] != 0, currParser->patternCounter++)); ;
    break;}
case 6:
#line 170 ".\\rdopat.y"
{ yyval = (int)(new RDOPATPatternKeyboard(	(std::string *)yyvsp[-2], yyvsp[0] != 0, currParser->patternCounter++)); ;
    break;}
case 7:
#line 172 ".\\rdopat.y"
{ yyval = 0; ;
    break;}
case 8:
#line 173 ".\\rdopat.y"
{ yyval = 1; ;
    break;}
case 9:
#line 174 ".\\rdopat.y"
{ yyval = 0; ;
    break;}
case 10:
#line 177 ".\\rdopat.y"
{ yyval = yyvsp[-1]; ;
    break;}
case 11:
#line 179 ".\\rdopat.y"
{ ((RDOPATPattern *)yyvsp[-2])->add(new RDOFUNFunctionParam((std::string *)yyvsp[-1], (RDORTPResParam *)yyvsp[0])); yyval = yyvsp[-2]; ;
    break;}
case 12:
#line 180 ".\\rdopat.y"
{ ((RDOPATPattern *)yyvsp[-2])->add(new RDOFUNFunctionParam((std::string *)yyvsp[-1], (RDORTPResParam *)yyvsp[0])); yyval = yyvsp[-2]; ;
    break;}
case 13:
#line 182 ".\\rdopat.y"
{ yyval = yyvsp[-1]; ;
    break;}
case 14:
#line 183 ".\\rdopat.y"
{ yyval = yyvsp[-1]; ;
    break;}
case 15:
#line 186 ".\\rdopat.y"
{ ((RDOPATPattern *)yyvsp[-4])->addRelRes((std::string *)yyvsp[-3], (std::string *)yyvsp[-2], (ConvertStatus)yyvsp[-1],	(ConvertStatus)yyvsp[0]);	yyval = yyvsp[-4]; ;
    break;}
case 16:
#line 187 ".\\rdopat.y"
{ ((RDOPATPattern *)yyvsp[-4])->addRelRes((std::string *)yyvsp[-3], (std::string *)yyvsp[-2], (ConvertStatus)yyvsp[-1],	(ConvertStatus)yyvsp[0]);	yyval = yyvsp[-4]; ;
    break;}
case 17:
#line 188 ".\\rdopat.y"
{ ((RDOPATPattern *)yyvsp[-3])->addRelRes((std::string *)yyvsp[-2], (std::string *)yyvsp[-1], (ConvertStatus)yyvsp[0]);								yyval = yyvsp[-3]; ;
    break;}
case 18:
#line 189 ".\\rdopat.y"
{ ((RDOPATPattern *)yyvsp[-3])->addRelRes((std::string *)yyvsp[-2], (std::string *)yyvsp[-1], (ConvertStatus)yyvsp[0]);								yyval = yyvsp[-3]; ;
    break;}
case 19:
#line 190 ".\\rdopat.y"
{ ((RDOPATPattern *)yyvsp[-3])->addRelRes((std::string *)yyvsp[-2], (std::string *)yyvsp[-1], CS_NoChange,			(ConvertStatus)yyvsp[0]);	yyval = yyvsp[-3]; ;
    break;}
case 20:
#line 191 ".\\rdopat.y"
{ ((RDOPATPattern *)yyvsp[-3])->addRelRes((std::string *)yyvsp[-2], (std::string *)yyvsp[-1], CS_NoChange,			(ConvertStatus)yyvsp[0]);	yyval = yyvsp[-3]; ;
    break;}
case 21:
#line 192 ".\\rdopat.y"
{ ((RDOPATPattern *)yyvsp[-2])->addRelRes((std::string *)yyvsp[-1], (std::string *)yyvsp[0], CS_NoChange,			CS_NoChange);			yyval = yyvsp[-2]; ;
    break;}
case 22:
#line 193 ".\\rdopat.y"
{ ((RDOPATPattern *)yyvsp[-2])->addRelRes((std::string *)yyvsp[-1], (std::string *)yyvsp[0], CS_NoChange,			CS_NoChange);			yyval = yyvsp[-2]; ;
    break;}
case 23:
#line 194 ".\\rdopat.y"
{ ((RDOPATPattern *)yyvsp[-2])->addRelRes((std::string *)yyvsp[-1], (std::string *)yyvsp[0], CS_NoChange);										yyval = yyvsp[-2]; ;
    break;}
case 24:
#line 195 ".\\rdopat.y"
{ ((RDOPATPattern *)yyvsp[-2])->addRelRes((std::string *)yyvsp[-1], (std::string *)yyvsp[0], CS_NoChange);										yyval = yyvsp[-2]; ;
    break;}
case 25:
#line 196 ".\\rdopat.y"
{ ((RDOPATPattern *)yyvsp[-3])->addRelRes((std::string *)yyvsp[-2], (std::string *)yyvsp[-1], (std::string *)yyvsp[0]);										yyval = yyvsp[-3]; ;
    break;}
case 26:
#line 197 ".\\rdopat.y"
{ ((RDOPATPattern *)yyvsp[-3])->addRelRes((std::string *)yyvsp[-2], (std::string *)yyvsp[-1], (std::string *)yyvsp[0]);										yyval = yyvsp[-3]; ;
    break;}
case 27:
#line 199 ".\\rdopat.y"
{ yyval = CS_Keep;		;
    break;}
case 28:
#line 200 ".\\rdopat.y"
{ yyval = CS_Create;		;
    break;}
case 29:
#line 201 ".\\rdopat.y"
{ yyval = CS_Erase;		;
    break;}
case 30:
#line 202 ".\\rdopat.y"
{ yyval = CS_NonExist;	;
    break;}
case 32:
#line 205 ".\\rdopat.y"
{	((RDOPATPattern *)yyvsp[-1])->setCommonChoiceFirst(); yyval = yyvsp[-1]; ;
    break;}
case 33:
#line 206 ".\\rdopat.y"
{	((RDOPATPattern *)yyvsp[-2])->setCommonChoiceWithMin((RDOFUNArithm *)yyvsp[0]); yyval = yyvsp[-2]; ;
    break;}
case 34:
#line 207 ".\\rdopat.y"
{	((RDOPATPattern *)yyvsp[-2])->setCommonChoiceWithMax((RDOFUNArithm *)yyvsp[0]); yyval = yyvsp[-2]; ;
    break;}
case 35:
#line 209 ".\\rdopat.y"
{	yyval = yyvsp[-1]; ;
    break;}
case 36:
#line 210 ".\\rdopat.y"
{	((RDOPATPattern *)yyvsp[-4])->setTime((RDOFUNArithm *)yyvsp[-1]); yyval = yyvsp[-4]; ;
    break;}
case 37:
#line 213 ".\\rdopat.y"
{	((RDOPATPattern *)yyvsp[-1])->addRelResBody((std::string *)yyvsp[0]); yyval = yyvsp[-1]; ;
    break;}
case 38:
#line 214 ".\\rdopat.y"
{	((RDOPATPattern *)yyvsp[-1])->addRelResBody((std::string *)yyvsp[0]); yyval = yyvsp[-1]; ;
    break;}
case 39:
#line 216 ".\\rdopat.y"
{	((RDOPATPattern *)yyvsp[-2])->addRelResUsage((RDOPATChoice *)yyvsp[-1], (RDOPATFirst *)yyvsp[0]); yyval = yyvsp[-2]; ;
    break;}
case 40:
#line 218 ".\\rdopat.y"
{	yyval = (int) new RDOPATChoice(); ;
    break;}
case 41:
#line 219 ".\\rdopat.y"
{	yyval = (int) new RDOPATChoice(1); ;
    break;}
case 42:
#line 220 ".\\rdopat.y"
{	yyval = (int) new RDOPATChoice((RDOFUNLogic *)yyvsp[0]); ;
    break;}
case 43:
#line 222 ".\\rdopat.y"
{	yyval = (int) new RDOPATFirst(); ;
    break;}
case 44:
#line 223 ".\\rdopat.y"
{	yyval = (int) new RDOPATFirst(1); ;
    break;}
case 45:
#line 224 ".\\rdopat.y"
{	yyval = (int) new RDOPATFirst(true, (RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 46:
#line 225 ".\\rdopat.y"
{	yyval = (int) new RDOPATFirst(false, (RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 47:
#line 227 ".\\rdopat.y"
{	((RDOPATPattern *)yyvsp[0])->addRelResConvert(); yyval = yyvsp[0]; ;
    break;}
case 48:
#line 229 ".\\rdopat.y"
{	((RDOPATPattern *)yyvsp[-3])->addRelResConvertBegin(yyvsp[-1] != 0, (RDOPATParamsSet *)yyvsp[0]); yyval = yyvsp[-3]; ;
    break;}
case 49:
#line 231 ".\\rdopat.y"
{	((RDOPATPattern *)yyvsp[-3])->addRelResConvertEnd(yyvsp[-1] != 0, (RDOPATParamsSet *)yyvsp[0]); yyval = yyvsp[-3]; ;
    break;}
case 50:
#line 233 ".\\rdopat.y"
{	((RDOPATPattern *)yyvsp[-6])->addRelResConvertBeginEnd(yyvsp[-4] != 0, (RDOPATParamsSet *)yyvsp[-3], yyvsp[-1] != 0, (RDOPATParamsSet *)yyvsp[0]); yyval = yyvsp[-6]; ;
    break;}
case 51:
#line 235 ".\\rdopat.y"
{	((RDOPATPattern *)yyvsp[-3])->addRelResConvertRule(yyvsp[-1] != 0, (RDOPATParamsSet *)yyvsp[0]); yyval = yyvsp[-3]; ;
    break;}
case 52:
#line 237 ".\\rdopat.y"
{	((RDOPATPattern *)yyvsp[-3])->addRelResConvertEvent(yyvsp[-1] != 0, (RDOPATParamsSet *)yyvsp[0]); yyval = yyvsp[-3]; ;
    break;}
case 53:
#line 239 ".\\rdopat.y"
{  yyval = (int) new RDOPATParamsSet(); ;
    break;}
case 54:
#line 240 ".\\rdopat.y"
{	((RDOPATParamsSet *)yyvsp[-2])->addIdentif((std::string *)yyvsp[-1], (RDOFUNArithm *)yyvsp[0]); yyval = yyvsp[-2];;
    break;}
case 55:
#line 241 ".\\rdopat.y"
{	((RDOPATParamsSet *)yyvsp[-1])->addIdentif((std::string *)yyvsp[0]); yyval = yyvsp[-1];;
    break;}
case 56:
#line 243 ".\\rdopat.y"
{	((RDOPATPattern *)yyvsp[-1])->end(); yyval = yyvsp[-1];;
    break;}
case 57:
#line 244 ".\\rdopat.y"
{	((RDOPATPattern *)yyvsp[-1])->end(); yyval = yyvsp[-1];;
    break;}
case 58:
#line 250 ".\\rdopat.y"
{ 
						RDORTPIntDiap *diap = (RDORTPIntDiap *)yyvsp[-1];
						RDORTPIntDefVal *dv = (RDORTPIntDefVal *)yyvsp[0];
						RDORTPIntResParam *rp = new RDORTPIntResParam(diap, dv);
						yyval = (int)rp;
					;
    break;}
case 59:
#line 256 ".\\rdopat.y"
{ 
                  RDORTPIntDiap *diap = (RDORTPIntDiap *)yyvsp[0];
                  RDORTPIntDefVal *dv = new RDORTPIntDefVal();
                  RDORTPIntResParam *rp = new RDORTPIntResParam(diap, dv);
                  yyval = (int)rp;
					;
    break;}
case 60:
#line 262 ".\\rdopat.y"
{ 
                  RDORTPRealDiap *diap = (RDORTPRealDiap *)yyvsp[-1];
                  RDORTPRealDefVal *dv = (RDORTPRealDefVal *)yyvsp[0];
                  RDORTPRealResParam *rp = new RDORTPRealResParam(diap, dv);
                  yyval = (int)rp;
					;
    break;}
case 61:
#line 268 ".\\rdopat.y"
{ 
                  RDORTPRealDiap *diap = (RDORTPRealDiap *)yyvsp[0];
                  RDORTPRealDefVal *dv = new RDORTPRealDefVal();
                  RDORTPRealResParam *rp = new RDORTPRealResParam(diap, dv);
                  yyval = (int)rp;
					;
    break;}
case 62:
#line 274 ".\\rdopat.y"
{ 
                  RDORTPEnum *enu = (RDORTPEnum *)yyvsp[-1];
                  RDORTPEnumDefVal *dv = (RDORTPEnumDefVal *)yyvsp[0];
						enu->findValue(dv->value);	 // if no value - Syntax exception will be thrown
                  RDORTPEnumResParam *rp = new RDORTPEnumResParam(enu, dv);
                  yyval = (int)rp;
					;
    break;}
case 63:
#line 281 ".\\rdopat.y"
{ 
                  RDORTPEnum *enu = (RDORTPEnum *)yyvsp[0];
                  RDORTPEnumDefVal *dv = new RDORTPEnumDefVal();
                  RDORTPEnumResParam *rp = new RDORTPEnumResParam(enu, dv);
                  yyval = (int)rp;
					;
    break;}
case 64:
#line 288 ".\\rdopat.y"
{
						RDORTPParamDesc *desc = (RDORTPParamDesc *)yyvsp[0];
						yyval = (int)desc->getType()->constructSuchAs();
					;
    break;}
case 65:
#line 292 ".\\rdopat.y"
{
						RDORTPParamDesc *desc = (RDORTPParamDesc *)yyvsp[-1];
						RDORTPIntDefVal *dv = (RDORTPIntDefVal *)yyvsp[0];
						yyval = (int)desc->getType()->constructSuchAs((int)dv->val);
					;
    break;}
case 66:
#line 297 ".\\rdopat.y"
{
						RDORTPParamDesc *desc = (RDORTPParamDesc *)yyvsp[-1];
						RDORTPRealDefVal *dv = (RDORTPRealDefVal *)yyvsp[0];
						if(!dv->exist)
							yyval = (int)desc->getType()->constructSuchAs();
						else
							yyval = (int)desc->getType()->constructSuchAs((double *)&(dv->val));
					;
    break;}
case 67:
#line 305 ".\\rdopat.y"
{
						RDORTPParamDesc *desc = (RDORTPParamDesc *)yyvsp[-1];
						RDORTPEnumDefVal *dv = (RDORTPEnumDefVal *)yyvsp[0];
						if(!dv->exist)
							yyval = (int)desc->getType()->constructSuchAs();
						else
							yyval = (int)desc->getType()->constructSuchAs((std::string *)dv->value);
					;
    break;}
case 68:
#line 315 ".\\rdopat.y"
{
						std::string *val = (std::string *)yyvsp[0];
						RDORTPEnumDefVal *dv = new RDORTPEnumDefVal(val);
						yyval = (int)dv;	  
					;
    break;}
case 69:
#line 322 ".\\rdopat.y"
{
						double val = *((double *)yyvsp[0]);
						RDORTPRealDefVal *dv = new RDORTPRealDefVal(val);
						yyval = (int)dv;
					;
    break;}
case 70:
#line 328 ".\\rdopat.y"
{
						RDORTPIntDefVal *dv = new RDORTPIntDefVal(yyvsp[0]);
						yyval = (int)dv;
					;
    break;}
case 71:
#line 334 ".\\rdopat.y"
{
						RDORTPIntDiap *diap = new RDORTPIntDiap();
						yyval = (int)diap;
					;
    break;}
case 72:
#line 338 ".\\rdopat.y"
{
						RDORTPIntDiap *diap = new RDORTPIntDiap(yyvsp[-3], yyvsp[-1]);
						yyval = (int)diap;
					;
    break;}
case 73:
#line 344 ".\\rdopat.y"
{
						RDORTPRealDiap *diap = new RDORTPRealDiap();
						yyval = (int)diap;
					;
    break;}
case 74:
#line 348 ".\\rdopat.y"
{
						double min = *((double *)yyvsp[-3]);
						double max = *((double *)yyvsp[-1]);
						RDORTPRealDiap *diap = new RDORTPRealDiap(min, max);
						yyval = (int)diap;
					;
    break;}
case 75:
#line 357 ".\\rdopat.y"
{ yyval = yyvsp[-1]; ;
    break;}
case 76:
#line 359 ".\\rdopat.y"
{
							RDORTPEnum *enu = new RDORTPEnum((std::string *)yyvsp[0]);
							yyval = (int)enu;
						;
    break;}
case 77:
#line 363 ".\\rdopat.y"
{
							RDORTPEnum *enu = (RDORTPEnum *)yyvsp[-2];
							enu->add((std::string *)yyvsp[0]);
							yyval = (int)enu;
						;
    break;}
case 78:
#line 370 ".\\rdopat.y"
{
							std::string *type = (std::string *)yyvsp[-2];
							std::string *param = (std::string *)yyvsp[0];
							const RDORTPResType *const rt = currParser->findRTPResType(type);
							if(!rt)
								currParser->error(("Invalid resource type in such_as: " + *type).c_str());

							const RDORTPParamDesc *const rp = rt->findRTPParam(param);
							if(!rp)
								currParser->error(("Invalid resource parameter in such_as: " + *param).c_str());
								
							yyval = (int)rp;
						;
    break;}
case 79:
#line 383 ".\\rdopat.y"
{
							std::string *constName = (std::string *)yyvsp[0];
							const RDOFUNConstant *const cons = currParser->findFUNConst(constName);
							if(!cons)
								currParser->error(("Invalid constant reference: " + *constName).c_str());
								
							yyval = (int)(cons->descr);
						;
    break;}
case 80:
#line 394 ".\\rdopat.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] == *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 81:
#line 395 ".\\rdopat.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] != *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 82:
#line 396 ".\\rdopat.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] <  *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 83:
#line 397 ".\\rdopat.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] >  *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 84:
#line 398 ".\\rdopat.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] <= *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 85:
#line 399 ".\\rdopat.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] >= *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 86:
#line 400 ".\\rdopat.y"
{ yyval = (int)(*(RDOFUNLogic *)yyvsp[-2] && *(RDOFUNLogic *)yyvsp[0]); ;
    break;}
case 87:
#line 401 ".\\rdopat.y"
{ yyval = (int)(*(RDOFUNLogic *)yyvsp[-2] || *(RDOFUNLogic *)yyvsp[0]); ;
    break;}
case 88:
#line 402 ".\\rdopat.y"
{ yyval = yyvsp[-1]; ;
    break;}
case 89:
#line 403 ".\\rdopat.y"
{ yyval = yyvsp[0]; ;
    break;}
case 90:
#line 406 ".\\rdopat.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] + *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 91:
#line 407 ".\\rdopat.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] - *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 92:
#line 408 ".\\rdopat.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] * *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 93:
#line 409 ".\\rdopat.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] / *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 94:
#line 410 ".\\rdopat.y"
{ yyval = yyvsp[-1]; ;
    break;}
case 96:
#line 412 ".\\rdopat.y"
{ yyval = (int)(new RDOFUNArithm((std::string *)yyvsp[-2], (std::string *)yyvsp[0])); ;
    break;}
case 97:
#line 413 ".\\rdopat.y"
{ yyval = (int)(new RDOFUNArithm((int)yyvsp[0])); ;
    break;}
case 98:
#line 414 ".\\rdopat.y"
{ yyval = (int)(new RDOFUNArithm((double*)yyvsp[0])); ;
    break;}
case 99:
#line 415 ".\\rdopat.y"
{ yyval = (int)(new RDOFUNArithm((std::string *)yyvsp[0])); ;
    break;}
case 100:
#line 417 ".\\rdopat.y"
{ yyval = (int)((RDOFUNParams *)yyvsp[-1])->createCall((std::string *)yyvsp[-3]) ;
    break;}
case 101:
#line 419 ".\\rdopat.y"
{ yyval = (int)(new RDOFUNParams()); ;
    break;}
case 102:
#line 420 ".\\rdopat.y"
{ yyval = (int)(((RDOFUNParams *)yyvsp[-1])->addParameter((RDOFUNArithm *)yyvsp[0])); ;
    break;}
case 103:
#line 421 ".\\rdopat.y"
{ yyval = (int)(((RDOFUNParams *)yyvsp[-2])->addParameter((RDOFUNArithm *)yyvsp[0])); ;
    break;}
case 104:
#line 424 ".\\rdopat.y"
{ yyval = 1; ;
    break;}
case 105:
#line 425 ".\\rdopat.y"
{ yyval = 2; ;
    break;}
case 106:
#line 426 ".\\rdopat.y"
{ yyval = 3; ;
    break;}
case 107:
#line 427 ".\\rdopat.y"
{ yyval = 4; ;
    break;}
case 108:
#line 429 ".\\rdopat.y"
{ yyval = (int)(new RDOFUNGroup(yyvsp[-2], (std::string *)yyvsp[0])); ;
    break;}
case 109:
#line 431 ".\\rdopat.y"
{ yyval = (int)(((RDOFUNGroup *)yyvsp[-2])->createFunLogin((RDOFUNLogic *)yyvsp[-1])); ;
    break;}
case 110:
#line 432 ".\\rdopat.y"
{ yyval = (int)(((RDOFUNGroup *)yyvsp[-2])->createFunLogin()); ;
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
#line 439 ".\\rdopat.y"


}
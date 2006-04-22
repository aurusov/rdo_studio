/* A Bison parser, made from .\rdodpt.y
   by GNU bison 1.34.  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse dptparse
#define yylex dptlex
#define yyerror dpterror
#define yylval dptlval
#define yychar dptchar
#define yydebug dptdebug
#define yynerrs dptnerrs
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

#line 131 ".\\rdodpt.y"

#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#include <FlexLexer.h>

#include "rdoparser.h"
#include "rdoparselex.h"
#include "rdofun.h"
#include "rdodpt.h"
#include "rdortp.h"
#include "rdorss.h"

namespace rdoParse 
{
#ifndef YYSTYPE
#define YYSTYPE int
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		142
#define	YYFLAG		-32768
#define	YYNTBASE	139

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 415 ? yytranslate[x] : 171)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     135,   136,   128,   126,   138,   127,   137,   129,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     131,   130,   132,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   133,     2,   134,     2,     2,     2,     2,     2,     2,
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
       0,     0,     1,     4,     7,    10,    13,    17,    22,    27,
      32,    37,    41,    45,    49,    53,    58,    63,    66,    68,
      72,    74,    77,    80,    83,    86,    90,    94,    97,   101,
     106,   110,   114,   117,   119,   123,   125,   128,   131,   134,
     137,   140,   142,   144,   148,   150,   153,   157,   160,   163,
     166,   169,   171,   174,   177,   178,   181,   183,   186,   189,
     193,   197,   201,   205,   209,   213,   217,   221,   225,   227,
     231,   235,   239,   243,   247,   252,   256,   258,   260,   262,
     263,   266,   270,   272,   274,   276,   278,   282,   286
};
static const short yyrhs[] =
{
      -1,   139,   149,     0,   139,   155,     0,   139,   160,     0,
     139,   164,     0,    93,    18,    94,     0,    93,    18,    94,
      17,     0,    93,    18,    94,    95,     0,    93,    18,    94,
      96,     0,    93,    18,    94,    97,     0,   140,    98,   165,
       0,   140,    98,    34,     0,   141,    99,   165,     0,   142,
     100,   166,     0,   143,   101,   130,   102,     0,   143,   101,
     130,   103,     0,   144,   104,     0,   148,     0,   145,    18,
      10,     0,   146,     0,   147,    11,     0,   147,    12,     0,
     147,    10,     0,   147,   128,     0,   147,   105,   166,     0,
     147,   106,   166,     0,   145,     8,     0,    93,    18,   107,
       0,    93,    18,   107,    17,     0,   150,    98,   165,     0,
     150,    98,    34,     0,   151,   104,     0,   154,     0,   152,
      18,    10,     0,   153,     0,   154,    11,     0,   154,    12,
       0,   154,    10,     0,   154,   128,     0,   152,     8,     0,
     104,     0,   159,     0,   156,    18,    10,     0,   157,     0,
     158,   125,     0,   158,   126,   125,     0,   159,    11,     0,
     159,    12,     0,   159,    10,     0,   159,   128,     0,   158,
       0,   156,     8,     0,   108,   162,     0,     0,   162,   163,
       0,    10,     0,   109,    10,     0,   161,     8,     0,   166,
     130,   166,     0,   166,    31,   166,     0,   166,   131,   166,
       0,   166,   132,   166,     0,   166,    32,   166,     0,   166,
      33,   166,     0,   165,    36,   165,     0,   165,    35,   165,
       0,   133,   165,   134,     0,   170,     0,   166,   126,   166,
       0,   166,   127,   166,     0,   166,   128,   166,     0,   166,
     129,   166,     0,   135,   166,   136,     0,    10,   135,   167,
     136,     0,    10,   137,    10,     0,    11,     0,    12,     0,
      10,     0,     0,   167,   166,     0,   167,   138,   166,     0,
      27,     0,    28,     0,    29,     0,    30,     0,   168,   135,
      18,     0,   169,   165,   136,     0,   169,    34,   136,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   163,   164,   165,   166,   167,   172,   173,   174,   175,
     176,   178,   179,   181,   183,   185,   186,   188,   189,   191,
     193,   194,   195,   196,   197,   199,   200,   202,   207,   208,
     210,   211,   213,   214,   216,   218,   219,   220,   221,   222,
     224,   229,   230,   232,   234,   235,   236,   238,   239,   240,
     241,   242,   244,   249,   251,   252,   254,   255,   281,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   310,
     311,   312,   315,   316,   317,   318,   320,   322,   323
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
  "'+'", "'-'", "'*'", "'/'", "'='", "'<'", "'>'", "'['", "']'", "'('", 
  "')'", "'.'", "','", "dpt_main", "dpt_begin_search", 
  "dpt_condition_search", "dpt_term_search", "dpt_evaluate_search", 
  "dpt_compare_search", "dpt_activ_search", "dpt_activ_search_descr", 
  "dpt_activ_search_descr_param", "dpt_activ_search_descr_value", 
  "dpt_activ_search_end", "dpt_begin_some", "dpt_condition_some", 
  "dpt_activ_some", "dpt_activ_some_descr", "dpt_activ_some_descr_param", 
  "dpt_activ_some_end", "dpt_activ_free", "dpt_activ_free_descr", 
  "dpt_activ_free_descr_keyb", "dpt_activ_free_descr_param", 
  "dpt_activ_free_end", "dpt_process", "dpt_process_input", 
  "dpt_process_line", "dpt_process_end", "dpt_logic", "dpt_arithm", 
  "dpt_arithm_func_call_pars", "fun_group_keyword", "fun_group_header", 
  "fun_group", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,   139,   139,   139,   139,   139,   140,   140,   140,   140,
     140,   141,   141,   142,   143,   144,   144,   145,   145,   146,
     147,   147,   147,   147,   147,   148,   148,   149,   150,   150,
     151,   151,   152,   152,   153,   154,   154,   154,   154,   154,
     155,   156,   156,   157,   158,   158,   158,   159,   159,   159,
     159,   159,   160,   161,   162,   162,   163,   163,   164,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   167,
     167,   167,   168,   168,   168,   168,   169,   170,   170
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     0,     2,     2,     2,     2,     3,     4,     4,     4,
       4,     3,     3,     3,     3,     4,     4,     2,     1,     3,
       1,     2,     2,     2,     2,     3,     3,     2,     3,     4,
       3,     3,     2,     1,     3,     1,     2,     2,     2,     2,
       2,     1,     1,     3,     1,     2,     3,     2,     2,     2,
       2,     1,     2,     2,     0,     2,     1,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     3,
       3,     3,     3,     3,     4,     3,     1,     1,     1,     0,
       2,     3,     1,     1,     1,     1,     3,     3,     3
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       1,     0,     0,    41,    54,     0,     0,     0,     0,     0,
       0,    20,     0,    18,     2,     0,     0,     0,    35,    33,
       3,     0,    44,    51,    42,     4,     0,     5,     0,    53,
       0,     0,     0,     0,    17,    27,     0,    23,    21,    22,
       0,     0,    24,     0,    32,    40,     0,    38,    36,    37,
      39,    52,     0,    45,     0,    49,    47,    48,    50,    58,
       6,    28,    56,     0,    55,    78,    76,    77,    82,    83,
      84,    85,    12,     0,     0,    11,     0,     0,     0,    68,
      13,    14,     0,    19,    25,    26,    31,    30,    34,    43,
      46,     7,     8,     9,    10,    29,    57,    79,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    15,    16,     0,    75,
      67,    73,    66,    65,    60,    63,    64,    69,    70,    71,
      72,    59,    61,    62,    86,    88,    87,    74,     0,    80,
      81,     0,     0
};

static const short yydefgoto[] =
{
       1,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    29,    64,    27,    75,    76,   118,    77,
      78,    79
};

static const short yypact[] =
{
  -32768,    10,    22,-32768,-32768,   -87,   -52,    -9,   -18,   -14,
      40,-32768,     5,-32768,-32768,    -5,   -12,    60,-32768,     8,
  -32768,    69,-32768,   -71,    27,-32768,    86,-32768,   -48,    -7,
      -6,    23,    -3,   -35,-32768,-32768,    87,-32768,-32768,-32768,
      -3,    -3,-32768,     2,-32768,-32768,    88,-32768,-32768,-32768,
  -32768,-32768,    89,-32768,   -29,-32768,-32768,-32768,-32768,-32768,
      24,    83,-32768,    91,-32768,   -53,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,    23,    -3,    44,    33,   -31,    15,-32768,
      44,   -66,   -17,-32768,   -66,   -66,-32768,    44,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    95,    34,
     -55,    23,    23,    -3,    -3,    -3,    -3,    -3,    -3,    -3,
      -3,    -3,    -3,    90,   -30,    21,-32768,-32768,   -10,-32768,
  -32768,-32768,    71,-32768,   -66,   -66,   -66,   -40,   -40,-32768,
  -32768,   -66,   -66,   -66,-32768,-32768,-32768,-32768,    -3,   -66,
     -66,   112,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   123,    35,-32768,-32768,
  -32768,-32768
};


#define	YYLAST		225


static const short yytable[] =
{
      65,    66,    67,    62,    65,    66,    67,    65,    66,    67,
     141,    30,    65,    66,    67,    37,    38,    39,    47,    48,
      49,    68,    69,    70,    71,    65,    66,    67,    72,    68,
      69,    70,    71,    65,    66,    67,    86,    55,    56,    57,
      28,    91,    68,    69,    70,    71,    60,    31,    35,   114,
      68,    69,    70,    71,    53,    54,   101,   102,    36,    61,
     106,   107,   108,   109,   103,   104,   105,    81,    45,   101,
     102,   106,   107,   108,   109,    84,    85,    51,    46,   101,
     102,   121,    97,    33,    98,   116,   117,    52,   108,   109,
      34,    32,    44,    43,    59,    82,    90,    83,    88,    89,
      95,    96,    63,     2,   113,   119,   135,   102,   134,   100,
      40,    41,   142,     0,     3,     0,     0,     0,     4,    92,
      93,    94,     0,     0,     0,    74,   137,    73,   138,    74,
       0,     0,    74,    42,     0,    73,    50,    74,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,    73,     0,
      74,     0,     0,   139,    80,    58,    73,   136,    74,   106,
     107,   108,   109,   110,   111,   112,    87,     0,   120,     0,
       0,     0,     0,   140,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,     0,
       0,   115,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   122,   123
};

static const short yycheck[] =
{
      10,    11,    12,    10,    10,    11,    12,    10,    11,    12,
       0,    98,    10,    11,    12,    10,    11,    12,    10,    11,
      12,    27,    28,    29,    30,    10,    11,    12,    34,    27,
      28,    29,    30,    10,    11,    12,    34,    10,    11,    12,
      18,    17,    27,    28,    29,    30,    94,    99,     8,    34,
      27,    28,    29,    30,   125,   126,    35,    36,    18,   107,
     126,   127,   128,   129,    31,    32,    33,    32,     8,    35,
      36,   126,   127,   128,   129,    40,    41,     8,    18,    35,
      36,   136,   135,   101,   137,   102,   103,    18,   128,   129,
     104,   100,   104,    98,     8,   130,   125,    10,    10,    10,
      17,    10,   109,    93,   135,    10,   136,    36,    18,    74,
     105,   106,     0,    -1,   104,    -1,    -1,    -1,   108,    95,
      96,    97,    -1,    -1,    -1,   135,   136,   133,   138,   135,
      -1,    -1,   135,   128,    -1,   133,   128,   135,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   133,    -1,
     135,    -1,    -1,   118,    31,   128,   133,   136,   135,   126,
     127,   128,   129,   130,   131,   132,    43,    -1,   134,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102
};
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

case 6:
#line 172 ".\\rdodpt.y"
{ yyval = (int)(new RDODPTSearch((string *)yyvsp[-1])); ;
    break;}
case 7:
#line 173 ".\\rdodpt.y"
{ yyval = (int)(new RDODPTSearch((string *)yyvsp[-2], DPTnotrace)); ;
    break;}
case 8:
#line 174 ".\\rdodpt.y"
{ yyval = (int)(new RDODPTSearch((string *)yyvsp[-2], DPTtracestat)); ;
    break;}
case 9:
#line 175 ".\\rdodpt.y"
{ yyval = (int)(new RDODPTSearch((string *)yyvsp[-2], DPTtracetops)); ;
    break;}
case 10:
#line 176 ".\\rdodpt.y"
{ yyval = (int)(new RDODPTSearch((string *)yyvsp[-2], DPTtraceall)); ;
    break;}
case 11:
#line 178 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->setCondition((RDOFUNLogic *)yyvsp[0]); ;
    break;}
case 12:
#line 179 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->setCondition(); ;
    break;}
case 13:
#line 181 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->setTermCondition((RDOFUNLogic *)yyvsp[0]); ;
    break;}
case 14:
#line 183 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->setEvaluateBy((RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 15:
#line 185 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->setCompareTops(false); ;
    break;}
case 16:
#line 186 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->setCompareTops(true); ;
    break;}
case 19:
#line 191 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->addNewActivity((string *)yyvsp[-1], (string *)yyvsp[0]); ;
    break;}
case 21:
#line 194 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->addActivityParam((int)yyvsp[0]); ;
    break;}
case 22:
#line 195 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->addActivityParam((double *)yyvsp[0]); ;
    break;}
case 23:
#line 196 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->addActivityParam((string *)yyvsp[0]); ;
    break;}
case 24:
#line 197 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->addActivityParam(); ;
    break;}
case 25:
#line 199 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->setActivityValue(DPT_value_before, (RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 26:
#line 200 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->setActivityValue(DPT_value_after, (RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 27:
#line 202 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->end();;
    break;}
case 28:
#line 207 ".\\rdodpt.y"
{ yyval = (int)(new RDODPTSome((string *)yyvsp[-1])); ;
    break;}
case 29:
#line 208 ".\\rdodpt.y"
{ yyval = (int)(new RDODPTSome((string *)yyvsp[-2])); ;
    break;}
case 30:
#line 210 ".\\rdodpt.y"
{ ((RDODPTSome *)yyval)->setCondition((RDOFUNLogic *)yyvsp[0]); ;
    break;}
case 31:
#line 211 ".\\rdodpt.y"
{ ((RDODPTSome *)yyval)->setCondition(); ;
    break;}
case 34:
#line 216 ".\\rdodpt.y"
{ ((RDODPTSome *)yyval)->addNewActivity((string *)yyvsp[-1], (string *)yyvsp[0]); ;
    break;}
case 36:
#line 219 ".\\rdodpt.y"
{ ((RDODPTSome *)yyval)->addActivityParam((int)yyvsp[0]); ;
    break;}
case 37:
#line 220 ".\\rdodpt.y"
{ ((RDODPTSome *)yyval)->addActivityParam((double *)yyvsp[0]); ;
    break;}
case 38:
#line 221 ".\\rdodpt.y"
{ ((RDODPTSome *)yyval)->addActivityParam((string *)yyvsp[0]); ;
    break;}
case 39:
#line 222 ".\\rdodpt.y"
{ ((RDODPTSome *)yyval)->addActivityParam(); ;
    break;}
case 40:
#line 224 ".\\rdodpt.y"
{ ((RDODPTSome *)yyval)->end();;
    break;}
case 41:
#line 229 ".\\rdodpt.y"
{ yyval = NULL; ;
    break;}
case 42:
#line 230 ".\\rdodpt.y"
{ ((RDODPTFreeActivity *)yyvsp[0])->end();;
    break;}
case 43:
#line 232 ".\\rdodpt.y"
{ yyval = (int)(new RDODPTFreeActivity((string *)yyvsp[-1], (string *)yyvsp[0])); ;
    break;}
case 45:
#line 235 ".\\rdodpt.y"
{ ((RDODPTFreeActivity *)yyvsp[-1])->addHotKey((string *)yyvsp[0]); ;
    break;}
case 46:
#line 236 ".\\rdodpt.y"
{ ((RDODPTFreeActivity *)yyvsp[-2])->addHotKey((string *)yyvsp[0]); ;
    break;}
case 47:
#line 238 ".\\rdodpt.y"
{ ((RDODPTFreeActivity *)yyvsp[-1])->addParam((int)yyvsp[0]); ;
    break;}
case 48:
#line 239 ".\\rdodpt.y"
{ ((RDODPTFreeActivity *)yyvsp[-1])->addParam((double *)yyvsp[0]); ;
    break;}
case 49:
#line 240 ".\\rdodpt.y"
{ ((RDODPTFreeActivity *)yyvsp[-1])->addParam((string *)yyvsp[0]); ;
    break;}
case 50:
#line 241 ".\\rdodpt.y"
{ ((RDODPTFreeActivity *)yyvsp[-1])->addParam(); ;
    break;}
case 52:
#line 244 ".\\rdodpt.y"
{ if(yyvsp[-1] != NULL) ((RDODPTFreeActivity *)yyvsp[-1])->end(); ;
    break;}
case 53:
#line 249 ".\\rdodpt.y"
{;
    break;}
case 56:
#line 254 ".\\rdodpt.y"
{ yyval=yyvsp[0]; TRACE( "%s\n", ((string *)yyvsp[0])->c_str() ); ;
    break;}
case 57:
#line 255 ".\\rdodpt.y"
{
										yyval=yyvsp[0];
										TRACE( "SIEZE found, resource name = %s\n", ((string *)yyvsp[0])->c_str() );
	std::string res_name( *(string*)yyvsp[0] );
	std::string res_type_name = "RTP_" + res_name;

	const RDORTPResType* res_type = currParser->findRTPResType( &res_type_name );
	if ( res_type ) {

	} else {
//		currParser->error(("Second appearance of the same resource type : " + *(name)).c_str());
		RDORTPResType* res_type = new RDORTPResType( &res_type_name, true, currParser->resourceTypeCounter++ );
		currParser->allRTPResType.push_back( res_type );
		currParser->lastRTPResType = res_type;

//		if( currParser->findRSSResource(name) )
//			currParser->error(("Double resource name: " + *name).c_str());

		RDORSSResource* res = new RDORSSResource( &res_name, res_type, currParser->resourceCounter++ );
		currParser->lastRSSResource = res;
		currParser->allRSSResource.push_back( res );

		yyval = (int)res_type;
	}
									;
    break;}
case 58:
#line 281 ".\\rdodpt.y"
{;
    break;}
case 59:
#line 287 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] == *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 60:
#line 288 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] != *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 61:
#line 289 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] <  *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 62:
#line 290 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] >  *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 63:
#line 291 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] <= *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 64:
#line 292 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] >= *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 65:
#line 293 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNLogic *)yyvsp[-2] && *(RDOFUNLogic *)yyvsp[0]); ;
    break;}
case 66:
#line 294 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNLogic *)yyvsp[-2] || *(RDOFUNLogic *)yyvsp[0]); ;
    break;}
case 67:
#line 295 ".\\rdodpt.y"
{ yyval = yyvsp[-1]; ;
    break;}
case 68:
#line 296 ".\\rdodpt.y"
{ yyval = yyvsp[0]; ;
    break;}
case 69:
#line 299 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] + *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 70:
#line 300 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] - *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 71:
#line 301 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] * *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 72:
#line 302 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] / *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 73:
#line 303 ".\\rdodpt.y"
{ yyval = yyvsp[-1]; ;
    break;}
case 74:
#line 304 ".\\rdodpt.y"
{ yyval = (int)((RDOFUNParams *)yyvsp[-1])->createCall((string *)yyvsp[-3]) ;
    break;}
case 75:
#line 305 ".\\rdodpt.y"
{ yyval = (int)(new RDOFUNArithm((string *)yyvsp[-2], (string *)yyvsp[0])); ;
    break;}
case 76:
#line 306 ".\\rdodpt.y"
{ yyval = (int)(new RDOFUNArithm((int)yyvsp[0])); ;
    break;}
case 77:
#line 307 ".\\rdodpt.y"
{ yyval = (int)(new RDOFUNArithm((double*)yyvsp[0])); ;
    break;}
case 78:
#line 308 ".\\rdodpt.y"
{ yyval = (int)(new RDOFUNArithm((string *)yyvsp[0])); ;
    break;}
case 79:
#line 310 ".\\rdodpt.y"
{ yyval = (int)(new RDOFUNParams()); ;
    break;}
case 80:
#line 311 ".\\rdodpt.y"
{ yyval = (int)(((RDOFUNParams *)yyvsp[-1])->addParameter((RDOFUNArithm *)yyvsp[0])); ;
    break;}
case 81:
#line 312 ".\\rdodpt.y"
{ yyval = (int)(((RDOFUNParams *)yyvsp[-2])->addParameter((RDOFUNArithm *)yyvsp[0])); ;
    break;}
case 82:
#line 315 ".\\rdodpt.y"
{ yyval = 1; ;
    break;}
case 83:
#line 316 ".\\rdodpt.y"
{ yyval = 2; ;
    break;}
case 84:
#line 317 ".\\rdodpt.y"
{ yyval = 3; ;
    break;}
case 85:
#line 318 ".\\rdodpt.y"
{ yyval = 4; ;
    break;}
case 86:
#line 320 ".\\rdodpt.y"
{ yyval = (int)(new RDOFUNGroup(yyvsp[-2], (string *)yyvsp[0])); ;
    break;}
case 87:
#line 322 ".\\rdodpt.y"
{ yyval = (int)(((RDOFUNGroup *)yyvsp[-2])->createFunLogin((RDOFUNLogic *)yyvsp[-1])); ;
    break;}
case 88:
#line 323 ".\\rdodpt.y"
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
#line 330 ".\\rdodpt.y"


}
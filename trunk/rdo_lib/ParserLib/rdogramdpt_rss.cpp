/* A Bison parser, made from .\rdodpt_rss.y
   by GNU bison 1.34.  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse dpt_rss_parse
#define yylex dpt_rss_lex
#define yyerror dpt_rss_error
#define yylval dpt_rss_lval
#define yychar dpt_rss_char
#define yydebug dpt_rss_debug
#define yynerrs dpt_rss_nerrs
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

#line 1 ".\\rdodpt_rss.y"

#define YYPARSE_PARAM lexer
#define YYLEX_PARAM lexer
#line 139 ".\\rdodpt_rss.y"

#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoparser.h"
#include "rdofun.h"
#include "rdodpt.h"
#include "rdortp.h"
#include "rdorss.h"

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



#define	YYFINAL		143
#define	YYFLAG		-32768
#define	YYNTBASE	140

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 415 ? yytranslate[x] : 172)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     136,   137,   129,   127,   139,   128,   138,   130,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     132,   131,   133,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   134,     2,   135,     2,     2,     2,     2,     2,     2,
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
     121,   122,   123,   124,   125,   126
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     1,     4,     7,    10,    13,    17,    22,    27,
      32,    37,    41,    45,    49,    53,    58,    63,    66,    68,
      72,    74,    77,    80,    83,    86,    90,    94,    97,   101,
     106,   110,   114,   117,   119,   123,   125,   128,   131,   134,
     137,   140,   142,   144,   148,   150,   153,   157,   160,   163,
     166,   169,   171,   174,   178,   182,   186,   190,   194,   198,
     202,   206,   210,   212,   216,   220,   224,   228,   232,   237,
     241,   243,   245,   247,   248,   251,   255,   257,   259,   261,
     263,   267,   271,   275,   278,   279,   282,   284,   286,   288,
     291
};
static const short yyrhs[] =
{
      -1,   140,   150,     0,   140,   156,     0,   140,   161,     0,
     140,   171,     0,    93,    18,    94,     0,    93,    18,    94,
      17,     0,    93,    18,    94,    95,     0,    93,    18,    94,
      96,     0,    93,    18,    94,    97,     0,   141,    98,   162,
       0,   141,    98,    34,     0,   142,    99,   162,     0,   143,
     100,   163,     0,   144,   101,   131,   102,     0,   144,   101,
     131,   103,     0,   145,   104,     0,   149,     0,   146,    18,
      10,     0,   147,     0,   148,    11,     0,   148,    12,     0,
     148,    10,     0,   148,   129,     0,   148,   105,   163,     0,
     148,   106,   163,     0,   146,     8,     0,    93,    18,   107,
       0,    93,    18,   107,    17,     0,   151,    98,   162,     0,
     151,    98,    34,     0,   152,   104,     0,   155,     0,   153,
      18,    10,     0,   154,     0,   155,    11,     0,   155,    12,
       0,   155,    10,     0,   155,   129,     0,   153,     8,     0,
     104,     0,   160,     0,   157,    18,    10,     0,   158,     0,
     159,   126,     0,   159,   127,   126,     0,   160,    11,     0,
     160,    12,     0,   160,    10,     0,   160,   129,     0,   159,
       0,   157,     8,     0,   163,   131,   163,     0,   163,    31,
     163,     0,   163,   132,   163,     0,   163,   133,   163,     0,
     163,    32,   163,     0,   163,    33,   163,     0,   162,    36,
     162,     0,   162,    35,   162,     0,   134,   162,   135,     0,
     167,     0,   163,   127,   163,     0,   163,   128,   163,     0,
     163,   129,   163,     0,   163,   130,   163,     0,   136,   163,
     137,     0,    10,   136,   164,   137,     0,    10,   138,    10,
       0,    11,     0,    12,     0,    10,     0,     0,   164,   163,
       0,   164,   139,   163,     0,    27,     0,    28,     0,    29,
       0,    30,     0,   165,   136,    18,     0,   166,   162,   137,
       0,   166,    34,   137,     0,   108,   169,     0,     0,   169,
     170,     0,    10,     0,   110,     0,   109,     0,   109,    10,
       0,   168,     8,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   168,   169,   170,   171,   172,   176,   177,   178,   179,
     180,   182,   183,   185,   187,   189,   190,   192,   193,   195,
     197,   198,   199,   200,   201,   203,   204,   206,   210,   211,
     213,   214,   216,   217,   219,   221,   222,   223,   224,   225,
     227,   231,   232,   234,   236,   237,   238,   240,   241,   242,
     243,   244,   246,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   272,   273,   274,   276,   277,   278,   279,
     281,   283,   284,   288,   290,   291,   293,   294,   304,   311,
     484
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
  "QUOTED_IDENTIF", "'+'", "'-'", "'*'", "'/'", "'='", "'<'", "'>'", 
  "'['", "']'", "'('", "')'", "'.'", "','", "dptrtp_main", 
  "dpt_begin_search", "dpt_condition_search", "dpt_term_search", 
  "dpt_evaluate_search", "dpt_compare_search", "dpt_activ_search", 
  "dpt_activ_search_descr", "dpt_activ_search_descr_param", 
  "dpt_activ_search_descr_value", "dpt_activ_search_end", 
  "dpt_begin_some", "dpt_condition_some", "dpt_activ_some", 
  "dpt_activ_some_descr", "dpt_activ_some_descr_param", 
  "dpt_activ_some_end", "dpt_activ_free", "dpt_activ_free_descr", 
  "dpt_activ_free_descr_keyb", "dpt_activ_free_descr_param", 
  "dpt_activ_free_end", "dpt_logic", "dpt_arithm", 
  "dpt_arithm_func_call_pars", "fun_group_keyword", "fun_group_header", 
  "fun_group", "dpt_process", "dpt_process_input", "dpt_process_line", 
  "dpt_process_end", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,   140,   140,   140,   140,   140,   141,   141,   141,   141,
     141,   142,   142,   143,   144,   145,   145,   146,   146,   147,
     148,   148,   148,   148,   148,   149,   149,   150,   151,   151,
     152,   152,   153,   153,   154,   155,   155,   155,   155,   155,
     156,   157,   157,   158,   159,   159,   159,   160,   160,   160,
     160,   160,   161,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   164,   164,   164,   165,   165,   165,   165,
     166,   167,   167,   168,   169,   169,   170,   170,   170,   170,
     171
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     0,     2,     2,     2,     2,     3,     4,     4,     4,
       4,     3,     3,     3,     3,     4,     4,     2,     1,     3,
       1,     2,     2,     2,     2,     3,     3,     2,     3,     4,
       3,     3,     2,     1,     3,     1,     2,     2,     2,     2,
       2,     1,     1,     3,     1,     2,     3,     2,     2,     2,
       2,     1,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     3,     3,     3,     3,     3,     4,     3,
       1,     1,     1,     0,     2,     3,     1,     1,     1,     1,
       3,     3,     3,     2,     0,     2,     1,     1,     1,     2,
       2
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       1,     0,     0,    41,    84,     0,     0,     0,     0,     0,
       0,    20,     0,    18,     2,     0,     0,     0,    35,    33,
       3,     0,    44,    51,    42,     4,     0,     5,     0,    83,
       0,     0,     0,     0,    17,    27,     0,    23,    21,    22,
       0,     0,    24,     0,    32,    40,     0,    38,    36,    37,
      39,    52,     0,    45,     0,    49,    47,    48,    50,    90,
       6,    28,    86,    88,    87,    85,    72,    70,    71,    76,
      77,    78,    79,    12,     0,     0,    11,     0,     0,     0,
      62,    13,    14,     0,    19,    25,    26,    31,    30,    34,
      43,    46,     7,     8,     9,    10,    29,    89,    73,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    15,    16,     0,
      69,    61,    67,    60,    59,    54,    57,    58,    63,    64,
      65,    66,    53,    55,    56,    80,    82,    81,    68,     0,
      74,    75,     0,     0
};

static const short yydefgoto[] =
{
       1,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    76,    77,   119,    78,    79,    80,    26,    29,
      65,    27
};

static const short yypact[] =
{
  -32768,    10,   -15,-32768,-32768,   -87,   -31,   -29,   -22,   -19,
      52,-32768,     5,-32768,-32768,   -11,    -9,    54,-32768,     8,
  -32768,    55,-32768,   -62,    27,-32768,    85,-32768,   -46,    30,
      -6,    23,    -3,   -37,-32768,-32768,    88,-32768,-32768,-32768,
      -3,    -3,-32768,     2,-32768,-32768,    89,-32768,-32768,-32768,
  -32768,-32768,    90,-32768,   -25,-32768,-32768,-32768,-32768,-32768,
      24,    87,-32768,    92,-32768,-32768,   -69,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,    23,    -3,    48,    49,   -30,    15,
  -32768,    48,   -52,   -13,-32768,   -52,   -52,-32768,    48,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    95,
      19,   -71,    23,    23,    -3,    -3,    -3,    -3,    -3,    -3,
      -3,    -3,    -3,    -3,    91,   -24,    11,-32768,-32768,   -10,
  -32768,-32768,-32768,    71,-32768,   -52,   -52,   -52,   -38,   -38,
  -32768,-32768,   -52,   -52,   -52,-32768,-32768,-32768,-32768,    -3,
     -52,   -52,   108,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,    43,    56,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768
};


#define	YYLAST		195


static const short yytable[] =
{
      66,    67,    68,    28,    66,    67,    68,    66,    67,    68,
     142,    30,    66,    67,    68,    37,    38,    39,    47,    48,
      49,    69,    70,    71,    72,    66,    67,    68,    73,    69,
      70,    71,    72,    66,    67,    68,    87,    55,    56,    57,
      62,    92,    69,    70,    71,    72,   102,   103,    60,   115,
      69,    70,    71,    72,   102,   103,   107,   108,   109,   110,
      35,    61,    45,    51,    53,    54,   122,    98,    31,    99,
      36,    32,    46,    52,    81,   107,   108,   109,   110,    33,
     104,   105,   106,   102,   103,    34,    88,    43,    82,   117,
     118,   109,   110,    59,    83,    44,    85,    86,    84,    89,
      90,    91,    97,     2,    96,   120,   114,   103,   143,   135,
      40,    41,     0,   136,     3,     0,     0,   100,     4,    93,
      94,    95,   116,     0,     0,     0,    75,   138,    74,   139,
      75,   101,     0,    75,    42,     0,    74,    50,    75,    63,
      64,     0,     0,     0,     0,   123,   124,     0,   137,    74,
       0,    75,     0,     0,   121,     0,    58,    74,     0,    75,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
       0,     0,     0,     0,     0,   140,   107,   108,   109,   110,
     111,   112,   113,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   141
};

static const short yycheck[] =
{
      10,    11,    12,    18,    10,    11,    12,    10,    11,    12,
       0,    98,    10,    11,    12,    10,    11,    12,    10,    11,
      12,    27,    28,    29,    30,    10,    11,    12,    34,    27,
      28,    29,    30,    10,    11,    12,    34,    10,    11,    12,
      10,    17,    27,    28,    29,    30,    35,    36,    94,    34,
      27,    28,    29,    30,    35,    36,   127,   128,   129,   130,
       8,   107,     8,     8,   126,   127,   137,   136,    99,   138,
      18,   100,    18,    18,    31,   127,   128,   129,   130,   101,
      31,    32,    33,    35,    36,   104,    43,    98,    32,   102,
     103,   129,   130,     8,   131,   104,    40,    41,    10,    10,
      10,   126,    10,    93,    17,    10,   136,    36,     0,    18,
     105,   106,    -1,   137,   104,    -1,    -1,    74,   108,    95,
      96,    97,    79,    -1,    -1,    -1,   136,   137,   134,   139,
     136,    75,    -1,   136,   129,    -1,   134,   129,   136,   109,
     110,    -1,    -1,    -1,    -1,   102,   103,    -1,   137,   134,
      -1,   136,    -1,    -1,   135,    -1,   129,   134,    -1,   136,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      -1,    -1,    -1,    -1,    -1,   119,   127,   128,   129,   130,
     131,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139
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

case 6:
#line 176 ".\\rdodpt_rss.y"
{;
    break;}
case 7:
#line 177 ".\\rdodpt_rss.y"
{;
    break;}
case 8:
#line 178 ".\\rdodpt_rss.y"
{;
    break;}
case 9:
#line 179 ".\\rdodpt_rss.y"
{;
    break;}
case 10:
#line 180 ".\\rdodpt_rss.y"
{;
    break;}
case 11:
#line 182 ".\\rdodpt_rss.y"
{;
    break;}
case 12:
#line 183 ".\\rdodpt_rss.y"
{;
    break;}
case 13:
#line 185 ".\\rdodpt_rss.y"
{;
    break;}
case 14:
#line 187 ".\\rdodpt_rss.y"
{;
    break;}
case 15:
#line 189 ".\\rdodpt_rss.y"
{;
    break;}
case 16:
#line 190 ".\\rdodpt_rss.y"
{;
    break;}
case 19:
#line 195 ".\\rdodpt_rss.y"
{;
    break;}
case 21:
#line 198 ".\\rdodpt_rss.y"
{;
    break;}
case 22:
#line 199 ".\\rdodpt_rss.y"
{;
    break;}
case 23:
#line 200 ".\\rdodpt_rss.y"
{;
    break;}
case 24:
#line 201 ".\\rdodpt_rss.y"
{;
    break;}
case 25:
#line 203 ".\\rdodpt_rss.y"
{;
    break;}
case 26:
#line 204 ".\\rdodpt_rss.y"
{;
    break;}
case 27:
#line 206 ".\\rdodpt_rss.y"
{;
    break;}
case 28:
#line 210 ".\\rdodpt_rss.y"
{;
    break;}
case 29:
#line 211 ".\\rdodpt_rss.y"
{;
    break;}
case 30:
#line 213 ".\\rdodpt_rss.y"
{;
    break;}
case 31:
#line 214 ".\\rdodpt_rss.y"
{;
    break;}
case 34:
#line 219 ".\\rdodpt_rss.y"
{;
    break;}
case 36:
#line 222 ".\\rdodpt_rss.y"
{;
    break;}
case 37:
#line 223 ".\\rdodpt_rss.y"
{;
    break;}
case 38:
#line 224 ".\\rdodpt_rss.y"
{;
    break;}
case 39:
#line 225 ".\\rdodpt_rss.y"
{;
    break;}
case 40:
#line 227 ".\\rdodpt_rss.y"
{;
    break;}
case 41:
#line 231 ".\\rdodpt_rss.y"
{;
    break;}
case 42:
#line 232 ".\\rdodpt_rss.y"
{;
    break;}
case 43:
#line 234 ".\\rdodpt_rss.y"
{;
    break;}
case 45:
#line 237 ".\\rdodpt_rss.y"
{;
    break;}
case 46:
#line 238 ".\\rdodpt_rss.y"
{;
    break;}
case 47:
#line 240 ".\\rdodpt_rss.y"
{;
    break;}
case 48:
#line 241 ".\\rdodpt_rss.y"
{;
    break;}
case 49:
#line 242 ".\\rdodpt_rss.y"
{;
    break;}
case 50:
#line 243 ".\\rdodpt_rss.y"
{;
    break;}
case 52:
#line 246 ".\\rdodpt_rss.y"
{;
    break;}
case 53:
#line 250 ".\\rdodpt_rss.y"
{;
    break;}
case 54:
#line 251 ".\\rdodpt_rss.y"
{;
    break;}
case 55:
#line 252 ".\\rdodpt_rss.y"
{;
    break;}
case 56:
#line 253 ".\\rdodpt_rss.y"
{;
    break;}
case 57:
#line 254 ".\\rdodpt_rss.y"
{;
    break;}
case 58:
#line 255 ".\\rdodpt_rss.y"
{;
    break;}
case 59:
#line 256 ".\\rdodpt_rss.y"
{;
    break;}
case 60:
#line 257 ".\\rdodpt_rss.y"
{;
    break;}
case 61:
#line 258 ".\\rdodpt_rss.y"
{;
    break;}
case 62:
#line 259 ".\\rdodpt_rss.y"
{;
    break;}
case 63:
#line 261 ".\\rdodpt_rss.y"
{;
    break;}
case 64:
#line 262 ".\\rdodpt_rss.y"
{;
    break;}
case 65:
#line 263 ".\\rdodpt_rss.y"
{;
    break;}
case 66:
#line 264 ".\\rdodpt_rss.y"
{;
    break;}
case 67:
#line 265 ".\\rdodpt_rss.y"
{;
    break;}
case 68:
#line 266 ".\\rdodpt_rss.y"
{;
    break;}
case 69:
#line 267 ".\\rdodpt_rss.y"
{;
    break;}
case 70:
#line 268 ".\\rdodpt_rss.y"
{;
    break;}
case 71:
#line 269 ".\\rdodpt_rss.y"
{;
    break;}
case 72:
#line 270 ".\\rdodpt_rss.y"
{;
    break;}
case 73:
#line 272 ".\\rdodpt_rss.y"
{;
    break;}
case 74:
#line 273 ".\\rdodpt_rss.y"
{;
    break;}
case 75:
#line 274 ".\\rdodpt_rss.y"
{;
    break;}
case 76:
#line 276 ".\\rdodpt_rss.y"
{;
    break;}
case 77:
#line 277 ".\\rdodpt_rss.y"
{;
    break;}
case 78:
#line 278 ".\\rdodpt_rss.y"
{;
    break;}
case 79:
#line 279 ".\\rdodpt_rss.y"
{;
    break;}
case 80:
#line 281 ".\\rdodpt_rss.y"
{;
    break;}
case 81:
#line 283 ".\\rdodpt_rss.y"
{;
    break;}
case 82:
#line 284 ".\\rdodpt_rss.y"
{;
    break;}
case 83:
#line 288 ".\\rdodpt_rss.y"
{;
    break;}
case 86:
#line 293 ".\\rdodpt_rss.y"
{ TRACE( "IDENTIF %s\n", ((std::string *)yyvsp[0])->c_str() ); ;
    break;}
case 87:
#line 294 ".\\rdodpt_rss.y"
{

	std::string* ie_name      = currParser->registerName( "PAT_GENERATE" );
	std::string* rel_res_name = currParser->registerName( "Транзакт" );
	std::string* rtp_name     = currParser->registerName( "Транзакты" );
	RDOPATPatternEvent* ie = new RDOPATPatternEvent( ie_name, true );
	ie->addRelRes( rel_res_name, rtp_name, RDOPATPattern::CS_Create );
	ie->end();

					;
    break;}
case 88:
#line 304 ".\\rdodpt_rss.y"
{
	yyloc.first_column = yylsp[0].first_column;
	yyloc.first_line   = yylsp[0].first_line;
	yyloc.last_column  = yylsp[0].last_column;
	yyloc.last_line    = yylsp[0].first_column;
	currParser->error( std::string(_T("Ожидается имя ресурса")).c_str() );
;
    break;}
case 89:
#line 311 ".\\rdodpt_rss.y"
{

	std::string* res_name       = (std::string*)yyvsp[0];
	std::string* rtp_param_name = currParser->registerName( "Состояние" );
	std::string* rtp_state_free = currParser->registerName( "Свободен" );
	std::string* rtp_state_buzy = currParser->registerName( "Занят" );

	// Найти ресурс, если его нет, то создать
	RDORSSResource* res = const_cast<RDORSSResource*>(currParser->findRSSResource( res_name ));
	if ( !res ) {

		// Сформировать имя типа по имени ресурса
		std::string* res_type_name = currParser->registerName( std::string( RDODPTProcess::name_prefix + *res_name + RDODPTProcess::name_sufix ).c_str() );

		// Найти тип ресурса, если его нет, то создать тип и положить в него параметр
		RDORTPParamDesc* rtp_param = NULL;
		RDORTPResType* res_type = const_cast<RDORTPResType*>(currParser->findRTPResType( res_type_name ));
		if ( !res_type ) {
			// Создадим тип ресурса
			res_type = new RDORTPResType( res_type_name, true );

			// Создадим параметр перечислимого типа
			RDORTPEnum* state_enum = new RDORTPEnum( rtp_state_free );
			state_enum->add( rtp_state_buzy );

			RDORTPEnumDefVal* state_default = new RDORTPEnumDefVal( rtp_state_free );

			RDORTPEnumResParam* rtp_param_enum = new RDORTPEnumResParam( state_enum, state_default );

			rtp_param = new RDORTPParamDesc( rtp_param_name, rtp_param_enum );
			res_type->add( rtp_param );
		} else {
			// Тип найден, проверить на наличие перечислимого параметра
			rtp_param = const_cast<RDORTPParamDesc*>(res_type->findRTPParam( rtp_param_name ));
			if ( rtp_param ) {
				// Параметр Состояние есть, надо проверить, чтобы в нем были значения Свободен и Занят
				// Для начала проверим тип параметра
				if ( rtp_param->getType()->getType() == RDORTPResParam::pt_enum ) {
					// Теперь проверим сами значения
					try {
						rtp_param->getType()->getRSSEnumValue( rtp_state_free );
						rtp_param->getType()->getRSSEnumValue( rtp_state_buzy );
					} catch ( RDOSyntaxException& ) {
						currParser->error( rdo::format( "У типа ресурса '%s' перечислимый параметр '%s' должен иметь как минимум два обязательных значения: %s и %s", res_type->getName()->c_str(), rtp_param_name->c_str(), rtp_state_free->c_str(), rtp_state_buzy->c_str() ) );
					}
				} else {
					// Параметр Состояние есть, но он не перечислимого типа
					currParser->error( rdo::format( "У типа ресурса '%s' параметр '%s' не является перечислимым типом", res_type->getName()->c_str(), rtp_param_name->c_str() ) );
				}
			} else {
				currParser->error( rdo::format( "У типа ресурса '%s' нет параметра перечислимого типа '%s'", res_type->getName()->c_str(), rtp_param_name->c_str() ) );
			}
		}

		// Создать ресурс
		RDORSSResource* res = new RDORSSResource( res_name, res_type );
		res->setTrace( true );

		// Пропишем значения параметров перечислимого типа по-умолчанию
		RDOValue state_val = rtp_param->getType()->getRSSDefaultValue();
		res->addValue( state_val );
		res->currParam++;

		// Пропишем значения параметров (всех) для созданного ресурса. Берутся как значения по-умолчанию
//		const std::vector<const RDORTPParamDesc *>& res_params = res->getType()->getParams();
//		res->currParam = res_params.begin();
//		while ( res->currParam != res_params.end() ) {
//			RDOValue res_param_val = (*res->currParam)->getType()->getRSSDefaultValue();
//			res->addValue( res_param_val );
//			res->currParam++;
//		}

	} else {
		// Ресурс найден, найдем для него тип
		RDORTPResType* res_type = const_cast<RDORTPResType*>(currParser->findRTPResType( res->getType()->getName() ));
		if ( !res_type ) {
			// Тип не найден, выдай сообщение об ошибке
			currParser->error( rdo::format( "Для ресурса %s не наден тип %s", res_name->c_str(), res_type->getName()->c_str() ) );
		}
		RDORTPParamDesc* rtp_param = const_cast<RDORTPParamDesc*>(res_type->findRTPParam( rtp_param_name ));
		if ( !rtp_param ) {
			// Не найден перечислимый параметр Состояние, выдай сообщение об ошибке
			currParser->error( rdo::format( "Для типа ресурса %s не наден параметр перечислимого типа %s", res_type->getName()->c_str(), rtp_param_name->c_str() ) );
		}
		// Параметр Состояние есть, надо проверить, чтобы в нем были значения Свободен и Занят
		// Для начала проверим тип параметра
		if ( rtp_param->getType()->getType() == RDORTPResParam::pt_enum ) {
			// Теперь проверим сами значения
			try {
				rtp_param->getType()->getRSSEnumValue( rtp_state_free );
				rtp_param->getType()->getRSSEnumValue( rtp_state_buzy );
			} catch ( RDOSyntaxException& ) {
				currParser->error( rdo::format( "У типа ресурса '%s' перечислимый параметр '%s' должен иметь как минимум два обязательных значения: %s и %s", res_type->getName()->c_str(), rtp_param_name->c_str(), rtp_state_free->c_str(), rtp_state_buzy->c_str() ) );
			}
		} else {
			// Параметр Состояние есть, но он не перечислимого типа
			currParser->error( rdo::format( "У типа ресурса '%s' параметр '%s' не является перечислимым типом", res_type->getName()->c_str(), rtp_param_name->c_str() ) );
		}
	}

/*
	std::string* res_name      = (std::string*)$2;
	std::string* res_type_name = currParser->registerName( std::string( RDODPTProcess::name_prefix + *res_name + RDODPTProcess::name_sufix ).c_str() );

	// Найти тип ресурса, если его нет, то создать
	RDORTPResType* res_type = const_cast<RDORTPResType*>(currParser->findRTPResType( res_type_name ));
	if ( !res_type ) {
		res_type = new RDORTPResType( res_type_name, true, currParser->resourceTypeCounter++ );
		currParser->allRTPResType.push_back( res_type );
		currParser->lastRTPResType = res_type;
	}

	// Найти параметр Состояние внутри типа, если его нет, то создать
	std::string* rtp_param_name = currParser->registerName( "Состояние" );
	std::string* rtp_state_free = currParser->registerName( "Свободен" );
	std::string* rtp_state_bizy = currParser->registerName( "Занят" );
	bool param_added = false;
	RDORTPParamDesc* rtp_param = const_cast<RDORTPParamDesc*>(res_type->findRTPParam( rtp_param_name ));
	if ( !rtp_param ) {
		RDORTPEnum* state_enum = new RDORTPEnum( rtp_state_free );
		state_enum->add( rtp_state_bizy );

		RDORTPEnumDefVal* state_default = new RDORTPEnumDefVal( rtp_state_free );

		RDORTPEnumResParam* rtp_param_enum = new RDORTPEnumResParam( state_enum, state_default );

		rtp_param = new RDORTPParamDesc( rtp_param_name, rtp_param_enum );
		currParser->lastRTPResType->add( rtp_param );
		param_added = true;
	} else {
		// Параметр Состояние есть, надо проверить, чтобы в нем были значения Свободен и Занят
		// Для начала проверим тип параметра
		if ( rtp_param->getType()->getType() == RDORTPResParam::pt_enum ) {
			// Теперь проверим сами значения
			try {
				rtp_param->getType()->getRSSEnumValue( rtp_state_free );
				rtp_param->getType()->getRSSEnumValue( rtp_state_bizy );
			} catch ( RDOSyntaxException& ) {
				currParser->error( std::string(_T("У типа ресурса \"") + *res_type_name + "\" перечислимый параметр \"" + *rtp_param_name + "\" должен иметь как минимум два обязательных значения: " + *rtp_state_free + " и " + *rtp_state_bizy ).c_str() );
			}
		} else {
			// Параметр Состояние есть, но он не перечислимого типа
			currParser->error( std::string(_T("У типа ресурса \"") + *res_type_name + "\" параметр \"" + *rtp_param_name + "\" не является перечислимым типом" ).c_str() );
		}
	}

	// Найти ресурс, если его нет, то создать
	RDORSSResource* res = const_cast<RDORSSResource*>(currParser->findRSSResource( res_name ));
	if ( !res ) {
		res = new RDORSSResource( res_name, res_type, currParser->resourceCounter++ );
		res->setTrace( true );
		currParser->lastRSSResource = res;
		currParser->allRSSResource.push_back( res );

		// Пропишем значения параметров для созданного ресурса. Берутся как значения по-умолчанию
		const std::vector<const RDORTPParamDesc *>& res_params = res->getType()->getParams();
		res->currParam = res_params.begin();
		while ( res->currParam != res_params.end() ) {
			RDOValue res_param_val = (*res->currParam)->getType()->getRSSDefaultValue();
			res->addValue( res_param_val );
			res->currParam++;
		}
	} else if ( param_added ) {
		RDOValue state_val = rtp_param->getType()->getRSSDefaultValue();
		res->addValue( state_val );
		res->currParam++;
	} else {
		TRACE( "q" );
	}
*/

;
    break;}
case 90:
#line 484 ".\\rdodpt_rss.y"
{;
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
#line 486 ".\\rdodpt_rss.y"


}

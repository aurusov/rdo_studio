%{
#define YYPARSE_PARAM lexer
#define YYLEX_PARAM lexer
%}

%pure-parser

%token Resource_type		257
%token permanent			258
%token Parameters			259
%token integer				260
%token real					261
%token End					262
%token temporary			263
%token IDENTIF				264
%token INT_CONST			265
%token REAL_CONST			266
%token such_as				267
%token dblpoint				268
%token Resources			269
%token trace_keyword		270
%token no_trace				271
%token IDENTIF_COLON		272
%token Constant				273
%token Body					274
%token Function_keyword		275
%token Type_keyword			276
%token algorithmic			277
%token table_keyword		278
%token list_keyword			279
%token Calculate_if			280
%token Exist				281
%token Not_Exist			282
%token For_All				283
%token Not_For_All			284
%token neq					285
%token leq					286
%token geq					287
%token NoCheck				288
%token Calculate_if			289
%token or_keyword			290
%token and_keyword			291
%token Sequence				292
%token uniform				293
%token exponential			294
%token normal_keyword		295
%token by_hist				296
%token enumerative			297

%token Pattern				298
%token operation			299
%token irregular_event		300
%token rule_keyword			301
%token keyboard				302
%token Relevant_resources	303
%token Keep					304
%token Create_keyword		305
%token Erase				306
%token NonExist				307
%token IDENTIF_NoChange		308
%token Time					309
%token Choice				310
%token from_keyword			311
%token first_keyword		312
%token Convert_begin		313
%token Convert_end			314
%token Convert_rule			315
%token Convert_event		316
%token with_max				317
%token with_min				318
%token IDENTIF_set			319
%token IDENTIF_NoChange_NoChange	320
%token Operations			321
	
%token Results				322
%token watch_par			323
%token watch_state			324
%token watch_quant			325
%token watch_value			326
%token get_value			327

%token Model_name			328
%token Resource_file		329
%token OprIev_file			330
%token Frame_file			331
%token Statistic_file		332
%token Results_file			333
%token Trace_file			334
%token Show_mode			335
%token Frame_number			336
%token Show_rate			337
%token Run_StartTime		338
%token Trace_StartTime		339
%token Trace_EndTime		340
%token Terminate_if			341
%token Break_point			342
%token Seed					343
%token NoShow				344
%token Monitor				345
%token Animation			346
%token NoChange				347

%token Decision_point		348
%token search_keyword		349
%token trace_stat			350
%token trace_tops			351
%token trace_all			352
%token Condition_keyword	353
%token Term_condition		354
%token Evaluate_by			355
%token Compare_tops			356
%token NO					357
%token YES					358
%token Activities			359
%token value_before			360
%token value_after			361
%token some					362
%token Process				363
%token SIEZE				364

%token Frame				400
%token Show_if				401
%token Back_picture			402
%token Show					403
%token frm_cell				404
%token text					405
%token transparent			406
%token bitmap				407
%token s_bmp				408
%token rect_keyword			409
%token r_rect				410
%token line					411
%token ellipse				412
%token triang				413
%token active				414
%token QUOTED_IDENTIF		415

%{
#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <FlexLexer.h>

#include "rdofun.h"
#include "rdofrm.h"

namespace rdoParse 
{
%}

%left or_keyword
%left and_keyword
%left '+' '-'
%left '*' '/'

%start frm_main

%%

/* ///////////////////////  GENERAL PART ///////////////////////////// */

frm_main:	
	| frm_main frm_end;


/* ///////////////////////  FRAME ///////////////////////////// */

frm_begin: Frame IDENTIF							{ $$ = (int)(new RDOFRMFrame((std::string *)$2)); @$; }
			| Frame IDENTIF Show_if frm_logic	{ $$ = (int)(new RDOFRMFrame((std::string *)$2, (RDOFUNLogic *)$4)); };

frm_background: frm_begin Back_picture '='													{ ((RDOFRMFrame *)$1)->setBackground(); }
				  | frm_begin Back_picture '=' '<' INT_CONST INT_CONST INT_CONST '>' { ((RDOFRMFrame *)$1)->setBackground($5, $6, $7); } ;

frm_backpicture: frm_background INT_CONST INT_CONST	{ ((RDOFRMFrame *)$1)->setBackPicture($2, $3); }
					| frm_background IDENTIF					{ ((RDOFRMFrame *)$1)->setBackPicture((std::string*)$2); };

frm_pre_show: frm_backpicture
				| frm_item;

frm_show: frm_pre_show							{ ((RDOFRMFrame *)$1)->startShow(); }
		  | frm_pre_show Show					{ ((RDOFRMFrame *)$1)->startShow(); }
		  | frm_pre_show Show_if frm_logic	{ ((RDOFRMFrame *)$1)->startShow((RDOFUNLogic *)$3); };

frm_item: frm_show								
		  | frm_item frm_text					{ ((RDOFRMFrame *)$1)->addItem((RDOFRMText *)$2); }
		  | frm_item frm_bitmap					{ ((RDOFRMFrame *)$1)->addItem((RDOFRMBitmap *)$2); }
		  | frm_item frm_rect					{ ((RDOFRMFrame *)$1)->addItem((RDOFRMRect *)$2); }
		  | frm_item frm_line					{ ((RDOFRMFrame *)$1)->addItem((RDOFRMLine *)$2); }
		  | frm_item frm_ellipse				{ ((RDOFRMFrame *)$1)->addItem((RDOFRMEllipse *)$2); }
		  | frm_item frm_r_rect					{ ((RDOFRMFrame *)$1)->addItem((RDOFRMR_rect *)$2); }
		  | frm_item frm_triang					{ ((RDOFRMFrame *)$1)->addItem((RDOFRMTriang *)$2); }
		  | frm_item frm_s_bmp					{ ((RDOFRMFrame *)$1)->addItem((RDOFRMS_bmp *)$2); }
		  | frm_item frm_active					{ ((RDOFRMFrame *)$1)->addItem((RDOFRMActive *)$2); };

frm_end: frm_item End	{ ((RDOFRMFrame *)$1)->end(); };

/* ///////////////////////  ITEMS ///////////////////////////// */

frm_text_common: text '[' frm_arithm ',' frm_arithm ',' frm_arithm ',' frm_arithm ',' frm_color ',' frm_color ','  { $$ = (int)(new RDOFRMText((RDOFUNArithm *)$3, (RDOFUNArithm *)$5, (RDOFUNArithm *)$7, (RDOFUNArithm *)$9, (RDOFRMColor *)$11, (RDOFRMColor *)$13)); };

frm_text: frm_text_common frm_align frm_arithm ']'		{ ((RDOFRMText *)$1)->setText($2, (RDOFUNArithm *)$3); }
		  | frm_text_common frm_align QUOTED_IDENTIF ']'	{ ((RDOFRMText *)$1)->setText($2, (std::string *)$3); };

frm_align:		{ $$ = 1; }
		| '<'		{ $$ = 1; }
		| '='		{ $$ = 2; }
		| '>'		{ $$ = 3; };

frm_color: transparent										{ $$ = (int)(new RDOFRMColor()); }
		| '<' INT_CONST INT_CONST INT_CONST '>'		{ $$ = (int)(new RDOFRMColor($2, $3, $4)); };		  

frm_bitmap: bitmap '[' frm_arithm ',' frm_arithm ',' IDENTIF ']'				{ $$ = (int)(new RDOFRMBitmap((RDOFUNArithm *)$3, (RDOFUNArithm *)$5, (std::string *)$7)); }
		| bitmap '[' frm_arithm ',' frm_arithm ',' IDENTIF ',' IDENTIF ']'	{ $$ = (int)(new RDOFRMBitmap((RDOFUNArithm *)$3, (RDOFUNArithm *)$5, (std::string *)$7, (std::string *)$9)); };

frm_s_bmp: s_bmp '[' frm_arithm ',' frm_arithm ',' frm_arithm ',' frm_arithm ',' IDENTIF ']' 				 { $$ = (int)(new RDOFRMS_bmp((RDOFUNArithm *)$3, (RDOFUNArithm *)$5, (RDOFUNArithm *)$7, (RDOFUNArithm *)$9, (std::string *)$11)); }
		  |  s_bmp '[' frm_arithm ',' frm_arithm ',' frm_arithm ',' frm_arithm ',' IDENTIF ',' IDENTIF ']'  { $$ = (int)(new RDOFRMS_bmp((RDOFUNArithm *)$3, (RDOFUNArithm *)$5, (RDOFUNArithm *)$7, (RDOFUNArithm *)$9, (std::string *)$11, (std::string *)$13)); };

frm_rect: rect_keyword '[' frm_arithm ',' frm_arithm ',' frm_arithm ',' frm_arithm ',' frm_color ',' frm_color ']'	{ $$ = (int)(new RDOFRMRect((RDOFUNArithm *)$3, (RDOFUNArithm *)$5, (RDOFUNArithm *)$7, (RDOFUNArithm *)$9, (RDOFRMColor *)$11, (RDOFRMColor *)$13)); };

frm_r_rect: r_rect '[' frm_arithm ',' frm_arithm ',' frm_arithm ',' frm_arithm ',' frm_color ',' frm_color ']'			{ $$ = (int)(new RDOFRMR_rect((RDOFUNArithm *)$3, (RDOFUNArithm *)$5, (RDOFUNArithm *)$7, (RDOFUNArithm *)$9, (RDOFRMColor *)$11, (RDOFRMColor *)$13)); };

frm_line: line '[' frm_arithm ',' frm_arithm ',' frm_arithm ',' frm_arithm ',' frm_color ']'		{ $$ = (int)(new RDOFRMLine((RDOFUNArithm *)$3, (RDOFUNArithm *)$5, (RDOFUNArithm *)$7, (RDOFUNArithm *)$9, (RDOFRMColor *)$11)); };

frm_ellipse: ellipse '[' frm_arithm ',' frm_arithm ',' frm_arithm ',' frm_arithm ',' frm_color ',' frm_color ']'   { $$ = (int)(new RDOFRMEllipse((RDOFUNArithm *)$3, (RDOFUNArithm *)$5, (RDOFUNArithm *)$7, (RDOFUNArithm *)$9, (RDOFRMColor *)$11, (RDOFRMColor *)$13)); };

frm_triang: triang '[' frm_arithm ',' frm_arithm ',' frm_arithm ',' frm_arithm ',' frm_arithm ',' frm_arithm ',' frm_color ',' frm_color ']'		{ $$ = (int)(new RDOFRMTriang((RDOFUNArithm *)$3, (RDOFUNArithm *)$5, (RDOFUNArithm *)$7, (RDOFUNArithm *)$9, (RDOFUNArithm *)$11, (RDOFUNArithm *)$13, (RDOFRMColor *)$15, (RDOFRMColor *)$17)); };

frm_active: active IDENTIF '[' frm_arithm ',' frm_arithm ',' frm_arithm ',' frm_arithm ']'	{ $$ = (int)(new RDOFRMActive((RDOFUNArithm *)$4, (RDOFUNArithm *)$6, (RDOFUNArithm *)$8, (RDOFUNArithm *)$10, (std::string *)$2)); };


/* ///////////////////////  ARITHMETIC/LOGIC ///////////////////////////// */

frm_logic: frm_arithm '=' frm_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 == *(RDOFUNArithm *)$3); }
			| frm_arithm neq frm_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 != *(RDOFUNArithm *)$3); }
			| frm_arithm '<' frm_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 <  *(RDOFUNArithm *)$3); }
			| frm_arithm '>' frm_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 >  *(RDOFUNArithm *)$3); }
			| frm_arithm leq frm_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 <= *(RDOFUNArithm *)$3); }
			| frm_arithm geq frm_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 >= *(RDOFUNArithm *)$3); }
			| frm_logic and_keyword frm_logic  { $$ = (int)(*(RDOFUNLogic *)$1 && *(RDOFUNLogic *)$3); }
			| frm_logic or_keyword frm_logic	  { $$ = (int)(*(RDOFUNLogic *)$1 || *(RDOFUNLogic *)$3); }
			| '[' frm_logic ']'				{ $$ = $2; }
			| fun_group							{ $$ = $1; };

			
frm_arithm: frm_arithm '+' frm_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 + *(RDOFUNArithm *)$3); }
			|	frm_arithm '-' frm_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 - *(RDOFUNArithm *)$3); }
			|	frm_arithm '*' frm_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 * *(RDOFUNArithm *)$3); }
			|	frm_arithm '/' frm_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 / *(RDOFUNArithm *)$3); }
			|	'(' frm_arithm ')'			{ $$ = $2; }
			|	IDENTIF '(' frm_arithm_func_call_pars ')' { $$ = (int)((RDOFUNParams *)$3)->createCall((std::string *)$1) };
			|	IDENTIF '.' IDENTIF			{ $$ = (int)(new RDOFUNArithm((std::string *)$1, (std::string *)$3)); }
			|	INT_CONST						{ $$ = (int)(new RDOFUNArithm((int)$1)); }
			|	REAL_CONST						{ $$ = (int)(new RDOFUNArithm((double*)$1)); }
			|	IDENTIF							{ $$ = (int)(new RDOFUNArithm((std::string *)$1)); };

frm_arithm_func_call_pars:								{ $$ = (int)(new RDOFUNParams()); };
			| frm_arithm_func_call_pars frm_arithm	{ $$ = (int)(((RDOFUNParams *)$1)->addParameter((RDOFUNArithm *)$2)); };
			| frm_arithm_func_call_pars ',' frm_arithm	{ $$ = (int)(((RDOFUNParams *)$1)->addParameter((RDOFUNArithm *)$3)); };


fun_group_keyword:	Exist			{ $$ = 1; }
						|	Not_Exist	{ $$ = 2; }
						|	For_All		{ $$ = 3; }
						|	Not_For_All	{ $$ = 4; };

fun_group_header:	fun_group_keyword '(' IDENTIF_COLON { $$ = (int)(new RDOFUNGroup($1, (std::string *)$3)); };

fun_group:	fun_group_header frm_logic ')'		{ $$ = (int)(((RDOFUNGroup *)$1)->createFunLogin((RDOFUNLogic *)$2)); }
					|	fun_group_header NoCheck ')'	{ $$ = (int)(((RDOFUNGroup *)$1)->createFunLogin()); };






%%

}
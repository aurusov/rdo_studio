%{
#define YYPARSE_PARAM lexer
#define YYLEX_PARAM lexer
%}

%pure-parser

%token Resource_type		257
%token permanent_kw			258
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
%token operation_kw			299
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
%token SEIZE				364
%token GENERATE				365
%token TERMINATE			366

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

#include "rdoparser.h"
#include "rdofun.h"
#include "rdodpt.h"
#include "rdortp.h"
#include "rdorss.h"

namespace rdoParse 
{
%}

%left or_keyword
%left and_keyword
%left '+' '-'
%left '*' '/'

%start dptrtp_main

%%

/* ///////////////////////  GENERAL PART ///////////////////////////// */

dptrtp_main:
	| dptrtp_main dpt_activ_search_end
	| dptrtp_main dpt_activ_some_end
	| dptrtp_main dpt_activ_free_end;
	| dptrtp_main dpt_process_end;

/* ///////////////////////  SEARCH POINT ///////////////////////////// */

dpt_begin_search:	Decision_point IDENTIF_COLON search_keyword					{}
					|	Decision_point IDENTIF_COLON search_keyword no_trace	{}
					|	Decision_point IDENTIF_COLON search_keyword trace_stat	{}
					|	Decision_point IDENTIF_COLON search_keyword trace_tops	{}
					|	Decision_point IDENTIF_COLON search_keyword trace_all	{};

dpt_condition_search:	dpt_begin_search Condition_keyword dpt_logic			{}
						|	dpt_begin_search Condition_keyword NoCheck			{};

dpt_term_search: dpt_condition_search Term_condition dpt_logic					{};

dpt_evaluate_search:	dpt_term_search Evaluate_by dpt_arithm					{};

dpt_compare_search:	dpt_evaluate_search Compare_tops '=' NO						{}
					|	dpt_evaluate_search Compare_tops '=' YES				{};

dpt_activ_search:	dpt_compare_search Activities
					|	dpt_activ_search_descr_value;

dpt_activ_search_descr:	dpt_activ_search IDENTIF_COLON IDENTIF					{};

dpt_activ_search_descr_param:	dpt_activ_search_descr
								|	dpt_activ_search_descr_param INT_CONST		{}
								|	dpt_activ_search_descr_param REAL_CONST		{}
								|	dpt_activ_search_descr_param IDENTIF		{}
								|	dpt_activ_search_descr_param '*'			{};

dpt_activ_search_descr_value:	dpt_activ_search_descr_param value_before	dpt_arithm		{}
								|	dpt_activ_search_descr_param value_after  dpt_arithm	{};

dpt_activ_search_end:	dpt_activ_search End									{};

/* ///////////////////////  SOME POINT ///////////////////////////// */

dpt_begin_some:	Decision_point IDENTIF_COLON some				{}
				|	Decision_point IDENTIF_COLON some no_trace	{};

dpt_condition_some:	dpt_begin_some Condition_keyword dpt_logic		{}
					|	dpt_begin_some Condition_keyword NoCheck	{};

dpt_activ_some:	dpt_condition_some Activities
				|	dpt_activ_some_descr_param;

dpt_activ_some_descr:	dpt_activ_some IDENTIF_COLON IDENTIF		{};

dpt_activ_some_descr_param:	dpt_activ_some_descr
							|	dpt_activ_some_descr_param INT_CONST	{}
							|	dpt_activ_some_descr_param REAL_CONST	{}
							|	dpt_activ_some_descr_param IDENTIF		{}
							|	dpt_activ_some_descr_param '*'			{};

dpt_activ_some_end:	dpt_activ_some End	{};

/* ///////////////////////  FREE ACTIVITIES ///////////////////////////// */

dpt_activ_free:	Activities						{};
				|	dpt_activ_free_descr_param	{};										
																									
dpt_activ_free_descr:	dpt_activ_free IDENTIF_COLON IDENTIF	{};
																									                                                                    
dpt_activ_free_descr_keyb:	dpt_activ_free_descr
							|	dpt_activ_free_descr_keyb QUOTED_IDENTIF		{}
							|	dpt_activ_free_descr_keyb '+' QUOTED_IDENTIF	{};

dpt_activ_free_descr_param:	dpt_activ_free_descr_param INT_CONST			{}
							|	dpt_activ_free_descr_param REAL_CONST		{}             
							|	dpt_activ_free_descr_param IDENTIF			{}
							|	dpt_activ_free_descr_param '*'				{}
							|	dpt_activ_free_descr_keyb;
																									                                                                    
dpt_activ_free_end:	dpt_activ_free End	{};

/* ///////////////////////  ARITHMETIC/LOGIC ///////////////////////////// */

dpt_logic: dpt_arithm '=' dpt_arithm	{}
			| dpt_arithm neq dpt_arithm	{}
			| dpt_arithm '<' dpt_arithm	{}
			| dpt_arithm '>' dpt_arithm	{}
			| dpt_arithm leq dpt_arithm	{}
			| dpt_arithm geq dpt_arithm	{}
			| dpt_logic and_keyword dpt_logic	{}
			| dpt_logic or_keyword dpt_logic	{}
			| '[' dpt_logic ']'					{}
			| fun_group							{};
			
dpt_arithm: dpt_arithm '+' dpt_arithm {
				$$ = (int)(*(RDOFUNArithm *)$1 + *(RDOFUNArithm *)$3);
			}
			| dpt_arithm '-' dpt_arithm {
				$$ = (int)(*(RDOFUNArithm *)$1 - *(RDOFUNArithm *)$3);
			}
			| dpt_arithm '*' dpt_arithm {
				$$ = (int)(*(RDOFUNArithm *)$1 * *(RDOFUNArithm *)$3);
			}
			| dpt_arithm '/' dpt_arithm {
				$$ = (int)(*(RDOFUNArithm *)$1 / *(RDOFUNArithm *)$3);
			}
			| '(' dpt_arithm ')' {
				$$ = $2;
			}
			| IDENTIF '(' dpt_arithm_func_call_pars ')' {
				$$ = (int)((RDOFUNParams *)$3)->createCall((std::string *)$1)
			}
			| IDENTIF '.' IDENTIF {
				$$ = (int)(new RDOFUNArithm((std::string *)$1, (std::string *)$3));
			}
			| INT_CONST {
				$$ = (int)(new RDOFUNArithm((int)$1));
			}
			| REAL_CONST {
				$$ = (int)(new RDOFUNArithm((double*)$1));
			}
			| IDENTIF {
				$$ = (int)(new RDOFUNArithm((std::string *)$1));
			};

dpt_arithm_func_call_pars:	/* empty */	{
				$$ = (int)(new RDOFUNParams()); 
			}
			| dpt_arithm_func_call_pars dpt_arithm		{
				$$ = (int)(((RDOFUNParams *)$1)->addParameter((RDOFUNArithm *)$2));
			}
			| dpt_arithm_func_call_pars ',' dpt_arithm	{
				$$ = (int)(((RDOFUNParams *)$1)->addParameter((RDOFUNArithm *)$3));
			};

fun_group_keyword:	Exist			{}
					|	Not_Exist	{}
					|	For_All		{}
					|	Not_For_All	{};

fun_group_header:	fun_group_keyword '(' IDENTIF_COLON {};

fun_group:	fun_group_header dpt_logic ')'		{}
			|	fun_group_header NoCheck ')'	{};

/* ///////////////////////  PROCESS ///////////////////////////// */

dpt_process:		dpt_process_begin dpt_process_input {
					};

dpt_process_begin:	Process {
						RDOPROCProcess* proc = parser->getLastDPTProcess();
						if ( proc && !proc->isend() ) {
							parser->error( "Незакрыт предыдущий блок $Process" );
						}
						proc = new RDOPROCProcess( "Process" );
						@$ = @1;
					};

dpt_process_input:	/* empty */
					| dpt_process_input dpt_process_line;

dpt_process_line:	IDENTIF	{
						parser->error( rdo::format("Неизвестный оператор '%s'", ((std::string *)$1)->c_str()) );
					}
					| GENERATE dpt_arithm {
						RDOPROCGenerate* generate = new RDOPROCGenerate( "GENERATE", ((RDOFUNArithm*)$2)->createCalc() );
						$$ = int(generate);
					}
					| TERMINATE {
						RDOPROCTerminate* terminate = new RDOPROCTerminate( "TERMINATE" );
						$$ = int(terminate);
					}
					| SEIZE {
						parser->error( std::string(_T("Ожидается имя ресурса")).c_str() );
					}
					| SEIZE IDENTIF {
						RDOPROCSeize* seize = new RDOPROCSeize( "SEIZE" );
						$$ = int(seize);
					};

dpt_process_end:	dpt_process End	{
						parser->getLastDPTProcess()->end();
					};

%%

}

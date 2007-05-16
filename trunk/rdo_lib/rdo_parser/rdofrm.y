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
%token temporary_kw			263
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
%token Select				418
%token Size_kw				419
%token Empty_kw				420
%token not_keyword			421
%token UMINUS				422

%{
#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <FlexLexer.h>

#include "rdoparser.h"
#include "rdoparser_rdo.h"
#include "rdofun.h"
#include "rdofrm.h"
#include <rdoframe.h>
#include <rdocalc.h>

namespace rdoParse 
{
%}

%left or_keyword
%left and_keyword
%left '+' '-'
%left '*' '/'
%left not_keyword
%left UMINUS

%start frm_main

%%

/* ///////////////////////  GENERAL PART ///////////////////////////// */

frm_main:	
	| frm_main frm_end;


/* ///////////////////////  FRAME ///////////////////////////// */

frm_begin: Frame IDENTIF						{ $$ = (int)(new rdoRuntime::RDOFRMFrame(parser->runTime, (std::string *)$2)); @$; }
			| Frame IDENTIF Show_if fun_logic	{ $$ = (int)(new rdoRuntime::RDOFRMFrame(parser->runTime, (std::string *)$2, ((RDOFUNLogic *)$4)->calc)); };

frm_background:		frm_begin Back_picture '='                                         { ((rdoRuntime::RDOFRMFrame *)$1)->setBackground();           }
					| frm_begin Back_picture '=' '<' INT_CONST INT_CONST INT_CONST '>' { ((rdoRuntime::RDOFRMFrame *)$1)->setBackground($5, $6, $7); };

frm_backpicture:	frm_background INT_CONST INT_CONST { ((rdoRuntime::RDOFRMFrame *)$1)->setBackPicture($2, $3);           }
					| frm_background IDENTIF           { ((rdoRuntime::RDOFRMFrame *)$1)->setBackPicture((std::string*)$2); };

frm_pre_show: frm_backpicture
				| frm_item;

frm_show:	frm_pre_show                      { ((rdoRuntime::RDOFRMFrame *)$1)->startShow();                         }
			| frm_pre_show Show               { ((rdoRuntime::RDOFRMFrame *)$1)->startShow();                         }
			| frm_pre_show Show_if fun_logic  { ((rdoRuntime::RDOFRMFrame *)$1)->startShow(((RDOFUNLogic*)$3)->calc); };

frm_item: frm_show								
		  | frm_item frm_text					{ ((rdoRuntime::RDOFRMFrame *)$1)->addItem((rdoRuntime::RDOFRMText *)$2); }
		  | frm_item frm_bitmap					{ ((rdoRuntime::RDOFRMFrame *)$1)->addItem((rdoRuntime::RDOFRMBitmap *)$2); }
		  | frm_item frm_rect					{ ((rdoRuntime::RDOFRMFrame *)$1)->addItem((rdoRuntime::RDOFRMRect *)$2); }
		  | frm_item frm_line					{ ((rdoRuntime::RDOFRMFrame *)$1)->addItem((rdoRuntime::RDOFRMLine *)$2); }
		  | frm_item frm_ellipse				{ ((rdoRuntime::RDOFRMFrame *)$1)->addItem((rdoRuntime::RDOFRMEllipse *)$2); }
		  | frm_item frm_r_rect					{ ((rdoRuntime::RDOFRMFrame *)$1)->addItem((rdoRuntime::RDOFRMR_rect *)$2); }
		  | frm_item frm_triang					{ ((rdoRuntime::RDOFRMFrame *)$1)->addItem((rdoRuntime::RDOFRMTriang *)$2); }
		  | frm_item frm_s_bmp					{ ((rdoRuntime::RDOFRMFrame *)$1)->addItem((rdoRuntime::RDOFRMS_bmp *)$2); }
		  | frm_item frm_active					{ ((rdoRuntime::RDOFRMFrame *)$1)->addItem((rdoRuntime::RDOFRMActive *)$2); };

frm_end: frm_item End	{ ((rdoRuntime::RDOFRMFrame *)$1)->end(); };

/* ///////////////////////  ITEMS ///////////////////////////// */

frm_text_common: text '[' fun_arithm ',' fun_arithm ',' fun_arithm ',' fun_arithm ',' frm_color ',' frm_color ','  { $$ = (int)(new rdoRuntime::RDOFRMText(parser->runTime->lastFrame(), ((RDOFUNArithm *)$3)->createCalc(), ((RDOFUNArithm *)$5)->createCalc(), ((RDOFUNArithm *)$7)->createCalc(), ((RDOFUNArithm *)$9)->createCalc(), (rdoRuntime::RDOFRMColor *)$11, (rdoRuntime::RDOFRMColor *)$13)); };

frm_text: frm_text_common frm_align fun_arithm ']'			{ ((rdoRuntime::RDOFRMText *)$1)->setText($2, ((RDOFUNArithm *)$3)->createCalc(), NULL /*//qq((RDOFUNArithm *)$3)->enu*/); }
		  | frm_text_common frm_align QUOTED_IDENTIF ']'	{ ((rdoRuntime::RDOFRMText *)$1)->setText($2, (std::string *)$3);  };

frm_align: /* empty */	{ $$ = 1; }
		| '<'			{ $$ = 1; }
		| '='			{ $$ = 2; }
		| '>'			{ $$ = 3; };

frm_color:	transparent                             { $$ = (int)(new rdoRuntime::RDOFRMColor(parser->runTime->lastFrame()));             }
			| '<' INT_CONST INT_CONST INT_CONST '>' { $$ = (int)(new rdoRuntime::RDOFRMColor(parser->runTime->lastFrame(), $2, $3, $4)); };

frm_bitmap:	bitmap '[' fun_arithm ',' fun_arithm ',' IDENTIF ']'               { $$ = (int)(new rdoRuntime::RDOFRMBitmap(parser->runTime->lastFrame(), ((RDOFUNArithm *)$3)->createCalc(), ((RDOFUNArithm *)$5)->createCalc(), (std::string *)$7));                    }
			| bitmap '[' fun_arithm ',' fun_arithm ',' IDENTIF ',' IDENTIF ']' { $$ = (int)(new rdoRuntime::RDOFRMBitmap(parser->runTime->lastFrame(), ((RDOFUNArithm *)$3)->createCalc(), ((RDOFUNArithm *)$5)->createCalc(), (std::string *)$7, (std::string *)$9)); };

frm_s_bmp: s_bmp '[' fun_arithm ',' fun_arithm ',' fun_arithm ',' fun_arithm ',' IDENTIF ']' 				 { $$ = (int)(new rdoRuntime::RDOFRMS_bmp(parser->runTime->lastFrame(), ((RDOFUNArithm *)$3)->createCalc(), ((RDOFUNArithm *)$5)->createCalc(), ((RDOFUNArithm *)$7)->createCalc(), ((RDOFUNArithm *)$9)->createCalc(), (std::string *)$11)); }
		  |  s_bmp '[' fun_arithm ',' fun_arithm ',' fun_arithm ',' fun_arithm ',' IDENTIF ',' IDENTIF ']'  { $$ = (int)(new rdoRuntime::RDOFRMS_bmp(parser->runTime->lastFrame(), ((RDOFUNArithm *)$3)->createCalc(), ((RDOFUNArithm *)$5)->createCalc(), ((RDOFUNArithm *)$7)->createCalc(), ((RDOFUNArithm *)$9)->createCalc(), (std::string *)$11, (std::string *)$13)); };

frm_rect: rect_keyword '[' fun_arithm ',' fun_arithm ',' fun_arithm ',' fun_arithm ',' frm_color ',' frm_color ']'	{ $$ = (int)(new rdoRuntime::RDOFRMRect(parser->runTime->lastFrame(), ((RDOFUNArithm *)$3)->createCalc(), ((RDOFUNArithm *)$5)->createCalc(), ((RDOFUNArithm *)$7)->createCalc(), ((RDOFUNArithm *)$9)->createCalc(), (rdoRuntime::RDOFRMColor *)$11, (rdoRuntime::RDOFRMColor *)$13)); };

frm_r_rect: r_rect '[' fun_arithm ',' fun_arithm ',' fun_arithm ',' fun_arithm ',' frm_color ',' frm_color ']'			{ $$ = (int)(new rdoRuntime::RDOFRMR_rect(parser->runTime->lastFrame(), ((RDOFUNArithm *)$3)->createCalc(), ((RDOFUNArithm *)$5)->createCalc(), ((RDOFUNArithm *)$7)->createCalc(), ((RDOFUNArithm *)$9)->createCalc(), (rdoRuntime::RDOFRMColor *)$11, (rdoRuntime::RDOFRMColor *)$13)); };

frm_line: line '[' fun_arithm ',' fun_arithm ',' fun_arithm ',' fun_arithm ',' frm_color ']'		{ $$ = (int)(new rdoRuntime::RDOFRMLine(parser->runTime->lastFrame(), ((RDOFUNArithm *)$3)->createCalc(), ((RDOFUNArithm *)$5)->createCalc(), ((RDOFUNArithm *)$7)->createCalc(), ((RDOFUNArithm *)$9)->createCalc(), (rdoRuntime::RDOFRMColor *)$11)); };

frm_ellipse: ellipse '[' fun_arithm ',' fun_arithm ',' fun_arithm ',' fun_arithm ',' frm_color ',' frm_color ']'   { $$ = (int)(new rdoRuntime::RDOFRMEllipse(parser->runTime->lastFrame(), ((RDOFUNArithm *)$3)->createCalc(), ((RDOFUNArithm *)$5)->createCalc(), ((RDOFUNArithm *)$7)->createCalc(), ((RDOFUNArithm *)$9)->createCalc(), (rdoRuntime::RDOFRMColor *)$11, (rdoRuntime::RDOFRMColor *)$13)); };

frm_triang: triang '[' fun_arithm ',' fun_arithm ',' fun_arithm ',' fun_arithm ',' fun_arithm ',' fun_arithm ',' frm_color ',' frm_color ']'		{ $$ = (int)(new rdoRuntime::RDOFRMTriang(parser->runTime->lastFrame(), ((RDOFUNArithm *)$3)->createCalc(), ((RDOFUNArithm *)$5)->createCalc(), ((RDOFUNArithm *)$7)->createCalc(), ((RDOFUNArithm *)$9)->createCalc(), ((RDOFUNArithm *)$11)->createCalc(), ((RDOFUNArithm *)$13)->createCalc(), (rdoRuntime::RDOFRMColor *)$15, (rdoRuntime::RDOFRMColor *)$17)); };

frm_active: active IDENTIF '[' fun_arithm ',' fun_arithm ',' fun_arithm ',' fun_arithm ']'	{ $$ = (int)(new rdoRuntime::RDOFRMActive(parser->runTime->lastFrame(), ((RDOFUNArithm *)$4)->createCalc(), ((RDOFUNArithm *)$6)->createCalc(), ((RDOFUNArithm *)$8)->createCalc(), ((RDOFUNArithm *)$10)->createCalc(), (std::string *)$2)); };


// ----------------------------------------------------------------------------
// ---------- Логические выражения
// ----------------------------------------------------------------------------
// Пока не использьзуется RDOErrorPos, но в ариф. выражениях уже назначается
fun_logic: fun_arithm '=' fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 == *(RDOFUNArithm *)$3); }
			| fun_arithm neq fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 != *(RDOFUNArithm *)$3); }
			| fun_arithm '<' fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 <  *(RDOFUNArithm *)$3); }
			| fun_arithm '>' fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 >  *(RDOFUNArithm *)$3); }
			| fun_arithm leq fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 <= *(RDOFUNArithm *)$3); }
			| fun_arithm geq fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 >= *(RDOFUNArithm *)$3); }
			| fun_logic and_keyword fun_logic	{ $$ = (int)(*(RDOFUNLogic *)$1 && *(RDOFUNLogic *)$3);   }
			| fun_logic or_keyword fun_logic	{ $$ = (int)(*(RDOFUNLogic *)$1 || *(RDOFUNLogic *)$3);   }
			| '[' fun_logic ']'					{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setErrorPos( @1.first_line, @1.first_column, @3.last_line, @3.last_column );
				$$ = $2;
			}
			| '(' fun_logic ')'					{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setErrorPos( @1.first_line, @1.first_column, @3.last_line, @3.last_column );
				$$ = $2;
			}
			| not_keyword fun_logic				{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setErrorPos( @1.first_line, @1.first_column, @2.last_line, @2.last_column );
				$$ = (int)logic->operator_not();
			}
			| fun_group							{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($1);
				logic->setErrorPos( @1 );
				$$ = $1;
			}
			| fun_select_logic					{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($1);
				logic->setErrorPos( @1 );
				$$ = $1;
			}
			| '[' fun_logic error {
				parser->lexer_loc_set( @2.last_line, @2.last_column );
				parser->error( "Ожидается закрывающаяся скобка" );
			}
			| '(' fun_logic error {
				parser->lexer_loc_set( @2.last_line, @2.last_column );
				parser->error( "Ожидается закрывающаяся скобка" );
			}
			| error								{
				parser->lexer_loc_set( &(@1) );
				parser->error( "Ошибка в логическом выражении" );
			};

// ----------------------------------------------------------------------------
// ---------- Арифметические выражения
// ----------------------------------------------------------------------------
fun_arithm: fun_arithm '+' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 + *(RDOFUNArithm *)$3); }
			| fun_arithm '-' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 - *(RDOFUNArithm *)$3); }
			| fun_arithm '*' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 * *(RDOFUNArithm *)$3); }
			| fun_arithm '/' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 / *(RDOFUNArithm *)$3); }
			| '(' fun_arithm ')'			{
				RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($2);
				arithm->setErrorPos( @1.first_line, @1.first_column, @3.last_line, @3.last_column );
				$$ = $2;
			}
			| fun_arithm_func_call			{
				RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($1);
				arithm->setErrorPos( @1 );
				$$ = $1;
			}
			| fun_select_arithm				{
				RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($1);
				arithm->setErrorPos( @1 );
				$$ = $1;
			}
			| IDENTIF '.' IDENTIF			{
				$$ = (int)new RDOFUNArithm( parser, reinterpret_cast<std::string*>($1), reinterpret_cast<std::string*>($3), @1, @3 );
			}
			| INT_CONST						{ $$ = (int)new RDOFUNArithm( parser, (int)$1, @1 );          }
			| REAL_CONST					{ $$ = (int)new RDOFUNArithm( parser, (double*)$1, @1 );      }
			| IDENTIF						{ $$ = (int)new RDOFUNArithm( parser, (std::string*)$1, @1 ); }
			| '-' fun_arithm %prec UMINUS	{
				YYLTYPE error_pos;
				error_pos.first_line   = @1.first_line;
				error_pos.first_column = @1.first_column;
				error_pos.last_line    = @2.last_line;
				error_pos.last_column  = @2.last_column;
				$$ = (int)new RDOFUNArithm( parser, reinterpret_cast<RDOFUNArithm*>($2)->getType(), new rdoRuntime::RDOCalcUMinus( parser->runTime, reinterpret_cast<RDOFUNArithm*>($2)->createCalc() ), error_pos );
			}
			| error							{
				parser->lexer_loc_set( &(@1) );
				if ( @1.first_line = @1.last_line ) {
					parser->error( rdo::format("Неизвестный идентификатор: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				} else {
					parser->error( "Ошибка в арифметическом выражении" );
				}
			};

fun_arithm_func_call:	IDENTIF '(' fun_arithm_func_call_pars ')' {
							RDOFUNParams* fun = ((RDOFUNParams*)$3);
							fun->name_error_pos.setErrorPos( @1 );
							fun->setErrorPos( @1.first_line, @1.first_column, @4.last_line, @4.last_column );
							$$ = (int)fun->createCall((std::string *)$1);
						}
						| IDENTIF '(' error {
							parser->lexer_loc_set( &(@3) );
							parser->error( "Ошибка в параметрах функции" );
						};

fun_arithm_func_call_pars:	/* empty */ {
								RDOFUNParams* fun = new RDOFUNParams( parser );
								$$ = (int)fun;
							}
							| fun_arithm_func_call_pars fun_arithm {
								RDOFUNParams* fun = reinterpret_cast<RDOFUNParams*>($1);
								fun->setErrorPos( @2 );
								fun = fun->addParameter((RDOFUNArithm *)$2);
								$$ = (int)fun;
							}
							| fun_arithm_func_call_pars ',' fun_arithm {
								RDOFUNParams* fun = reinterpret_cast<RDOFUNParams*>($1);
								fun->setErrorPos( @3 );
								fun = fun->addParameter((RDOFUNArithm *)$3);
								$$ = (int)fun;
							};

// ----------------------------------------------------------------------------
// ---------- Групповые выражения
// ----------------------------------------------------------------------------
fun_group_keyword:	Exist			{ $$ = 1; }
					| Not_Exist		{ $$ = 2; }
					| For_All		{ $$ = 3; }
					| Not_For_All	{ $$ = 4; };

fun_group_header:	fun_group_keyword '(' IDENTIF_COLON {
						parser->lexer_loc_backup();
						parser->lexer_loc_set( @3.first_line, @3.first_column + ((std::string*)$3)->length() );
						$$ = (int)(new RDOFUNGroupLogic( parser, $1, (std::string *)$3) );
						parser->lexer_loc_restore();
					}
					| fun_group_keyword '(' error {
						parser->lexer_loc_set( &(@3) );
						parser->error( "Ожидается имя типа" );
					}
					| fun_group_keyword error {
						parser->lexer_loc_set( &(@1) );
						parser->error( "Ожидается октрывающаяся скобка" );
					};

fun_group:			fun_group_header fun_logic ')' {
						$$ = (int)(((RDOFUNGroupLogic *)$1)->createFunLogic((RDOFUNLogic *)$2));
					}
					| fun_group_header NoCheck ')' {
						RDOFUNLogic* trueLogic = new RDOFUNLogic( new rdoRuntime::RDOCalcConst( parser->runTime, 1 ) );
						trueLogic->setErrorPos( @2 );
						$$ = (int)(((RDOFUNGroupLogic *)$1)->createFunLogic( trueLogic ));
					}
					| fun_group_header fun_logic error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "Ожидается закрывающаяся скобка" );
					}
					| fun_group_header NoCheck error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "Ожидается закрывающаяся скобка" );
					};

fun_select_header:	Select '(' IDENTIF_COLON {
						$$ = (int)new RDOFUNSelect( parser, (std::string*)$3 );
					}
					| Select '(' error {
						parser->lexer_loc_set( &(@3) );
						parser->error( "Ожидается имя типа" );
					}
					| Select error {
						parser->lexer_loc_set( &(@1) );
						parser->error( "Ожидается октрывающаяся скобка" );
					};

fun_select_body:	fun_select_header fun_logic ')' {
						RDOFUNLogic* logic = ((RDOFUNSelect*)$1)->createFunSelect((RDOFUNLogic*)$2);
						logic->setErrorPos( @2 );
						$$ = $1;
					}
					| fun_select_header NoCheck ')' {
						RDOFUNLogic* logic = ((RDOFUNSelect*)$1)->createFunSelect();
						logic->setErrorPos( @2 );
						$$ = $1;
					}
					| fun_select_header fun_logic error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "Ожидается закрывающаяся скобка" );
					}
					| fun_select_header NoCheck error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "Ожидается закрывающаяся скобка" );
					};

fun_select_keyword:	Exist			{ $$ = 1; }
					| Not_Exist		{ $$ = 2; }
					| For_All		{ $$ = 3; }
					| Not_For_All	{ $$ = 4; };

fun_select_logic:	fun_select_body '.' fun_select_keyword '(' fun_logic ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						RDOFUNLogic* logic = select->createFunSelectGroup( $3, (RDOFUNLogic*)$5 );
						select->setErrorPos( @1.first_line, @1.first_column, @6.last_line, @6.last_column );
						logic->setErrorPos( select->error() );
						$$ = (int)logic;
					}
					| fun_select_body '.' Empty_kw '(' ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						RDOFUNLogic* logic = select->createFunSelectEmpty();
						select->setErrorPos( @1.first_line, @1.first_column, @5.last_line, @5.last_column );
						logic->setErrorPos( select->error() );
						$$ = (int)logic;
					}
					| fun_select_body error {
						parser->lexer_loc_set( &(@1) );
						parser->error( "Ожидается '.' (точка) для вызова метода списка ресурсов" );
					}
					| fun_select_body '.' error {
						parser->lexer_loc_set( &(@2), &(@3) );
						parser->error( "Ожидается метод списка ресурсов" );
					}
					| fun_select_body '.' fun_select_keyword error {
						parser->lexer_loc_set( &(@3) );
						parser->error( "Ожидается октрывающаяся скобка" );
					}
					| fun_select_body '.' Empty_kw error {
						parser->lexer_loc_set( &(@3) );
						parser->error( "Ожидается октрывающаяся скобка" );
					}
					| fun_select_body '.' fun_select_keyword '(' error ')' {
						parser->lexer_loc_set( @5.first_line, @5.first_column );
						parser->error( "Ошибка в логическом выражении" );
					}
					| fun_select_body '.' Empty_kw '(' error {
						parser->lexer_loc_set( &(@4) );
						parser->error( "Ожидается закрывающаяся скобка" );
					};

fun_select_arithm:	fun_select_body '.' Size_kw '(' ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						RDOFUNArithm* arithm = select->createFunSelectSize();
						select->setErrorPos( @1.first_line, @1.first_column, @5.last_line, @5.last_column );
						arithm->setErrorPos( select->error() );
						$$ = (int)arithm;
					}
					| fun_select_body error {
						parser->lexer_loc_set( &(@1) );
						parser->error( "Ожидается '.' (точка) для вызова метода списка ресурсов" );
					}
					| fun_select_body '.' error {
						parser->lexer_loc_set( &(@2), &(@3) );
						parser->error( "Ожидается метод списка ресурсов: Size()" );
					}
					| fun_select_body '.' Size_kw error {
						parser->lexer_loc_set( &(@3) );
						parser->error( "Ожидается октрывающаяся скобка" );
					}
					| fun_select_body '.' Size_kw '(' error {
						parser->lexer_loc_set( &(@4) );
						parser->error( "Ожидается закрывающаяся скобка" );
					};

%%

}

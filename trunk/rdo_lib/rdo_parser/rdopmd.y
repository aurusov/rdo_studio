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
%token if_keyword			369
%token result_keyword		370

%token Frame				400
%token Show_if				401
%token Back_picture			402
%token Show					403
%token frm_cell				404
%token text					405
%token bitmap				406
%token s_bmp				407
%token rect_keyword			408
%token r_rect				409
%token line					410
%token ellipse				411
%token triang				412
%token active				413
%token ruler				414
%token space_kw				415
%token color_transparent_kw	416
%token color_last_kw		417
%token color_white_kw		418
%token color_black_kw		419
%token color_red_kw			420
%token color_green_kw		421
%token color_blue_kw		422
%token color_cyan_kw		423
%token color_magenta_kw		424
%token color_yellow_kw		425
%token color_gray_kw		426

%token QUOTED_IDENTIF		430
%token QUOTED_IDENTIF_BAD	431
%token IDENTIF_BAD			432
%token Select				433
%token Size_kw				434
%token Empty_kw				435
%token not_keyword			436
%token UMINUS				437

%{
#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoparser.h"
#include "rdoparser_rdo.h"
#include "rdopmd.h"
#include "rdofun.h"
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

%%

pmd_main:	pmd_end;

pmd_body:	/* empty */
			| pmd_body pmd_pokaz;

pmd_trace:	/* empty */ {
				$$ = 0;
			}
			| trace_keyword {
				$$ = 1;
			}
			| no_trace {
				$$ = 0;
			};

pmd_pokaz:	IDENTIF_COLON pmd_trace watch_par IDENTIF '.' IDENTIF {
				$$ = (int)(new RDOPMDWatchPar( parser, RDOParserSrcInfo(@1, *reinterpret_cast<std::string*>($1), RDOParserSrcInfo::psi_align_bytext), $2 != 0, RDOParserSrcInfo(@4, *reinterpret_cast<std::string*>($4)), RDOParserSrcInfo(@6, *reinterpret_cast<std::string*>($6)) ));
			}
			| IDENTIF_COLON pmd_trace watch_par IDENTIF '.' error {
				parser->error( @5, @6, "Ожидается имя параметра" );
			}
			| IDENTIF_COLON pmd_trace watch_par IDENTIF error {
				parser->error( @4, "Ожидается '.'" );
			}
			| IDENTIF_COLON pmd_trace watch_par error {
				parser->error( @3, @4, "После ключевого слова watch_par ожидается имя ресурса" );
			}
			| IDENTIF_COLON pmd_trace error {
				parser->error( @2, @3, "Ожидается тип показателя" );
			}
			| IDENTIF_COLON error {
				parser->error( @1, @2, "Ожидается признак трассировки или тип показателя" );
			}
			| IDENTIF error {
				parser->error( @1, @2, "Ожидается ':'" );
			}
			| error {
				parser->error( @1, "Ожидается имя показателя" );
			}
			| IDENTIF_COLON pmd_trace watch_state fun_logic {
				$$ = (int)(new RDOPMDWatchState( parser, RDOParserSrcInfo(@1, *reinterpret_cast<std::string*>($1), RDOParserSrcInfo::psi_align_bytext), $2 != 0, reinterpret_cast<RDOFUNLogic*>($4) ));
			}
			| IDENTIF_COLON pmd_trace watch_state error {
				parser->error( @3, @4, "После ключевого слова watch_state ожидается логическое выражение" );
			}
			| IDENTIF_COLON pmd_trace watch_quant IDENTIF fun_logic {
				RDOPMDWatchQuant* pmd = new RDOPMDWatchQuant( parser, RDOParserSrcInfo(@1, *reinterpret_cast<std::string*>($1), RDOParserSrcInfo::psi_align_bytext), $2 != 0, RDOParserSrcInfo(@4, *reinterpret_cast<std::string*>($4)) );
				pmd->setLogic( reinterpret_cast<RDOFUNLogic*>($5) );
			}
			| IDENTIF_COLON pmd_trace watch_quant IDENTIF NoCheck {
				RDOPMDWatchQuant* pmd = new RDOPMDWatchQuant( parser, RDOParserSrcInfo(@1, *reinterpret_cast<std::string*>($1), RDOParserSrcInfo::psi_align_bytext), $2 != 0, RDOParserSrcInfo(@4, *reinterpret_cast<std::string*>($4)) );
				pmd->setLogicNoCheck();
			}
			| IDENTIF_COLON pmd_trace watch_quant IDENTIF error {
				parser->error( @4, @5, "После имени типа ожидается логическое выражение" );
			}
			| IDENTIF_COLON pmd_trace watch_quant error {
				parser->error( @3, @4, "После ключевого слова watch_quant ожидается тип ресурса" );
			}
			| IDENTIF_COLON pmd_trace watch_value IDENTIF fun_logic fun_arithm {
				RDOPMDWatchValue* pmd = new RDOPMDWatchValue( parser, RDOParserSrcInfo(@1, *reinterpret_cast<std::string*>($1), RDOParserSrcInfo::psi_align_bytext), $2 != 0, RDOParserSrcInfo(@4, *reinterpret_cast<std::string*>($4)) );
				pmd->setLogic( reinterpret_cast<RDOFUNLogic*>($5), reinterpret_cast<RDOFUNArithm*>($6) );
			}
			| IDENTIF_COLON pmd_trace watch_value IDENTIF NoCheck fun_arithm {
				RDOPMDWatchValue* pmd = new RDOPMDWatchValue( parser, RDOParserSrcInfo(@1, *reinterpret_cast<std::string*>($1), RDOParserSrcInfo::psi_align_bytext), $2 != 0, RDOParserSrcInfo(@4, *reinterpret_cast<std::string*>($4)) );
				pmd->setLogicNoCheck( reinterpret_cast<RDOFUNArithm*>($6) );
			}
			| IDENTIF_COLON pmd_trace watch_value IDENTIF fun_logic error {
				parser->error( @5, @6, "После логического ожидается арифметическое выражение" );
			}
			| IDENTIF_COLON pmd_trace watch_value IDENTIF NoCheck error {
				parser->error( @5, @6, "После логического ожидается арифметическое выражение" );
			}
			| IDENTIF_COLON pmd_trace watch_value IDENTIF error {
				parser->error( @4, @5, "После имени типа ожидается логическое выражение" );
			}
			| IDENTIF_COLON pmd_trace watch_value error {
				parser->error( @3, @4, "После ключевого слова watch_value ожидается тип ресурса" );
			}
			| IDENTIF_COLON get_value fun_arithm {
				$$ = (int)(new RDOPMDGetValue( parser, RDOParserSrcInfo(@1, *reinterpret_cast<std::string*>($1), RDOParserSrcInfo::psi_align_bytext), reinterpret_cast<RDOFUNArithm*>($3) ));
			}
			| IDENTIF_COLON get_value error {
				parser->error( @2, @3, "После ключевого слова get_value ожидается арифметическое выражение" );
			};

pmd_end:	Results pmd_body End
			| error {
				YYLTYPE pos( @1 );
				pos.last_line = pos.first_line;
				pos.last_column = pos.first_column;
				parser->error( pos, "Ожидается ключевое слово $Results" );
			}
			| Results pmd_body error {
				parser->error( @3, "Ожидается ключевое слово $End" );
			};

// ----------------------------------------------------------------------------
// ---------- Логические выражения
// ----------------------------------------------------------------------------
fun_logic:	fun_arithm '=' fun_arithm         { $$ = (int)(*(RDOFUNArithm *)$1 == *(RDOFUNArithm *)$3); }
			| fun_arithm neq fun_arithm       { $$ = (int)(*(RDOFUNArithm *)$1 != *(RDOFUNArithm *)$3); }
			| fun_arithm '<' fun_arithm       { $$ = (int)(*(RDOFUNArithm *)$1 <  *(RDOFUNArithm *)$3); }
			| fun_arithm '>' fun_arithm       { $$ = (int)(*(RDOFUNArithm *)$1 >  *(RDOFUNArithm *)$3); }
			| fun_arithm leq fun_arithm       { $$ = (int)(*(RDOFUNArithm *)$1 <= *(RDOFUNArithm *)$3); }
			| fun_arithm geq fun_arithm       { $$ = (int)(*(RDOFUNArithm *)$1 >= *(RDOFUNArithm *)$3); }
			| fun_logic and_keyword fun_logic { $$ = (int)(*(RDOFUNLogic *)$1 && *(RDOFUNLogic *)$3);   }
			| fun_logic or_keyword fun_logic  { $$ = (int)(*(RDOFUNLogic *)$1 || *(RDOFUNLogic *)$3);   }
			| '[' fun_logic ']' {
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setSrcPos( @1, @3 );
				logic->setSrcText( "[" + logic->src_text() + "]" );
				$$ = $2;
			}
			| '(' fun_logic ')' {
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setSrcPos( @1, @3 );
				logic->setSrcText( "(" + logic->src_text() + ")" );
				$$ = $2;
			}
			| '[' fun_logic error {
				parser->error( @2, "Ожидается закрывающаяся скобка" );
			}
			| '(' fun_logic error {
				parser->error( @2, "Ожидается закрывающаяся скобка" );
			}
			| not_keyword fun_logic {
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				RDOFUNLogic* logic_not = logic->operator_not();
				logic_not->setSrcPos( @1, @2 );
				logic_not->setSrcText( "not " + logic->src_text() );
				$$ = (int)logic_not;
			}
			| fun_group {
			}
			| fun_select_logic {
			};

// ----------------------------------------------------------------------------
// ---------- Арифметические выражения
// ----------------------------------------------------------------------------
fun_arithm: fun_arithm '+' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 + *(RDOFUNArithm *)$3); }
			| fun_arithm '-' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 - *(RDOFUNArithm *)$3); }
			| fun_arithm '*' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 * *(RDOFUNArithm *)$3); }
			| fun_arithm '/' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 / *(RDOFUNArithm *)$3); }
			| '(' fun_arithm ')' {
				RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($2);
				arithm->setSrcPos( @1, @3 );
				arithm->setSrcText( "(" + arithm->src_text() + ")" );
				$$ = $2;
			}
			| fun_arithm_func_call {
			}
			| fun_select_arithm {
			}
			| IDENTIF '.' IDENTIF {
				$$ = (int)new RDOFUNArithm( parser, RDOParserSrcInfo( @1, *reinterpret_cast<std::string*>($1) ), RDOParserSrcInfo( @3, *reinterpret_cast<std::string*>($3) ) );
			}
			| INT_CONST                   { $$ = (int)new RDOFUNArithm( parser, (int)$1, RDOParserSrcInfo( @1, reinterpret_cast<RDOLexer*>(lexer)->YYText() ) );     }
			| REAL_CONST                  { $$ = (int)new RDOFUNArithm( parser, (double*)$1, RDOParserSrcInfo( @1, reinterpret_cast<RDOLexer*>(lexer)->YYText() ) ); }
			| IDENTIF                     { $$ = (int)new RDOFUNArithm( parser, *(std::string*)$1, @1 );                                                             }
			| '-' fun_arithm %prec UMINUS {
				RDOParserSrcInfo info;
				info.setSrcPos( @1, @2 );
				info.setSrcText( "-" + reinterpret_cast<RDOFUNArithm*>($2)->src_text() );
				$$ = (int)new RDOFUNArithm( parser, reinterpret_cast<RDOFUNArithm*>($2)->getType(), new rdoRuntime::RDOCalcUMinus( parser->runtime, reinterpret_cast<RDOFUNArithm*>($2)->createCalc() ), info );
			};

// ----------------------------------------------------------------------------
// ---------- Функции и последовательности
// ----------------------------------------------------------------------------
fun_arithm_func_call:	IDENTIF '(' ')' {
							RDOFUNParams* fun = new RDOFUNParams( parser );
							std::string fun_name = *reinterpret_cast<std::string*>($1);
							fun->funseq_name.setSrcInfo( RDOParserSrcInfo(@1, fun_name) );
							fun->setSrcPos( @1, @3 );
							fun->setSrcText( fun_name + "()" );
							RDOFUNArithm* arithm = fun->createCall( fun_name );
							$$ = (int)arithm;
						}
						| IDENTIF '(' fun_arithm_func_call_pars ')' {
							RDOFUNParams* fun    = reinterpret_cast<RDOFUNParams*>($3);
							std::string fun_name = *reinterpret_cast<std::string*>($1);
							fun->funseq_name.setSrcInfo( RDOParserSrcInfo(@1, fun_name) );
							fun->setSrcPos( @1, @4 );
							fun->setSrcText( fun_name + "(" + fun->src_text() + ")" );
							RDOFUNArithm* arithm = fun->createCall( fun_name );
							$$ = (int)arithm;
						}
						| IDENTIF '(' error {
							parser->error( @3, "Ошибка в параметрах функции" );
						};

fun_arithm_func_call_pars:	fun_arithm {
								RDOFUNParams* fun = new RDOFUNParams( parser );
								RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($1);
								fun->setSrcText( arithm->src_text() );
								fun->addParameter( arithm );
								$$ = (int)fun;
							}
							| fun_arithm_func_call_pars ',' fun_arithm {
								RDOFUNParams* fun    = reinterpret_cast<RDOFUNParams*>($1);
								RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($3);
								fun->setSrcText( fun->src_text() + ", " + arithm->src_text() );
								fun->addParameter( arithm );
								$$ = (int)fun;
							}
							| fun_arithm_func_call_pars error {
								parser->error( @2, "Ошибка в арифметическом выражении" );
							}
							| fun_arithm_func_call_pars ',' error {
								parser->error( @3, "Ошибка в арифметическом выражении" );
							};

// ----------------------------------------------------------------------------
// ---------- Групповые выражения
// ----------------------------------------------------------------------------
fun_group_keyword:	Exist			{ $$ = RDOFUNGroupLogic::fgt_exist;     }
					| Not_Exist		{ $$ = RDOFUNGroupLogic::fgt_notexist;  }
					| For_All		{ $$ = RDOFUNGroupLogic::fgt_forall;    }
					| Not_For_All	{ $$ = RDOFUNGroupLogic::fgt_notforall; };

fun_group_header:	fun_group_keyword '(' IDENTIF_COLON {
						std::string type_name = *reinterpret_cast<std::string*>($3);
						$$ = (int)(new RDOFUNGroupLogic( parser, (RDOFUNGroupLogic::FunGroupType)$1, RDOParserSrcInfo(@3, type_name, RDOParserSrcInfo::psi_align_bytext) ));
					}
					| fun_group_keyword '(' error {
						parser->error( @3, "Ожидается имя типа" );
					}
					| fun_group_keyword error {
						parser->error( @1, "После имени функции ожидается октрывающаяся скобка" );
					};

fun_group:			fun_group_header fun_logic ')' {
						RDOFUNGroupLogic* groupfun = reinterpret_cast<RDOFUNGroupLogic*>($1);
						groupfun->setSrcPos( @1, @3 );
						$$ = (int)groupfun->createFunLogic( reinterpret_cast<RDOFUNLogic*>($2) );
					}
					| fun_group_header NoCheck ')' {
						RDOFUNGroupLogic* groupfun = reinterpret_cast<RDOFUNGroupLogic*>($1);
						groupfun->setSrcPos( @1, @3 );
						RDOFUNLogic* trueLogic = new RDOFUNLogic( groupfun, new rdoRuntime::RDOCalcConst( parser->runtime, 1 ) );
						trueLogic->setSrcPos( @2 );
						trueLogic->setSrcText( "NoCheck" );
						$$ = (int)groupfun->createFunLogic( trueLogic );
					}
					| fun_group_header fun_logic error {
						parser->error( @2, "Ожидается закрывающаяся скобка" );
					}
					| fun_group_header NoCheck error {
						parser->error( @2, "Ожидается закрывающаяся скобка" );
					}
					| fun_group_header error {
						parser->error( @1, @2, "Ошибка в логическом выражении" )
					};

// ----------------------------------------------------------------------------
// ---------- Select
// ----------------------------------------------------------------------------
fun_select_header:	Select '(' IDENTIF_COLON {
						std::string type_name = *reinterpret_cast<std::string*>($3);
						RDOFUNSelect* select = new RDOFUNSelect( parser, RDOParserSrcInfo(@3, type_name, RDOParserSrcInfo::psi_align_bytext) );
						select->setSrcText( "Select(" + type_name + ": " );
						$$ = (int)select;
					}
					| Select '(' error {
						parser->error( @3, "Ожидается имя типа" );
					}
					| Select error {
						parser->error( @1, "Ожидается октрывающаяся скобка" );
					};

fun_select_body:	fun_select_header fun_logic ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						RDOFUNLogic*  flogic = reinterpret_cast<RDOFUNLogic*>($2);
						select->setSrcText( select->src_text() + flogic->src_text() + ")" );
						select->initSelect( flogic );
					}
					| fun_select_header NoCheck ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						RDOParserSrcInfo logic_info(@2, "NoCheck");
						select->setSrcText( select->src_text() + logic_info.src_text() + ")" );
						rdoRuntime::RDOCalcConst* calc_nocheck = new rdoRuntime::RDOCalcConst( parser->runtime, 1 );
						RDOFUNLogic* flogic = new RDOFUNLogic( select, calc_nocheck, true );
						flogic->setSrcInfo( logic_info );
						select->initSelect( flogic );
					}
					| fun_select_header fun_logic error {
						parser->error( @2, "Ожидается закрывающаяся скобка" );
					}
					| fun_select_header NoCheck error {
						parser->error( @2, "Ожидается закрывающаяся скобка" );
					}
					| fun_select_header error {
						parser->error( @1, @2, "Ошибка в логическом выражении" )
					};

fun_select_keyword:	Exist			{ $$ = RDOFUNGroupLogic::fgt_exist;     }
					| Not_Exist		{ $$ = RDOFUNGroupLogic::fgt_notexist;  }
					| For_All		{ $$ = RDOFUNGroupLogic::fgt_forall;    }
					| Not_For_All	{ $$ = RDOFUNGroupLogic::fgt_notforall; };

fun_select_logic:	fun_select_body '.' fun_select_keyword '(' fun_logic ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcPos( @1, @6 );
						RDOFUNLogic* logic = select->createFunSelectGroup( (RDOFUNGroupLogic::FunGroupType)$3, reinterpret_cast<RDOFUNLogic*>($5) );
						$$ = (int)logic;
					}
					| fun_select_body '.' fun_select_keyword '(' error {
						parser->error( @4, @5, "Ошибка в логическом выражении" )
					}
					| fun_select_body '.' fun_select_keyword error {
						parser->error( @3, "Ожидается октрывающаяся скобка" );
					}
					| fun_select_body '.' Empty_kw '(' ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcPos( @1, @5 );
						RDOParserSrcInfo empty_info(@3, @5, "Empty()");
						RDOFUNLogic* logic = select->createFunSelectEmpty( empty_info );
						$$ = (int)logic;
					}
					| fun_select_body '.' Empty_kw '(' error {
						parser->error( @4, "Ожидается закрывающаяся скобка" );
					}
					| fun_select_body '.' Empty_kw error {
						parser->error( @3, "Ожидается октрывающаяся скобка" );
					}
					| fun_select_body '.' error {
						parser->error( @2, @3, "Ожидается метод списка ресурсов" );
					}
					| fun_select_body error {
						parser->error( @1, "Ожидается '.' (точка) для вызова метода списка ресурсов" );
					};

fun_select_arithm:	fun_select_body '.' Size_kw '(' ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcPos( @1, @5 );
						RDOParserSrcInfo size_info(@3, @5, "Size()");
						RDOFUNArithm* arithm = select->createFunSelectSize( size_info );
						$$ = (int)arithm;
					}
					| fun_select_body '.' Size_kw error {
						parser->error( @3, "Ожидается октрывающаяся скобка" );
					}
					| fun_select_body '.' Size_kw '(' error {
						parser->error( @4, "Ожидается закрывающаяся скобка" );
					};

%%

}

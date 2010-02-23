/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdofun.y
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

%{
#define YYPARSE_PARAM lexer
#define YYLEX_PARAM lexer
%}

%pure-parser

%token RDO_Resource_type				257
%token RDO_permanent					258
%token RDO_Parameters					259
%token RDO_integer						260
%token RDO_real							261
%token RDO_End							262
%token RDO_temporary					263
%token RDO_IDENTIF						264
%token RDO_INT_CONST					265
%token RDO_REAL_CONST					266
%token RDO_such_as						267
%token RDO_dblpoint						268
%token RDO_Resources					269
%token RDO_trace						270
%token RDO_no_trace						271
%token RDO_IDENTIF_COLON				272
%token RDO_Constant						273
%token RDO_Body							274
%token RDO_Function						275
%token RDO_Type							276
%token RDO_algorithmic					277
%token RDO_table						278
%token RDO_list							279
%token RDO_Exist						281
%token RDO_Not_Exist					282
%token RDO_For_All						283
%token RDO_Not_For_All					284
%token RDO_neq							285
%token RDO_leq							286
%token RDO_geq							287
%token RDO_NoCheck						288
%token RDO_Calculate_if					289
%token RDO_or							290
%token RDO_and							291
%token RDO_Sequence						292
%token RDO_uniform						293
%token RDO_exponential					294
%token RDO_normal						295
%token RDO_by_hist						296
%token RDO_enumerative					297

%token RDO_Pattern						298
%token RDO_operation					299
%token RDO_irregular_event				300
%token RDO_rule							301
%token RDO_keyboard						302
%token RDO_Relevant_resources			303
%token RDO_Keep							304
%token RDO_Create						305
%token RDO_Erase						306
%token RDO_NonExist						307
%token RDO_IDENTIF_NoChange				308
%token RDO_Time							309
%token RDO_Choice						310
%token RDO_from							311
%token RDO_first						312
%token RDO_Convert_begin				313
%token RDO_Convert_end					314
%token RDO_Convert_rule					315
%token RDO_Convert_event				316
%token RDO_with_max						317
%token RDO_with_min						318
%token RDO_set							319
%token RDO_IDENTIF_NoChange_NoChange	320
%token RDO_Operations					321
	
%token RDO_Results						322
%token RDO_watch_par					323
%token RDO_watch_state					324
%token RDO_watch_quant					325
%token RDO_watch_value					326
%token RDO_get_value					327

%token RDO_Model_name					328
%token RDO_Resource_file				329
%token RDO_OprIev_file					330
%token RDO_Frame_file					331
%token RDO_Statistic_file				332
%token RDO_Results_file					333
%token RDO_Trace_file					334
%token RDO_Show_mode					335
%token RDO_Frame_number					336
%token RDO_Show_rate					337
%token RDO_Run_StartTime				338
%token RDO_Trace_StartTime				339
%token RDO_Trace_EndTime				340
%token RDO_Terminate_if					341
%token RDO_Break_point					342
%token RDO_Seed							343
%token RDO_NoShow						344
%token RDO_Monitor						345
%token RDO_Animation					346
%token RDO_NoChange						347

%token RDO_Decision_point				348
%token RDO_search						349
%token RDO_trace_stat					350
%token RDO_trace_tops					351
%token RDO_trace_all					352
%token RDO_Condition					353
%token RDO_Term_condition				354
%token RDO_Evaluate_by					355
%token RDO_Compare_tops					356
%token RDO_NO							357
%token RDO_YES							358
%token RDO_Activities					359
%token RDO_value_before					360
%token RDO_value_after					361
%token RDO_some							362
%token RDO_Process						363
%token RDO_SEIZE						364
%token RDO_GENERATE						365
%token RDO_TERMINATE					366
%token RDO_ADVANCE						367
%token RDO_RELEASE						368
%token RDO_if							369
%token RDO_result						370
%token RDO_CF							371
%token RDO_Priority						372
%token RDO_prior						373
%token RDO_Parent						374
%token RDO_PlusEqual					375
%token RDO_MinusEqual					376
%token RDO_MultiplyEqual				377
%token RDO_DivideEqual					378

%token RDO_Frame						400
%token RDO_Show_if						401
%token RDO_Back_picture					402
%token RDO_Show							403
%token RDO_frm_cell						404
%token RDO_text							405
%token RDO_bitmap						406
%token RDO_s_bmp						407
%token RDO_rect							408
%token RDO_r_rect						409
%token RDO_line							410
%token RDO_ellipse						411
%token RDO_triang						412
%token RDO_active						413
%token RDO_ruler						414
%token RDO_space						415
%token RDO_color_transparent			416
%token RDO_color_last					417
%token RDO_color_white					418
%token RDO_color_black					419
%token RDO_color_red					420
%token RDO_color_green					421
%token RDO_color_blue					422
%token RDO_color_cyan					423
%token RDO_color_magenta				424
%token RDO_color_yellow					425
%token RDO_color_gray					426

%token RDO_IDENTIF_RELRES				427
%token RDO_typedef						428
%token RDO_enum							429

%token RDO_STRING_CONST					430
%token RDO_STRING_CONST_BAD				431
%token RDO_IDENTIF_BAD					432
%token RDO_Select						433
%token RDO_Size							434
%token RDO_Empty						435
%token RDO_not							436
%token RDO_UMINUS						437
%token RDO_string						438
%token RDO_bool							439
%token RDO_BOOL_CONST					440
%token RDO_Fuzzy						441
%token RDO_Fuzzy_Term					442
%token RDO_eq							443
%token RDO_External_Model				444
%token RDO_QUEUE						445
%token RDO_DEPART						446
%token RDO_ASSIGN						447


%{
// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ===============================================================================

#define PARSER  LEXER->parser()
#define RUNTIME PARSER->runtime()

#define P_RDOVALUE(A) reinterpret_cast<PTR(RDOValue)>(A)
#define P_ARITHM(A)   reinterpret_cast<PTR(RDOFUNArithm)>(A)
#define P_LOGIC(A)    reinterpret_cast<PTR(RDOFUNLogic)>(A)

#define RDOVALUE(A)   (*P_RDOVALUE(A))
#define ARITHM(A)     (*P_ARITHM(A))
#define LOGIC(A)      (*P_LOGIC(A))

OPEN_RDO_PARSER_NAMESPACE
%}

%left RDO_or
%left RDO_and	
%left '+' '-'
%left '*' '/'
%left RDO_not
%left RDO_UMINUS

%start fun_list

%%

fun_list:	fun_consts fun_func_seq;
			| error {
				if ( PARSER->getFUNConstants().empty() ) {
					PARSER->error().error( @1, "Ожидается описание функции, последовательности или константы" );
				} else {
					PARSER->error().error( @1, "Ожидается описание функции или последовательности" );
				}
			};

// ----------------------------------------------------------------------------
// ---------- Константы
// ----------------------------------------------------------------------------
fun_consts:		/* empty */
				| RDO_Constant fun_const_body RDO_End
				| RDO_Constant fun_const_body
				{
					PARSER->error().error( @2, "После описания констант ожидается ключевое слово $End" );
				};

fun_const_body:	/* empty */
				| fun_const_body fun_const_param_desc
				| fun_const_body error
				{
					PARSER->error().error( @2, "Ожидается описание константы" );
				};

fun_const_param_desc:	RDO_IDENTIF_COLON param_type
						{
							RDOValue* name = reinterpret_cast<RDOValue*>($1);
							PARSER->checkFunctionName( name->src_info() );
							RDORTPParamType* parType = reinterpret_cast<RDORTPParamType*>($2);
							if ( !parType->getDV().isExist() )
							{
								PARSER->error().error( @2, "Константа должна иметь значение" );
							}
							RDOFUNConst* param = new RDOFUNConst( PARSER, name->src_info(), parType );
							RDOFUNConstant* newConst = new RDOFUNConstant( PARSER, param );
							newConst->setSrcInfo( name->src_info() );
							RUNTIME->setConstValue( newConst->getNumber(), newConst->getType()->getDefaultValue( RDOParserSrcInfo(param->src_info()) ) );
							$$ = (int)newConst;
						}
						| RDO_IDENTIF_COLON
						{
							PARSER->error().error( @1, "Ожидается тип константы" );
						}
						| RDO_IDENTIF_COLON error
						{
							PARSER->error().error( @2, "Ошибка описания типа константы" );
						};

// ----------------------------------------------------------------------------
// ---------- Последовательности и функции
// ----------------------------------------------------------------------------
fun_func_seq:	/* empty */
			| fun_func_seq fun_func_descr
			| fun_func_seq fun_seq_descr
			| fun_func_seq RDO_Constant {
				PARSER->error().error( @2, "Константы долны быть описаны первыми, перед функциями и последовательностями" );
			};

// ----------------------------------------------------------------------------
// ---------- Функции
// ----------------------------------------------------------------------------
fun_func_descr:	fun_func_header fun_func_footer
				| fun_func_header error {
					RDOFUNFunction* fun = reinterpret_cast<RDOFUNFunction*>($1);
					PARSER->error().error( @2, rdo::format("Ожидается ключевое слово $Type с указанием типа функции '%s'", fun->name().c_str()) );
				};

fun_func_header:	RDO_Function RDO_IDENTIF_COLON param_type {
						RDOValue* name = reinterpret_cast<RDOValue*>($2);
						PARSER->checkFunctionName( name->src_info() );
						RDORTPParamType* retType = reinterpret_cast<RDORTPParamType*>($3);
						RDOFUNFunction* fun = new RDOFUNFunction( PARSER, name->src_info(), retType );
						if ( retType->typeID() == rdoRuntime::RDOType::t_enum && static_cast<RDORTPEnumParamType*>(retType)->enum_name.empty() ) {
							static_cast<RDORTPEnumParamType*>(retType)->enum_name = name->value().getIdentificator();
							static_cast<RDORTPEnumParamType*>(retType)->enum_fun  = true;
						}
						$$ = (int)fun;
					}
					| RDO_Function RDO_IDENTIF_COLON error {
						std::string name = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
						PARSER->error().error( @3, rdo::format("Ожидается тип возвращаемого значения функции '%s'", name.c_str()) );
					}
					| RDO_Function error {
						PARSER->error().error( @2, "После ключевого слова $Function ожидается имя фунции" );
					};

fun_func_parameters:	/* empty */
					| RDO_Parameters fun_func_params {
					};

fun_func_params:	/* empty */
				| fun_func_params RDO_IDENTIF_COLON param_type {
					RDOValue*        name = reinterpret_cast<RDOValue*>($2);
					RDORTPParamType* type = reinterpret_cast<RDORTPParamType*>($3);
					RDOFUNFunctionParam* param = new RDOFUNFunctionParam( PARSER->getLastFUNFunction(), name->src_info(), type );
					PARSER->getLastFUNFunction()->add( param );
				}
				| fun_func_params RDO_IDENTIF_COLON error {
					PARSER->error().error( @3, "Ожидается тип параметра функции" );
				}
				| fun_func_params error {
					PARSER->error().error( @2, "Ожидается описание параметра функции в формате <имя>: <тип>" );
				};

fun_func_footer:	RDO_Type '=' RDO_algorithmic fun_func_parameters RDO_Body fun_func_algorithmic_body RDO_End {
						RDOFUNFunction* currFunc = PARSER->getLastFUNFunction();
						currFunc->createAlgorithmicCalc( @5 );
					}
					| RDO_Type '=' RDO_list fun_func_parameters RDO_Body fun_func_list_body RDO_End {
						RDOFUNFunction* currFunc = PARSER->getLastFUNFunction();
						currFunc->createListCalc();
					}
					| RDO_Type '=' RDO_table fun_func_parameters RDO_Body fun_func_list_body RDO_End {
						RDOFUNFunction* currFunc = PARSER->getLastFUNFunction();
						currFunc->createTableCalc( @6 );
					}
					| RDO_Type '=' RDO_algorithmic fun_func_parameters RDO_Body fun_func_algorithmic_body error {
						PARSER->error().error( @7, "Ожидается ключевое слово $End" );
					}
					| RDO_Type '=' RDO_list fun_func_parameters RDO_Body fun_func_list_body error {
						PARSER->error().error( @7, "Ожидается ключевое слово $End" );
					}
					| RDO_Type '=' RDO_table fun_func_parameters RDO_Body fun_func_list_body error {
						PARSER->error().error( @7, "Ожидается ключевое слово $End" );
					}
					| RDO_Type '=' RDO_algorithmic error {
						PARSER->error().error( @4, "Ожидается ключевое слово $Parameters" );
					}
					| RDO_Type '=' RDO_list error {
						PARSER->error().error( @4, "Ожидается ключевое слово $Parameters" );
					}
					| RDO_Type '=' RDO_table error {
						PARSER->error().error( @4, "Ожидается ключевое слово $Parameters" );
					}
					| RDO_Type '=' error {
						PARSER->error().error( @3, "Неизвестный тип функции" );
					}
					| RDO_Type error {
						PARSER->error().error( @2, "После ключевого слова $Type ожидается тип функции" );
					};

fun_func_algorithmic_body:	/* empty */
							| fun_func_algorithmic_body fun_func_algorithmic_calc_if {
								RDOFUNCalculateIf* calculateIf = reinterpret_cast<RDOFUNCalculateIf*>($2);
								PARSER->getLastFUNFunction()->add( calculateIf );
							};

fun_func_calc_if:	RDO_Calculate_if {
					}
					| RDO_if {
					};

fun_func_calc_name:	RDO_result {
					}
					| RDO_IDENTIF {
						std::string name = reinterpret_cast<RDOValue*>($1)->value().getIdentificator();
						if ( name != PARSER->getLastFUNFunction()->name() ) {
							PARSER->error().error( @1, rdo::format("Ожидается имя функции '%s'", PARSER->getLastFUNFunction()->name().c_str()) );
						}
					};

fun_func_algorithmic_calc_if:	fun_func_calc_if fun_logic fun_func_calc_name '=' fun_arithm {
									RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
									logic->setSrcText( "Calculate_if " + logic->src_text() );
									RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($5);
									RDOFUNCalculateIf* calculateIf = new RDOFUNCalculateIf( PARSER->getLastFUNFunction(), logic, arithm );
									$$ = (int)calculateIf;
								}
								| fun_func_calc_if fun_logic fun_func_calc_name '=' error {
									PARSER->error().error( @5, "Ошибка в арифметическом выражении" );
								}
								| fun_func_calc_name '=' fun_arithm {
									rdoRuntime::RDOCalcConst* calc_cond = new rdoRuntime::RDOCalcConst( RUNTIME, 1 );
									RDOParserSrcInfo logic_src_info( "Calculate_if 1 = 1" );
									logic_src_info.setSrcPos( @1.first_line, @1.first_column, @1.first_line, @1.first_column );
									calc_cond->setSrcInfo( logic_src_info );
									RDOFUNLogic* logic = new RDOFUNLogic( PARSER->getLastFUNFunction(), calc_cond, true );
									logic->setSrcInfo( logic_src_info );
									RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($3);
									RDOFUNCalculateIf* calculateIf = new RDOFUNCalculateIf( PARSER->getLastFUNFunction(), logic, arithm );
									$$ = (int)calculateIf;
								}
								| fun_func_calc_name '=' error {
									PARSER->error().error( @3, "Ошибка в арифметическом выражении" );
								}
								| fun_func_calc_if fun_logic fun_func_calc_name error {
									PARSER->error().error( @4, "Ожидается '='" );
								}
								| fun_func_calc_name error {
									PARSER->error().error( @2, "Ожидается '='" );
								}
								| fun_func_calc_if fun_logic error {
									PARSER->error().error( @2, @3, "После логического выражения ожидается <имя_функции> = <результат_функции>" );
								}
								| fun_func_calc_if error {
									PARSER->error().error( @2, "Ошибка в логическом выражении" );
								}
								| error {
									PARSER->error().error( @1, "Ожидается ключевое слово Calculate_if" );
								};

fun_func_list_body:	/* empty */
			| fun_func_list_body fun_func_list_value;

fun_func_list_value: RDO_IDENTIF {
						std::string str = reinterpret_cast<RDOValue*>($1)->value().getIdentificator();
						RDOFUNFunctionListElementIdentif* value = new RDOFUNFunctionListElementIdentif( PARSER->getLastFUNFunction(), RDOParserSrcInfo( @1, str ) );
						PARSER->getLastFUNFunction()->add( value );
						$$ = (int)value;
					}
					| RDO_REAL_CONST {
						RDOFUNFunctionListElementReal* value = new RDOFUNFunctionListElementReal( PARSER->getLastFUNFunction(), @1, reinterpret_cast<RDOValue*>($1)->value().getDouble() );
						PARSER->getLastFUNFunction()->add( value );
						$$ = (int)value;
					}
					| RDO_INT_CONST {
						RDOFUNFunctionListElementInt* value = new RDOFUNFunctionListElementInt( PARSER->getLastFUNFunction(), @1, reinterpret_cast<RDOValue*>($1)->value().getInt() );
						PARSER->getLastFUNFunction()->add( value );
						$$ = (int)value;
					}
					| '=' {
						RDOFUNFunctionListElementEq* value = new RDOFUNFunctionListElementEq( PARSER->getLastFUNFunction(), @1 );
						PARSER->getLastFUNFunction()->add( value );
						$$ = (int)value;
					};

// ----------------------------------------------------------------------------
// ---------- Последовательности
// ----------------------------------------------------------------------------
fun_seq_descr:		fun_seq_uniform
					| fun_seq_exponential
					| fun_seq_normal
					| fun_seq_by_hist
					| fun_seq_enumerative;

fun_seq_header:		RDO_Sequence RDO_IDENTIF_COLON param_type RDO_Type '=' {
						RDOValue* name = reinterpret_cast<RDOValue*>($2);
						PARSER->checkFunctionName( name->src_info() );
						$$ = (int)(new RDOFUNSequence::RDOFUNSequenceHeader( PARSER, reinterpret_cast<RDORTPParamType*>($3), name->src_info() ));
					}
					| RDO_Sequence RDO_IDENTIF_COLON param_type RDO_Type '=' error {
						PARSER->error().error( @6, "После знака равенства ожидается тип последовательности" );
					}
					| RDO_Sequence RDO_IDENTIF_COLON param_type RDO_Type error {
						PARSER->error().error( @5, "После ключевого слова $Type ожидается знак равенства и тип последовательности" );
					}
					| RDO_Sequence RDO_IDENTIF_COLON param_type error {
						PARSER->error().error( @4, "Ожидается ключевое слово $Type" );
					}
					| RDO_Sequence RDO_IDENTIF_COLON error {
						PARSER->error().error( @2, @3, "После имени последовательности ожидается тип возвращаемого значения" );
					}
					| RDO_Sequence error {
						PARSER->error().error( @1, @2, "После ключевого слова $Sequence ожидаются имя и тип результата последовательности в формате '<имя> : <тип>'" );
					};

// ----------------------------------------------------------------------------
// ---------- Датчики случайных чисел
// ----------------------------------------------------------------------------
fun_seq_uniform:	fun_seq_header RDO_uniform RDO_End {
						RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequence::RDOFUNSequenceHeader*>($1);
						RDOFUNSequence* seq = new RDOFUNSequenceUniform( PARSER, header );
						seq->createCalcs();
						$$ = (int)seq;
					}
					| fun_seq_header RDO_uniform RDO_INT_CONST RDO_End {
						RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequence::RDOFUNSequenceHeader*>($1);
						RDOFUNSequence* seq = new RDOFUNSequenceUniform( PARSER, header, reinterpret_cast<RDOValue*>($3)->value().getInt() );
						seq->createCalcs();
						$$ = (int)seq;
					}
					| fun_seq_header RDO_uniform RDO_INT_CONST error {
						PARSER->error().error( @4, "После базы ожидается ключевое слово $End" );
					}
					| fun_seq_header RDO_uniform error {
						PARSER->error().error( @3, "После типа последовательности ожидается база генератора или ключевое слово $End" );
					};

fun_seq_exponential: fun_seq_header RDO_exponential RDO_End {
						RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequence::RDOFUNSequenceHeader*>($1);
						RDOFUNSequence* seq = new RDOFUNSequenceExponential( PARSER, header, $3 );
						seq->createCalcs();
						$$ = (int)seq;
					}
					| fun_seq_header RDO_exponential RDO_INT_CONST RDO_End {
						RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequence::RDOFUNSequenceHeader*>($1);
						RDOFUNSequence* seq = new RDOFUNSequenceExponential( PARSER, header, reinterpret_cast<RDOValue*>($3)->value().getInt() );
						seq->createCalcs();
						$$ = (int)seq;
					}
					| fun_seq_header RDO_exponential RDO_INT_CONST error {
						PARSER->error().error( @4, "После базы ожидается ключевое слово $End" );
					}
					| fun_seq_header RDO_exponential error {
						PARSER->error().error( @3, "После типа последовательности ожидается база генератора или ключевое слово $End" );
					};

fun_seq_normal:		fun_seq_header RDO_normal RDO_End {
						RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequence::RDOFUNSequenceHeader*>($1);
						RDOFUNSequence* seq = new RDOFUNSequenceNormal( PARSER, header );
						seq->createCalcs();
						$$ = (int)seq;
					}
					| fun_seq_header RDO_normal RDO_INT_CONST RDO_End {
						RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequence::RDOFUNSequenceHeader*>($1);
						RDOFUNSequence* seq = new RDOFUNSequenceNormal( PARSER, header, reinterpret_cast<RDOValue*>($3)->value().getInt() );
						seq->createCalcs();
						$$ = (int)seq;
					}
					| fun_seq_header RDO_normal RDO_INT_CONST error {
						PARSER->error().error( @4, "После базы ожидается ключевое слово $End" );
					}
					| fun_seq_header RDO_normal error {
						PARSER->error().error( @3, "После типа последовательности ожидается база генератора или ключевое слово $End" );
					};

// ----------------------------------------------------------------------------
// ---------- Гистограмма
// ----------------------------------------------------------------------------
fun_seq_by_hist_header:		fun_seq_header RDO_by_hist RDO_Body {
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequence::RDOFUNSequenceHeader*>($1);
								$$ = (int)new RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader( PARSER, header );
							}
							| fun_seq_header RDO_by_hist RDO_INT_CONST RDO_Body {
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequence::RDOFUNSequenceHeader*>($1);
								$$ = (int)new RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader( PARSER, header, reinterpret_cast<RDOValue*>($3)->value().getInt() );
							}
							| fun_seq_header RDO_by_hist RDO_INT_CONST error {
								PARSER->error().error( @4, "После базы ожидается ключевое слово $Body" );
							}
							| fun_seq_header RDO_by_hist error {
								PARSER->error().error( @3, "После типа последовательности ожидается база генератора или ключевое слово $Body" );
							};

fun_seq_by_hist_body_real:	fun_seq_by_hist_header RDO_REAL_CONST RDO_REAL_CONST RDO_REAL_CONST
							{
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->typeID() != rdoRuntime::RDOType::t_real && header->header->getType()->typeID() != rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								if ( header->header->getType()->typeID() == rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @2, rdo::format("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными", header->header->src_text().c_str()) );
								}
								$$ = (int)new RDOFUNSequenceByHistReal( PARSER, header, *reinterpret_cast<RDOValue*>($2), *reinterpret_cast<RDOValue*>($3), *reinterpret_cast<RDOValue*>($4) );
							}
							| fun_seq_by_hist_header RDO_INT_CONST RDO_REAL_CONST RDO_REAL_CONST {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->typeID() != rdoRuntime::RDOType::t_real && header->header->getType()->typeID() != rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								if ( header->header->getType()->typeID() == rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @3, rdo::format("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными", header->header->src_text().c_str()) );
								}
								$$ = (int)new RDOFUNSequenceByHistReal( PARSER, header, *reinterpret_cast<RDOValue*>($2), *reinterpret_cast<RDOValue*>($3), *reinterpret_cast<RDOValue*>($4) );
							}
							| fun_seq_by_hist_header RDO_REAL_CONST RDO_INT_CONST RDO_REAL_CONST {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->typeID() != rdoRuntime::RDOType::t_real && header->header->getType()->typeID() != rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								if ( header->header->getType()->typeID() == rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @2, rdo::format("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными", header->header->src_text().c_str()) );
								}
								$$ = (int)new RDOFUNSequenceByHistReal( PARSER, header, *reinterpret_cast<RDOValue*>($2), *reinterpret_cast<RDOValue*>($3), *reinterpret_cast<RDOValue*>($4) );
							}
							| fun_seq_by_hist_header RDO_REAL_CONST RDO_REAL_CONST RDO_INT_CONST {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->typeID() != rdoRuntime::RDOType::t_real && header->header->getType()->typeID() != rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								if ( header->header->getType()->typeID() == rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @2, rdo::format("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными", header->header->src_text().c_str()) );
								}
								$$ = (int)new RDOFUNSequenceByHistReal( PARSER, header, *reinterpret_cast<RDOValue*>($2), *reinterpret_cast<RDOValue*>($3), *reinterpret_cast<RDOValue*>($4) );
							}
							| fun_seq_by_hist_header RDO_INT_CONST RDO_INT_CONST RDO_REAL_CONST {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->typeID() != rdoRuntime::RDOType::t_real && header->header->getType()->typeID() != rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								$$ = (int)new RDOFUNSequenceByHistReal( PARSER, header, *reinterpret_cast<RDOValue*>($2), *reinterpret_cast<RDOValue*>($3), *reinterpret_cast<RDOValue*>($4) );
							}
							| fun_seq_by_hist_header RDO_REAL_CONST RDO_INT_CONST RDO_INT_CONST {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->typeID() != rdoRuntime::RDOType::t_real && header->header->getType()->typeID() != rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								if ( header->header->getType()->typeID() == rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @2, rdo::format("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными", header->header->src_text().c_str()) );
								}
								$$ = (int)new RDOFUNSequenceByHistReal( PARSER, header, *reinterpret_cast<RDOValue*>($2), *reinterpret_cast<RDOValue*>($3), *reinterpret_cast<RDOValue*>($4) );
							}
							| fun_seq_by_hist_header RDO_INT_CONST RDO_REAL_CONST RDO_INT_CONST {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->typeID() != rdoRuntime::RDOType::t_real && header->header->getType()->typeID() != rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								if ( header->header->getType()->typeID() == rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @3, rdo::format("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными", header->header->src_text().c_str()) );
								}
								$$ = (int)new RDOFUNSequenceByHistReal( PARSER, header, *reinterpret_cast<RDOValue*>($2), *reinterpret_cast<RDOValue*>($3), *reinterpret_cast<RDOValue*>($4) );
							}
							| fun_seq_by_hist_header RDO_INT_CONST RDO_INT_CONST RDO_INT_CONST {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->typeID() != rdoRuntime::RDOType::t_real && header->header->getType()->typeID() != rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								$$ = (int)new RDOFUNSequenceByHistReal( PARSER, header, *reinterpret_cast<RDOValue*>($2), *reinterpret_cast<RDOValue*>($3), *reinterpret_cast<RDOValue*>($4) );
							}
							| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_REAL_CONST RDO_REAL_CONST {
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->getType()->typeID() == rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @2, rdo::format("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными", header->src_text().c_str()) );
								}
								reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->addReal( *reinterpret_cast<RDOValue*>($2), *reinterpret_cast<RDOValue*>($3), *reinterpret_cast<RDOValue*>($4) );
								$$ = $1;
							}
							| fun_seq_by_hist_body_real RDO_INT_CONST RDO_REAL_CONST RDO_REAL_CONST {
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->getType()->typeID() == rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @3, rdo::format("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными", header->src_text().c_str()) );
								}
								reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->addReal( *reinterpret_cast<RDOValue*>($2), *reinterpret_cast<RDOValue*>($3), *reinterpret_cast<RDOValue*>($4) );
								$$ = $1;
							}
							| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_INT_CONST RDO_REAL_CONST {
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->getType()->typeID() == rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @2, rdo::format("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными", header->src_text().c_str()) );
								}
								reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->addReal( *reinterpret_cast<RDOValue*>($2), *reinterpret_cast<RDOValue*>($3), *reinterpret_cast<RDOValue*>($4) );
								$$ = $1;
							}
							| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_REAL_CONST RDO_INT_CONST {
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->getType()->typeID() == rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @2, rdo::format("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными", header->src_text().c_str()) );
								}
								reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->addReal( *reinterpret_cast<RDOValue*>($2), *reinterpret_cast<RDOValue*>($3), *reinterpret_cast<RDOValue*>($4) );
								$$ = $1;
							}
							| fun_seq_by_hist_body_real RDO_INT_CONST RDO_INT_CONST RDO_REAL_CONST {
								reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->addReal( *reinterpret_cast<RDOValue*>($2), *reinterpret_cast<RDOValue*>($3), *reinterpret_cast<RDOValue*>($4) );
								$$ = $1;
							}
							| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_INT_CONST RDO_INT_CONST {
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->getType()->typeID() == rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @2, rdo::format("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными", header->src_text().c_str()) );
								}
								reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->addReal( *reinterpret_cast<RDOValue*>($2), *reinterpret_cast<RDOValue*>($3), *reinterpret_cast<RDOValue*>($4) );
								$$ = $1;
							}
							| fun_seq_by_hist_body_real RDO_INT_CONST RDO_REAL_CONST RDO_INT_CONST {
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->getType()->typeID() == rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @3, rdo::format("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными", header->src_text().c_str()) );
								}
								reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->addReal( *reinterpret_cast<RDOValue*>($2), *reinterpret_cast<RDOValue*>($3), *reinterpret_cast<RDOValue*>($4) );
								$$ = $1;
							}
							| fun_seq_by_hist_body_real RDO_INT_CONST RDO_INT_CONST RDO_INT_CONST {
								reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->addReal( *reinterpret_cast<RDOValue*>($2), *reinterpret_cast<RDOValue*>($3), *reinterpret_cast<RDOValue*>($4) );
								$$ = $1;
							}
							| fun_seq_by_hist_header RDO_REAL_CONST error {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->typeID() != rdoRuntime::RDOType::t_real && header->header->getType()->typeID() != rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								PARSER->error().error( @2, @3, "Ожидается конец диапазона" );
							}
							| fun_seq_by_hist_header RDO_INT_CONST error {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->typeID() != rdoRuntime::RDOType::t_real && header->header->getType()->typeID() != rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								PARSER->error().error( @2, @3, "Ожидается конец диапазона" );
							}
							| fun_seq_by_hist_header RDO_REAL_CONST RDO_REAL_CONST error {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->typeID() != rdoRuntime::RDOType::t_real && header->header->getType()->typeID() != rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								PARSER->error().error( @3, @4, "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_header RDO_INT_CONST RDO_REAL_CONST error {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->typeID() != rdoRuntime::RDOType::t_real && header->header->getType()->typeID() != rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								PARSER->error().error( @3, @4, "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_header RDO_REAL_CONST RDO_INT_CONST error {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->typeID() != rdoRuntime::RDOType::t_real && header->header->getType()->typeID() != rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								PARSER->error().error( @3, @4, "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_header RDO_INT_CONST RDO_INT_CONST error {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->typeID() != rdoRuntime::RDOType::t_real && header->header->getType()->typeID() != rdoRuntime::RDOType::t_int ) {
									PARSER->error().error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								PARSER->error().error( @3, @4, "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_body_real RDO_REAL_CONST error {
								PARSER->error().error( @2, @3, "Ожидается конец диапазона" );
							}
							| fun_seq_by_hist_body_real RDO_INT_CONST error {
								PARSER->error().error( @2, @3, "Ожидается конец диапазона" );
							}
							| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_REAL_CONST error {
								PARSER->error().error( @3, @4, "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_body_real RDO_INT_CONST RDO_REAL_CONST error {
								PARSER->error().error( @3, @4, "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_INT_CONST error {
								PARSER->error().error( @3, @4, "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_body_real RDO_INT_CONST RDO_INT_CONST error {
								PARSER->error().error( @3, @4, "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_header RDO_End {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								PARSER->error().error( header->src_info(), rdo::format("Последовательность '%s' не должна быть пустой", header->src_text().c_str()) );
							};

fun_seq_by_hist_body_enum:	fun_seq_by_hist_header RDO_IDENTIF RDO_REAL_CONST {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->typeID() != rdoRuntime::RDOType::t_enum ) {
									PARSER->error().error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								$$ = (int)new RDOFUNSequenceByHistEnum( PARSER, header, *reinterpret_cast<RDOValue*>($2), *reinterpret_cast<RDOValue*>($3) );
							}
							| fun_seq_by_hist_header RDO_IDENTIF RDO_INT_CONST {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->typeID() != rdoRuntime::RDOType::t_enum ) {
									PARSER->error().error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								$$ = (int)new RDOFUNSequenceByHistEnum( PARSER, header, *reinterpret_cast<RDOValue*>($2), *reinterpret_cast<RDOValue*>($3) );
							}
							| fun_seq_by_hist_body_enum RDO_IDENTIF RDO_REAL_CONST {
								reinterpret_cast<RDOFUNSequenceByHistEnum*>($1)->addEnum( *reinterpret_cast<RDOValue*>($2), *reinterpret_cast<RDOValue*>($3) );
								$$ = $1;
							}
							| fun_seq_by_hist_body_enum RDO_IDENTIF RDO_INT_CONST {
								reinterpret_cast<RDOFUNSequenceByHistEnum*>($1)->addEnum( *reinterpret_cast<RDOValue*>($2), *reinterpret_cast<RDOValue*>($3) );
								$$ = $1;
							}
							| fun_seq_by_hist_header RDO_IDENTIF error {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->typeID() != rdoRuntime::RDOType::t_enum ) {
									PARSER->error().error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								PARSER->error().error( @2, @3, rdo::format("Ожидается относительная вероятность для значения: %s", reinterpret_cast<RDOValue*>($2)->value().getIdentificator().c_str()) );
							}
							| fun_seq_by_hist_body_enum RDO_IDENTIF error {
								PARSER->error().error( @2, @3, rdo::format("Ожидается относительная вероятность для значения: %s", reinterpret_cast<RDOValue*>($2)->value().getIdentificator().c_str()) );
							}
							| fun_seq_by_hist_body_enum RDO_REAL_CONST error {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								PARSER->error().error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
							}
							| fun_seq_by_hist_body_enum RDO_INT_CONST error {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								PARSER->error().error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
							};

fun_seq_by_hist:	fun_seq_by_hist_body_real RDO_End {
						RDOFUNSequence* seq = reinterpret_cast<RDOFUNSequence*>($1);
						seq->createCalcs();
					}
					| fun_seq_by_hist_body_enum RDO_End {
						RDOFUNSequence* seq = reinterpret_cast<RDOFUNSequence*>($1);
						seq->createCalcs();
					}
					| fun_seq_by_hist_body_real error {
						PARSER->error().error( @2, "Ошибка в описании последовательности" );
					}
					| fun_seq_by_hist_body_enum error {
						PARSER->error().error( @2, "Ошибка в описании последовательности" );
					}
					| fun_seq_by_hist_body_real {
						PARSER->error().error( @1, "Ожидается ключевое слово $End" );
					}
					| fun_seq_by_hist_body_enum {
						PARSER->error().error( @1, "Ожидается ключевое слово $End" );
					};

// ----------------------------------------------------------------------------
// ---------- Перечень значений
// ----------------------------------------------------------------------------
fun_seq_enumerative:		fun_seq_enumerative_body RDO_End
							{
								reinterpret_cast<RDOFUNSequence*>($1)->createCalcs();
							}
							| fun_seq_enumerative_header RDO_End
							{
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader*>($1);
								PARSER->error().error( header->src_info(), rdo::format("Последовательность '%s' не должна быть пустой", header->src_text().c_str()) );
							};

fun_seq_enumerative_header:	fun_seq_header RDO_enumerative RDO_Body {
							}
							| fun_seq_header RDO_enumerative RDO_INT_CONST error {
								PARSER->error().error( @3, "У последовательности типа enumerative нет базы генератора" );
							}
							| fun_seq_header RDO_enumerative RDO_Parameters error {
								PARSER->error().error( @3, "У последовательности типа enumerative нет параметров" );
							}
							| fun_seq_header RDO_enumerative error {
								PARSER->error().error( @3, "После типа последовательности ожидается ключевое слово $Body" );
							};

fun_seq_enumerative_body:	  fun_seq_enumerative_header RDO_INT_CONST      { $$ = (int)new RDOFUNSequenceEnumerative( PARSER, reinterpret_cast<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader*>($1), *reinterpret_cast<RDOValue*>($2) ); }
							| fun_seq_enumerative_header RDO_REAL_CONST     { $$ = (int)new RDOFUNSequenceEnumerative( PARSER, reinterpret_cast<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader*>($1), *reinterpret_cast<RDOValue*>($2) ); }
							| fun_seq_enumerative_header RDO_BOOL_CONST     { $$ = (int)new RDOFUNSequenceEnumerative( PARSER, reinterpret_cast<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader*>($1), *reinterpret_cast<RDOValue*>($2) ); }
							| fun_seq_enumerative_header RDO_STRING_CONST   { $$ = (int)new RDOFUNSequenceEnumerative( PARSER, reinterpret_cast<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader*>($1), *reinterpret_cast<RDOValue*>($2) ); }
							| fun_seq_enumerative_header RDO_IDENTIF        { $$ = (int)new RDOFUNSequenceEnumerative( PARSER, reinterpret_cast<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader*>($1), *reinterpret_cast<RDOValue*>($2) ); }

							| fun_seq_enumerative_body   RDO_INT_CONST      { reinterpret_cast<RDOFUNSequenceEnumerative*>($1)->addValue( *reinterpret_cast<RDOValue*>($2) ); }
							| fun_seq_enumerative_body   RDO_REAL_CONST     { reinterpret_cast<RDOFUNSequenceEnumerative*>($1)->addValue( *reinterpret_cast<RDOValue*>($2) ); }
							| fun_seq_enumerative_body   RDO_BOOL_CONST     { reinterpret_cast<RDOFUNSequenceEnumerative*>($1)->addValue( *reinterpret_cast<RDOValue*>($2) ); }
							| fun_seq_enumerative_body   RDO_STRING_CONST   { reinterpret_cast<RDOFUNSequenceEnumerative*>($1)->addValue( *reinterpret_cast<RDOValue*>($2) ); }
							| fun_seq_enumerative_body   RDO_IDENTIF        { reinterpret_cast<RDOFUNSequenceEnumerative*>($1)->addValue( *reinterpret_cast<RDOValue*>($2) ); }

							| fun_seq_enumerative_body error
							{
								PARSER->error().error( @1, @2, "Ожидается элемент последовательности или ключевое слово $End" );
							};

// ----------------------------------------------------------------------------
// ---------- Описание типа параметра
// ----------------------------------------------------------------------------
param_type:		RDO_integer param_int_diap param_int_default_val
				{
					PTR(RDORTPIntDiap)    diap = reinterpret_cast<PTR(RDORTPIntDiap)>($2);
					PTR(RDORTPDefVal)       dv = reinterpret_cast<PTR(RDORTPDefVal)>($3);
					PTR(RDORTPIntParamType) rp = new RDORTPIntParamType(PARSER->getLastParsingObject(), diap, dv, RDOParserSrcInfo(@1, @3));
					$$ = (int)rp;
				}
				| RDO_real param_real_diap param_real_default_val
				{
					PTR(RDORTPRealDiap)    diap = reinterpret_cast<PTR(RDORTPRealDiap)>($2);
					PTR(RDORTPDefVal)        dv = reinterpret_cast<PTR(RDORTPDefVal)>($3);
					PTR(RDORTPRealParamType) rp = new RDORTPRealParamType(PARSER->getLastParsingObject(), diap, dv, RDOParserSrcInfo(@1, @3));
					$$ = (int)rp;
				}
				| RDO_string param_string_default_val
				{
					PTR(RDORTPDefVal)          dv = reinterpret_cast<PTR(RDORTPDefVal)>($2);
					PTR(RDORTPStringParamType) rp = new RDORTPStringParamType(PARSER->getLastParsingObject(), dv, RDOParserSrcInfo(@1, @2));
					$$ = (int)rp;
				}
				| RDO_bool param_bool_default_val
				{
					PTR(RDORTPDefVal)        dv = reinterpret_cast<PTR(RDORTPDefVal)>($2);
					PTR(RDORTPBoolParamType) rp = new RDORTPBoolParamType(PARSER->getLastParsingObject(), dv, RDOParserSrcInfo(@1, @2));
					$$ = (int)rp;
				}
				| param_enum param_enum_default_val
				{
					LEXER->enumReset();
					PTR(RDOEnumType)  enu = reinterpret_cast<PTR(RDOEnumType)>($1);
					PTR(RDORTPDefVal) dv = reinterpret_cast<PTR(RDORTPDefVal)>($2);
					if (dv->isExist())
					{
						enu->findEnumValueWithThrow(dv->value().src_pos(), dv->value().value().getAsString()); // Если не найдено, то будет сообщение об ошибке, т.е. throw
					}
					PTR(RDORTPEnumParamType) rp = new RDORTPEnumParamType(PARSER->getLastParsingObject(), enu, dv, RDOParserSrcInfo(@1, @2));
					$$ = (int)rp;
				}
				| param_such_as
				{
					CPTR(RDORTPParam) param = reinterpret_cast<PTR(RDORTPParam)>($1);
					RDOParserSrcInfo src_info(@1);
					src_info.setSrcText(_T("such_as ") + (param->getResType() ? param->getResType()->name() + _T(".") : _T("")) + param->name());
					$$ = (int)param->getType()->constructorSuchAs(src_info);
				}
				| param_such_as '=' RDO_INT_CONST
				{
					CPTR(RDORTPParam) param = reinterpret_cast<PTR(RDORTPParam)>($1);
					RDOParserSrcInfo src_info(@1, @3);
					src_info.setSrcText(_T("such_as ") + (param->getResType() ? param->getResType()->name() + _T(".") : _T("")) + param->name());
					$$ = (int)param->getType()->constructorSuchAs(src_info, RDOVALUE($3));
				}
				| param_such_as '=' RDO_REAL_CONST
				{
					CPTR(RDORTPParam) param = reinterpret_cast<PTR(RDORTPParam)>($1);
					RDOParserSrcInfo src_info(@1, @3);
					src_info.setSrcText(_T("such_as ") + (param->getResType() ? param->getResType()->name() + _T(".") : _T("")) + param->name());
					$$ = (int)param->getType()->constructorSuchAs(src_info, RDOVALUE($3));
				}
				| param_such_as '=' RDO_IDENTIF
				{
					CPTR(RDORTPParam) param = reinterpret_cast<PTR(RDORTPParam)>($1);
					RDOParserSrcInfo src_info(@1, @3);
					src_info.setSrcText(_T("such_as ") + (param->getResType() ? param->getResType()->name() + _T(".") : _T("")) + param->name());
					$$ = (int)param->getType()->constructorSuchAs(src_info, RDOVALUE($3));
				}
				| param_such_as '=' error
				{
					PARSER->error().error(rdoParse::RDOParserSrcInfo(), _T("Ожидается зачение по-умолчанию"));
				};
/*
				| RDO_integer error {
					PARSER->error().error(@2, _T("Ошибка после ключевого слова integer. Возможно, не хватает значения по-умолчанию."));
				}
				| RDO_real error {
					PARSER->error().error(@2, _T("Ошибка после ключевого слова real. Возможно, не хватает значения по-умолчанию."));
				}
				| param_enum error {
					PARSER->error().error(@2, _T("Ошибка после перечислимого типа. Возможно, не хватает значения по-умолчанию."));
				};
*/
param_int_diap:	/* empty */ {
					YYLTYPE pos = @0;
					pos.first_line   = pos.last_line;
					pos.first_column = pos.last_column;
					PTR(RDORTPIntDiap) diap = new RDORTPIntDiap(PARSER, pos);
					$$ = (int)diap;
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST ']' {
					PTR(RDORTPIntDiap) diap = new RDORTPIntDiap(PARSER, RDOVALUE($2)->getInt(), RDOVALUE($4)->getInt(), RDOParserSrcInfo(@1, @5), @4);
					$$ = (int)diap;
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST {
					PARSER->error().error(@2, _T("Требуется целочисленный диапазон, указан вещественный"));
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST {
					PARSER->error().error(@2, _T("Требуется целочисленный диапазон, указан вещественный"));
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST {
					PARSER->error().error(@4, _T("Требуется целочисленный диапазон, указан вещественный"));
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST error {
					PARSER->error().error(@4, _T("Диапазон задан неверно"));
				}
				| '[' RDO_INT_CONST RDO_dblpoint error {
					PARSER->error().error(@4, _T("Диапазон задан неверно"));
				}
				| '[' error {
					PARSER->error().error(@2, _T("Диапазон задан неверно"));
				};

param_real_diap:	/* empty */ {
					YYLTYPE pos = @0;
					pos.first_line   = pos.last_line;
					pos.first_column = pos.last_column;
					PTR(RDORTPRealDiap) diap = new RDORTPRealDiap(PARSER, pos);
					$$ = (int)diap;
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST ']' {
					double min = RDOVALUE($2)->getDouble();
					double max = RDOVALUE($4)->getDouble();
					PTR(RDORTPRealDiap) diap = new RDORTPRealDiap(PARSER, min, max, RDOParserSrcInfo(@1, @5), @4);
					$$ = (int)diap;
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST ']' {
					double min = RDOVALUE($2)->getDouble();
					double max = RDOVALUE($4)->getDouble();
					PTR(RDORTPRealDiap) diap = new RDORTPRealDiap(PARSER, min, max, RDOParserSrcInfo(@1, @5), @4);
					$$ = (int)diap;
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST ']' {
					double min = RDOVALUE($2)->getDouble();
					double max = RDOVALUE($4)->getDouble();
					PTR(RDORTPRealDiap) diap = new RDORTPRealDiap(PARSER, min, max, RDOParserSrcInfo(@1, @5), @4);
					$$ = (int)diap;
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST ']' {
					double min = RDOVALUE($2)->getDouble();
					double max = RDOVALUE($4)->getDouble();
					PTR(RDORTPRealDiap) diap = new RDORTPRealDiap(PARSER, min, max, RDOParserSrcInfo(@1, @5), @4);
					$$ = (int)diap;
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST error {
					PARSER->error().error(@4, _T("Диапазон задан неверно"));
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST error {
					PARSER->error().error(@4, _T("Диапазон задан неверно"));
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST error {
					PARSER->error().error(@4, _T("Диапазон задан неверно"));
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST error {
					PARSER->error().error(@4, _T("Диапазон задан неверно"));
				}
				| '[' RDO_REAL_CONST RDO_dblpoint error {
					PARSER->error().error(@4, _T("Диапазон задан неверно"));
				}
				| '[' RDO_INT_CONST RDO_dblpoint error {
					PARSER->error().error(@4, _T("Диапазон задан неверно"));
				}
				| '[' error {
					PARSER->error().error(@2, _T("Диапазон задан неверно"));
				};

param_int_default_val:	/* empty */ {
						$$ = (int)new RDORTPDefVal(PARSER);
					}
					| '=' RDO_INT_CONST {
						$$ = (int)new RDORTPDefVal(PARSER, RDOVALUE($2));
					}
					| '=' RDO_REAL_CONST {
						PARSER->error().error(@2, rdo::format(_T("Целое число инициализируется вещественным: %f"), RDOVALUE($2)->getDouble()));
					}
					| '=' error {
						RDOParserSrcInfo _src_info(@1, @2, true);
						if (_src_info.src_pos().point())
						{
							PARSER->error().error(_src_info, _T("Не указано значение по-умолчанию для целого типа"));
						}
						else
						{
							PARSER->error().error(_src_info, _T("Неверное значение по-умолчанию для целого типа"));
						}
					};

param_real_default_val:	/* empty */ {
						$$ = (int)new RDORTPDefVal(PARSER);
					}
					| '=' RDO_REAL_CONST {
						$$ = (int)new RDORTPDefVal(PARSER, RDOVALUE($2));
					}
					| '=' RDO_INT_CONST {
						$$ = (int)new RDORTPDefVal(PARSER, RDOVALUE($2));
					}
					| '=' error {
						RDOParserSrcInfo _src_info(@1, @2, true);
						if (_src_info.src_pos().point())
						{
							PARSER->error().error(_src_info, _T("Не указано значение по-умолчанию для вещественного типа"));
						}
						else
						{
							PARSER->error().error(_src_info, _T("Неверное значение по-умолчанию для вещественного типа"));
						}
					};

param_string_default_val:	/* empty */
					{
						$$ = (int)new RDORTPDefVal(PARSER);
					}
					| '=' RDO_STRING_CONST
					{
						$$ = (int)new RDORTPDefVal(PARSER, RDOVALUE($2));
					}
					| '=' error
					{
						RDOParserSrcInfo _src_info(@1, @2, true);
						if (_src_info.src_pos().point())
						{
							PARSER->error().error(_src_info, _T("Не указано значение по-умолчанию для строчного типа"));
						}
						else
						{
							PARSER->error().error(_src_info, _T("Неверное значение по-умолчанию для строчного типа"));
						}
					};

param_bool_default_val:	/* empty */
					{
						$$ = (int)new RDORTPDefVal(PARSER);
					}
					| '=' RDO_BOOL_CONST
					{
						$$ = (int)new RDORTPDefVal(PARSER, RDOVALUE($2));
					}
					| '=' error
					{
						RDOParserSrcInfo _src_info(@1, @2, true);
						if (_src_info.src_pos().point())
						{
							PARSER->error().error(_src_info, _T("Не указано значение по-умолчанию для булевского типа"));
						}
						else
						{
							PARSER->error().error(_src_info, _T("Неверное значение по-умолчанию для булевского типа"));
						}
					};

param_enum:	'(' param_enum_list ')' {
				PTR(RDOEnumType) enu = reinterpret_cast<PTR(RDOEnumType)>($2);
				enu->setSrcPos(@1, @3);
				enu->setSrcText(enu->getEnums().asString());
				$$ = $2;
			}
			| '(' param_enum_list error {
				PARSER->error().error(@2, _T("Перечисление должно заканчиваться скобкой"));
			};

param_enum_list: RDO_IDENTIF {
					PTR(RDOEnumType) enu = new RDOEnumType(PARSER->getLastParsingObject(), RDOVALUE($1));
					enu->setSrcInfo(P_RDOVALUE($1)->src_info());
					LEXER->enumBegin();
					$$ = (int)enu;
				}
				| param_enum_list ',' RDO_IDENTIF {
					if (!LEXER->enumEmpty())
					{
						PTR(RDOEnumType) enu  = reinterpret_cast<PTR(RDOEnumType)>($1);
						enu->add(RDOVALUE($3));
						$$ = (int)enu;
					}
					else
					{
						PARSER->error().error(@3, _T("Ошибка в описании значений перечислимого типа"));
					}
				}
				| param_enum_list RDO_IDENTIF {
					if (!LEXER->enumEmpty())
					{
						PTR(RDOEnumType) enu  = reinterpret_cast<PTR(RDOEnumType)>($1);
						enu->add(RDOVALUE($2));
						$$ = (int)enu;
						PARSER->error().warning(@1, rdo::format(_T("Пропущена запятая перед: %s"), RDOVALUE($2)->getIdentificator().c_str()));
					}
					else
					{
						PARSER->error().error(@2, _T("Ошибка в описании значений перечислимого типа"));
					}
				}
				| param_enum_list ',' RDO_INT_CONST {
					PARSER->error().error(@3, _T("Значение перечислимого типа не может быть цифрой"));
				}
				| param_enum_list ',' RDO_REAL_CONST {
					PARSER->error().error(@3, _T("Значение перечислимого типа не может быть цифрой"));
				}
				| param_enum_list RDO_INT_CONST {
					PARSER->error().error(@2, _T("Значение перечислимого типа не может быть цифрой"));
				}
				| param_enum_list RDO_REAL_CONST {
					PARSER->error().error(@2, _T("Значение перечислимого типа не может быть цифрой"));
				}
				| RDO_INT_CONST {
					PARSER->error().error(@1, _T("Значение перечислимого типа не может начинаться с цифры"));
				}
				| RDO_REAL_CONST {
					PARSER->error().error(@1, _T("Значение перечислимого типа не может начинаться с цифры"));
				};

param_enum_default_val:	/* empty */ {
						$$ = (int)new RDORTPDefVal(PARSER);
					}
					| '=' RDO_IDENTIF {
						$$ = (int)new RDORTPDefVal(PARSER, RDOVALUE($2));
					}
					| '=' error {
						RDOParserSrcInfo _src_info(@1, @2, true);
						if (_src_info.src_pos().point())
						{
							PARSER->error().error(_src_info, _T("Не указано значение по-умолчанию для перечислимого типа"));
						}
						else
						{
							PARSER->error().error(_src_info, _T("Неверное значение по-умолчанию для перечислимого типа"));
						}
					};

param_such_as:	RDO_such_as RDO_IDENTIF '.' RDO_IDENTIF {
					tstring type  = RDOVALUE($2)->getIdentificator();
					tstring param = RDOVALUE($4)->getIdentificator();
					CPTR(RDORTPResType) const rt = PARSER->findRTPResType(type);
					if (!rt)
					{
						PARSER->error().error(@2, rdo::format(_T("Ссылка на неизвестный тип ресурса: %s"), type.c_str()));
					}
					CPTR(RDORTPParam) const rp = rt->findRTPParam(param);
					if (!rp)
					{
						PARSER->error().error(@4, rdo::format(_T("Ссылка на неизвестный параметр ресурса: %s.%s"), type.c_str(), param.c_str()));
					}
					$$ = (int)rp;
				}
				| RDO_such_as RDO_IDENTIF {
					tstring constName = RDOVALUE($2)->getIdentificator();
					CPTR(RDOFUNConstant) const cons = PARSER->findFUNConstant(constName);
					if (!cons)
					{
						PARSER->error().error(@2, rdo::format(_T("Ссылка на несуществующую константу: %s"), constName.c_str()));
					}
					$$ = (int)cons->getDescr();
				}
				| RDO_such_as RDO_IDENTIF '.' error {
					tstring type = RDOVALUE($2)->getIdentificator();
					CPTR(RDORTPResType) const rt = PARSER->findRTPResType(type);
					if (!rt)
					{
						PARSER->error().error(@2, rdo::format(_T("Ссылка на неизвестный тип ресурса: %s"), type.c_str()));
					}
					else
					{
						PARSER->error().error(@4, _T("Ошибка при указании параметра"));
					}
				}
				| RDO_such_as error {
					PARSER->error().error(@2, _T("После ключевого слова such_as необходимо указать тип и параметер ресурса для ссылки"));
				};
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// ---------- Логические выражения
// ----------------------------------------------------------------------------
fun_logic_eq: '='    { $1 = RDO_eq; }
			| RDO_eq { $1 = RDO_eq; };

fun_logic:	  fun_arithm  fun_logic_eq  fun_arithm   { $$ = (int)(ARITHM($1) == ARITHM($3));  }
			| fun_arithm  RDO_neq       fun_arithm   { $$ = (int)(ARITHM($1) != ARITHM($3));  }
			| fun_arithm  '<'           fun_arithm   { $$ = (int)(ARITHM($1) <  ARITHM($3));  }
			| fun_arithm  '>'           fun_arithm   { $$ = (int)(ARITHM($1) >  ARITHM($3));  }
			| fun_arithm  RDO_leq       fun_arithm   { $$ = (int)(ARITHM($1) <= ARITHM($3));  }
			| fun_arithm  RDO_geq       fun_arithm   { $$ = (int)(ARITHM($1) >= ARITHM($3));  }
			| fun_logic   RDO_and       fun_logic    { $$ = (int)(LOGIC($1) && LOGIC($3));    }
			| fun_logic   RDO_or        fun_logic    { $$ = (int)(LOGIC($1) || LOGIC($3));    }
			| fun_arithm                             { $$ = (int)new RDOFUNLogic(ARITHM($1)); }
			| fun_group
			| fun_select_logic
			| '[' fun_logic ']'
			{
				PTR(RDOFUNLogic) logic = P_LOGIC($2);
				logic->setSrcPos (@1, @3);
				logic->setSrcText(_T("[") + logic->src_text() + _T("]"));
				$$ = $2;
			}
			| '(' fun_logic ')'
			{
				PTR(RDOFUNLogic) logic = P_LOGIC($2);
				logic->setSrcPos (@1, @3);
				logic->setSrcText(_T("(") + logic->src_text() + _T(")"));
				$$ = $2;
			}
			| RDO_not fun_logic
			{
				PTR(RDOFUNLogic) logic = P_LOGIC($2);
				PTR(RDOFUNLogic) logic_not = logic->operator_not();
				logic_not->setSrcPos (@1, @2);
				logic_not->setSrcText(_T("not ") + logic->src_text());
				$$ = (int)logic_not;
			}
			| '[' fun_logic error {
				PARSER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
			}
			| '(' fun_logic error {
				PARSER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
			};

// ----------------------------------------------------------------------------
// ---------- Арифметические выражения
// ----------------------------------------------------------------------------
fun_arithm:	  RDO_INT_CONST                      { $$ = (int)new RDOFUNArithm(PARSER, RDOVALUE($1)); }
			| RDO_REAL_CONST                     { $$ = (int)new RDOFUNArithm(PARSER, RDOVALUE($1)); }
			| RDO_BOOL_CONST                     { $$ = (int)new RDOFUNArithm(PARSER, RDOVALUE($1)); }
			| RDO_STRING_CONST                   { $$ = (int)new RDOFUNArithm(PARSER, RDOVALUE($1)); }
			| RDO_IDENTIF                        { $$ = (int)new RDOFUNArithm(PARSER, RDOVALUE($1)); }
			| RDO_IDENTIF '.' RDO_IDENTIF        { $$ = (int)new RDOFUNArithm(PARSER, RDOVALUE($1), RDOVALUE($3)); }
			| RDO_IDENTIF_RELRES '.' RDO_IDENTIF { $$ = (int)new RDOFUNArithm(PARSER, RDOVALUE($1), RDOVALUE($3)); }
			| fun_arithm '+' fun_arithm		     { $$ = (int)(ARITHM($1) + ARITHM($3)); }
			| fun_arithm '-' fun_arithm		     { $$ = (int)(ARITHM($1) - ARITHM($3)); }
			| fun_arithm '*' fun_arithm		     { $$ = (int)(ARITHM($1) * ARITHM($3)); }
			| fun_arithm '/' fun_arithm		     { $$ = (int)(ARITHM($1) / ARITHM($3)); }
			| fun_arithm_func_call
			| fun_select_arithm
			| '(' fun_arithm ')'
			{
				PTR(RDOFUNArithm) arithm = P_ARITHM($2);
				arithm->setSrcPos (@1, @3);
				arithm->setSrcText(_T("(") + arithm->src_text() + _T(")"));
				$$ = $2;
			}
			| '-' fun_arithm %prec RDO_UMINUS
			{
				RDOParserSrcInfo info;
				info.setSrcPos (@1, @2);
				info.setSrcText(_T("-") + ARITHM($2).src_text());
				$$ = (int)new RDOFUNArithm(PARSER, RDOValue(ARITHM($2).type(), info), new rdoRuntime::RDOCalcUMinus(RUNTIME, ARITHM($2).createCalc()));
			};

// ----------------------------------------------------------------------------
// ---------- Функции и последовательности
// ----------------------------------------------------------------------------
fun_arithm_func_call:	RDO_IDENTIF '(' ')' {
							PTR(RDOFUNParams) fun = new RDOFUNParams(PARSER);
							tstring fun_name = RDOVALUE($1)->getIdentificator();
							fun->funseq_name.setSrcInfo(RDOParserSrcInfo(@1, fun_name));
							fun->setSrcPos (@1, @3);
							fun->setSrcText(fun_name + _T("()"));
							PTR(RDOFUNArithm) arithm = fun->createCall(fun_name);
							$$ = (int)arithm;
						}
						| RDO_IDENTIF '(' fun_arithm_func_call_pars ')' {
							PTR(RDOFUNParams) fun      = reinterpret_cast<PTR(RDOFUNParams)>($3);
							tstring           fun_name = RDOVALUE($1)->getIdentificator();
							fun->funseq_name.setSrcInfo(RDOParserSrcInfo(@1, fun_name));
							fun->setSrcPos (@1, @4);
							fun->setSrcText(fun_name + _T("(") + fun->src_text() + _T(")"));
							PTR(RDOFUNArithm) arithm = fun->createCall(fun_name);
							$$ = (int)arithm;
						}
						| RDO_IDENTIF '(' error {
							PARSER->error().error(@3, _T("Ошибка в параметрах функции"));
						};

fun_arithm_func_call_pars:	fun_arithm {
								PTR(RDOFUNParams) fun    = new RDOFUNParams(PARSER);
								PTR(RDOFUNArithm) arithm = P_ARITHM($1);
								fun->setSrcText  (arithm->src_text());
								fun->addParameter(arithm);
								$$ = (int)fun;
							}
							| fun_arithm_func_call_pars ',' fun_arithm {
								PTR(RDOFUNParams) fun    = reinterpret_cast<PTR(RDOFUNParams)>($1);
								PTR(RDOFUNArithm) arithm = P_ARITHM($3);
								fun->setSrcText  (fun->src_text() + _T(", ") + arithm->src_text());
								fun->addParameter(arithm);
								$$ = (int)fun;
							}
							| fun_arithm_func_call_pars error {
								PARSER->error().error(@2, _T("Ошибка в арифметическом выражении"));
							}
							| fun_arithm_func_call_pars ',' error {
								PARSER->error().error(@3, _T("Ошибка в арифметическом выражении"));
							};

// ----------------------------------------------------------------------------
// ---------- Групповые выражения
// ----------------------------------------------------------------------------
fun_group_keyword:	RDO_Exist			{ $$ = RDOFUNGroupLogic::fgt_exist;     }
					| RDO_Not_Exist		{ $$ = RDOFUNGroupLogic::fgt_notexist;  }
					| RDO_For_All		{ $$ = RDOFUNGroupLogic::fgt_forall;    }
					| RDO_Not_For_All	{ $$ = RDOFUNGroupLogic::fgt_notforall; };

fun_group_header:	fun_group_keyword '(' RDO_IDENTIF_COLON {
						PTR(RDOValue) type_name = P_RDOVALUE($3);
						$$ = (int)(new RDOFUNGroupLogic(PARSER, (RDOFUNGroupLogic::FunGroupType)$1, type_name->src_info()));
					}
					| fun_group_keyword '(' error {
						PARSER->error().error(@3, _T("Ожидается имя типа"));
					}
					| fun_group_keyword error {
						PARSER->error().error(@1, _T("После имени функции ожидается октрывающаяся скобка"));
					};

fun_group:			fun_group_header fun_logic ')' {
						PTR(RDOFUNGroupLogic) groupfun = reinterpret_cast<PTR(RDOFUNGroupLogic)>($1);
						groupfun->setSrcPos(@1, @3);
						$$ = (int)groupfun->createFunLogic(P_LOGIC($2));
					}
					| fun_group_header RDO_NoCheck ')' {
						PTR(RDOFUNGroupLogic) groupfun = reinterpret_cast<PTR(RDOFUNGroupLogic)>($1);
						groupfun->setSrcPos(@1, @3);
						PTR(RDOFUNLogic) trueLogic = new RDOFUNLogic(groupfun, new rdoRuntime::RDOCalcConst(RUNTIME, 1));
						trueLogic->setSrcPos (@2);
						trueLogic->setSrcText(_T("NoCheck"));
						$$ = (int)groupfun->createFunLogic(trueLogic);
					}
					| fun_group_header fun_logic error {
						PARSER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
					}
					| fun_group_header RDO_NoCheck error {
						PARSER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
					}
					| fun_group_header error {
						PARSER->error().error(@1, @2, _T("Ошибка в логическом выражении"));
					};

// ----------------------------------------------------------------------------
// ---------- Select
// ----------------------------------------------------------------------------
fun_select_header:	RDO_Select '(' RDO_IDENTIF_COLON {
						PTR(RDOValue) type_name  = P_RDOVALUE($3);
						PTR(RDOFUNSelect) select = new RDOFUNSelect(PARSER, type_name->src_info());
						select->setSrcText(_T("Select(") + type_name->value().getIdentificator() + _T(": "));
						$$ = (int)select;
					}
					| RDO_Select '(' error {
						PARSER->error().error(@3, _T("Ожидается имя типа"));
					}
					| RDO_Select error {
						PARSER->error().error(@1, _T("Ожидается октрывающаяся скобка"));
					};

fun_select_body:	fun_select_header fun_logic ')' {
						PTR(RDOFUNSelect) select = reinterpret_cast<PTR(RDOFUNSelect)>($1);
						PTR(RDOFUNLogic)  flogic = P_LOGIC($2);
						select->setSrcText(select->src_text() + flogic->src_text() + _T(")"));
						select->initSelect(flogic);
					}
					| fun_select_header RDO_NoCheck ')' {
						PTR(RDOFUNSelect) select = reinterpret_cast<PTR(RDOFUNSelect)>($1);
						RDOParserSrcInfo logic_info(@2, _T("NoCheck"));
						select->setSrcText(select->src_text() + logic_info.src_text() + _T(")"));
						PTR(rdoRuntime::RDOCalcConst) calc_nocheck = new rdoRuntime::RDOCalcConst(RUNTIME, 1);
						PTR(RDOFUNLogic)              flogic       = new RDOFUNLogic(select, calc_nocheck, true);
						flogic->setSrcInfo(logic_info);
						select->initSelect(flogic);
					}
					| fun_select_header fun_logic error {
						PARSER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
					}
					| fun_select_header RDO_NoCheck error {
						PARSER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
					}
					| fun_select_header error {
						PARSER->error().error(@1, @2, _T("Ошибка в логическом выражении"));
					};

fun_select_keyword:	RDO_Exist			{ $$ = RDOFUNGroupLogic::fgt_exist;     }
					| RDO_Not_Exist		{ $$ = RDOFUNGroupLogic::fgt_notexist;  }
					| RDO_For_All		{ $$ = RDOFUNGroupLogic::fgt_forall;    }
					| RDO_Not_For_All	{ $$ = RDOFUNGroupLogic::fgt_notforall; };

fun_select_logic:	fun_select_body '.' fun_select_keyword '(' fun_logic ')' {
						PTR(RDOFUNSelect) select = reinterpret_cast<PTR(RDOFUNSelect)>($1);
						select->setSrcPos(@1, @6);
						PTR(RDOFUNLogic) logic = select->createFunSelectGroup((RDOFUNGroupLogic::FunGroupType)$3, P_LOGIC($5));
						$$ = (int)logic;
					}
					| fun_select_body '.' fun_select_keyword '(' error {
						PARSER->error().error(@4, @5, _T("Ошибка в логическом выражении"));
					}
					| fun_select_body '.' fun_select_keyword error {
						PARSER->error().error(@3, _T("Ожидается октрывающаяся скобка"));
					}
					| fun_select_body '.' RDO_Empty '(' ')' {
						PTR(RDOFUNSelect) select = reinterpret_cast<PTR(RDOFUNSelect)>($1);
						select->setSrcPos(@1, @5);
						RDOParserSrcInfo empty_info(@3, @5, _T("Empty()"));
						PTR(RDOFUNLogic) logic = select->createFunSelectEmpty(empty_info);
						$$ = (int)logic;
					}
					| fun_select_body '.' RDO_Empty '(' error {
						PARSER->error().error(@4, _T("Ожидается закрывающаяся скобка"));
					}
					| fun_select_body '.' RDO_Empty error {
						PARSER->error().error(@3, _T("Ожидается октрывающаяся скобка"));
					}
					| fun_select_body '.' error {
						PARSER->error().error(@2, @3, _T("Ожидается метод списка ресурсов"));
					}
					| fun_select_body error {
						PARSER->error().error(@1, _T("Ожидается '.' (точка) для вызова метода списка ресурсов"));
					};

fun_select_arithm:	fun_select_body '.' RDO_Size '(' ')' {
						PTR(RDOFUNSelect) select = reinterpret_cast<PTR(RDOFUNSelect)>($1);
						select->setSrcPos(@1, @5);
						RDOParserSrcInfo size_info(@3, @5, _T("Size()"));
						PTR(RDOFUNArithm) arithm = select->createFunSelectSize(size_info);
						$$ = (int)arithm;
					}
					| fun_select_body '.' RDO_Size error {
						PARSER->error().error(@3, _T("Ожидается октрывающаяся скобка"));
					}
					| fun_select_body '.' RDO_Size '(' error {
						PARSER->error().error(@4, _T("Ожидается закрывающаяся скобка"));
					};

%%

CLOSE_RDO_PARSER_NAMESPACE

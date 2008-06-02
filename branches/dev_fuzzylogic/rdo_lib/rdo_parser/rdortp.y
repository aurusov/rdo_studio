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
%token RDO_IDENTIF_set					319
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

%token RDO_QUOTED_IDENTIF				430
%token RDO_QUOTED_IDENTIF_BAD			431
%token RDO_IDENTIF_BAD					432
%token RDO_Select						433
%token RDO_Size							434
%token RDO_Empty						435
%token RDO_not							436
%token RDO_UMINUS						437
%token RDO_string						438

%token RDO_Fuzzy_Parameters				439
%token RDO_Fuzzy_Term					440

%{
#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoparser.h"
#include "rdoparser_lexer.h"
#include "rdortp.h"
#include "rdofun.h"

#define PARSER reinterpret_cast<rdoParse::RDOLexer*>(lexer)->m_parser

namespace rdoParse
{
%}

%start rtp_list

%%

rtp_list:	/* empty */
			| rtp_list rtp_res_type
			| error {
//				PARSER->error( @1, rdoSimulator::RDOSyntaxError::UNKNOWN );
				PARSER->error( "Ожидается ключевое слово $Resource_type" );
			};

rtp_res_type:	rtp_res_type_hdr RDO_Parameters rtp_body RDO_Fuzzy_Parameters rtp_fuzzy_body RDO_End {
					RDORTPResType* res_type = reinterpret_cast<RDORTPResType*>($1);
					if ( $3 == 0 ) {
						PARSER->warning( @2, rdo::format( "Тип ресурса '%s' не содежит параметров", res_type->name().c_str() ) );
					}
					if ( $5 == 0 ) {
						PARSER->warning( @4, rdo::format( "Тип ресурса '%s' не содежит нечетких параметров", res_type->name().c_str() ) );
					}
					//Случай четких и нечетких параметров ресурса
				}
				| rtp_res_type_hdr RDO_Parameters rtp_body RDO_End {
					RDORTPResType* res_type = reinterpret_cast<RDORTPResType*>($1);
					if ( $3 == 0 ) {
						PARSER->warning( @2, rdo::format( "Тип ресурса '%s' не содежит параметров", res_type->name().c_str() ) );
					}
				}
				| rtp_res_type_hdr RDO_Fuzzy_Parameters rtp_fuzzy_body RDO_End {
					RDORTPResType* res_type = reinterpret_cast<RDORTPResType*>($1);
					if ( $3 == 0 ) {
						PARSER->warning( @2, rdo::format( "Тип ресурса '%s' не содежит нечетких параметров", res_type->name().c_str() ) );
					}
					//Случай только нечетких параметров ресурса
				}
				| rtp_res_type_hdr RDO_Parameters rtp_body RDO_Fuzzy_Parameters rtp_fuzzy_body {
					PARSER->error( @5, "Не найдено ключевое слово $End" );
				}
				| rtp_res_type_hdr RDO_Parameters rtp_body {
					PARSER->error( @3, "Не найдено ключевое слово $End" );
				}
				| rtp_res_type_hdr error {
					PARSER->error( @2, "Не найдено ключевое слово $Parameters" );
				};

rtp_res_type_hdr:	RDO_Resource_type RDO_IDENTIF_COLON rtp_vid_res {
						reinterpret_cast<RDOLexer*>(lexer)->m_enum_param_cnt = 0;
						std::string name = *reinterpret_cast<std::string*>($2);
						RDOParserSrcInfo src_info(@2, name, RDOParserSrcInfo::psi_align_bytext);
						const RDORTPResType* _rtp = PARSER->findRTPResType( name );
						if ( _rtp ) {
							PARSER->error_push_only( src_info, rdoSimulator::RDOSyntaxError::RTP_SECOND_RES_TYPE, name.c_str() );
							PARSER->error_push_only( _rtp->src_info(), "См. первое определение" );
							PARSER->error_push_done();
						}
						RDORTPResType* rtp = new RDORTPResType( PARSER, src_info, $3 != 0 );
						$$ = (int)rtp;
					}
					| RDO_Resource_type error {
						std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
						PARSER->error( @2, rdo::format("Ошибка в описании имени типа ресурса: %s", str.c_str()) );
					}
					| RDO_Resource_type RDO_IDENTIF_COLON error {
						PARSER->error( @2, "Не указан вид ресурса" );
					};

rtp_vid_res:	RDO_permanent	{ $$ = 1; }
				| RDO_temporary	{ $$ = 0; };

rtp_body:	/* empty */ {
				$$ = 0; // warning
			}
			| rtp_body rtp_param {
				RDORTPParam* param = reinterpret_cast<RDORTPParam*>($2);
				PARSER->getLastRTPResType()->addParam( param );
				$$ = 1; // no warning
			};
//-------------------------------------- FOR FUZZY LOGIC --------------------------------------	
rtp_fuzzy_body:	/* empty */ {
				$$ = 0; // warning
			}
			| rtp_fuzzy_body rtp_fuzzy_param {
				RDORTPFuzzyParam* fuzzy_param = reinterpret_cast<RDORTPFuzzyParam*>($2);
				PARSER->getLastRTPResType()->addFuzzyParam( fuzzy_param );
				$$ = 1; // no warning
			};
rtp_fuzzy_param: RDO_IDENTIF_COLON fuzzy_param_type fuzzy_param_terms {	
					RDOParserSrcInfo par_src_info(@1, *reinterpret_cast<std::string*>($1), RDOParserSrcInfo::psi_align_bytext);
					RDORTPFuzzyParamType* fuzzy_parType = reinterpret_cast<RDORTPFuzzyParamType*>($2);
					RDORTPFuzzyParam* fuzzy_param = new RDORTPFuzzyParam( PARSER->getLastRTPResType(), par_src_info, fuzzy_parType );
					fuzzy_parType->reparent( fuzzy_param );
					$$ = (int)fuzzy_param;		
				}
				| RDO_IDENTIF_COLON error {
					if ( PARSER->lexer_loc_line() == @1.last_line ) {
						std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
						PARSER->error( @2, rdo::format( "Неверный тип параметра: %s", str.c_str() ) );
					} else {
						PARSER->error( @1, "Ожидается тип параметра" );
					}					
				}
				| error {
					PARSER->error( @1, "Неверное описание нечеткого параметра" );
				};
fuzzy_param_type: RDO_real param_real_diap param_real_default_val {
					RDORTPRealDiap* diap = reinterpret_cast<RDORTPRealDiap*>($2);
					RDORTPRealDefVal* dv = reinterpret_cast<RDORTPRealDefVal*>($3);
					RDORTPRealParamType* rp = new RDORTPRealParamType( PARSER->getLastParsingObject(), diap, dv, RDOParserSrcInfo( @1, @3 ) );
					$$ = (int)rp;
					//Случай вещественного четкого значения
				};
				
fuzzy_param_terms: /* empty */ {
					$$ = 0; // warning
				}
				| fuzzy_param_terms fuzzy_term {
					RDORTPFuzzyTerm* fuzzy_term = reinterpret_cast<RDORTPFuzzyTerm*>($2);
					PARSER->getLastRTPFuzzyParam()->addTerm( fuzzy_term );
					$$ = 1; // no warning				
				};
				
fuzzy_term: RDO_Fuzzy_Term RDO_IDENTIF fuzzy_membershift_fun {
					RDOParserSrcInfo par_src_info(@2, *reinterpret_cast<std::string*>($2), RDOParserSrcInfo::psi_align_bytext);
					RDORTPFuzzyMembershiftFun* fuzzy_membershift_fun = reinterpret_cast<RDORTPFuzzyMembershiftFun*>($3);
					RDORTPFuzzyTerm* fuzzy_term = new RDORTPFuzzyTerm( PARSER->getLastRTPFuzzyParam(), par_src_info, fuzzy_membershift_fun );
					fuzzy_membershift_fun->reparent( fuzzy_term );
					$$ = (int)fuzzy_term;				
				};
								
fuzzy_membershift_fun: /* empty */ {
					$$ = 0; // warning
				}
				| fuzzy_membershift_fun membershift_point {					
					RDORTPFuzzyMembershiftPoint* fuzzy_membershift_point = reinterpret_cast<RDORTPFuzzyMembershiftPoint*>($2);
					PARSER->getLastRTPFuzzyMembershiftFun()->addMembershiftPoint( fuzzy_membershift_point );
					$$ = 1; // no warning
					//Задание функции принадлежности точками - вершинами ломанных кривых
				};
membershift_point: '(' RDO_REAL_CONST ',' RDO_REAL_CONST ')' {					
					double x_value = *reinterpret_cast<double*>($2);
					double y_value = *reinterpret_cast<double*>($4);
					RDORTPFuzzyMembershiftPoint* fuzzy_membershift_point = new RDORTPFuzzyMembershiftPoint( PARSER, x_value, y_value );
					$$ = (int)fuzzy_membershift_point;
				}
				| '(' RDO_REAL_CONST ',' RDO_REAL_CONST ')' ',' {					
					double x_value = *reinterpret_cast<double*>($2);
					double y_value = *reinterpret_cast<double*>($4);
					RDORTPFuzzyMembershiftPoint* fuzzy_membershift_point = new RDORTPFuzzyMembershiftPoint( PARSER, x_value, y_value );
					$$ = (int)fuzzy_membershift_point;
				};							
//---------------------------------------------------------------------------------------------		
	
rtp_param: RDO_IDENTIF_COLON param_type {
					RDOParserSrcInfo par_src_info(@1, *reinterpret_cast<std::string*>($1), RDOParserSrcInfo::psi_align_bytext);
					RDORTPParamType* parType = reinterpret_cast<RDORTPParamType*>($2);
					RDORTPParam* param = new RDORTPParam( PARSER->getLastRTPResType(), par_src_info, parType );
					parType->reparent( param );
					if ( parType->getType() == rdoRuntime::RDOValue::rvt_enum ) {
						static_cast<RDORTPEnumParamType*>(parType)->enum_name = rdo::format( "%s.%s", PARSER->getLastRTPResType()->name().c_str(), par_src_info.src_text().c_str() );
					}
					$$ = (int)param;
				}
				| RDO_IDENTIF_COLON error {
					if ( PARSER->lexer_loc_line() == @1.last_line ) {
						std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
						PARSER->error( @2, rdo::format( "Неверный тип параметра: %s", str.c_str() ) );
					} else {
						PARSER->error( @1, "Ожидается тип параметра" );
					}
				}
				| error {
					PARSER->error( @1, "Неправильное описание параметра" );
				};

// ----------------------------------------------------------------------------
// ---------- Описание типа параметра
// ----------------------------------------------------------------------------
param_type:		RDO_integer param_int_diap param_int_default_val {
					RDORTPIntDiap* diap = reinterpret_cast<RDORTPIntDiap*>($2);
					RDORTPIntDefVal* dv = reinterpret_cast<RDORTPIntDefVal*>($3);
					RDORTPIntParamType* rp = new RDORTPIntParamType( PARSER->getLastParsingObject(), diap, dv, RDOParserSrcInfo( @1, @3 ) );
					$$ = (int)rp;
				}
				| RDO_real param_real_diap param_real_default_val {
					RDORTPRealDiap* diap = reinterpret_cast<RDORTPRealDiap*>($2);
					RDORTPRealDefVal* dv = reinterpret_cast<RDORTPRealDefVal*>($3);
					RDORTPRealParamType* rp = new RDORTPRealParamType( PARSER->getLastParsingObject(), diap, dv, RDOParserSrcInfo( @1, @3 ) );
					$$ = (int)rp;
				}
				| param_enum param_enum_default_val {
					reinterpret_cast<RDOLexer*>(lexer)->m_enum_param_cnt = 0;
					RDORTPEnum* enu      = reinterpret_cast<RDORTPEnum*>($1);
					RDORTPEnumDefVal* dv = reinterpret_cast<RDORTPEnumDefVal*>($2);
					if ( dv->isExist() ) {
						enu->findEnumValueWithThrow( dv->src_pos(), dv->getEnumValue() ); // Если не найдено, то будет сообщение об ошибке, т.е. throw
					}
					RDORTPEnumParamType* rp = new RDORTPEnumParamType( PARSER->getLastParsingObject(), enu, dv, RDOParserSrcInfo( @1, @2 ) );
					$$ = (int)rp;
				}
				| RDO_string param_string_default_val {
					RDORTPStringDefVal* dv = reinterpret_cast<RDORTPStringDefVal*>($2);
					RDORTPStringParamType* rp = new RDORTPStringParamType( PARSER->getLastParsingObject(), dv, RDOParserSrcInfo( @1, @2 ) );
					$$ = (int)rp;
				}
				| param_such_as {
					RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->name() + "." : "") + param->name() );
					$$ = (int)param->getType()->constructSuchAs( src_info );
				}
				| param_such_as '=' RDO_INT_CONST {
					RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1, @3 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->name() + "." : "") + param->name() );
					$$ = (int)param->getType()->constructSuchAs( (int)$3, src_info, RDOParserSrcInfo( @3 ) );
				}
				| param_such_as '=' RDO_REAL_CONST {
					RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1, @3 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->name() + "." : "") + param->name() );
					$$ = (int)param->getType()->constructSuchAs( *(double*)$3, src_info, RDOParserSrcInfo( @3 ) );
				}
				| param_such_as '=' RDO_IDENTIF {
					RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1, @3 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->name() + "." : "") + param->name() );
					$$ = (int)param->getType()->constructSuchAs( *(std::string*)$3, src_info, RDOParserSrcInfo( @3 ) );
				}
				| param_such_as '=' error {
					PARSER->error( "Ожидается зачение по-умолчанию" );
				}
				| param_such_as error {
					PARSER->error( "Ожидается окончание описания параметра-ссылки, например, зачение по-умолчанию" );
				};
/*
				| integer error {
					PARSER->error( @2, "Ошибка после ключевого слова integer. Возможно, не хватает значения по-умолчанию." );
//					PARSER->error( rdoSimulator::RDOSyntaxError::RTP_WAITING_FOR_INT_PARAM_END );
				}
				| real error {
					PARSER->error( @2, "Ошибка после ключевого слова real. Возможно, не хватает значения по-умолчанию." );
//					PARSER->error( rdoSimulator::RDOSyntaxError::RTP_WAITING_FOR_REAL_PARAM_END );
				}
				| param_enum error {
					PARSER->error( @2, "Ошибка после перечислимого типа. Возможно, не хватает значения по-умолчанию." );
//					PARSER->error( rdoSimulator::RDOSyntaxError::RTP_WAITING_FOR_ENUM_PARAM_END );
				};
*/
param_int_diap:	/* empty */ {
					YYLTYPE pos = @0;
					pos.first_line   = pos.last_line;
					pos.first_column = pos.last_column;
					RDORTPIntDiap* diap = new RDORTPIntDiap( PARSER, pos );
					$$ = (int)diap;
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST ']' {
					RDORTPIntDiap* diap = new RDORTPIntDiap( PARSER, $2, $4, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST {
					PARSER->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST {
					PARSER->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST {
					PARSER->error( @4, rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST error {
					PARSER->error( @4, "Диапазон задан неверно" );
				}
				| '[' RDO_INT_CONST RDO_dblpoint error {
					PARSER->error( @4, "Диапазон задан неверно" );
				}
				| '[' error {
					PARSER->error( @2, "Диапазон задан неверно" );
//					PARSER->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_RANGE );
				};

param_real_diap:	/* empty */ {
					YYLTYPE pos = @0;
					pos.first_line   = pos.last_line;
					pos.first_column = pos.last_column;
					RDORTPRealDiap* diap = new RDORTPRealDiap( PARSER, pos );
					$$ = (int)diap;
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST ']' {
					double min = *reinterpret_cast<double*>($2);
					double max = *reinterpret_cast<double*>($4);
					RDORTPRealDiap* diap = new RDORTPRealDiap( PARSER, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST ']' {
					double min = *reinterpret_cast<double*>($2);
					double max = $4;
					RDORTPRealDiap* diap = new RDORTPRealDiap( PARSER, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST ']' {
					double min = $2;
					double max = *reinterpret_cast<double*>($4);
					RDORTPRealDiap* diap = new RDORTPRealDiap( PARSER, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST ']' {
					double min = $2;
					double max = $4;
					RDORTPRealDiap* diap = new RDORTPRealDiap( PARSER, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST error {
					PARSER->error( @4, "Диапазон задан неверно" );
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST error {
					PARSER->error( @4, "Диапазон задан неверно" );
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST error {
					PARSER->error( @4, "Диапазон задан неверно" );
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST error {
					PARSER->error( @4, "Диапазон задан неверно" );
				}
				| '[' RDO_REAL_CONST RDO_dblpoint error {
					PARSER->error( @4, "Диапазон задан неверно" );
				}
				| '[' RDO_INT_CONST RDO_dblpoint error {
					PARSER->error( @4, "Диапазон задан неверно" );
				}
				| '[' error {
					PARSER->error( @2, "Диапазон задан неверно" );
//					PARSER->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_RANGE );
				};

param_int_default_val:	/* empty */ {
						YYLTYPE pos = @0;
						pos.first_line   = pos.last_line;
						pos.first_column = pos.last_column;
						$$ = (int)(new RDORTPIntDefVal(PARSER, pos));
					}
					| '=' RDO_INT_CONST {
						$$ = (int)(new RDORTPIntDefVal(PARSER, $2, RDOParserSrcInfo( @1, @2 )));
					}
					| '=' RDO_REAL_CONST {
						// Целое число инициализируется вещественным: %f
						PARSER->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_DEFVAULT_INT_AS_REAL, *(double*)$2 );
					}
					| '=' {
						PARSER->error( @1, "Не указано значение по-умолчанию для целого типа" );
					}
					| '=' error {
						PARSER->error( @2, "Неверное значение по-умолчанию для целого типа" );
					};

param_real_default_val:	/* empty */ {
						YYLTYPE pos = @0;
						pos.first_line   = pos.last_line;
						pos.first_column = pos.last_column;
						$$ = (int)(new RDORTPRealDefVal(PARSER, pos));
					}
					| '=' RDO_REAL_CONST {
						$$ = (int)(new RDORTPRealDefVal(PARSER, *((double *)$2), RDOParserSrcInfo( @1, @2 )));
					}
					| '=' RDO_INT_CONST {
						$$ = (int)(new RDORTPRealDefVal(PARSER, $2, RDOParserSrcInfo( @1, @2 )));
					}
					| '=' {
						PARSER->error( @1, "Не указано значение по-умолчанию для вещественного типа" );
					}
					| '=' error {
						PARSER->error( @2, "Неверное значение по-умолчанию для вещественного типа" );
					};

param_enum:	'(' param_enum_list ')' {
				RDORTPEnum* enu = reinterpret_cast<RDORTPEnum*>($2);
				enu->setSrcPos( @1, @3 );
				enu->setSrcText( enu->src_text() + ")" );
				$$ = $2;
			}
			| '(' param_enum_list {
				PARSER->error( @2, "Перечисление должно заканчиваться скобкой" );
			};

param_enum_list: RDO_IDENTIF {
					RDORTPEnum* enu = new RDORTPEnum( PARSER->getLastParsingObject(), *(std::string *)$1 );
					std::string* first = reinterpret_cast<std::string*>($1);
					enu->setSrcText( "(" + *first );
					reinterpret_cast<RDOLexer*>(lexer)->m_enum_param_cnt = 1;
					$$ = (int)enu;
				}
				| param_enum_list ',' RDO_IDENTIF {
					if ( reinterpret_cast<RDOLexer*>(lexer)->m_enum_param_cnt >= 1 ) {
						RDORTPEnum* enu  = reinterpret_cast<RDORTPEnum*>($1);
						std::string next = *reinterpret_cast<std::string*>($3);
						enu->add( RDOParserSrcInfo(@3, next) );
						enu->setSrcText( enu->src_text() + ", " + next );
						$$ = (int)enu;
					} else {
						PARSER->error( "Ошибка в описании значений перечислимого типа" );
					}
				}
				| param_enum_list RDO_IDENTIF {
					if ( reinterpret_cast<RDOLexer*>(lexer)->m_enum_param_cnt >= 1 ) {
						PARSER->error( rdo::format("Пропущена запятая перед: %s", ((std::string*)$2)->c_str()) );
					} else {
						PARSER->error( "Ошибка в описании значений перечислимого типа" );
					}
				}
				| param_enum_list error {
					std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
					if ( str.empty() ) {
						PARSER->error( @1, "Ошибка в описании значений перечислимого типа" );
					} else {
						PARSER->error( @2, rdo::format( "Неверное значение перечислимого типа: %s", str.c_str() ) );
					}
				}
				| param_enum_list ',' RDO_INT_CONST {
					PARSER->error( @3, "Значение перечислимого типа не может начинаться с цифры" );
				}
				| param_enum_list ',' RDO_REAL_CONST {
					PARSER->error( @3, "Значение перечислимого типа не может начинаться с цифры" );
				}
				| RDO_INT_CONST {
					PARSER->error( @1, "Значение перечислимого типа не может начинаться с цифры" );
				}
				| RDO_REAL_CONST {
					PARSER->error( @1, "Значение перечислимого типа не может начинаться с цифры" );
				}
				| error {
					PARSER->error( @1, "Ошибка в описании значений перечислимого типа" );
				};

param_enum_default_val:	/* empty */ {
						YYLTYPE pos = @0;
						pos.first_line   = pos.last_line;
						pos.first_column = pos.last_column;
						$$ = (int)(new RDORTPEnumDefVal(PARSER, pos));
					}
					| '=' RDO_IDENTIF {
						$$ = (int)(new RDORTPEnumDefVal( PARSER, *(std::string*)$2, RDOParserSrcInfo( @1, @2 ) ));
					}
					| '=' {
						PARSER->error( @1, "Не указано значение по-умолчанию для перечислимого типа" );
					}
					| '=' error {
						PARSER->error( @2, "Неверное значение по-умолчанию для перечислимого типа" );
					};

param_string_default_val:	/* empty */ {
						YYLTYPE pos = @0;
						pos.first_line   = pos.last_line;
						pos.first_column = pos.last_column;
						$$ = (int)(new RDORTPStringDefVal(PARSER, pos));
					}
					| '=' RDO_QUOTED_IDENTIF {
						$$ = (int)(new RDORTPStringDefVal( PARSER, *(std::string*)$2, RDOParserSrcInfo( @1, @2 ) ));
					}
					| '=' {
						PARSER->error( @1, "Не указано значение по-умолчанию для строчного типа" );
					}
					| '=' error {
						PARSER->error( @2, "Неверное значение по-умолчанию для строчного типа" );
					};

param_such_as:	RDO_such_as RDO_IDENTIF '.' RDO_IDENTIF {
					std::string type  = *reinterpret_cast<std::string*>($2);
					std::string param = *reinterpret_cast<std::string*>($4);
					const RDORTPResType* const rt = PARSER->findRTPResType( type );
					if ( !rt ) {
						PARSER->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					}
					const RDORTPParam* const rp = rt->findRTPParam( param );
					if ( !rp ) {
						PARSER->error( @4, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_PARAM, type.c_str(), param.c_str() );
					}
					$$ = (int)rp;
				}
				| RDO_such_as RDO_IDENTIF {
					std::string constName = *reinterpret_cast<std::string*>($2);
					const RDOFUNConstant* const cons = PARSER->findFUNConstant( constName );
					if ( !cons ) {
						PARSER->error( @2, rdo::format("Ссылка на несуществующую константу: %s", constName.c_str()) );
					}
					$$ = (int)(cons->getDescr());
				}
				| RDO_such_as RDO_IDENTIF '.' {
					std::string type = *reinterpret_cast<std::string*>($2);
					const RDORTPResType* const rt = PARSER->findRTPResType( type );
					if ( !rt ) {
						PARSER->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					} else {
						PARSER->error( @3, "Не указан параметер" );
					}
				}
				| RDO_such_as RDO_IDENTIF '.' error {
					std::string type = *reinterpret_cast<std::string*>($2);
					const RDORTPResType* const rt = PARSER->findRTPResType( type );
					if ( !rt ) {
						PARSER->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					} else {
						PARSER->error( @4, "Ошибка при указании параметра" );
					}
				}
				| RDO_such_as RDO_IDENTIF error {
					std::string type = *reinterpret_cast<std::string*>($2);
					const RDORTPResType* const rt = PARSER->findRTPResType( type );
					if ( !rt ) {
						PARSER->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					} else {
						PARSER->error( @2, "После имени типа должен быть указан параметер через точку" );
					}
				}
				| RDO_such_as error {
					PARSER->error( @2, "После ключевого слова such_as необходимо указать тип и параметер ресурса для ссылки" );
				};
// ----------------------------------------------------------------------------

%%

}

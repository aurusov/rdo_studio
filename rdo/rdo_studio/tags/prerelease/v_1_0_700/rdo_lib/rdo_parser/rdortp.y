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
#include "pch.h"

#include "rdoparser.h"
#include "rdoparser_lexer.h"
#include "rdortp.h"
#include "rdofun.h"

#define LEXER   reinterpret_cast<rdoParse::RDOLexer*>(lexer)
#define PARSER  LEXER->m_parser
#define RUNTIME PARSER->runtime()

namespace rdoParse
{
%}

%start rtp_list

%%

rtp_list:			/* empty */
					| rtp_list rtp_res_type
					| error {
						PARSER->error( "Ожидается ключевое слово $Resource_type" );
					};

rtp_res_type:		rtp_header RDO_Parameters rtp_body RDO_End
					{
						RDORTPResType* res_type = reinterpret_cast<RDORTPResType*>($1);
						if ( res_type->getParams().empty() )
						{
							PARSER->warning( @2, rdo::format( "Тип ресурса '%s' не содежит параметров", res_type->name().c_str() ) );
						}
					}
					| rtp_header RDO_Parameters rtp_body {
						PARSER->error( @2, "Не найдено ключевое слово $End" );
					}
					| rtp_header error {
						PARSER->error( @2, "Не найдено ключевое слово $Parameters" );
					};

rtp_header:			RDO_Resource_type RDO_IDENTIF_COLON rtp_vid_res
					{
						LEXER->m_enum_param_cnt = 0;
						RDOValue*            type_name = reinterpret_cast<RDOValue*>($2);
						std::string          name      = type_name->value().getIdentificator();
						const RDORTPResType* _rtp      = PARSER->findRTPResType( name );
						if ( _rtp ) {
							PARSER->error_push_only( type_name->src_info(), rdo::format("Тип ресурса уже существует: %s", name.c_str()) );
							PARSER->error_push_only( _rtp->src_info(), "См. первое определение" );
							PARSER->error_push_done();
						}
						RDORTPResType* rtp = new RDORTPResType( PARSER, type_name->src_info(), $3 != 0 );
						$$ = (int)rtp;
					}
					| RDO_Resource_type RDO_IDENTIF_COLON error {
						PARSER->error( @2, "Не указан вид ресурса" );
					}
					| RDO_Resource_type error {
						std::string str( LEXER->YYText() );
						PARSER->error( @2, rdo::format("Ошибка в описании имени типа ресурса: %s", str.c_str()) );
					};

rtp_vid_res:		RDO_permanent	{ $$ = 1; }
					| RDO_temporary	{ $$ = 0; };

rtp_body:			/* empty */ {
					}
					| rtp_body rtp_param {
						RDORTPParam* param = reinterpret_cast<RDORTPParam*>($2);
						PARSER->getLastRTPResType()->addParam( param );
					};

rtp_param:			RDO_IDENTIF_COLON param_type fuzzy_terms_list
					{
						RDOValue*            param_name = reinterpret_cast<RDOValue*>($1);
						RDORTPParamType*     param_type = reinterpret_cast<RDORTPParamType*>($2);
						RDORTPFuzzyTermsSet* terms_set  = reinterpret_cast<RDORTPFuzzyTermsSet*>($3);
						if ( terms_set->empty() )
						{
							RDORTPParam* param = new RDORTPParam( PARSER->getLastRTPResType(), param_name->src_info(), param_type );
							param_type->reparent( param );
							if ( param_type->typeID() == rdoRuntime::RDOType::t_enum ) {
								static_cast<RDORTPEnumParamType*>(param_type)->enum_name = rdo::format( "%s.%s", PARSER->getLastRTPResType()->name().c_str(), param_name->src_info().src_text().c_str() );
							}
							$$ = (int)param;
						}
						else
						{
							RDORTPFuzzyParam* param = new RDORTPFuzzyParam( PARSER, param_name->src_info(), terms_set );
							param_type->reparent( param );
							$$ = (int)param;
						}
					}
					| RDO_IDENTIF_COLON error {
						if ( PARSER->lexer_loc_line() == @1.last_line ) {
							std::string str( LEXER->YYText() );
							PARSER->error( @2, rdo::format( "Неверный тип параметра: %s", str.c_str() ) );
						} else {
							PARSER->error( @1, "Ожидается тип параметра" );
						}
					}
					| error {
						PARSER->error( @1, "Неправильное описание параметра" );
					};

fuzzy_terms_list:	/* empty */ {
						RDORTPFuzzyTermsSet* terms_set = new RDORTPFuzzyTermsSet( PARSER );
						$$ = (int)terms_set;
					}
					| fuzzy_terms_list fuzzy_term {
						RDORTPFuzzyTermsSet* terms_set = reinterpret_cast<RDORTPFuzzyTermsSet*>($1);
						RDORTPFuzzyTerm*     term      = reinterpret_cast<RDORTPFuzzyTerm*>($2);
						terms_set->add( term );
						$$ = $1;				
					};

fuzzy_term:			RDO_Fuzzy_Term RDO_IDENTIF {
						RDOValue* param_name = reinterpret_cast<RDOValue*>($2);
//						RDORTPFuzzyMembershiftFun* fuzzy_membershift_fun = reinterpret_cast<RDORTPFuzzyMembershiftFun*>($3);
//						RDORTPFuzzyTerm* fuzzy_term = new RDORTPFuzzyTerm( PARSER, param_name->src_info(), fuzzy_membershift_fun );
//						fuzzy_membershift_fun->reparent( fuzzy_term );
//						$$ = (int)fuzzy_term;				
					};

fuzzy_membershift_fun: /* empty */ {
						RDORTPFuzzyMembershiftFun* fun = new RDORTPFuzzyMembershiftFun( PARSER );
						$$ = (int)fun;
					}
					| fuzzy_membershift_fun membershift_point {					
						RDORTPFuzzyMembershiftFun*   fun   = reinterpret_cast<RDORTPFuzzyMembershiftFun*>($1);
						RDORTPFuzzyMembershiftPoint* point = reinterpret_cast<RDORTPFuzzyMembershiftPoint*>($2);
						fun->add( point );
						$$ = $1;					
						//Задание функции принадлежности точками - вершинами ломанных кривых
					};

membershift_point:	'(' RDO_REAL_CONST ',' RDO_REAL_CONST ')' {					
						double x_value = reinterpret_cast<RDOValue*>($2)->value().getDouble();
						double y_value = reinterpret_cast<RDOValue*>($4)->value().getDouble();
						RDORTPFuzzyMembershiftPoint* fuzzy_membershift_point = new RDORTPFuzzyMembershiftPoint( PARSER, RDOParserSrcInfo( @1, @5 ), x_value, y_value);
						$$ = (int)fuzzy_membershift_point;
					}
					| '(' RDO_REAL_CONST ',' RDO_REAL_CONST ')' ',' {					
						double x_value = reinterpret_cast<RDOValue*>($2)->value().getDouble();
						double y_value = reinterpret_cast<RDOValue*>($4)->value().getDouble();
						RDORTPFuzzyMembershiftPoint* fuzzy_membershift_point = new RDORTPFuzzyMembershiftPoint( PARSER, RDOParserSrcInfo( @1, @5 ), x_value, y_value);
						$$ = (int)fuzzy_membershift_point;
					}
					| '(' RDO_REAL_CONST ',' RDO_INT_CONST ')' {					
						double x_value = reinterpret_cast<RDOValue*>($2)->value().getDouble();
						double y_value = reinterpret_cast<RDOValue*>($4)->value().getDouble();
						RDORTPFuzzyMembershiftPoint* fuzzy_membershift_point = new RDORTPFuzzyMembershiftPoint( PARSER, RDOParserSrcInfo( @1, @5 ), x_value, y_value);
						$$ = (int)fuzzy_membershift_point;
					}
					| '(' RDO_REAL_CONST ',' RDO_INT_CONST ')' ',' {					
						double x_value = reinterpret_cast<RDOValue*>($2)->value().getDouble();
						double y_value = reinterpret_cast<RDOValue*>($4)->value().getDouble();
						RDORTPFuzzyMembershiftPoint* fuzzy_membershift_point = new RDORTPFuzzyMembershiftPoint( PARSER, RDOParserSrcInfo( @1, @5 ), x_value, y_value);
						$$ = (int)fuzzy_membershift_point;
					};							

// ----------------------------------------------------------------------------
// ---------- Описание типа параметра
// ----------------------------------------------------------------------------
param_type:		RDO_integer param_int_diap param_int_default_val
				{
					RDORTPIntDiap*    diap = reinterpret_cast<RDORTPIntDiap*>($2);
					RDORTPDefVal*       dv = reinterpret_cast<RDORTPDefVal*>($3);
					RDORTPIntParamType* rp = new RDORTPIntParamType( PARSER->getLastParsingObject(), diap, dv, RDOParserSrcInfo( @1, @3 ) );
					$$ = (int)rp;
				}
				| RDO_real param_real_diap param_real_default_val
				{
					RDORTPRealDiap*    diap = reinterpret_cast<RDORTPRealDiap*>($2);
					RDORTPDefVal*        dv = reinterpret_cast<RDORTPDefVal*>($3);
					RDORTPRealParamType* rp = new RDORTPRealParamType( PARSER->getLastParsingObject(), diap, dv, RDOParserSrcInfo( @1, @3 ) );
					$$ = (int)rp;
				}
				| RDO_string param_string_default_val
				{
					RDORTPDefVal*          dv = reinterpret_cast<RDORTPDefVal*>($2);
					RDORTPStringParamType* rp = new RDORTPStringParamType( PARSER->getLastParsingObject(), dv, RDOParserSrcInfo( @1, @2 ) );
					$$ = (int)rp;
				}
				| RDO_bool param_bool_default_val
				{
					RDORTPDefVal*        dv = reinterpret_cast<RDORTPDefVal*>($2);
					RDORTPBoolParamType* rp = new RDORTPBoolParamType( PARSER->getLastParsingObject(), dv, RDOParserSrcInfo( @1, @2 ) );
					$$ = (int)rp;
				}
				| param_enum param_enum_default_val
				{
					LEXER->m_enum_param_cnt = 0;
					RDORTPEnum*  enu = reinterpret_cast<RDORTPEnum*>($1);
					RDORTPDefVal* dv = reinterpret_cast<RDORTPDefVal*>($2);
					if ( dv->isExist() )
					{
						enu->findEnumValueWithThrow( dv->value().src_pos(), dv->value().value().getAsString() ); // Если не найдено, то будет сообщение об ошибке, т.е. throw
					}
					RDORTPEnumParamType* rp = new RDORTPEnumParamType( PARSER->getLastParsingObject(), enu, dv, RDOParserSrcInfo( @1, @2 ) );
					$$ = (int)rp;
				}
				| param_such_as
				{
					const RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->name() + "." : "") + param->name() );
					$$ = (int)param->getType()->constructorSuchAs( src_info );
				}
				| param_such_as '=' RDO_INT_CONST
				{
					const RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1, @3 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->name() + "." : "") + param->name() );
					$$ = (int)param->getType()->constructorSuchAs( src_info, *reinterpret_cast<RDOValue*>($3) );
				}
				| param_such_as '=' RDO_REAL_CONST
				{
					const RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1, @3 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->name() + "." : "") + param->name() );
					$$ = (int)param->getType()->constructorSuchAs( src_info, *reinterpret_cast<RDOValue*>($3) );
				}
				| param_such_as '=' RDO_IDENTIF
				{
					const RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1, @3 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->name() + "." : "") + param->name() );
					$$ = (int)param->getType()->constructorSuchAs( src_info, *reinterpret_cast<RDOValue*>($3) );
				}
				| param_such_as '=' error
				{
					PARSER->error( "Ожидается зачение по-умолчанию" );
				};
/*
				| RDO_integer error {
					PARSER->error( @2, "Ошибка после ключевого слова integer. Возможно, не хватает значения по-умолчанию." );
				}
				| RDO_real error {
					PARSER->error( @2, "Ошибка после ключевого слова real. Возможно, не хватает значения по-умолчанию." );
				}
				| param_enum error {
					PARSER->error( @2, "Ошибка после перечислимого типа. Возможно, не хватает значения по-умолчанию." );
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
					RDORTPIntDiap* diap = new RDORTPIntDiap( PARSER, reinterpret_cast<RDOValue*>($2)->value().getInt(), reinterpret_cast<RDOValue*>($4)->value().getInt(), RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST {
					PARSER->error( @2, "Требуется целочисленный диапазон, указан вещественный" );
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST {
					PARSER->error( @2, "Требуется целочисленный диапазон, указан вещественный" );
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST {
					PARSER->error( @4, "Требуется целочисленный диапазон, указан вещественный" );
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST error {
					PARSER->error( @4, "Диапазон задан неверно" );
				}
				| '[' RDO_INT_CONST RDO_dblpoint error {
					PARSER->error( @4, "Диапазон задан неверно" );
				}
				| '[' error {
					PARSER->error( @2, "Диапазон задан неверно" );
				};

param_real_diap:	/* empty */ {
					YYLTYPE pos = @0;
					pos.first_line   = pos.last_line;
					pos.first_column = pos.last_column;
					RDORTPRealDiap* diap = new RDORTPRealDiap( PARSER, pos );
					$$ = (int)diap;
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST ']' {
					double min = reinterpret_cast<RDOValue*>($2)->value().getDouble();
					double max = reinterpret_cast<RDOValue*>($4)->value().getDouble();
					RDORTPRealDiap* diap = new RDORTPRealDiap( PARSER, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST ']' {
					double min = reinterpret_cast<RDOValue*>($2)->value().getDouble();
					double max = reinterpret_cast<RDOValue*>($4)->value().getDouble();
					RDORTPRealDiap* diap = new RDORTPRealDiap( PARSER, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST ']' {
					double min = reinterpret_cast<RDOValue*>($2)->value().getDouble();
					double max = reinterpret_cast<RDOValue*>($4)->value().getDouble();
					RDORTPRealDiap* diap = new RDORTPRealDiap( PARSER, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST ']' {
					double min = reinterpret_cast<RDOValue*>($2)->value().getDouble();
					double max = reinterpret_cast<RDOValue*>($4)->value().getDouble();
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
				};

param_int_default_val:	/* empty */ {
						$$ = (int)new RDORTPDefVal(PARSER);
					}
					| '=' RDO_INT_CONST {
						$$ = (int)new RDORTPDefVal(PARSER, *reinterpret_cast<RDOValue*>($2) );
					}
					| '=' RDO_REAL_CONST {
						PARSER->error( @2, rdo::format("Целое число инициализируется вещественным: %f", reinterpret_cast<RDOValue*>($2)->value().getDouble()) );
					}
					| '=' error {
						RDOParserSrcInfo _src_info(@1, @2, true);
						if ( _src_info.src_pos().point() )
						{
							PARSER->error( _src_info, "Не указано значение по-умолчанию для целого типа" );
						}
						else
						{
							PARSER->error( _src_info, "Неверное значение по-умолчанию для целого типа" );
						}
					};

param_real_default_val:	/* empty */ {
						$$ = (int)new RDORTPDefVal(PARSER);
					}
					| '=' RDO_REAL_CONST {
						$$ = (int)new RDORTPDefVal(PARSER, *reinterpret_cast<RDOValue*>($2));
					}
					| '=' RDO_INT_CONST {
						$$ = (int)new RDORTPDefVal(PARSER, *reinterpret_cast<RDOValue*>($2));
					}
					| '=' error {
						RDOParserSrcInfo _src_info(@1, @2, true);
						if ( _src_info.src_pos().point() )
						{
							PARSER->error( _src_info, "Не указано значение по-умолчанию для вещественного типа" );
						}
						else
						{
							PARSER->error( _src_info, "Неверное значение по-умолчанию для вещественного типа" );
						}
					};

param_string_default_val:	/* empty */
					{
						$$ = (int)new RDORTPDefVal(PARSER);
					}
					| '=' RDO_STRING_CONST
					{
						$$ = (int)new RDORTPDefVal(PARSER, *reinterpret_cast<RDOValue*>($2));
					}
					| '=' error
					{
						RDOParserSrcInfo _src_info(@1, @2, true);
						if ( _src_info.src_pos().point() )
						{
							PARSER->error( _src_info, "Не указано значение по-умолчанию для строчного типа" );
						}
						else
						{
							PARSER->error( _src_info, "Неверное значение по-умолчанию для строчного типа" );
						}
					};

param_bool_default_val:	/* empty */
					{
						$$ = (int)new RDORTPDefVal(PARSER);
					}
					| '=' RDO_BOOL_CONST
					{
						$$ = (int)new RDORTPDefVal(PARSER, *reinterpret_cast<RDOValue*>($2));
					}
					| '=' error
					{
						RDOParserSrcInfo _src_info(@1, @2, true);
						if ( _src_info.src_pos().point() )
						{
							PARSER->error( _src_info, "Не указано значение по-умолчанию для булевского типа" );
						}
						else
						{
							PARSER->error( _src_info, "Неверное значение по-умолчанию для булевского типа" );
						}
					};

param_enum:	'(' param_enum_list ')' {
				RDORTPEnum* enu = reinterpret_cast<RDORTPEnum*>($2);
				enu->setSrcPos( @1, @3 );
				enu->setSrcText( enu->getEnums().asString() );
				$$ = $2;
			}
			| '(' param_enum_list error {
				PARSER->error( @2, "Перечисление должно заканчиваться скобкой" );
			};

param_enum_list: RDO_IDENTIF {
					RDORTPEnum* enu = new RDORTPEnum( PARSER->getLastParsingObject(), *reinterpret_cast<RDOValue*>($1) );
					enu->setSrcInfo( reinterpret_cast<RDOValue*>($1)->src_info() );
					LEXER->m_enum_param_cnt = 1;
					$$ = (int)enu;
				}
				| param_enum_list ',' RDO_IDENTIF {
					if ( LEXER->m_enum_param_cnt >= 1 ) {
						RDORTPEnum* enu  = reinterpret_cast<RDORTPEnum*>($1);
						enu->add( *reinterpret_cast<RDOValue*>($3) );
						$$ = (int)enu;
					} else {
						PARSER->error( @3, "Ошибка в описании значений перечислимого типа" );
					}
				}
				| param_enum_list RDO_IDENTIF {
					if ( LEXER->m_enum_param_cnt >= 1 ) {
						RDORTPEnum* enu  = reinterpret_cast<RDORTPEnum*>($1);
						enu->add( *reinterpret_cast<RDOValue*>($2) );
						$$ = (int)enu;
						PARSER->warning( @1, rdo::format("Пропущена запятая перед: %s", reinterpret_cast<RDOValue*>($2)->value().getIdentificator().c_str()) );
					} else {
						PARSER->error( @2, "Ошибка в описании значений перечислимого типа" );
					}
				}
				| param_enum_list ',' RDO_INT_CONST {
					PARSER->error( @3, "Значение перечислимого типа не может быть цифрой" );
				}
				| param_enum_list ',' RDO_REAL_CONST {
					PARSER->error( @3, "Значение перечислимого типа не может быть цифрой" );
				}
				| param_enum_list RDO_INT_CONST {
					PARSER->error( @2, "Значение перечислимого типа не может быть цифрой" );
				}
				| param_enum_list RDO_REAL_CONST {
					PARSER->error( @2, "Значение перечислимого типа не может быть цифрой" );
				}
				| RDO_INT_CONST {
					PARSER->error( @1, "Значение перечислимого типа не может начинаться с цифры" );
				}
				| RDO_REAL_CONST {
					PARSER->error( @1, "Значение перечислимого типа не может начинаться с цифры" );
				};

param_enum_default_val:	/* empty */ {
						$$ = (int)new RDORTPDefVal(PARSER);
					}
					| '=' RDO_IDENTIF {
						$$ = (int)new RDORTPDefVal(PARSER, *reinterpret_cast<RDOValue*>($2));
					}
					| '=' error {
						RDOParserSrcInfo _src_info(@1, @2, true);
						if ( _src_info.src_pos().point() )
						{
							PARSER->error( _src_info, "Не указано значение по-умолчанию для перечислимого типа" );
						}
						else
						{
							PARSER->error( _src_info, "Неверное значение по-умолчанию для перечислимого типа" );
						}
					};

param_such_as:	RDO_such_as RDO_IDENTIF '.' RDO_IDENTIF {
					std::string type  = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
					std::string param = reinterpret_cast<RDOValue*>($4)->value().getIdentificator();
					const RDORTPResType* const rt = PARSER->findRTPResType( type );
					if ( !rt ) {
						PARSER->error( @2, rdo::format("Ссылка на неизвестный тип ресурса: %s", type.c_str()) );
					}
					const RDORTPParam* const rp = rt->findRTPParam( param );
					if ( !rp ) {
						PARSER->error( @4, rdo::format("Ссылка на неизвестный параметр ресурса: %s.%s", type.c_str(), param.c_str()) );
					}
					$$ = (int)rp;
				}
				| RDO_such_as RDO_IDENTIF {
					std::string constName = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
					const RDOFUNConstant* const cons = PARSER->findFUNConstant( constName );
					if ( !cons ) {
						PARSER->error( @2, rdo::format("Ссылка на несуществующую константу: %s", constName.c_str()) );
					}
					$$ = (int)cons->getDescr();
				}
				| RDO_such_as RDO_IDENTIF '.' error {
					std::string type = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
					const RDORTPResType* const rt = PARSER->findRTPResType( type );
					if ( !rt ) {
						PARSER->error( @2, rdo::format("Ссылка на неизвестный тип ресурса: %s", type.c_str()) );
					} else {
						PARSER->error( @4, "Ошибка при указании параметра" );
					}
				}
				| RDO_such_as error {
					PARSER->error( @2, "После ключевого слова such_as необходимо указать тип и параметер ресурса для ссылки" );
				};
// ----------------------------------------------------------------------------

%%

}

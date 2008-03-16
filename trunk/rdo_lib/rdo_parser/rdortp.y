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
%token GENERATE				365
%token TERMINATE			366
%token ADVANCE				367
%token RELEASE				368
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

rtp_res_type:	rtp_res_type_hdr Parameters rtp_body End {
					RDORTPResType* res_type = reinterpret_cast<RDORTPResType*>($1);
					if ( $3 == 0 ) {
						PARSER->warning( @2, rdo::format( "Тип ресурса '%s' не содежит параметров", res_type->getName().c_str() ) );
					}
				}
				| rtp_res_type_hdr Parameters rtp_body {
					PARSER->error( @3, "Не найдено ключевое слово $End" );
				}
				| rtp_res_type_hdr error {
					PARSER->error( @2, "Не найдено ключевое слово $Parameters" );
				};

rtp_res_type_hdr:	Resource_type IDENTIF_COLON rtp_vid_res {
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
					| Resource_type error {
						std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
						PARSER->error( @2, rdo::format("Ошибка в описании имени типа ресурса: %s", str.c_str()) );
					}
					| Resource_type IDENTIF_COLON error {
						PARSER->error( @2, "Не указан вид ресурса" );
					};

rtp_vid_res:	permanent_kw	{ $$ = 1; }
				| temporary_kw	{ $$ = 0; };

rtp_body:	/* empty */ {
				$$ = 0; // warning
			}
			| rtp_body rtp_param {
				RDORTPParam* param = reinterpret_cast<RDORTPParam*>($2);
				PARSER->getLastRTPResType()->addParam( param );
				$$ = 1; // no warning
			};

rtp_param: IDENTIF_COLON param_type {
					RDOParserSrcInfo par_src_info(@1, *reinterpret_cast<std::string*>($1), RDOParserSrcInfo::psi_align_bytext);
					RDORTPParamType* parType = reinterpret_cast<RDORTPParamType*>($2);
					RDORTPParam* param = new RDORTPParam( PARSER->getLastRTPResType(), par_src_info, parType );
					parType->reparent( param );
					if ( parType->getType() == rdoRuntime::RDOValue::Type::rvt_enum ) {
						static_cast<RDORTPEnumParamType*>(parType)->enum_name = rdo::format( "%s.%s", PARSER->getLastRTPResType()->getName().c_str(), par_src_info.src_text().c_str() );
					}
					$$ = (int)param;
				}
				| IDENTIF_COLON error {
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
param_type:		integer param_int_diap param_int_default_val {
					RDORTPIntDiap* diap = reinterpret_cast<RDORTPIntDiap*>($2);
					RDORTPIntDefVal* dv = reinterpret_cast<RDORTPIntDefVal*>($3);
					RDORTPIntParamType* rp = new RDORTPIntParamType( PARSER->getLastParsingObject(), diap, dv, RDOParserSrcInfo( @1, @3 ) );
					$$ = (int)rp;
				}
				| real param_real_diap param_real_default_val {
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
				| param_such_as {
					RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->getName() + "." : "") + param->getName() );
					$$ = (int)param->getType()->constructSuchAs( src_info );
				}
				| param_such_as '=' INT_CONST {
					RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1, @3 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->getName() + "." : "") + param->getName() );
					$$ = (int)param->getType()->constructSuchAs( (int)$3, src_info, RDOParserSrcInfo( @3 ) );
				}
				| param_such_as '=' REAL_CONST {
					RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1, @3 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->getName() + "." : "") + param->getName() );
					$$ = (int)param->getType()->constructSuchAs( *(double*)$3, src_info, RDOParserSrcInfo( @3 ) );
				}
				| param_such_as '=' IDENTIF {
					RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1, @3 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->getName() + "." : "") + param->getName() );
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
				| '[' INT_CONST dblpoint INT_CONST ']' {
					RDORTPIntDiap* diap = new RDORTPIntDiap( PARSER, $2, $4, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint REAL_CONST {
					PARSER->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' REAL_CONST dblpoint INT_CONST {
					PARSER->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' INT_CONST dblpoint REAL_CONST {
					PARSER->error( @4, rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' INT_CONST dblpoint INT_CONST error {
					PARSER->error( @4, "Диапазон задан неверно" );
				}
				| '[' INT_CONST dblpoint error {
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
				| '[' REAL_CONST dblpoint REAL_CONST ']' {
					double min = *reinterpret_cast<double*>($2);
					double max = *reinterpret_cast<double*>($4);
					RDORTPRealDiap* diap = new RDORTPRealDiap( PARSER, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint INT_CONST ']' {
					double min = *reinterpret_cast<double*>($2);
					double max = $4;
					RDORTPRealDiap* diap = new RDORTPRealDiap( PARSER, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint REAL_CONST ']' {
					double min = $2;
					double max = *reinterpret_cast<double*>($4);
					RDORTPRealDiap* diap = new RDORTPRealDiap( PARSER, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint INT_CONST ']' {
					double min = $2;
					double max = $4;
					RDORTPRealDiap* diap = new RDORTPRealDiap( PARSER, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint REAL_CONST error {
					PARSER->error( @4, "Диапазон задан неверно" );
				}
				| '[' REAL_CONST dblpoint INT_CONST error {
					PARSER->error( @4, "Диапазон задан неверно" );
				}
				| '[' INT_CONST dblpoint REAL_CONST error {
					PARSER->error( @4, "Диапазон задан неверно" );
				}
				| '[' INT_CONST dblpoint INT_CONST error {
					PARSER->error( @4, "Диапазон задан неверно" );
				}
				| '[' REAL_CONST dblpoint error {
					PARSER->error( @4, "Диапазон задан неверно" );
				}
				| '[' INT_CONST dblpoint error {
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
					| '=' INT_CONST {
						$$ = (int)(new RDORTPIntDefVal(PARSER, $2, RDOParserSrcInfo( @1, @2 )));
					}
					| '=' REAL_CONST {
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
					| '=' REAL_CONST {
						$$ = (int)(new RDORTPRealDefVal(PARSER, *((double *)$2), RDOParserSrcInfo( @1, @2 )));
					}
					| '=' INT_CONST {
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

param_enum_list: IDENTIF {
					RDORTPEnum* enu = new RDORTPEnum( PARSER->getLastParsingObject(), *(std::string *)$1 );
					std::string* first = reinterpret_cast<std::string*>($1);
					enu->setSrcText( "(" + *first );
					reinterpret_cast<RDOLexer*>(lexer)->m_enum_param_cnt = 1;
					$$ = (int)enu;
				}
				| param_enum_list ',' IDENTIF {
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
				| param_enum_list IDENTIF {
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
				| param_enum_list ',' INT_CONST {
					PARSER->error( @3, "Значение перечислимого типа не может начинаться с цифры" );
				}
				| param_enum_list ',' REAL_CONST {
					PARSER->error( @3, "Значение перечислимого типа не может начинаться с цифры" );
				}
				| INT_CONST {
					PARSER->error( @1, "Значение перечислимого типа не может начинаться с цифры" );
				}
				| REAL_CONST {
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
					| '=' IDENTIF {
						$$ = (int)(new RDORTPEnumDefVal( PARSER, *(std::string*)$2, RDOParserSrcInfo( @1, @2 ) ));
					}
					| '=' {
						PARSER->error( @1, "Не указано значение по-умолчанию для перечислимого типа" );
					}
					| '=' error {
						PARSER->error( @2, "Неверное значение по-умолчанию для перечислимого типа" );
					};

param_such_as:	such_as IDENTIF '.' IDENTIF {
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
				| such_as IDENTIF {
					std::string constName = *reinterpret_cast<std::string*>($2);
					const RDOFUNConstant* const cons = PARSER->findFUNConst( constName );
					if ( !cons ) {
						PARSER->error( @2, rdo::format("Ссылка на несуществующую константу: %s", constName.c_str()) );
					}
					$$ = (int)(cons->getDescr());
				}
				| such_as IDENTIF '.' {
					std::string type = *reinterpret_cast<std::string*>($2);
					const RDORTPResType* const rt = PARSER->findRTPResType( type );
					if ( !rt ) {
						PARSER->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					} else {
						PARSER->error( @3, "Не указан параметер" );
					}
				}
				| such_as IDENTIF '.' error {
					std::string type = *reinterpret_cast<std::string*>($2);
					const RDORTPResType* const rt = PARSER->findRTPResType( type );
					if ( !rt ) {
						PARSER->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					} else {
						PARSER->error( @4, "Ошибка при указании параметра" );
					}
				}
				| such_as IDENTIF error {
					std::string type = *reinterpret_cast<std::string*>($2);
					const RDORTPResType* const rt = PARSER->findRTPResType( type );
					if ( !rt ) {
						PARSER->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					} else {
						PARSER->error( @2, "После имени типа должен быть указан параметер через точку" );
					}
				}
				| such_as error {
					PARSER->error( @2, "После ключевого слова such_as необходимо указать тип и параметер ресурса для ссылки" );
				};
// ----------------------------------------------------------------------------

%%

}

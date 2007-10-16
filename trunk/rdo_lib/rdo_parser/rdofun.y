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
#include "rdortp.h"
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

%start fun_list

%%

fun_list:	fun_consts fun_func_seq;
			| error {
				if ( parser->hasConstant() ) {
					parser->error( @1, "Ожидается описание функции или последовательности" );
				} else {
					parser->error( @1, "Ожидается описание функции, последовательности или константы" );
				}
			};

// ----------------------------------------------------------------------------
// ---------- Константы
// ----------------------------------------------------------------------------
fun_consts:	/* empty */
			| Constant fun_const_body End
			| Constant fun_const_body {
				parser->error( @2, "После описания констант ожидается ключевое слово $End" );
			};

fun_const_body:	/* empty */
				| fun_const_body fun_const_param_desc {
				}
				| fun_const_body error {
					parser->error( @2, "Ожидается описание константы" );
				};

fun_const_param_desc:	IDENTIF_COLON param_type {
							std::string name = *reinterpret_cast<std::string*>($1);
							RDOParserSrcInfo src_info = RDOParserSrcInfo(@1, name, RDOParserSrcInfo::psi_align_bytext);
							parser->checkFunctionName( src_info );
							RDORTPParamType* parType = reinterpret_cast<RDORTPParamType*>($2);
							if ( !parType->dv->isExist() ) {
								parser->error( @2, "Константа должна иметь значение" );
//								parser->error( "Constant must have value" );
							}
							RDORTPParam* param = new RDORTPParam( parser, src_info, parType );
							RDOFUNConstant* newConst = new RDOFUNConstant( parser, param );
							newConst->setSrcInfo( src_info );
							parser->runtime->setConstValue( newConst->number, newConst->getType()->getParamDefaultValue( param->src_info() ) );
							$$ = (int)newConst;
						}
						| IDENTIF_COLON {
							parser->error( @1, "Ожидается тип константы" );
						}
						| IDENTIF_COLON error {
							parser->error( @2, "Ошибка описания типа константы" );
						};

// ----------------------------------------------------------------------------
// ---------- Описание типа параметра
// ----------------------------------------------------------------------------
param_type:		integer param_int_diap param_int_default_val {
					RDORTPIntDiap* diap = reinterpret_cast<RDORTPIntDiap*>($2);
					RDORTPIntDefVal* dv = reinterpret_cast<RDORTPIntDefVal*>($3);
					RDORTPIntParamType* rp = new RDORTPIntParamType( parser->getLastParsingObject(), diap, dv, RDOParserSrcInfo( @1, @3 ) );
					$$ = (int)rp;
				}
				| real param_real_diap param_real_default_val {
					RDORTPRealDiap* diap = reinterpret_cast<RDORTPRealDiap*>($2);
					RDORTPRealDefVal* dv = reinterpret_cast<RDORTPRealDefVal*>($3);
					RDORTPRealParamType* rp = new RDORTPRealParamType( parser->getLastParsingObject(), diap, dv, RDOParserSrcInfo( @1, @3 ) );
					$$ = (int)rp;
				}
				| param_enum param_enum_default_val {
					reinterpret_cast<RDOLexer*>(lexer)->enum_param_cnt = 0;
					RDORTPEnum* enu      = reinterpret_cast<RDORTPEnum*>($1);
					RDORTPEnumDefVal* dv = reinterpret_cast<RDORTPEnumDefVal*>($2);
					if ( dv->isExist() ) {
						enu->findEnumValueWithThrow( dv->src_pos(), dv->getEnumValue() ); // Если не найдено, то будет сообщение об ошибке, т.е. throw
					}
					RDORTPEnumParamType* rp = new RDORTPEnumParamType( parser->getLastParsingObject(), enu, dv, RDOParserSrcInfo( @1, @2 ) );
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
					parser->error( "Ожидается зачение по-умолчанию" );
				}
				| param_such_as error {
					parser->error( "Ожидается окончание описания параметра-ссылки, например, зачение по-умолчанию" );
				};
/*
				| integer error {
					parser->error( @2, "Ошибка после ключевого слова integer. Возможно, не хватает значения по-умолчанию." );
//					parser->error( rdoSimulator::RDOSyntaxError::RTP_WAITING_FOR_INT_PARAM_END );
				}
				| real error {
					parser->error( @2, "Ошибка после ключевого слова real. Возможно, не хватает значения по-умолчанию." );
//					parser->error( rdoSimulator::RDOSyntaxError::RTP_WAITING_FOR_REAL_PARAM_END );
				}
				| param_enum error {
					parser->error( @2, "Ошибка после перечислимого типа. Возможно, не хватает значения по-умолчанию." );
//					parser->error( rdoSimulator::RDOSyntaxError::RTP_WAITING_FOR_ENUM_PARAM_END );
				};
*/
param_int_diap:	/* empty */ {
					YYLTYPE pos = @0;
					pos.first_line   = pos.last_line;
					pos.first_column = pos.last_column;
					RDORTPIntDiap* diap = new RDORTPIntDiap( parser, pos );
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint INT_CONST ']' {
					RDORTPIntDiap* diap = new RDORTPIntDiap( parser, $2, $4, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint REAL_CONST {
					parser->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' REAL_CONST dblpoint INT_CONST {
					parser->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' INT_CONST dblpoint REAL_CONST {
					parser->error( @4, rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' INT_CONST dblpoint INT_CONST error {
					parser->error( @4, "Диапазон задан неверно" );
				}
				| '[' INT_CONST dblpoint error {
					parser->error( @4, "Диапазон задан неверно" );
				}
				| '[' error {
					parser->error( @2, "Диапазон задан неверно" );
//					parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_RANGE );
				};

param_real_diap:	/* empty */ {
					YYLTYPE pos = @0;
					pos.first_line   = pos.last_line;
					pos.first_column = pos.last_column;
					RDORTPRealDiap* diap = new RDORTPRealDiap( parser, pos );
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint REAL_CONST ']' {
					double min = *reinterpret_cast<double*>($2);
					double max = *reinterpret_cast<double*>($4);
					RDORTPRealDiap* diap = new RDORTPRealDiap( parser, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint INT_CONST ']' {
					double min = *reinterpret_cast<double*>($2);
					double max = $4;
					RDORTPRealDiap* diap = new RDORTPRealDiap( parser, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint REAL_CONST ']' {
					double min = $2;
					double max = *reinterpret_cast<double*>($4);
					RDORTPRealDiap* diap = new RDORTPRealDiap( parser, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint INT_CONST ']' {
					double min = $2;
					double max = $4;
					RDORTPRealDiap* diap = new RDORTPRealDiap( parser, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint REAL_CONST error {
					parser->error( @4, "Диапазон задан неверно" );
				}
				| '[' REAL_CONST dblpoint INT_CONST error {
					parser->error( @4, "Диапазон задан неверно" );
				}
				| '[' INT_CONST dblpoint REAL_CONST error {
					parser->error( @4, "Диапазон задан неверно" );
				}
				| '[' INT_CONST dblpoint INT_CONST error {
					parser->error( @4, "Диапазон задан неверно" );
				}
				| '[' REAL_CONST dblpoint error {
					parser->error( @4, "Диапазон задан неверно" );
				}
				| '[' INT_CONST dblpoint error {
					parser->error( @4, "Диапазон задан неверно" );
				}
				| '[' error {
					parser->error( @2, "Диапазон задан неверно" );
//					parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_RANGE );
				};

param_int_default_val:	/* empty */ {
						YYLTYPE pos = @0;
						pos.first_line   = pos.last_line;
						pos.first_column = pos.last_column;
						$$ = (int)(new RDORTPIntDefVal(parser, pos));
					}
					| '=' INT_CONST {
						$$ = (int)(new RDORTPIntDefVal(parser, $2, RDOParserSrcInfo( @1, @2 )));
					}
					| '=' REAL_CONST {
						// Целое число инициализируется вещественным: %f
						parser->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_DEFVAULT_INT_AS_REAL, *(double*)$2 );
					}
					| '=' {
						parser->error( @1, "Не указано значение по-умолчанию для целого типа" );
					}
					| '=' error {
						parser->error( @2, "Неверное значение по-умолчанию для целого типа" );
					};

param_real_default_val:	/* empty */ {
						YYLTYPE pos = @0;
						pos.first_line   = pos.last_line;
						pos.first_column = pos.last_column;
						$$ = (int)(new RDORTPRealDefVal(parser, pos));
					}
					| '=' REAL_CONST {
						$$ = (int)(new RDORTPRealDefVal(parser, *((double *)$2), RDOParserSrcInfo( @1, @2 )));
					}
					| '=' INT_CONST {
						$$ = (int)(new RDORTPRealDefVal(parser, $2, RDOParserSrcInfo( @1, @2 )));
					}
					| '=' {
						parser->error( @1, "Не указано значение по-умолчанию для вещественного типа" );
					}
					| '=' error {
						parser->error( @2, "Неверное значение по-умолчанию для вещественного типа" );
					};

param_enum:	'(' param_enum_list ')' {
				RDORTPEnum* enu = reinterpret_cast<RDORTPEnum*>($2);
				enu->setSrcPos( @1, @3 );
				enu->setSrcText( enu->src_text() + ")" );
				$$ = $2;
			}
			| '(' param_enum_list {
				parser->error( @2, "Перечисление должно заканчиваться скобкой" );
			};

param_enum_list: IDENTIF {
					RDORTPEnum* enu = new RDORTPEnum( parser->getLastParsingObject(), *(std::string *)$1 );
					std::string* first = reinterpret_cast<std::string*>($1);
					enu->setSrcText( "(" + *first );
					reinterpret_cast<RDOLexer*>(lexer)->enum_param_cnt = 1;
					$$ = (int)enu;
				}
				| param_enum_list ',' IDENTIF {
					if ( reinterpret_cast<RDOLexer*>(lexer)->enum_param_cnt >= 1 ) {
						RDORTPEnum* enu  = reinterpret_cast<RDORTPEnum*>($1);
						std::string next = *reinterpret_cast<std::string*>($3);
						enu->add( next, @3 );
						enu->setSrcText( enu->src_text() + ", " + next );
						$$ = (int)enu;
					} else {
						parser->error( "Ошибка в описании значений перечислимого типа" );
					}
				}
				| param_enum_list IDENTIF {
					if ( reinterpret_cast<RDOLexer*>(lexer)->enum_param_cnt >= 1 ) {
						parser->error( rdo::format("Пропущена запятая перед: %s", ((std::string*)$2)->c_str()) );
					} else {
						parser->error( "Ошибка в описании значений перечислимого типа" );
					}
				}
				| param_enum_list error {
					std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
					if ( str.empty() ) {
						parser->error( @1, "Ошибка в описании значений перечислимого типа" );
					} else {
						parser->error( @2, rdo::format( "Неверное значение перечислимого типа: %s", str.c_str() ) );
					}
				}
				| param_enum_list ',' INT_CONST {
					parser->error( @3, "Значение перечислимого типа не может начинаться с цифры" );
				}
				| param_enum_list ',' REAL_CONST {
					parser->error( @3, "Значение перечислимого типа не может начинаться с цифры" );
				}
				| INT_CONST {
					parser->error( @1, "Значение перечислимого типа не может начинаться с цифры" );
				}
				| REAL_CONST {
					parser->error( @1, "Значение перечислимого типа не может начинаться с цифры" );
				}
				| error {
					parser->error( @1, "Ошибка в описании значений перечислимого типа" );
				};

param_enum_default_val:	/* empty */ {
						YYLTYPE pos = @0;
						pos.first_line   = pos.last_line;
						pos.first_column = pos.last_column;
						$$ = (int)(new RDORTPEnumDefVal(parser, pos));
					}
					| '=' IDENTIF {
						$$ = (int)(new RDORTPEnumDefVal(parser, *(std::string*)$2, RDOParserSrcInfo( @1, @2 ) ));
					}
					| '=' {
						parser->error( @1, "Не указано значение по-умолчанию для перечислимого типа" );
					}
					| '=' error {
						parser->error( @2, "Неверное значение по-умолчанию для перечислимого типа" );
					};

param_such_as:	such_as IDENTIF '.' IDENTIF {
					std::string type  = *reinterpret_cast<std::string*>($2);
					std::string param = *reinterpret_cast<std::string*>($4);
					const RDORTPResType* const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					}
					const RDORTPParam* const rp = rt->findRTPParam( param );
					if ( !rp ) {
						parser->error( @4, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_PARAM, type.c_str(), param.c_str() );
					}
					$$ = (int)rp;
				}
				| such_as IDENTIF {
					std::string constName = *reinterpret_cast<std::string*>($2);
					const RDOFUNConstant* const cons = parser->findFUNConst( constName );
					if ( !cons ) {
						parser->error( @2, rdo::format("Ссылка на несуществующую константу: %s", constName.c_str()) );
					}
					$$ = (int)(cons->getDescr());
				}
				| such_as IDENTIF '.' {
					std::string type = *reinterpret_cast<std::string*>($2);
					const RDORTPResType* const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					} else {
						parser->error( @3, "Не указан параметер" );
					}
				}
				| such_as IDENTIF '.' error {
					std::string type = *reinterpret_cast<std::string*>($2);
					const RDORTPResType* const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					} else {
						parser->error( @4, "Ошибка при указании параметра" );
					}
				}
				| such_as IDENTIF error {
					std::string type = *reinterpret_cast<std::string*>($2);
					const RDORTPResType* const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					} else {
						parser->error( @2, "После имени типа должен быть указан параметер через точку" );
					}
				}
				| such_as error {
					parser->error( @2, "После ключевого слова such_as необходимо указать тип и параметер ресурса для ссылки" );
				};
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// ---------- Последовательности и функции
// ----------------------------------------------------------------------------
fun_func_seq:	/* empty */
			| fun_func_seq fun_func_descr
			| fun_func_seq fun_seq_descr
			| fun_func_seq Constant {
				parser->error( @2, "Константы долны быть описаны первыми, перед функциями и последовательностями" );
			};

// ----------------------------------------------------------------------------
// ---------- Функции
// ----------------------------------------------------------------------------
fun_func_descr:	fun_func_header fun_func_footer
				| fun_func_header error {
					RDOFUNFunction* fun = reinterpret_cast<RDOFUNFunction*>($1);
					parser->error( @2, rdo::format("Ожидается ключевое слово $Type с указанием типа функции '%s'", fun->getName().c_str()) );
				};

fun_func_header:	Function_keyword IDENTIF_COLON param_type {
						std::string name = *reinterpret_cast<std::string*>($2);
						RDOParserSrcInfo src_info = RDOParserSrcInfo(@2, name, RDOParserSrcInfo::psi_align_bytext);
						parser->checkFunctionName( src_info );
						RDORTPParamType* retType = reinterpret_cast<RDORTPParamType*>($3);
						RDOFUNFunction* fun = new RDOFUNFunction( parser, src_info, retType );
						if ( retType->getType() == RDORTPParamType::pt_enum && static_cast<RDORTPEnumParamType*>(retType)->enum_name.empty() ) {
							static_cast<RDORTPEnumParamType*>(retType)->enum_name = name;
							static_cast<RDORTPEnumParamType*>(retType)->enum_fun  = true;
						}
						$$ = (int)fun;
					}
					| Function_keyword IDENTIF_COLON error {
						std::string name = *reinterpret_cast<std::string*>($2);
						parser->error( @3, rdo::format("Ожидается тип возвращаемого значения функции '%s'", name.c_str()) );
					}
					| Function_keyword error {
						parser->error( @2, "После ключевого слова $Function ожидается имя фунции" );
					};

fun_func_parameters:	/* empty */
					| Parameters fun_func_params {
					};

fun_func_params:	/* empty */
				| fun_func_params IDENTIF_COLON param_type {
					std::string      name = *reinterpret_cast<std::string*>($2);
					RDORTPParamType* type = reinterpret_cast<RDORTPParamType*>($3);
					RDOFUNFunctionParam* param = new RDOFUNFunctionParam( parser->getLastFUNFunction(), RDOParserSrcInfo( @2, name, RDOParserSrcInfo::psi_align_bytext ), type );
					parser->getLastFUNFunction()->add( param );
				}
				| fun_func_params IDENTIF_COLON error {
					parser->error( @3, "Ожидается тип параметра функции" );
				}
				| fun_func_params error {
					parser->error( @2, "Ожидается описание параметра функции в формате <имя>: <тип>" );
				};

fun_func_footer:	Type_keyword '=' algorithmic fun_func_parameters Body fun_func_algorithmic_body End {
						RDOFUNFunction* currFunc = parser->getLastFUNFunction();
						currFunc->createAlgorithmicCalc( @5 );
					}
					| Type_keyword '=' list_keyword fun_func_parameters Body fun_func_list_body End {
						RDOFUNFunction* currFunc = parser->getLastFUNFunction();
						currFunc->createListCalc();
					}
					| Type_keyword '=' table_keyword fun_func_parameters Body fun_func_list_body End {
						RDOFUNFunction* currFunc = parser->getLastFUNFunction();
						currFunc->createTableCalc( @6 );
					}
					| Type_keyword '=' algorithmic fun_func_parameters Body fun_func_algorithmic_body error {
						parser->error( @7, "Ожидается ключевое слово $End" );
					}
					| Type_keyword '=' list_keyword fun_func_parameters Body fun_func_list_body error {
						parser->error( @7, "Ожидается ключевое слово $End" );
					}
					| Type_keyword '=' table_keyword fun_func_parameters Body fun_func_list_body error {
						parser->error( @7, "Ожидается ключевое слово $End" );
					}
					| Type_keyword '=' algorithmic error {
						parser->error( @4, "Ожидается ключевое слово $Parameters" );
					}
					| Type_keyword '=' list_keyword error {
						parser->error( @4, "Ожидается ключевое слово $Parameters" );
					}
					| Type_keyword '=' table_keyword error {
						parser->error( @4, "Ожидается ключевое слово $Parameters" );
					}
					| Type_keyword '=' error {
						parser->error( @3, "Неизвестный тип функции" );
					}
					| Type_keyword error {
						parser->error( @2, "После ключевого слова $Type ожидается тип функции" );
					};

fun_func_algorithmic_body:	/* empty */
							| fun_func_algorithmic_body fun_func_algorithmic_calc_if {
								RDOFUNCalculateIf* calculateIf = reinterpret_cast<RDOFUNCalculateIf*>($2);
								parser->getLastFUNFunction()->add( calculateIf );
							};

fun_func_calc_if:	Calculate_if {
					}
					| if_keyword {
					};

fun_func_calc_name:	result_keyword {
					}
					| IDENTIF {
						std::string name = *reinterpret_cast<std::string*>($1);
						if ( name != parser->getLastFUNFunction()->getName() ) {
							parser->error( @1, rdo::format("Ожидается имя функции '%s'", parser->getLastFUNFunction()->getName().c_str()) );
						}
					};

fun_func_algorithmic_calc_if:	fun_func_calc_if fun_logic fun_func_calc_name '=' fun_arithm {
									RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
									logic->setSrcText( "Calculate_if " + logic->src_text() );
									RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($5);
									RDOFUNCalculateIf* calculateIf = new RDOFUNCalculateIf( parser->getLastFUNFunction(), logic, arithm );
									$$ = (int)calculateIf;
								}
								| fun_func_calc_if fun_logic fun_func_calc_name '=' error {
									parser->error( @5, "Ошибка в арифметическом выражении" );
								}
								| fun_func_calc_name '=' fun_arithm {
									rdoRuntime::RDOCalcConst* calc_cond = new rdoRuntime::RDOCalcConst( parser->runtime, 1 );
									RDOParserSrcInfo logic_src_info( "Calculate_if 1 = 1" );
									logic_src_info.setSrcPos( @1.first_line, @1.first_column, @1.first_line, @1.first_column );
									calc_cond->setSrcInfo( logic_src_info );
									RDOFUNLogic* logic = new RDOFUNLogic( parser->getLastFUNFunction(), calc_cond, true );
									logic->setSrcInfo( logic_src_info );
									RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($3);
									RDOFUNCalculateIf* calculateIf = new RDOFUNCalculateIf( parser->getLastFUNFunction(), logic, arithm );
									$$ = (int)calculateIf;
								}
								| fun_func_calc_name '=' error {
									parser->error( @3, "Ошибка в арифметическом выражении" );
								}
								| fun_func_calc_if fun_logic fun_func_calc_name error {
									parser->error( @4, "Ожидается '='" );
								}
								| fun_func_calc_name error {
									parser->error( @2, "Ожидается '='" );
								}
								| fun_func_calc_if fun_logic error {
									parser->error( @2, @3, "После логического выражения ожидается <имя_функции> = <результат_функции>" );
								}
								| fun_func_calc_if error {
									parser->error( @2, "Ошибка в логическом выражении" );
								}
								| error {
									parser->error( @1, "Ожидается ключевое слово Calculate_if" );
								};

fun_func_list_body:	/* empty */
			| fun_func_list_body fun_func_list_value;

fun_func_list_value: IDENTIF {
						std::string str = *reinterpret_cast<std::string*>($1);
						RDOFUNFunctionListElementIdentif* value = new RDOFUNFunctionListElementIdentif( parser->getLastFUNFunction(), RDOParserSrcInfo( @1, str ) );
						parser->getLastFUNFunction()->add( value );
						$$ = (int)value;
					}
					| REAL_CONST {
						RDOFUNFunctionListElementReal* value = new RDOFUNFunctionListElementReal( parser->getLastFUNFunction(), @1, *(double *)$1 );
						parser->getLastFUNFunction()->add( value );
						$$ = (int)value;
					}
					| INT_CONST {
						RDOFUNFunctionListElementInt* value = new RDOFUNFunctionListElementInt( parser->getLastFUNFunction(), @1, (int)$1 );
						parser->getLastFUNFunction()->add( value );
						$$ = (int)value;
					}
					| '=' {
						RDOFUNFunctionListElementEq* value = new RDOFUNFunctionListElementEq( parser->getLastFUNFunction(), @1 );
						parser->getLastFUNFunction()->add( value );
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

fun_seq_header:		Sequence IDENTIF_COLON param_type Type_keyword '=' {
						std::string name = *reinterpret_cast<std::string*>($2);
						RDOParserSrcInfo src_info = RDOParserSrcInfo(@2, name, RDOParserSrcInfo::psi_align_bytext);
						parser->checkFunctionName( src_info );
						$$ = (int)(new RDOFUNSequence::RDOFUNSequenceHeader( parser, reinterpret_cast<RDORTPParamType*>($3), src_info ));
					}
					| Sequence IDENTIF_COLON param_type Type_keyword '=' error {
						parser->error( @6, "После знака равенства ожидается тип последовательности" );
					}
					| Sequence IDENTIF_COLON param_type Type_keyword error {
						parser->error( @5, "После ключевого слова $Type ожидается знак равенства и тип последовательности" );
					}
					| Sequence IDENTIF_COLON param_type error {
						parser->error( @4, "Ожидается ключевое слово $Type" );
					}
					| Sequence IDENTIF_COLON error {
						parser->error( @2, @3, "После имени последовательности ожидается тип возвращаемого значения" );
					}
					| Sequence error {
						parser->error( @1, @2, "После ключевого слова $Sequence ожидаются имя и тип результата последовательности в формате '<имя> : <тип>'" );
					};

// ----------------------------------------------------------------------------
// ---------- Датчики случайных чисел
// ----------------------------------------------------------------------------
fun_seq_uniform:	fun_seq_header uniform End {
						RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequence::RDOFUNSequenceHeader*>($1);
						RDOFUNSequence* seq = new RDOFUNSequenceUniform( parser, header );
						seq->createCalcs();
						$$ = (int)seq;
					}
					| fun_seq_header uniform INT_CONST End {
						RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequence::RDOFUNSequenceHeader*>($1);
						RDOFUNSequence* seq = new RDOFUNSequenceUniform( parser, header, $3 );
						seq->createCalcs();
						$$ = (int)seq;
					}
					| fun_seq_header uniform INT_CONST error {
						parser->error( @4, "После базы ожидается ключевое слово $End" );
					}
					| fun_seq_header uniform error {
						parser->error( @3, "После типа последовательности ожидается база генератора или ключевое слово $End" );
					};

fun_seq_exponential: fun_seq_header exponential End {
						RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequence::RDOFUNSequenceHeader*>($1);
						RDOFUNSequence* seq = new RDOFUNSequenceExponential( parser, header, $3 );
						seq->createCalcs();
						$$ = (int)seq;
					}
					| fun_seq_header exponential INT_CONST End {
						RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequence::RDOFUNSequenceHeader*>($1);
						RDOFUNSequence* seq = new RDOFUNSequenceExponential( parser, header, $3 );
						seq->createCalcs();
						$$ = (int)seq;
					}
					| fun_seq_header exponential INT_CONST error {
						parser->error( @4, "После базы ожидается ключевое слово $End" );
					}
					| fun_seq_header exponential error {
						parser->error( @3, "После типа последовательности ожидается база генератора или ключевое слово $End" );
					};

fun_seq_normal:		fun_seq_header normal_keyword End {
						RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequence::RDOFUNSequenceHeader*>($1);
						RDOFUNSequence* seq = new RDOFUNSequenceNormal( parser, header );
						seq->createCalcs();
						$$ = (int)seq;
					}
					| fun_seq_header normal_keyword INT_CONST End {
						RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequence::RDOFUNSequenceHeader*>($1);
						RDOFUNSequence* seq = new RDOFUNSequenceNormal( parser, header, $3 );
						seq->createCalcs();
						$$ = (int)seq;
					}
					| fun_seq_header normal_keyword INT_CONST error {
						parser->error( @4, "После базы ожидается ключевое слово $End" );
					}
					| fun_seq_header normal_keyword error {
						parser->error( @3, "После типа последовательности ожидается база генератора или ключевое слово $End" );
					};

// ----------------------------------------------------------------------------
// ---------- Гистограмма
// ----------------------------------------------------------------------------
fun_seq_by_hist_header:		fun_seq_header by_hist Body {
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequence::RDOFUNSequenceHeader*>($1);
								$$ = (int)(new RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader( parser, header ));
							}
							| fun_seq_header by_hist INT_CONST Body {
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequence::RDOFUNSequenceHeader*>($1);
								$$ = (int)(new RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader( parser, header, $3 ));
							}
							| fun_seq_header by_hist INT_CONST error {
								parser->error( @4, "После базы ожидается ключевое слово $Body" );
							}
							| fun_seq_header by_hist error {
								parser->error( @3, "После типа последовательности ожидается база генератора или ключевое слово $Body" );
							};

fun_seq_by_hist_body_real:	fun_seq_by_hist_header REAL_CONST REAL_CONST REAL_CONST {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->getType() != RDORTPParamType::pt_real && header->header->getType()->getType() != RDORTPParamType::pt_int ) {
									parser->error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								if ( header->header->getType()->getType() == RDORTPParamType::pt_int ) {
									parser->error( @2, rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->header->src_text().c_str()) );
								}
								rdoRuntime::RDOValue value_from = *((double*)$2);
								rdoRuntime::RDOValue value_to   = *((double*)$3);
								rdoRuntime::RDOValue value_freq = *((double*)$4);
								$$ = (int)(new RDOFUNSequenceByHistReal( parser, header, value_from, value_to, value_freq, @2, @3, @4 ));
							}
							| fun_seq_by_hist_header INT_CONST REAL_CONST REAL_CONST {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->getType() != RDORTPParamType::pt_real && header->header->getType()->getType() != RDORTPParamType::pt_int ) {
									parser->error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								if ( header->header->getType()->getType() == RDORTPParamType::pt_int ) {
									parser->error( @3, rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->header->src_text().c_str()) );
								}
								rdoRuntime::RDOValue value_from = $2;
								rdoRuntime::RDOValue value_to   = *((double*)$3);
								rdoRuntime::RDOValue value_freq = *((double*)$4);
								$$ = (int)(new RDOFUNSequenceByHistReal( parser, header, value_from, value_to, value_freq, @2, @3, @4 ));
							}
							| fun_seq_by_hist_header REAL_CONST INT_CONST REAL_CONST {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->getType() != RDORTPParamType::pt_real && header->header->getType()->getType() != RDORTPParamType::pt_int ) {
									parser->error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								if ( header->header->getType()->getType() == RDORTPParamType::pt_int ) {
									parser->error( @2, rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->header->src_text().c_str()) );
								}
								rdoRuntime::RDOValue value_from = *((double*)$2);
								rdoRuntime::RDOValue value_to   = $3;
								rdoRuntime::RDOValue value_freq = *((double*)$4);
								$$ = (int)(new RDOFUNSequenceByHistReal( parser, header, value_from, value_to, value_freq, @2, @3, @4 ));
							}
							| fun_seq_by_hist_header REAL_CONST REAL_CONST INT_CONST {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->getType() != RDORTPParamType::pt_real && header->header->getType()->getType() != RDORTPParamType::pt_int ) {
									parser->error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								if ( header->header->getType()->getType() == RDORTPParamType::pt_int ) {
									parser->error( @2, rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->header->src_text().c_str()) );
								}
								rdoRuntime::RDOValue value_from = *((double*)$2);
								rdoRuntime::RDOValue value_to   = *((double*)$3);
								rdoRuntime::RDOValue value_freq = $4;
								$$ = (int)(new RDOFUNSequenceByHistReal( parser, header, value_from, value_to, value_freq, @2, @3, @4 ));
							}
							| fun_seq_by_hist_header INT_CONST INT_CONST REAL_CONST {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->getType() != RDORTPParamType::pt_real && header->header->getType()->getType() != RDORTPParamType::pt_int ) {
									parser->error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								rdoRuntime::RDOValue value_from = $2;
								rdoRuntime::RDOValue value_to   = $3;
								rdoRuntime::RDOValue value_freq = *((double*)$4);
								$$ = (int)(new RDOFUNSequenceByHistReal( parser, header, value_from, value_to, value_freq, @2, @3, @4 ));
							}
							| fun_seq_by_hist_header REAL_CONST INT_CONST INT_CONST {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->getType() != RDORTPParamType::pt_real && header->header->getType()->getType() != RDORTPParamType::pt_int ) {
									parser->error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								if ( header->header->getType()->getType() == RDORTPParamType::pt_int ) {
									parser->error( @2, rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->header->src_text().c_str()) );
								}
								rdoRuntime::RDOValue value_from = *((double*)$2);
								rdoRuntime::RDOValue value_to   = $3;
								rdoRuntime::RDOValue value_freq = $4;
								$$ = (int)(new RDOFUNSequenceByHistReal( parser, header, value_from, value_to, value_freq, @2, @3, @4 ));
							}
							| fun_seq_by_hist_header INT_CONST REAL_CONST INT_CONST {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->getType() != RDORTPParamType::pt_real && header->header->getType()->getType() != RDORTPParamType::pt_int ) {
									parser->error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								if ( header->header->getType()->getType() == RDORTPParamType::pt_int ) {
									parser->error( @3, rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->header->src_text().c_str()) );
								}
								rdoRuntime::RDOValue value_from = $2;
								rdoRuntime::RDOValue value_to   = *((double*)$3);
								rdoRuntime::RDOValue value_freq = $4;
								$$ = (int)(new RDOFUNSequenceByHistReal( parser, header, value_from, value_to, value_freq, @2, @3, @4 ));
							}
							| fun_seq_by_hist_header INT_CONST INT_CONST INT_CONST {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->getType() != RDORTPParamType::pt_real && header->header->getType()->getType() != RDORTPParamType::pt_int ) {
									parser->error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								rdoRuntime::RDOValue value_from = $2;
								rdoRuntime::RDOValue value_to   = $3;
								rdoRuntime::RDOValue value_freq = $4;
								$$ = (int)(new RDOFUNSequenceByHistReal( parser, header, value_from, value_to, value_freq, @2, @3, @4 ));
							}
							| fun_seq_by_hist_body_real REAL_CONST REAL_CONST REAL_CONST {
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->getType()->getType() == RDORTPParamType::pt_int ) {
									parser->error( @2, rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->src_text().c_str()) );
								}
								rdoRuntime::RDOValue value_from = *((double*)$2);
								rdoRuntime::RDOValue value_to   = *((double*)$3);
								rdoRuntime::RDOValue value_freq = *((double*)$4);
								reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->addReal( value_from, value_to, value_freq, @2, @3, @4 );
								$$ = $1;
							}
							| fun_seq_by_hist_body_real INT_CONST REAL_CONST REAL_CONST {
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->getType()->getType() == RDORTPParamType::pt_int ) {
									parser->error( @3, rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->src_text().c_str()) );
								}
								rdoRuntime::RDOValue value_from = $2;
								rdoRuntime::RDOValue value_to   = *((double*)$3);
								rdoRuntime::RDOValue value_freq = *((double*)$4);
								reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->addReal( value_from, value_to, value_freq, @2, @3, @4 );
								$$ = $1;
							}
							| fun_seq_by_hist_body_real REAL_CONST INT_CONST REAL_CONST {
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->getType()->getType() == RDORTPParamType::pt_int ) {
									parser->error( @2, rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->src_text().c_str()) );
								}
								rdoRuntime::RDOValue value_from = *((double*)$2);
								rdoRuntime::RDOValue value_to   = $3;
								rdoRuntime::RDOValue value_freq = *((double*)$4);
								reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->addReal( value_from, value_to, value_freq, @2, @3, @4 );
								$$ = $1;
							}
							| fun_seq_by_hist_body_real REAL_CONST REAL_CONST INT_CONST {
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->getType()->getType() == RDORTPParamType::pt_int ) {
									parser->error( @2, rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->src_text().c_str()) );
								}
								rdoRuntime::RDOValue value_from = *((double*)$2);
								rdoRuntime::RDOValue value_to   = *((double*)$3);
								rdoRuntime::RDOValue value_freq = $4;
								reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->addReal( value_from, value_to, value_freq, @2, @3, @4 );
								$$ = $1;
							}
							| fun_seq_by_hist_body_real INT_CONST INT_CONST REAL_CONST {
								rdoRuntime::RDOValue value_from = $2;
								rdoRuntime::RDOValue value_to   = $3;
								rdoRuntime::RDOValue value_freq = *((double*)$4);
								reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->addReal( value_from, value_to, value_freq, @2, @3, @4 );
								$$ = $1;
							}
							| fun_seq_by_hist_body_real REAL_CONST INT_CONST INT_CONST {
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->getType()->getType() == RDORTPParamType::pt_int ) {
									parser->error( @2, rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->src_text().c_str()) );
								}
								rdoRuntime::RDOValue value_from = *((double*)$2);
								rdoRuntime::RDOValue value_to   = $3;
								rdoRuntime::RDOValue value_freq = $4;
								reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->addReal( value_from, value_to, value_freq, @2, @3, @4 );
								$$ = $1;
							}
							| fun_seq_by_hist_body_real INT_CONST REAL_CONST INT_CONST {
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->getType()->getType() == RDORTPParamType::pt_int ) {
									parser->error( @3, rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->src_text().c_str()) );
								}
								rdoRuntime::RDOValue value_from = $2;
								rdoRuntime::RDOValue value_to   = *((double*)$3);
								rdoRuntime::RDOValue value_freq = $4;
								reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->addReal( value_from, value_to, value_freq, @2, @3, @4 );
								$$ = $1;
							}
							| fun_seq_by_hist_body_real INT_CONST INT_CONST INT_CONST {
								rdoRuntime::RDOValue value_from = $2;
								rdoRuntime::RDOValue value_to   = $3;
								rdoRuntime::RDOValue value_freq = $4;
								reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->addReal( value_from, value_to, value_freq, @2, @3, @4 );
								$$ = $1;
							}
							| fun_seq_by_hist_header REAL_CONST error {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->getType() != RDORTPParamType::pt_real && header->header->getType()->getType() != RDORTPParamType::pt_int ) {
									parser->error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								parser->error( @2, @3, "Ожидается конец диапазона" );
							}
							| fun_seq_by_hist_header INT_CONST error {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->getType() != RDORTPParamType::pt_real && header->header->getType()->getType() != RDORTPParamType::pt_int ) {
									parser->error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								parser->error( @2, @3, "Ожидается конец диапазона" );
							}
							| fun_seq_by_hist_header REAL_CONST REAL_CONST error {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->getType() != RDORTPParamType::pt_real && header->header->getType()->getType() != RDORTPParamType::pt_int ) {
									parser->error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								parser->error( @3, @4, "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_header INT_CONST REAL_CONST error {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->getType() != RDORTPParamType::pt_real && header->header->getType()->getType() != RDORTPParamType::pt_int ) {
									parser->error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								parser->error( @3, @4, "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_header REAL_CONST INT_CONST error {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->getType() != RDORTPParamType::pt_real && header->header->getType()->getType() != RDORTPParamType::pt_int ) {
									parser->error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								parser->error( @3, @4, "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_header INT_CONST INT_CONST error {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->getType() != RDORTPParamType::pt_real && header->header->getType()->getType() != RDORTPParamType::pt_int ) {
									parser->error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								parser->error( @3, @4, "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_body_real REAL_CONST error {
								parser->error( @2, @3, "Ожидается конец диапазона" );
							}
							| fun_seq_by_hist_body_real INT_CONST error {
								parser->error( @2, @3, "Ожидается конец диапазона" );
							}
							| fun_seq_by_hist_body_real REAL_CONST REAL_CONST error {
								parser->error( @3, @4, "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_body_real INT_CONST REAL_CONST error {
								parser->error( @3, @4, "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_body_real REAL_CONST INT_CONST error {
								parser->error( @3, @4, "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_body_real INT_CONST INT_CONST error {
								parser->error( @3, @4, "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_header End {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								parser->error( header->src_info(), rdo::format("Последовательность '%s' не должна быть пустой", header->src_text().c_str()) );
							};

fun_seq_by_hist_body_enum:	fun_seq_by_hist_header IDENTIF REAL_CONST {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->getType() != RDORTPParamType::pt_enum ) {
									parser->error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								rdoRuntime::RDOValue value_freq = *((double*)$3);
								$$ = (int)(new RDOFUNSequenceByHistEnum( parser, header, RDOParserSrcInfo(@2, *reinterpret_cast<std::string*>($2)), value_freq, @3 ) );
							}
							| fun_seq_by_hist_header IDENTIF INT_CONST {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->getType() != RDORTPParamType::pt_enum ) {
									parser->error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								rdoRuntime::RDOValue value_freq = $3;
								$$ = (int)(new RDOFUNSequenceByHistEnum( parser, header, RDOParserSrcInfo(@2, *reinterpret_cast<std::string*>($2)), value_freq, @3 ) );
							}
							| fun_seq_by_hist_body_enum IDENTIF REAL_CONST {
								rdoRuntime::RDOValue value_freq = *((double*)$3);
								reinterpret_cast<RDOFUNSequenceByHistEnum*>($1)->addEnum( RDOParserSrcInfo(@2, *reinterpret_cast<std::string*>($2)), value_freq, @3 );
								$$ = $1;
							}
							| fun_seq_by_hist_body_enum IDENTIF INT_CONST {
								rdoRuntime::RDOValue value_freq = $3;
								reinterpret_cast<RDOFUNSequenceByHistEnum*>($1)->addEnum( RDOParserSrcInfo(@2, *reinterpret_cast<std::string*>($2)), value_freq, @3 );
								$$ = $1;
							}
							| fun_seq_by_hist_header IDENTIF error {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->getType()->getType() != RDORTPParamType::pt_enum ) {
									parser->error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
								}
								parser->error( @2, @3, rdo::format("Ожидается относительная вероятность для значения: %s", reinterpret_cast<std::string*>($2)->c_str()) );
							}
							| fun_seq_by_hist_body_enum IDENTIF error {
								parser->error( @2, @3, rdo::format("Ожидается относительная вероятность для значения: %s", reinterpret_cast<std::string*>($2)->c_str()) );
							}
							| fun_seq_by_hist_body_enum REAL_CONST error {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								parser->error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
							}
							| fun_seq_by_hist_body_enum INT_CONST error {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								parser->error( @2, rdo::format("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s", header->header->getType()->src_text().c_str()) );
							};

fun_seq_by_hist:	fun_seq_by_hist_body_real End {
						RDOFUNSequence* seq = reinterpret_cast<RDOFUNSequence*>($1);
						seq->createCalcs();
					}
					| fun_seq_by_hist_body_enum End {
						RDOFUNSequence* seq = reinterpret_cast<RDOFUNSequence*>($1);
						seq->createCalcs();
					}
					| fun_seq_by_hist_body_real error {
						parser->error( @2, "Ошибка в описании последовательности" );
					}
					| fun_seq_by_hist_body_enum error {
						parser->error( @2, "Ошибка в описании последовательности" );
					}
					| fun_seq_by_hist_body_real {
						parser->error( @1, "Ожидается ключевое слово $End" );
					}
					| fun_seq_by_hist_body_enum {
						parser->error( @1, "Ожидается ключевое слово $End" );
					};

// ----------------------------------------------------------------------------
// ---------- Перечень значений
// ----------------------------------------------------------------------------
fun_seq_enumerative_header:	fun_seq_header enumerative Body {
							}
							| fun_seq_header enumerative INT_CONST error {
								parser->error( @3, "У последовательности типа enumerative нет базы генератора" );
							}
							| fun_seq_header enumerative Parameters error {
								parser->error( @3, "У последовательности типа enumerative нет параметров" );
							}
							| fun_seq_header enumerative error {
								parser->error( @3, "После типа последовательности ожидается ключевое слово $Body" );
							};

fun_seq_enumerative_body_int:	fun_seq_enumerative_header INT_CONST {
									RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader*>($1);
									int value = $2;
									switch ( header->getType()->getType() ) {
										case RDORTPParamType::pt_int : header->getType()->checkRSSIntValue( value, @2 ); break;
										case RDORTPParamType::pt_real: parser->error( @2, rdo::format("Последовательность '%s' определена как вещественная, её значения тоже должны быть вещественными", header->src_text().c_str()) );
										case RDORTPParamType::pt_enum: parser->error( @2, rdo::format("Последовательность '%s' определена как перечислимая, её значения тоже должны быть перечислимого типа", header->src_text().c_str()) );
										default                      : parser->error( @1, "Внутренная ошибка парсера: не все типы обработаны" );
									}
									$$ = (int)new RDOFUNSequenceEnumerativeInt( parser, header, value );
								}
								| fun_seq_enumerative_body_int INT_CONST {
									RDOFUNSequenceEnumerativeInt* seq = reinterpret_cast<RDOFUNSequenceEnumerativeInt*>($1);
									int value = $2;
									switch ( seq->header->getType()->getType() ) {
										case RDORTPParamType::pt_int : seq->header->getType()->checkRSSIntValue( value, @2 ); break;
										default                      : parser->error( @1, "Внутренная ошибка парсера: fun_seq_enumerative_body_int INT_CONST" );
									}
									seq->addInt( value );
								}
								| fun_seq_enumerative_body_int error {
									parser->error( @1, @2, "Ожидается целое число или ключевое слово $End" );
								};

fun_seq_enumerative_body_real:	fun_seq_enumerative_header REAL_CONST {
									RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader*>($1);
									double value = *reinterpret_cast<double*>($2);
									switch ( header->getType()->getType() ) {
										case RDORTPParamType::pt_int : parser->error( @2, rdo::format("Последовательность '%s' определена как целочисленная, её значения тоже должны быть челочисленными", header->src_text().c_str()) );
										case RDORTPParamType::pt_real: header->getType()->checkRSSRealValue( value, @2 ); break;
										case RDORTPParamType::pt_enum: parser->error( @2, rdo::format("Последовательность '%s' определена как перечислимая, её значения тоже должны быть перечислимого типа", header->src_text().c_str()) );
										default                      : parser->error( @1, "Внутренная ошибка парсера: не все типы обработаны" );
									}
									$$ = (int)new RDOFUNSequenceEnumerativeReal( parser, header, value );
								}
								| fun_seq_enumerative_body_real REAL_CONST {
									RDOFUNSequenceEnumerativeReal* seq = reinterpret_cast<RDOFUNSequenceEnumerativeReal*>($1);
									double value = *reinterpret_cast<double*>($2);
									switch ( seq->header->getType()->getType() ) {
										case RDORTPParamType::pt_real: seq->header->getType()->checkRSSRealValue( value, @2 ); break;
										default                      : parser->error( @1, "Внутренная ошибка парсера: fun_seq_enumerative_body_real REAL_CONST" );
									}
									seq->addReal( value );
								}
								| fun_seq_enumerative_body_real error {
									parser->error( @1, @2, "Ожидается вещественное число или ключевое слово $End" );
								};

fun_seq_enumerative_body_enum:	fun_seq_enumerative_header IDENTIF {
									RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader*>($1);
									if ( header->getType()->getType() != RDORTPParamType::pt_enum ) {
										switch ( header->getType()->getType() ) {
											case RDORTPParamType::pt_int : parser->error( @2, rdo::format("Последовательность '%s' определена как целочисленная, её значения тоже должны быть челочисленными", header->src_text().c_str()) );
											case RDORTPParamType::pt_real: parser->error( @2, rdo::format("Последовательность '%s' определена как вещественная, её значения тоже должны быть вещественными", header->src_text().c_str()) );
											case RDORTPParamType::pt_enum: break;
											default                      : parser->error( @1, "Внутренная ошибка парсера: не все типы обработаны" );
										}
									}
									std::string value = *reinterpret_cast<std::string*>($2);
									$$ = (int)new RDOFUNSequenceEnumerativeEnum( parser, header, RDOParserSrcInfo(@2, value) );
								}
								| fun_seq_enumerative_body_enum IDENTIF {
									RDOFUNSequenceEnumerativeEnum* seq = reinterpret_cast<RDOFUNSequenceEnumerativeEnum*>($1);
									std::string value = *reinterpret_cast<std::string*>($2);
									seq->addEnum( RDOParserSrcInfo(@2, value) );
								}
								| fun_seq_enumerative_body_enum error {
									parser->error( @1, @2, "Ожидается элемент перечислимого типа или ключевое слово $End" );
								};

fun_seq_enumerative:	fun_seq_enumerative_body_int End {
							RDOFUNSequence* seq = reinterpret_cast<RDOFUNSequence*>($1);
							seq->createCalcs();
						}
						| fun_seq_enumerative_body_real End {
							RDOFUNSequence* seq = reinterpret_cast<RDOFUNSequence*>($1);
							seq->createCalcs();
						}
						| fun_seq_enumerative_body_enum End {
							RDOFUNSequence* seq = reinterpret_cast<RDOFUNSequence*>($1);
							seq->createCalcs();
						}
						| fun_seq_enumerative_header End {
							RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader*>($1);
							parser->error( header->src_info(), rdo::format("Последовательность '%s' не должна быть пустой", header->src_text().c_str()) );
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

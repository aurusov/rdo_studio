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

#include "rdoparser.h"
#include "rdoparser_rdo.h"
#include "rdortp.h"
#include "rdofun.h"
#include "rdoruntime.h"

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
				parser->lexer_loc_set( &(@1) );
				if ( parser->hasConstant() ) {
					parser->error( "Ожидается описание функции или последовательности" );
				} else {
					parser->error( "Ожидается описание функции, последовательности или константы" );
				}
			};

fun_consts:	/* empty */
			| Constant fun_const_body End
			| Constant fun_const_body {
				parser->lexer_loc_set( @2.last_line, @2.last_column );
				parser->error( "После описания констант ожидается ключевое слово $End" );
			};

fun_const_body:	/* empty */
				| fun_const_body fun_const_param_desc {
					RDORTPParamDesc* cons = (RDORTPParamDesc*)$2;
					parser->addConstant(cons);
				}
				| fun_const_body error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "Ожидается описание константы" );
				};

fun_const_param_desc:	IDENTIF_COLON fun_param_type {
							std::string* name = (std::string*)$1;
							if ( parser->findFUNConst( name ) ) {
								parser->lexer_loc_set( &(@1) );
								parser->error( rdo::format("Константа с таким именем уже существует: %s", name->c_str()) );
//								parser->error("Second appearance of the same constant name: " + *(_cons->getName()));
							}
							RDORTPResParam*  parType = (RDORTPResParam*)$2;
							RDORTPParamDesc* param   = new RDORTPParamDesc( name, parType );
							if ( !parType->dv->exist ) {
								parser->lexer_loc_set( &(@2) );
								parser->error( "Константа должна иметь значение" );
//								parser->error( "Constant must have value" );
							}
							$$ = (int)param;
						}
						| IDENTIF_COLON {
							parser->lexer_loc_set( &(@1) );
							parser->error( "Ожидается тип константы" );
						}
						| IDENTIF_COLON error {
							parser->lexer_loc_set( &(@2) );
							parser->error( "Неверный тип константы" );
						};

// ----------------------------------------------------------------------------
// ---------- Описание параметров
// ----------------------------------------------------------------------------
fun_param_type: integer fun_int_diap fun_int_default_val {
					RDORTPIntDiap *diap = (RDORTPIntDiap *)$2;
					RDORTPIntDefVal *dv = (RDORTPIntDefVal *)$3;
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@3) );
					RDORTPIntResParam *rp = new RDORTPIntResParam(diap, dv);
					parser->lexer_loc_restore();
					$$ = (int)rp;
				}
				| integer fun_int_diap {
					RDORTPIntDiap *diap = (RDORTPIntDiap *)$2;
					RDORTPIntDefVal *dv = new RDORTPIntDefVal();
					RDORTPIntResParam *rp = new RDORTPIntResParam(diap, dv);
					$$ = (int)rp;
				}
				| real fun_real_diap fun_real_default_val {
					RDORTPRealDiap *diap = (RDORTPRealDiap *)$2;
					RDORTPRealDefVal *dv = (RDORTPRealDefVal *)$3;
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@3) );
					RDORTPRealResParam *rp = new RDORTPRealResParam(diap, dv);
					parser->lexer_loc_restore();
					$$ = (int)rp;
				}
				| real fun_real_diap {
					RDORTPRealDiap *diap = (RDORTPRealDiap *)$2;
					RDORTPRealDefVal *dv = new RDORTPRealDefVal();
					RDORTPRealResParam *rp = new RDORTPRealResParam(diap, dv);
					$$ = (int)rp;
				}
				| fun_enum fun_enum_default_val {
					reinterpret_cast<RDOLexerFUN*>(lexer)->enum_param_cnt = 0;
					RDORTPEnum *enu = (RDORTPEnum *)$1;
					RDORTPEnumDefVal *dv = (RDORTPEnumDefVal *)$2;
					enu->findValue(dv->value);	 // if no value - Syntax exception will be thrown
					RDORTPEnumResParam *rp = new RDORTPEnumResParam(enu, dv);
					$$ = (int)rp;
				}
				| fun_enum {
					reinterpret_cast<RDOLexerFUN*>(lexer)->enum_param_cnt = 0;
					RDORTPEnum *enu = (RDORTPEnum *)$1;
					RDORTPEnumDefVal *dv = new RDORTPEnumDefVal();
					RDORTPEnumResParam *rp = new RDORTPEnumResParam(enu, dv);
					$$ = (int)rp;
				}
				| fun_such_as {
					RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
					$$ = (int)desc->getType()->constructSuchAs();
				}
				| fun_such_as fun_int_default_val {
					RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
					RDORTPIntDefVal *dv = (RDORTPIntDefVal *)$2;
					$$ = (int)desc->getType()->constructSuchAs((int)dv->val);
				}
				| fun_such_as	fun_real_default_val {
					RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
					RDORTPRealDefVal *dv = (RDORTPRealDefVal *)$2;
					if(!dv->exist)
						$$ = (int)desc->getType()->constructSuchAs();
					else
						$$ = (int)desc->getType()->constructSuchAs((double *)&(dv->val));
				}
				| fun_such_as fun_enum_default_val {
					RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
					RDORTPEnumDefVal *dv = (RDORTPEnumDefVal *)$2;
					if ( !dv->exist ) {
						$$ = (int)desc->getType()->constructSuchAs();
					} else {
						parser->lexer_loc_backup();
						parser->lexer_loc_set( &(@2) );
						$$ = (int)desc->getType()->constructSuchAs((std::string *)dv->value);
						parser->lexer_loc_restore();
					}
				}
				| fun_such_as '=' error {
					parser->error( "Ожидается зачение по-умолчанию" );
				}
				| fun_such_as error {
					parser->error( "Ожидается окончание описания ссылки, например, зачение по-умолчанию" );
				}
				| integer error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "Ошибка после ключевого слова integer. Возможно, не хватает значения по-умолчанию." );
				}
				| real error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "Ошибка после ключевого слова real. Возможно, не хватает значения по-умолчанию." );
				}
				| fun_enum error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "Ошибка после перечислимого типа. Возможно, не хватает значения по-умолчанию." );
				};

fun_int_diap:	/* empty */ {
					RDORTPIntDiap *diap = new RDORTPIntDiap();
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint INT_CONST ']' {
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@2) );
					RDORTPIntDiap *diap = new RDORTPIntDiap($2, $4);
					parser->lexer_loc_restore();
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint REAL_CONST {
					parser->lexer_loc_set( &(@2) );
					parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' REAL_CONST dblpoint INT_CONST {
					parser->lexer_loc_set( &(@2) );
					parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' INT_CONST dblpoint REAL_CONST {
					parser->lexer_loc_set( &(@4) );
					parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' INT_CONST dblpoint INT_CONST error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "Диапазон задан неверно" );
				}
				| '[' INT_CONST dblpoint error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "Диапазон задан неверно" );
				}
				| '[' error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "Диапазон задан неверно" );
				};

fun_real_diap: /* empty */ {
					RDORTPRealDiap *diap = new RDORTPRealDiap();
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint REAL_CONST ']' {
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@2) );
					double min = *((double *)$2);
					double max = *((double *)$4);
					RDORTPRealDiap *diap = new RDORTPRealDiap(min, max);
					parser->lexer_loc_restore();
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint INT_CONST ']' {
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@2) );
					double min = *((double *)$2);
					double max = $4;
					RDORTPRealDiap *diap = new RDORTPRealDiap(min, max);
					parser->lexer_loc_restore();
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint REAL_CONST ']' {
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@2) );
					double min = $2;
					double max = *((double *)$4);
					RDORTPRealDiap *diap = new RDORTPRealDiap(min, max);
					parser->lexer_loc_restore();
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint INT_CONST ']' {
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@2) );
					double min = $2;
					double max = $4;
					RDORTPRealDiap *diap = new RDORTPRealDiap(min, max);
					parser->lexer_loc_restore();
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint REAL_CONST error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "Диапазон задан неверно" );
				}
				| '[' REAL_CONST dblpoint INT_CONST error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "Диапазон задан неверно" );
				}
				| '[' INT_CONST dblpoint REAL_CONST error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "Диапазон задан неверно" );
				}
				| '[' INT_CONST dblpoint INT_CONST error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "Диапазон задан неверно" );
				}
				| '[' REAL_CONST dblpoint error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "Диапазон задан неверно" );
				}
				| '[' INT_CONST dblpoint error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "Диапазон задан неверно" );
				}
				| '[' error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "Диапазон задан неверно" );
				};

fun_enum:	'(' fun_enum_item ')' {
				$$ = $2;
			}
			| '(' fun_enum_item {
				parser->lexer_loc_set( &(@2) );
				parser->error( "Перечисление должно заканчиваться скобкой" );
			};

fun_enum_item:	IDENTIF {
					RDORTPEnum *enu = new RDORTPEnum((std::string *)$1);
					$$ = (int)enu;
					reinterpret_cast<RDOLexerFUN*>(lexer)->enum_param_cnt = 1;
				}
				| fun_enum_item ',' IDENTIF {
					if ( reinterpret_cast<RDOLexerFUN*>(lexer)->enum_param_cnt >= 1 ) {
						parser->lexer_loc_backup();
						parser->lexer_loc_set( &(@3) );
						RDORTPEnum *enu = (RDORTPEnum *)$1;
						enu->add((std::string *)$3);
						parser->lexer_loc_restore();
						$$ = (int)enu;
					} else {
						parser->error( "Ошибка в описании значений перечислимого типа" );
					}
				}
				| fun_enum_item IDENTIF {
					if ( reinterpret_cast<RDOLexerFUN*>(lexer)->enum_param_cnt >= 1 ) {
						parser->error( rdo::format("Пропущена запятая перед: %s", ((std::string*)$2)->c_str()) );
					} else {
						parser->error( "Ошибка в описании значений перечислимого типа" );
					}
				}
				| fun_enum_item error {
					std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
					if ( str.empty() ) {
						parser->lexer_loc_set( &(@1) );
						parser->error( "Ошибка в описании значений перечислимого типа" );
					} else {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdo::format( "Неверное значение перечислимого типа: %s", str.c_str() ) );
					}
				}
				| fun_enum_item ',' INT_CONST {
					parser->lexer_loc_set( &(@3) );
					parser->error( "Значение перечислимого типа не может начинаться с цифры" );
				}
				| fun_enum_item ',' REAL_CONST {
					parser->lexer_loc_set( &(@3) );
					parser->error( "Значение перечислимого типа не может начинаться с цифры" );
				}
				| INT_CONST {
					parser->lexer_loc_set( &(@1) );
					parser->error( "Значение перечислимого типа не может начинаться с цифры" );
				}
				| REAL_CONST {
					parser->lexer_loc_set( &(@1) );
					parser->error( "Значение перечислимого типа не может начинаться с цифры" );
				}
				| error {
					parser->lexer_loc_set( &(@1) );
					parser->error( "Ошибка в описании значений перечислимого типа" );
				};

fun_such_as:	such_as IDENTIF '.' IDENTIF {
					std::string* type = (std::string *)$2;
					std::string* param = (std::string *)$4;
					const RDORTPResType *const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type->c_str() );
					}
					const RDORTPParamDesc *const rp = rt->findRTPParam( param );
					if ( !rp ) {
						parser->lexer_loc_set( &(@4) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_PARAM, type->c_str(), param->c_str() );
					}
					$$ = (int)rp;
				}
				| such_as IDENTIF {
					std::string *constName = (std::string *)$2;
					const RDOFUNConstant *const cons = parser->findFUNConst(constName);
					if ( !cons ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdo::format("Ссылка на несужествующую константу: %s", constName->c_str()) );
					}
					$$ = (int)(cons->descr);
				}
				| such_as IDENTIF '.' {
					std::string* type = (std::string *)$2;
					const RDORTPResType *const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type->c_str() );
					} else {
						parser->lexer_loc_set( &(@3) );
						parser->error( "Не указан параметр" );
					}
				}
				| such_as IDENTIF '.' error {
					std::string* type = (std::string *)$2;
					const RDORTPResType *const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type->c_str() );
					} else {
						parser->lexer_loc_set( &(@4) );
						parser->error( "Ошибка при указании параметра" );
					}
				}
				| such_as IDENTIF error {
					std::string* type = (std::string *)$2;
					const RDORTPResType *const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type->c_str() );
					} else {
						parser->lexer_loc_set( &(@2) );
						parser->error( "После имени типа должен быть указан параметр через точку" );
					}
				}
				| such_as error {
					if ( @1.last_line == @2.first_line ) {
						parser->lexer_loc_set( @2.first_line, @2.first_column );
					} else {
						parser->lexer_loc_set( &(@1) );
					}
					parser->error( "После ключевого слова such_as необходимо указать тип и парамтер ресурса для ссылки" );
				};

fun_int_default_val:	'=' INT_CONST {
						RDORTPIntDefVal *dv = new RDORTPIntDefVal($2);
						$$ = (int)dv;
					}
					| '=' REAL_CONST {
						// Целое число инициализируется вещественным: %f
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_DEFVAULT_INT_AS_REAL, *(double*)$2 );
					}
					| '=' {
						parser->lexer_loc_set( &(@1) );
						parser->error( "Не указано значение по-умолчанию целого типа" );
					}
					| '=' error {
						parser->lexer_loc_set( &(@2) );
						parser->error( "Неверное значение по-умолчанию целого типа" );
					};

fun_real_default_val:	'=' REAL_CONST {
						$$ = (int)(new RDORTPRealDefVal(*((double *)$2)));
					}
					| '=' INT_CONST {
						$$ = (int)(new RDORTPRealDefVal($2));
					}
					| '=' {
						parser->lexer_loc_set( &(@1) );
						parser->error( "Не указано значение по-умолчанию вещественного типа" );
					}
					| '=' error {
						parser->lexer_loc_set( &(@2) );
						parser->error( "Неверное значение по-умолчанию вещественного типа" );
					};

fun_enum_default_val:	'=' IDENTIF {
						std::string *val = (std::string *)$2;
						RDORTPEnumDefVal* dv = new RDORTPEnumDefVal(val);
						$$ = (int)dv;
					}
					| '=' {
						parser->lexer_loc_set( &(@1) );
						parser->error( "Не указано значение по-умолчанию перечислимого типа" );
					}
					| '=' error {
						parser->lexer_loc_set( &(@2) );
						parser->error( "Неверное значение по-умолчанию перечислимого типа" );
					};

fun_func_seq:	/* empty */
			| fun_func_seq fun_func_descr
			| fun_func_seq fun_seq_descr
			| fun_func_seq Constant {
				parser->lexer_loc_set( &(@2) );
				parser->error( "Константы долны быть описаны первыми, перед функциями и последовательностями" );
			};

fun_func_descr:	fun_func_header fun_func_footer;

fun_func_header:	Function_keyword IDENTIF_COLON fun_param_type	{
						std::string* name = (std::string*)$2;
						if ( parser->findFunction(name) ) {
							parser->lexer_loc_set( &(@2) );
							parser->error( rdo::format( "Функция уже существует: %s", name->c_str() ) );
//							parser->error(("Second appearance of the same function: " + *(name)).c_str());
						}
						if ( parser->findSequence(name) ) {
							parser->lexer_loc_set( &(@2) );
							parser->error( rdo::format( "Существует последовательность с таким же именем: %s", name->c_str() ) );
						}
						RDORTPResParam *retType = (RDORTPResParam *)$3;
						RDOFUNFunction *fun = new RDOFUNFunction(name, retType);
						$$ = (int)fun;
					};

fun_func_footer:	Type_keyword '=' algorithmic Parameters fun_func_params Body fun_func_algorithmic_body End {
						RDOFUNFunction *currFunc = parser->getLastFUNFunction();
						currFunc->createAlgorithmicCalc();
					}
					| Type_keyword '=' list_keyword Parameters fun_func_params Body fun_func_list_body End {
						RDOFUNFunction *currFunc = parser->getLastFUNFunction();
						currFunc->createListCalc();
					}
					| Type_keyword '=' table_keyword Parameters fun_func_params Body fun_func_list_body End {
						RDOFUNFunction *currFunc = parser->getLastFUNFunction();
						currFunc->createTableCalc();
					}
					| Type_keyword '=' algorithmic Parameters fun_func_params Body fun_func_algorithmic_body error {
						parser->lexer_loc_set( @8.first_line, @8.first_column );
						parser->error( "Ожидается ключевое слово $End" );
					}
					| Type_keyword '=' list_keyword Parameters fun_func_params Body fun_func_list_body error {
						parser->lexer_loc_set( @8.first_line, @8.first_column );
						parser->error( "Ожидается ключевое слово $End" );
					}
					| Type_keyword '=' table_keyword Parameters fun_func_params Body fun_func_list_body error {
						parser->lexer_loc_set( @8.first_line, @8.first_column );
						parser->error( "Ожидается ключевое слово $End" );
					}
					| Type_keyword '=' algorithmic Parameters error {
						parser->lexer_loc_set( @5.first_line, @5.first_column );
						parser->error( "Ожидается список параметров" );
					}
					| Type_keyword '=' list_keyword Parameters error {
						parser->lexer_loc_set( @5.first_line, @5.first_column );
						parser->error( "Ожидается список параметров" );
					}
					| Type_keyword '=' table_keyword Parameters error {
						parser->lexer_loc_set( @5.first_line, @5.first_column );
						parser->error( "Ожидается список параметров" );
					}
					| Type_keyword '=' algorithmic error {
						parser->lexer_loc_set( @4.first_line, @4.first_column );
						parser->error( "Ожидается ключевое слово $Parameters" );
					}
					| Type_keyword '=' list_keyword error {
						parser->lexer_loc_set( @4.first_line, @4.first_column );
						parser->error( "Ожидается ключевое слово $Parameters" );
					}
					| Type_keyword '=' table_keyword error {
						parser->lexer_loc_set( @4.first_line, @4.first_column );
						parser->error( "Ожидается ключевое слово $Parameters" );
					}
					| Type_keyword '=' error {
						parser->lexer_loc_set( &(@3) );
						parser->error( "Неизвестный тип функции" );
					}
					| Type_keyword error {
						parser->lexer_loc_set( &(@1) );
						parser->error( "Ожидается тип функции" );
					};

fun_func_params:	/* empty */
			|	fun_func_params IDENTIF_COLON fun_param_type	{
				std::string*         name  = (std::string *)$2;
				RDORTPResParam*      type  = (RDORTPResParam *)$3;
				RDOFUNFunctionParam* param = new RDOFUNFunctionParam(name, type);
				param->setErrorPos( @2 );
				parser->getLastFUNFunction()->add(param);	// the function must check uniquness of parameters names
				$$ = (int)param;
			};

fun_func_list_body:	/* empty */
			| fun_func_list_body fun_std_value;

fun_std_value:	IDENTIF {
					RDOFUNFunctionListElementIdentif *value = new RDOFUNFunctionListElementIdentif((std::string *)$1);
					parser->getLastFUNFunction()->add(value);
					value->setErrorPos( @1 );
					$$ = (int)value;
				}
				| REAL_CONST {
					RDOFUNFunctionListElementReal *value = new RDOFUNFunctionListElementReal((double *)$1);
					parser->getLastFUNFunction()->add(value);
					value->setErrorPos( @1 );
					$$ = (int)value;
				}
				| INT_CONST {
					RDOFUNFunctionListElementInt *value = new RDOFUNFunctionListElementInt((int)$1);
					parser->getLastFUNFunction()->add(value);
					value->setErrorPos( @1 );
					$$ = (int)value;
				}
				| '=' {
					RDOFUNFunctionListElementEq *value = new RDOFUNFunctionListElementEq();
					parser->getLastFUNFunction()->add(value);
					value->setErrorPos( @1 );
					$$ = (int)value;
				};

fun_func_algorithmic_body:	/* empty */
			| fun_func_algorithmic_body fun_func_algorithmic_calc_if {
				parser->getLastFUNFunction()->add((RDOFUNCalculateIf*)$2);
			};

fun_func_algorithmic_calc_if:	Calculate_if fun_logic IDENTIF '=' fun_arithm {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@3) );
									$$ = (int)(new RDOFUNCalculateIf((RDOFUNLogic *)$2, (std::string *)$3, (RDOFUNArithm *)$5));
									parser->lexer_loc_restore();
								}
								| Calculate_if fun_logic error {
									parser->lexer_loc_set( &(@2) );
									parser->error( "Ожидается <имя_функции> = <результат_функции>" );
								}
								| error {
									parser->lexer_loc_set( &(@1) );
									parser->error( "Ожидается ключевое слово Calculate_if" );
								};

fun_seq_descr:	fun_seq_uniform
				| fun_seq_exponential
				| fun_seq_normal
				| fun_seq_by_hist
				| fun_seq_enumerative;

fun_seq_header:	Sequence IDENTIF_COLON fun_param_type Type_keyword '=' {
					std::string* name = (std::string*)$2;
					if ( parser->findSequence(name) ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdo::format( "Последовательность уже существует: %s", name->c_str() ) );
					}
					if ( parser->findFunction(name) ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdo::format( "Существует функция с таким же именем: %s", name->c_str() ) );
					}
					$$ = (int)(new RDOFUNSequenceHeader( name, (RDORTPResParam *)$3) );
				}
				| Sequence IDENTIF_COLON fun_param_type Type_keyword '=' error {
					parser->lexer_loc_set( &(@5), &(@6) );
					parser->error( "Ожидается тип последовательности" );
				}
				| Sequence IDENTIF_COLON fun_param_type Type_keyword error {
					parser->lexer_loc_set( &(@4), &(@5) );
					parser->error( "Ожидается тип последовательности в формате '=' <тип>" );
				}
				| Sequence IDENTIF_COLON fun_param_type error {
					parser->lexer_loc_set( @4.first_line, @4.first_column );
					parser->error( "Ожидается ключевое слово $Type" );
				}
				| Sequence IDENTIF_COLON error {
					parser->lexer_loc_set( &(@2), &(@3) );
					parser->error( "Ожидается тип возвращаемого значения" );
				}
				| Sequence error {
					parser->lexer_loc_set( &(@1) );
					parser->error( "Ожидается имя последовательности" );
				};

fun_seq_uniform:	fun_seq_header uniform End {
						$$ = (int)(new RDOFUNSequenceUniform((RDOFUNSequenceHeader *)$1));
					}
					| fun_seq_header uniform INT_CONST End {
						$$ = (int)(new RDOFUNSequenceUniform((RDOFUNSequenceHeader *)$1, $3));
					}
					| fun_seq_header uniform INT_CONST error {
						parser->lexer_loc_set( @4.first_line, @4.first_column );
						parser->error( "Ожидается ключевое слово $End" );
					}
					| fun_seq_header uniform error {
						parser->lexer_loc_set( &(@2), &(@3) );
						parser->error( "Ожидается база генератора" );
					};

fun_seq_exponential:	fun_seq_header exponential End {
							$$ = (int)(new RDOFUNSequenceExponential((RDOFUNSequenceHeader *)$1));
						}
						| fun_seq_header exponential INT_CONST End {
							$$ = (int)(new RDOFUNSequenceExponential((RDOFUNSequenceHeader *)$1, $3));
						}
						| fun_seq_header exponential INT_CONST error {
							parser->lexer_loc_set( @4.first_line, @4.first_column );
							parser->error( "Ожидается ключевое слово $End" );
						}
						| fun_seq_header exponential error {
							parser->lexer_loc_set( &(@2), &(@3) );
							parser->error( "Ожидается база генератора" );
						};

fun_seq_normal:	fun_seq_header normal_keyword End {
					$$ = (int)(new RDOFUNSequenceNormal((RDOFUNSequenceHeader *)$1));
				}
				| fun_seq_header normal_keyword INT_CONST End {
					$$ = (int)(new RDOFUNSequenceNormal((RDOFUNSequenceHeader *)$1, $3));
				}
				| fun_seq_header normal_keyword INT_CONST error {
					parser->lexer_loc_set( @4.first_line, @4.first_column );
					parser->error( "Ожидается ключевое слово $End" );
				}
				| fun_seq_header normal_keyword error {
					parser->lexer_loc_set( &(@2), &(@3) );
					parser->error( "Ожидается база генератора" );
				};

fun_seq_by_hist_header:	fun_seq_header by_hist Body {
							$$ = (int)(new RDOFUNSequenceByHistHeader((RDOFUNSequenceHeader *)$1));
						}
						| fun_seq_header by_hist INT_CONST Body {
							$$ = (int)(new RDOFUNSequenceByHistHeader((RDOFUNSequenceHeader *)$1, $3));
						}
						| fun_seq_header by_hist error Body {
							parser->lexer_loc_set( &(@2), &(@3) );
							parser->error( "Ожидается база генератора" );
						}
						| fun_seq_header by_hist INT_CONST error {
							parser->lexer_loc_set( &(@3), &(@4) );
							parser->error( "Ожидается ключевое слово $Body" );
						}
						| fun_seq_header by_hist error {
							parser->lexer_loc_set( &(@2), &(@3) );
							parser->error( "Ожидается ключевое слово $Body" );
						};

fun_seq_by_hist_body_real:	fun_seq_by_hist_header REAL_CONST REAL_CONST REAL_CONST {
								RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPResParam::pt_real && header->header->type->getType() != RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "Значение не соответствует типу последовательности" );
								}
								if ( header->header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->header->name->c_str()) );
								}
								if ( *((double*)$2) > *((double*)$3) ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "Начало интервала должно быть меньше его конца" );
								}
								if ( *((double*)$4) <= 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "Относительная вероятность должна быть больше нуля" );
								}
								$$ = (int)(new RDOFUNSequenceByHistReal( header, *((double*)$2), *((double*)$3), *((double*)$4)) );
							}
							| fun_seq_by_hist_header INT_CONST REAL_CONST REAL_CONST {
								RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPResParam::pt_real && header->header->type->getType() != RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "Значение не соответствует типу последовательности" );
								}
								if ( header->header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->header->name->c_str()) );
								}
								if ( $2 > *((double*)$3) ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "Начало интервала должно быть меньше его конца" );
								}
								if ( *((double*)$4) <= 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "Относительная вероятность должна быть больше нуля" );
								}
								$$ = (int)(new RDOFUNSequenceByHistReal( header, $2, *((double*)$3), *((double*)$4)) );
							}
							| fun_seq_by_hist_header REAL_CONST INT_CONST REAL_CONST {
								RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPResParam::pt_real && header->header->type->getType() != RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "Значение не соответствует типу последовательности" );
								}
								if ( header->header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->header->name->c_str()) );
								}
								if ( *((double*)$2) > $3 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "Начало интервала должно быть меньше его конца" );
								}
								if ( *((double*)$4) <= 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "Относительная вероятность должна быть больше нуля" );
								}
								$$ = (int)(new RDOFUNSequenceByHistReal( header, *((double*)$2), $3, *((double*)$4)) );
							}
							| fun_seq_by_hist_header REAL_CONST REAL_CONST INT_CONST {
								RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPResParam::pt_real && header->header->type->getType() != RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "Значение не соответствует типу последовательности" );
								}
								if ( header->header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->header->name->c_str()) );
								}
								if ( *((double*)$2) > *((double*)$3) ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "Начало интервала должно быть меньше его конца" );
								}
								if ( $4 <= 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "Относительная вероятность должна быть больше нуля" );
								}
								$$ = (int)(new RDOFUNSequenceByHistReal( header, *((double*)$2), *((double*)$3), $4) );
							}
							| fun_seq_by_hist_header INT_CONST INT_CONST REAL_CONST {
								RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPResParam::pt_real && header->header->type->getType() != RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "Значение не соответствует типу последовательности" );
								}
								if ( $2 > $3 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "Начало интервала должно быть меньше его конца" );
								}
								if ( *((double*)$4) <= 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "Относительная вероятность должна быть больше нуля" );
								}
								$$ = (int)(new RDOFUNSequenceByHistReal( (RDOFUNSequenceByHistHeader *)$1, $2, $3, *((double*)$4)) );
							}
							| fun_seq_by_hist_header REAL_CONST INT_CONST INT_CONST {
								RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPResParam::pt_real && header->header->type->getType() != RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "Значение не соответствует типу последовательности" );
								}
								if ( header->header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->header->name->c_str()) );
								}
								if ( *((double*)$2) > $3 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "Начало интервала должно быть меньше его конца" );
								}
								if ( $4 <= 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "Относительная вероятность должна быть больше нуля" );
								}
								$$ = (int)(new RDOFUNSequenceByHistReal( header, *((double*)$2), $3, $4) );
							}
							| fun_seq_by_hist_header INT_CONST REAL_CONST INT_CONST {
								RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPResParam::pt_real && header->header->type->getType() != RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "Значение не соответствует типу последовательности" );
								}
								if ( header->header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->header->name->c_str()) );
								}
								if ( $2 > *((double*)$3) ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "Начало интервала должно быть меньше его конца" );
								}
								if ( $4 <= 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "Относительная вероятность должна быть больше нуля" );
								}
								$$ = (int)(new RDOFUNSequenceByHistReal( header, $2, *((double*)$3), $4) );
							}
							| fun_seq_by_hist_header INT_CONST INT_CONST INT_CONST {
								RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPResParam::pt_real && header->header->type->getType() != RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "Значение не соответствует типу последовательности" );
								}
								if ( $2 > $3 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "Начало интервала должно быть меньше его конца" );
								}
								if ( $4 <= 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "Относительная вероятность должна быть больше нуля" );
								}
								$$ = (int)(new RDOFUNSequenceByHistReal((RDOFUNSequenceByHistHeader *)$1, $2, $3, $4));
							}
							| fun_seq_by_hist_body_real REAL_CONST REAL_CONST REAL_CONST {
								RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->name->c_str()) );
								}
								if ( *((double*)$2) != ((RDOFUNSequenceByHistReal *)$1)->lastTo() ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "Начало интервала должно совпадать с окончанием предыдущего" );
								}
								if ( *((double*)$2) > *((double*)$3) ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "Начало интервала должно быть меньше его конца" );
								}
								if ( *((double*)$4) <= 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "Относительная вероятность должна быть больше нуля" );
								}
								((RDOFUNSequenceByHistReal *)$1)->addReal(*((double*)$2), *((double*)$3), *((double*)$4)); $$ = $1;
							}
							| fun_seq_by_hist_body_real INT_CONST REAL_CONST REAL_CONST {
								RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->name->c_str()) );
								}
								if ( $2 != ((RDOFUNSequenceByHistReal *)$1)->lastTo() ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "Начало интервала должно совпадать с окончанием предыдущего" );
								}
								if ( $2 > *((double*)$3) ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "Начало интервала должно быть меньше его конца" );
								}
								if ( *((double*)$4) <= 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "Относительная вероятность должна быть больше нуля" );
								}
								((RDOFUNSequenceByHistReal *)$1)->addReal($2, *((double*)$3), *((double*)$4)); $$ = $1;
							}
							| fun_seq_by_hist_body_real REAL_CONST INT_CONST REAL_CONST {
								RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->name->c_str()) );
								}
								if ( *((double*)$2) != ((RDOFUNSequenceByHistReal *)$1)->lastTo() ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "Начало интервала должно совпадать с окончанием предыдущего" );
								}
								if ( *((double*)$2) > $3 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "Начало интервала должно быть меньше его конца" );
								}
								if ( *((double*)$4) <= 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "Относительная вероятность должна быть больше нуля" );
								}
								((RDOFUNSequenceByHistReal *)$1)->addReal(*((double*)$2), $3, *((double*)$4)); $$ = $1;
							}
							| fun_seq_by_hist_body_real REAL_CONST REAL_CONST INT_CONST {
								RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->name->c_str()) );
								}
								if ( *((double*)$2) != ((RDOFUNSequenceByHistReal *)$1)->lastTo() ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "Начало интервала должно совпадать с окончанием предыдущего" );
								}
								if ( *((double*)$2) > *((double*)$3) ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "Начало интервала должно быть меньше его конца" );
								}
								if ( $4 <= 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "Относительная вероятность должна быть больше нуля" );
								}
								((RDOFUNSequenceByHistReal *)$1)->addReal(*((double*)$2), *((double*)$3), $4); $$ = $1;
							}
							| fun_seq_by_hist_body_real INT_CONST INT_CONST REAL_CONST {
								if ( $2 != ((RDOFUNSequenceByHistReal *)$1)->lastTo() ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "Начало интервала должно совпадать с окончанием предыдущего" );
								}
								if ( $2 > $3 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "Начало интервала должно быть меньше его конца" );
								}
								if ( *((double*)$4) <= 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "Относительная вероятность должна быть больше нуля" );
								}
								((RDOFUNSequenceByHistReal *)$1)->addReal($2, $3, *((double*)$4)); $$ = $1;
							}
							| fun_seq_by_hist_body_real REAL_CONST INT_CONST INT_CONST {
								RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->name->c_str()) );
								}
								if ( *((double*)$2) != ((RDOFUNSequenceByHistReal *)$1)->lastTo() ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "Начало интервала должно совпадать с окончанием предыдущего" );
								}
								if ( *((double*)$2) > $3 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "Начало интервала должно быть меньше его конца" );
								}
								if ( $4 <= 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "Относительная вероятность должна быть больше нуля" );
								}
								((RDOFUNSequenceByHistReal *)$1)->addReal(*((double*)$2), $3, $4); $$ = $1;
							}
							| fun_seq_by_hist_body_real INT_CONST REAL_CONST INT_CONST {
								RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( rdo::format("Последовательность '%s' определена как целочисленная, её даипазоны тоже должны быть челочисленными", header->name->c_str()) );
								}
								if ( $2 != ((RDOFUNSequenceByHistReal *)$1)->lastTo() ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "Начало интервала должно совпадать с окончанием предыдущего" );
								}
								if ( $2 > *((double*)$3) ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "Начало интервала должно быть меньше его конца" );
								}
								if ( $4 <= 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "Относительная вероятность должна быть больше нуля" );
								}
								((RDOFUNSequenceByHistReal *)$1)->addReal($2, *((double*)$3), $4); $$ = $1;
							}
							| fun_seq_by_hist_body_real INT_CONST INT_CONST INT_CONST {
								if ( $2 != ((RDOFUNSequenceByHistReal *)$1)->lastTo() ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "Начало интервала должно совпадать с окончанием предыдущего" );
								}
								if ( $2 > $3 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "Начало интервала должно быть меньше его конца" );
								}
								if ( $4 <= 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "Относительная вероятность должна быть больше нуля" );
								}
								((RDOFUNSequenceByHistReal *)$1)->addReal($2, $3, $4); $$ = $1;
							}
							| fun_seq_by_hist_header REAL_CONST error {
								parser->lexer_loc_set( &(@2), &(@3) );
								parser->error( "Ожидается конец диапазона" );
							}
							| fun_seq_by_hist_header INT_CONST error {
								parser->lexer_loc_set( &(@2), &(@3) );
								parser->error( "Ожидается конец диапазона" );
							}
							| fun_seq_by_hist_header REAL_CONST REAL_CONST error {
								parser->lexer_loc_set( &(@3), &(@4) );
								parser->error( "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_header INT_CONST REAL_CONST error {
								parser->lexer_loc_set( &(@3), &(@4) );
								parser->error( "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_header REAL_CONST INT_CONST error {
								parser->lexer_loc_set( &(@3), &(@4) );
								parser->error( "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_header INT_CONST INT_CONST error {
								parser->lexer_loc_set( &(@3), &(@4) );
								parser->error( "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_body_real REAL_CONST error {
								parser->lexer_loc_set( &(@2), &(@3) );
								parser->error( "Ожидается конец диапазона" );
							}
							| fun_seq_by_hist_body_real INT_CONST error {
								parser->lexer_loc_set( &(@2), &(@3) );
								parser->error( "Ожидается конец диапазона" );
							}
							| fun_seq_by_hist_body_real REAL_CONST REAL_CONST error {
								parser->lexer_loc_set( &(@3), &(@4) );
								parser->error( "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_body_real INT_CONST REAL_CONST error {
								parser->lexer_loc_set( &(@3), &(@4) );
								parser->error( "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_body_real REAL_CONST INT_CONST error {
								parser->lexer_loc_set( &(@3), &(@4) );
								parser->error( "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_body_real INT_CONST INT_CONST error {
								parser->lexer_loc_set( &(@3), &(@4) );
								parser->error( "Ожидается относительная вероятность" );
							}
							| fun_seq_by_hist_body_real IDENTIF error {
								parser->lexer_loc_set( &(@2) );
								parser->error( "Значение не соответствует типу последовательности" );
							};

fun_seq_by_hist_body_enum:	fun_seq_by_hist_header IDENTIF REAL_CONST {
								RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPResParam::pt_enum ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "Значение не соответствует типу последовательности" );
								}
								if ( *((double*)$3) <= 0 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "Относительная вероятность должна быть больше нуля" );
								}
								parser->lexer_loc_backup();
								parser->lexer_loc_set( &(@2) );
								$$ = (int)(new RDOFUNSequenceByHistEnum(header, (std::string*)$2, *((double*)$3)));
								parser->lexer_loc_restore();
							}
							| fun_seq_by_hist_header IDENTIF INT_CONST {
								RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPResParam::pt_enum ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "Значение не соответствует типу последовательности" );
								}
								if ( $3 <= 0 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "Относительная вероятность должна быть больше нуля" );
								}
								parser->lexer_loc_backup();
								parser->lexer_loc_set( &(@2) );
								$$ = (int)(new RDOFUNSequenceByHistEnum(header, (std::string*)$2, $3));
								parser->lexer_loc_restore();
							}
							| fun_seq_by_hist_body_enum IDENTIF REAL_CONST {
								if ( *((double*)$3) <= 0 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "Относительная вероятность должна быть больше нуля" );
								}
								parser->lexer_loc_backup();
								parser->lexer_loc_set( &(@2) );
								((RDOFUNSequenceByHistEnum *)$1)->addEnum((std::string*)$2, *((double*)$3));
								parser->lexer_loc_restore();
								$$ = $1;
							}
							| fun_seq_by_hist_body_enum IDENTIF INT_CONST {
								if ( $3 <= 0 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "Относительная вероятность должна быть больше нуля" );
								}
								parser->lexer_loc_backup();
								parser->lexer_loc_set( &(@2) );
								((RDOFUNSequenceByHistEnum *)$1)->addEnum((std::string*)$2, $3);
								parser->lexer_loc_restore();
								$$ = $1;
							}
							| fun_seq_by_hist_header IDENTIF error {
								parser->lexer_loc_set( &(@2), &(@3) );
								parser->error( rdo::format("Ожидается относительная вероятность для значения: %s", ((std::string*)$2)->c_str()) );
							}
							| fun_seq_by_hist_body_enum IDENTIF error {
								parser->lexer_loc_set( &(@2), &(@3) );
								parser->error( rdo::format("Ожидается относительная вероятность для значения: %s", ((std::string*)$2)->c_str()) );
							}
							| fun_seq_by_hist_body_enum REAL_CONST error {
								parser->lexer_loc_set( &(@2) );
								parser->error( "Значение не соответствует типу последовательности" );
							}
							| fun_seq_by_hist_body_enum INT_CONST error {
								parser->lexer_loc_set( &(@2) );
								parser->error( "Значение не соответствует типу последовательности" );
							};

fun_seq_by_hist:	fun_seq_by_hist_body_real End {
						((RDOFUNSequenceByHist *)$1)->createCalcs();
					}
					| fun_seq_by_hist_body_enum End {
						((RDOFUNSequenceByHist *)$1)->createCalcs();
					}
					| fun_seq_by_hist_body_real error {
						parser->lexer_loc_set( &(@2) );
						parser->error( "Ошибка в описании последовательности" );
					}
					| fun_seq_by_hist_body_enum error {
						parser->lexer_loc_set( &(@2) );
						parser->error( "Ошибка в описании последовательности" );
					}
					| fun_seq_by_hist_body_real {
						parser->lexer_loc_set( @1.last_line, @1.last_column );
						parser->error( "Ожидается ключевое слово $End" );
					}
					| fun_seq_by_hist_body_enum {
						parser->lexer_loc_set( @1.last_line, @1.last_column );
						parser->error( "Ожидается ключевое слово $End" );
					};

fun_seq_enumerative_header:	fun_seq_header enumerative Body {
								$$ = (int)(new RDOFUNSequenceEnumerativeHeader((RDOFUNSequenceHeader *)$1));
							}
							| fun_seq_header enumerative INT_CONST Body {
								$$ = (int)(new RDOFUNSequenceEnumerativeHeader((RDOFUNSequenceHeader *)$1, $3));
							}
							| fun_seq_header enumerative Parameters {
								parser->lexer_loc_set( &(@3) );
								parser->error( "Последовательности типа enumerative не имеет параметров" );
							}
							| fun_seq_header enumerative INT_CONST Parameters {
								parser->lexer_loc_set( &(@4) );
								parser->error( "Последовательности типа enumerative не имеет параметров" );
							}
							| fun_seq_header enumerative error {
								parser->lexer_loc_set( &(@2), &(@3) );
								parser->error( "Ожидается база генератора" );
							};


fun_seq_enumerative_body_int:	fun_seq_enumerative_header INT_CONST {
									RDOFUNSequenceEnumerativeHeader* header = reinterpret_cast<RDOFUNSequenceEnumerativeHeader*>($1);
									if ( header->header->type->getType() != RDORTPResParam::pt_int ) {
										parser->lexer_loc_set( &(@2) );
										switch ( header->header->type->getType() ) {
											case RDORTPResParam::pt_real: {
												parser->error( rdo::format("Последовательность '%s' определена как вещественная, её значения тоже должны быть вещественными", header->header->name->c_str()) );
												break;
											}
											case RDORTPResParam::pt_enum: {
												parser->error( rdo::format("Последовательность '%s' определена как перечислимая, её значения тоже должны быть перечислимого типа", header->header->name->c_str()) );
												break;
											}
										}
									}
									$$ = (int)(new RDOFUNSequenceEnumerativeInt( header, $2 ));
								}
								| fun_seq_enumerative_body_int INT_CONST {
									((RDOFUNSequenceEnumerativeInt *)$1)->addInt($2); $$ = $1;
								}
								| fun_seq_enumerative_body_int error {
									parser->lexer_loc_set( &(@1), &(@2) );
									parser->error( "Ожидается целое число или ключевое слово $End" );
								};

fun_seq_enumerative_body_real:	fun_seq_enumerative_header REAL_CONST {
									RDOFUNSequenceEnumerativeHeader* header = reinterpret_cast<RDOFUNSequenceEnumerativeHeader*>($1);
									if ( header->header->type->getType() != RDORTPResParam::pt_real ) {
										parser->lexer_loc_set( &(@2) );
										switch ( header->header->type->getType() ) {
											case RDORTPResParam::pt_int: {
												parser->error( rdo::format("Последовательность '%s' определена как целочисленная, её значения тоже должны быть челочисленными", header->header->name->c_str()) );
												break;
											}
											case RDORTPResParam::pt_enum: {
												parser->error( rdo::format("Последовательность '%s' определена как перечислимая, её значения тоже должны быть перечислимого типа", header->header->name->c_str()) );
												break;
											}
										}
									}
									$$ = (int)(new RDOFUNSequenceEnumerativeReal( header, (double*)$2) );
								}
								| fun_seq_enumerative_body_real REAL_CONST {
									((RDOFUNSequenceEnumerativeReal *)$1)->addReal((double*)$2); $$ = $1;
								}
								| fun_seq_enumerative_body_real error {
									parser->lexer_loc_set( &(@1), &(@2) );
									parser->error( "Ожидается вещественное число или ключевое слово $End" );
								};

fun_seq_enumerative_body_enum:	fun_seq_enumerative_header IDENTIF {
									RDOFUNSequenceEnumerativeHeader* header = reinterpret_cast<RDOFUNSequenceEnumerativeHeader*>($1);
									if ( header->header->type->getType() != RDORTPResParam::pt_enum ) {
										parser->lexer_loc_set( &(@2) );
										switch ( header->header->type->getType() ) {
											case RDORTPResParam::pt_int: {
												parser->error( rdo::format("Последовательность '%s' определена как целочисленная, её значения тоже должны быть челочисленными", header->header->name->c_str()) );
												break;
											}
											case RDORTPResParam::pt_real: {
												parser->error( rdo::format("Последовательность '%s' определена как вещественная, её значения тоже должны быть вещественными", header->header->name->c_str()) );
												break;
											}
										}
									}
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@2) );
									$$ = (int)(new RDOFUNSequenceEnumerativeEnum((RDOFUNSequenceEnumerativeHeader *)$1, (std::string*)$2));
									parser->lexer_loc_restore();
								}
								| fun_seq_enumerative_body_enum IDENTIF {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@2) );
									((RDOFUNSequenceEnumerativeEnum *)$1)->addEnum((std::string*)$2); $$ = $1;
									parser->lexer_loc_restore();
								}
								| fun_seq_enumerative_body_enum error {
									parser->lexer_loc_set( &(@1), &(@2) );
									parser->error( "Ожидается элемент перечислимого типа или ключевое слово $End" );
								};

fun_seq_enumerative:	fun_seq_enumerative_body_int End {
							((RDOFUNSequenceEnumerative *)$1)->createCalcs();
						}
						| fun_seq_enumerative_body_real End {
							((RDOFUNSequenceEnumerative *)$1)->createCalcs();
						}
						| fun_seq_enumerative_body_enum End {
							((RDOFUNSequenceEnumerative *)$1)->createCalcs();
						};

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
				$$ = (int)new RDOFUNArithm( reinterpret_cast<std::string*>($1), reinterpret_cast<std::string*>($3), @1, @3 );
			}
			| INT_CONST						{ $$ = (int)new RDOFUNArithm( (int)$1, @1 );          }
			| REAL_CONST					{ $$ = (int)new RDOFUNArithm( (double*)$1, @1 );      }
			| IDENTIF						{ $$ = (int)new RDOFUNArithm( (std::string*)$1, @1 ); }
			| '-' fun_arithm %prec UMINUS	{
				YYLTYPE error_pos;
				error_pos.first_line   = @1.first_line;
				error_pos.first_column = @1.first_column;
				error_pos.last_line    = @2.last_line;
				error_pos.last_column  = @2.last_column;
				$$ = (int)new RDOFUNArithm( reinterpret_cast<RDOFUNArithm*>($2)->getType(), new rdoRuntime::RDOCalcUMinus( reinterpret_cast<RDOFUNArithm*>($2)->createCalc() ), error_pos );
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
								RDOFUNParams* fun = new RDOFUNParams();
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
						$$ = (int)(new RDOFUNGroupLogic($1, (std::string *)$3));
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
						RDOFUNLogic* trueLogic = new RDOFUNLogic( new rdoRuntime::RDOCalcConst(1) );
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
						$$ = (int)new RDOFUNSelect((std::string*)$3);
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

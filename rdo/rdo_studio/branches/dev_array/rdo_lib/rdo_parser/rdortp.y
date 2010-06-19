/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdortp.y
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
%token RDO_array						379
%token RDO_event						380
%token RDO_Planning						381

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
#include "rdo_lib/rdo_parser/rdo_type_range.h"
#include "rdo_lib/rdo_parser/rdo_array.h"
#include "rdo_common/rdosmart_ptr.h"
// ===============================================================================

#define PARSER  LEXER->parser()
#define RUNTIME PARSER->runtime()

#define P_RDOVALUE(A) reinterpret_cast<PTR(RDOValue)>(A)
#define RDOVALUE(A)   (*P_RDOVALUE(A))

OPEN_RDO_PARSER_NAMESPACE
%}

%start type_list

%%

type_list:			/* empty */
					| type_list rtp_res_type
					| type_list ext_param_type
					| error {
						PARSER->error().error(rdoParse::RDOParserSrcInfo(), _T("Ожидается ключевое слово $Resource_type"));
					};

ext_param_type:		RDO_typedef RDO_enum ext_par_type_enum
					{
					};

ext_par_type_enum:	param_type_enum RDO_IDENTIF
					{
						PTR(RDOEnumType) enu = reinterpret_cast<PTR(RDOEnumType)>($1);
					};

rtp_res_type:		rtp_header RDO_Parameters rtp_body RDO_End
					{
						LPRDORTPResType pResourceType = PARSER->stack().pop<RDORTPResType>($1);
						ASSERT(pResourceType);
						if (pResourceType->getParams().empty())
						{
							PARSER->error().warning(@2, rdo::format(_T("Тип ресурса '%s' не содежит параметров"), pResourceType->name().c_str()));
						}
						$$ = PARSER->stack().push(pResourceType);
					}
					| rtp_header RDO_Parameters rtp_body {
						PARSER->error().error(@2, _T("Не найдено ключевое слово $End"));
					}
					| rtp_header error {
						PARSER->error().error(@2, _T("Не найдено ключевое слово $Parameters"));
					};

rtp_header:			RDO_Resource_type RDO_IDENTIF_COLON rtp_vid_res
					{
						LEXER->enumReset();
						PTR(RDOValue)       type_name = P_RDOVALUE($2);
						tstring             name      = type_name->value().getIdentificator();
						LPRDORTPResType     _rtp      = PARSER->findRTPResType(name);
						if (_rtp)
						{
							PARSER->error().push_only(type_name->src_info(), rdo::format(_T("Тип ресурса уже существует: %s"), name.c_str()));
							PARSER->error().push_only(_rtp->src_info(), _T("См. первое определение"));
							PARSER->error().push_done();
						}
						LPRDORTPResType pResourceType = rdo::Factory<RDORTPResType>::create(PARSER, type_name->src_info(), $3 != 0);
						ASSERT(pResourceType);
						$$ = PARSER->stack().push(pResourceType);
					}
					|	RDO_Resource_type RDO_IDENTIF_COLON RDO_IDENTIF_COLON rtp_vid_res
					{
						LEXER->enumReset();
						PTR(RDOValue)       type_name = P_RDOVALUE($2);
						tstring             name      = type_name->value().getIdentificator();
						LPRDORTPResType     _rtp      = PARSER->findRTPResType(name);
						if (_rtp)
						{
							PARSER->error().push_only(type_name->src_info(), rdo::format(_T("Тип ресурса уже существует: %s"), name.c_str()));
							PARSER->error().push_only(_rtp->src_info(), _T("См. первое определение"));
							PARSER->error().push_done();
						}
						PTR(RDOValue)       prnt_type_name = P_RDOVALUE($3);
						tstring             prnt_name      = prnt_type_name->value().getIdentificator();
						LPRDORTPResType     _rtp_prnt      = PARSER->findRTPResType(prnt_name);

						if (_rtp_prnt)
						{
							LPRDORTPResType pResourceType = rdo::Factory<RDORTPResType>::create(PARSER, type_name->src_info(), $4 != 0);
							ASSERT(pResourceType);
							ruint t_ind   = 0;
							ruint col_par = _rtp_prnt->getParams().size();
							while (t_ind < col_par)
							{
								pResourceType->addParam(_rtp_prnt->getParams()[t_ind]);
								PARSER->error().warning(_rtp_prnt->getParams()[t_ind]->src_info(), rdo::format(_T("Параметр %s передан от родителя %s потомку %s"), _rtp_prnt->getParams()[t_ind]->src_info().src_text().c_str(), prnt_name.c_str(), name.c_str()));
								t_ind++;
							}
							$$ = PARSER->stack().push(pResourceType);
							PARSER->error().warning(@2, rdo::format(_T("Тип ресурса %s является потомком типа ресурса %s"), name.c_str(), prnt_name.c_str()));
						}
						else
						{
							PARSER->error().push_only(@3, rdo::format(_T("Родительский тип ресурса не существует: %s"), prnt_name.c_str()));
							PARSER->error().push_done();
						}
					}
					| RDO_Resource_type RDO_IDENTIF_COLON error {
						PARSER->error().error(@2, _T("Не указан вид ресурса"));
					}
					| RDO_Resource_type RDO_IDENTIF_COLON RDO_IDENTIF_COLON error {
						PARSER->error().error(@3, _T("Не указан вид ресурса"));
					}
					| RDO_Resource_type error {
						tstring str(LEXER->YYText());
						PARSER->error().error(@2, rdo::format(_T("Ошибка в описании имени типа ресурса: %s"), str.c_str()));
					};

rtp_vid_res:		RDO_permanent	{ $$ = 1; }
					| RDO_temporary	{ $$ = 0; };

rtp_body:			/* empty */ {
					}
					| rtp_body rtp_param {
						LPRDORTPParam pParam = PARSER->stack().pop<RDORTPParam>($2);
						PARSER->getLastRTPResType()->addParam(pParam);
					};

rtp_param:			RDO_IDENTIF_COLON param_type
					{
						PTR(RDOValue)  param_name = P_RDOVALUE($1);
						LPRDOTypeParam param_type = PARSER->stack().pop<RDOTypeParam>($2);
						LPRDORTPParam  pParam     = rdo::Factory<RDORTPParam>::create(PARSER->getLastRTPResType(), param_type, param_name->src_info());
						$$ = PARSER->stack().push(pParam);
					}
					| RDO_IDENTIF_COLON error {
						if (PARSER->lexer_loc_line() == @1.last_line)
						{
							tstring str(LEXER->YYText());
							PARSER->error().error(@2, rdo::format(_T("Неверный тип параметра: %s"), str.c_str()));
						}
						else
						{
							PARSER->error().error(@1, _T("Ожидается тип параметра"));
						}
					}
					| error {
						PARSER->error().error(@1, _T("Неправильное описание параметра"));
					};

// ----------------------------------------------------------------------------
// ---------- Описание типа параметра
// ----------------------------------------------------------------------------
param_type:		RDO_integer param_type_range param_value_default
				{
					LPRDOTypeRangeRange pRange = PARSER->stack().pop<RDOTypeRangeRange>($2);
					LPRDOTypeParam pType;
					if (pRange)
					{
						if (pRange->getMin().typeID() != rdoRuntime::RDOType::t_int ||
						    pRange->getMax().typeID() != rdoRuntime::RDOType::t_int)
						{
							PARSER->error().error(@2, _T("Диапазон целого типа должен быть целочисленным"));
						}
						LPRDOTypeIntRange pIntRange = rdo::Factory<RDOTypeIntRange>::create(pRange);
						pType = rdo::Factory<RDOTypeParam>::create(pIntRange, RDOVALUE($3), RDOParserSrcInfo(@1, @3));
					}
					else
					{
						pType = rdo::Factory<RDOTypeParam>::create(g_int, RDOVALUE($3), RDOParserSrcInfo(@1, @3));
					}
					$$ = PARSER->stack().push(pType);
				}
				| RDO_real param_type_range param_value_default
				{
					LPRDOTypeRangeRange pRange = PARSER->stack().pop<RDOTypeRangeRange>($2);
					LPRDOTypeParam pType;
					if (pRange)
					{
						LPRDOTypeRealRange pRealRange = rdo::Factory<RDOTypeRealRange>::create(pRange);
						pType = rdo::Factory<RDOTypeParam>::create(pRealRange, RDOVALUE($3), RDOParserSrcInfo(@1, @3));
					}
					else
					{
						pType = rdo::Factory<RDOTypeParam>::create(g_real, RDOVALUE($3), RDOParserSrcInfo(@1, @3));
					}
					$$ = PARSER->stack().push(pType);
				}
				| RDO_string param_value_default
				{
					LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(g_string, RDOVALUE($2), RDOParserSrcInfo(@1, @2));
					$$ = PARSER->stack().push(pType);
				}
				| param_type_array param_value_default
				{
					LPRDOArrayType pArray = PARSER->stack().pop<RDOArrayType>($1);
					LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(pArray, RDOVALUE($2), RDOParserSrcInfo(@1, @2));
					$$ = PARSER->stack().push(pType);
				}
				| RDO_bool param_value_default
				{
					LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(g_bool, RDOVALUE($2), RDOParserSrcInfo(@1, @2));
					$$ = PARSER->stack().push(pType);
				}
				| param_type_enum param_value_default
				{
					LEXER->enumReset();
					LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
					LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(pEnum, RDOVALUE($2), RDOParserSrcInfo(@1, @2));
					$$ = PARSER->stack().push(pType);
				}
				| param_type_such_as param_value_default
				{
					LPRDOTypeParam pTypeSuchAs = PARSER->stack().pop<RDOTypeParam>($1);
					ASSERT(pTypeSuchAs);
					RDOValue default = RDOVALUE($2);
					if (!default.defined())
					{
						if (pTypeSuchAs->default().defined())
							default = pTypeSuchAs->default();
					}
					LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(pTypeSuchAs->type(), default, RDOParserSrcInfo(@1, @2));
					$$ = PARSER->stack().push(pType);
				};
/*
				| RDO_integer error {
					PARSER->error().error(@2, _T("Ошибка после ключевого слова integer. Возможно, не хватает значения по-умолчанию."));
				}
				| RDO_real error {
					PARSER->error().error(@2, _T("Ошибка после ключевого слова real. Возможно, не хватает значения по-умолчанию."));
				}
				| param_type_enum error {
					PARSER->error().error(@2, _T("Ошибка после перечислимого типа. Возможно, не хватает значения по-умолчанию."));
				};
*/

param_type_range:	/* empty */ {
					$$ = PARSER->stack().push<RDOTypeRangeRange>(LPRDOTypeRangeRange());
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST ']' {
					LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
					pRange->checkRange();
					$$ = PARSER->stack().push(pRange);
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST ']' {
					LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
					pRange->checkRange();
					$$ = PARSER->stack().push(pRange);
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST ']' {
					LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
					pRange->checkRange();
					$$ = PARSER->stack().push(pRange);
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST ']' {
					LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
					pRange->checkRange();
					$$ = PARSER->stack().push(pRange);
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

param_type_enum:	'(' param_type_enum_list ')' {
						LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($2);
						$$ = PARSER->stack().push(pEnum);
					}
					| '(' param_type_enum_list error {
						PARSER->error().error(@2, _T("Перечисление должно заканчиваться скобкой"));
					};

param_type_enum_list: RDO_IDENTIF {
						LPRDOEnumType pEnum = rdo::Factory<RDOEnumType>::create();
						pEnum->add(RDOVALUE($1));
						LEXER->enumBegin();
						$$ = PARSER->stack().push(pEnum);
					}
					| param_type_enum_list ',' RDO_IDENTIF {
						if (!LEXER->enumEmpty())
						{
							LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
							pEnum->add(RDOVALUE($3));
							$$ = PARSER->stack().push(pEnum);
						}
						else
						{
							PARSER->error().error(@3, _T("Ошибка в описании значений перечислимого типа"));
						}
					}
					| param_type_enum_list RDO_IDENTIF {
						if (!LEXER->enumEmpty())
						{
							LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
							pEnum->add(RDOVALUE($2));
							$$ = PARSER->stack().push(pEnum);
							PARSER->error().warning(@1, rdo::format(_T("Пропущена запятая перед: %s"), RDOVALUE($2)->getIdentificator().c_str()));
						}
						else
						{
							PARSER->error().error(@2, _T("Ошибка в описании значений перечислимого типа"));
						}
					}
					| param_type_enum_list ',' RDO_INT_CONST {
						PARSER->error().error(@3, _T("Значение перечислимого типа не может быть цифрой"));
					}
					| param_type_enum_list ',' RDO_REAL_CONST {
						PARSER->error().error(@3, _T("Значение перечислимого типа не может быть цифрой"));
					}
					| param_type_enum_list RDO_INT_CONST {
						PARSER->error().error(@2, _T("Значение перечислимого типа не может быть цифрой"));
					}
					| param_type_enum_list RDO_REAL_CONST {
						PARSER->error().error(@2, _T("Значение перечислимого типа не может быть цифрой"));
					}
					| RDO_INT_CONST {
						PARSER->error().error(@1, _T("Значение перечислимого типа не может начинаться с цифры"));
					}
					| RDO_REAL_CONST {
						PARSER->error().error(@1, _T("Значение перечислимого типа не может начинаться с цифры"));
					};

param_type_such_as:	RDO_such_as RDO_IDENTIF '.' RDO_IDENTIF {
						tstring type  = RDOVALUE($2)->getIdentificator();
						tstring param = RDOVALUE($4)->getIdentificator();
						LPRDORTPResType pRTP = PARSER->findRTPResType(type);
						if (!pRTP)
						{
							PARSER->error().error(@2, rdo::format(_T("Ссылка на неизвестный тип ресурса: %s"), type.c_str()));
						}
						LPRDORTPParam pParam = pRTP->findRTPParam(param);
						if (!pParam)
						{
							PARSER->error().error(@4, rdo::format(_T("Ссылка на неизвестный параметр ресурса: %s.%s"), type.c_str(), param.c_str()));
						}
						$$ = PARSER->stack().push(pParam->getParamType());
					}
					| RDO_such_as RDO_IDENTIF {
						tstring constName = RDOVALUE($2)->getIdentificator();
						CPTR(RDOFUNConstant) const cons = PARSER->findFUNConstant(constName);
						if (!cons)
						{
							PARSER->error().error(@2, rdo::format(_T("Ссылка на несуществующую константу: %s"), constName.c_str()));
						}
						$$ = PARSER->stack().push(cons->getType());
					}
					| RDO_such_as RDO_IDENTIF '.' error {
						tstring type = RDOVALUE($2)->getIdentificator();
						LPRDORTPResType rt = PARSER->findRTPResType(type);
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

param_value_default:	/* empty */ {
						$$ = (int)PARSER->addValue(new rdoParse::RDOValue());
					}
					| '=' param_value {
						$$ = $2;
					}
					| '=' error {
						RDOParserSrcInfo src_info(@1, @2, true);
						if (src_info.src_pos().point())
						{
							PARSER->error().error(src_info, _T("Не указано значение по-умолчанию"));
						}
						else
						{
							PARSER->error().error(src_info, _T("Неверное значение по-умолчанию"));
						}
					};

param_value:	RDO_INT_CONST {
					$$ = $1;
				}
				| RDO_REAL_CONST {
					$$ = $1;
				}
				| RDO_STRING_CONST {
					$$ = $1;
				}
				| RDO_IDENTIF {
					$$ = $1;
				}
				| RDO_BOOL_CONST {
					$$ = $1;
				}
				| param_array_value {
					$$ = $1;
				};

param_type_array:	RDO_array '<' param_type '>' {
						LPRDOTypeParam pParamType = PARSER->stack().pop<RDOTypeParam>($3);
						ASSERT(pParamType);
						LPRDOArrayType pArray = rdo::Factory<RDOArrayType>::create(pParamType->type(), RDOParserSrcInfo(@1, @4));
						$$ = PARSER->stack().push(pArray);
					};

param_array_value:	'[' array_item ']' {
						LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($2);
						ASSERT(pArrayValue);
						$$ = (int)PARSER->addValue(new RDOValue(pArrayValue->getRArray(), pArrayValue->getArrayType(), RDOParserSrcInfo(@2)));
					}
					|'[' array_item error {
						PARSER->error().error(@2, _T("Массив должен закрываться скобкой"));
					};

array_item:			param_value {
						LPRDOArrayType pArrayType = rdo::Factory<RDOArrayType>::create(RDOVALUE($1).type(), RDOParserSrcInfo(@1));
						ASSERT(pArrayType);
						LPRDOArrayValue pArrayValue = rdo::Factory<RDOArrayValue>::create(pArrayType);
						pArrayValue->insertItem(RDOVALUE($1));
						$$ = PARSER->stack().push(pArrayValue);
					}
					|
					array_item ',' param_value {
						LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($1);
						ASSERT(pArrayValue);
						pArrayValue->insertItem(RDOVALUE($3));
						$$ = PARSER->stack().push(pArrayValue);
					}
					|
					array_item param_value {
						LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($1);
						ASSERT(pArrayValue);
						pArrayValue->insertItem(RDOVALUE($2));
						$$ = PARSER->stack().push(pArrayValue);
						PARSER->error().warning(@1, rdo::format(_T("Пропущена запятая перед: %s"), RDOVALUE($2)->getAsString().c_str()));
					};

// ----------------------------------------------------------------------------

%%

CLOSE_RDO_PARSER_NAMESPACE

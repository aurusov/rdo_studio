/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdopat.y
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
#include "rdo_lib/rdo_parser/rdopat.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_parser/rdo_type_range.h"
#include "rdo_lib/rdo_runtime/rdotrace.h"
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

%%

pat_main:
			| pat_main pat_pattern;
			| error {
				PARSER->error().error(@1, _T("Неизвестная ошибка"));
			};

pat_header:	RDO_Pattern RDO_IDENTIF_COLON RDO_operation pat_trace {
				PTR(RDOValue) name = P_RDOVALUE($2);
				$$ = (int)new RDOPatternOperation(PARSER, name->src_info(), $4 != 0);
			}
			| RDO_Pattern RDO_IDENTIF_COLON RDO_irregular_event pat_trace {
				PTR(RDOValue) name = P_RDOVALUE($2);
				$$ = (int)new RDOPatternIrregEvent(PARSER, name->src_info(), $4 != 0);
			}
			| RDO_Pattern RDO_IDENTIF_COLON RDO_rule pat_trace {
				PTR(RDOValue) name = P_RDOVALUE($2);
				$$ = (int)new RDOPatternRule(PARSER, name->src_info(), $4 != 0);
			}
			| RDO_Pattern RDO_IDENTIF_COLON RDO_keyboard pat_trace {
				PTR(RDOValue) name = P_RDOVALUE($2);
				$$ = (int)new RDOPatternKeyboard(PARSER, name->src_info(), $4 != 0);
			};
			| RDO_Pattern error {
				PARSER->error().error(@2, _T("Ожидается имя образца"));
			}
			| RDO_Pattern RDO_IDENTIF_COLON error {
				PARSER->error().error(@2, @3, _T("Ожидается тип образца"));
//			}
//			| RDO_Pattern RDO_IDENTIF_COLON RDO_irregular_event error {
//				PARSER->error().error(@2, @3, _T("Ожидается признак трассировки"));
//				PARSER->error().error(@2, @3, _T("Ожидается признак трассировки, описание параметров или релевантных ресурсов образца"));
			};

pat_trace:	/* empty */		{ $$ = 0; }
			| RDO_trace		{ $$ = 1; }
			| RDO_no_trace	{ $$ = 0; };

pat_params_begin: pat_header RDO_Parameters { $$ = $1; };

pat_params:	pat_params_begin RDO_IDENTIF_COLON param_type {
				PTR(RDOPATPattern)       pattern    = reinterpret_cast<PTR(RDOPATPattern)>($1);
				PTR(RDOValue)            param_name = P_RDOVALUE($2);
				LPRDOTypeParam           param_type = PARSER->stack().pop<RDOTypeParam>($3);
				PTR(RDOFUNFunctionParam) param      = new RDOFUNFunctionParam(pattern, param_name->src_info(), param_type);
				pattern->add(param);
				if (param_type->type()->typeID() == rdoRuntime::RDOType::t_enum)
				{
//					static_cast<PTR(RDORTPEnumParamType)>(param_type)->enum_name = rdo::format(_T("%s.%s"), pattern->name().c_str(), param_name->value().getIdentificator().c_str());
				}
			}
			| pat_params RDO_IDENTIF_COLON param_type {
				PTR(RDOPATPattern)       pattern    = reinterpret_cast<PTR(RDOPATPattern)>($1);
				PTR(RDOValue)            param_name = P_RDOVALUE($2);
				LPRDOTypeParam           param_type = PARSER->stack().pop<RDOTypeParam>($3);
				PTR(RDOFUNFunctionParam) param      = new RDOFUNFunctionParam(pattern, param_name->src_info(), param_type);
				pattern->add(param);
				if (param_type->type()->typeID() == rdoRuntime::RDOType::t_enum)
				{
//					static_cast<PTR(RDORTPEnumParamType)>(param_type)->enum_name = rdo::format(_T("%s.%s"), pattern->name().c_str(), param_name->value().getIdentificator().c_str());
				}
			}
			| pat_params_begin error {
				if (@1.last_line != @2.last_line)
				{
					PARSER->error().error(@2, _T("Ожидается имя параметра образца"));
				}
				else
				{
					PARSER->error().error(@2, rdo::format(_T("Ожидается имя параметра образца, найдено: %s"), LEXER->YYText()));
				}
			}
			| pat_params_begin RDO_IDENTIF error {
				if (@2.last_line != @3.last_line)
				{
					PARSER->error().error(@2, @3, _T("Ожидается двоеточие"));
				}
				else
				{
					PARSER->error().error(@2, @3, rdo::format(_T("Ожидается двоеточие, найдено: %s"), LEXER->YYText()));
				}
			}
			| pat_params_begin RDO_IDENTIF_COLON error {
				if (@2.last_line != @3.last_line)
				{
					PARSER->error().error(@2, @3, _T("Ожидается тип параметра образца"));
				}
				else
				{
					PARSER->error().error(@2, @3, rdo::format(_T("Ожидается тип параметра образца, найдено: %s"), LEXER->YYText()));
				}
			}
			| pat_params error {
				if (@1.last_line != @2.last_line)
				{
					PARSER->error().error(@2, _T("Ожидается имя параметра образца"));
				}
				else
				{
					PARSER->error().error(@2, rdo::format(_T("Ожидается имя параметра образца, найдено: %s"), LEXER->YYText()));
				}
			}
			| pat_params RDO_IDENTIF error {
				if (@2.last_line != @3.last_line)
				{
					PARSER->error().error(@2, @3, _T("Ожидается двоеточие"));
				}
				else
				{
					PARSER->error().error(@2, @3, rdo::format(_T("Ожидается двоеточие, найдено: %s"), LEXER->YYText()));
				}
			}
			| pat_params RDO_IDENTIF_COLON error {
				if (@2.last_line != @3.last_line)
				{
					PARSER->error().error(@2, @3, _T("Ожидается тип параметра образца"));
				}
				else
				{
					PARSER->error().error(@2, @3, rdo::format(_T("Ожидается тип параметра образца, найдено: %s"), LEXER->YYText()));
				}
			};

pat_params_end:	pat_params RDO_Relevant_resources   { $$ = $1; }
				| pat_header RDO_Relevant_resources { $$ = $1; }
				| pat_header error {
					PARSER->error().error(@2, _T("Ожидается ключевое слово $Relevant_resources"));
				};

pat_rel_res:	pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF pat_conv pat_conv {
					// проверено для ie,rule,opr,key
					PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
					switch (pattern->getType())
					{
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard :
						{
							PTR(RDOValue) rel_name  = P_RDOVALUE($2);
							PTR(RDOValue) type_name = P_RDOVALUE($3);
							static_cast<PTR(RDOPatternOperation)>(pattern)->addRelRes(rel_name->src_info(), type_name->src_info(), (rdoRuntime::RDOResource::ConvertStatus)$4, (rdoRuntime::RDOResource::ConvertStatus)$5, @4, @5);
							break;
						}
						case RDOPATPattern::PT_IE:
							PARSER->error().error(@5, _T("У нерегулярного события нет события конца, а значит и второго статуса конвертора"));
							break;
						case RDOPATPattern::PT_Rule:
							PARSER->error().error(@5, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
							break;
					}
				}
				| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF pat_conv pat_conv {
					// проверено для ie,rule,opr,key
					PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
					switch (pattern->getType())
					{
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard :
						{
							PTR(RDOValue) rel_name  = P_RDOVALUE($2);
							PTR(RDOValue) type_name = P_RDOVALUE($3);
							static_cast<PTR(RDOPatternOperation)>(pattern)->addRelRes(rel_name->src_info(), type_name->src_info(), (rdoRuntime::RDOResource::ConvertStatus)$4, (rdoRuntime::RDOResource::ConvertStatus)$5, @4, @5);
							break;
						}
						case RDOPATPattern::PT_IE:
							PARSER->error().error(@5, _T("У нерегулярного события нет события конца, а значит и второго статуса конвертора"));
							break;
						case RDOPATPattern::PT_Rule:
							PARSER->error().error(@5, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
							break;
					}
				}
				| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF pat_conv {
					// проверено для ie,rule,opr,key
					PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
					switch (pattern->getType())
					{
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard :
							PARSER->error().error(@4, rdo::format(_T("Помимо статуса конвертора начала (%s), ожидается статус конвертора конца, потому что у операции есть событие конца"), RDOPATPattern::StatusToStr((rdoRuntime::RDOResource::ConvertStatus)$4).c_str()));
							break;
						case RDOPATPattern::PT_IE  : 
						case RDOPATPattern::PT_Rule:
						{
							PTR(RDOValue) rel_name  = P_RDOVALUE($2);
							PTR(RDOValue) type_name = P_RDOVALUE($3);
							pattern->addRelRes(rel_name->src_info(), type_name->src_info(), (rdoRuntime::RDOResource::ConvertStatus)$4, @4);
							break;
						}
					}
				}
				| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF pat_conv {
					// проверено для ie,rule,opr,key
					PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
					switch (pattern->getType())
					{
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard :
							PARSER->error().error(@4, rdo::format(_T("Помимо статуса конвертора начала (%s), ожидается статус конвертора конца, потому что у операции есть событие конца"), RDOPATPattern::StatusToStr((rdoRuntime::RDOResource::ConvertStatus)$4).c_str()));
							break;
						case RDOPATPattern::PT_IE  : 
						case RDOPATPattern::PT_Rule:
						{
							PTR(RDOValue) rel_name  = P_RDOVALUE($2);
							PTR(RDOValue) type_name = P_RDOVALUE($3);
							pattern->addRelRes(rel_name->src_info(), type_name->src_info(), (rdoRuntime::RDOResource::ConvertStatus)$4, @4);
							break;
						}
					}
				}
				| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange pat_conv {
					// проверено для ie,rule,opr,key
					PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
					switch (pattern->getType())
					{
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard :
						{
							PTR(RDOValue) rel_name  = P_RDOVALUE($2);
							PTR(RDOValue) type_name = P_RDOVALUE($3);
							YYLTYPE convertor_pos = @3;
							convertor_pos.first_line   = convertor_pos.last_line;
							convertor_pos.first_column = convertor_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
							static_cast<PTR(RDOPatternOperation)>(pattern)->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, (rdoRuntime::RDOResource::ConvertStatus)$4, convertor_pos, @4);
							break;
						}
						case RDOPATPattern::PT_IE:
							PARSER->error().error(@4, _T("У нерегулярного события нет события конца, а значит и второго статуса конвертора"));
							break;
						case RDOPATPattern::PT_Rule:
							PARSER->error().error(@4, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
							break;
					}
				}
				| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange pat_conv {
					// проверено для ie,rule,opr,key
					PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
					switch (pattern->getType())
					{
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard :
						{
							PTR(RDOValue) rel_name  = P_RDOVALUE($2);
							PTR(RDOValue) type_name = P_RDOVALUE($3);
							YYLTYPE convertor_pos = @3;
							convertor_pos.first_line   = convertor_pos.last_line;
							convertor_pos.first_column = convertor_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
							static_cast<PTR(RDOPatternOperation)>(pattern)->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, (rdoRuntime::RDOResource::ConvertStatus)$4, convertor_pos, @4);
							break;
						}
						case RDOPATPattern::PT_IE:
							PARSER->error().error(@4, _T("У нерегулярного события нет события конца, а значит и второго статуса конвертора"));
							break;
						case RDOPATPattern::PT_Rule:
							PARSER->error().error(@4, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
							break;
					}
				}
				| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange_NoChange {
					// проверено для ie,rule,opr,key
					PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
					switch (pattern->getType())
					{
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard :
						{
							PTR(RDOValue) rel_name  = P_RDOVALUE($2);
							PTR(RDOValue) type_name = P_RDOVALUE($3);
							YYLTYPE convertor_begin_pos = @3;
							tstring str = LEXER->YYText();
							rdo::toLower(str);
							tstring::size_type first_nochange = str.find(_T("nochange"));
							int i = 0;
							while (true)
							{
								if (str[i] == '\n')
								{
									convertor_begin_pos.first_line++;
									convertor_begin_pos.first_column = 0;
								}
								else if (str[i] != '\r')
								{
									convertor_begin_pos.first_column++;
								}
								i++;
								if (i == first_nochange)
									break;
							}
							convertor_begin_pos.last_line   = convertor_begin_pos.first_line;
							convertor_begin_pos.last_column = convertor_begin_pos.first_column + RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
							YYLTYPE convertor_end_pos = @3;
							convertor_end_pos.first_line   = convertor_end_pos.last_line;
							convertor_end_pos.first_column = convertor_end_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
							static_cast<PTR(RDOPatternOperation)>(pattern)->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, rdoRuntime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos);
							break;
						}
						case RDOPATPattern::PT_IE:
							PARSER->error().error(@3, _T("У нерегулярного события нет события конца, а значит и второго статуса конвертора"));
							break;
						case RDOPATPattern::PT_Rule:
							PARSER->error().error(@3, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
							break;
					}
				}
				| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange_NoChange {
					// проверено для ie,rule,opr,key
					PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
					switch (pattern->getType())
					{
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard :
						{
							PTR(RDOValue) rel_name  = P_RDOVALUE($2);
							PTR(RDOValue) type_name = P_RDOVALUE($3);
							YYLTYPE convertor_begin_pos = @3;
							tstring str = LEXER->YYText();
							rdo::toLower(str);
							tstring::size_type first_nochange = str.find(_T("nochange"));
							int i = 0;
							while (true)
							{
								if (str[i] == '\n')
								{
									convertor_begin_pos.first_line++;
									convertor_begin_pos.first_column = 0;
								}
								else if (str[i] != '\r')
								{
									convertor_begin_pos.first_column++;
								}
								i++;
								if (i == first_nochange)
									break;
							}
							convertor_begin_pos.last_line   = convertor_begin_pos.first_line;
							convertor_begin_pos.last_column = convertor_begin_pos.first_column + RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
							YYLTYPE convertor_end_pos = @3;
							convertor_end_pos.first_line   = convertor_end_pos.last_line;
							convertor_end_pos.first_column = convertor_end_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
							static_cast<PTR(RDOPatternOperation)>(pattern)->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, rdoRuntime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos);
							break;
						}
						case RDOPATPattern::PT_IE:
							PARSER->error().error(@3, _T("У нерегулярного события нет события конца, а значит и второго статуса конвертора"));
							break;
						case RDOPATPattern::PT_Rule:
							PARSER->error().error(@3, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
							break;
					}
				}
				| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange {
					// проверено для ie,rule,opr,key
					PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
					switch (pattern->getType())
					{
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard :
							PARSER->error().error(@3, rdo::format(_T("Помимо статуса конвертора начала (%s), ожидается статус конвертора конца, потому что у операции есть событие конца"), RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).c_str()));
							break;
						case RDOPATPattern::PT_IE  : 
						case RDOPATPattern::PT_Rule:
						{
							PTR(RDOValue) rel_name  = P_RDOVALUE($2);
							PTR(RDOValue) type_name = P_RDOVALUE($3);
							YYLTYPE convertor_pos = @3;
							convertor_pos.first_line   = convertor_pos.last_line;
							convertor_pos.first_column = convertor_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
							pattern->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, convertor_pos);
							break;
						}
					}
				}
				| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange {
					// проверено для ie,rule,opr,key
					PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
					switch (pattern->getType())
					{
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard :
							PARSER->error().error(@3, rdo::format(_T("Помимо статуса конвертора начала (%s), ожидается статус конвертора конца, потому что у операции есть событие конца"), RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).c_str()));
							break;
						case RDOPATPattern::PT_IE  : 
						case RDOPATPattern::PT_Rule:
						{
							PTR(RDOValue) rel_name  = P_RDOVALUE($2);
							PTR(RDOValue) type_name = P_RDOVALUE($3);
							YYLTYPE convertor_pos = @3;
							convertor_pos.first_line   = convertor_pos.last_line;
							convertor_pos.first_column = convertor_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
							pattern->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, convertor_pos);
							break;
						}
					}
				}
				| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF RDO_IDENTIF_NoChange {
					// проверено для ie,rule,opr,key
					PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
					switch (pattern->getType())
					{
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard :
						{
							PTR(RDOValue) rel_name  = P_RDOVALUE($2);
							PTR(RDOValue) type_name = P_RDOVALUE($3);
							tstring convert_begin = RDOVALUE($4)->getIdentificator();
							YYLTYPE convertor_begin_pos = @4;
							convertor_begin_pos.last_line   = convertor_begin_pos.first_line;
							convertor_begin_pos.last_column = convertor_begin_pos.first_column + convert_begin.length();
							YYLTYPE convertor_end_pos = @4;
							convertor_end_pos.first_line   = convertor_end_pos.last_line;
							convertor_end_pos.first_column = convertor_end_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
							static_cast<PTR(RDOPatternOperation)>(pattern)->addRelRes(rel_name->src_info(), type_name->src_info(), pattern->StrToStatus(convert_begin, convertor_begin_pos), rdoRuntime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos);
							break;
						}
						case RDOPATPattern::PT_IE:
							PARSER->error().error(@4, _T("У нерегулярного события нет события конца, а значит и второго статуса конвертора"));
							break;
						case RDOPATPattern::PT_Rule:
							PARSER->error().error(@4, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
							break;
					}
				}
				| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF RDO_IDENTIF_NoChange {
					// проверено для ie,rule,opr,key
					PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
					switch (pattern->getType())
					{
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard :
						{
							PTR(RDOValue) rel_name  = P_RDOVALUE($2);
							PTR(RDOValue) type_name = P_RDOVALUE($3);
							tstring convert_begin = RDOVALUE($4)->getIdentificator();
							YYLTYPE convertor_begin_pos = @4;
							convertor_begin_pos.last_line   = convertor_begin_pos.first_line;
							convertor_begin_pos.last_column = convertor_begin_pos.first_column + convert_begin.length();
							YYLTYPE convertor_end_pos = @4;
							convertor_end_pos.first_line   = convertor_end_pos.last_line;
							convertor_end_pos.first_column = convertor_end_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
							static_cast<PTR(RDOPatternOperation)>(pattern)->addRelRes(rel_name->src_info(), type_name->src_info(), pattern->StrToStatus(convert_begin, convertor_begin_pos), rdoRuntime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos);
							break;
						}
						case RDOPATPattern::PT_IE:
							PARSER->error().error(@4, _T("У нерегулярного события нет события конца, а значит и второго статуса конвертора"));
							break;
						case RDOPATPattern::PT_Rule:
							PARSER->error().error(@4, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
							break;
					}
				}
				| pat_params_end error {
					PARSER->error().error(@2, _T("Ошибка в описании релевантных ресурсов"));
				}
				| pat_rel_res error {
					PARSER->error().error(@2, _T("Ошибка в описании релевантных ресурсов"));
				}
				| pat_params_end RDO_IDENTIF_COLON error {
					PARSER->error().error(@2, @3, _T("Ожидается описатель (имя типа или ресурса)"));
				}
				| pat_rel_res RDO_IDENTIF_COLON error {
					PARSER->error().error(@2, @3, _T("Ожидается описатель (имя типа или ресурса)"));
				}
				| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF error {
					if (PARSER->getLastPATPattern()->isHaveConvertEnd())
					{
						PARSER->error().error(@3, @4, _T("Ожидается статус конвертора начала"));
					}
					else
					{
						PARSER->error().error(@3, @4, _T("Ожидается статус конвертора"));
					}
				}
				| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF error {
					if (PARSER->getLastPATPattern()->isHaveConvertEnd())
					{
						PARSER->error().error(@3, @4, _T("Ожидается статус конвертора начала"));
					}
					else
					{
						PARSER->error().error(@3, @4, _T("Ожидается статус конвертора"));
					}
				}
				| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF pat_conv error {
					switch (PARSER->getLastPATPattern()->getType())
					{
						case RDOPATPattern::PT_Rule:
							PARSER->error().error(@5, _T("Ожидается способ выбора (first/with_min/with_max) или $Body"));
							break;
						case RDOPATPattern::PT_IE:
							PARSER->error().error(@5, _T("Ожидается способ выбора (first/with_min/with_max) или $Time"));
							break;
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard :
							PARSER->error().error(@4, @5, rdo::format(_T("Ожидается статус конвертора конца, найдено: %s"), LEXER->YYText()));
							break;
					}
				}
				| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF pat_conv error {
					switch (PARSER->getLastPATPattern()->getType())
					{
						case RDOPATPattern::PT_Rule:
							PARSER->error().error(@5, _T("Ожидается способ выбора (first/with_min/with_max) или $Body"));
							break;
						case RDOPATPattern::PT_IE:
							PARSER->error().error(@5, _T("Ожидается способ выбора (first/with_min/with_max) или $Time"));
							break;
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard :
							PARSER->error().error(@4, @5, rdo::format(_T("Ожидается статус конвертора конца, найдено: %s"), LEXER->YYText()));
							break;
					}
				}
				| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange error {
					switch (PARSER->getLastPATPattern()->getType())
					{
						case RDOPATPattern::PT_Rule:
							PARSER->error().error(@4, _T("Ожидается способ выбора (first/with_min/with_max) или $Body"));
							break;
						case RDOPATPattern::PT_IE:
							PARSER->error().error(@4, _T("Ожидается способ выбора (first/with_min/with_max) или $Time"));
							break;
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard :
							PARSER->error().error(@3, @4, rdo::format(_T("Ожидается статус конвертора конца, найдено: %s"), LEXER->YYText()));
							break;
					}
				}
				| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange error {
					switch (PARSER->getLastPATPattern()->getType())
					{
						case RDOPATPattern::PT_Rule:
							PARSER->error().error(@4, _T("Ожидается способ выбора (first/with_min/with_max) или $Body"));
							break;
						case RDOPATPattern::PT_IE:
							PARSER->error().error(@4, _T("Ожидается способ выбора (first/with_min/with_max) или $Time"));
							break;
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard :
							PARSER->error().error(@3, @4, rdo::format(_T("Ожидается статус конвертора конца, найдено: %s"), LEXER->YYText()));
							break;
					}
				};

pat_conv:	RDO_Keep			{ $$ = rdoRuntime::RDOResource::CS_Keep;     }
			| RDO_Create		{ $$ = rdoRuntime::RDOResource::CS_Create;   }
			| RDO_Erase			{ $$ = rdoRuntime::RDOResource::CS_Erase;    }
			| RDO_NonExist		{ $$ = rdoRuntime::RDOResource::CS_NonExist; };

pat_common_choice:	pat_rel_res
					| pat_rel_res RDO_first {
						PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
						if (pattern->getType() == RDOPATPattern::PT_IE)
						{
							PARSER->error().error(@2, _T("В нерегулярном событии не используется способ выбора релевантных ресурсов"));
						}
						else
						{
							pattern->setCommonChoiceFirst();
//							PTR(RDOFUNArithm) arithm = new RDOFUNArithm(PARSER, 1, @2);
//							arithm->setSrcPos(@2);
//							arithm->setSrcText(_T("first"));
//							pattern->setCommonChoiceWithMax(arithm);
						}
					}
					| pat_rel_res RDO_with_min fun_arithm {
						PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
						if (pattern->getType() == RDOPATPattern::PT_IE)
						{
							PARSER->error().error(@2, _T("В нерегулярном событии не используется способ выбора релевантных ресурсов"));
						}
						else
						{
							PTR(RDOFUNArithm) arithm = P_ARITHM($3);
							arithm->setSrcPos (@2, @3);
							arithm->setSrcText(_T("with_min ") + arithm->src_text());
							pattern->setCommonChoiceWithMin(arithm);
						}
					}
					| pat_rel_res RDO_with_max fun_arithm {
						PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
						if (pattern->getType() == RDOPATPattern::PT_IE)
						{
							PARSER->error().error(@2, _T("В нерегулярном событии не используется способ выбора релевантных ресурсов"));
						}
						else
						{
							PTR(RDOFUNArithm) arithm = P_ARITHM($3);
							arithm->setSrcPos (@2, @3);
							arithm->setSrcText(_T("with_max ") + arithm->src_text());
							pattern->setCommonChoiceWithMax(arithm);
						}
					}
					| pat_rel_res RDO_with_min error {
						PARSER->error().error(@3, _T("Ошибка в арифметическом выражении"));
					}
					| pat_rel_res RDO_with_max error {
						PARSER->error().error(@3, _T("Ошибка в арифметическом выражении"));
					};

pat_time:	pat_common_choice RDO_Body {
				PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
				switch (pattern->getType())
				{
					case RDOPATPattern::PT_IE       :
					case RDOPATPattern::PT_Operation:
					case RDOPATPattern::PT_Keyboard :
						PARSER->error().error(@2, _T("Перед $Body пропущено ключевое слово $Time"));
						break;
				}
			}
			| pat_common_choice RDO_Time '=' fun_arithm RDO_Body {
				PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
				switch (pattern->getType())
				{
					case RDOPATPattern::PT_IE       :
					case RDOPATPattern::PT_Operation:
					case RDOPATPattern::PT_Keyboard : break;
					case RDOPATPattern::PT_Rule     :
						PARSER->error().error(@2, _T("Поле $Time не используется в продукционном правиле"));
//						getParser()->error(_T("Rule must have no $Time field"));
						break;
				}
				PTR(RDOFUNArithm) arithm = P_ARITHM($4);
				arithm->setSrcPos (@2, @4);
				arithm->setSrcText(_T("$Time = ") + arithm->src_text());
				pattern->setTime(arithm);
			}
			| pat_common_choice RDO_Time '=' fun_arithm error {
				PARSER->error().error(@4, @5, _T("Ожидается ключевое слово $Body"));
			}
			| pat_common_choice RDO_Time '=' error {
				PARSER->error().error(@4, _T("Ошибка в арифметическом выражении"));
			}
			| pat_common_choice RDO_Time error {
				PARSER->error().error(@2, @3, _T("После ключевого слова $Time ожидается знак равенства"));
			}
			| pat_common_choice error {
				PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
				switch (pattern->getType())
				{
					case RDOPATPattern::PT_Rule:
						PARSER->error().error(@2, rdo::format(_T("Ожидается $Body, найдено: %s"), LEXER->YYText()));
						break;
					case RDOPATPattern::PT_IE       :
					case RDOPATPattern::PT_Operation:
					case RDOPATPattern::PT_Keyboard :
						PARSER->error().error(@2, rdo::format(_T("Ожидается $Time, найдено: %s"), LEXER->YYText()));
						break;
				}
			};

pat_body:	pat_time RDO_IDENTIF_RELRES {
				PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
				tstring            name    = RDOVALUE($2)->getIdentificator();
				pattern->addRelResBody(RDOParserSrcInfo(@2, name));
			}
			| pat_convert RDO_IDENTIF_RELRES {
				PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
				tstring            name    = RDOVALUE($2)->getIdentificator();
				pattern->addRelResBody(RDOParserSrcInfo(@2, name));
			}
			| pat_time error {
				tstring str(LEXER->YYText());
				PARSER->error().error(@2, rdo::format(_T("Неизвестный релевантный ресурс: %s"), str.c_str()));
			}
			| pat_convert error {
				tstring str(LEXER->YYText());
				PARSER->error().error(@2, rdo::format(_T("Неизвестный релевантный ресурс: %s"), str.c_str()));
			};

pat_res_usage:	pat_body pat_choice pat_order {
					PTR(RDOPATChoiceFrom) choice_from = reinterpret_cast<PTR(RDOPATChoiceFrom)>($2);
					choice_from->setSrcPos(@2);
					PTR(RDOPATChoiceOrder) choice_order = reinterpret_cast<PTR(RDOPATChoiceOrder)>($3);
					choice_order->setSrcPos(@3);
					PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
					pattern->addRelResUsage(choice_from, choice_order);
				};

pat_choice: /* empty */ {
				PARSER->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceEmpty;
				$$ = (int) new RDOPATChoiceFrom(PARSER->getLastPATPattern()->currRelRes, RDOParserSrcInfo(_T("Choice NoCheck")), RDOPATChoiceFrom::ch_empty);
			}
			| pat_choice_nocheck {
				$$ = (int) new RDOPATChoiceFrom(PARSER->getLastPATPattern()->currRelRes, RDOParserSrcInfo(_T("Choice NoCheck")), RDOPATChoiceFrom::ch_nocheck);
			}
			| pat_choice_from fun_logic {
				PTR(RDOFUNLogic) logic = P_LOGIC($2);
				$$ = (int) new RDOPATChoiceFrom(PARSER->getLastPATPattern()->currRelRes, RDOParserSrcInfo(_T("Choice from ") + logic->src_text()), RDOPATChoiceFrom::ch_from, logic);
			}
			| pat_choice_from error {
				PARSER->error().error(@2, _T("Ошибка в логическом выражении"));
			};

pat_choice_nocheck: RDO_Choice RDO_NoCheck {
				PARSER->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceNoCheck;
			};

pat_choice_from: RDO_Choice RDO_from {
				PARSER->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceFrom;
			};

pat_order:	/* empty */ {
				PARSER->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceOrderEmpty;
				$$ = (int) new RDOPATChoiceOrder(PARSER->getLastPATPattern()->currRelRes, RDOParserSrcInfo(), rdoRuntime::RDOSelectResourceCalc::order_empty);
			}
			| pat_choice_first {
				$$ = (int) new RDOPATChoiceOrder(PARSER->getLastPATPattern()->currRelRes, RDOParserSrcInfo(_T("first")), rdoRuntime::RDOSelectResourceCalc::order_first);
			}
			| pat_choice_with_min fun_arithm {
				PTR(RDOFUNArithm) arithm = P_ARITHM($2);
				$$ = (int) new RDOPATChoiceOrder(PARSER->getLastPATPattern()->currRelRes, RDOParserSrcInfo(_T("with_min ") + arithm->src_text()), rdoRuntime::RDOSelectResourceCalc::order_with_min, arithm);
			}
			| pat_choice_with_max fun_arithm {
				PTR(RDOFUNArithm) arithm = P_ARITHM($2);
				$$ = (int) new RDOPATChoiceOrder(PARSER->getLastPATPattern()->currRelRes, RDOParserSrcInfo(_T("with_max ") + arithm->src_text()), rdoRuntime::RDOSelectResourceCalc::order_with_max, arithm);
			}
			| pat_choice_with_min error {
				PARSER->error().error(@2, _T("Ошибка в арифметическом выражении"));
			}
			| pat_choice_with_max error {
				PARSER->error().error(@2, _T("Ошибка в арифметическом выражении"));
			};

pat_choice_first: RDO_first {
				PARSER->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceOrderFirst;
			};

pat_choice_with_min: RDO_with_min {
				PARSER->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceOrderWithMin;
			};

pat_choice_with_max: RDO_with_max {
				PARSER->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceOrderWithMax;
			};

pat_convert:	pat_res_usage {
					PTR(RDOPATPattern)       pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
					PTR(RDORelevantResource) rel_res = pattern->currRelRes;
					tstring str;
					if (rel_res->choice_order->type != rdoRuntime::RDOSelectResourceCalc::order_empty)
					{
						str = _T("Сразу после ключевого слова ") + rel_res->choice_order->asString();
					}
					else if (rel_res->choice_from->type != RDOPATChoiceFrom::ch_empty)
					{
						str = _T("Сразу после условия выбора");
					}
					else
					{
						str = _T("Сразу после имени");
					}
					if (rel_res->begin != rdoRuntime::RDOResource::CS_NoChange && rel_res->begin != rdoRuntime::RDOResource::CS_Erase && rel_res->begin != rdoRuntime::RDOResource::CS_NonExist)
					{
						switch (pattern->getType())
						{
							case RDOPATPattern::PT_IE       : PARSER->error().error(@1, rdo::format(_T("%s ожидается ключевое слово Convert_event для релевантного ресурса '%s', т.к. его статус '%s', но найдено: %s"), str.c_str(), rel_res->name().c_str(), RDOPATPattern::StatusToStr(rel_res->begin).c_str(), LEXER->YYText())); break;
							case RDOPATPattern::PT_Rule     : PARSER->error().error(@1, rdo::format(_T("%s ожидается ключевое слово Convert_rule для релевантного ресурса '%s', т.к. его статус '%s', но найдено: %s"), str.c_str(), rel_res->name().c_str(), RDOPATPattern::StatusToStr(rel_res->begin).c_str(), LEXER->YYText())); break;
							case RDOPATPattern::PT_Operation:
							case RDOPATPattern::PT_Keyboard : PARSER->error().error(@1, rdo::format(_T("%s ожидается ключевое слово Convert_begin для релевантного ресурса '%s', т.к. его статус '%s', но найдено: %s"), str.c_str(), rel_res->name().c_str(), RDOPATPattern::StatusToStr(rel_res->begin).c_str(), LEXER->YYText())); break;
						}
//						PARSER->error().error(_T("Converter needed for \"") + *rel_res->name() + _T("\" relevant resource in pattern \"") + getName() + _T("\""));
					}
					if (rel_res->end != rdoRuntime::RDOResource::CS_NoChange && rel_res->end != rdoRuntime::RDOResource::CS_Erase && rel_res->end != rdoRuntime::RDOResource::CS_NonExist)
					{
						switch (pattern->getType())
						{
							case RDOPATPattern::PT_IE       : PARSER->error().error(@1, _T("Внутренняя ошибка")); break;
							case RDOPATPattern::PT_Rule     : PARSER->error().error(@1, _T("Внутренняя ошибка")); break;
							case RDOPATPattern::PT_Operation:
							case RDOPATPattern::PT_Keyboard : PARSER->error().error(@1, rdo::format(_T("%s ожидается ключевое слово Convert_end для релевантного ресурса '%s', т.к. его статус '%s', но найдено: %s"), str.c_str(), rel_res->name().c_str(), RDOPATPattern::StatusToStr(rel_res->begin).c_str(), LEXER->YYText())); break;
						}
					}
				}
				| pat_res_usage convert_begin pat_trace pat_params_set {
					PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
					if (pattern->getType() != RDOPATPattern::PT_Operation && pattern->getType() != RDOPATPattern::PT_Keyboard)
					{
						tstring type = _T("");
						switch (pattern->getType())
						{
							case RDOPATPattern::PT_IE       : type = _T("нерегулярном событии");  break;
							case RDOPATPattern::PT_Rule     : type = _T("продукционном правиле"); break;
						}
						PARSER->error().error(@2, rdo::format(_T("Ключевое слово Convert_begin может быть использовано в обыкновенной или клавиатурной операции, но не в %s '%s'"), type.c_str(), pattern->name().c_str()));
					}
					PTR(RDOPATParamSet) par_set = reinterpret_cast<PTR(RDOPATParamSet)>($4);
					par_set->setSrcPos(@4);
					static_cast<PTR(RDOPatternOperation)>(pattern)->addRelResConvertBeginEnd($3 != 0, par_set, false, NULL, @2, @2, @3, @3);
				}
				| pat_res_usage pat_params_set convert_end pat_trace pat_params_set {
					PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
					if (pattern->getType() != RDOPATPattern::PT_Operation && pattern->getType() != RDOPATPattern::PT_Keyboard)
					{
						tstring type = _T("");
						switch (pattern->getType())
						{
							case RDOPATPattern::PT_IE       : type = _T("нерегулярном событии");  break;
							case RDOPATPattern::PT_Rule     : type = _T("продукционном правиле"); break;
						}
						PARSER->error().error(@3, rdo::format(_T("Ключевое слово Convert_end может быть использовано в обыкновенной и клавиатурной операции, но не в %s '%s'"), type.c_str(), pattern->name().c_str()));
					}
					pattern->currRelRes->deleteParamSetBegin();
					PTR(RDOPATParamSet) par_set = reinterpret_cast<PTR(RDOPATParamSet)>($5);
					par_set->setSrcPos(@5);
					static_cast<PTR(RDOPatternOperation)>(pattern)->addRelResConvertBeginEnd(false, NULL, $4 != 0, par_set, @3, @3, @4, @4);
				}
				| pat_res_usage convert_begin pat_trace pat_params_set convert_end pat_trace pat_params_set {
					PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
					if (pattern->getType() != RDOPATPattern::PT_Operation && pattern->getType() != RDOPATPattern::PT_Keyboard)
					{
						tstring type = _T("");
						switch (pattern->getType())
						{
							case RDOPATPattern::PT_IE       : type = _T("нерегулярном событии");  break;
							case RDOPATPattern::PT_Rule     : type = _T("продукционном правиле"); break;
						}
						PARSER->error().error(@2, rdo::format(_T("Ключевые слова Convert_begin и Convert_end могут быть использованы в обыкновенной и клавиатурной операции, но не в %s '%s'"), type.c_str(), pattern->name().c_str()));
					}
					PTR(RDOPATParamSet) par_set_begin = reinterpret_cast<PTR(RDOPATParamSet)>($4);
					PTR(RDOPATParamSet) par_set_end   = reinterpret_cast<PTR(RDOPATParamSet)>($7);
					par_set_begin->setSrcPos(@4);
					par_set_end->setSrcPos(@7);
					static_cast<PTR(RDOPatternOperation)>(pattern)->addRelResConvertBeginEnd($3 != 0, par_set_begin, $6 != 0, par_set_end, @2, @5, @3, @6);
				}
				| pat_res_usage convert_rule pat_trace pat_params_set {
					PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
					if (pattern->getType() != RDOPATPattern::PT_Rule)
					{
						tstring type = _T("");
						switch (pattern->getType())
						{
							case RDOPATPattern::PT_IE       : type = _T("нерегулярном событии");  break;
							case RDOPATPattern::PT_Operation: type = _T("операции");              break;
							case RDOPATPattern::PT_Keyboard : type = _T("клавиатурной операции"); break;
						}
						PARSER->error().error(@2, rdo::format(_T("Ключевое слово Convert_rule может быть использовано в продукционном правиле, но не в %s '%s'"), type.c_str(), pattern->name().c_str()));
					}
					PTR(RDOPATParamSet) par_set = reinterpret_cast<PTR(RDOPATParamSet)>($4);
					par_set->setSrcPos(@4);
					pattern->addRelResConvert($3 != 0, par_set, @2, @3);
				}
				| pat_res_usage convert_event pat_trace pat_params_set {
					PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
					if (pattern->getType() != RDOPATPattern::PT_IE)
					{
						tstring type = _T("");
						switch (pattern->getType())
						{
							case RDOPATPattern::PT_Rule     : type = _T("продукционном правиле"); break;
							case RDOPATPattern::PT_Operation: type = _T("операции");              break;
							case RDOPATPattern::PT_Keyboard : type = _T("клавиатурной операции"); break;
						}
						PARSER->error().error(@2, rdo::format(_T("Ключевое слово Convert_event может быть использовано в нерегулярном событии, но не в %s '%s'"), type.c_str(), pattern->name().c_str()));
					}
					PTR(RDOPATParamSet) par_set = reinterpret_cast<PTR(RDOPATParamSet)>($4);
					par_set->setSrcPos(@4);
					pattern->addRelResConvert($3 != 0, par_set, @2, @3);
				};

convert_rule:	RDO_Convert_rule {
				PARSER->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::convertBegin;
			};

convert_event:	RDO_Convert_event {
				PARSER->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::convertBegin;
			};

convert_begin:	RDO_Convert_begin {
				PARSER->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::convertBegin;
			};

convert_end:	RDO_Convert_end {
				PARSER->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::convertEnd;
			};

pat_params_set:	/* empty */	{
					PTR(RDOPATParamSet) paramSet = PARSER->getLastPATPattern()->currRelRes->createParamSet();
					YYLTYPE pos = @0;
					pos.first_line   = pos.last_line;
					pos.first_column = pos.last_column;
					paramSet->setSrcPos(pos);
					$$ = (int)paramSet;
				}
				|	pat_params_set RDO_IDENTIF param_equal_type param_set_right_value {
					PTR(RDOPATParamSet)   paramSet    = reinterpret_cast<PTR(RDOPATParamSet)>($1);
					rdoRuntime::EqualType equalType   = static_cast<rdoRuntime::EqualType>($3);
					PTR(RDOFUNArithm)     rightArithm = P_ARITHM($4);
					tstring               paramName   = RDOVALUE($2)->getIdentificator();
					paramSet->addSet(paramName, @2, equalType, rightArithm);
				}
				| pat_params_set RDO_IDENTIF param_equal_type error {
					PARSER->error().error(@4, _T("Ошибка в арифметическом выражении"));
				}
				|	pat_params_set RDO_IDENTIF_NoChange {
					PTR(RDOPATParamSet) paramSet     = reinterpret_cast<PTR(RDOPATParamSet)>($1);
					tstring             paramName    = RDOVALUE($2)->getIdentificator();
					YYLTYPE             paramNamePos = @2;
					paramNamePos.last_line   = paramNamePos.first_line;
					paramNamePos.last_column = paramNamePos.first_column + paramName.length();
					paramSet->addSet(paramName, paramNamePos, rdoRuntime::ET_NOCHANGE, NULL);
				};

param_equal_type: RDO_set {
					$$ = rdoRuntime::ET_EQUAL;
				}
				| '=' {
					$$ = rdoRuntime::ET_EQUAL;
				}
				| RDO_PlusEqual {
					$$ = rdoRuntime::ET_PLUS;
				}
				| RDO_MinusEqual {
					$$ = rdoRuntime::ET_MINUS;
				}
				| RDO_MultiplyEqual {
					$$ = rdoRuntime::ET_MULTIPLY;
				}
				| RDO_DivideEqual {
					$$ = rdoRuntime::ET_DIVIDE;
				};

param_set_right_value: fun_arithm;

pat_pattern:	pat_convert RDO_End {
					PTR(RDOPATPattern) pattern = reinterpret_cast<PTR(RDOPATPattern)>($1);
					pattern->end();
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
					pType->setSrcText(pEnum->name());
					$$ = PARSER->stack().push(pType);
				}
				| param_type_such_as param_value_default
				{
					//CPTR(RDORTPParam) param = reinterpret_cast<PTR(RDORTPParam)>($1);
					//RDOParserSrcInfo src_info(@1);
					//src_info.setSrcText(_T("such_as ") + (param->getResType() ? param->getResType()->name() + _T(".") : _T("")) + param->name());
					//$$ = (int)param->getType()->constructorSuchAs(src_info);
				}
				| param_type_such_as '=' RDO_INT_CONST
				{
					//CPTR(RDORTPParam) param = reinterpret_cast<PTR(RDORTPParam)>($1);
					//RDOParserSrcInfo src_info(@1, @3);
					//src_info.setSrcText(_T("such_as ") + (param->getResType() ? param->getResType()->name() + _T(".") : _T("")) + param->name());
					//$$ = (int)param->getType()->constructorSuchAs(src_info, RDOVALUE($3));
				}
				| param_type_such_as '=' RDO_REAL_CONST
				{
					//CPTR(RDORTPParam) param = reinterpret_cast<PTR(RDORTPParam)>($1);
					//RDOParserSrcInfo src_info(@1, @3);
					//src_info.setSrcText(_T("such_as ") + (param->getResType() ? param->getResType()->name() + _T(".") : _T("")) + param->name());
					//$$ = (int)param->getType()->constructorSuchAs(src_info, RDOVALUE($3));
				}
				| param_type_such_as '=' RDO_IDENTIF
				{
					//CPTR(RDORTPParam) param = reinterpret_cast<PTR(RDORTPParam)>($1);
					//RDOParserSrcInfo src_info(@1, @3);
					//src_info.setSrcText(_T("such_as ") + (param->getResType() ? param->getResType()->name() + _T(".") : _T("")) + param->name());
					//$$ = (int)param->getType()->constructorSuchAs(src_info, RDOVALUE($3));
				}
				| param_type_such_as '=' error
				{
					//PARSER->error().error(rdoParse::RDOParserSrcInfo(), _T("Ожидается зачение по-умолчанию"));
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
						CPTR(RDORTPResType) const rt = PARSER->findRTPResType(type);
						if (!rt)
						{
							PARSER->error().error(@2, rdo::format(_T("Ссылка на неизвестный тип ресурса: %s"), type.c_str()));
						}
						LPRDORTPParam rp = rt->findRTPParam(param);
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
						NEVER_REACH_HERE;
						$$ = NULL;
//						$$ = (int)cons->getDescr();
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

param_value_default:	/* empty */ {
						$$ = (int)PARSER->addValue(new rdoParse::RDOValue());
					}
					| '=' RDO_INT_CONST {
						$$ = $2;
					}
					| '=' RDO_REAL_CONST {
						$$ = $2;
					}
					| '=' RDO_STRING_CONST{
						$$ = $2;
					}
					| '=' RDO_IDENTIF {
						$$ = $2;
					}
					| '=' RDO_BOOL_CONST {
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
							PARSER->error().error(src_info, _T("Неверное значение по-умолчанию "));
						}
					};
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// ---------- Логические выражения
// ----------------------------------------------------------------------------
fun_logic_eq: '='    { $$ = RDO_eq; }
			| RDO_eq { $$ = RDO_eq; };

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

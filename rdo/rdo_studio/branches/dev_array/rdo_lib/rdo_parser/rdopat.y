/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdopat.y
 * author   : Александ Барс, Урусов Андрей, Лущан Дмитрий
 * date     : 12.07.2010
 * bref     : закладка PAT - описание образцов активностей модели
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
%token RDO_else							382
%token RDO_IncrEqual					383
%token RDO_DecrEqual					384
%token RDO_Stopping						385
%token RDO_Start						386
%token RDO_Stop							387

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
#include "rdo_lib/rdo_parser/rdo_array.h"
#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/calc_event_plan.h"
// ===============================================================================

#define PARSER  LEXER->parser()
#define RUNTIME PARSER->runtime()

#define P_RDOVALUE(A) reinterpret_cast<PTR(RDOValue)>(A)
#define RDOVALUE(A)   (*P_RDOVALUE(A))

OPEN_RDO_PARSER_NAMESPACE
%}

%left RDO_or
%left RDO_and
%left '+' '-'
%left '*' '/'
%left RDO_not
%left RDO_UMINUS

%%

pat_main
	: /* empty */
	| pat_main pat_pattern
	| error
	{
		PARSER->error().error(@1, _T("Неизвестная ошибка"));
	}
	;

pat_header
	: RDO_Pattern RDO_IDENTIF_COLON RDO_operation pat_trace 
	{
		PTR(RDOValue) name = P_RDOVALUE($2);
		LPRDOPATPattern pPattern = rdo::Factory<RDOPatternOperation>::create(name->src_info(), $4 != 0);
		ASSERT(pPattern);
		$$ = PARSER->stack().push(pPattern);
	}
	| RDO_Pattern RDO_IDENTIF_COLON RDO_rule pat_trace
	{
		PTR(RDOValue) name = P_RDOVALUE($2);
		LPRDOPATPattern pPattern = rdo::Factory<RDOPatternRule>::create(name->src_info(), $4 != 0);
		ASSERT(pPattern);
		$$ = PARSER->stack().push(pPattern);
	}
	| RDO_Pattern RDO_IDENTIF_COLON RDO_keyboard pat_trace
	{
		PTR(RDOValue) name = P_RDOVALUE($2);
		LPRDOPATPattern pPattern = rdo::Factory<RDOPatternKeyboard>::create(name->src_info(), $4 != 0);
		ASSERT(pPattern);
		$$ = PARSER->stack().push(pPattern);
	}
	| RDO_Pattern error
	{
		PARSER->error().error(@2, _T("Ожидается имя образца"));
	}
	| RDO_Pattern RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@2, @3, _T("Ожидается тип образца"));
	}
	;

pat_trace
	: /* empty */	{ $$ = 0; }
	| RDO_trace		{ $$ = 1; }
	| RDO_no_trace	{ $$ = 0; }
	;

pat_params_begin
	: pat_header RDO_Parameters
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		$$ = PARSER->stack().push(pPattern);
	}
	;

pat_params
	: pat_params_begin RDO_IDENTIF_COLON param_type
	{
		LPRDOPATPattern          pPattern   = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		PTR(RDOValue)            param_name = P_RDOVALUE($2);
		LPRDOTypeParam           param_type = PARSER->stack().pop<RDOTypeParam>($3);
		LPRDOFUNFunctionParam    pParam     = rdo::Factory<RDOFUNFunctionParam>::create(param_name->src_info(), param_type);
		pPattern->add(pParam);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params RDO_IDENTIF_COLON param_type
	{
		LPRDOPATPattern          pPattern   = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		PTR(RDOValue)            param_name = P_RDOVALUE($2);
		LPRDOTypeParam           param_type = PARSER->stack().pop<RDOTypeParam>($3);
		LPRDOFUNFunctionParam    pParam     = rdo::Factory<RDOFUNFunctionParam>::create(param_name->src_info(), param_type);
		pPattern->add(pParam);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params_begin error
	{
		if (@1.m_last_line != @2.m_last_line)
		{
			PARSER->error().error(@2, _T("Ожидается имя параметра образца"));
		}
		else
		{
			PARSER->error().error(@2, rdo::format(_T("Ожидается имя параметра образца, найдено: %s"), LEXER->YYText()));
		}
	}
	| pat_params_begin RDO_IDENTIF error
	{
		if (@2.m_last_line != @3.m_last_line)
		{
			PARSER->error().error(@2, @3, _T("Ожидается двоеточие"));
		}
		else
		{
			PARSER->error().error(@2, @3, rdo::format(_T("Ожидается двоеточие, найдено: %s"), LEXER->YYText()));
		}
	}
	| pat_params_begin RDO_IDENTIF_COLON error
	{
		if (@2.m_last_line != @3.m_last_line)
		{
			PARSER->error().error(@2, @3, _T("Ожидается тип параметра образца"));
		}
		else
		{
			PARSER->error().error(@2, @3, rdo::format(_T("Ожидается тип параметра образца, найдено: %s"), LEXER->YYText()));
		}
	}
	| pat_params error
	{
		if (@1.m_last_line != @2.m_last_line)
		{
			PARSER->error().error(@2, _T("Ожидается имя параметра образца"));
		}
		else
		{
			PARSER->error().error(@2, rdo::format(_T("Ожидается имя параметра образца, найдено: %s"), LEXER->YYText()));
		}
	}
	| pat_params RDO_IDENTIF error
	{
		if (@2.m_last_line != @3.m_last_line)
		{
			PARSER->error().error(@2, @3, _T("Ожидается двоеточие"));
		}
		else
		{
			PARSER->error().error(@2, @3, rdo::format(_T("Ожидается двоеточие, найдено: %s"), LEXER->YYText()));
		}
	}
	| pat_params RDO_IDENTIF_COLON error
	{
		if (@2.m_last_line != @3.m_last_line)
		{
			PARSER->error().error(@2, @3, _T("Ожидается тип параметра образца"));
		}
		else
		{
			PARSER->error().error(@2, @3, rdo::format(_T("Ожидается тип параметра образца, найдено: %s"), LEXER->YYText()));
		}
	}
	;

pat_params_end
	: pat_params RDO_Relevant_resources
	{
		$$ = $1;
	}
	| pat_header RDO_Relevant_resources
	{
		$$ = $1;
	}
	| pat_header error
	{
		PARSER->error().error(@2, _T("Ожидается ключевое слово $Relevant_resources"));
	}
	;

pat_rel_res
	: pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF pat_conv pat_conv
	{
		// проверено для ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(rel_name->src_info(), type_name->src_info(), (rdoRuntime::RDOResource::ConvertStatus)$4, (rdoRuntime::RDOResource::ConvertStatus)$5, @4, @5);
				break;
			}
			case RDOPATPattern::PT_IE:
			{
				PARSER->error().error(@5, _T("У нерегулярного события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@5, _T("У события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@5, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF pat_conv pat_conv
	{
		// проверено для ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(rel_name->src_info(), type_name->src_info(), (rdoRuntime::RDOResource::ConvertStatus)$4, (rdoRuntime::RDOResource::ConvertStatus)$5, @4, @5);
				break;
			}
			case RDOPATPattern::PT_IE:
			{
				PARSER->error().error(@5, _T("У нерегулярного события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@5, _T("У события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@5, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF pat_conv
	{
		// проверено для ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@4, rdo::format(_T("Помимо статуса конвертора начала (%s), ожидается статус конвертора конца, потому что у операции есть событие конца"), RDOPATPattern::StatusToStr((rdoRuntime::RDOResource::ConvertStatus)$4).c_str()));
				break;
			}
			case RDOPATPattern::PT_IE   : 
			case RDOPATPattern::PT_Event: 
			case RDOPATPattern::PT_Rule :
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				pPattern->addRelRes(rel_name->src_info(), type_name->src_info(), (rdoRuntime::RDOResource::ConvertStatus)$4, @4);
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF pat_conv
	{
		// проверено для ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@4, rdo::format(_T("Помимо статуса конвертора начала (%s), ожидается статус конвертора конца, потому что у операции есть событие конца"), RDOPATPattern::StatusToStr((rdoRuntime::RDOResource::ConvertStatus)$4).c_str()));
				break;
			}
			case RDOPATPattern::PT_IE   : 
			case RDOPATPattern::PT_Event: 
			case RDOPATPattern::PT_Rule :
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				pPattern->addRelRes(rel_name->src_info(), type_name->src_info(), (rdoRuntime::RDOResource::ConvertStatus)$4, @4);
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange pat_conv
	{
		// проверено для ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				YYLTYPE convertor_pos = @3;
				convertor_pos.m_first_line = convertor_pos.m_last_line;
				convertor_pos.m_first_pos  = convertor_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_pos.m_first_seek = convertor_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, (rdoRuntime::RDOResource::ConvertStatus)$4, convertor_pos, @4);
				break;
			}
			case RDOPATPattern::PT_IE:
			{
				PARSER->error().error(@4, _T("У нерегулярного события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@4, _T("У события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@4, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange pat_conv
	{
		// проверено для ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				YYLTYPE convertor_pos = @3;
				convertor_pos.m_first_line = convertor_pos.m_last_line;
				convertor_pos.m_first_pos  = convertor_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_pos.m_first_seek = convertor_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, (rdoRuntime::RDOResource::ConvertStatus)$4, convertor_pos, @4);
				break;
			}
			case RDOPATPattern::PT_IE:
			{
				PARSER->error().error(@4, _T("У нерегулярного события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@4, _T("У события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@4, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange_NoChange
	{
		// проверено для ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
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
						convertor_begin_pos.m_first_line++;
						convertor_begin_pos.m_first_seek++;
						convertor_begin_pos.m_first_pos = 0;
					}
					else if (str[i] != '\r')
					{
						convertor_begin_pos.m_first_pos++;
						convertor_begin_pos.m_first_seek++;
					}
					i++;
					if (i == first_nochange)
						break;
				}
				convertor_begin_pos.m_last_line = convertor_begin_pos.m_first_line;
				convertor_begin_pos.m_last_pos  = convertor_begin_pos.m_first_pos  + RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_begin_pos.m_last_seek = convertor_begin_pos.m_first_seek + RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				YYLTYPE convertor_end_pos = @3;
				convertor_end_pos.m_first_line = convertor_end_pos.m_last_line;
				convertor_end_pos.m_first_pos  = convertor_end_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_end_pos.m_first_seek = convertor_end_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, rdoRuntime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos);
				break;
			}
			case RDOPATPattern::PT_IE:
			{
				PARSER->error().error(@3, _T("У нерегулярного события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@3, _T("У события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@3, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange_NoChange
	{
		// проверено для ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
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
						convertor_begin_pos.m_first_line++;
						convertor_begin_pos.m_first_seek++;
						convertor_begin_pos.m_first_pos = 0;
					}
					else if (str[i] != '\r')
					{
						convertor_begin_pos.m_first_pos++;
						convertor_begin_pos.m_first_seek++;
					}
					i++;
					if (i == first_nochange)
						break;
				}
				convertor_begin_pos.m_last_line = convertor_begin_pos.m_first_line;
				convertor_begin_pos.m_last_pos  = convertor_begin_pos.m_first_pos  + RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_begin_pos.m_last_seek = convertor_begin_pos.m_first_seek + RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				YYLTYPE convertor_end_pos = @3;
				convertor_end_pos.m_first_line = convertor_end_pos.m_last_line;
				convertor_end_pos.m_first_pos  = convertor_end_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_end_pos.m_first_seek = convertor_end_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, rdoRuntime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos);
				break;
			}
			case RDOPATPattern::PT_IE:
			{
				PARSER->error().error(@3, _T("У нерегулярного события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@3, _T("У события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@3, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange
	{
		// проверено для ie,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@3, rdo::format(_T("Помимо статуса конвертора начала (%s), ожидается статус конвертора конца, потому что у операции есть событие конца"), RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).c_str()));
				break;
			}
			case RDOPATPattern::PT_IE   : 
			case RDOPATPattern::PT_Event: 
			case RDOPATPattern::PT_Rule :
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				YYLTYPE convertor_pos = @3;
				convertor_pos.m_first_line = convertor_pos.m_last_line;
				convertor_pos.m_first_pos  = convertor_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_pos.m_first_seek = convertor_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, convertor_pos);
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange
	{
		// проверено для ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@3, rdo::format(_T("Помимо статуса конвертора начала (%s), ожидается статус конвертора конца, потому что у операции есть событие конца"), RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).c_str()));
				break;
			}
			case RDOPATPattern::PT_IE   : 
			case RDOPATPattern::PT_Event: 
			case RDOPATPattern::PT_Rule :
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				YYLTYPE convertor_pos = @3;
				convertor_pos.m_first_line = convertor_pos.m_last_line;
				convertor_pos.m_first_pos  = convertor_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_pos.m_first_seek = convertor_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, convertor_pos);
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF RDO_IDENTIF_NoChange
	{
		// проверено для ie,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				tstring convert_begin = RDOVALUE($4)->getIdentificator();
				YYLTYPE convertor_begin_pos = @4;
				convertor_begin_pos.m_last_line = convertor_begin_pos.m_first_line;
				convertor_begin_pos.m_last_pos  = convertor_begin_pos.m_first_pos  + convert_begin.length();
				convertor_begin_pos.m_last_seek = convertor_begin_pos.m_first_seek + convert_begin.length();
				YYLTYPE convertor_end_pos = @4;
				convertor_end_pos.m_first_line = convertor_end_pos.m_last_line;
				convertor_end_pos.m_first_pos  = convertor_end_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_end_pos.m_first_seek = convertor_end_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(rel_name->src_info(), type_name->src_info(), pPattern->StrToStatus(convert_begin, convertor_begin_pos), rdoRuntime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos);
				break;
			}
			case RDOPATPattern::PT_IE:
			{
				PARSER->error().error(@4, _T("У нерегулярного события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@4, _T("У события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@4, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF RDO_IDENTIF_NoChange
	{
		// проверено для ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				tstring convert_begin = RDOVALUE($4)->getIdentificator();
				YYLTYPE convertor_begin_pos = @4;
				convertor_begin_pos.m_last_line = convertor_begin_pos.m_first_line;
				convertor_begin_pos.m_last_pos  = convertor_begin_pos.m_first_pos  + convert_begin.length();
				convertor_begin_pos.m_last_seek = convertor_begin_pos.m_first_seek + convert_begin.length();
				YYLTYPE convertor_end_pos = @4;
				convertor_end_pos.m_first_line = convertor_end_pos.m_last_line;
				convertor_end_pos.m_first_pos  = convertor_end_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_end_pos.m_first_seek = convertor_end_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(rel_name->src_info(), type_name->src_info(), pPattern->StrToStatus(convert_begin, convertor_begin_pos), rdoRuntime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos);
				break;
			}
			case RDOPATPattern::PT_IE:
			{
				PARSER->error().error(@4, _T("У нерегулярного события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@4, _T("У события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@4, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params_end error
	{
		PARSER->error().error(@2, _T("Ошибка в описании релевантных ресурсов"));
	}
	| pat_rel_res error
	{
		PARSER->error().error(@2, _T("Ошибка в описании релевантных ресурсов"));
	}
	| pat_params_end RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@2, @3, _T("Ожидается описатель (имя типа или ресурса)"));
	}
	| pat_rel_res RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@2, @3, _T("Ожидается описатель (имя типа или ресурса)"));
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF error
	{
		if (PARSER->getLastPATPattern()->isHaveConvertEnd())
		{
			PARSER->error().error(@3, @4, _T("Ожидается статус конвертора начала"));
		}
		else
		{
			PARSER->error().error(@3, @4, _T("Ожидается статус конвертора"));
		}
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF error
	{
		if (PARSER->getLastPATPattern()->isHaveConvertEnd())
		{
			PARSER->error().error(@3, @4, _T("Ожидается статус конвертора начала"));
		}
		else
		{
			PARSER->error().error(@3, @4, _T("Ожидается статус конвертора"));
		}
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF pat_conv error
	{
		switch (PARSER->getLastPATPattern()->getType())
		{
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@5, _T("Ожидается способ выбора (first/with_min/with_max) или $Body"));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@5, _T("Ожидается способ выбора (first/with_min/with_max) или $Body"));
				break;
			}
			case RDOPATPattern::PT_IE:
			{
				PARSER->error().error(@5, _T("Ожидается способ выбора (first/with_min/with_max) или $Time"));
				break;
			}
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@4, @5, rdo::format(_T("Ожидается статус конвертора конца, найдено: %s"), LEXER->YYText()));
				break;
			}
		}
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF pat_conv error
	{
		switch (PARSER->getLastPATPattern()->getType())
		{
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@5, _T("Ожидается способ выбора (first/with_min/with_max) или $Body"));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@5, _T("Ожидается способ выбора (first/with_min/with_max) или $Body"));
				break;
			}
			case RDOPATPattern::PT_IE:
			{
				PARSER->error().error(@5, _T("Ожидается способ выбора (first/with_min/with_max) или $Time"));
				break;
			}
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@4, @5, rdo::format(_T("Ожидается статус конвертора конца, найдено: %s"), LEXER->YYText()));
				break;
			}
		}
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange error
	{
		switch (PARSER->getLastPATPattern()->getType())
		{
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@4, _T("Ожидается способ выбора (first/with_min/with_max) или $Body"));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@4, _T("Ожидается способ выбора (first/with_min/with_max) или $Body"));
				break;
			}
			case RDOPATPattern::PT_IE:
			{
				PARSER->error().error(@4, _T("Ожидается способ выбора (first/with_min/with_max) или $Time"));
				break;
			}
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@3, @4, rdo::format(_T("Ожидается статус конвертора конца, найдено: %s"), LEXER->YYText()));
				break;
			}
		}
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange error
	{
		switch (PARSER->getLastPATPattern()->getType())
		{
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@4, _T("Ожидается способ выбора (first/with_min/with_max) или $Body"));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@4, _T("Ожидается способ выбора (first/with_min/with_max) или $Body"));
				break;
			}
			case RDOPATPattern::PT_IE:
			{
				PARSER->error().error(@4, _T("Ожидается способ выбора (first/with_min/with_max) или $Time"));
				break;
			}
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@3, @4, rdo::format(_T("Ожидается статус конвертора конца, найдено: %s"), LEXER->YYText()));
				break;
			}
		}
	}
	;

pat_conv
	: RDO_Keep			{ $$ = rdoRuntime::RDOResource::CS_Keep;     }
	| RDO_Create		{ $$ = rdoRuntime::RDOResource::CS_Create;   }
	| RDO_Erase			{ $$ = rdoRuntime::RDOResource::CS_Erase;    }
	| RDO_NonExist		{ $$ = rdoRuntime::RDOResource::CS_NonExist; }
	;

pat_common_choice
	: pat_rel_res
	| pat_rel_res RDO_first
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		if (pPattern->getType() == RDOPATPattern::PT_IE || pPattern->getType() == RDOPATPattern::PT_Event)
		{
			PARSER->error().error(@2, _T("В событиях не используется способ выбора релевантных ресурсов"));
		}
		else
		{
			pPattern->setCommonChoiceFirst();
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_with_min fun_arithm
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		if (pPattern->getType() == RDOPATPattern::PT_IE || pPattern->getType() == RDOPATPattern::PT_Event)
		{
			PARSER->error().error(@2, _T("В событиях не используется способ выбора релевантных ресурсов"));
		}
		else
		{
			LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($3);
			ASSERT(pArithm);
			pArithm->setSrcPos (@2, @3);
			pArithm->setSrcText(_T("with_min ") + pArithm->src_text());
			pPattern->setCommonChoiceWithMin(pArithm);
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_with_max fun_arithm
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		if (pPattern->getType() == RDOPATPattern::PT_IE || pPattern->getType() == RDOPATPattern::PT_Event)
		{
			PARSER->error().error(@2, _T("В событиях не используется способ выбора релевантных ресурсов"));
		}
		else
		{
			LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($3);
			ASSERT(pArithm);
			pArithm->setSrcPos (@2, @3);
			pArithm->setSrcText(_T("with_max ") + pArithm->src_text());
			pPattern->setCommonChoiceWithMax(pArithm);
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_with_min error
	{
		PARSER->error().error(@3, _T("Ошибка в арифметическом выражении"));
	}
	| pat_rel_res RDO_with_max error
	{
		PARSER->error().error(@3, _T("Ошибка в арифметическом выражении"));
	}
	;

pat_time
	: pat_common_choice RDO_Body
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_IE       :
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@2, _T("Перед $Body пропущено ключевое слово $Time"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_common_choice RDO_Time '=' fun_arithm RDO_Body
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@2, _T("Поле $Time не используется в событии"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@2, _T("Поле $Time не используется в продукционном правиле"));
				break;
			}
		}
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($4);
		ASSERT(pArithm);
		pArithm->setSrcPos (@2, @4);
		pArithm->setSrcText(_T("$Time = ") + pArithm->src_text());
		pPattern->setTime(pArithm);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_common_choice RDO_Time '=' fun_arithm error
	{
		PARSER->error().error(@4, @5, _T("Ожидается ключевое слово $Body"));
	}
	| pat_common_choice RDO_Time '=' error
	{
		PARSER->error().error(@4, _T("Ошибка в арифметическом выражении"));
	}
	| pat_common_choice RDO_Time error
	{
		PARSER->error().error(@2, @3, _T("После ключевого слова $Time ожидается знак равенства"));
	}
	| pat_common_choice error
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@2, rdo::format(_T("Ожидается $Body, найдено: %s"), LEXER->YYText()));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@2, rdo::format(_T("Ожидается $Body, найдено: %s"), LEXER->YYText()));
				break;
			}
			case RDOPATPattern::PT_IE       :
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@2, rdo::format(_T("Ожидается $Time, найдено: %s"), LEXER->YYText()));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	;

pat_body
	: pat_time RDO_IDENTIF_RELRES
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		tstring         name     = RDOVALUE($2)->getIdentificator();
		pPattern->addRelResBody(RDOParserSrcInfo(@2, name));
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_convert RDO_IDENTIF_RELRES
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		tstring         name    = RDOVALUE($2)->getIdentificator();
		pPattern->addRelResBody(RDOParserSrcInfo(@2, name));
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_time error
	{
		tstring str(LEXER->YYText());
		PARSER->error().error(@2, rdo::format(_T("Неизвестный релевантный ресурс: %s"), str.c_str()));
	}
	| pat_convert error
	{
		tstring str(LEXER->YYText());
		PARSER->error().error(@2, rdo::format(_T("Неизвестный релевантный ресурс: %s"), str.c_str()));
	}
	;

pat_res_usage
	: pat_body pat_choice pat_order
	{
		LPRDOPATChoiceFrom pChoiceFrom = PARSER->stack().pop<RDOPATChoiceFrom>($2);
		ASSERT(pChoiceFrom);
		pChoiceFrom->setSrcPos(@2);

		LPRDOPATChoiceOrder pChoiceOrder = PARSER->stack().pop<RDOPATChoiceOrder>($3);
		pChoiceOrder->setSrcPos(@3);

		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		pPattern->addRelResUsage(pChoiceFrom, pChoiceOrder);
		$$ = PARSER->stack().push(pPattern);
	}
	;

pat_choice
	: /* empty */
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::choiceEmpty;
		LPRDOPATChoiceFrom pChoiceFrom = rdo::Factory<RDOPATChoiceFrom>::create(RDOParserSrcInfo(_T("Choice NoCheck")), RDOPATChoiceFrom::ch_empty);
		ASSERT(pChoiceFrom);
		$$ = PARSER->stack().push(pChoiceFrom);
	}
	| pat_choice_nocheck
	{
		LPRDOPATChoiceFrom pChoiceFrom = rdo::Factory<RDOPATChoiceFrom>::create(RDOParserSrcInfo(_T("Choice NoCheck")), RDOPATChoiceFrom::ch_nocheck);
		ASSERT(pChoiceFrom);
		$$ = PARSER->stack().push(pChoiceFrom);
	}
	| pat_choice_from fun_logic
	{
		LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		LPRDOPATChoiceFrom pChoiceFrom = rdo::Factory<RDOPATChoiceFrom>::create(RDOParserSrcInfo(_T("Choice from ") + pLogic->src_text()), RDOPATChoiceFrom::ch_from, pLogic);
		ASSERT(pChoiceFrom);
		$$ = PARSER->stack().push(pChoiceFrom);
	}
	| pat_choice_from error
	{
		PARSER->error().error(@2, _T("Ошибка в логическом выражении"));
	}
	;

pat_choice_nocheck
	: RDO_Choice RDO_NoCheck
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::choiceNoCheck;
	}
	;

pat_choice_from
	: RDO_Choice RDO_from
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::choiceFrom;
	}
	;

pat_order
	: /* empty */
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::choiceOrderEmpty;
		LPRDOPATChoiceOrder pChoiceOrder = rdo::Factory<RDOPATChoiceOrder>::create(RDOParserSrcInfo(), rdoRuntime::RDOSelectResourceCalc::order_empty);
		ASSERT(pChoiceOrder);
		$$ = PARSER->stack().push(pChoiceOrder);
	}
	| pat_choice_first
	{
		LPRDOPATChoiceOrder pChoiceOrder = rdo::Factory<RDOPATChoiceOrder>::create(RDOParserSrcInfo(_T("first")), rdoRuntime::RDOSelectResourceCalc::order_first);
		ASSERT(pChoiceOrder);
		$$ = PARSER->stack().push(pChoiceOrder);
	}
	| pat_choice_with_min fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		LPRDOPATChoiceOrder pChoiceOrder = rdo::Factory<RDOPATChoiceOrder>::create(RDOParserSrcInfo(_T("with_min ") + pArithm->src_text()), rdoRuntime::RDOSelectResourceCalc::order_with_min, pArithm);
		ASSERT(pChoiceOrder);
		$$ = PARSER->stack().push(pChoiceOrder);
	}
	| pat_choice_with_max fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		LPRDOPATChoiceOrder pChoiceOrder = rdo::Factory<RDOPATChoiceOrder>::create(RDOParserSrcInfo(_T("with_max ") + pArithm->src_text()), rdoRuntime::RDOSelectResourceCalc::order_with_max, pArithm);
		ASSERT(pChoiceOrder);
		$$ = PARSER->stack().push(pChoiceOrder);
	}
	| pat_choice_with_min error
	{
		PARSER->error().error(@2, _T("Ошибка в арифметическом выражении"));
	}
	| pat_choice_with_max error
	{
		PARSER->error().error(@2, _T("Ошибка в арифметическом выражении"));
	}
	;

pat_choice_first
	: RDO_first
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::choiceOrderFirst;
	}
	;

pat_choice_with_min
	: RDO_with_min
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::choiceOrderWithMin;
	}
	;

pat_choice_with_max
	: RDO_with_max
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::choiceOrderWithMax;
	}
	;

pat_convert
	: pat_res_usage
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		LPRDORelevantResource rel_res  = pPattern->m_pCurrRelRes;
		tstring str;
		if (rel_res->m_pChoiceOrder->m_type != rdoRuntime::RDOSelectResourceCalc::order_empty)
		{
			str = _T("Сразу после ключевого слова ") + rel_res->m_pChoiceOrder->asString();
		}
		else if (rel_res->m_pChoiceFrom->m_type != RDOPATChoiceFrom::ch_empty)
		{
			str = _T("Сразу после условия выбора");
		}
		else
		{
			str = _T("Сразу после имени");
		}
		if (rel_res->m_statusBegin != rdoRuntime::RDOResource::CS_NoChange && rel_res->m_statusBegin != rdoRuntime::RDOResource::CS_Erase && rel_res->m_statusBegin != rdoRuntime::RDOResource::CS_NonExist)
		{
			switch (pPattern->getType())
			{
				case RDOPATPattern::PT_IE:
				case RDOPATPattern::PT_Event:
				{
					PARSER->error().error(@1, rdo::format(_T("%s ожидается ключевое слово Convert_event для релевантного ресурса '%s', т.к. его статус '%s', но найдено: %s"), str.c_str(), rel_res->name().c_str(), RDOPATPattern::StatusToStr(rel_res->m_statusBegin).c_str(), LEXER->YYText()));
					break;
				}
				case RDOPATPattern::PT_Rule:
				{
					PARSER->error().error(@1, rdo::format(_T("%s ожидается ключевое слово Convert_rule для релевантного ресурса '%s', т.к. его статус '%s', но найдено: %s"), str.c_str(), rel_res->name().c_str(), RDOPATPattern::StatusToStr(rel_res->m_statusBegin).c_str(), LEXER->YYText()));
					break;
				}
				case RDOPATPattern::PT_Operation:
				case RDOPATPattern::PT_Keyboard :
				{
					PARSER->error().error(@1, rdo::format(_T("%s ожидается ключевое слово Convert_begin для релевантного ресурса '%s', т.к. его статус '%s', но найдено: %s"), str.c_str(), rel_res->name().c_str(), RDOPATPattern::StatusToStr(rel_res->m_statusBegin).c_str(), LEXER->YYText()));
					break;
				}
			}
		}
		if (rel_res->m_statusEnd != rdoRuntime::RDOResource::CS_NoChange && rel_res->m_statusEnd != rdoRuntime::RDOResource::CS_Erase && rel_res->m_statusEnd != rdoRuntime::RDOResource::CS_NonExist)
		{
			switch (pPattern->getType())
			{
				case RDOPATPattern::PT_IE:
				case RDOPATPattern::PT_Event:
				case RDOPATPattern::PT_Rule:
				{
					PARSER->error().error(@1, _T("Внутренняя ошибка"));
					break;
				}
				case RDOPATPattern::PT_Operation:
				case RDOPATPattern::PT_Keyboard :
				{
					PARSER->error().error(@1, rdo::format(_T("%s ожидается ключевое слово Convert_end для релевантного ресурса '%s', т.к. его статус '%s', но найдено: %s"), str.c_str(), rel_res->name().c_str(), RDOPATPattern::StatusToStr(rel_res->m_statusBegin).c_str(), LEXER->YYText()));
					break;
				}
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_res_usage convert_begin pat_trace pat_convert_cmd
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		if (pPattern->getType() != RDOPATPattern::PT_Operation && pPattern->getType() != RDOPATPattern::PT_Keyboard)
		{
			tstring type = _T("");
			switch (pPattern->getType())
			{
				case RDOPATPattern::PT_IE:
				{
					type = _T("нерегулярном событии"); 
					break;
				}
				case RDOPATPattern::PT_Event:
				{
					type = _T("событии"); 
					break;
				}
				case RDOPATPattern::PT_Rule:
				{
					type = _T("продукционном правиле");
					break;
				}
			}
			PARSER->error().error(@2, rdo::format(_T("Ключевое слово Convert_begin может быть использовано в обыкновенной или клавиатурной операции, но не в %s '%s'"), type.c_str(), pPattern->name().c_str()));
		}
		LPConvertCmdList pCmdList = PARSER->stack().pop<ConvertCmdList>($4);
		pPattern.object_static_cast<RDOPatternOperation>()->addRelResConvertBeginEnd($3 != 0, pCmdList, false, NULL, @2, @2, @3, @3);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_res_usage convert_end pat_trace pat_convert_cmd
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		if (pPattern->getType() != RDOPATPattern::PT_Operation && pPattern->getType() != RDOPATPattern::PT_Keyboard)
		{
			tstring type = _T("");
			switch (pPattern->getType())
			{
				case RDOPATPattern::PT_IE:
				{
					type = _T("нерегулярном событии");
					break;
				}
				case RDOPATPattern::PT_Event:
				{
					type = _T("событии");
					break;
				}
				case RDOPATPattern::PT_Rule:
				{
					type = _T("продукционном правиле");
					break;
				}
			}
			PARSER->error().error(@2, rdo::format(_T("Ключевое слово Convert_end может быть использовано в обыкновенной и клавиатурной операции, но не в %s '%s'"), type.c_str(), pPattern->name().c_str()));
		}
		LPConvertCmdList pCmdList = PARSER->stack().pop<ConvertCmdList>($4);
		pPattern.object_static_cast<RDOPatternOperation>()->addRelResConvertBeginEnd(false, NULL, $3 != 0, pCmdList, @2, @2, @3, @3);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_res_usage convert_begin pat_trace pat_convert_cmd convert_end pat_trace pat_convert_cmd
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		if (pPattern->getType() != RDOPATPattern::PT_Operation && pPattern->getType() != RDOPATPattern::PT_Keyboard)
		{
			tstring type = _T("");
			switch (pPattern->getType())
			{
				case RDOPATPattern::PT_IE:
				{
					type = _T("нерегулярном событии");
					break;
				}
				case RDOPATPattern::PT_Event:
				{
					type = _T("событии");
					break;
				}
				case RDOPATPattern::PT_Rule:
				{
					type = _T("продукционном правиле");
					break;
				}
			}
			PARSER->error().error(@2, rdo::format(_T("Ключевые слова Convert_begin и Convert_end могут быть использованы в обыкновенной и клавиатурной операции, но не в %s '%s'"), type.c_str(), pPattern->name().c_str()));
		}
		LPConvertCmdList pCmdListBegin = PARSER->stack().pop<ConvertCmdList>($4);
		LPConvertCmdList pCmdListEnd   = PARSER->stack().pop<ConvertCmdList>($7);
		pPattern.object_static_cast<RDOPatternOperation>()->addRelResConvertBeginEnd($3 != 0, pCmdListBegin, $6 != 0, pCmdListEnd, @2, @5, @3, @6);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_res_usage convert_rule pat_trace pat_convert_cmd
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		if (pPattern->getType() != RDOPATPattern::PT_Rule)
		{
			tstring type = _T("");
			switch (pPattern->getType())
			{
				case RDOPATPattern::PT_IE:
				{
					type = _T("нерегулярном событии");
					break;
				}
				case RDOPATPattern::PT_Event:
				{
					type = _T("событии");
					break;
				}
				case RDOPATPattern::PT_Operation:
				{
					type = _T("операции");
					break;
				}
				case RDOPATPattern::PT_Keyboard :
				{
					type = _T("клавиатурной операции");
					break;
				}
			}
			PARSER->error().error(@2, rdo::format(_T("Ключевое слово Convert_rule может быть использовано в продукционном правиле, но не в %s '%s'"), type.c_str(), pPattern->name().c_str()));
		}
		LPConvertCmdList pCmdList = PARSER->stack().pop<ConvertCmdList>($4);
		ASSERT(pPattern->m_pCurrRelRes);
		pPattern->addRelResConvert($3 != 0, pCmdList, @2, @3, pPattern->m_pCurrRelRes->m_statusBegin);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_res_usage convert_event pat_trace pat_convert_cmd
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		if (pPattern->getType() != RDOPATPattern::PT_IE && pPattern->getType() != RDOPATPattern::PT_Event)
		{
			tstring type = _T("");
			switch (pPattern->getType())
			{
				case RDOPATPattern::PT_Rule     :
				{
					type = _T("продукционном правиле");
					break;
				}
				case RDOPATPattern::PT_Operation:
				{
					type = _T("операции");
					break;
				}
				case RDOPATPattern::PT_Keyboard :
				{
					type = _T("клавиатурной операции");
					break;
				}
			}
			PARSER->error().error(@2, rdo::format(_T("Ключевое слово Convert_event может быть использовано в событии или в нерегулярном событии, но не в %s '%s'"), type.c_str(), pPattern->name().c_str()));
		}
		LPConvertCmdList pCmdList = PARSER->stack().pop<ConvertCmdList>($4);
		ASSERT(pPattern->m_pCurrRelRes);
		pPattern->addRelResConvert($3 != 0, pCmdList, @2, @3, pPattern->m_pCurrRelRes->m_statusBegin);
		$$ = PARSER->stack().push(pPattern);
	}
	;

convert_rule
	: RDO_Convert_rule
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::convertBegin;
	}
	;

convert_event
	: RDO_Convert_event
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::convertBegin;
	}
	;

convert_begin
	: RDO_Convert_begin
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::convertBegin;
	}
	;

convert_end
	:	RDO_Convert_end
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::convertEnd;
	}
	;

pat_convert_cmd
	: /* empty */
	{
		LPConvertCmdList pCmdList = rdo::Factory<ConvertCmdList>::create();
		LPRDORelevantResource pRelRes = PARSER->getLastPATPattern()->m_pCurrRelRes;
		ASSERT(pRelRes);
		pRelRes->getParamSetList().reset();
		$$ = PARSER->stack().push(pCmdList);
	}
	| pat_convert_cmd statement
	{
		LPConvertCmdList      pCmdList = PARSER->stack().pop<ConvertCmdList>($1);
		rdoRuntime::LPRDOCalc pCalc    = PARSER->stack().pop<rdoRuntime::RDOCalc>($2);
		pCmdList->insertCommand(pCalc);
		$$ = PARSER->stack().push(pCmdList);
	}
	| pat_convert_cmd statement_old_style
	{
		LPConvertCmdList pCmdList = PARSER->stack().pop<ConvertCmdList>($1);
		YYLTYPE          statement_pos = @2;
		LPCorrection     pCorrection = new Correction(++statement_pos.m_last_line, ++statement_pos.m_last_pos, Semicolon);
		pCmdList->insertCorrection(pCorrection);
		$$ = PARSER->stack().push(pCmdList);
		PARSER->error().error(@2, rdo::format(_T("В позиции Ln %i Col %i не хватает точки с запятой"), statement_pos.m_last_line, statement_pos.m_last_pos));
	}
	;

statement_old_style
	: nochange_statement_old_style
	| equal_statement_old_style
	;

nochange_statement_old_style
	: RDO_IDENTIF_NoChange
	;

equal_statement_old_style
	: RDO_IDENTIF increment_or_decrement_type
	{
		$$ = $2;
	}
	| RDO_IDENTIF param_equal_type fun_arithm
	{
		$$ = $3;
	}
	;

statement
	: empty_statement
	| nochange_statement
	| equal_statement
	| stopping_statement
	| planning_statement
	| if_statement
	| '{' statement_list '}'
	{
		rdoRuntime::LPRDOCalcList pCalcList = PARSER->stack().pop<rdoRuntime::RDOCalcList>($2);
		ASSERT(pCalcList);
		rdoRuntime::LPRDOCalc pCalc = pCalcList;
		ASSERT(pCalc);
		$$ = PARSER->stack().push(pCalc);
	}
	;

statement_list
	: /* empty */
	{
		rdoRuntime::LPRDOCalcList pCalcList = rdo::Factory<rdoRuntime::RDOCalcList>::create();
		ASSERT(pCalcList);
		$$ = PARSER->stack().push(pCalcList);
	}
	| statement_list statement
	{
		rdoRuntime::LPRDOCalcList pCalcList = PARSER->stack().pop<rdoRuntime::RDOCalcList>($1);
		ASSERT(pCalcList);

		rdoRuntime::LPRDOCalc     pCalc     = PARSER->stack().pop<rdoRuntime::RDOCalc>($2);
		ASSERT(pCalc);

		pCalcList->addCalc(pCalc);

		$$ = PARSER->stack().push(pCalcList);
	}
	;

empty_statement
	: ';'
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcNoChange>::create();
		ASSERT(pCalc);
		$$ = PARSER->stack().push(pCalc);
	}
	| error ';'
	{
		PARSER->error().error(@1, _T("Ошибка в инструкции"));
	}
	;

nochange_statement
	: RDO_IDENTIF_NoChange ';'
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcNoChange>::create();
		ASSERT(pCalc);
		$$ = PARSER->stack().push(pCalc);
	}
	| RDO_IDENTIF_NoChange error
	{
		PARSER->error().error(@2, _T("Не найден символ окончания инструкции - точка с запятой"));
	}
	;

equal_statement
	: RDO_IDENTIF increment_or_decrement_type ';'
	{
		tstring                paramName = RDOVALUE($1)->getIdentificator();
		rdoRuntime::EqualType  equalType = static_cast<rdoRuntime::EqualType>($2);
		LPRDORelevantResource  pRelRes   = PARSER->getLastPATPattern()->m_pCurrRelRes;
		ASSERT(pRelRes);
		LPRDORTPParam param = pRelRes->getType()->findRTPParam(paramName);
		if (!param)
		{
			PARSER->error().error(@1, rdo::format(_T("Неизвестный параметр: %s"), paramName.c_str()));
		}
		rdoRuntime::LPRDOCalc pCalc;
		switch (equalType)
		{
			case rdoRuntime::ET_INCR:
			{
				pCalc = rdo::Factory<rdoRuntime::RDOSetRelParamCalc<rdoRuntime::ET_INCR> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName));
				break;
			}
			case rdoRuntime::ET_DECR:
			{
				pCalc = rdo::Factory<rdoRuntime::RDOSetRelParamCalc<rdoRuntime::ET_DECR> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName));
				break;
			}
			default:
			{
				NEVER_REACH_HERE;
			}
		}
		ASSERT(pCalc);
		//! Проверка на диапазон
		//! TODO: проверить работоспособность
		if (dynamic_cast<PTR(RDOTypeIntRange)>(param->getParamType().get()))
		{
			LPRDOTypeIntRange pRange = param->getParamType()->type().object_static_cast<RDOTypeIntRange>();
			pCalc = rdo::Factory<rdoRuntime::RDOSetRelParamDiapCalc>::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pRange->range()->getMin().value(), pRange->range()->getMax().value(), pCalc);
		}
		else if (dynamic_cast<PTR(RDOTypeRealRange)>(param->getParamType().get()))
		{
			LPRDOTypeRealRange pRange = param->getParamType()->type().object_static_cast<RDOTypeRealRange>();
			pCalc = rdo::Factory<rdoRuntime::RDOSetRelParamDiapCalc>::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pRange->range()->getMin().value(), pRange->range()->getMax().value(), pCalc);
		}
		tstring oprStr;
		switch (equalType)
		{
			case rdoRuntime::ET_INCR:
			{
				oprStr = _T("++");
				break;
			}
			case rdoRuntime::ET_DECR:
			{
				oprStr = _T("--");
				break;
			}
			default:
			{
				oprStr = _T("");
				break;
			}
		}
		pCalc->setSrcText(rdo::format(_T("%s %s %s"), paramName.c_str(), oprStr.c_str()));
		pCalc->setSrcPos (@1.m_first_line, @1.m_first_pos, @2.m_last_line, @2.m_last_pos);

		$$ = PARSER->stack().push(pCalc);
	}
	| RDO_IDENTIF param_equal_type fun_arithm ';'
	{
		tstring               paramName    = RDOVALUE($1)->getIdentificator();
		rdoRuntime::EqualType equalType    = static_cast<rdoRuntime::EqualType>($2);
		LPRDOFUNArithm        pRightArithm = PARSER->stack().pop<RDOFUNArithm>($3);
		LPRDORelevantResource pRelRes      = PARSER->getLastPATPattern()->m_pCurrRelRes;
		ASSERT(pRelRes);
		LPRDORTPParam param = pRelRes->getType()->findRTPParam(paramName);
		if (!param)
		{
			PARSER->error().error(@1, rdo::format(_T("Неизвестный параметр: %s"), paramName.c_str()));
		}
		rdoRuntime::LPRDOCalc pCalcRight = pRightArithm->createCalc(param->getParamType().get());
		rdoRuntime::LPRDOCalc pCalc;
		switch (equalType)
		{
			case rdoRuntime::ET_NOCHANGE:
			{
				break;
			}
			case rdoRuntime::ET_EQUAL:
			{
				pCalc = rdo::Factory<rdoRuntime::RDOSetRelParamCalc<rdoRuntime::ET_EQUAL> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pCalcRight);
				pRelRes->getParamSetList().insert(param);
				break;
			}
			case rdoRuntime::ET_PLUS:
			{
				pCalc = rdo::Factory<rdoRuntime::RDOSetRelParamCalc<rdoRuntime::ET_PLUS> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pCalcRight);
				break;
			}
			case rdoRuntime::ET_MINUS:
			{
				pCalc = rdo::Factory<rdoRuntime::RDOSetRelParamCalc<rdoRuntime::ET_MINUS> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pCalcRight);
				break;
			}
			case rdoRuntime::ET_MULTIPLY:
			{
				pCalc = rdo::Factory<rdoRuntime::RDOSetRelParamCalc<rdoRuntime::ET_MULTIPLY> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pCalcRight);
				break;
			}
			case rdoRuntime::ET_DIVIDE:
			{
				pCalc = rdo::Factory<rdoRuntime::RDOSetRelParamCalc<rdoRuntime::ET_DIVIDE> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pCalcRight);
				break;
			}
			default:
			{
				NEVER_REACH_HERE;
			}
		}
		ASSERT(pCalc);
		//! Проверка на диапазон
		//! TODO: проверить работоспособность
		if (dynamic_cast<PTR(RDOTypeIntRange)>(param->getParamType().get()))
		{
			LPRDOTypeIntRange pRange = param->getParamType()->type().object_static_cast<RDOTypeIntRange>();
			pCalc = rdo::Factory<rdoRuntime::RDOSetRelParamDiapCalc>::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pRange->range()->getMin().value(), pRange->range()->getMax().value(), pCalc);
		}
		else if (dynamic_cast<PTR(RDOTypeRealRange)>(param->getParamType().get()))
		{
			LPRDOTypeRealRange pRange = param->getParamType()->type().object_static_cast<RDOTypeRealRange>();
			pCalc = rdo::Factory<rdoRuntime::RDOSetRelParamDiapCalc>::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pRange->range()->getMin().value(), pRange->range()->getMax().value(), pCalc);
		}
		tstring oprStr;
		switch (equalType)
		{
			case rdoRuntime::ET_EQUAL:
			{
				oprStr = _T("=");
				break;
			}
			case rdoRuntime::ET_PLUS:
			{
				oprStr = _T("+=");
				break;
			}
			case rdoRuntime::ET_MINUS:
			{
				oprStr = _T("-=");
				break;
			}
			case rdoRuntime::ET_MULTIPLY:
			{
				oprStr = _T("*=");
				break;
			}
			case rdoRuntime::ET_DIVIDE:
			{
				oprStr = _T("/=");
				break;
			}
			default:
			{
				oprStr = _T("");
				break;
			}
		}
		pCalc->setSrcText(rdo::format(_T("%s %s %s"), paramName.c_str(), oprStr.c_str(), pCalcRight->src_text().c_str()));
		pCalc->setSrcPos (@1.m_first_line, @1.m_first_pos, @3.m_last_line, @3.m_last_pos);

		$$ = PARSER->stack().push(pCalc);
	}
	| RDO_IDENTIF param_equal_type error
	{
		PARSER->error().error(@3, _T("Ошибка в арифметическом выражении"));
	}
	| RDO_IDENTIF param_equal_type fun_arithm error
	{
		PARSER->error().error(@4, _T("Не найден символ окончания инструкции - точка с запятой"));
	}
	| RDO_IDENTIF error fun_arithm
	{
		PARSER->error().error(@2, _T("Ошибка в операторе присваивания"));
	}
	;

stopping_statement
	: RDO_IDENTIF '.' RDO_Stopping ';'
	{
		tstring           eventName   = RDOVALUE($1)->getIdentificator();
		LPRDOEvent        pEvent      = PARSER->findEvent(eventName);
		if (!pEvent)
		{
			PARSER->error().error(@1, rdo::format(_T("Попытка запланировать неизвестное событие: %s"), eventName.c_str()));
		}

		rdoRuntime::LPRDOCalcEventStop pCalc = rdo::Factory<rdoRuntime::RDOCalcEventStop>::create();
		ASSERT(pCalc);
		pEvent->attachCalc(pCalc);

		$$ = PARSER->stack().push(pCalc);
	}
	| RDO_IDENTIF '.' RDO_Stopping error
	{
		PARSER->error().error(@4, _T("Не найден символ окончания инструкции - точка с запятой"));
	}
	;

planning_statement
	: RDO_IDENTIF '.' RDO_Planning '(' fun_arithm event_descr_param ')' ';'
	{
		tstring        eventName   = RDOVALUE($1)->getIdentificator();
		LPRDOFUNArithm pTimeArithm = PARSER->stack().pop<RDOFUNArithm>($5);
		LPRDOEvent     pEvent      = PARSER->findEvent(eventName);
		if (!pEvent)
		{
			PARSER->error().error(@1, rdo::format(_T("Попытка запланировать неизвестное событие: %s"), eventName.c_str()));
		}

		rdoRuntime::LPRDOCalc pCalcTime = pTimeArithm->createCalc(NULL);
		ASSERT(pCalcTime);

		rdoRuntime::LPRDOCalcEventPlan pCalc = rdo::Factory<rdoRuntime::RDOCalcEventPlan>::create(pCalcTime);
		ASSERT(pCalc);
		pEvent->attachCalc(pCalc);

		$$ = PARSER->stack().push(pCalc);
	}
	| RDO_IDENTIF '.' RDO_Planning '(' fun_arithm event_descr_param ')' error
	{
		PARSER->error().error(@7, _T("Не найден символ окончания инструкции - точка с запятой"));
	}
	| RDO_IDENTIF '.' RDO_Planning '(' error
	{
		PARSER->error().error(@5, _T("Ошибка в арифметическом выражении"));
	}
	| RDO_IDENTIF '.' RDO_Planning error
	{
		PARSER->error().error(@4, _T("Ожидается открывающая скобка"));
	}
	| RDO_IDENTIF '.' RDO_Planning '(' fun_arithm event_descr_param error
	{
		PARSER->error().error(@6, _T("Ожидается закрывающая скобка"));
	}
	;

event_descr_param
	: /* empty */
	| event_descr_param ',' '*'
	{
		PARSER->error().error(@1, @2, "Планировать события с параметрами пока нельзя")
	}
	| event_descr_param ',' fun_arithm
	{
		PARSER->error().error(@1, @2, "Планировать события с параметрами пока нельзя")
	}
	| event_descr_param ',' error
	{
		PARSER->error().error(@1, @2, "Ошибка описания параметра события")
	}
	;

if_statement
	: RDO_if '(' fun_logic ')' statement
	{
		LPRDOFUNLogic pCondition = PARSER->stack().pop<RDOFUNLogic>($3);
		ASSERT(pCondition);
		
		rdoRuntime::LPRDOCalc pConditionCalc = pCondition->getCalc();
		ASSERT(pConditionCalc);

		rdoRuntime::LPRDOCalc pStatementCalc = PARSER->stack().pop<rdoRuntime::RDOCalc>($5);
		ASSERT(pStatementCalc);

		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcIf>::create(pConditionCalc, pStatementCalc);
		ASSERT(pCalc);

		$$ = PARSER->stack().push(pCalc);
	}
	| RDO_if '(' fun_logic ')' statement RDO_else statement
	{
		LPRDOFUNLogic pCondition = PARSER->stack().pop<RDOFUNLogic>($3);
		ASSERT(pCondition);
		
		rdoRuntime::LPRDOCalc pConditionCalc = pCondition->getCalc();
		ASSERT(pConditionCalc);

		rdoRuntime::LPRDOCalc pIfStatementCalc = PARSER->stack().pop<rdoRuntime::RDOCalc>($5);
		ASSERT(pIfStatementCalc);

		rdoRuntime::LPRDOCalc pElseStatementCalc = PARSER->stack().pop<rdoRuntime::RDOCalc>($7);
		ASSERT(pElseStatementCalc);

		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcIfElse>::create(pConditionCalc, pIfStatementCalc, pElseStatementCalc);
		ASSERT(pCalc);
		
		$$ = PARSER->stack().push(pCalc);
	}
	| RDO_if error fun_logic
	{
		PARSER->error().error(@2, _T("Ожидается открывающая скобка"));
	}
	| RDO_if '(' fun_logic error
	{
		PARSER->error().error(@4, _T("Ожидается закрывающая скобка"));
	}
	;

increment_or_decrement_type
	: RDO_IncrEqual
	{
		$$ = rdoRuntime::ET_INCR;
	}
	| RDO_DecrEqual
	{
		$$ = rdoRuntime::ET_DECR;
	}
	;

param_equal_type
	: RDO_set
	{
		$$ = rdoRuntime::ET_EQUAL;
	}
	| '='
	{
		$$ = rdoRuntime::ET_EQUAL;
	}
	| RDO_PlusEqual
	{
		$$ = rdoRuntime::ET_PLUS;
	}
	| RDO_MinusEqual
	{
		$$ = rdoRuntime::ET_MINUS;
	}
	| RDO_MultiplyEqual
	{
		$$ = rdoRuntime::ET_MULTIPLY;
	}
	| RDO_DivideEqual
	{
		$$ = rdoRuntime::ET_DIVIDE;
	}
	;

pat_pattern
	: pat_convert RDO_End
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		pPattern->end();
		$$ = PARSER->stack().push(pPattern);
	}
	;

// ----------------------------------------------------------------------------
// ---------- Описание типа параметра
// ----------------------------------------------------------------------------
param_type
	: RDO_integer param_type_range param_value_default
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
			ASSERT(pIntRange);
			pType = rdo::Factory<RDOTypeParam>::create(pIntRange, RDOVALUE($3), RDOParserSrcInfo(@1, @3));
		}
		else
		{
			pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__int>::create(), RDOVALUE($3), RDOParserSrcInfo(@1, @3));
		}
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_real param_type_range param_value_default
	{
		LPRDOTypeRangeRange pRange = PARSER->stack().pop<RDOTypeRangeRange>($2);
		LPRDOTypeParam pType;
		if (pRange)
		{
			LPRDOTypeRealRange pRealRange = rdo::Factory<RDOTypeRealRange>::create(pRange);
			ASSERT(pRealRange);
			pType = rdo::Factory<RDOTypeParam>::create(pRealRange, RDOVALUE($3), RDOParserSrcInfo(@1, @3));
		}
		else
		{
			pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__real>::create(), RDOVALUE($3), RDOParserSrcInfo(@1, @3));
		}
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_string param_value_default
	{
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__string>::create(), RDOVALUE($2), RDOParserSrcInfo(@1, @2));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| param_array param_value_default
	{
		LEXER->array_cnt_rst();
		LPRDOArrayType pArray = PARSER->stack().pop<RDOArrayType>($1);
		ASSERT(pArray);
		LPRDOTypeParam pType  = rdo::Factory<RDOTypeParam>::create(pArray, RDOVALUE($2), RDOParserSrcInfo(@1, @2));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_bool param_value_default
	{
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__bool>::create(), RDOVALUE($2), RDOParserSrcInfo(@1, @2));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_enum param_value_default
	{
		LEXER->enumReset();
		LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
		ASSERT(pEnum);
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(pEnum, RDOVALUE($2), RDOParserSrcInfo(@1, @2));
		ASSERT(pType);
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
			{
				default = pTypeSuchAs->default();
			}
		}
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(pTypeSuchAs->type(), default, RDOParserSrcInfo(@1, @2));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	;
	//| RDO_integer error
	//{
	//	PARSER->error().error(@2, _T("Ошибка после ключевого слова integer. Возможно, не хватает значения по-умолчанию."));
	//}
	//| RDO_real error
	//{
	//	PARSER->error().error(@2, _T("Ошибка после ключевого слова real. Возможно, не хватает значения по-умолчанию."));
	//}
	//| param_type_enum error
	//{
	//	PARSER->error().error(@2, _T("Ошибка после перечислимого типа. Возможно, не хватает значения по-умолчанию."));
	//}

param_type_range
	: /* empty */
	{
		$$ = PARSER->stack().push<RDOTypeRangeRange>(LPRDOTypeRangeRange());
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = PARSER->stack().push(pRange);
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = PARSER->stack().push(pRange);
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = PARSER->stack().push(pRange);
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = PARSER->stack().push(pRange);
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST error
	{
		PARSER->error().error(@4, _T("Диапазон задан неверно"));
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST error
	{
		PARSER->error().error(@4, _T("Диапазон задан неверно"));
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST error
	{
		PARSER->error().error(@4, _T("Диапазон задан неверно"));
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST error
	{
		PARSER->error().error(@4, _T("Диапазон задан неверно"));
	}
	| '[' RDO_REAL_CONST RDO_dblpoint error
	{
		PARSER->error().error(@4, _T("Диапазон задан неверно"));
	}
	| '[' RDO_INT_CONST RDO_dblpoint error
	{
		PARSER->error().error(@4, _T("Диапазон задан неверно"));
	}
	| '[' error
	{
		PARSER->error().error(@2, _T("Диапазон задан неверно"));
	}
	;

param_type_enum
	: '(' param_type_enum_list ')'
	{
		LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($2);
		ASSERT(pEnum);
		$$ = PARSER->stack().push(pEnum);
	}
	| '(' param_type_enum_list error
	{
		PARSER->error().error(@2, _T("Перечисление должно заканчиваться скобкой"));
	}
	;

param_type_enum_list
	: RDO_IDENTIF
	{
		LPRDOEnumType pEnum = rdo::Factory<RDOEnumType>::create();
		ASSERT(pEnum);
		pEnum->add(RDOVALUE($1));
		LEXER->enumBegin();
		$$ = PARSER->stack().push(pEnum);
	}
	| param_type_enum_list ',' RDO_IDENTIF
	{
		if (!LEXER->enumEmpty())
		{
			LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
			ASSERT(pEnum);
			pEnum->add(RDOVALUE($3));
			$$ = PARSER->stack().push(pEnum);
		}
		else
		{
			PARSER->error().error(@3, _T("Ошибка в описании значений перечислимого типа"));
		}
	}
	| param_type_enum_list RDO_IDENTIF
	{
		if (!LEXER->enumEmpty())
		{
			LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
			ASSERT(pEnum);
			pEnum->add(RDOVALUE($2));
			$$ = PARSER->stack().push(pEnum);
			PARSER->error().warning(@1, rdo::format(_T("Пропущена запятая перед: %s"), RDOVALUE($2)->getIdentificator().c_str()));
		}
		else
		{
			PARSER->error().error(@2, _T("Ошибка в описании значений перечислимого типа"));
		}
	}
	| param_type_enum_list ',' RDO_INT_CONST
	{
		PARSER->error().error(@3, _T("Значение перечислимого типа не может быть цифрой"));
	}
	| param_type_enum_list ',' RDO_REAL_CONST
	{
		PARSER->error().error(@3, _T("Значение перечислимого типа не может быть цифрой"));
	}
	| param_type_enum_list RDO_INT_CONST
	{
		PARSER->error().error(@2, _T("Значение перечислимого типа не может быть цифрой"));
	}
	| param_type_enum_list RDO_REAL_CONST
	{
		PARSER->error().error(@2, _T("Значение перечислимого типа не может быть цифрой"));
	}
	| RDO_INT_CONST
	{
		PARSER->error().error(@1, _T("Значение перечислимого типа не может начинаться с цифры"));
	}
	| RDO_REAL_CONST
	{
		PARSER->error().error(@1, _T("Значение перечислимого типа не может начинаться с цифры"));
	}
	;

param_type_such_as
	: RDO_such_as RDO_IDENTIF '.' RDO_IDENTIF
	{
		tstring type  = RDOVALUE($2)->getIdentificator();
		tstring param = RDOVALUE($4)->getIdentificator();
		LPRDORTPResType pResType = PARSER->findRTPResType(type);
		if (!pResType)
		{
			PARSER->error().error(@2, rdo::format(_T("Ссылка на неизвестный тип ресурса: %s"), type.c_str()));
		}
		LPRDORTPParam pParam = pResType->findRTPParam(param);
		if (!pParam)
		{
			PARSER->error().error(@4, rdo::format(_T("Ссылка на неизвестный параметр ресурса: %s.%s"), type.c_str(), param.c_str()));
		}
		$$ = PARSER->stack().push(pParam->getParamType());
	}
	| RDO_such_as RDO_IDENTIF
	{
		tstring constName = RDOVALUE($2)->getIdentificator();
		LPRDOFUNConstant pConstant = PARSER->findFUNConstant(constName);
		if (!pConstant)
		{
			PARSER->error().error(@2, rdo::format(_T("Ссылка на несуществующую константу: %s"), constName.c_str()));
		}
		$$ = PARSER->stack().push(pConstant->getType());
	}
	| RDO_such_as RDO_IDENTIF '.' error
	{
		tstring type = RDOVALUE($2)->getIdentificator();
		LPRDORTPResType pResType = PARSER->findRTPResType(type);
		if (!pResType)
		{
			PARSER->error().error(@2, rdo::format(_T("Ссылка на неизвестный тип ресурса: %s"), type.c_str()));
		}
		else
		{
			PARSER->error().error(@4, _T("Ошибка при указании параметра"));
		}
	}
	| RDO_such_as error
	{
		PARSER->error().error(@2, _T("После ключевого слова such_as необходимо указать тип и параметер ресурса для ссылки"));
	}
	;

param_value_default
	: /* empty */
	{
		$$ = (int)PARSER->addValue(new rdoParse::RDOValue());
	}
	| '=' RDO_INT_CONST
	{
		$$ = $2;
	}
	| '=' RDO_REAL_CONST
	{
		$$ = $2;
	}
	| '=' RDO_STRING_CONST
	{
		$$ = $2;
	}
	| '=' RDO_IDENTIF
	{
		$$ = $2;
	}
	| '=' RDO_BOOL_CONST
	{
		$$ = $2;
	}
	| '=' error
	{
		RDOParserSrcInfo src_info(@1, @2, true);
		if (src_info.src_pos().point())
		{
			PARSER->error().error(src_info, _T("Не указано значение по-умолчанию"));
		}
		else
		{
			PARSER->error().error(src_info, _T("Неверное значение по-умолчанию"));
		}
	}
	;

param_array
	: RDO_array '<' param_type '>'
	{
		LPRDOArrayType pArray = PARSER->stack().pop<RDOArrayType>($2);
		ASSERT(pArray);
		$$ = PARSER->stack().push(pArray);
	}
	;

// ----------------------------------------------------------------------------
// ---------- Общие составные токены для всех объектов РДО
// ----------------------------------------------------------------------------
// ---------- Логические выражения
// ----------------------------------------------------------------------------
fun_logic_eq
	: RDO_eq { $$ = RDO_eq; }
	;

fun_logic
	: fun_arithm fun_logic_eq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator ==(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm RDO_neq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator !=(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm '<' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator <(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm '>' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator >(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm RDO_leq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator <=(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm RDO_geq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator >=(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_logic RDO_and fun_logic
	{
		LPRDOFUNLogic pLogic1 = PARSER->stack().pop<RDOFUNLogic>($1);
		LPRDOFUNLogic pLogic2 = PARSER->stack().pop<RDOFUNLogic>($3);
		ASSERT(pLogic1);
		ASSERT(pLogic2);
		LPRDOFUNLogic pResult = pLogic1->operator &&(pLogic2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_logic RDO_or fun_logic
	{
		LPRDOFUNLogic pLogic1 = PARSER->stack().pop<RDOFUNLogic>($1);
		LPRDOFUNLogic pLogic2 = PARSER->stack().pop<RDOFUNLogic>($3);
		ASSERT(pLogic1);
		ASSERT(pLogic2);
		LPRDOFUNLogic pResult = pLogic1->operator ||(pLogic2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($1);
		ASSERT(pArithm);
		LPRDOFUNLogic pResult = rdo::Factory<RDOFUNLogic>::create(pArithm);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm '=' fun_arithm
	{
		YYLTYPE          equal_pos = @2;
		LPCorrection     pCorrection = new Correction(++equal_pos.m_last_line, ++equal_pos.m_last_pos, Equality);
		PARSER->error().error(@2, rdo::format(_T("В позиции Ln %i Col %i не хватает знака равенства"), equal_pos.m_last_line, equal_pos.m_last_pos));
	}
	| fun_group
	| fun_select_logic
	| '[' fun_logic ']'
	{
		LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		pLogic->setSrcPos (@1, @3);
		pLogic->setSrcText(_T("[") + pLogic->src_text() + _T("]"));
		$$ = PARSER->stack().push(pLogic);
	}
	| '(' fun_logic ')'
	{
		LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		pLogic->setSrcPos (@1, @3);
		pLogic->setSrcText(_T("(") + pLogic->src_text() + _T(")"));
		$$ = PARSER->stack().push(pLogic);
	}
	| RDO_not fun_logic
	{
		LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		LPRDOFUNLogic pLogicNot = pLogic->operator_not();
		ASSERT(pLogicNot);
		pLogicNot->setSrcPos (@1, @2);
		pLogicNot->setSrcText(_T("not ") + pLogic->src_text());
		$$ = PARSER->stack().push(pLogicNot);
	}
	| '[' fun_logic error
	{
		PARSER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
	}
	| '(' fun_logic error
	{
		PARSER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- Арифметические выражения
// ----------------------------------------------------------------------------
fun_arithm
	: RDO_INT_CONST                      { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
	| RDO_REAL_CONST                     { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
	| RDO_BOOL_CONST                     { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
	| RDO_STRING_CONST                   { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
	| RDO_IDENTIF                        { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
	| RDO_IDENTIF '.' RDO_IDENTIF        { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1), RDOVALUE($3))); }
	| RDO_IDENTIF_RELRES '.' RDO_IDENTIF { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1), RDOVALUE($3))); }
	| fun_arithm '+' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator +(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm '-' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator -(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm '*' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator *(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm '/' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator /(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm_func_call
	| fun_select_arithm
	| '(' fun_arithm ')'
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		pArithm->setSrcPos (@1, @3);
		pArithm->setSrcText(_T("(") + pArithm->src_text() + _T(")"));
		$$ = PARSER->stack().push(pArithm);
	}
	| '-' fun_arithm %prec RDO_UMINUS
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		RDOParserSrcInfo info;
		info.setSrcPos (@1, @2);
		info.setSrcText(_T("-") + pArithm->src_text());
		$$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOValue(pArithm->type(), info), rdo::Factory<rdoRuntime::RDOCalcUMinus>::create(pArithm->createCalc())));
	}
	;

// ----------------------------------------------------------------------------
// ---------- Функции и последовательности
// ----------------------------------------------------------------------------
fun_arithm_func_call
	: RDO_IDENTIF '(' ')'
	{
		LPRDOFUNParams pFunParams = rdo::Factory<RDOFUNParams>::create();
		ASSERT(pFunParams);
		tstring funName = RDOVALUE($1)->getIdentificator();
		pFunParams->getFunseqName().setSrcInfo(RDOParserSrcInfo(@1, funName));
		pFunParams->setSrcPos (@1, @3);
		pFunParams->setSrcText(funName + _T("()"));
		LPRDOFUNArithm pArithm = pFunParams->createCall(funName);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
	| RDO_IDENTIF '(' fun_arithm_func_call_pars ')'
	{
		LPRDOFUNParams pFunParams = PARSER->stack().pop<RDOFUNParams>($3);
		ASSERT(pFunParams);
		tstring funName = RDOVALUE($1)->getIdentificator();
		pFunParams->getFunseqName().setSrcInfo(RDOParserSrcInfo(@1, funName));
		pFunParams->setSrcPos (@1, @4);
		pFunParams->setSrcText(funName + _T("(") + pFunParams->src_text() + _T(")"));
		LPRDOFUNArithm pArithm = pFunParams->createCall(funName);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
	| RDO_IDENTIF '(' error
	{
		PARSER->error().error(@3, _T("Ошибка в параметрах функции"));
	}
	;

fun_arithm_func_call_pars
	: fun_arithm
	{
		LPRDOFUNParams pFunParams = rdo::Factory<RDOFUNParams>::create();
		LPRDOFUNArithm pArithm    = PARSER->stack().pop<RDOFUNArithm>($1);
		ASSERT(pFunParams);
		ASSERT(pArithm   );
		pFunParams->setSrcText  (pArithm->src_text());
		pFunParams->addParameter(pArithm);
		$$ = PARSER->stack().push(pFunParams);
	}
	| fun_arithm_func_call_pars ',' fun_arithm
	{
		LPRDOFUNParams pFunParams = PARSER->stack().pop<RDOFUNParams>($1);
		LPRDOFUNArithm pArithm    = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pFunParams);
		ASSERT(pArithm   );
		pFunParams->setSrcText  (pFunParams->src_text() + _T(", ") + pArithm->src_text());
		pFunParams->addParameter(pArithm);
		$$ = PARSER->stack().push(pFunParams);
	}
	| fun_arithm_func_call_pars error
	{
		PARSER->error().error(@2, _T("Ошибка в арифметическом выражении"));
	}
	| fun_arithm_func_call_pars ',' error
	{
		PARSER->error().error(@3, _T("Ошибка в арифметическом выражении"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- Групповые выражения
// ----------------------------------------------------------------------------
fun_group_keyword
	: RDO_Exist       { $$ = RDOFUNGroupLogic::fgt_exist;     }
	| RDO_Not_Exist   { $$ = RDOFUNGroupLogic::fgt_notexist;  }
	| RDO_For_All     { $$ = RDOFUNGroupLogic::fgt_forall;    }
	| RDO_Not_For_All { $$ = RDOFUNGroupLogic::fgt_notforall; }
	;

fun_group_header
	: fun_group_keyword '(' RDO_IDENTIF_COLON
	{
		PTR(RDOValue) type_name = P_RDOVALUE($3);
		$$ = PARSER->stack().push(rdo::Factory<RDOFUNGroupLogic>::create((RDOFUNGroupLogic::FunGroupType)$1, type_name->src_info()));
	}
	| fun_group_keyword '(' error
	{
		PARSER->error().error(@3, _T("Ожидается имя типа"));
	}
	| fun_group_keyword error
	{
		PARSER->error().error(@1, _T("После имени функции ожидается октрывающаяся скобка"));
	}
	;

fun_group
	: fun_group_header fun_logic ')'
	{
		LPRDOFUNGroupLogic pGroupFun = PARSER->stack().pop<RDOFUNGroupLogic>($1);
		LPRDOFUNLogic      pLogic    = PARSER->stack().pop<RDOFUNLogic>     ($2);
		ASSERT(pGroupFun);
		ASSERT(pLogic   );
		pGroupFun->setSrcPos(@1, @3);
		$$ = PARSER->stack().push(pGroupFun->createFunLogic(pLogic));
	}
	| fun_group_header RDO_NoCheck ')'
	{
		LPRDOFUNGroupLogic pGroupFun = PARSER->stack().pop<RDOFUNGroupLogic>($1);
		ASSERT(pGroupFun);
		pGroupFun->setSrcPos(@1, @3);
		LPRDOFUNLogic pTrueLogic = rdo::Factory<RDOFUNLogic>::create(rdo::Factory<rdoRuntime::RDOCalcConst>::create(1), false);
		ASSERT(pTrueLogic);
		pTrueLogic->setSrcPos (@2);
		pTrueLogic->setSrcText(_T("NoCheck"));
		$$ = PARSER->stack().push(pGroupFun->createFunLogic(pTrueLogic));
	}
	| fun_group_header fun_logic error
	{
		PARSER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
	}
	| fun_group_header RDO_NoCheck error
	{
		PARSER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
	}
	| fun_group_header error
	{
		PARSER->error().error(@1, @2, _T("Ошибка в логическом выражении"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- Select
// ----------------------------------------------------------------------------
fun_select_header
	: RDO_Select '(' RDO_IDENTIF_COLON
	{
		PTR(RDOValue)  type_name = P_RDOVALUE($3);
		LPRDOFUNSelect pSelect   = rdo::Factory<RDOFUNSelect>::create(type_name->src_info());
		ASSERT(pSelect);
		pSelect->setSrcText(_T("Select(") + type_name->value().getIdentificator() + _T(": "));
		$$ = PARSER->stack().push(pSelect);
	}
	| RDO_Select '(' error
	{
		PARSER->error().error(@3, _T("Ожидается имя типа"));
	}
	| RDO_Select error
	{
		PARSER->error().error(@1, _T("Ожидается октрывающаяся скобка"));
	}
	;

fun_select_body
	: fun_select_header fun_logic ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		LPRDOFUNLogic  pLogic  = PARSER->stack().pop<RDOFUNLogic> ($2);
		ASSERT(pSelect);
		ASSERT(pLogic );
		pSelect->setSrcText(pSelect->src_text() + pLogic->src_text() + _T(")"));
		pSelect->initSelect(pLogic);
		$$ = PARSER->stack().push(pSelect);
	}
	| fun_select_header RDO_NoCheck ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		RDOParserSrcInfo logicInfo(@2, _T("NoCheck"));
		pSelect->setSrcText(pSelect->src_text() + logicInfo.src_text() + _T(")"));
		rdoRuntime::LPRDOCalcConst pCalc  = rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);
		ASSERT(pCalc);
		LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pCalc, true);
		ASSERT(pLogic);
		pLogic->setSrcInfo(logicInfo);
		pSelect->initSelect(pLogic);
		$$ = PARSER->stack().push(pSelect);
	}
	| fun_select_header fun_logic error
	{
		PARSER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
	}
	| fun_select_header RDO_NoCheck error
	{
		PARSER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
	}
	| fun_select_header error
	{
		PARSER->error().error(@1, @2, _T("Ошибка в логическом выражении"));
	}
	;

fun_select_keyword
	: RDO_Exist			{ $$ = RDOFUNGroupLogic::fgt_exist;     }
	| RDO_Not_Exist		{ $$ = RDOFUNGroupLogic::fgt_notexist;  }
	| RDO_For_All		{ $$ = RDOFUNGroupLogic::fgt_forall;    }
	| RDO_Not_For_All	{ $$ = RDOFUNGroupLogic::fgt_notforall; }
	;

fun_select_logic
	: fun_select_body '.' fun_select_keyword '(' fun_logic ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		LPRDOFUNLogic  pLogic  = PARSER->stack().pop<RDOFUNLogic> ($5);
		ASSERT(pSelect);
		ASSERT(pLogic );
		pSelect->setSrcPos(@1, @6);
		LPRDOFUNLogic pLogicSelect = pSelect->createFunSelectGroup((RDOFUNGroupLogic::FunGroupType)$3, pLogic);
		ASSERT(pLogicSelect);
		$$ = PARSER->stack().push(pLogicSelect);
	}
	| fun_select_body '.' fun_select_keyword '(' error
	{
		PARSER->error().error(@4, @5, _T("Ошибка в логическом выражении"));
	}
	| fun_select_body '.' fun_select_keyword error
	{
		PARSER->error().error(@3, _T("Ожидается октрывающаяся скобка"));
	}
	| fun_select_body '.' RDO_Empty '(' ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		pSelect->setSrcPos(@1, @5);
		RDOParserSrcInfo emptyInfo(@3, @5, _T("Empty()"));
		LPRDOFUNLogic pLogic = pSelect->createFunSelectEmpty(emptyInfo);
		ASSERT(pLogic);
		$$ = PARSER->stack().push(pLogic);
	}
	| fun_select_body '.' RDO_Empty '(' error
	{
		PARSER->error().error(@4, _T("Ожидается закрывающаяся скобка"));
	}
	| fun_select_body '.' RDO_Empty error
	{
		PARSER->error().error(@3, _T("Ожидается октрывающаяся скобка"));
	}
	| fun_select_body '.' error
	{
		PARSER->error().error(@2, @3, _T("Ожидается метод списка ресурсов"));
	}
	| fun_select_body error
	{
		PARSER->error().error(@1, _T("Ожидается '.' (точка) для вызова метода списка ресурсов"));
	}
	;

fun_select_arithm
	: fun_select_body '.' RDO_Size '(' ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		pSelect->setSrcPos(@1, @5);
		RDOParserSrcInfo sizeInfo(@3, @5, _T("Size()"));
		LPRDOFUNArithm pArithm = pSelect->createFunSelectSize(sizeInfo);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
	| fun_select_body '.' RDO_Size error
	{
		PARSER->error().error(@3, _T("Ожидается октрывающаяся скобка"));
	}
	| fun_select_body '.' RDO_Size '(' error
	{
		PARSER->error().error(@4, _T("Ожидается закрывающаяся скобка"));
	}
	;

%%

CLOSE_RDO_PARSER_NAMESPACE

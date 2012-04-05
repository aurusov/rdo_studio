/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoopr.y
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
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
%token RDO_CF							370
%token RDO_Priority						371
%token RDO_prior						372
%token RDO_Parent						373

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
// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdoparser_lexer.h"
#include "converter/smr2rdox/rdoopr.h"
#include "converter/smr2rdox/rdopat.h"
#include "converter/smr2rdox/rdofun.h"
#include "converter/smr2rdox/update/update.h"
// --------------------------------------------------------------------------------

#define CONVERTER  LEXER->converter()
#define RUNTIME    CONVERTER->runtime()

OPEN_RDO_CONVERTER_NAMESPACE
%}

%%

opr_main
	: /* empty */
	| opr_end
	{
		rdoConverter::LPDocUpdate pOperationsMove = rdo::Factory<rdoConverter::UpdateMove>::create(
			@1.m_first_seek,
			@1.m_last_seek,
			0,
			rdoConverter::IDocument::DPT
		);
		ASSERT(pOperationsMove);
		CONVERTER->insertDocUpdate(pOperationsMove);
	}
	| error
	{
		CONVERTER->error().error(@1, _T("Неизвестная ошибка"));
	}
	;

opr_header
	: RDO_Operations
	{
		rdoConverter::LPDocUpdate pOperationsReplace = rdo::Factory<rdoConverter::UpdateReplace>::create(
			@1.m_first_seek,
			@1.m_last_seek,
			_T("$Decision_point Operations: some\n$Activities")
		);
		ASSERT(pOperationsReplace);
		CONVERTER->insertDocUpdate(pOperationsReplace);

		LPRDOOperations pOperations = rdo::Factory<RDOOperations>::create(@1);
		ASSERT(pOperations);
		$$ = CONVERTER->stack().push(pOperations);
	}
	;

opr_body
	: opr_header RDO_IDENTIF_COLON RDO_IDENTIF
	{
		LPRDOOperations pOperations = CONVERTER->stack().pop<RDOOperations>($1);
		ASSERT(pOperations);
		LPRDOValue pName    = CONVERTER->stack().pop<RDOValue>($2);
		LPRDOValue pPattern = CONVERTER->stack().pop<RDOValue>($3);
		LPRDOOPROperation pOperation = pOperations->addNewActivity(pName->src_info(), pPattern->src_info());
		ASSERT(pOperation);

		if (pOperation->pattern()->getType() == RDOPATPattern::PT_IE)
		{
			rdoConverter::LPDocUpdate pIEDelete = rdo::Factory<rdoConverter::UpdateDelete>::create(
				@2.m_first_seek,
				@3.m_last_seek
			);
			ASSERT(pIEDelete);
			CONVERTER->insertDocUpdate(pIEDelete);
		}

		$$ = CONVERTER->stack().push(pOperation);
	}
	| opr_param RDO_IDENTIF_COLON RDO_IDENTIF
	{
		LPRDOOPROperation pOperation = CONVERTER->stack().pop<RDOOPROperation>($1);
		ASSERT(pOperation);
		pOperation->endParam(@1);

		LPRDOOperations pOperations = CONVERTER->getLastOperations();
		ASSERT(pOperations);
		LPRDOValue pName    = CONVERTER->stack().pop<RDOValue>($2);
		LPRDOValue pPattern = CONVERTER->stack().pop<RDOValue>($3);
		pOperation = pOperations->addNewActivity(pName->src_info(), pPattern->src_info());
		ASSERT(pOperation);

		if (pOperation->pattern()->getType() == RDOPATPattern::PT_IE)
		{
			rdoConverter::LPDocUpdate pIEDelete = rdo::Factory<rdoConverter::UpdateDelete>::create(
				@2.m_first_seek,
				@3.m_last_seek
			);
			ASSERT(pIEDelete);
			CONVERTER->insertDocUpdate(pIEDelete);
		}

		$$ = CONVERTER->stack().push(pOperation);
	}
	| opr_header RDO_IDENTIF_COLON error
	{
		CONVERTER->error().error(@2, @3, _T("Ожидается имя образца"));
	}
	| opr_param RDO_IDENTIF_COLON error
	{
		CONVERTER->error().error(@2, @3, _T("Ожидается имя образца"));
	}
	| opr_header error
	{
		CONVERTER->error().error( @2, _T("Ожидается имя операции"));
	}
	;

opr_keyb
	: opr_body
	| opr_keyb RDO_STRING_CONST
	{
		LPRDOOPROperation pOperation = CONVERTER->stack().pop<RDOOPROperation>($1);
		ASSERT(pOperation);
		tstring key = CONVERTER->stack().pop<RDOValue>($2)->value().getString();
		pOperation->addHotKey(key, @2);
		$$ = CONVERTER->stack().push(pOperation);
	}
	| opr_keyb '+' RDO_STRING_CONST
	{
		LPRDOOPROperation pOperation = CONVERTER->stack().pop<RDOOPROperation>($1);
		ASSERT(pOperation);
		tstring key = CONVERTER->stack().pop<RDOValue>($3)->value().getString();
		pOperation->addHotKey(key, @3);
		$$ = CONVERTER->stack().push(pOperation);
	}
	;

opr_param
	: opr_param '*'
	{
		LPRDOOPROperation pOperation = CONVERTER->stack().pop<RDOOPROperation>($1);
		ASSERT(pOperation);

		if (pOperation->pattern()->getType() == RDOPATPattern::PT_IE)
		{
			rdoConverter::LPDocUpdate pIEParamDelete = rdo::Factory<rdoConverter::UpdateDelete>::create(
				@2.m_first_seek,
				@2.m_last_seek
			);
			ASSERT(pIEParamDelete);
			CONVERTER->insertDocUpdate(pIEParamDelete);
		}

		pOperation->addParam(rdo::Factory<RDOValue>::create(RDOParserSrcInfo(@2, _T("*"))));
		$$ = CONVERTER->stack().push(pOperation);
	}
	| opr_param RDO_INT_CONST
	{
		LPRDOOPROperation pOperation = CONVERTER->stack().pop<RDOOPROperation>($1);
		ASSERT(pOperation);

		if (pOperation->pattern()->getType() == RDOPATPattern::PT_IE)
		{
			rdoConverter::LPDocUpdate pIEParamDelete = rdo::Factory<rdoConverter::UpdateDelete>::create(
				@2.m_first_seek,
				@2.m_last_seek
			);
			ASSERT(pIEParamDelete);
			CONVERTER->insertDocUpdate(pIEParamDelete);
		}

		pOperation->addParam(CONVERTER->stack().pop<RDOValue>($2));
		$$ = CONVERTER->stack().push(pOperation);
	}
	| opr_param RDO_REAL_CONST
	{
		LPRDOOPROperation pOperation = CONVERTER->stack().pop<RDOOPROperation>($1);
		ASSERT(pOperation);

		if (pOperation->pattern()->getType() == RDOPATPattern::PT_IE)
		{
			rdoConverter::LPDocUpdate pIEParamDelete = rdo::Factory<rdoConverter::UpdateDelete>::create(
				@2.m_first_seek,
				@2.m_last_seek
			);
			ASSERT(pIEParamDelete);
			CONVERTER->insertDocUpdate(pIEParamDelete);
		}

		pOperation->addParam(CONVERTER->stack().pop<RDOValue>($2));
		$$ = CONVERTER->stack().push(pOperation);
	}
	| opr_param RDO_BOOL_CONST
	{
		LPRDOOPROperation pOperation = CONVERTER->stack().pop<RDOOPROperation>($1);
		ASSERT(pOperation);

		if (pOperation->pattern()->getType() == RDOPATPattern::PT_IE)
		{
			rdoConverter::LPDocUpdate pIEParamDelete = rdo::Factory<rdoConverter::UpdateDelete>::create(
				@2.m_first_seek,
				@2.m_last_seek
			);
			ASSERT(pIEParamDelete);
			CONVERTER->insertDocUpdate(pIEParamDelete);
		}

		pOperation->addParam(CONVERTER->stack().pop<RDOValue>($2));
		$$ = CONVERTER->stack().push(pOperation);
	}
	| opr_param RDO_STRING_CONST
	{
		LPRDOOPROperation pOperation = CONVERTER->stack().pop<RDOOPROperation>($1);
		ASSERT(pOperation);

		if (pOperation->pattern()->getType() == RDOPATPattern::PT_IE)
		{
			rdoConverter::LPDocUpdate pIEParamDelete = rdo::Factory<rdoConverter::UpdateDelete>::create(
				@2.m_first_seek,
				@2.m_last_seek
			);
			ASSERT(pIEParamDelete);
			CONVERTER->insertDocUpdate(pIEParamDelete);
		}

		pOperation->addParam(CONVERTER->stack().pop<RDOValue>($2));
		$$ = CONVERTER->stack().push(pOperation);
	}
	| opr_param RDO_IDENTIF
	{
		LPRDOOPROperation pOperation = CONVERTER->stack().pop<RDOOPROperation>($1);
		ASSERT(pOperation);

		if (pOperation->pattern()->getType() == RDOPATPattern::PT_IE)
		{
			rdoConverter::LPDocUpdate pIEParamDelete = rdo::Factory<rdoConverter::UpdateDelete>::create(
				@2.m_first_seek,
				@2.m_last_seek
			);
			ASSERT(pIEParamDelete);
			CONVERTER->insertDocUpdate(pIEParamDelete);
		}

		pOperation->addParam(CONVERTER->stack().pop<RDOValue>($2));
		$$ = CONVERTER->stack().push(pOperation);
	}
	| opr_keyb
	;

opr_end
	: opr_param RDO_End
	{
		LPRDOOPROperation pOperation = CONVERTER->stack().pop<RDOOPROperation>($1);
		ASSERT(pOperation);
		pOperation->endParam(@1);
		$$ = CONVERTER->stack().push(pOperation);
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Общие составные токены для всех объектов РДО
// --------------------------------------------------------------------------------
// -------------------- Логические выражения
// --------------------------------------------------------------------------------
fun_logic_eq
	: '='
	{
		LPDocUpdate pInsert = rdo::Factory<UpdateInsert>::create(@1.m_last_seek, _T("="));
		ASSERT(pInsert);
		CONVERTER->insertDocUpdate(pInsert);

		$$ = RDO_eq;
	}
	| RDO_eq
	{
		$$ = RDO_eq;
	}
	;

fun_logic
	: fun_arithm fun_logic_eq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = CONVERTER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator ==(pArithm2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_arithm RDO_neq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = CONVERTER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator !=(pArithm2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_arithm '<' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = CONVERTER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator <(pArithm2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_arithm '>' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = CONVERTER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator >(pArithm2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_arithm RDO_leq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = CONVERTER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator <=(pArithm2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_arithm RDO_geq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = CONVERTER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator >=(pArithm2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_logic RDO_and fun_logic
	{
		LPRDOFUNLogic pLogic1 = CONVERTER->stack().pop<RDOFUNLogic>($1);
		LPRDOFUNLogic pLogic2 = CONVERTER->stack().pop<RDOFUNLogic>($3);
		ASSERT(pLogic1);
		ASSERT(pLogic2);
		LPRDOFUNLogic pResult = pLogic1->operator &&(pLogic2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_logic RDO_or fun_logic
	{
		LPRDOFUNLogic pLogic1 = CONVERTER->stack().pop<RDOFUNLogic>($1);
		LPRDOFUNLogic pLogic2 = CONVERTER->stack().pop<RDOFUNLogic>($3);
		ASSERT(pLogic1);
		ASSERT(pLogic2);
		LPRDOFUNLogic pResult = pLogic1->operator ||(pLogic2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_arithm
	{
		LPRDOFUNArithm pArithm = CONVERTER->stack().pop<RDOFUNArithm>($1);
		ASSERT(pArithm);
		LPRDOFUNLogic pResult = rdo::Factory<RDOFUNLogic>::create(pArithm);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_group
	| fun_select_logic
	| '[' fun_logic ']'
	{
		LPRDOFUNLogic pLogic = CONVERTER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		pLogic->setSrcPos (@1, @3);
		pLogic->setSrcText(_T("[") + pLogic->src_text() + _T("]"));
		$$ = CONVERTER->stack().push(pLogic);
	}
	| '(' fun_logic ')'
	{
		LPRDOFUNLogic pLogic = CONVERTER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		pLogic->setSrcPos (@1, @3);
		pLogic->setSrcText(_T("(") + pLogic->src_text() + _T(")"));
		$$ = CONVERTER->stack().push(pLogic);
	}
	| RDO_not fun_logic
	{
		LPRDOFUNLogic pLogic = CONVERTER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		LPRDOFUNLogic pLogicNot = pLogic->operator_not();
		ASSERT(pLogicNot);
		pLogicNot->setSrcPos (@1, @2);
		pLogicNot->setSrcText(_T("not ") + pLogic->src_text());
		$$ = CONVERTER->stack().push(pLogicNot);
	}
	| '[' fun_logic error
	{
		CONVERTER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
	}
	| '(' fun_logic error
	{
		CONVERTER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Арифметические выражения
// --------------------------------------------------------------------------------
fun_arithm
	: RDO_INT_CONST                      { $$ = CONVERTER->stack().push(rdo::Factory<RDOFUNArithm>::create(CONVERTER->stack().pop<RDOValue>($1))); }
	| RDO_REAL_CONST                     { $$ = CONVERTER->stack().push(rdo::Factory<RDOFUNArithm>::create(CONVERTER->stack().pop<RDOValue>($1))); }
	| RDO_BOOL_CONST                     { $$ = CONVERTER->stack().push(rdo::Factory<RDOFUNArithm>::create(CONVERTER->stack().pop<RDOValue>($1))); }
	| RDO_STRING_CONST                   { $$ = CONVERTER->stack().push(rdo::Factory<RDOFUNArithm>::create(CONVERTER->stack().pop<RDOValue>($1))); }
	| RDO_IDENTIF                        { $$ = CONVERTER->stack().push(rdo::Factory<RDOFUNArithm>::create(CONVERTER->stack().pop<RDOValue>($1))); }
	| RDO_IDENTIF '.' RDO_IDENTIF        { $$ = CONVERTER->stack().push(rdo::Factory<RDOFUNArithm>::create(CONVERTER->stack().pop<RDOValue>($1), CONVERTER->stack().pop<RDOValue>($3))); }
	| RDO_IDENTIF_RELRES '.' RDO_IDENTIF { $$ = CONVERTER->stack().push(rdo::Factory<RDOFUNArithm>::create(CONVERTER->stack().pop<RDOValue>($1), CONVERTER->stack().pop<RDOValue>($3))); }
	| fun_arithm '+' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = CONVERTER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator +(pArithm2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_arithm '-' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = CONVERTER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator -(pArithm2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_arithm '*' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = CONVERTER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator *(pArithm2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_arithm '/' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = CONVERTER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator /(pArithm2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_arithm_func_call
	| fun_select_arithm
	| '(' fun_arithm ')'
	{
		LPRDOFUNArithm pArithm = CONVERTER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		pArithm->setSrcPos (@1, @3);
		pArithm->setSrcText(_T("(") + pArithm->src_text() + _T(")"));
		$$ = CONVERTER->stack().push(pArithm);
	}
	| '-' fun_arithm %prec RDO_UMINUS
	{
		LPRDOFUNArithm pArithm = CONVERTER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		RDOParserSrcInfo info;
		info.setSrcPos (@1, @2);
		info.setSrcText(_T("-") + pArithm->src_text());
		$$ = CONVERTER->stack().push(
			rdo::Factory<RDOFUNArithm>::create(
				rdo::Factory<RDOValue>::create(pArithm->type(), info),
				rdo::Factory<rdoRuntime::RDOCalcUMinus>::create(info.src_pos(), pArithm->createCalc()).object_parent_cast<rdoRuntime::RDOCalc>()
			)
		);
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Функции и последовательности
// --------------------------------------------------------------------------------
fun_arithm_func_call
	: RDO_IDENTIF '(' ')'
	{
		LPRDOFUNParams pFunParams = rdo::Factory<RDOFUNParams>::create();
		ASSERT(pFunParams);
		tstring funName = CONVERTER->stack().pop<RDOValue>($1)->value().getIdentificator();
		pFunParams->getFunseqName().setSrcInfo(RDOParserSrcInfo(@1, funName));
		pFunParams->setSrcPos (@1, @3);
		pFunParams->setSrcText(funName + _T("()"));
		LPRDOFUNArithm pArithm = pFunParams->createCall(funName);
		ASSERT(pArithm);
		$$ = CONVERTER->stack().push(pArithm);
	}
	| RDO_IDENTIF '(' fun_arithm_func_call_pars ')'
	{
		LPRDOFUNParams pFunParams = CONVERTER->stack().pop<RDOFUNParams>($3);
		ASSERT(pFunParams);
		tstring funName = CONVERTER->stack().pop<RDOValue>($1)->value().getIdentificator();
		pFunParams->getFunseqName().setSrcInfo(RDOParserSrcInfo(@1, funName));
		pFunParams->setSrcPos (@1, @4);
		pFunParams->setSrcText(funName + _T("(") + pFunParams->src_text() + _T(")"));
		LPRDOFUNArithm pArithm = pFunParams->createCall(funName);
		ASSERT(pArithm);
		$$ = CONVERTER->stack().push(pArithm);
	}
	| RDO_IDENTIF '(' error
	{
		CONVERTER->error().error(@3, _T("Ошибка в параметрах функции"));
	}
	;

fun_arithm_func_call_pars
	: fun_arithm
	{
		LPRDOFUNParams pFunParams = rdo::Factory<RDOFUNParams>::create();
		LPRDOFUNArithm pArithm    = CONVERTER->stack().pop<RDOFUNArithm>($1);
		ASSERT(pFunParams);
		ASSERT(pArithm   );
		pFunParams->setSrcText  (pArithm->src_text());
		pFunParams->addParameter(pArithm);
		$$ = CONVERTER->stack().push(pFunParams);
	}
	| fun_arithm_func_call_pars ',' fun_arithm
	{
		LPRDOFUNParams pFunParams = CONVERTER->stack().pop<RDOFUNParams>($1);
		LPRDOFUNArithm pArithm    = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pFunParams);
		ASSERT(pArithm   );
		pFunParams->setSrcText  (pFunParams->src_text() + _T(", ") + pArithm->src_text());
		pFunParams->addParameter(pArithm);
		$$ = CONVERTER->stack().push(pFunParams);
	}
	| fun_arithm_func_call_pars error
	{
		CONVERTER->error().error(@2, _T("Ошибка в арифметическом выражении"));
	}
	| fun_arithm_func_call_pars ',' error
	{
		CONVERTER->error().error(@3, _T("Ошибка в арифметическом выражении"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Групповые выражения
// --------------------------------------------------------------------------------
fun_group_keyword
	: RDO_Exist       { $$ = RDOFUNGroupLogic::fgt_exist;     }
	| RDO_Not_Exist   { $$ = RDOFUNGroupLogic::fgt_notexist;  }
	| RDO_For_All     { $$ = RDOFUNGroupLogic::fgt_forall;    }
	| RDO_Not_For_All { $$ = RDOFUNGroupLogic::fgt_notforall; }
	;

fun_group_header
	: fun_group_keyword '(' RDO_IDENTIF_COLON
	{
		LPRDOValue pValue = CONVERTER->stack().pop<RDOValue>($3);
		ASSERT(pValue);
		$$ = CONVERTER->stack().push(rdo::Factory<RDOFUNGroupLogic>::create((RDOFUNGroupLogic::FunGroupType)$1, pValue->src_info()));
	}
	| fun_group_keyword '(' error
	{
		CONVERTER->error().error(@3, _T("Ожидается имя типа"));
	}
	| fun_group_keyword error
	{
		CONVERTER->error().error(@1, _T("После имени функции ожидается октрывающаяся скобка"));
	}
	;

fun_group
	: fun_group_header fun_logic ')'
	{
		LPRDOFUNGroupLogic pGroupFun = CONVERTER->stack().pop<RDOFUNGroupLogic>($1);
		LPRDOFUNLogic      pLogic    = CONVERTER->stack().pop<RDOFUNLogic>     ($2);
		ASSERT(pGroupFun);
		ASSERT(pLogic   );
		pGroupFun->setSrcPos(@1, @3);
		$$ = CONVERTER->stack().push(pGroupFun->createFunLogic(pLogic));
	}
	| fun_group_header RDO_NoCheck ')'
	{
		LPRDOFUNGroupLogic pGroupFun = CONVERTER->stack().pop<RDOFUNGroupLogic>($1);
		ASSERT(pGroupFun);
		pGroupFun->setSrcPos(@1, @3);
		LPRDOFUNLogic pTrueLogic = rdo::Factory<RDOFUNLogic>::create(rdo::Factory<rdoRuntime::RDOCalcConst>::create(1), false);
		ASSERT(pTrueLogic);
		pTrueLogic->setSrcPos (@2);
		pTrueLogic->setSrcText(_T("NoCheck"));
		$$ = CONVERTER->stack().push(pGroupFun->createFunLogic(pTrueLogic));
	}
	| fun_group_header fun_logic error
	{
		CONVERTER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
	}
	| fun_group_header RDO_NoCheck error
	{
		CONVERTER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
	}
	| fun_group_header error
	{
		CONVERTER->error().error(@1, @2, _T("Ошибка в логическом выражении"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Select
// --------------------------------------------------------------------------------
fun_select_header
	: RDO_Select '(' RDO_IDENTIF_COLON
	{
		LPRDOValue pValue = CONVERTER->stack().pop<RDOValue>($3);
		ASSERT(pValue);
		LPRDOFUNSelect pSelect = rdo::Factory<RDOFUNSelect>::create(pValue->src_info());
		ASSERT(pSelect);
		pSelect->setSrcText(_T("Select(") + pValue->value().getIdentificator() + _T(": "));
		$$ = CONVERTER->stack().push(pSelect);
	}
	| RDO_Select '(' error
	{
		CONVERTER->error().error(@3, _T("Ожидается имя типа"));
	}
	| RDO_Select error
	{
		CONVERTER->error().error(@1, _T("Ожидается октрывающаяся скобка"));
	}
	;

fun_select_body
	: fun_select_header fun_logic ')'
	{
		LPRDOFUNSelect pSelect = CONVERTER->stack().pop<RDOFUNSelect>($1);
		LPRDOFUNLogic  pLogic  = CONVERTER->stack().pop<RDOFUNLogic> ($2);
		ASSERT(pSelect);
		ASSERT(pLogic );
		pSelect->setSrcText(pSelect->src_text() + pLogic->src_text() + _T(")"));
		pSelect->initSelect(pLogic);
		$$ = CONVERTER->stack().push(pSelect);
	}
	| fun_select_header RDO_NoCheck ')'
	{
		LPRDOFUNSelect pSelect = CONVERTER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		RDOParserSrcInfo logicInfo(@2, _T("NoCheck"));
		pSelect->setSrcText(pSelect->src_text() + logicInfo.src_text() + _T(")"));
		rdoRuntime::LPRDOCalcConst pCalc  = rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);
		ASSERT(pCalc);
		LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pCalc, true);
		ASSERT(pLogic);
		pLogic->setSrcInfo(logicInfo);
		pSelect->initSelect(pLogic);
		$$ = CONVERTER->stack().push(pSelect);
	}
	| fun_select_header fun_logic error
	{
		CONVERTER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
	}
	| fun_select_header RDO_NoCheck error
	{
		CONVERTER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
	}
	| fun_select_header error
	{
		CONVERTER->error().error(@1, @2, _T("Ошибка в логическом выражении"));
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
		LPRDOFUNSelect pSelect = CONVERTER->stack().pop<RDOFUNSelect>($1);
		LPRDOFUNLogic  pLogic  = CONVERTER->stack().pop<RDOFUNLogic> ($5);
		ASSERT(pSelect);
		ASSERT(pLogic );
		pSelect->setSrcPos(@1, @6);
		LPRDOFUNLogic pLogicSelect = pSelect->createFunSelectGroup((RDOFUNGroupLogic::FunGroupType)$3, pLogic);
		ASSERT(pLogicSelect);
		$$ = CONVERTER->stack().push(pLogicSelect);
	}
	| fun_select_body '.' fun_select_keyword '(' error
	{
		CONVERTER->error().error(@4, @5, _T("Ошибка в логическом выражении"));
	}
	| fun_select_body '.' fun_select_keyword error
	{
		CONVERTER->error().error(@3, _T("Ожидается октрывающаяся скобка"));
	}
	| fun_select_body '.' RDO_Empty '(' ')'
	{
		LPRDOFUNSelect pSelect = CONVERTER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		pSelect->setSrcPos(@1, @5);
		RDOParserSrcInfo emptyInfo(@3, @5, _T("Empty()"));
		LPRDOFUNLogic pLogic = pSelect->createFunSelectEmpty(emptyInfo);
		ASSERT(pLogic);
		$$ = CONVERTER->stack().push(pLogic);
	}
	| fun_select_body '.' RDO_Empty '(' error
	{
		CONVERTER->error().error(@4, _T("Ожидается закрывающаяся скобка"));
	}
	| fun_select_body '.' RDO_Empty error
	{
		CONVERTER->error().error(@3, _T("Ожидается октрывающаяся скобка"));
	}
	| fun_select_body '.' error
	{
		CONVERTER->error().error(@2, @3, _T("Ожидается метод списка ресурсов"));
	}
	| fun_select_body error
	{
		CONVERTER->error().error(@1, _T("Ожидается '.' (точка) для вызова метода списка ресурсов"));
	}
	;

fun_select_arithm
	: fun_select_body '.' RDO_Size '(' ')'
	{
		LPRDOFUNSelect pSelect = CONVERTER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		pSelect->setSrcPos(@1, @5);
		RDOParserSrcInfo sizeInfo(@3, @5, _T("Size()"));
		LPRDOFUNArithm pArithm = pSelect->createFunSelectSize(sizeInfo);
		ASSERT(pArithm);
		$$ = CONVERTER->stack().push(pArithm);
	}
	| fun_select_body '.' RDO_Size error
	{
		CONVERTER->error().error(@3, _T("Ожидается октрывающаяся скобка"));
	}
	| fun_select_body '.' RDO_Size '(' error
	{
		CONVERTER->error().error(@4, _T("Ожидается закрывающаяся скобка"));
	}
	;

%%

CLOSE_RDO_CONVERTER_NAMESPACE

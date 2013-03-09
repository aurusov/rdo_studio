/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofun.y
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
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
#include "converter/smr2rdox/rdortp.h"
#include "converter/smr2rdox/rdofun.h"
#include "converter/smr2rdox/rdo_type_range.h"
#include "converter/smr2rdox/rdo_type_param_suchas.h"
#include "converter/smr2rdox/update/update.h"
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

#define CONVERTER LEXER->converter()
#define RUNTIME   CONVERTER->runtime()

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE
%}

%left RDO_or
%left RDO_and	
%left '+' '-'
%left '*' '/'
%left RDO_not
%left RDO_UMINUS

%start fun_list

%%

fun_list
	: fun_consts fun_func_seq
	| error
	{
		if (CONVERTER->getFUNConstants().empty())
		{
			CONVERTER->error().error(@1, _T("Ожидается описание функции, последовательности или константы"));
		}
		else
		{
			CONVERTER->error().error(@1, _T("Ожидается описание функции или последовательности"));
		}
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Константы
// --------------------------------------------------------------------------------
fun_consts
	: /* empty */
	| RDO_Constant fun_const_body RDO_End
	| RDO_Constant fun_const_body
	{
		CONVERTER->error().error(@2, _T("После описания констант ожидается ключевое слово $End"));
	}
	;

fun_const_body
	: /* empty */
	| fun_const_body fun_const_param_desc
	| fun_const_body error
	{
		CONVERTER->error().error(@2, _T("Ожидается описание константы"));
	}
	;

fun_const_param_desc
	: RDO_IDENTIF_COLON param_type param_value_default
	{
		LPRDOValue pName = CONVERTER->stack().pop<RDOValue>($1);
		CONVERTER->checkFunctionName(pName->src_info());
		LPRDOTypeParam pParamType = CONVERTER->stack().pop<RDOTypeParam>($2);
		ASSERT(pParamType);
		LPRDOFUNConstant pConstant = rdo::Factory<RDOFUNConstant>::create(pName->src_info(), pParamType, CONVERTER->stack().pop<RDOValue>($3));
		ASSERT(pConstant);
		RUNTIME->setConstValue(pConstant->getNumber(), pConstant->getDefault()->value());
		$$ = CONVERTER->stack().push(pConstant);
	}
	| RDO_IDENTIF_COLON
	{
		CONVERTER->error().error(@1, _T("Ожидается тип константы"));
	}
	| RDO_IDENTIF_COLON error
	{
		CONVERTER->error().error(@2, _T("Ошибка описания типа константы"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Последовательности и функции
// --------------------------------------------------------------------------------
fun_func_seq
	: /* empty */
	| fun_func_seq fun_func_descr
	| fun_func_seq fun_seq_descr
	| fun_func_seq RDO_Constant
	{
		CONVERTER->error().error(@2, _T("Константы долны быть описаны первыми, перед функциями и последовательностями"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Функции
// --------------------------------------------------------------------------------
fun_func_descr
	: fun_func_header fun_func_footer
	| fun_func_header error
	{
		LPRDOFUNFunction pFunction = CONVERTER->stack().pop<RDOFUNFunction>($1);
		ASSERT(pFunction);
		CONVERTER->error().error(@2, rdo::format(_T("Ожидается ключевое слово $Type с указанием типа функции '%s'"), pFunction->name().c_str()));
	}
	;

fun_func_header
	: RDO_Function RDO_IDENTIF_COLON param_type param_value_default
	{
		LPRDOValue pName = CONVERTER->stack().pop<RDOValue>($2);
		CONVERTER->checkFunctionName(pName->src_info());
		LPRDOTypeParam pRetType = CONVERTER->stack().pop<RDOTypeParam>($3);
		ASSERT(pRetType);
		LPRDOParam pReturn = rdo::Factory<RDOParam>::create(pName->src_info(), pRetType, CONVERTER->stack().pop<RDOValue>($4));
		ASSERT(pReturn);
		LPRDOFUNFunction pFunction = rdo::Factory<RDOFUNFunction>::create(pName->src_info(), pReturn);
		ASSERT(pFunction);
		$$ = CONVERTER->stack().push(pFunction);
	}
	| RDO_Function RDO_IDENTIF_COLON error
	{
		tstring name = CONVERTER->stack().pop<RDOValue>($2)->value().getIdentificator();
		CONVERTER->error().error(@3, rdo::format(_T("Ожидается тип возвращаемого значения функции '%s'"), name.c_str()));
	}
	| RDO_Function error
	{
		CONVERTER->error().error(@2, _T("После ключевого слова $Function ожидается имя фунции"));
	}
	;

fun_func_parameters
	: /* empty */
	| RDO_Parameters fun_func_params
	;

fun_func_params
	: /* empty */
	| fun_func_params RDO_IDENTIF_COLON param_type param_value_default
	{
		LPRDOValue     pName = CONVERTER->stack().pop<RDOValue>($2);
		LPRDOTypeParam pType = CONVERTER->stack().pop<RDOTypeParam>($3);
		ASSERT(pType);
		LPRDOParam pParam = rdo::Factory<RDOParam>::create(pName->src_info(), pType, CONVERTER->stack().pop<RDOValue>($4));
		ASSERT(pParam);
		CONVERTER->getLastFUNFunction()->add(pParam);
	}
	| fun_func_params RDO_IDENTIF_COLON error
	{
		CONVERTER->error().error(@3, _T("Ожидается тип параметра функции"));
	}
	| fun_func_params error
	{
		CONVERTER->error().error(@2, _T("Ожидается описание параметра функции в формате <имя>: <тип>"));
	}
	;

fun_func_footer
	:	RDO_Type '=' RDO_algorithmic fun_func_parameters RDO_Body fun_func_algorithmic_body RDO_End
	{
		LPRDOFUNFunction pFunction = CONVERTER->getLastFUNFunction();
		ASSERT(pFunction);
		pFunction->createAlgorithmicCalc(@5);
	}
	| RDO_Type '=' RDO_list fun_func_parameters RDO_Body fun_func_list_body RDO_End
	{
		LPRDOFUNFunction pFunction = CONVERTER->getLastFUNFunction();
		ASSERT(pFunction);
		pFunction->createListCalc();
	}
	| RDO_Type '=' RDO_table fun_func_parameters RDO_Body fun_func_list_body RDO_End
	{
		LPRDOFUNFunction pFunction = CONVERTER->getLastFUNFunction();
		ASSERT(pFunction);
		pFunction->createTableCalc(@6);
	}
	| RDO_Type '=' RDO_algorithmic fun_func_parameters RDO_Body fun_func_algorithmic_body error
	{
		CONVERTER->error().error(@7, _T("Ожидается ключевое слово $End"));
	}
	| RDO_Type '=' RDO_list fun_func_parameters RDO_Body fun_func_list_body error
	{
		CONVERTER->error().error(@7, _T("Ожидается ключевое слово $End"));
	}
	| RDO_Type '=' RDO_table fun_func_parameters RDO_Body fun_func_list_body error
	{
		CONVERTER->error().error(@7, _T("Ожидается ключевое слово $End"));
	}
	| RDO_Type '=' RDO_algorithmic error
	{
		CONVERTER->error().error(@4, _T("Ожидается ключевое слово $Parameters"));
	}
	| RDO_Type '=' RDO_list error
	{
		CONVERTER->error().error(@4, _T("Ожидается ключевое слово $Parameters"));
	}
	| RDO_Type '=' RDO_table error
	{
		CONVERTER->error().error(@4, _T("Ожидается ключевое слово $Parameters"));
	}
	| RDO_Type '=' error
	{
		CONVERTER->error().error(@3, _T("Неизвестный тип функции"));
	}
	| RDO_Type error
	{
		CONVERTER->error().error(@2, _T("После ключевого слова $Type ожидается тип функции"));
	}
	;

fun_func_algorithmic_body
	: /* empty */
	| fun_func_algorithmic_body fun_func_algorithmic_calc_if
	{
		LPRDOFUNCalculateIf pCalculateIf = CONVERTER->stack().pop<RDOFUNCalculateIf>($2);
		ASSERT(pCalculateIf);
		CONVERTER->getLastFUNFunction()->add(pCalculateIf);
	}
	;

fun_func_calc_if
	: RDO_Calculate_if
	| RDO_if
	;

fun_func_calc_name
	: RDO_IDENTIF
	{
		tstring name = CONVERTER->stack().pop<RDOValue>($1)->value().getIdentificator();
		if (name != CONVERTER->getLastFUNFunction()->name() && name != _T("result"))
		{
			CONVERTER->error().error(@1, rdo::format(_T("Ожидается имя функции '%s'"), CONVERTER->getLastFUNFunction()->name().c_str()));
		}
	}
	;

fun_func_algorithmic_calc_if
	: fun_func_calc_if fun_logic fun_func_calc_name '=' fun_arithm
	{
		LPDocUpdate pIFReplace = rdo::Factory<UpdateReplace>::create(
			@1.m_first_seek,
			@1.m_last_seek,
			_T("if")
		);
		ASSERT(pIFReplace);
		CONVERTER->insertDocUpdate(pIFReplace);

		LPDocUpdate pIFInsertOpenBrace = rdo::Factory<UpdateInsert>::create(
			@2.m_first_seek,
			_T("(")
		);
		ASSERT(pIFInsertOpenBrace);
		CONVERTER->insertDocUpdate(pIFInsertOpenBrace);

		LPDocUpdate pIFInsertCloseBrace = rdo::Factory<UpdateInsert>::create(
			@2.m_last_seek,
			_T(")")
		);
		ASSERT(pIFInsertCloseBrace);
		CONVERTER->insertDocUpdate(pIFInsertCloseBrace);

		LPDocUpdate pReturnReplace = rdo::Factory<UpdateReplace>::create(
			@3.m_first_seek,
			@4.m_last_seek,
			_T("return")
		);
		ASSERT(pReturnReplace);
		CONVERTER->insertDocUpdate(pReturnReplace);

		LPDocUpdate pSemicolonInsert = rdo::Factory<UpdateInsert>::create(
			@5.m_last_seek,
			_T(";")
		);
		ASSERT(pSemicolonInsert);
		CONVERTER->insertDocUpdate(pSemicolonInsert);

		LPRDOFUNLogic pLogic = CONVERTER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		pLogic->setSrcText(_T("Calculate_if ") + pLogic->src_text());
		LPRDOFUNArithm pArithm = CONVERTER->stack().pop<RDOFUNArithm>($5);
		ASSERT(pArithm);
		LPRDOFUNCalculateIf pCalculateIf = rdo::Factory<RDOFUNCalculateIf>::create(pLogic, pArithm);
		ASSERT(pCalculateIf);
		$$ = CONVERTER->stack().push(pCalculateIf);
	}
	| fun_func_calc_if fun_logic fun_func_calc_name '=' error
	{
		CONVERTER->error().error(@5, _T("Ошибка в арифметическом выражении"));
	}
	| fun_func_calc_name '=' fun_arithm
	{
		rdo::runtime::LPRDOCalc pCondition = rdo::Factory<rdo::runtime::RDOCalcConst>::create(1);
		RDOParserSrcInfo logic_src_info(_T("Calculate_if 1 = 1"));
		logic_src_info.setSrcPos( @1.m_first_line, @1.m_first_pos, @1.m_first_line, @1.m_first_pos );
		pCondition->setSrcInfo( logic_src_info );
		LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pCondition, true);
		pLogic->setSrcInfo( logic_src_info );
		LPRDOFUNArithm pArithm = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm);
		LPRDOFUNCalculateIf pCalculateIf = rdo::Factory<RDOFUNCalculateIf>::create(pLogic, pArithm);
		ASSERT(pCalculateIf);
		$$ = CONVERTER->stack().push(pCalculateIf);
	}
	| fun_func_calc_name '=' error
	{
		CONVERTER->error().error(@3, _T("Ошибка в арифметическом выражении"));
	}
	| fun_func_calc_if fun_logic fun_func_calc_name error
	{
		CONVERTER->error().error(@4, _T("Ожидается '='"));
	}
	| fun_func_calc_name error
	{
		CONVERTER->error().error(@2, _T("Ожидается '='"));
	}
	| fun_func_calc_if fun_logic error
	{
		CONVERTER->error().error(@2, @3, _T("После логического выражения ожидается <имя_функции> = <результат_функции>"));
	}
	| fun_func_calc_if error
	{
		CONVERTER->error().error(@2, _T("Ошибка в логическом выражении"));
	}
	| error
	{
		CONVERTER->error().error(@1, _T("Ожидается ключевое слово Calculate_if"));
	}
	;

fun_func_list_body
	: /* empty */
	| fun_func_list_body fun_func_list_value
	;

fun_func_list_value
	: RDO_IDENTIF
	{
		tstring str = CONVERTER->stack().pop<RDOValue>($1)->value().getIdentificator();
		LPRDOFUNFunctionListElementIdentif pValue = rdo::Factory<RDOFUNFunctionListElementIdentif>::create(RDOParserSrcInfo(@1, str));
		ASSERT(pValue);
		CONVERTER->getLastFUNFunction()->add(pValue.object_parent_cast<RDOFUNFunctionListElement>());
		$$ = CONVERTER->stack().push(pValue);
	}
	| RDO_REAL_CONST
	{
		LPRDOFUNFunctionListElementReal pValue = rdo::Factory<RDOFUNFunctionListElementReal>::create(@1, CONVERTER->stack().pop<RDOValue>($1)->value().getDouble());
		ASSERT(pValue);
		CONVERTER->getLastFUNFunction()->add(pValue.object_parent_cast<RDOFUNFunctionListElement>());
		$$ = CONVERTER->stack().push(pValue);
	}
	| RDO_INT_CONST
	{
		LPRDOFUNFunctionListElementInt pValue = rdo::Factory<RDOFUNFunctionListElementInt>::create(@1, CONVERTER->stack().pop<RDOValue>($1)->value().getInt());
		ASSERT(pValue);
		CONVERTER->getLastFUNFunction()->add(pValue.object_parent_cast<RDOFUNFunctionListElement>());
		$$ = CONVERTER->stack().push(pValue);
	}
	| '='
	{
		LPRDOFUNFunctionListElementEq pValue = rdo::Factory<RDOFUNFunctionListElementEq>::create(@1);
		ASSERT(pValue);
		CONVERTER->getLastFUNFunction()->add(pValue.object_parent_cast<RDOFUNFunctionListElement>());
		$$ = CONVERTER->stack().push(pValue);
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Последовательности
// --------------------------------------------------------------------------------
fun_seq_descr
	: fun_seq_uniform
	| fun_seq_exponential
	| fun_seq_normal
	| fun_seq_by_hist
	| fun_seq_enumerative
	;

fun_seq_header
	: RDO_Sequence RDO_IDENTIF_COLON param_type RDO_Type '='
	{
		LPRDOValue pName = CONVERTER->stack().pop<RDOValue>($2);
		CONVERTER->checkFunctionName(pName->src_info());
		LPRDOTypeParam pType = CONVERTER->stack().pop<RDOTypeParam>($3);
		ASSERT(pType);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = rdo::Factory<RDOFUNSequence::RDOFUNSequenceHeader>::create(pType, pName->src_info());
		ASSERT(pHeader);
		$$ = CONVERTER->stack().push(pHeader);
	}
	| RDO_Sequence RDO_IDENTIF_COLON param_type RDO_Type '=' error
	{
		CONVERTER->error().error(@6, _T("После знака равенства ожидается тип последовательности"));
	}
	| RDO_Sequence RDO_IDENTIF_COLON param_type RDO_Type error
	{
		CONVERTER->error().error(@5, _T("После ключевого слова $Type ожидается знак равенства и тип последовательности"));
	}
	| RDO_Sequence RDO_IDENTIF_COLON param_type error
	{
		CONVERTER->error().error(@4, _T("Ожидается ключевое слово $Type"));
	}
	| RDO_Sequence RDO_IDENTIF_COLON error
	{
		CONVERTER->error().error(@2, @3, _T("После имени последовательности ожидается тип возвращаемого значения"));
	}
	| RDO_Sequence error
	{
		CONVERTER->error().error(@1, @2, _T("После ключевого слова $Sequence ожидаются имя и тип результата последовательности в формате '<имя> : <тип>'"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Датчики случайных чисел
// --------------------------------------------------------------------------------
fun_seq_uniform
	: fun_seq_header RDO_uniform RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = CONVERTER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceUniform>::create(pHeader);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_header RDO_uniform RDO_INT_CONST RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = CONVERTER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceUniform>::create(pHeader, CONVERTER->stack().pop<RDOValue>($3)->value().getInt());
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_header RDO_uniform RDO_INT_CONST error
	{
		CONVERTER->error().error(@4, _T("После базы ожидается ключевое слово $End"));
	}
	| fun_seq_header RDO_uniform error
	{
		CONVERTER->error().error(@3, _T("После типа последовательности ожидается база генератора или ключевое слово $End"));
	}
	;

fun_seq_exponential
	: fun_seq_header RDO_exponential RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = CONVERTER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceExponential>::create(pHeader);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_header RDO_exponential RDO_INT_CONST RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = CONVERTER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceExponential>::create(pHeader, CONVERTER->stack().pop<RDOValue>($3)->value().getInt());
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_header RDO_exponential RDO_INT_CONST error
	{
		CONVERTER->error().error( @4, _T("После базы ожидается ключевое слово $End"));
	}
	| fun_seq_header RDO_exponential error
	{
		CONVERTER->error().error( @3, _T("После типа последовательности ожидается база генератора или ключевое слово $End"));
	}
	;

fun_seq_normal
	: fun_seq_header RDO_normal RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = CONVERTER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceNormal>::create(pHeader);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_header RDO_normal RDO_INT_CONST RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = CONVERTER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceNormal>::create(pHeader, CONVERTER->stack().pop<RDOValue>($3)->value().getInt());
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_header RDO_normal RDO_INT_CONST error
	{
		CONVERTER->error().error( @4, _T("После базы ожидается ключевое слово $End"));
	}
	| fun_seq_header RDO_normal error
	{
		CONVERTER->error().error( @3, _T("После типа последовательности ожидается база генератора или ключевое слово $End"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Гистограмма
// --------------------------------------------------------------------------------
fun_seq_by_hist_header
	: fun_seq_header RDO_by_hist RDO_Body
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = CONVERTER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pByHistHeader = rdo::Factory<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>::create(pHeader);
		ASSERT(pByHistHeader);
		$$ = CONVERTER->stack().push(pByHistHeader);
	}
	| fun_seq_header RDO_by_hist RDO_INT_CONST RDO_Body
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = CONVERTER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pByHistHeader = rdo::Factory<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>::create(pHeader, CONVERTER->stack().pop<RDOValue>($3)->value().getInt());
		ASSERT(pByHistHeader);
		$$ = CONVERTER->stack().push(pByHistHeader);
	}
	| fun_seq_header RDO_by_hist RDO_INT_CONST error
	{
		CONVERTER->error().error( @4, _T("После базы ожидается ключевое слово $Body"));
	}
	| fun_seq_header RDO_by_hist error
	{
		CONVERTER->error().error( @3, _T("После типа последовательности ожидается база генератора или ключевое слово $Body"));
	}
	;

fun_seq_by_hist_body_real
	: fun_seq_by_hist_header RDO_REAL_CONST RDO_REAL_CONST RDO_REAL_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		if ( pHeader->getHeader()->getType()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->getHeader()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($3), CONVERTER->stack().pop<RDOValue>($4));
		ASSERT(pSequence);
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_REAL_CONST RDO_REAL_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		if ( pHeader->getHeader()->getType()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @3, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->getHeader()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($3), CONVERTER->stack().pop<RDOValue>($4));
		ASSERT(pSequence);
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_REAL_CONST RDO_INT_CONST RDO_REAL_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		if ( pHeader->getHeader()->getType()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->getHeader()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($3), CONVERTER->stack().pop<RDOValue>($4));
		ASSERT(pSequence);
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_REAL_CONST RDO_REAL_CONST RDO_INT_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		if ( pHeader->getHeader()->getType()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->getHeader()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($3), CONVERTER->stack().pop<RDOValue>($4));
		ASSERT(pSequence);
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_INT_CONST RDO_REAL_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($3), CONVERTER->stack().pop<RDOValue>($4));
		ASSERT(pSequence);
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_REAL_CONST RDO_INT_CONST RDO_INT_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		if ( pHeader->getHeader()->getType()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->getHeader()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($3), CONVERTER->stack().pop<RDOValue>($4));
		ASSERT(pSequence);
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_REAL_CONST RDO_INT_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		if ( pHeader->getHeader()->getType()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @3, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->getHeader()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($3), CONVERTER->stack().pop<RDOValue>($4));
		ASSERT(pSequence);
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_INT_CONST RDO_INT_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($3), CONVERTER->stack().pop<RDOValue>($4));
		ASSERT(pSequence);
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_REAL_CONST RDO_REAL_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = CONVERTER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getType()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->src_text().c_str()) );
		}
		pSequence->addReal(CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($3), CONVERTER->stack().pop<RDOValue>($4));
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_REAL_CONST RDO_REAL_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = CONVERTER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getType()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @3, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->src_text().c_str()) );
		}
		pSequence->addReal(CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($3), CONVERTER->stack().pop<RDOValue>($4));
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_INT_CONST RDO_REAL_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = CONVERTER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getType()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->src_text().c_str()) );
		}
		pSequence->addReal(CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($3), CONVERTER->stack().pop<RDOValue>($4));
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_REAL_CONST RDO_INT_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = CONVERTER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getType()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->src_text().c_str()) );
		}
		pSequence->addReal(CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($3), CONVERTER->stack().pop<RDOValue>($4));
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_INT_CONST RDO_REAL_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = CONVERTER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		pSequence->addReal(CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($3), CONVERTER->stack().pop<RDOValue>($4));
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_INT_CONST RDO_INT_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = CONVERTER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getType()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->src_text().c_str()) );
		}
		pSequence->addReal(CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($3), CONVERTER->stack().pop<RDOValue>($4));
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_REAL_CONST RDO_INT_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = CONVERTER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getType()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @3, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->src_text().c_str()) );
		}
		pSequence->addReal(CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($3), CONVERTER->stack().pop<RDOValue>($4));
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_INT_CONST RDO_INT_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = CONVERTER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		pSequence->addReal(CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($3), CONVERTER->stack().pop<RDOValue>($4));
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_REAL_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if (pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_int)
		{
			CONVERTER->error().error( @2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		CONVERTER->error().error( @2, @3, _T("Ожидается конец диапазона") );
	}
	| fun_seq_by_hist_header RDO_INT_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		CONVERTER->error().error( @2, @3, _T("Ожидается конец диапазона") );
	}
	| fun_seq_by_hist_header RDO_REAL_CONST RDO_REAL_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		CONVERTER->error().error( @3, @4, _T("Ожидается относительная вероятность") );
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_REAL_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		CONVERTER->error().error( @3, @4, _T("Ожидается относительная вероятность") );
	}
	| fun_seq_by_hist_header RDO_REAL_CONST RDO_INT_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		CONVERTER->error().error( @3, @4, _T("Ожидается относительная вероятность"));
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_INT_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		CONVERTER->error().error( @3, @4, _T("Ожидается относительная вероятность") );
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST error
	{
		CONVERTER->error().error( @2, @3, _T("Ожидается конец диапазона") );
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST error
	{
		CONVERTER->error().error( @2, @3, _T("Ожидается конец диапазона") );
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_REAL_CONST error
	{
		CONVERTER->error().error( @3, @4, _T("Ожидается относительная вероятность") );
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_REAL_CONST error
	{
		CONVERTER->error().error( @3, @4, _T("Ожидается относительная вероятность") );
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_INT_CONST error
	{
		CONVERTER->error().error( @3, @4, _T("Ожидается относительная вероятность") );
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_INT_CONST error
	{
		CONVERTER->error().error( @3, @4, _T("Ожидается относительная вероятность") );
	}
	| fun_seq_by_hist_header RDO_End
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		CONVERTER->error().error( pHeader->src_info(), rdo::format(_T("Последовательность '%s' не должна быть пустой"), pHeader->src_text().c_str()) );
	}
	;

fun_seq_by_hist_body_enum
	: fun_seq_by_hist_header RDO_IDENTIF RDO_REAL_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_enum )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistEnum pSequence = rdo::Factory<RDOFUNSequenceByHistEnum>::create(pHeader, CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($3));
		ASSERT(pSequence);
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_IDENTIF RDO_INT_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_enum )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistEnum pSequence = rdo::Factory<RDOFUNSequenceByHistEnum>::create(pHeader, CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($3));
		ASSERT(pSequence);
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_enum RDO_IDENTIF RDO_REAL_CONST
	{
		LPRDOFUNSequenceByHistEnum pSequence = CONVERTER->stack().pop<RDOFUNSequenceByHistEnum>($1);
		ASSERT(pSequence);
		pSequence->addEnum(CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($3));
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_enum RDO_IDENTIF RDO_INT_CONST
	{
		LPRDOFUNSequenceByHistEnum pSequence = CONVERTER->stack().pop<RDOFUNSequenceByHistEnum>($1);
		ASSERT(pSequence);
		pSequence->addEnum(CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($3));
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_IDENTIF error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdo::runtime::RDOType::t_enum )
		{
			CONVERTER->error().error( @2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		CONVERTER->error().error( @2, @3, rdo::format(_T("Ожидается относительная вероятность для значения: %s"), CONVERTER->stack().pop<RDOValue>($2)->value().getIdentificator().c_str()) );
	}
	| fun_seq_by_hist_body_enum RDO_IDENTIF error
	{
		CONVERTER->error().error( @2, @3, rdo::format(_T("Ожидается относительная вероятность для значения: %s"), CONVERTER->stack().pop<RDOValue>($2)->value().getIdentificator().c_str()) );
	}
	| fun_seq_by_hist_body_enum RDO_REAL_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		CONVERTER->error().error( @2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
	}
	| fun_seq_by_hist_body_enum RDO_INT_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		CONVERTER->error().error( @2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
	}
	;

fun_seq_by_hist
	: fun_seq_by_hist_body_real RDO_End
	{
		LPRDOFUNSequence pSequence = CONVERTER->stack().pop<RDOFUNSequence>($1);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_enum RDO_End
	{
		LPRDOFUNSequence pSequence = CONVERTER->stack().pop<RDOFUNSequence>($1);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real error
	{
		CONVERTER->error().error( @2, _T("Ошибка в описании последовательности") );
	}
	| fun_seq_by_hist_body_enum error
	{
		CONVERTER->error().error( @2, _T("Ошибка в описании последовательности") );
	}
	| fun_seq_by_hist_body_real
	{
		CONVERTER->error().error( @1, _T("Ожидается ключевое слово $End") );
	}
	| fun_seq_by_hist_body_enum
	{
		CONVERTER->error().error( @1, _T("Ожидается ключевое слово $End") );
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Перечень значений
// --------------------------------------------------------------------------------
fun_seq_enumerative
	: fun_seq_enumerative_body RDO_End
	{
		LPRDOFUNSequence pSequence = CONVERTER->stack().pop<RDOFUNSequence>($1);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_enumerative_header RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		CONVERTER->error().error( pHeader->src_info(), rdo::format(_T("Последовательность '%s' не должна быть пустой"), pHeader->src_text().c_str()) );
	}
	;

fun_seq_enumerative_header
	: fun_seq_header RDO_enumerative RDO_Body
	| fun_seq_header RDO_enumerative RDO_INT_CONST error
	{
		CONVERTER->error().error( @3, _T("У последовательности типа enumerative нет базы генератора") );
	}
	| fun_seq_header RDO_enumerative RDO_Parameters error
	{
		CONVERTER->error().error( @3, _T("У последовательности типа enumerative нет параметров") );
	}
	| fun_seq_header RDO_enumerative error
	{
		CONVERTER->error().error( @3, _T("После типа последовательности ожидается ключевое слово $Body") );
	}
	;

fun_seq_enumerative_body
	: fun_seq_enumerative_header RDO_INT_CONST
	{
		RDOFUNSequenceEnumerative::LPRDOFUNSequenceHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequenceEnumerative pSequence = rdo::Factory<RDOFUNSequenceEnumerative>::create(pHeader, CONVERTER->stack().pop<RDOValue>($2));
		ASSERT(pSequence);
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_enumerative_header RDO_REAL_CONST
	{
		RDOFUNSequenceEnumerative::LPRDOFUNSequenceHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequenceEnumerative pSequence = rdo::Factory<RDOFUNSequenceEnumerative>::create(pHeader, CONVERTER->stack().pop<RDOValue>($2));
		ASSERT(pSequence);
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_enumerative_header RDO_BOOL_CONST
	{
		RDOFUNSequenceEnumerative::LPRDOFUNSequenceHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequenceEnumerative pSequence = rdo::Factory<RDOFUNSequenceEnumerative>::create(pHeader, CONVERTER->stack().pop<RDOValue>($2));
		ASSERT(pSequence);
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_enumerative_header RDO_STRING_CONST
	{
		RDOFUNSequenceEnumerative::LPRDOFUNSequenceHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequenceEnumerative pSequence = rdo::Factory<RDOFUNSequenceEnumerative>::create(pHeader, CONVERTER->stack().pop<RDOValue>($2));
		ASSERT(pSequence);
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_enumerative_header RDO_IDENTIF
	{
		RDOFUNSequenceEnumerative::LPRDOFUNSequenceHeader pHeader = CONVERTER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequenceEnumerative pSequence = rdo::Factory<RDOFUNSequenceEnumerative>::create(pHeader, CONVERTER->stack().pop<RDOValue>($2));
		ASSERT(pSequence);
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body RDO_INT_CONST
	{
		LPRDOFUNSequenceEnumerative pSequence = CONVERTER->stack().pop<RDOFUNSequenceEnumerative>($1);
		ASSERT(pSequence);
		pSequence->addValue(CONVERTER->stack().pop<RDOValue>($2));
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body RDO_REAL_CONST
	{
		LPRDOFUNSequenceEnumerative pSequence = CONVERTER->stack().pop<RDOFUNSequenceEnumerative>($1);
		ASSERT(pSequence);
		pSequence->addValue(CONVERTER->stack().pop<RDOValue>($2));
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body RDO_BOOL_CONST
	{
		LPRDOFUNSequenceEnumerative pSequence = CONVERTER->stack().pop<RDOFUNSequenceEnumerative>($1);
		ASSERT(pSequence);
		pSequence->addValue(CONVERTER->stack().pop<RDOValue>($2));
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body RDO_STRING_CONST
	{
		LPRDOFUNSequenceEnumerative pSequence = CONVERTER->stack().pop<RDOFUNSequenceEnumerative>($1);
		ASSERT(pSequence);
		pSequence->addValue(CONVERTER->stack().pop<RDOValue>($2));
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body RDO_IDENTIF
	{
		LPRDOFUNSequenceEnumerative pSequence = CONVERTER->stack().pop<RDOFUNSequenceEnumerative>($1);
		ASSERT(pSequence);
		pSequence->addValue(CONVERTER->stack().pop<RDOValue>($2));
		$$ = CONVERTER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body error
	{
		CONVERTER->error().error(@1, @2, _T("Ожидается элемент последовательности или ключевое слово $End"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Описание типа параметра
// --------------------------------------------------------------------------------
param_type
	: RDO_integer param_type_range
	{
		LPDocUpdate pReplace = rdo::Factory<UpdateReplace>::create(@1.m_first_seek, @1.m_last_seek, _T("int"));
		ASSERT(pReplace);
		//CONVERTER->insertDocUpdate(pReplace);

		LPRDOTypeRangeRange pRange = CONVERTER->stack().pop<RDOTypeRangeRange>($2);
		LPRDOTypeParam pType;
		if (pRange)
		{
			if (pRange->getMin()->typeID() != rdo::runtime::RDOType::t_int ||
			    pRange->getMax()->typeID() != rdo::runtime::RDOType::t_int)
			{
				CONVERTER->error().error(@2, _T("Диапазон целого типа должен быть целочисленным"));
			}
			LPRDOTypeIntRange pIntRange = rdo::Factory<RDOTypeIntRange>::create(pRange);
			ASSERT(pIntRange);
			pType = rdo::Factory<RDOTypeParam>::create(pIntRange, RDOParserSrcInfo(@1, @2));
		}
		else
		{
			pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__int>::create(), RDOParserSrcInfo(@1, @2));
		}
		ASSERT(pType);
		$$ = CONVERTER->stack().push(pType);
	}
	| RDO_real param_type_range
	{
		LPRDOTypeRangeRange pRange = CONVERTER->stack().pop<RDOTypeRangeRange>($2);
		LPRDOTypeParam pType;
		if (pRange)
		{
			LPRDOTypeRealRange pRealRange = rdo::Factory<RDOTypeRealRange>::create(pRange);
			ASSERT(pRealRange);
			pType = rdo::Factory<RDOTypeParam>::create(pRealRange, RDOParserSrcInfo(@1, @2));
		}
		else
		{
			pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo(@1, @2));
		}
		ASSERT(pType);
		$$ = CONVERTER->stack().push(pType);
	}
	| RDO_string
	{
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__string>::create(), RDOParserSrcInfo(@1));
		ASSERT(pType);
		$$ = CONVERTER->stack().push(pType);
	}
	| RDO_bool
	{
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__bool>::create(), RDOParserSrcInfo(@1));
		ASSERT(pType);
		$$ = CONVERTER->stack().push(pType);
	}
	| param_type_enum
	{
		LEXER->enumReset();
		LPRDOEnumType pEnum = CONVERTER->stack().pop<RDOEnumType>($1);
		ASSERT(pEnum);
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(pEnum, RDOParserSrcInfo(@1));
		ASSERT(pType);
		$$ = CONVERTER->stack().push(pType);
	}
	| param_type_such_as
	{
		LPRDOTypeParamSuchAs pTypeSuchAs = CONVERTER->stack().pop<RDOTypeParamSuchAs>($1);
		ASSERT(pTypeSuchAs);
		LPRDOTypeParam pType = pTypeSuchAs.object_parent_cast<RDOTypeParam>();
		ASSERT(pType);
		$$ = CONVERTER->stack().push(pType);
	}
	;

param_type_range
	: /* empty */
	{
		$$ = CONVERTER->stack().push<RDOTypeRangeRange>(LPRDOTypeRangeRange());
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = CONVERTER->stack().push(pRange);
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = CONVERTER->stack().push(pRange);
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = CONVERTER->stack().push(pRange);
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(CONVERTER->stack().pop<RDOValue>($2), CONVERTER->stack().pop<RDOValue>($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = CONVERTER->stack().push(pRange);
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST error
	{
		CONVERTER->error().error(@4, _T("Диапазон задан неверно"));
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST error
	{
		CONVERTER->error().error(@4, _T("Диапазон задан неверно"));
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST error
	{
		CONVERTER->error().error(@4, _T("Диапазон задан неверно"));
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST error
	{
		CONVERTER->error().error(@4, _T("Диапазон задан неверно"));
	}
	| '[' RDO_REAL_CONST RDO_dblpoint error
	{
		CONVERTER->error().error(@4, _T("Диапазон задан неверно"));
	}
	| '[' RDO_INT_CONST RDO_dblpoint error
	{
		CONVERTER->error().error(@4, _T("Диапазон задан неверно"));
	}
	| '[' error
	{
		CONVERTER->error().error(@2, _T("Диапазон задан неверно"));
	}
	;

param_type_enum
	: '(' param_type_enum_list ')'
	{
		LPRDOEnumType pEnum = CONVERTER->stack().pop<RDOEnumType>($2);
		ASSERT(pEnum);
		$$ = CONVERTER->stack().push(pEnum);
	}
	| '(' param_type_enum_list error
	{
		CONVERTER->error().error(@2, _T("Перечисление должно заканчиваться скобкой"));
	}
	;

param_type_enum_list
	: RDO_IDENTIF
	{
		LPRDOEnumType pEnum = rdo::Factory<RDOEnumType>::create();
		ASSERT(pEnum);
		pEnum->add(CONVERTER->stack().pop<RDOValue>($1));
		LEXER->enumBegin();
		$$ = CONVERTER->stack().push(pEnum);
	}
	| param_type_enum_list ',' RDO_IDENTIF
	{
		if (!LEXER->enumEmpty())
		{
			LPRDOEnumType pEnum = CONVERTER->stack().pop<RDOEnumType>($1);
			ASSERT(pEnum);
			pEnum->add(CONVERTER->stack().pop<RDOValue>($3));
			$$ = CONVERTER->stack().push(pEnum);
		}
		else
		{
			CONVERTER->error().error(@3, _T("Ошибка в описании значений перечислимого типа"));
		}
	}
	| param_type_enum_list RDO_IDENTIF
	{
		if (!LEXER->enumEmpty())
		{
			LPRDOEnumType pEnum = CONVERTER->stack().pop<RDOEnumType>($1);
			ASSERT(pEnum);
			pEnum->add(CONVERTER->stack().pop<RDOValue>($2));
			$$ = CONVERTER->stack().push(pEnum);
			CONVERTER->error().warning(@1, rdo::format(_T("Пропущена запятая перед: %s"), CONVERTER->stack().pop<RDOValue>($2)->value().getIdentificator().c_str()));
		}
		else
		{
			CONVERTER->error().error(@2, _T("Ошибка в описании значений перечислимого типа"));
		}
	}
	| param_type_enum_list ',' RDO_INT_CONST
	{
		CONVERTER->error().error(@3, _T("Значение перечислимого типа не может быть цифрой"));
	}
	| param_type_enum_list ',' RDO_REAL_CONST
	{
		CONVERTER->error().error(@3, _T("Значение перечислимого типа не может быть цифрой"));
	}
	| param_type_enum_list RDO_INT_CONST
	{
		CONVERTER->error().error(@2, _T("Значение перечислимого типа не может быть цифрой"));
	}
	| param_type_enum_list RDO_REAL_CONST
	{
		CONVERTER->error().error(@2, _T("Значение перечислимого типа не может быть цифрой"));
	}
	| RDO_INT_CONST
	{
		CONVERTER->error().error(@1, _T("Значение перечислимого типа не может начинаться с цифры"));
	}
	| RDO_REAL_CONST
	{
		CONVERTER->error().error(@1, _T("Значение перечислимого типа не может начинаться с цифры"));
	}
	;

param_type_such_as
	: RDO_such_as RDO_IDENTIF '.' RDO_IDENTIF
	{
		tstring type  = CONVERTER->stack().pop<RDOValue>($2)->value().getIdentificator();
		tstring param = CONVERTER->stack().pop<RDOValue>($4)->value().getIdentificator();
		LPRDORTPResType pResType = CONVERTER->findRTPResType(type);
		if (!pResType)
		{
			CONVERTER->error().error(@2, rdo::format(_T("Ссылка на неизвестный тип ресурса: %s"), type.c_str()));
		}
		LPRDORTPParam pRTPParam = pResType->findRTPParam(param);
		if (!pRTPParam)
		{
			CONVERTER->error().error(@4, rdo::format(_T("Ссылка на неизвестный параметр ресурса: %s.%s"), type.c_str(), param.c_str()));
		}
		LPRDOParam pParam = pRTPParam.object_parent_cast<RDOParam>();
		ASSERT(pParam);
		LPRDOTypeParamSuchAs pTypeSuchAs = rdo::Factory<RDOTypeParamSuchAs>::create(pParam);
		ASSERT(pTypeSuchAs);
		$$ = CONVERTER->stack().push(pTypeSuchAs);
	}
	| RDO_such_as RDO_IDENTIF
	{
		tstring constName = CONVERTER->stack().pop<RDOValue>($2)->value().getIdentificator();
		LPRDOFUNConstant pConstant = CONVERTER->findFUNConstant(constName);
		if (!pConstant)
		{
			CONVERTER->error().error(@2, rdo::format(_T("Ссылка на несуществующую константу: %s"), constName.c_str()));
		}
		LPRDOParam pParam = pConstant.object_parent_cast<RDOParam>();
		ASSERT(pParam);
		LPRDOTypeParamSuchAs pTypeSuchAs = rdo::Factory<RDOTypeParamSuchAs>::create(pParam);
		ASSERT(pTypeSuchAs);
		$$ = CONVERTER->stack().push(pTypeSuchAs);
	}
	| RDO_such_as RDO_IDENTIF '.' error
	{
		tstring type = CONVERTER->stack().pop<RDOValue>($2)->value().getIdentificator();
		LPRDORTPResType pResType = CONVERTER->findRTPResType(type);
		if (!pResType)
		{
			CONVERTER->error().error(@2, rdo::format(_T("Ссылка на неизвестный тип ресурса: %s"), type.c_str()));
		}
		else
		{
			CONVERTER->error().error(@4, _T("Ошибка при указании параметра"));
		}
	}
	| RDO_such_as error
	{
		CONVERTER->error().error(@2, _T("После ключевого слова such_as необходимо указать тип и параметер ресурса для ссылки"));
	}
	;

param_value_default
	: /* empty */
	{
		$$ = CONVERTER->stack().push(rdo::Factory<RDOValue>::create());
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
			CONVERTER->error().error(src_info, _T("Не указано значение по умолчанию"));
		}
		else
		{
			CONVERTER->error().error(src_info, _T("Неверное значение по умолчанию"));
		}
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
				rdo::Factory<rdo::runtime::RDOCalcUMinus>::create(info.src_pos(), pArithm->createCalc()).object_parent_cast<rdo::runtime::RDOCalc>()
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
		LPRDOFUNLogic pTrueLogic = rdo::Factory<RDOFUNLogic>::create(rdo::Factory<rdo::runtime::RDOCalcConst>::create(1), false);
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
		rdo::runtime::LPRDOCalcConst pCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(1);
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

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

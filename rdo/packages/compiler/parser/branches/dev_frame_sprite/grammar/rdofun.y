/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofun.y
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Поподьянец Евгений (kurt.gigacore@gmail.com)
  \date      20.02.2003
  \brief     Синтаксис функций, констант и последовательностей
  \indent    4T
*/

%{
#define YYPARSE_PARAM lexer
#define YYLEX_PARAM lexer
%}

%pure-parser

%token RDO_Resource_type
%token RDO_permanent
%token RDO_Parameters
%token RDO_integer
%token RDO_real
%token RDO_End
%token RDO_temporary
%token RDO_IDENTIF
%token RDO_INT_CONST
%token RDO_REAL_CONST
%token RDO_such_as
%token RDO_dblpoint
%token RDO_Resources
%token RDO_trace
%token RDO_no_trace
%token RDO_IDENTIF_COLON
%token RDO_Constant
%token RDO_Body
%token RDO_Function
%token RDO_Type
%token RDO_algorithmic
%token RDO_table
%token RDO_list
%token RDO_Exist
%token RDO_Not_Exist
%token RDO_For_All
%token RDO_Not_For_All
%token RDO_neq
%token RDO_leq
%token RDO_geq
%token RDO_NoCheck
%token RDO_Calculate_if
%token RDO_or
%token RDO_and
%token RDO_Sequence
%token RDO_uniform
%token RDO_exponential
%token RDO_normal
%token RDO_triangular
%token RDO_by_hist
%token RDO_enumerative

%token RDO_Pattern
%token RDO_operation
%token RDO_rule
%token RDO_keyboard
%token RDO_Relevant_resources
%token RDO_Keep
%token RDO_Create
%token RDO_Erase
%token RDO_NonExist
%token RDO_IDENTIF_NoChange
%token RDO_Time
%token RDO_Choice
%token RDO_from
%token RDO_first
%token RDO_Convert_begin
%token RDO_Convert_end
%token RDO_Convert_rule
%token RDO_Convert_event
%token RDO_with_max
%token RDO_with_min
%token RDO_set
%token RDO_IDENTIF_NoChange_NoChange
%token RDO_Operations

%token RDO_Results
%token RDO_watch_par
%token RDO_watch_state
%token RDO_watch_quant
%token RDO_watch_value
%token RDO_get_value

%token RDO_Show_mode
%token RDO_Frame_number
%token RDO_Show_rate
%token RDO_Run_StartTime
%token RDO_Trace_StartTime
%token RDO_Trace_EndTime
%token RDO_Terminate_if
%token RDO_Break_point
%token RDO_Seed
%token RDO_NoShow
%token RDO_Monitor
%token RDO_Animation
%token RDO_NoChange

%token RDO_ProcessStart
%token RDO_Decision_point
%token RDO_search
%token RDO_trace_stat
%token RDO_trace_tops
%token RDO_trace_all
%token RDO_Condition
%token RDO_Term_condition
%token RDO_Evaluate_by
%token RDO_Compare_tops
%token RDO_NO
%token RDO_YES
%token RDO_Activities
%token RDO_value_before
%token RDO_value_after
%token RDO_some
%token RDO_Process
%token RDO_SEIZE
%token RDO_GENERATE
%token RDO_TERMINATE
%token RDO_ADVANCE
%token RDO_RELEASE
%token RDO_if
%token RDO_for
%token RDO_Return
%token RDO_Break
%token RDO_CF
%token RDO_Priority
%token RDO_prior
%token RDO_Parent
%token RDO_PlusEqual
%token RDO_MinusEqual
%token RDO_MultiplyEqual
%token RDO_DivideEqual
%token RDO_array
%token RDO_event
%token RDO_Planning
%token RDO_else
%token RDO_IncrEqual
%token RDO_DecrEqual
%token RDO_Stopping
%token RDO_WatchStart
%token RDO_WatchStop
%token RDO_Multithreading

%token RDO_Frame
%token RDO_Back_picture
%token RDO_frm_cell
%token RDO_text
%token RDO_bitmap
%token RDO_s_bmp
%token RDO_rect
%token RDO_r_rect
%token RDO_line
%token RDO_circle
%token RDO_ellipse
%token RDO_triang
%token RDO_active
%token RDO_ruler
%token RDO_space
%token RDO_color_transparent
%token RDO_color_last
%token RDO_color_white
%token RDO_color_black
%token RDO_color_red
%token RDO_color_green
%token RDO_color_blue
%token RDO_color_cyan
%token RDO_color_magenta
%token RDO_color_yellow
%token RDO_color_gray

%token RDO_IDENTIF_RELRES
%token RDO_typedef
%token RDO_enum

%token RDO_STRING_CONST
%token RDO_STRING_CONST_BAD
%token RDO_IDENTIF_BAD
%token RDO_Select
%token RDO_Size
%token RDO_Empty
%token RDO_Select_Array
%token RDO_not
%token RDO_UMINUS
%token RDO_string
%token RDO_bool
%token RDO_BOOL_CONST
%token RDO_Fuzzy
%token RDO_Fuzzy_Term
%token RDO_eq
%token RDO_External_Model
%token RDO_QUEUE
%token RDO_DEPART
%token RDO_ASSIGN

%token RDO_Sprite
%token RDO_sprite_call

%{
// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/type/range.h"
#include "simulator/compiler/parser/rdo_array.h"
#include "simulator/compiler/parser/type/such_as.h"
#include "simulator/compiler/parser/local_variable.h"
#include "simulator/compiler/parser/context/context_type.h"
#include "simulator/compiler/parser/context/memory.h"
#include "simulator/compiler/parser/context/statement.h"
#include "simulator/runtime/calc/procedural/calc_locvar.h"
#include "simulator/runtime/calc/procedural/calc_statement.h"
#include "simulator/runtime/calc/procedural/calc_braces.h"
#include "simulator/runtime/calc/calc_array.h"
// --------------------------------------------------------------------------------

#define PARSER  LEXER->parser()
#define RUNTIME PARSER->runtime()

OPEN_RDO_PARSER_NAMESPACE
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
		if (PARSER->getFUNConstants().empty())
		{
			PARSER->error().error(@1, _T("Ожидается описание функции, последовательности или константы"));
		}
		else
		{
			PARSER->error().error(@1, _T("Ожидается описание функции или последовательности"));
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
		PARSER->error().error(@2, _T("После описания констант ожидается ключевое слово $End"));
	}
	;

fun_const_body
	: /* empty */
	| fun_const_body fun_const_param_desc
	| fun_const_body error
	{
		PARSER->error().error(@2, _T("Ожидается описание константы"));
	}
	;

fun_const_param_desc
	: RDO_IDENTIF_COLON param_type param_value_default
	{
		LPRDOValue pName = PARSER->stack().pop<RDOValue>($1);
		ASSERT(pName);
		PARSER->checkFunctionName(pName->src_info());
		LPTypeInfo pParamType = PARSER->stack().pop<TypeInfo>($2);
		ASSERT(pParamType);
		LPRDOFUNConstant pConstant = rdo::Factory<RDOFUNConstant>::create(pName->src_info(), pParamType, PARSER->stack().pop<RDOValue>($3));
		ASSERT(pConstant);
		RUNTIME->setConstValue(pConstant->getNumber(), pConstant->getDefault()->value());
		$$ = PARSER->stack().push(pConstant);
	}
	| RDO_IDENTIF_COLON
	{
		PARSER->error().error(@1, _T("Ожидается тип константы"));
	}
	| RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@2, _T("Ошибка описания типа константы"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Последовательности и функции
// --------------------------------------------------------------------------------
fun_func_seq
	: /* empty */
	| fun_func_seq fun_func_descr
	| fun_func_seq fun_seq_descr
	{
		LPRDOFUNSequence pSequence = PARSER->stack().pop<RDOFUNSequence>($2);
		ASSERT(pSequence);
	}
	| fun_func_seq RDO_Constant
	{
		PARSER->error().error(@2, _T("Константы долны быть описаны первыми, перед функциями и последовательностями"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Функции
// --------------------------------------------------------------------------------
fun_func_descr
	: fun_func_header fun_func_footer
	{
		LPRDOFUNFunction pFunction = PARSER->stack().pop<RDOFUNFunction>($1);
		ASSERT(pFunction);
		pFunction->end();
	}
	| fun_func_header error
	{
		LPRDOFUNFunction pFunction = PARSER->stack().pop<RDOFUNFunction>($1);
		ASSERT(pFunction);
		PARSER->error().error(@2, rdo::format(_T("Ожидается ключевое слово $Type с указанием типа функции '%s'"), pFunction->name().c_str()));
	}
	;

fun_func_header
	: RDO_Function RDO_IDENTIF_COLON param_type param_value_default
	{
		LPRDOValue pName = PARSER->stack().pop<RDOValue>($2);
		ASSERT(pName);
		PARSER->checkFunctionName(pName->src_info());
		LPTypeInfo pRetType = PARSER->stack().pop<TypeInfo>($3);
		ASSERT(pRetType);
		LPRDOParam pReturn = rdo::Factory<RDOParam>::create(pName->src_info(), pRetType, PARSER->stack().pop<RDOValue>($4));
		ASSERT(pReturn);
		LPRDOFUNFunction pFunction = rdo::Factory<RDOFUNFunction>::create(pName->src_info(), pReturn);
		ASSERT(pFunction);
		$$ = PARSER->stack().push(pFunction);
	}
	| RDO_Function RDO_IDENTIF_COLON error
	{
		LPRDOValue pName = PARSER->stack().pop<RDOValue>($2);
		ASSERT(pName);
		PARSER->error().error(@3, rdo::format(_T("Ожидается тип возвращаемого значения функции '%s'"), pName->value().getIdentificator().c_str()));
	}
	| RDO_Function error
	{
		PARSER->error().error(@2, _T("После ключевого слова $Function ожидается имя фунции"));
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
		LPRDOValue pName = PARSER->stack().pop<RDOValue>($2);
		ASSERT(pName);
		LPTypeInfo pType = PARSER->stack().pop<TypeInfo>($3);
		ASSERT(pType);
		LPRDOParam pParam = rdo::Factory<RDOParam>::create(pName->src_info(), pType, PARSER->stack().pop<RDOValue>($4));
		ASSERT(pParam);
		PARSER->getLastFUNFunction()->add(pParam);
	}
	| fun_func_params RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@3, _T("Ожидается тип параметра функции"));
	}
	| fun_func_params error
	{
		PARSER->error().error(@2, _T("Ожидается описание параметра функции в формате <имя>: <тип>"));
	}
	;

fun_func_footer
	: RDO_Type '=' RDO_algorithmic fun_func_parameters alg_body statement_list alg_end
	{
		LPExpression pExpressionFunBody = PARSER->stack().pop<Expression>($6);
		ASSERT(pExpressionFunBody);

		rdo::runtime::LPRDOCalcStatementList pCalcStatementList = pExpressionFunBody->calc().object_dynamic_cast<rdo::runtime::RDOCalcStatementList>();
		ASSERT(pCalcStatementList);

		rdo::runtime::LPRDOCalcBaseStatementList pCalcBaseStatementList = rdo::Factory<rdo::runtime::RDOCalcBaseStatementList>::create();
		ASSERT(pCalcBaseStatementList);

		rdo::runtime::LPRDOCalcOpenBrace pCalcOpenBrace = rdo::Factory<rdo::runtime::RDOCalcOpenBrace>::create();
		ASSERT(pCalcOpenBrace);

		rdo::runtime::LPRDOCalcCloseBrace pCalcCloseBrace = rdo::Factory<rdo::runtime::RDOCalcCloseBrace>::create();
		ASSERT(pCalcCloseBrace);

		pCalcBaseStatementList->addCalcStatement(pCalcOpenBrace);
		pCalcBaseStatementList->addCalcStatement(pCalcStatementList);
		pCalcBaseStatementList->addCalcStatement(pCalcCloseBrace);

		LPExpression pExpressionReturnCatch = PARSER->stack().pop<Expression>($5);
		ASSERT(pExpressionReturnCatch);

		rdo::runtime::LPRDOCalcReturnCatch pCalcReturnCatch = pExpressionReturnCatch->calc().object_dynamic_cast<rdo::runtime::RDOCalcReturnCatch>();
		ASSERT(pCalcReturnCatch);

		pCalcReturnCatch->setTryCalc(pCalcBaseStatementList);

		LPRDOFUNFunction pFunction = PARSER->getLastFUNFunction();
		ASSERT(pFunction);

		LPContextReturnable pContextReturnableFun = PARSER->context()->cast<ContextReturnable>();
		ASSERT(pContextReturnableFun);

		pFunction->setReturnFlag(pContextReturnableFun->returnFlag());

		if (!pFunction->getReturnFlag())
		{
			PARSER->error().warning(pFunction->src_info(), rdo::format(_T("Возможно, не все ветки функции '%s' могут вернуть значение."), pFunction->name().c_str()));
		}

		pFunction->createAlgorithmicCalc();
	}
	| RDO_Type '=' RDO_list fun_func_parameters RDO_Body fun_func_list_body RDO_End
	{
		LPRDOFUNFunction pFunction = PARSER->getLastFUNFunction();
		ASSERT(pFunction);
		pFunction->createListCalc();
	}
	| RDO_Type '=' RDO_table fun_func_parameters RDO_Body fun_func_list_body RDO_End
	{
		LPRDOFUNFunction pFunction = PARSER->getLastFUNFunction();
		ASSERT(pFunction);
		pFunction->createTableCalc(@6);
	}
	| RDO_Type '=' RDO_algorithmic fun_func_parameters alg_body statement_list error
	{
		PARSER->error().error(@7, _T("Ожидается ключевое слово $End"));
	}
	| RDO_Type '=' RDO_algorithmic fun_func_parameters alg_body error
	{
		PARSER->error().error(@6, _T("Неверный синтаксис алгоритмической функции"));
	}
	| RDO_Type '=' RDO_list fun_func_parameters RDO_Body fun_func_list_body error
	{
		PARSER->error().error(@7, _T("Ожидается ключевое слово $End"));
	}
	| RDO_Type '=' RDO_table fun_func_parameters RDO_Body fun_func_list_body error
	{
		PARSER->error().error(@7, _T("Ожидается ключевое слово $End"));
	}
	| RDO_Type '=' RDO_algorithmic error
	{
		PARSER->error().error(@4, _T("Ожидается ключевое слово $Parameters"));
	}
	| RDO_Type '=' RDO_list error
	{
		PARSER->error().error(@4, _T("Ожидается ключевое слово $Parameters"));
	}
	| RDO_Type '=' RDO_table error
	{
		PARSER->error().error(@4, _T("Ожидается ключевое слово $Parameters"));
	}
	| RDO_Type '=' error
	{
		PARSER->error().error(@3, _T("Неизвестный тип функции"));
	}
	| RDO_Type error
	{
		PARSER->error().error(@2, _T("После ключевого слова $Type ожидается тип функции"));
	}
	;

alg_body
	: RDO_Body
	{
		ContextMemory::push();

		rdo::runtime::LPRDOCalcReturnCatch pCalcReturnCatch = rdo::Factory<rdo::runtime::RDOCalcReturnCatch>::create();
		ASSERT(pCalcReturnCatch);

		LPRDOFUNFunction pFunction = PARSER->getLastFUNFunction();
		ASSERT(pFunction);

		pFunction->setFunctionCalc(pCalcReturnCatch);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__void>(RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalcReturnCatch, RDOParserSrcInfo(@1));

		LPContextReturnable pContextReturnable = rdo::Factory<ContextReturnable>::create();
		ASSERT(pContextReturnable);

		PARSER->contextStack()->push(pContextReturnable);
		
		$$ = PARSER->stack().push(pExpression);
	}
	;

alg_end
	: RDO_End
	{
		ContextMemory::pop();
	}
	;

fun_func_list_body
	: /* empty */
	| fun_func_list_body fun_func_list_value
	;

fun_func_list_value
	: RDO_IDENTIF
	{
		LPRDOValue pItem = PARSER->stack().pop<RDOValue>($1);
		ASSERT(pItem);
		LPRDOFUNFunctionListElementIdentif pValue = rdo::Factory<RDOFUNFunctionListElementIdentif>::create(RDOParserSrcInfo(@1, pItem->value().getIdentificator()));
		ASSERT(pValue);
		PARSER->getLastFUNFunction()->add(pValue.object_parent_cast<RDOFUNFunctionListElement>());
		$$ = PARSER->stack().push(pValue);
	}
	| RDO_REAL_CONST
	{
		LPRDOValue pItem = PARSER->stack().pop<RDOValue>($1);
		ASSERT(pItem);
		LPRDOFUNFunctionListElementReal pValue = rdo::Factory<RDOFUNFunctionListElementReal>::create(@1, pItem->value().getDouble());
		ASSERT(pValue);
		PARSER->getLastFUNFunction()->add(pValue.object_parent_cast<RDOFUNFunctionListElement>());
		$$ = PARSER->stack().push(pValue);
	}
	| RDO_INT_CONST
	{
		LPRDOValue pItem = PARSER->stack().pop<RDOValue>($1);
		ASSERT(pItem);
		LPRDOFUNFunctionListElementInt pValue = rdo::Factory<RDOFUNFunctionListElementInt>::create(@1, pItem->value().getInt());
		ASSERT(pValue);
		PARSER->getLastFUNFunction()->add(pValue.object_parent_cast<RDOFUNFunctionListElement>());
		$$ = PARSER->stack().push(pValue);
	}
	| '='
	{
		LPRDOFUNFunctionListElementEq pValue = rdo::Factory<RDOFUNFunctionListElementEq>::create(@1);
		ASSERT(pValue);
		PARSER->getLastFUNFunction()->add(pValue.object_parent_cast<RDOFUNFunctionListElement>());
		$$ = PARSER->stack().push(pValue);
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Последовательности
// --------------------------------------------------------------------------------
fun_seq_descr
	: fun_seq_uniform
	| fun_seq_exponential
	| fun_seq_normal
	| fun_seq_triangular
	| fun_seq_by_hist
	| fun_seq_enumerative
	;

fun_seq_header
	: RDO_Sequence RDO_IDENTIF_COLON param_type RDO_Type '='
	{
		LPRDOValue pName = PARSER->stack().pop<RDOValue>($2);
		ASSERT(pName);
		PARSER->checkFunctionName(pName->src_info());
		LPTypeInfo pType = PARSER->stack().pop<TypeInfo>($3);
		ASSERT(pType);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = rdo::Factory<RDOFUNSequence::RDOFUNSequenceHeader>::create(pType, pName->src_info());
		ASSERT(pHeader);
		$$ = PARSER->stack().push(pHeader);
	}
	| RDO_Sequence RDO_IDENTIF_COLON param_type RDO_Type '=' error
	{
		PARSER->error().error(@6, _T("После знака равенства ожидается тип последовательности"));
	}
	| RDO_Sequence RDO_IDENTIF_COLON param_type RDO_Type error
	{
		PARSER->error().error(@5, _T("После ключевого слова $Type ожидается знак равенства и тип последовательности"));
	}
	| RDO_Sequence RDO_IDENTIF_COLON param_type error
	{
		PARSER->error().error(@4, _T("Ожидается ключевое слово $Type"));
	}
	| RDO_Sequence RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@2, @3, _T("После имени последовательности ожидается тип возвращаемого значения"));
	}
	| RDO_Sequence error
	{
		PARSER->error().error(@1, @2, _T("После ключевого слова $Sequence ожидаются имя и тип результата последовательности в формате '<имя> : <тип>'"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Датчики случайных чисел
// --------------------------------------------------------------------------------
fun_seq_uniform
	: fun_seq_header RDO_uniform RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceUniform>::create(pHeader);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_header RDO_uniform RDO_INT_CONST RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($3);
		ASSERT(pValue);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceUniform>::create(pHeader, pValue->value().getInt());
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_header RDO_uniform RDO_INT_CONST error
	{
		PARSER->error().error(@4, _T("После базы ожидается ключевое слово $End"));
	}
	| fun_seq_header RDO_uniform error
	{
		PARSER->error().error(@3, _T("После типа последовательности ожидается база генератора или ключевое слово $End"));
	}
	;

fun_seq_exponential
	: fun_seq_header RDO_exponential RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceExponential>::create(pHeader);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_header RDO_exponential RDO_INT_CONST RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($3);
		ASSERT(pValue);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceExponential>::create(pHeader, pValue->value().getInt());
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_header RDO_exponential RDO_INT_CONST error
	{
		PARSER->error().error(@4, _T("После базы ожидается ключевое слово $End"));
	}
	| fun_seq_header RDO_exponential error
	{
		PARSER->error().error(@3, _T("После типа последовательности ожидается база генератора или ключевое слово $End"));
	}
	;

fun_seq_normal
	: fun_seq_header RDO_normal RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceNormal>::create(pHeader);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_header RDO_normal RDO_INT_CONST RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($3);
		ASSERT(pValue);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceNormal>::create(pHeader, pValue->value().getInt());
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_header RDO_normal RDO_INT_CONST error
	{
		PARSER->error().error(@4, _T("После базы ожидается ключевое слово $End"));
	}
	| fun_seq_header RDO_normal error
	{
		PARSER->error().error(@3, _T("После типа последовательности ожидается база генератора или ключевое слово $End"));
	}
	;

fun_seq_triangular
	: fun_seq_header RDO_triangular RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceTriangular>::create(pHeader);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_header RDO_triangular RDO_INT_CONST RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($3);
		ASSERT(pValue);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceTriangular>::create(pHeader, pValue->value().getInt());
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_header RDO_triangular RDO_INT_CONST error
	{
		PARSER->error().error(@4, _T("После базы ожидается ключевое слово $End"));
	}
	| fun_seq_header RDO_triangular error
	{
		PARSER->error().error(@3, _T("После типа последовательности ожидается база генератора или ключевое слово $End"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- Гистограмма
// ----------------------------------------------------------------------------
fun_seq_by_hist_header
	: fun_seq_header RDO_by_hist RDO_Body
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pByHistHeader = rdo::Factory<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>::create(pHeader);
		ASSERT(pByHistHeader);
		$$ = PARSER->stack().push(pByHistHeader);
	}
	| fun_seq_header RDO_by_hist RDO_INT_CONST RDO_Body
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($3);
		ASSERT(pValue);
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pByHistHeader = rdo::Factory<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>::create(pHeader, pValue->value().getInt());
		ASSERT(pByHistHeader);
		$$ = PARSER->stack().push(pByHistHeader);
	}
	| fun_seq_header RDO_by_hist RDO_INT_CONST error
	{
		PARSER->error().error(@4, _T("После базы ожидается ключевое слово $Body"));
	}
	| fun_seq_header RDO_by_hist error
	{
		PARSER->error().error(@3, _T("После типа последовательности ожидается база генератора или ключевое слово $Body"));
	}
	;

fun_seq_by_hist_body_real
	: fun_seq_by_hist_header RDO_REAL_CONST RDO_REAL_CONST RDO_REAL_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()));
		}
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@2, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->getHeader()->src_text().c_str()));
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($3), PARSER->stack().pop<RDOValue>($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_REAL_CONST RDO_REAL_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()));
		}
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@3, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->getHeader()->src_text().c_str()));
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($3), PARSER->stack().pop<RDOValue>($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_REAL_CONST RDO_INT_CONST RDO_REAL_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()));
		}
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@2, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->getHeader()->src_text().c_str()));
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($3), PARSER->stack().pop<RDOValue>($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_REAL_CONST RDO_REAL_CONST RDO_INT_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()));
		}
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@2, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->getHeader()->src_text().c_str()));
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($3), PARSER->stack().pop<RDOValue>($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_INT_CONST RDO_REAL_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()));
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($3), PARSER->stack().pop<RDOValue>($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_REAL_CONST RDO_INT_CONST RDO_INT_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()));
		}
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@2, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->getHeader()->src_text().c_str()));
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($3), PARSER->stack().pop<RDOValue>($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_REAL_CONST RDO_INT_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()));
		}
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@3, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->getHeader()->src_text().c_str()));
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($3), PARSER->stack().pop<RDOValue>($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_INT_CONST RDO_INT_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()));
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($3), PARSER->stack().pop<RDOValue>($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_REAL_CONST RDO_REAL_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getTypeInfo()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@2, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->src_text().c_str()));
		}
		pSequence->addReal(PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($3), PARSER->stack().pop<RDOValue>($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_REAL_CONST RDO_REAL_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getTypeInfo()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@3, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->src_text().c_str()));
		}
		pSequence->addReal(PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($3), PARSER->stack().pop<RDOValue>($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_INT_CONST RDO_REAL_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getTypeInfo()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@2, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->src_text().c_str()));
		}
		pSequence->addReal(PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($3), PARSER->stack().pop<RDOValue>($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_REAL_CONST RDO_INT_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getTypeInfo()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@2, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->src_text().c_str()));
		}
		pSequence->addReal(PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($3), PARSER->stack().pop<RDOValue>($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_INT_CONST RDO_REAL_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		pSequence->addReal(PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($3), PARSER->stack().pop<RDOValue>($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_INT_CONST RDO_INT_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getTypeInfo()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@2, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->src_text().c_str()));
		}
		pSequence->addReal(PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($3), PARSER->stack().pop<RDOValue>($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_REAL_CONST RDO_INT_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getTypeInfo()->type()->typeID() == rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@3, rdo::format(_T("Последовательность '%s' определена как целочисленная, её диапазоны тоже должны быть целочисленными"), pHeader->src_text().c_str()));
		}
		pSequence->addReal(PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($3), PARSER->stack().pop<RDOValue>($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_INT_CONST RDO_INT_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		pSequence->addReal(PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($3), PARSER->stack().pop<RDOValue>($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_REAL_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if (pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_int)
		{
			PARSER->error().error(@2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()));
		}
		PARSER->error().error(@2, @3, _T("Ожидается конец диапазона"));
	}
	| fun_seq_by_hist_header RDO_INT_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()));
		}
		PARSER->error().error(@2, @3, _T("Ожидается конец диапазона"));
	}
	| fun_seq_by_hist_header RDO_REAL_CONST RDO_REAL_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()));
		}
		PARSER->error().error(@3, @4, _T("Ожидается относительная вероятность"));
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_REAL_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()));
		}
		PARSER->error().error(@3, @4, _T("Ожидается относительная вероятность"));
	}
	| fun_seq_by_hist_header RDO_REAL_CONST RDO_INT_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()));
		}
		PARSER->error().error(@3, @4, _T("Ожидается относительная вероятность"));
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_INT_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_int )
		{
			PARSER->error().error(@2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()));
		}
		PARSER->error().error(@3, @4, _T("Ожидается относительная вероятность"));
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST error
	{
		PARSER->error().error(@2, @3, _T("Ожидается конец диапазона"));
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST error
	{
		PARSER->error().error(@2, @3, _T("Ожидается конец диапазона"));
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_REAL_CONST error
	{
		PARSER->error().error(@3, @4, _T("Ожидается относительная вероятность"));
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_REAL_CONST error
	{
		PARSER->error().error(@3, @4, _T("Ожидается относительная вероятность"));
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_INT_CONST error
	{
		PARSER->error().error(@3, @4, _T("Ожидается относительная вероятность"));
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_INT_CONST error
	{
		PARSER->error().error(@3, @4, _T("Ожидается относительная вероятность"));
	}
	| fun_seq_by_hist_header RDO_End
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		PARSER->error().error(pHeader->src_info(), rdo::format(_T("Последовательность '%s' не должна быть пустой"), pHeader->src_text().c_str()));
	}
	;

fun_seq_by_hist_body_enum
	: fun_seq_by_hist_header RDO_IDENTIF RDO_REAL_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_enum )
		{
			PARSER->error().error(@2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()));
		}
		LPRDOFUNSequenceByHistEnum pSequence = rdo::Factory<RDOFUNSequenceByHistEnum>::create(pHeader, PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($3));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_IDENTIF RDO_INT_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_enum )
		{
			PARSER->error().error(@2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()));
		}
		LPRDOFUNSequenceByHistEnum pSequence = rdo::Factory<RDOFUNSequenceByHistEnum>::create(pHeader, PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($3));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_enum RDO_IDENTIF RDO_REAL_CONST
	{
		LPRDOFUNSequenceByHistEnum pSequence = PARSER->stack().pop<RDOFUNSequenceByHistEnum>($1);
		ASSERT(pSequence);
		pSequence->addEnum(PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($3));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_enum RDO_IDENTIF RDO_INT_CONST
	{
		LPRDOFUNSequenceByHistEnum pSequence = PARSER->stack().pop<RDOFUNSequenceByHistEnum>($1);
		ASSERT(pSequence);
		pSequence->addEnum(PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($3));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_IDENTIF error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_enum )
		{
			PARSER->error().error(@2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()));
		}
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($2);
		ASSERT(pValue);
		PARSER->error().error(@2, @3, rdo::format(_T("Ожидается относительная вероятность для значения: %s"), pValue->value().getIdentificator().c_str()));
	}
	| fun_seq_by_hist_body_enum RDO_IDENTIF error
	{
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($2);
		ASSERT(pValue);
		PARSER->error().error(@2, @3, rdo::format(_T("Ожидается относительная вероятность для значения: %s"), pValue->value().getIdentificator().c_str()));
	}
	| fun_seq_by_hist_body_enum RDO_REAL_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		PARSER->error().error(@2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()));
	}
	| fun_seq_by_hist_body_enum RDO_INT_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		PARSER->error().error(@2, rdo::format(_T("Значение не соответствует типу последовательности, ожидаемые значение должно соответствовать типу: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()));
	}
	;

fun_seq_by_hist
	: fun_seq_by_hist_body_real RDO_End
	{
		LPRDOFUNSequence pSequence = PARSER->stack().pop<RDOFUNSequence>($1);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_enum RDO_End
	{
		LPRDOFUNSequence pSequence = PARSER->stack().pop<RDOFUNSequence>($1);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real error
	{
		PARSER->error().error(@2, _T("Ошибка в описании последовательности"));
	}
	| fun_seq_by_hist_body_enum error
	{
		PARSER->error().error(@2, _T("Ошибка в описании последовательности"));
	}
	| fun_seq_by_hist_body_real
	{
		PARSER->error().error(@1, _T("Ожидается ключевое слово $End"));
	}
	| fun_seq_by_hist_body_enum
	{
		PARSER->error().error(@1, _T("Ожидается ключевое слово $End"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Перечень значений
// --------------------------------------------------------------------------------
fun_seq_enumerative
	: fun_seq_enumerative_body RDO_End
	{
		LPRDOFUNSequence pSequence = PARSER->stack().pop<RDOFUNSequence>($1);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_header RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		PARSER->error().error(pHeader->src_info(), rdo::format(_T("Последовательность '%s' не должна быть пустой"), pHeader->src_text().c_str()));
	}
	;

fun_seq_enumerative_header
	: fun_seq_header RDO_enumerative RDO_Body
	| fun_seq_header RDO_enumerative RDO_INT_CONST error
	{
		PARSER->error().error(@3, _T("У последовательности типа enumerative нет базы генератора"));
	}
	| fun_seq_header RDO_enumerative RDO_Parameters error
	{
		PARSER->error().error(@3, _T("У последовательности типа enumerative нет параметров"));
	}
	| fun_seq_header RDO_enumerative error
	{
		PARSER->error().error(@3, _T("После типа последовательности ожидается ключевое слово $Body"));
	}
	;

fun_seq_enumerative_body
	: fun_seq_enumerative_header RDO_INT_CONST
	{
		RDOFUNSequenceEnumerative::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequenceEnumerative pSequence = rdo::Factory<RDOFUNSequenceEnumerative>::create(pHeader, PARSER->stack().pop<RDOValue>($2));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_header RDO_REAL_CONST
	{
		RDOFUNSequenceEnumerative::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequenceEnumerative pSequence = rdo::Factory<RDOFUNSequenceEnumerative>::create(pHeader, PARSER->stack().pop<RDOValue>($2));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_header RDO_BOOL_CONST
	{
		RDOFUNSequenceEnumerative::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequenceEnumerative pSequence = rdo::Factory<RDOFUNSequenceEnumerative>::create(pHeader, PARSER->stack().pop<RDOValue>($2));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_header RDO_STRING_CONST
	{
		RDOFUNSequenceEnumerative::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequenceEnumerative pSequence = rdo::Factory<RDOFUNSequenceEnumerative>::create(pHeader, PARSER->stack().pop<RDOValue>($2));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_header RDO_IDENTIF
	{
		RDOFUNSequenceEnumerative::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequenceEnumerative pSequence = rdo::Factory<RDOFUNSequenceEnumerative>::create(pHeader, PARSER->stack().pop<RDOValue>($2));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body RDO_INT_CONST
	{
		LPRDOFUNSequenceEnumerative pSequence = PARSER->stack().pop<RDOFUNSequenceEnumerative>($1);
		ASSERT(pSequence);
		pSequence->addValue(PARSER->stack().pop<RDOValue>($2));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body RDO_REAL_CONST
	{
		LPRDOFUNSequenceEnumerative pSequence = PARSER->stack().pop<RDOFUNSequenceEnumerative>($1);
		ASSERT(pSequence);
		pSequence->addValue(PARSER->stack().pop<RDOValue>($2));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body RDO_BOOL_CONST
	{
		LPRDOFUNSequenceEnumerative pSequence = PARSER->stack().pop<RDOFUNSequenceEnumerative>($1);
		ASSERT(pSequence);
		pSequence->addValue(PARSER->stack().pop<RDOValue>($2));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body RDO_STRING_CONST
	{
		LPRDOFUNSequenceEnumerative pSequence = PARSER->stack().pop<RDOFUNSequenceEnumerative>($1);
		ASSERT(pSequence);
		pSequence->addValue(PARSER->stack().pop<RDOValue>($2));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body RDO_IDENTIF
	{
		LPRDOFUNSequenceEnumerative pSequence = PARSER->stack().pop<RDOFUNSequenceEnumerative>($1);
		ASSERT(pSequence);
		pSequence->addValue(PARSER->stack().pop<RDOValue>($2));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body error
	{
		PARSER->error().error(@1, @2, _T("Ожидается элемент последовательности или ключевое слово $End"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Список statement'ов для функций
// --------------------------------------------------------------------------------
return_statement
	:RDO_Return fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(PARSER->getLastFUNFunction()->getReturn()->getTypeInfo());
		ASSERT(pType);

		rdo::runtime::LPRDOCalc pCalc = pArithm->createCalc(pType);
		ASSERT(pCalc);

		rdo::runtime::LPRDOCalc pCalcReturn = rdo::Factory<rdo::runtime::RDOCalcFunReturn>::create(pCalc);
		ASSERT(pCalcReturn);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalcReturn, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		LPContextReturnable pContextReturnable = PARSER->context()->cast<ContextReturnable>();
		ASSERT(pContextReturnable);

		pContextReturnable->setReturnFlag();

		$$ = PARSER->stack().push(pExpression);
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Список общих statement'ов
// --------------------------------------------------------------------------------
statement
	: empty_statement ';'
	| empty_statement error
	{
		PARSER->error().error(@1, _T("Не найден символ окончания инструкции - точка с запятой"));
	}
	| equal_statement ';'
	| equal_statement error
	{
		PARSER->error().error(@1, _T("Не найден символ окончания инструкции - точка с запятой"));
	}
	| set_array_item_statement ';'
	| set_array_item_statement error
	{
		PARSER->error().error(@1, _T("Не найден символ окончания инструкции - точка с запятой"));
	}
	| local_variable_declaration ';'
	| local_variable_declaration error
	{
		PARSER->error().error(@1, _T("Не найден символ окончания инструкции - точка с запятой"));
	}
	| if_statement
	| RDO_else statement
	{
		PARSER->error().push_only(@1, rdo::format(_T("Нельзя использовать инструкцию else вне оператора if")));
		PARSER->error().push_only(@1, rdo::format(_T("Возможно вы использовали два else для одного if")));
		PARSER->error().push_done();
	}
	| for_statement
	| break_statement ';'
	| break_statement error
	{
		PARSER->error().error(@1, _T("Не найден символ окончания инструкции - точка с запятой"));
	}
	| return_statement ';'
	| return_statement error
	{
		PARSER->error().error(@1, _T("Не найден символ окончания инструкции - точка с запятой"));
	}
	| open_brace statement_list close_brace
	{
		LPExpression pStatementList = PARSER->stack().pop<Expression>($2);
		ASSERT(pStatementList);

		rdo::runtime::LPRDOCalcStatementList pCalcStatementList = pStatementList->calc().object_dynamic_cast<rdo::runtime::RDOCalcStatementList>();
		ASSERT(pCalcStatementList);

		rdo::runtime::LPRDOCalcBaseStatementList pCalcBaseStatementList = rdo::Factory<rdo::runtime::RDOCalcBaseStatementList>::create();
		ASSERT(pCalcBaseStatementList);

		rdo::runtime::LPRDOCalcOpenBrace pCalcOpenBrace = rdo::Factory<rdo::runtime::RDOCalcOpenBrace>::create();
		ASSERT(pCalcOpenBrace);

		rdo::runtime::LPRDOCalcCloseBrace pCalcCloseBrace = rdo::Factory<rdo::runtime::RDOCalcCloseBrace>::create();
		ASSERT(pCalcCloseBrace);

		pCalcBaseStatementList->addCalcStatement(pCalcOpenBrace);
		pCalcBaseStatementList->addCalcStatement(pCalcStatementList);
		pCalcBaseStatementList->addCalcStatement(pCalcCloseBrace);

		LPExpression pExpression = rdo::Factory<Expression>::create(pStatementList->typeInfo(), pCalcBaseStatementList, RDOParserSrcInfo(@1, @3));
		ASSERT(pExpression);

		$$ = PARSER->stack().push(pExpression);
	}
	| error
	{
		PARSER->error().error(@1, _T("Неизвестная инструкция"));
	}
	;

open_brace
	: '{'
	{
		ContextMemory::push();
	}
	;

close_brace
	: '}'
	{
		ContextMemory::pop();
	}
	;

statement_list
	: /* empty */
	{
		rdo::runtime::LPRDOCalcStatementList pCalcStatementList = rdo::Factory<rdo::runtime::RDOCalcStatementList>::create();
		ASSERT(pCalcStatementList);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__void>(RDOParserSrcInfo());
		ASSERT(pType);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalcStatementList, RDOParserSrcInfo());
		ASSERT(pExpression);

		//! @todo некрасивая заточка для привязки калка как точки входа в функцию
		//! 1. statement_list отличается от аналогичного токена в паттернах, сейчас это куда ни шло,
		//!    т.к. паттерна не возвращает никакого значения, но стратегически можно сказать, что он
		//!    возвращает void и свести его к функции
		//! 2. statement_list вызывается для каждых фигурных скобор, поэтому пришлось поставить
		//!    проверку if (!pFunction->getFunctionCalc()). решение некрасивое, м.б. стоит продумать
		//!    механиз линковки, которому достуточно будет на этапе компиляции знать только сигнатуру,
		//!    а точку входа будет назначаться только на этапе линковки
		$$ = PARSER->stack().push(pExpression);
	}
	| statement_list statement
	{
		LPExpression pExpressionStatementList = PARSER->stack().pop<Expression>($1);
		ASSERT(pExpressionStatementList);

		LPExpression pExpressionStatement = PARSER->stack().pop<Expression>($2);
		ASSERT(pExpressionStatement);

		rdo::runtime::LPRDOCalcStatementList pCalcStatementList = pExpressionStatementList->calc().object_dynamic_cast<rdo::runtime::RDOCalcStatementList>();
		ASSERT(pCalcStatementList);

		pCalcStatementList->addCalcStatement(pExpressionStatement->calc());

		pExpressionStatementList->setSrcInfo(RDOParserSrcInfo(@1, @2));

		$$ = PARSER->stack().push(pExpressionStatementList);
	}
	;

empty_statement
	: ';'
	{
		rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOCalcNoChange>::create();
		ASSERT(pCalc);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__void>(RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		$$ = PARSER->stack().push(pExpression);
	}
	| error ';'
	{
		PARSER->error().error(@1, _T("Ошибка в инструкции"));
	}
	;

equal_statement
	: RDO_IDENTIF increment_or_decrement_type
	{
		LPRDOValue              pParamName = PARSER->stack().pop<RDOValue>($1);
		tstring                 paramName  = pParamName->value().getIdentificator();
		rdo::runtime::EqualType equalType  = static_cast<rdo::runtime::EqualType>($2);
		LPContext pContext = PARSER->context();
		ASSERT(pContext);
		LPContextMemory pContextMemory = pContext->cast<ContextMemory>();
		ASSERT(pContextMemory);
		LPLocalVariableListStack pLocalVariableListStack = pContextMemory->getLocalMemory();
		ASSERT(pLocalVariableListStack);
		LPLocalVariable pLocalVariable = pLocalVariableListStack->findLocalVariable(paramName);
		rdo::runtime::LPRDOCalc pCalc;
		LPTypeInfo pLeftArithmType;
		if(pLocalVariable)
		{
			pLeftArithmType = pLocalVariable->getTypeInfo();

			switch (equalType)
			{
				case rdo::runtime::ET_INCR:
				{
					pCalc = rdo::Factory<rdo::runtime::RDOCalcSetLocalVariable<rdo::runtime::ET_INCR> >::create(paramName);
					break;
				}
				case rdo::runtime::ET_DECR:
				{
					pCalc = rdo::Factory<rdo::runtime::RDOCalcSetLocalVariable<rdo::runtime::ET_DECR> >::create(paramName);
					break;
				}
				default:
				{
					NEVER_REACH_HERE;
				}
			}
		}
		else
		{
			RDOFUNArithm::wrongVarInit(pParamName, paramName);
		}

		tstring oprStr;
		switch (equalType)
		{
			case rdo::runtime::ET_INCR:
			{
				oprStr = _T("++");
				break;
			}
			case rdo::runtime::ET_DECR:
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
		pCalc->setSrcInfo(RDOParserSrcInfo(@1, @2, rdo::format(_T("%s %s"), paramName.c_str(), oprStr.c_str())));

		LPExpression pExpression = rdo::Factory<Expression>::create(pLeftArithmType, pCalc, pCalc->srcInfo());
		ASSERT(pExpression);

		$$ = PARSER->stack().push(pExpression);
	}
	| RDO_IDENTIF param_equal_type fun_arithm
	{
		LPRDOValue              pParamName   = PARSER->stack().pop<RDOValue>($1);
		tstring                 paramName    = pParamName->value().getIdentificator();
		rdo::runtime::EqualType equalType    = static_cast<rdo::runtime::EqualType>($2);
		LPRDOFUNArithm          pRightArithm = PARSER->stack().pop<RDOFUNArithm>($3);
		LPContext pContext = PARSER->context();
		ASSERT(pContext);
		LPContextMemory pContextMemory = pContext->cast<ContextMemory>();
		ASSERT(pContextMemory);
		LPLocalVariableListStack pLocalVariableListStack = pContextMemory->getLocalMemory();
		ASSERT(pLocalVariableListStack);
		LPLocalVariable pLocalVariable = pLocalVariableListStack->findLocalVariable(paramName);
		rdo::runtime::LPRDOCalc pCalc;
		rdo::runtime::LPRDOCalc pCalcRight;
		LPTypeInfo pLeftArithmType;
		if (pLocalVariable)
		{
			pLeftArithmType = pLocalVariable->getTypeInfo();

			pCalcRight = pRightArithm->createCalc(pLocalVariable->getTypeInfo());
			switch (equalType)
			{
				case rdo::runtime::ET_NOCHANGE:
				{
					break;
				}
				case rdo::runtime::ET_EQUAL:
				{
					pCalc = rdo::Factory<rdo::runtime::RDOCalcSetLocalVariable<rdo::runtime::ET_EQUAL> >::create(paramName, pCalcRight);
					break;
				}
				case rdo::runtime::ET_PLUS:
				{
					pCalc = rdo::Factory<rdo::runtime::RDOCalcSetLocalVariable<rdo::runtime::ET_PLUS> >::create(paramName, pCalcRight);
					break;
				}
				case rdo::runtime::ET_MINUS:
				{
					pCalc = rdo::Factory<rdo::runtime::RDOCalcSetLocalVariable<rdo::runtime::ET_MINUS> >::create(paramName, pCalcRight);
					break;
				}
				case rdo::runtime::ET_MULTIPLY:
				{
					pCalc = rdo::Factory<rdo::runtime::RDOCalcSetLocalVariable<rdo::runtime::ET_MULTIPLY> >::create(paramName, pCalcRight);
					break;
				}
				case rdo::runtime::ET_DIVIDE:
				{
					pCalc = rdo::Factory<rdo::runtime::RDOCalcSetLocalVariable<rdo::runtime::ET_DIVIDE> >::create(paramName, pCalcRight);
					break;
				}
				default:
				{
					NEVER_REACH_HERE;
				}
			}
		}
		else
		{
			RDOFUNArithm::wrongVarInit(pParamName, paramName);
		}

		tstring oprStr;
		switch (equalType)
		{
			case rdo::runtime::ET_EQUAL:
			{
				oprStr = _T("=");
				break;
			}
			case rdo::runtime::ET_PLUS:
			{
				oprStr = _T("+=");
				break;
			}
			case rdo::runtime::ET_MINUS:
			{
				oprStr = _T("-=");
				break;
			}
			case rdo::runtime::ET_MULTIPLY:
			{
				oprStr = _T("*=");
				break;
			}
			case rdo::runtime::ET_DIVIDE:
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
		pCalc->setSrcInfo(RDOParserSrcInfo(@1, @3, rdo::format(_T("%s %s %s"), paramName.c_str(), oprStr.c_str(), pCalcRight->srcInfo().src_text().c_str())));

		LPExpression pExpression = rdo::Factory<Expression>::create(pLeftArithmType, pCalc, pCalc->srcInfo());
		ASSERT(pExpression);

		$$ = PARSER->stack().push(pExpression);
	}
	| RDO_IDENTIF param_equal_type error
	{
		PARSER->error().error(@3, _T("Ошибка в арифметическом выражении"));
	}
	;

increment_or_decrement_type
	: RDO_IncrEqual
	{
		$$ = rdo::runtime::ET_INCR;
	}
	| RDO_DecrEqual
	{
		$$ = rdo::runtime::ET_DECR;
	}
	;

param_equal_type
	: RDO_set
	{
		$$ = rdo::runtime::ET_EQUAL;
	}
	| '='
	{
		$$ = rdo::runtime::ET_EQUAL;
	}
	| RDO_PlusEqual
	{
		$$ = rdo::runtime::ET_PLUS;
	}
	| RDO_MinusEqual
	{
		$$ = rdo::runtime::ET_MINUS;
	}
	| RDO_MultiplyEqual
	{
		$$ = rdo::runtime::ET_MULTIPLY;
	}
	| RDO_DivideEqual
	{
		$$ = rdo::runtime::ET_DIVIDE;
	}
	;

set_array_item_statement
	: RDO_IDENTIF '[' fun_arithm ']' '=' fun_arithm
	{
		LPRDOValue     pParamName   = PARSER->stack().pop<RDOValue>($1);
		ASSERT(pParamName);
		LPRDOFUNArithm pArrayArithm = RDOFUNArithm::generateByIdentificator(pParamName);
		LPRDOFUNArithm pArithmInd   = PARSER->stack().pop<RDOFUNArithm>($3);
		LPRDOFUNArithm pRightArithm = PARSER->stack().pop<RDOFUNArithm>($6);
		ASSERT(pArrayArithm);
		ASSERT(pArithmInd  );
		ASSERT(pRightArithm);

		if (!pArrayArithm->typeInfo()->type().object_dynamic_cast<RDOArrayType>())
		{
			PARSER->error().error(@1, rdo::format(_T("'%s' не является массивом."), pParamName->value().getIdentificator().c_str()));
		}

		LPRDOType pType = pArrayArithm->typeInfo()->type();
		ASSERT(pType);
		LPRDOArrayType pArrayType = pType.object_dynamic_cast<RDOArrayType>();
		ASSERT(pArrayType);

		LPTypeInfo pItemType = pArrayType->getItemType()->type_cast(pRightArithm->typeInfo(), RDOParserSrcInfo(@1));
		ASSERT(pItemType);

		rdo::runtime::LPRDOCalc pArrayItemCalc = rdo::Factory<rdo::runtime::RDOCalcSetArrayItem>::create(pArrayArithm->calc(), pArithmInd->calc(), pRightArithm->calc());
		ASSERT(pArrayItemCalc);
		
		tstring   paramName = pParamName->value().getIdentificator();
		LPContext pContext  = PARSER->context();
		ASSERT(pContext);
		LPContextMemory pContextMemory = pContext->cast<ContextMemory>();
		ASSERT(pContextMemory);
		LPLocalVariableListStack pLocalVariableListStack = pContextMemory->getLocalMemory();
		ASSERT(pLocalVariableListStack);
		LPLocalVariable pLocalVariable = pLocalVariableListStack->findLocalVariable(paramName);
		rdo::runtime::LPRDOCalc pCalc;
		if (pLocalVariable)
		{
			pCalc = rdo::Factory<rdo::runtime::RDOCalcSetLocalVariable<rdo::runtime::ET_EQUAL> >::create(paramName, pArrayItemCalc);
		}
		else
		{
			RDOFUNArithm::wrongVarInit(pParamName, paramName);
		}

		LPExpression pExpression = rdo::Factory<Expression>::create(pArrayArithm->typeInfo(), pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		$$ = PARSER->stack().push(pExpression);
	}
	;

local_variable_declaration
	: type_declaration init_declaration_list
	{
		LPTypeInfo pType = PARSER->stack().pop<TypeInfo>($1);
		ASSERT(pType);

		rdo::runtime::LPRDOCalc pCalc = PARSER->stack().pop<rdo::runtime::RDOCalc>($2);
		ASSERT(pCalc);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, RDOParserSrcInfo(@1, @2));
		ASSERT(pExpression);

		$$ = PARSER->stack().push(pExpression);

		PARSER->contextStack()->pop();
	}
	;

type_declaration
	: RDO_integer
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__int>(RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPContext pTypeContext = rdo::Factory<TypeContext>::create(pType);
		ASSERT(pTypeContext);

		PARSER->contextStack()->push(pTypeContext);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_real
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__real>(RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPContext pTypeContext = rdo::Factory<TypeContext>::create(pType);
		ASSERT(pTypeContext);

		PARSER->contextStack()->push(pTypeContext);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_string
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__string>(RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPContext pTypeContext = rdo::Factory<TypeContext>::create(pType);
		ASSERT(pTypeContext);

		PARSER->contextStack()->push(pTypeContext);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_array
	{
		LPRDOArrayType pArray = PARSER->stack().pop<RDOArrayType>($1);
		ASSERT(pArray);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pArray, RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPContext pTypeContext = rdo::Factory<TypeContext>::create(pType);
		ASSERT(pTypeContext);

		PARSER->contextStack()->push(pTypeContext);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_bool
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__bool>(RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPContext pTypeContext = rdo::Factory<TypeContext>::create(pType);
		ASSERT(pTypeContext);

		PARSER->contextStack()->push(pTypeContext);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_enum
	{
		LEXER->enumReset();
		LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
		ASSERT(pEnum);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pEnum, RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPContext pTypeContext = rdo::Factory<TypeContext>::create(pType);
		ASSERT(pTypeContext);

		PARSER->contextStack()->push(pTypeContext);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_such_as
	{
		LPTypeInfo pTypeSuchAs = PARSER->stack().pop<TypeInfo>($1);
		ASSERT(pTypeSuchAs);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pTypeSuchAs->type(), RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPContext pTypeContext = rdo::Factory<TypeContext>::create(pType);
		ASSERT(pTypeContext);

		PARSER->contextStack()->push(pTypeContext);
		$$ = PARSER->stack().push(pType);
	}
	;

init_declaration_list
	: init_declaration
	{
		LPVariableWrapper pVariableWrapper = PARSER->stack().pop<VariableWrapper>($1);
		ASSERT(pVariableWrapper);

		LPLocalVariable pLocalVariable = pVariableWrapper->getSecond();
		ASSERT(pLocalVariable);

		LPContext pContext = PARSER->context();
		ASSERT(pContext);

		LPContextMemory pContextMemory = pContext->cast<ContextMemory>();
		ASSERT(pContextMemory);

		LPLocalVariableListStack pLocalVariableListStack = pContextMemory->getLocalMemory();
		ASSERT(pLocalVariableListStack);

		pLocalVariableListStack->append(pLocalVariable);

		rdo::runtime::LPRDOCalc pCalc = pVariableWrapper->getFirst();
		ASSERT(pCalc);

		rdo::runtime::LPRDOCalcLocalVariableList pCalcLocalVariableList = rdo::Factory<rdo::runtime::RDOCalcLocalVariableList>::create();
		ASSERT(pCalcLocalVariableList);

		pCalcLocalVariableList->addCalcLocalVariable(pCalc);

		$$ = PARSER->stack().push(pCalcLocalVariableList);
	}
	| init_declaration_list ',' init_declaration
	{
		LPVariableWrapper pVariableWrapper = PARSER->stack().pop<VariableWrapper>($3);
		ASSERT(pVariableWrapper);

		LPLocalVariable pLocalVariable = pVariableWrapper->getSecond();
		ASSERT(pLocalVariable);

		LPContext pContext = PARSER->context();
		ASSERT(pContext);

		LPContextMemory pContextMemory = pContext->cast<ContextMemory>();
		ASSERT(pContextMemory);

		LPLocalVariableListStack pLocalVariableListStack = pContextMemory->getLocalMemory();
		ASSERT(pLocalVariableListStack);

		pLocalVariableListStack->append(pLocalVariable);

		rdo::runtime::LPRDOCalc pCalc = pVariableWrapper->getFirst();
		ASSERT(pCalc);

		rdo::runtime::LPRDOCalcLocalVariableList pCalcLocalVariableList = PARSER->stack().pop<rdo::runtime::RDOCalcLocalVariableList>($1);
		ASSERT(pCalcLocalVariableList);

		pCalcLocalVariableList->addCalcLocalVariable(pCalc);

		$$ = PARSER->stack().push(pCalcLocalVariableList);
	}
	;

init_declaration
	: RDO_IDENTIF
	{
		LPRDOValue pVariableName = PARSER->stack().pop<RDOValue>($1);
		ASSERT(pVariableName);

		LPContext pContext = PARSER->context();
		ASSERT(pContext);

		LPTypeContext pTypeContext = pContext.object_static_cast<TypeContext>();
		ASSERT(pTypeContext);

		LPTypeInfo pTypeInfo = pTypeContext->getTypeInfo();
		ASSERT(pTypeInfo);

		LPExpression pExpression = rdo::Factory<Expression>::create(
			pTypeInfo,
			rdo::Factory<rdo::runtime::RDOCalcConst>::create(pTypeInfo->type()->get_default()),
			pVariableName->src_info()
		);
		ASSERT(pExpression);

		LPLocalVariable pLocalVariable = rdo::Factory<LocalVariable>::create(pVariableName, pExpression);
		ASSERT(pLocalVariable);

		rdo::runtime::LPRDOCalcCreateLocalVariable pCalcCreateLocalVariable = rdo::Factory<rdo::runtime::RDOCalcCreateLocalVariable>::create(pLocalVariable->getName(), pLocalVariable->getExpression()->calc());
		ASSERT(pCalcCreateLocalVariable);

		LPVariableWrapper pVariableWrapper = rdo::Factory<VariableWrapper>::create(pCalcCreateLocalVariable, pLocalVariable);
		ASSERT(pVariableWrapper);

		$$ = PARSER->stack().push(pVariableWrapper);
	}
	| RDO_IDENTIF '=' fun_arithm
	{
		LPRDOValue pVariableName = PARSER->stack().pop<RDOValue>($1);
		ASSERT(pVariableName);

		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm);

		LPLocalVariable pLocalVariable = rdo::Factory<LocalVariable>::create(pVariableName, pArithm->expression());
		ASSERT(pLocalVariable);

		rdo::runtime::LPRDOCalcCreateLocalVariable pCalcCreateLocalVariable = rdo::Factory<rdo::runtime::RDOCalcCreateLocalVariable>::create(pLocalVariable->getName(), pLocalVariable->getExpression()->calc());
		ASSERT(pCalcCreateLocalVariable);

		LPVariableWrapper pVariableWrapper = rdo::Factory<VariableWrapper>::create(pCalcCreateLocalVariable, pLocalVariable);
		ASSERT(pVariableWrapper);

		$$ = PARSER->stack().push(pVariableWrapper);
	}
	;

else_statement
	: RDO_else statement
	{
		LPExpression pExpressionStatement = PARSER->stack().pop<Expression>($2);
		ASSERT(pExpressionStatement);

		PARSER->contextStack()->pop();

		$$ = PARSER->stack().push(pExpressionStatement);
	}
	;

then_statement
	: statement
	{
		LPExpression pExpressionStatement = PARSER->stack().pop<Expression>($1);
		ASSERT(pExpressionStatement);
		
		LPContextReturnable pContextReturnableChild = rdo::Factory<ContextReturnable>::create();
		ASSERT(pContextReturnableChild);

		PARSER->contextStack()->pop();

		LPContextReturnable pContextReturnableParent = PARSER->context()->cast<ContextReturnable>();
		ASSERT(pContextReturnableParent);

		pContextReturnableParent->addContext(pContextReturnableChild);

		PARSER->contextStack()->push(pContextReturnableChild);

		$$ = PARSER->stack().push(pExpressionStatement);
	}
	;


if_statement
	: if_condition then_statement
	{
		LPExpression pExpressionCondition = PARSER->stack().pop<Expression>($1);
		ASSERT(pExpressionCondition);

		LPExpression pExpressionStatement = PARSER->stack().pop<Expression>($2);
		ASSERT(pExpressionStatement);

		rdo::runtime::LPRDOCalcIf pCalc = pExpressionCondition->calc().object_dynamic_cast<rdo::runtime::RDOCalcIf>();
		ASSERT(pCalc);

		rdo::runtime::LPRDOCalc pCalcStatement = pExpressionStatement->calc();
		ASSERT(pCalcStatement);

		pCalc->setThenStatement(pCalcStatement);

		PARSER->contextStack()->pop();

		$$ = PARSER->stack().push(pExpressionCondition);
	}
	| if_condition then_statement else_statement
	{
		LPExpression pExpressionCondition = PARSER->stack().pop<Expression>($1);
		ASSERT(pExpressionCondition);

		LPExpression pExpressionThenStatement = PARSER->stack().pop<Expression>($2);
		ASSERT(pExpressionThenStatement);

		LPExpression pExpressionElseStatement = PARSER->stack().pop<Expression>($3);
		ASSERT(pExpressionElseStatement);

		rdo::runtime::LPRDOCalcIf pCalc = pExpressionCondition->calc().object_dynamic_cast<rdo::runtime::RDOCalcIf>();
		ASSERT(pCalc);

		rdo::runtime::LPRDOCalc pCalcThenStatement = pExpressionThenStatement->calc();
		ASSERT(pCalcThenStatement);

		rdo::runtime::LPRDOCalc pCalcElseStatement = pExpressionThenStatement->calc();
		ASSERT(pCalcElseStatement);

		pCalc->setThenStatement(pCalcThenStatement);
		pCalc->setElseStatement(pCalcElseStatement);

		$$ = PARSER->stack().push(pExpressionCondition);
	}
	;

if_condition
	: RDO_if '(' fun_logic ')'
	{
		LPRDOFUNLogic pCondition = PARSER->stack().pop<RDOFUNLogic>($3);
		ASSERT(pCondition);

		rdo::runtime::LPRDOCalc pConditionCalc = pCondition->getCalc();
		ASSERT(pConditionCalc);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__void>(RDOParserSrcInfo(@1));
		ASSERT(pType);

		rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOCalcIf>::create(pConditionCalc);
		ASSERT(pCalc);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		LPContextReturnable pContextReturnableChild = rdo::Factory<ContextReturnable>::create();
		ASSERT(pContextReturnableChild);

		LPContextReturnable pContextReturnableParent = PARSER->context()->cast<ContextReturnable>();
		ASSERT(pContextReturnableParent);

		pContextReturnableParent->addContext(pContextReturnableChild);

		PARSER->contextStack()->push(pContextReturnableChild);

		$$ = PARSER->stack().push(pExpression);
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

for_statement
	: for_header statement
	{
		LPExpression pExpressionHeader = PARSER->stack().pop<Expression>($1);
		ASSERT(pExpressionHeader);

		LPExpression pExpressionStatement = PARSER->stack().pop<Expression>($2);
		ASSERT(pExpressionStatement);

		rdo::runtime::LPRDOCalc pCalcStatementList = pExpressionStatement->calc();
		ASSERT(pCalcStatementList);

		rdo::runtime::LPRDOCalcFor pCalcFor = pExpressionHeader->calc().object_dynamic_cast<rdo::runtime::RDOCalcFor>();
		ASSERT(pCalcFor);

		pCalcFor->setStatement(pCalcStatementList);

		rdo::runtime::LPRDOCalcBreakCatch pCalcBreakCatch = rdo::Factory<rdo::runtime::RDOCalcBreakCatch>::create();
		ASSERT(pCalcBreakCatch);
		pCalcBreakCatch->addStatementList(pCalcFor);

		LPExpression pExpression = rdo::Factory<Expression>::create(pExpressionStatement->typeInfo(), pCalcBreakCatch, RDOParserSrcInfo(@1, @2));

		PARSER->contextStack()->pop();

		$$ = PARSER->stack().push(pExpression);
	}
	;

for_header
	: RDO_for '(' local_variable_declaration ';' fun_logic ';' equal_statement ')'
	{
		LPExpression pDeclarationExpression = PARSER->stack().pop<Expression>($3);
		ASSERT(pDeclarationExpression);

		LPRDOFUNLogic pCondition = PARSER->stack().pop<RDOFUNLogic>($5);
		ASSERT(pCondition);
		
		rdo::runtime::LPRDOCalc pConditionCalc = pCondition->getCalc();
		ASSERT(pConditionCalc);

		LPExpression pEqualExpression = PARSER->stack().pop<Expression>($7);
		ASSERT(pEqualExpression);

		rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOCalcFor>::create(
			pDeclarationExpression->calc(),
			pConditionCalc                ,
			pEqualExpression->calc()      );
		ASSERT(pCalc);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__void>(RDOParserSrcInfo(@1, @8));
		ASSERT(pType);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, RDOParserSrcInfo(@1, @8));

		LPContextBreakable pContextBreakable = rdo::Factory<ContextBreakable>::create();
		ASSERT(pContextBreakable);

		PARSER->contextStack()->push(pContextBreakable);

		$$ = PARSER->stack().push(pExpression);
	}
	| RDO_for '(' equal_statement ';' fun_logic ';' equal_statement ')'
	{
		LPExpression pInitExpression = PARSER->stack().pop<Expression>($3);
		ASSERT(pInitExpression);

		LPRDOFUNLogic pCondition = PARSER->stack().pop<RDOFUNLogic>($5);
		ASSERT(pCondition);

		rdo::runtime::LPRDOCalc pConditionCalc = pCondition->getCalc();
		ASSERT(pConditionCalc);

		LPExpression pEqualExpression = PARSER->stack().pop<Expression>($7);
		ASSERT(pEqualExpression);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__void>(RDOParserSrcInfo(@1, @8));
		ASSERT(pType);

		rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOCalcFor>::create(
			pInitExpression->calc()     ,
			pConditionCalc              ,
			pEqualExpression->calc()    );
		ASSERT(pCalc);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, RDOParserSrcInfo(@1, @8));

		LPContextBreakable pContextBreakable = rdo::Factory<ContextBreakable>::create();
		ASSERT(pContextBreakable);

		PARSER->contextStack()->push(pContextBreakable);

		$$ = PARSER->stack().push(pExpression);
	}
	;

break_statement
	:RDO_Break
	{
		LPContext pContext = RDOParser::s_parser()->context();
		ASSERT(pContext);

		if (pContext->cast<ContextBreakable>())
		{
			rdo::runtime::LPRDOCalc pCalcBreak = rdo::Factory<rdo::runtime::RDOCalcFunBreak>::create();
			ASSERT(pCalcBreak);

			LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__void>(RDOParserSrcInfo(@1));
			ASSERT(pType);

			LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalcBreak, RDOParserSrcInfo(@1));
			ASSERT(pExpression);

			$$ = PARSER->stack().push(pExpression);
		}
		else
		{
			PARSER->error().error(@1, _T("Нельзя использовать break вне цикла"));
		}
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Описание типа параметра
// --------------------------------------------------------------------------------
param_type
	: RDO_integer param_type_range
	{
		LPRDOTypeRangeRange pRange = PARSER->stack().pop<RDOTypeRangeRange>($2);
		LPTypeInfo pType;
		if (pRange)
		{
			if (pRange->getMin()->typeID() != rdo::runtime::RDOType::t_int ||
			    pRange->getMax()->typeID() != rdo::runtime::RDOType::t_int)
			{
				PARSER->error().error(@2, _T("Диапазон целого типа должен быть целочисленным"));
			}
			LPRDOTypeIntRange pIntRange = rdo::Factory<RDOTypeIntRange>::create(pRange);
			ASSERT(pIntRange);
			pType = rdo::Factory<TypeInfo>::create(pIntRange, RDOParserSrcInfo(@1, @2));
		}
		else
		{
			pType = rdo::Factory<TypeInfo>::delegate<RDOType__int>(RDOParserSrcInfo(@1, @2));
		}
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_real param_type_range
	{
		LPRDOTypeRangeRange pRange = PARSER->stack().pop<RDOTypeRangeRange>($2);
		LPTypeInfo pType;
		if (pRange)
		{
			LPRDOTypeRealRange pRealRange = rdo::Factory<RDOTypeRealRange>::create(pRange);
			ASSERT(pRealRange);
			pType = rdo::Factory<TypeInfo>::create(pRealRange, RDOParserSrcInfo(@1, @2));
		}
		else
		{
			pType = rdo::Factory<TypeInfo>::delegate<RDOType__real>(RDOParserSrcInfo(@1, @2));
		}
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_string
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__string>(RDOParserSrcInfo(@1));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_array
	{
		LEXER->array_cnt_rst();
		LPRDOArrayType pArray = PARSER->stack().pop<RDOArrayType>($1);
		ASSERT(pArray);
		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pArray, RDOParserSrcInfo(@1));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_bool
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__bool>(RDOParserSrcInfo(@1));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_enum
	{
		LEXER->enumReset();
		LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
		ASSERT(pEnum);
		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pEnum, RDOParserSrcInfo(@1));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_IDENTIF
	{
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($1);
		ASSERT(pValue);

		LPContext pContext = RDOParser::s_parser()->context();
		ASSERT(pContext);

		pContext = pContext->find(pValue);
		ASSERT(pContext);

		LPExpression pExpression = pContext->create(pValue);
		ASSERT(pExpression);

		$$ = PARSER->stack().push(pExpression->typeInfo());
	}
	| param_type_such_as
	{
		LPRDOTypeParamSuchAs pTypeSuchAs = PARSER->stack().pop<RDOTypeParamSuchAs>($1);
		ASSERT(pTypeSuchAs);
		LPTypeInfo pType = pTypeSuchAs.object_parent_cast<TypeInfo>();
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	;

param_type_range
	: /* empty */
	{
		$$ = PARSER->stack().push<RDOTypeRangeRange>(LPRDOTypeRangeRange());
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = PARSER->stack().push(pRange);
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = PARSER->stack().push(pRange);
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = PARSER->stack().push(pRange);
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(PARSER->stack().pop<RDOValue>($2), PARSER->stack().pop<RDOValue>($4), RDOParserSrcInfo(@1, @5));
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
		pEnum->add(PARSER->stack().pop<RDOValue>($1));
		LEXER->enumBegin();
		$$ = PARSER->stack().push(pEnum);
	}
	| param_type_enum_list ',' RDO_IDENTIF
	{
		if (!LEXER->enumEmpty())
		{
			LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
			ASSERT(pEnum);
			LPRDOValue pValue = PARSER->stack().pop<RDOValue>($3);
			ASSERT(pValue);
			pEnum->add(pValue);
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
			LPRDOValue pValue = PARSER->stack().pop<RDOValue>($2);
			ASSERT(pValue);
			pEnum->add(pValue);
			$$ = PARSER->stack().push(pEnum);
			PARSER->error().warning(@1, rdo::format(_T("Пропущена запятая перед: %s"), pValue->value().getIdentificator().c_str()));
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
		tstring type  = PARSER->stack().pop<RDOValue>($2)->value().getIdentificator();
		tstring param = PARSER->stack().pop<RDOValue>($4)->value().getIdentificator();
		LPRDORTPResType pResType = PARSER->findRTPResType(type);
		if (!pResType)
		{
			PARSER->error().error(@2, rdo::format(_T("Ссылка на неизвестный тип ресурса: %s"), type.c_str()));
		}
		LPRDORTPParam pRTPParam = pResType->findRTPParam(param);
		if (!pRTPParam)
		{
			PARSER->error().error(@4, rdo::format(_T("Ссылка на неизвестный параметр ресурса: %s.%s"), type.c_str(), param.c_str()));
		}
		LPRDOParam pParam = pRTPParam.object_parent_cast<RDOParam>();
		ASSERT(pParam);
		LPRDOTypeParamSuchAs pTypeSuchAs = rdo::Factory<RDOTypeParamSuchAs>::create(pParam);
		ASSERT(pTypeSuchAs);
		$$ = PARSER->stack().push(pTypeSuchAs);
	}
	| RDO_such_as RDO_IDENTIF
	{
		tstring constName = PARSER->stack().pop<RDOValue>($2)->value().getIdentificator();
		LPRDOFUNConstant pConstant = PARSER->findFUNConstant(constName);
		if (!pConstant)
		{
			PARSER->error().error(@2, rdo::format(_T("Ссылка на несуществующую константу: %s"), constName.c_str()));
		}
		LPRDOParam pParam = pConstant.object_parent_cast<RDOParam>();
		ASSERT(pParam);
		LPRDOTypeParamSuchAs pTypeSuchAs = rdo::Factory<RDOTypeParamSuchAs>::create(pParam);
		ASSERT(pTypeSuchAs);
		$$ = PARSER->stack().push(pTypeSuchAs);
	}
	| RDO_such_as RDO_IDENTIF '.' error
	{
		tstring type = PARSER->stack().pop<RDOValue>($2)->value().getIdentificator();
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

param_type_array
	: RDO_array '<' param_type '>'
	{
		LPTypeInfo pParamType = PARSER->stack().pop<TypeInfo>($3);
		ASSERT(pParamType);
		LPRDOArrayType pArray = rdo::Factory<RDOArrayType>::create(pParamType, RDOParserSrcInfo(@1, @4));
		$$ = PARSER->stack().push(pArray);
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Общие составные токены для всех объектов РДО
// --------------------------------------------------------------------------------
// -------------------- Описание переменной
// --------------------------------------------------------------------------------
param_value
	: RDO_INT_CONST
	{
		$$ = $1;
	}
	| RDO_REAL_CONST
	{
		$$ = $1;
	}
	| RDO_STRING_CONST
	{
		$$ = $1;
	}
	| RDO_IDENTIF
	{
		$$ = $1;
	}
	| RDO_BOOL_CONST
	{
		$$ = $1;
	}
	| param_array_value
	{
		$$ = $1;
	}
	;

param_array_value
	: '[' array_item ']'
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($2);
		ASSERT(pArrayValue);
		RDOParserSrcInfo srcInfo(@1, @3, pArrayValue->getAsString());
		pArrayValue->setSrcInfo(srcInfo);
		pArrayValue->getArrayType()->setSrcInfo(srcInfo);
		$$ = PARSER->stack().push(rdo::Factory<RDOValue>::create(pArrayValue->getArrayType()->typeInfo(), pArrayValue));
	}
	| '[' array_item error
	{
		PARSER->error().error(@2, _T("Массив должен закрываться скобкой"));
	}
	;

array_item
	: param_value
	{
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($1);
		ASSERT(pValue);
		LPRDOArrayType pArrayType = rdo::Factory<RDOArrayType>::create(pValue->typeInfo(), RDOParserSrcInfo(@1));
		ASSERT(pArrayType);
		LPRDOArrayValue pArrayValue = rdo::Factory<RDOArrayValue>::create(pArrayType);
		ASSERT(pArrayValue);
		pArrayValue->insertItem(pValue);
		$$ = PARSER->stack().push(pArrayValue);
	}
	| array_item ',' param_value
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($1);
		ASSERT(pArrayValue);
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($3);
		ASSERT(pValue);
		pArrayValue->insertItem(pValue);
		$$ = PARSER->stack().push(pArrayValue);
	}
	| array_item param_value
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($1);
		ASSERT(pArrayValue);
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($2);
		ASSERT(pValue);
		pArrayValue->insertItem(pValue);
		$$ = PARSER->stack().push(pArrayValue);
		PARSER->error().warning(@1, rdo::format(_T("Пропущена запятая перед: %s"), pValue->value().getAsString().c_str()));
	}
	;

param_value_default
	: /* empty */
	{
		LPRDOValue pValue = rdo::Factory<RDOValue>::create();
		ASSERT(pValue);
		$$ = PARSER->stack().push(pValue);
	}
	| '=' param_value
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

// --------------------------------------------------------------------------------
// -------------------- Логические выражения
// --------------------------------------------------------------------------------
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
		RDOParserSrcInfo src_info(@1, @2);
		LPRDOFUNLogic pLogicNot = pLogic->operator_not(src_info.src_pos());
		ASSERT(pLogicNot);
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

// --------------------------------------------------------------------------------
// -------------------- Арифметические выражения
// --------------------------------------------------------------------------------
fun_arithm
	: RDO_INT_CONST                      { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($1))); }
	| RDO_REAL_CONST                     { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($1))); }
	| RDO_BOOL_CONST                     { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($1))); }
	| RDO_STRING_CONST                   { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($1))); }
	| param_array_value                  { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($1))); }
	| RDO_IDENTIF                        { $$ = PARSER->stack().push(RDOFUNArithm::generateByIdentificator(PARSER->stack().pop<RDOValue>($1))); }
	| RDO_IDENTIF_RELRES                 { $$ = PARSER->stack().push(RDOFUNArithm::generateByIdentificator(PARSER->stack().pop<RDOValue>($1))); }
	| RDO_IDENTIF '.' RDO_IDENTIF        { $$ = PARSER->stack().push(RDOFUNArithm::generateByIdentificator(PARSER->stack().pop<RDOValue>($1), PARSER->stack().pop<RDOValue>($3))); }
	| RDO_IDENTIF_RELRES '.' RDO_IDENTIF { $$ = PARSER->stack().push(RDOFUNArithm::generateByIdentificator(PARSER->stack().pop<RDOValue>($1), PARSER->stack().pop<RDOValue>($3))); }
	| '*' 
	{
		LPRDOValue pValue = rdo::Factory<RDOValue>::create(RDOParserSrcInfo(@1, _T("*")));
		ASSERT(pValue);
		LPExpression pExpression = rdo::Factory<Expression>::create(pValue);
		ASSERT(pExpression);
		LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
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
		info.setSrcPos(@1, @2);
		$$ = PARSER->stack().push(pArithm->uminus(info.src_pos()));
	}
	| RDO_IDENTIF '.' RDO_Size
	{
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($1);
		ASSERT(pValue);

		LPRDOFUNArithm pArithm = RDOFUNArithm::generateByIdentificator(pValue);
		ASSERT(pArithm);

		rdo::runtime::LPRDOCalc pCalc;
		if (pArithm->typeInfo()->type().object_dynamic_cast<RDOArrayType>())
		{
			pCalc = rdo::Factory<rdo::runtime::RDOCalcArraySize>::create(pArithm->calc());
			ASSERT(pCalc);
		}
		else
		{
			PARSER->error().error(@1, rdo::format(_T("'%s' не является массивом."), pValue->value().getIdentificator().c_str()));
		}

		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__int>(RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		LPRDOFUNArithm pArithmArraySize = rdo::Factory<RDOFUNArithm>::create(pExpression);
		ASSERT(pArithmArraySize);

		$$ = PARSER->stack().push(pArithmArraySize);
	}
	| RDO_IDENTIF '[' fun_arithm ']'
	{
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($1);
		ASSERT(pValue);

		LPRDOFUNArithm pArithm = RDOFUNArithm::generateByIdentificator(pValue);
		ASSERT(pArithm);

		LPRDOFUNArithm pArithmInd = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithmInd);

		LPRDOType pType = pArithm->typeInfo()->type();
		ASSERT(pType);

		LPRDOArrayType pArrayType = pType.object_dynamic_cast<RDOArrayType>();
		if (!pArrayType)
		{
			PARSER->error().error(@1, rdo::format(_T("'%s' не является массивом."), pValue->value().getIdentificator().c_str()));
		}

		rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOCalcArrayItem>::create(pArithm->calc(), pArithmInd->calc());
		ASSERT(pCalc);

		LPTypeInfo pItemType = pArrayType->getItemType();
		ASSERT(pItemType);

		LPExpression pExpression = rdo::Factory<Expression>::create(pItemType, pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		LPRDOFUNArithm pArithmArrayItem = rdo::Factory<RDOFUNArithm>::create(pExpression);
		ASSERT(pArithmArrayItem);

		$$ = PARSER->stack().push(pArithmArrayItem);
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Функции и последовательности
// --------------------------------------------------------------------------------
fun_arithm_func_call
	: RDO_IDENTIF '(' arithm_list ')'
	{
		tstring funName                    = PARSER->stack().pop<RDOValue>($1)->value().getIdentificator();
		LPArithmContainer pArithmContainer = PARSER->stack().pop<ArithmContainer>($3);
		ASSERT(pArithmContainer);

		LPRDOFUNParams pFunParams = rdo::Factory<RDOFUNParams>::create(pArithmContainer);
		ASSERT(pFunParams);

		pFunParams->getFunseqName().setSrcInfo(RDOParserSrcInfo(@1, funName));
		pFunParams->setSrcPos (@1, @4);
		pFunParams->setSrcText(funName + _T("(") + pArithmContainer->src_text() + _T(")"));
		LPRDOFUNArithm pArithm = pFunParams->createCall(funName);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
	| RDO_IDENTIF '(' error
	{
		PARSER->error().error(@3, _T("Ошибка в параметрах функции"));
	}
	;

arithm_list
	: /* empty */
	{
		LPArithmContainer pArithmContainer = rdo::Factory<ArithmContainer>::create();
		ASSERT(pArithmContainer);
		$$ = PARSER->stack().push(pArithmContainer);
	}
	| arithm_list_body
	;

arithm_list_body
	: fun_arithm
	{
		LPArithmContainer pArithmContainer = rdo::Factory<ArithmContainer>::create();
		LPRDOFUNArithm    pArithm          = PARSER->stack().pop<RDOFUNArithm>($1);
		ASSERT (pArithmContainer);
		ASSERT (pArithm);
		pArithmContainer->setSrcText(pArithm->src_text());
		pArithmContainer->addItem   (pArithm);
		$$ = PARSER->stack().push(pArithmContainer);
	}
	| arithm_list_body ',' fun_arithm
	{
		LPArithmContainer pArithmContainer = PARSER->stack().pop<ArithmContainer>($1);
		LPRDOFUNArithm    pArithm          = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT (pArithmContainer);
		ASSERT (pArithm);
		pArithmContainer->setSrcText(pArithmContainer->src_text() + _T(", ") + pArithm->src_text());
		pArithmContainer->addItem   (pArithm);
		$$ = PARSER->stack().push(pArithmContainer);
	}
	| arithm_list_body ',' error
	{
		PARSER->error().error(@3, _T("Ошибка в арифметическом выражении"));
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
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($3);
		ASSERT(pValue);
		$$ = PARSER->stack().push(rdo::Factory<RDOFUNGroupLogic>::create((RDOFUNGroupLogic::FunGroupType)$1, pValue->src_info()));
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
		LPRDOFUNLogic pTrueLogic = RDOFUNLogic::generateTrue(RDOParserSrcInfo(@2, _T("NoCheck")));
		ASSERT(pTrueLogic);
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

// --------------------------------------------------------------------------------
// -------------------- Select
// --------------------------------------------------------------------------------
fun_select_header
	: RDO_Select '(' RDO_IDENTIF_COLON
	{
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($3);
		ASSERT(pValue);
		LPRDOFUNSelect pSelect   = rdo::Factory<RDOFUNSelect>::create(pValue->src_info());
		ASSERT(pSelect);
		pSelect->setSrcText(_T("Select(") + pValue->value().getIdentificator() + _T(": "));
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
		RDOParserSrcInfo info(@2, _T("NoCheck"));
		pSelect->setSrcText(pSelect->src_text() + info.src_text() + _T(")"));
		LPRDOFUNLogic pTrueLogic = RDOFUNLogic::generateTrue(info);
		ASSERT(pTrueLogic);
		pSelect->initSelect(pTrueLogic);
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

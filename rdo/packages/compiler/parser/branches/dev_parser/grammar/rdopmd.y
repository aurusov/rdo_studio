/*!
  \copyright (c) RDO-Team, 2011
  \file      rdopmd.y
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     Синтаксис собираемых показателей
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
#include "simulator/compiler/parser/rdopmd.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/calc_array.h"
#include "simulator/runtime/calc/resource/calc_resource.h"
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

%%

pmd_main
	: /* empty */
	| pmd_main pmd_result_group
	;

pmd_result_group_name
	: /* empty */
	{
		LPRDOResultGroup pResultGroup = PARSER->findResultGroup("");
		if (!pResultGroup)
		{
			pResultGroup = rdo::Factory<RDOResultGroup>::create();
			ASSERT(pResultGroup);
			pResultGroup->init(RDOParserSrcInfo());
		}
		PARSER->contextStack()->push(pResultGroup);
	}
	| RDO_IDENTIF
	{
		LPRDOResultGroup pResultGroup = rdo::Factory<RDOResultGroup>::create();
		ASSERT(pResultGroup);
		pResultGroup->init(RDOParserSrcInfo(@1, PARSER->stack().pop<RDOValue>($1)->value().getIdentificator()));
		PARSER->contextStack()->push(pResultGroup);
	}
	;

pmd_result_group
	: RDO_Results pmd_result_group_name pmd_body RDO_End
	{
		PARSER->contextStack()->pop<RDOResultGroup>();
	}
	| RDO_Results pmd_result_group_name pmd_body error
	{
		PARSER->contextStack()->pop<RDOResultGroup>();
		PARSER->error().error(@3, "Ожидается ключевое слово $End");
	}
	| error
	{
		YYLTYPE pos( @1 );
		pos.m_last_line = pos.m_first_line;
		pos.m_last_pos  = pos.m_first_pos;
		pos.m_last_seek = pos.m_first_seek;
		PARSER->error().error(pos, "Ожидается ключевое слово $Results");
	}
	;

pmd_body
	: /* empty */
	| pmd_body pmd_result
	{
		LPRDOPMDResult pResult = PARSER->stack().pop<RDOPMDResult>($2);
		ASSERT(pResult);
	}
	;

pmd_trace
	: /* empty */
	{
		$$ = 0;
	}
	| RDO_trace
	{
		$$ = 1;
	}
	| RDO_no_trace
	{
		$$ = 0;
	}
	;

pmd_result_watch_quant_begin
	: RDO_IDENTIF_COLON pmd_trace RDO_watch_quant RDO_IDENTIF
	{
		LPRDOValue pName = PARSER->stack().pop<RDOValue>($1);
		LPRDOValue pType = PARSER->stack().pop<RDOValue>($4);
		ASSERT(pName);
		ASSERT(pType);

		LPRDOPMDWatchQuant pWatchQuant = rdo::Factory<RDOPMDWatchQuant>::create(pName->src_info(), pType->src_info());
		ASSERT(pWatchQuant);
		pWatchQuant->init($2 != 0, pType->src_info());
		$$ = PARSER->stack().push(pWatchQuant);
	}
	;

pmd_result_watch_value_begin
	: RDO_IDENTIF_COLON pmd_trace RDO_watch_value RDO_IDENTIF
	{
		LPRDOValue pName = PARSER->stack().pop<RDOValue>($1);
		LPRDOValue pType = PARSER->stack().pop<RDOValue>($4);
		ASSERT(pName);
		ASSERT(pType);

		LPRDOPMDWatchValue pWatchValue = rdo::Factory<RDOPMDWatchValue>::create(pName->src_info(), pType->src_info());
		ASSERT(pWatchValue);
		pWatchValue->init($2 != 0, pType->src_info());
		$$ = PARSER->stack().push(pWatchValue);
	}
	;

pmd_result
	: RDO_IDENTIF_COLON pmd_trace RDO_watch_par RDO_IDENTIF '.' RDO_IDENTIF
	{
		LPRDOPMDWatchPar pResult = rdo::Factory<RDOPMDWatchPar>::create(PARSER->stack().pop<RDOValue>($1)->src_info());
		ASSERT(pResult);
		pResult->init($2 != 0, PARSER->stack().pop<RDOValue>($4)->src_info(), PARSER->stack().pop<RDOValue>($6)->src_info());
		$$ = PARSER->stack().push(pResult);
	}
	| RDO_IDENTIF_COLON pmd_trace RDO_watch_par RDO_IDENTIF '.' error
	{
		PARSER->error().error(@5, @6, "Ожидается имя параметра");
	}
	| RDO_IDENTIF_COLON pmd_trace RDO_watch_par RDO_IDENTIF error
	{
		PARSER->error().error(@4, "Ожидается '.'");
	}
	| RDO_IDENTIF_COLON pmd_trace RDO_watch_par error
	{
		PARSER->error().error(@3, @4, "После ключевого слова watch_par ожидается имя ресурса");
	}
	| RDO_IDENTIF_COLON pmd_trace error
	{
		PARSER->error().error(@2, @3, "Ожидается тип показателя");
	}
	| RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@1, @2, "Ожидается признак трассировки или тип показателя");
	}
	| RDO_IDENTIF error
	{
		PARSER->error().error(@1, @2, "Ожидается ':'");
	}
	| error
	{
		PARSER->error().error(@1, "Ожидается имя показателя");
	}
	| RDO_IDENTIF_COLON pmd_trace RDO_watch_state fun_logic
	{
		LPRDOPMDWatchState pResult = rdo::Factory<RDOPMDWatchState>::create(PARSER->stack().pop<RDOValue>($1)->src_info());
		ASSERT(pResult);
		pResult->init($2 != 0, PARSER->stack().pop<RDOFUNLogic>($4));
		$$ = PARSER->stack().push(pResult);
	}
	| RDO_IDENTIF_COLON pmd_trace RDO_watch_state error
	{
		PARSER->error().error(@3, @4, "После ключевого слова watch_state ожидается логическое выражение");
	}
	| pmd_result_watch_quant_begin fun_logic
	{
		LPRDOPMDWatchQuant pWatchQuant = PARSER->stack().pop<RDOPMDWatchQuant>($1);
		ASSERT(pWatchQuant);
		LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		pWatchQuant->setLogic(pLogic);
		$$ = PARSER->stack().push(pWatchQuant);
	}
	| pmd_result_watch_quant_begin RDO_NoCheck
	{
		LPRDOPMDWatchQuant pWatchQuant = PARSER->stack().pop<RDOPMDWatchQuant>($1);
		ASSERT(pWatchQuant);
		pWatchQuant->setLogicNoCheck();
		$$ = PARSER->stack().push(pWatchQuant);
	}
	| pmd_result_watch_quant_begin error
	{
		PARSER->error().error(@1, @2, "После имени типа ожидается логическое выражение");
	}
	| RDO_IDENTIF_COLON pmd_trace RDO_watch_quant error
	{
		PARSER->error().error(@3, @4, "После ключевого слова watch_quant ожидается тип ресурса");
	}
	| pmd_result_watch_value_begin fun_logic fun_arithm
	{
		LPRDOPMDWatchValue pWatchValue = PARSER->stack().pop<RDOPMDWatchValue>($1);
		ASSERT(pWatchValue);
		LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm);
		pWatchValue->setLogic(pLogic, pArithm);
		$$ = PARSER->stack().push(pWatchValue);
	}
	| pmd_result_watch_value_begin RDO_NoCheck fun_arithm
	{
		LPRDOPMDWatchValue pWatchValue = PARSER->stack().pop<RDOPMDWatchValue>($1);
		ASSERT(pWatchValue);
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm);
		pWatchValue->setLogicNoCheck(pArithm);
		$$ = PARSER->stack().push(pWatchValue);
	}
	| pmd_result_watch_value_begin fun_logic error
	{
		PARSER->error().error(@2, @3, "После логического ожидается арифметическое выражение");
	}
	| pmd_result_watch_value_begin RDO_NoCheck error
	{
		PARSER->error().error(@2, @3, "После логического ожидается арифметическое выражение");
	}
	| pmd_result_watch_value_begin error
	{
		PARSER->error().error(@1, @2, "После имени типа ожидается логическое выражение");
	}
	| RDO_IDENTIF_COLON pmd_trace RDO_watch_value error
	{
		PARSER->error().error(@3, @4, "После ключевого слова watch_value ожидается тип ресурса");
	}
	| RDO_IDENTIF_COLON RDO_get_value fun_arithm
	{
		LPRDOPMDGetValue pResult = rdo::Factory<RDOPMDGetValue>::create(PARSER->stack().pop<RDOValue>($1)->src_info());
		ASSERT(pResult);
		pResult->init(PARSER->stack().pop<RDOFUNArithm>($3));
		$$ = PARSER->stack().push(pResult);
	}
	| RDO_IDENTIF_COLON RDO_get_value error
	{
		PARSER->error().error(@2, @3, "После ключевого слова get_value ожидается арифметическое выражение");
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
		PARSER->error().error(@2, "Массив должен закрываться скобкой");
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
		PARSER->error().warning(@1, rdo::format("Пропущена запятая перед: %s", pValue->value().getAsString().c_str()));
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
			PARSER->error().error(src_info, "Не указано значение по умолчанию");
		}
		else
		{
			PARSER->error().error(src_info, "Неверное значение по умолчанию");
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
		pLogic->setSrcText("[" + pLogic->src_text() + "]");
		$$ = PARSER->stack().push(pLogic);
	}
	| '(' fun_logic ')'
	{
		LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		pLogic->setSrcPos (@1, @3);
		pLogic->setSrcText("(" + pLogic->src_text() + ")");
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
		PARSER->error().error(@2, "Ожидается закрывающаяся скобка");
	}
	| '(' fun_logic error
	{
		PARSER->error().error(@2, "Ожидается закрывающаяся скобка");
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
		LPRDOValue pValue = rdo::Factory<RDOValue>::create(RDOParserSrcInfo(@1, "*"));
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
		pArithm->setSrcText("(" + pArithm->src_text() + ")");
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
			PARSER->error().error(@1, rdo::format("'%s' не является массивом.", pValue->value().getIdentificator().c_str()));
		}

		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__int>(RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		LPRDOFUNArithm pArithmArraySize = rdo::Factory<RDOFUNArithm>::create(pExpression);
		ASSERT(pArithmArraySize);

		$$ = PARSER->stack().push(pArithmArraySize);
	}
	| RDO_IDENTIF '[' fun_arithm ']' '.' RDO_IDENTIF
	{
		LPRDOValue pArrayValue = PARSER->stack().pop<RDOValue>($1);
		ASSERT(pArrayValue);

		LPRDOFUNArithm pArrayArithm = RDOFUNArithm::generateByIdentificator(pArrayValue);
		ASSERT(pArrayArithm);

		LPRDOArrayType pArrayType = pArrayArithm->typeInfo()->type().object_dynamic_cast<RDOArrayType>();
		if (!pArrayType)
		{
			PARSER->error().error(@1, rdo::format("'%s' не является массивом"
				, pArrayValue->value().getIdentificator().c_str())
			);
		}

		LPRDORTPResType pResType = pArrayType->getItemType()->type().object_dynamic_cast<RDORTPResType>();
		if (!pResType)
		{
			PARSER->error().error(@1, rdo::format("'%s' не является массивом ресурсов"
				, pArrayValue->value().getIdentificator().c_str())
			);
		}

		LPRDOFUNArithm pArrayIndex = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArrayIndex);

		LPRDOValue pParamName = PARSER->stack().pop<RDOValue>($6);
		ASSERT(pParamName);

		ruint paramIndex = pResType->getRTPParamNumber(pParamName->value().getAsString());

		if (paramIndex == RDORTPResType::UNDEFINED_PARAM)
		{
			PARSER->error().error(@6, rdo::format("'%s' не является параметром ресурса '%s'"
				, pParamName->value().getAsString().c_str()
				, pResType->name().c_str())
			);
		}

		rdo::runtime::LPRDOCalc pArrayItem = rdo::Factory<rdo::runtime::RDOCalcArrayItem>::create(
			pArrayArithm->calc(),
			pArrayIndex->calc()
		);
		ASSERT(pArrayItem);

		rdo::runtime::LPRDOCalc pParamValue = rdo::Factory<rdo::runtime::RDOCalcGetResourceParam>::create(
			pArrayItem, paramIndex
		);
		ASSERT(pParamValue);

		LPExpression pParamExpression = rdo::Factory<Expression>::create(
			pResType->getParams()[paramIndex]->getTypeInfo(),
			pParamValue,
			RDOParserSrcInfo(@6)
		);
		ASSERT(pParamExpression);

		LPRDOFUNArithm pParamArithm = rdo::Factory<RDOFUNArithm>::create(pParamExpression);
		ASSERT(pParamArithm);

		$$ = PARSER->stack().push(pParamArithm);
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
			PARSER->error().error(@1, rdo::format("'%s' не является массивом.", pValue->value().getIdentificator().c_str()));
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
		pFunParams->setSrcText(funName + "(" + pArithmContainer->src_text() + ")");
		LPRDOFUNArithm pArithm = pFunParams->createCall(funName);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
	| RDO_IDENTIF '(' error
	{
		PARSER->error().error(@3, "Ошибка в параметрах функции");
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
		pArithmContainer->setSrcText(pArithmContainer->src_text() + ", " + pArithm->src_text());
		pArithmContainer->addItem   (pArithm);
		$$ = PARSER->stack().push(pArithmContainer);
	}
	| arithm_list_body ',' error
	{
		PARSER->error().error(@3, "Ошибка в арифметическом выражении");
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
		PARSER->error().error(@3, "Ожидается имя типа");
	}
	| fun_group_keyword error
	{
		PARSER->error().error(@1, "После имени функции ожидается октрывающаяся скобка");
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
		pGroupFun->end();
		$$ = PARSER->stack().push(pGroupFun->createFunLogic(pLogic));
	}
	| fun_group_header RDO_NoCheck ')'
	{
		LPRDOFUNGroupLogic pGroupFun = PARSER->stack().pop<RDOFUNGroupLogic>($1);
		ASSERT(pGroupFun);
		pGroupFun->setSrcPos(@1, @3);
		pGroupFun->end();
		LPRDOFUNLogic pTrueLogic = RDOFUNLogic::generateTrue(RDOParserSrcInfo(@2, "NoCheck"));
		ASSERT(pTrueLogic);
		$$ = PARSER->stack().push(pGroupFun->createFunLogic(pTrueLogic));
	}
	| fun_group_header fun_logic error
	{
		PARSER->error().error(@2, "Ожидается закрывающаяся скобка");
	}
	| fun_group_header RDO_NoCheck error
	{
		PARSER->error().error(@2, "Ожидается закрывающаяся скобка");
	}
	| fun_group_header error
	{
		PARSER->error().error(@1, @2, "Ошибка в логическом выражении");
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
		pSelect->setSrcText("Select(" + pValue->value().getIdentificator() + ": ");
		$$ = PARSER->stack().push(pSelect);
	}
	| RDO_Select '(' error
	{
		PARSER->error().error(@3, "Ожидается имя типа");
	}
	| RDO_Select error
	{
		PARSER->error().error(@1, "Ожидается октрывающаяся скобка");
	}
	;

fun_select_body
	: fun_select_header fun_logic ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		LPRDOFUNLogic  pLogic  = PARSER->stack().pop<RDOFUNLogic> ($2);
		ASSERT(pSelect);
		ASSERT(pLogic );
		pSelect->setSrcText(pSelect->src_text() + pLogic->src_text() + ")");
		pSelect->initSelect(pLogic);
		$$ = PARSER->stack().push(pSelect);
	}
	| fun_select_header RDO_NoCheck ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		RDOParserSrcInfo info(@2, "NoCheck");
		pSelect->setSrcText(pSelect->src_text() + info.src_text() + ")");
		LPRDOFUNLogic pTrueLogic = RDOFUNLogic::generateTrue(info);
		ASSERT(pTrueLogic);
		pSelect->initSelect(pTrueLogic);
		$$ = PARSER->stack().push(pSelect);
	}
	| fun_select_header fun_logic error
	{
		PARSER->error().error(@2, "Ожидается закрывающаяся скобка");
	}
	| fun_select_header RDO_NoCheck error
	{
		PARSER->error().error(@2, "Ожидается закрывающаяся скобка");
	}
	| fun_select_header error
	{
		PARSER->error().error(@1, @2, "Ошибка в логическом выражении");
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
		PARSER->error().error(@4, @5, "Ошибка в логическом выражении");
	}
	| fun_select_body '.' fun_select_keyword error
	{
		PARSER->error().error(@3, "Ожидается октрывающаяся скобка");
	}
	| fun_select_body '.' RDO_Empty '(' ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		pSelect->setSrcPos(@1, @5);
		RDOParserSrcInfo emptyInfo(@3, @5, "Empty()");
		LPRDOFUNLogic pLogic = pSelect->createFunSelectEmpty(emptyInfo);
		ASSERT(pLogic);
		$$ = PARSER->stack().push(pLogic);
	}
	| fun_select_body '.' RDO_Empty '(' error
	{
		PARSER->error().error(@4, "Ожидается закрывающаяся скобка");
	}
	| fun_select_body '.' RDO_Empty error
	{
		PARSER->error().error(@3, "Ожидается октрывающаяся скобка");
	}
	| fun_select_body '.' error
	{
		PARSER->error().error(@2, @3, "Ожидается метод списка ресурсов");
	}
	| fun_select_body error
	{
		PARSER->error().error(@1, "Ожидается '.' (точка) для вызова метода списка ресурсов");
	}
	;

fun_select_arithm
	: fun_select_body '.' RDO_Size '(' ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		pSelect->setSrcPos(@1, @5);
		RDOParserSrcInfo sizeInfo(@3, @5, "Size()");
		LPRDOFUNArithm pArithm = pSelect->createFunSelectSize(sizeInfo);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
	| fun_select_body '.' RDO_Size error
	{
		PARSER->error().error(@3, "Ожидается октрывающаяся скобка");
	}
	| fun_select_body '.' RDO_Size '(' error
	{
		PARSER->error().error(@4, "Ожидается закрывающаяся скобка");
	}
	| fun_select_body '.' RDO_Select_Array '(' ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		pSelect->setSrcPos(@1, @5);
		RDOParserSrcInfo arrayInfo(@3, @5, "Array()");
		LPRDOFUNArithm pArithm = pSelect->createFunSelectArray(arrayInfo);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
	| fun_select_body '.' RDO_Select_Array error
	{
		PARSER->error().error(@3, "Ожидается октрывающаяся скобка");
	}
	| fun_select_body '.' RDO_Select_Array '(' error
	{
		PARSER->error().error(@4, "Ожидается закрывающаяся скобка");
	}
	;

%%

CLOSE_RDO_PARSER_NAMESPACE

/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdopmd.y
 * author   : Александ Барс, Урусов Андрей
 * date     : 20.02.2003
 * bref     : Синтаксис собираемых показателей
 * indent   : 4T
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
%token RDO_result
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
%token RDO_Start
%token RDO_Stop

%token RDO_Frame
%token RDO_Show_if
%token RDO_Back_picture
%token RDO_Show
%token RDO_frm_cell
%token RDO_text
%token RDO_bitmap
%token RDO_s_bmp
%token RDO_rect
%token RDO_r_rect
%token RDO_line
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

%{
// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_parser/rdopmd.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
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

pmd_main
	: pmd_end
	;

pmd_body
	: /* empty */
	| pmd_body pmd_pokaz
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

pmd_pokaz_watch_quant_begin
	: RDO_IDENTIF_COLON pmd_trace RDO_watch_quant RDO_IDENTIF
	{
		LPRDOPMDWatchQuant pWatchQuant = rdo::Factory<RDOPMDWatchQuant>::create(P_RDOVALUE($1)->src_info(), $2 != 0, P_RDOVALUE($4)->src_info());
		ASSERT(pWatchQuant);
		$$ = PARSER->stack().push(pWatchQuant);
	}
	;

pmd_pokaz_watch_value_begin
	: RDO_IDENTIF_COLON pmd_trace RDO_watch_value RDO_IDENTIF
	{
		LPRDOPMDWatchValue pWatchValue = rdo::Factory<RDOPMDWatchValue>::create(P_RDOVALUE($1)->src_info(), $2 != 0, P_RDOVALUE($4)->src_info());
		ASSERT(pWatchValue);
		$$ = PARSER->stack().push(pWatchValue);
	}
	;

pmd_pokaz
	: RDO_IDENTIF_COLON pmd_trace RDO_watch_par RDO_IDENTIF '.' RDO_IDENTIF
	{
		LPRDOPMDPokaz pPokaz = rdo::Factory<RDOPMDWatchPar>::create(P_RDOVALUE($1)->src_info(), $2 != 0, P_RDOVALUE($4)->src_info(), P_RDOVALUE($6)->src_info());
		ASSERT(pPokaz);
		$$ = PARSER->stack().push(pPokaz);
	}
	| RDO_IDENTIF_COLON pmd_trace RDO_watch_par RDO_IDENTIF '.' error
	{
		PARSER->error().error(@5, @6, _T("Ожидается имя параметра"));
	}
	| RDO_IDENTIF_COLON pmd_trace RDO_watch_par RDO_IDENTIF error
	{
		PARSER->error().error(@4, _T("Ожидается '.'"));
	}
	| RDO_IDENTIF_COLON pmd_trace RDO_watch_par error
	{
		PARSER->error().error(@3, @4, _T("После ключевого слова watch_par ожидается имя ресурса"));
	}
	| RDO_IDENTIF_COLON pmd_trace error
	{
		PARSER->error().error(@2, @3, _T("Ожидается тип показателя"));
	}
	| RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@1, @2, _T("Ожидается признак трассировки или тип показателя"));
	}
	| RDO_IDENTIF error
	{
		PARSER->error().error(@1, @2, _T("Ожидается ':'"));
	}
	| error
	{
		PARSER->error().error(@1, _T("Ожидается имя показателя"));
	}
	| RDO_IDENTIF_COLON pmd_trace RDO_watch_state fun_logic
	{
		LPRDOPMDPokaz pPokaz = rdo::Factory<RDOPMDWatchState>::create(P_RDOVALUE($1)->src_info(), $2 != 0, PARSER->stack().pop<RDOFUNLogic>($4));
		ASSERT(pPokaz);
		$$ = PARSER->stack().push(pPokaz);
	}
	| RDO_IDENTIF_COLON pmd_trace RDO_watch_state error
	{
		PARSER->error().error(@3, @4, _T("После ключевого слова watch_state ожидается логическое выражение"));
	}
	| pmd_pokaz_watch_quant_begin fun_logic
	{
		LPRDOPMDWatchQuant pWatchQuant = PARSER->stack().pop<RDOPMDWatchQuant>($1);
		ASSERT(pWatchQuant);
		pWatchQuant->setLogic(PARSER->stack().pop<RDOFUNLogic>($2));
		$$ = PARSER->stack().push(pWatchQuant);
	}
	| pmd_pokaz_watch_quant_begin RDO_NoCheck
	{
		LPRDOPMDWatchQuant pWatchQuant = PARSER->stack().pop<RDOPMDWatchQuant>($1);
		ASSERT(pWatchQuant);
		pWatchQuant->setLogicNoCheck();
		$$ = PARSER->stack().push(pWatchQuant);
	}
	| pmd_pokaz_watch_quant_begin error
	{
		PARSER->error().error(@1, @2, _T("После имени типа ожидается логическое выражение"));
	}
	| RDO_IDENTIF_COLON pmd_trace RDO_watch_quant error
	{
		PARSER->error().error(@3, @4, _T("После ключевого слова watch_quant ожидается тип ресурса"));
	}
	| pmd_pokaz_watch_value_begin fun_logic fun_arithm
	{
		LPRDOPMDWatchValue pWatchValue = PARSER->stack().pop<RDOPMDWatchValue>($1);
		ASSERT(pWatchValue);
		pWatchValue->setLogic(PARSER->stack().pop<RDOFUNLogic>($2), PARSER->stack().pop<RDOFUNArithm>($3));
		$$ = PARSER->stack().push(pWatchValue);
	}
	| pmd_pokaz_watch_value_begin RDO_NoCheck fun_arithm
	{
		LPRDOPMDWatchValue pWatchValue = PARSER->stack().pop<RDOPMDWatchValue>($1);
		ASSERT(pWatchValue);
		pWatchValue->setLogicNoCheck(PARSER->stack().pop<RDOFUNArithm>($3));
		$$ = PARSER->stack().push(pWatchValue);
	}
	| pmd_pokaz_watch_value_begin fun_logic error
	{
		PARSER->error().error(@2, @3, _T("После логического ожидается арифметическое выражение"));
	}
	| pmd_pokaz_watch_value_begin RDO_NoCheck error
	{
		PARSER->error().error(@2, @3, _T("После логического ожидается арифметическое выражение"));
	}
	| pmd_pokaz_watch_value_begin error
	{
		PARSER->error().error(@1, @2, _T("После имени типа ожидается логическое выражение"));
	}
	| RDO_IDENTIF_COLON pmd_trace RDO_watch_value error
	{
		PARSER->error().error(@3, @4, _T("После ключевого слова watch_value ожидается тип ресурса"));
	}
	| RDO_IDENTIF_COLON RDO_get_value fun_arithm
	{
		LPRDOPMDPokaz pPokaz = rdo::Factory<RDOPMDGetValue>::create(P_RDOVALUE($1)->src_info(), PARSER->stack().pop<RDOFUNArithm>($3));
		ASSERT(pPokaz);
		$$ = PARSER->stack().push(pPokaz);
	}
	| RDO_IDENTIF_COLON RDO_get_value error
	{
		PARSER->error().error(@2, @3, _T("После ключевого слова get_value ожидается арифметическое выражение"));
	}
	;

pmd_end
	: RDO_Results pmd_body RDO_End
	| error
	{
		YYLTYPE pos( @1 );
		pos.m_last_line = pos.m_first_line;
		pos.m_last_pos  = pos.m_first_pos;
		pos.m_last_seek = pos.m_first_seek;
		PARSER->error().error(pos, _T("Ожидается ключевое слово $Results"));
	}
	| RDO_Results pmd_body error
	{
		PARSER->error().error(@3, _T("Ожидается ключевое слово $End"));
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
	| param_array_value					 { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
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

param_value
	: RDO_INT_CONST
	{
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
	}
	;

param_array_value
	:	'[' array_item ']'
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($2);
		ASSERT(pArrayValue);
		$$ = (int)PARSER->addValue(new RDOValue(pArrayValue->getRArray(), pArrayValue->getArrayType(), RDOParserSrcInfo(@2)));
	}
	|'[' array_item error {
		PARSER->error().error(@2, _T("Массив должен закрываться скобкой"));
	}
	;

array_item
	: param_value
	{
		LPRDOArrayType pArrayType = rdo::Factory<RDOArrayType>::create(RDOVALUE($1).type(), RDOParserSrcInfo(@1));
		ASSERT(pArrayType);
		LPRDOArrayValue pArrayValue = rdo::Factory<RDOArrayValue>::create(pArrayType);
		pArrayValue->insertItem(RDOVALUE($1));
		$$ = PARSER->stack().push(pArrayValue);
	}
	| array_item ',' param_value
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($1);
		ASSERT(pArrayValue);
		pArrayValue->insertItem(RDOVALUE($3));
		$$ = PARSER->stack().push(pArrayValue);
	}
	| array_item param_value
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($1);
		ASSERT(pArrayValue);
		pArrayValue->insertItem(RDOVALUE($2));
		$$ = PARSER->stack().push(pArrayValue);
		PARSER->error().warning(@1, rdo::format(_T("Пропущена запятая перед: %s"), RDOVALUE($2)->getAsString().c_str()));
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

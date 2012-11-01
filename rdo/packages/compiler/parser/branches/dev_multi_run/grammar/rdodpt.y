/*!
  \copyright (c) RDO-Team, 2011
  \file      rdodpt.y
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     Синтаксис точек принятия решений DPT
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
%token RDO_MultiRun
%token RDO_Run_Count
%token RDO_Next
%token RDO_Seek

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
// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/rdodpt.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdorss.h"
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

%start dpt_main

%%

// --------------------------------------------------------------------------------
// -------------------- DPT
// --------------------------------------------------------------------------------
dpt_main
	: /* empty */
	| dpt_main dpt_search_end
	{
		LPRDODPTSearch pDPTSearch = PARSER->stack().pop<RDODPTSearch>($2);
		ASSERT(pDPTSearch);
	}
	| dpt_main dpt_some_end
	{
		LPRDODPTSome pDPTSome = PARSER->stack().pop<RDODPTSome>($2);
		ASSERT(pDPTSome);
	}
	| dpt_main dpt_prior_end
	{
		LPRDODPTPrior pDPTPrior = PARSER->stack().pop<RDODPTPrior>($2);
		ASSERT(pDPTPrior);
	}
	| error
	{
		PARSER->error().error(@1, _T("Ожидается описание точки принятия решений"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- DPTSearch
// --------------------------------------------------------------------------------
dpt_search_trace
	: /* empty */
	{
		$$ = rdo::runtime::RDODPTSearchTrace::DPT_no_trace;
	}
	| RDO_no_trace
	{
		$$ = rdo::runtime::RDODPTSearchTrace::DPT_no_trace;
	}
	| RDO_trace
	{
		PARSER->error().error(@1, _T("Данный признак трассировки не используется в точке типа search"));
	}
	| RDO_trace_stat
	{
		$$ = rdo::runtime::RDODPTSearchTrace::DPT_trace_stat;
	}
	| RDO_trace_tops
	{
		$$ = rdo::runtime::RDODPTSearchTrace::DPT_trace_tops;
	}
	| RDO_trace_all
	{
		$$ = rdo::runtime::RDODPTSearchTrace::DPT_trace_all;
	}
	;

dpt_search_parent
	: /* empty */
	{
		$$ = PARSER->stack().push(rdo::Factory<RDOValue>::create());
	}
	| RDO_Parent RDO_IDENTIF
	{
		$$ = $2;
	}
	| RDO_Parent error
	{
		PARSER->error().error(@1, _T("Ошибка в имени родительской точки"));
	}
	;

dpt_search_begin
	: RDO_Decision_point RDO_IDENTIF_COLON RDO_search dpt_search_parent dpt_search_trace
	{
		LPRDOValue pName       = PARSER->stack().pop<RDOValue>($2);
		LPRDOValue pParentName = PARSER->stack().pop<RDOValue>($4);
		ASSERT(pName);

		LPRDODPTSearch pDPTSearch;
		if (pParentName->defined())
		{
			LPRDODPTPrior  pParentDPTPrior  = PARSER->findDPTPrior (pParentName->value().getIdentificator());
			LPRDODPTSearch pParentDPTSearch = PARSER->findDPTSearch(pParentName->value().getIdentificator());
			LPRDODPTSome   pParentDPTSome   = PARSER->findDPTSome  (pParentName->value().getIdentificator());
			if (!pParentDPTPrior && !pParentDPTSearch && !pParentDPTSome)
			{
				PARSER->error().error(@1, rdo::format(_T("Не найдена родитеская точка %s"), pParentName->value().getIdentificator().c_str()));
			}
			if (pParentDPTSearch)
			{
				PARSER->error().error(@1, _T("Точка принятия решений типа search может содержать лишь активности типа rule и не может быть указана в качестве родительской точки"));
			}
			else if (pParentDPTPrior)
			{
				LPILogic pParent = pParentDPTPrior->getLogic();
				ASSERT(pParent);
				pDPTSearch = rdo::Factory<RDODPTSearch>::create(pName->src_info(), *reinterpret_cast<PTR(rdo::runtime::RDODPTSearchTrace::DPT_TraceFlag)>(&$5), pParent);
			}
			else if (pParentDPTSome)
			{
				LPILogic pParent = pParentDPTSome->getLogic();
				ASSERT(pParent);
				pDPTSearch = rdo::Factory<RDODPTSearch>::create(pName->src_info(), *reinterpret_cast<PTR(rdo::runtime::RDODPTSearchTrace::DPT_TraceFlag)>(&$5), pParent);
			}
		}
		else
		{
			pDPTSearch = rdo::Factory<RDODPTSearch>::create(pName->src_info(), *reinterpret_cast<PTR(rdo::runtime::RDODPTSearchTrace::DPT_TraceFlag)>(&$5));
		}
		ASSERT(pDPTSearch);
		$$ = PARSER->stack().push(pDPTSearch);
	}
	| RDO_Decision_point RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@2, @3, _T("Ожидается тип точки"));
	}
	| RDO_Decision_point RDO_IDENTIF error
	{
		PARSER->error().error(@2, _T("Ожидается двоеточие"));
	}
	| RDO_Decision_point error
	{
		PARSER->error().error(@1, @2, _T("После ключевого слова $Decision_point ожидается имя точки"));
	}
	;

dpt_search_condition
	: dpt_search_begin RDO_Condition fun_logic
	{
		LPRDODPTSearch pDPTSearch = PARSER->stack().pop<RDODPTSearch>($1);
		ASSERT(pDPTSearch);
		pDPTSearch->setCondition(PARSER->stack().pop<RDOFUNLogic>($3));
		$$ = PARSER->stack().push(pDPTSearch);
	}
	| dpt_search_begin RDO_Condition RDO_NoCheck
	{
		LPRDODPTSearch pDPTSearch = PARSER->stack().pop<RDODPTSearch>($1);
		ASSERT(pDPTSearch);
		pDPTSearch->setCondition();
		$$ = PARSER->stack().push(pDPTSearch);
	}
	| dpt_search_begin RDO_Condition error
	{
		PARSER->error().error(@2, @3, _T("После ключевого слова $Condition ожидается условие начала поиска (начальная вершина)"));
	}
	| dpt_search_begin error
	{
		PARSER->error().error(@2, _T("Ожидается ключевое слово $Condition"));
	}
	;

dpt_search_prior
	: dpt_search_condition
	| dpt_search_condition RDO_Priority fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm);
		if (!PARSER->getLastDPTSearch()->setPrior(pArithm))
		{
			PARSER->error().error(@3, _T("Точка принятия решений пока не может иметь приоритет"));
		}
	}
	| dpt_search_condition RDO_Priority error
	{
		PARSER->error().error(@1, @2, _T("Ошибка описания приоритета точки принятия решений"))
	}
	| dpt_search_condition error
	{
		PARSER->error().error(@1, @2, _T("Ожидается ключевое слово $Priority"))
	}
	;

dpt_search_term
	: dpt_search_prior RDO_Term_condition fun_logic
	{
		LPRDODPTSearch pDPTSearch = PARSER->stack().pop<RDODPTSearch>($1);
		ASSERT(pDPTSearch);
		pDPTSearch->setTermCondition(PARSER->stack().pop<RDOFUNLogic>($3));
		$$ = PARSER->stack().push(pDPTSearch);
	}
	| dpt_search_prior RDO_Term_condition RDO_NoCheck
	{
		LPRDODPTSearch pDPTSearch = PARSER->stack().pop<RDODPTSearch>($1);
		ASSERT(pDPTSearch);
		pDPTSearch->setTermCondition();
		$$ = PARSER->stack().push(pDPTSearch);
	}
	| dpt_search_prior RDO_Term_condition error
	{
		PARSER->error().error(@2, @3, _T("После ключевого слова $Term_condition ожидается условие остановки поиска (конечная вершина)"));
	}
	| dpt_search_prior error
	{
		PARSER->error().error(@2, _T("Ожидается ключевое слово $Term_condition"));
	}
	;

dpt_search_evaluate
	: dpt_search_term RDO_Evaluate_by fun_arithm
	{
		LPRDODPTSearch pDPTSearch = PARSER->stack().pop<RDODPTSearch>($1);
		ASSERT(pDPTSearch);
		pDPTSearch->setEvaluateBy(PARSER->stack().pop<RDOFUNArithm>($3));
		$$ = PARSER->stack().push(pDPTSearch);
	}
	| dpt_search_term RDO_Evaluate_by error
	{
		PARSER->error().error(@2, @3, _T("После ключевого слова $Evaluate_by ожидается оценочная функция, например, 0 для поиска в ширину"));
	}
	| dpt_search_term error
	{
		PARSER->error().error(@2, _T("Ожидается ключевое слово $Evaluate_by"));
	}
	;

dp_searcht_compare
	: dpt_search_evaluate RDO_Compare_tops '=' RDO_NO
	{
		LPRDODPTSearch pDPTSearch = PARSER->stack().pop<RDODPTSearch>($1);
		ASSERT(pDPTSearch);
		pDPTSearch->setCompareTops(false);
		$$ = PARSER->stack().push(pDPTSearch);
	}
	| dpt_search_evaluate RDO_Compare_tops '=' RDO_YES
	{
		LPRDODPTSearch pDPTSearch = PARSER->stack().pop<RDODPTSearch>($1);
		ASSERT(pDPTSearch);
		pDPTSearch->setCompareTops(true);
		$$ = PARSER->stack().push(pDPTSearch);
	}
	| dpt_search_evaluate RDO_Compare_tops '=' error
	{
		PARSER->error().error(@3, @4, _T("Ожидается режим запоминания пройденных вершин (YES или NO)"));
	}
	| dpt_search_evaluate RDO_Compare_tops error
	{
		PARSER->error().error(@2, @3, _T("Ожидается знак равенства"));
	}
	| dpt_search_evaluate error
	{
		PARSER->error().error(@2, _T("Ожидается ключевое слово $Compare_tops"));
	}
	;

dpt_search_descr_param
	: /* empty */
	| dpt_search_descr_param '*'
	{
		LPRDOValue pValue = rdo::Factory<RDOValue>::create(RDOParserSrcInfo(@2, _T("*")));
		ASSERT(pValue);
		PARSER->getLastDPTSearch()->getLastActivity()->addParam(pValue);
	}
	| dpt_search_descr_param fun_arithm
	{
		LPRDOValue pConstant = PARSER->stack().pop<RDOFUNArithm>($2)->expression()->constant();
		if (!pConstant || !pConstant->defined())
		{
			PARSER->error().error(@2, _T("Параметр может быть только константой"));
		}
		LPRDOValue pValue = rdo::Factory<RDOValue>::create(pConstant);
		ASSERT(pValue);
		PARSER->getLastDPTSearch()->getLastActivity()->addParam(pValue);
	}
	| dpt_search_descr_param error
	{
		PARSER->error().error(@1, @2, _T("Ошибка описания параметра образца"))
	}
	;

dpt_search_descr_value
	: RDO_value_before fun_arithm
	{
		LPRDODPTSearch pDPTSearch = PARSER->getLastDPTSearch();
		ASSERT(pDPTSearch);
		pDPTSearch->getLastActivity()->setValue(IDPTSearchActivity::vt_before, PARSER->stack().pop<RDOFUNArithm>($2));
	}
	| RDO_value_after fun_arithm
	{
		LPRDODPTSearch pDPTSearch = PARSER->getLastDPTSearch();
		ASSERT(pDPTSearch);
		pDPTSearch->getLastActivity()->setValue(IDPTSearchActivity::vt_after, PARSER->stack().pop<RDOFUNArithm>($2));
	}
	| RDO_value_before error
	{
		PARSER->error().error(@1, @2, _T("Ошибка в арифметическом выражении"));
	}
	| RDO_value_after error
	{
		PARSER->error().error(@1, @2, _T("Ошибка в арифметическом выражении"));
	}
	;

dpt_search_name
	: RDO_IDENTIF_COLON RDO_IDENTIF
	{
		LPRDODPTSearch pDPTSearch = PARSER->getLastDPTSearch();
		ASSERT(pDPTSearch);
		LPRDOValue pName    = PARSER->stack().pop<RDOValue>($1);
		LPRDOValue pPattern = PARSER->stack().pop<RDOValue>($2);
		ASSERT(pName   );
		ASSERT(pPattern);
		LPRDODPTActivity pActivity = pDPTSearch->addNewActivity(pName->src_info(), pPattern->src_info());
		ASSERT(pActivity);
		$$ = PARSER->stack().push(pActivity);
	}
	| RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@1, @2, _T("Ожидается имя образца"));
	}
	| RDO_IDENTIF
	{
		PARSER->error().error(@1, _T("Ожидается ':'"));
	}
	| error
	{
		PARSER->error().error(@1, _T("Ожидается имя активности"));
	}
	;

dpt_searcht_activity
	: /* empty */
	| dpt_searcht_activity dpt_search_name dpt_search_descr_param dpt_search_descr_value
	{
		LPRDODPTActivity pActivity = PARSER->stack().pop<RDODPTActivity>($2);
		ASSERT(pActivity)
		pActivity->endParam(@3);
		$$ = PARSER->stack().push(pActivity);
	}
	| dpt_searcht_activity dpt_search_name dpt_search_descr_param error
	{
		PARSER->error().error(@3, @4, _T("Ожидаются ключевые слова value before или value after и стоимость применения правила"));
	}
	;

dpt_search_header
	: dp_searcht_compare RDO_Activities dpt_searcht_activity
	{
		LPRDODPTActivity pActivity = PARSER->stack().pop<RDODPTActivity>($3);
		ASSERT(pActivity);
	}
	| dp_searcht_compare error
	{
		PARSER->error().error(@1, @2, _T("После режима запоминания пройденных вершин ожидается ключевое слово $Activities"));
	}
	;

dpt_search_end
	: dpt_search_header RDO_End
	{
		LPRDODPTSearch pDPTSearch = PARSER->stack().pop<RDODPTSearch>($1);
		ASSERT(pDPTSearch);
		pDPTSearch->end();
		$$ = PARSER->stack().push(pDPTSearch);
	}
	| dpt_search_header
	{
		PARSER->error().error(@1, _T("Ожидается ключевое слово $End"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- DPTSome
// --------------------------------------------------------------------------------
dpt_some_trace
	: /* empty */
	{
		$$ = 1;
	}
	| RDO_no_trace
	{
		$$ = 1;
	}
	| RDO_trace
	{
		$$ = 2;
	}
	| RDO_trace_stat
	{
		PARSER->error().error(@1, _T("Данный признак трассировки не используется в точке типа some"));
	}
	| RDO_trace_tops
	{
		PARSER->error().error(@1, _T("Данный признак трассировки не используется в точке типа some"));
	}
	| RDO_trace_all
	{
		PARSER->error().error(@1, _T("Данный признак трассировки не используется в точке типа some"));
	}
	;

dpt_some_parent
	: /* empty */
	{
		$$ = PARSER->stack().push(rdo::Factory<RDOValue>::create());
	}
	| RDO_Parent RDO_IDENTIF
	{
		$$ = $2;
	}
	| RDO_Parent error
	{
		PARSER->error().error(@1, _T("Ошибка в имени родительской точки"));
	}
	;

dpt_some_begin
	: RDO_Decision_point RDO_IDENTIF_COLON RDO_some dpt_some_trace dpt_some_parent
	{
		LPRDOValue pName       = PARSER->stack().pop<RDOValue>($2);
		LPRDOValue pParentName = PARSER->stack().pop<RDOValue>($5);
		ASSERT(pName);

		LPRDODPTSome pDPTSome;
		if (pParentName->defined())
		{
			LPRDODPTPrior  pParentDPTPrior  = PARSER->findDPTPrior (pParentName->value().getIdentificator());
			LPRDODPTSearch pParentDPTSearch = PARSER->findDPTSearch(pParentName->value().getIdentificator());
			LPRDODPTSome   pParentDPTSome   = PARSER->findDPTSome  (pParentName->value().getIdentificator());
			if (!pParentDPTPrior && !pParentDPTSearch && !pParentDPTSome)
			{
				PARSER->error().error(@1, rdo::format(_T("Не найдена родитеская точка %s"), pParentName->value().getIdentificator().c_str()));
			}
			if (pParentDPTSearch)
			{
				PARSER->error().error(@5, @1, _T("Точка принятия решений типа search может содержать лишь активности типа rule и не может быть указана в качестве родительской точки"));
			}
			else if (pParentDPTPrior)
			{
				LPILogic pParentLogic = pParentDPTPrior->getLogic();
				ASSERT(pParentLogic);
				pDPTSome = rdo::Factory<RDODPTSome>::create(pName->src_info(), pParentLogic);
			}
			else if (pParentDPTSome)
			{
				LPILogic pParentLogic = pParentDPTSome->getLogic();
				ASSERT(pParentLogic);
				pDPTSome = rdo::Factory<RDODPTSome>::create(pName->src_info(), pParentLogic);
			}
		}
		else
		{
			pDPTSome = rdo::Factory<RDODPTSome>::create(pName->src_info());
		}
		ASSERT(pDPTSome);
		$$ = PARSER->stack().push(pDPTSome);
	}
	;

dpt_some_condition
	: dpt_some_begin RDO_Condition fun_logic
	{
		LPRDOLogicBase pLogicBase = PARSER->stack().pop<RDOLogicBase>($1);
		ASSERT(pLogicBase);
		pLogicBase->setCondition(PARSER->stack().pop<RDOFUNLogic>($3));
		$$ = PARSER->stack().push(pLogicBase);
	}
	| dpt_some_begin RDO_Condition RDO_NoCheck
	{
		LPRDOLogicBase pLogicBase = PARSER->stack().pop<RDOLogicBase>($1);
		ASSERT(pLogicBase);
		pLogicBase->setCondition();
		$$ = PARSER->stack().push(pLogicBase);
	}
	| dpt_some_begin RDO_Condition error
	{
		PARSER->error().error(@2, @3, _T("После ключевого слова $Condition ожидается условие запуска точки"));
	}
	| dpt_some_begin
	{
		LPRDOLogicBase pLogicBase = PARSER->stack().pop<RDOLogicBase>($1);
		ASSERT(pLogicBase);
		pLogicBase->setCondition();
		$$ = PARSER->stack().push(pLogicBase);
	}
	;

dpt_some_multi
	: dpt_some_condition RDO_Multithreading '=' RDO_NO
	{
		LPRDOLogicBase pLogicBase = PARSER->stack().pop<RDOLogicBase>($1);
		ASSERT(pLogicBase);
		pLogicBase->setMultithreading(false);
		$$ = PARSER->stack().push(pLogicBase);
	}
	| dpt_some_condition RDO_Multithreading '=' RDO_YES
	{
		LPRDOLogicBase pLogicBase = PARSER->stack().pop<RDOLogicBase>($1);
		ASSERT(pLogicBase);
		pLogicBase->setMultithreading(true);
		$$ = PARSER->stack().push(pLogicBase);
	}
	| dpt_some_condition RDO_Multithreading error
	{
		PARSER->error().error(@2, @3, _T("После ключевого слова $Multithreading ожидается знак равенства и слово YES или NO"));
	}
	| dpt_some_condition
	{
		LPRDOLogicBase pLogicBase = PARSER->stack().pop<RDOLogicBase>($1);
		ASSERT(pLogicBase);
		pLogicBase->setMultithreading(false);
		$$ = PARSER->stack().push(pLogicBase);
	}
	;

dpt_some_prior
	: dpt_some_multi
	| dpt_some_multi RDO_Priority fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm);
		if (!PARSER->getLastDPTSome()->setPrior(pArithm))
		{
			PARSER->error().error(@3, _T("Точка принятия решений пока не может иметь приоритет"));
		}
	}
	| dpt_some_multi RDO_Priority error
	{
		PARSER->error().error(@1, @2, _T("Ошибка описания приоритета точки принятия решений"))
	}
	| dpt_some_multi error
	{
		PARSER->error().error(@1, @2, _T("После слова $Condition ожидается ключевое слово $Multithreading"))
	}
	;

dpt_some_name
	: RDO_IDENTIF_COLON RDO_IDENTIF
	{
		LPRDODPTSome pDPTSome = PARSER->getLastDPTSome();
		ASSERT(pDPTSome);
		LPRDOValue pName    = PARSER->stack().pop<RDOValue>($1);
		LPRDOValue pPattern = PARSER->stack().pop<RDOValue>($2);
		ASSERT(pName   );
		ASSERT(pPattern);
		LPRDODPTActivity pActivity = pDPTSome->addNewActivity(pName->src_info(), pPattern->src_info());
		ASSERT(pActivity);
		$$ = PARSER->stack().push(pActivity);
	}
	| RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@1, @2, _T("Ожидается имя образца"));
	}
	| RDO_IDENTIF
	{
		PARSER->error().error(@1, _T("Ожидается ':'"));
	}
	| error
	{
		PARSER->error().error(@1, _T("Ожидается имя активности"));
	}
	;

dpt_some_descr_keyb
	: /* empty */
	| dpt_some_descr_keyb '+' RDO_STRING_CONST
	{
		LPRDODPTActivityHotKey pActivityHotKey = PARSER->getLastDPTSome()->getLastActivity();
		ASSERT(pActivityHotKey);
		tstring key = PARSER->stack().pop<RDOValue>($3)->value().getString();
		pActivityHotKey->addHotKey(key, @3);
	}
	| RDO_STRING_CONST
	{
		LPRDODPTActivityHotKey pActivityHotKey = PARSER->getLastDPTSome()->getLastActivity();
		ASSERT(pActivityHotKey);
		tstring key = PARSER->stack().pop<RDOValue>($1)->value().getString();
		pActivityHotKey->addHotKey(key, @1);
	}
	;

dpt_some_descr_param
	: /* empty */
	| dpt_some_descr_param '*'
	{
		LPRDOValue pValue = rdo::Factory<RDOValue>::create(RDOParserSrcInfo(@2, _T("*")));
		ASSERT(pValue);
		PARSER->getLastDPTSome()->getLastActivity()->addParam(pValue);
	}
	| dpt_some_descr_param fun_arithm
	{
		LPRDOValue pConstant = PARSER->stack().pop<RDOFUNArithm>($2)->expression()->constant();
		if (!pConstant || !pConstant->defined())
		{
			PARSER->error().error(@2, _T("Параметр может быть только константой"));
		}
		LPRDOValue pValue = rdo::Factory<RDOValue>::create(pConstant);
		ASSERT(pValue);
		PARSER->getLastDPTSome()->getLastActivity()->addParam(pValue);
	}
	| dpt_some_descr_param error
	{
		PARSER->error().error(@1, @2, _T("Ошибка описания параметра образца"))
	}
	;

dpt_some_activity
	: /* empty */
	{
		LPRDODPTActivity pActivity = NULL;
		$$ = PARSER->stack().push(pActivity);
	}
	| dpt_some_activity dpt_some_name dpt_some_descr_keyb dpt_some_descr_param
	{
		LPRDODPTActivity pActivity = PARSER->stack().pop<RDODPTSomeActivity>($2);
		ASSERT(pActivity);
		pActivity->endParam(@3);
		$$ = PARSER->stack().push(pActivity);
	}
	;

dpt_some_header
	: dpt_some_prior RDO_Activities dpt_some_activity
	{
		LPRDODPTActivity pActivity = PARSER->stack().pop<RDODPTActivity>($3);
	}
	| dpt_some_prior error
	{
		PARSER->error().error(@1, @2, _T("Ожидается ключевое слово $Activities"));
	}
	;

dpt_some_end
	: dpt_some_header RDO_End
	{
		LPRDOLogicBase pLogicBase = PARSER->stack().pop<RDOLogicBase>($1);
		ASSERT(pLogicBase);
		pLogicBase->end();
		$$ = PARSER->stack().push(pLogicBase);
	}
	| dpt_some_header
	{
		PARSER->error().error(@1, _T("Ожидается ключевое слово $End"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- DPT Prior
// --------------------------------------------------------------------------------
dpt_prior_trace
	: /* empty */
	{
		$$ = 1;
	}
	| RDO_no_trace
	{
		$$ = 1;
	}
	| RDO_trace
	{
		$$ = 2;
	}
	| RDO_trace_stat
	{
		PARSER->error().error(@1, _T("Данный признак трассировки не используется в точке типа prior"));
	}
	| RDO_trace_tops
	{
		PARSER->error().error(@1, _T("Данный признак трассировки не используется в точке типа prior"));
	}
	| RDO_trace_all
	{
		PARSER->error().error(@1, _T("Данный признак трассировки не используется в точке типа prior"));
	}
	;

dpt_prior_parent
	: /* empty */
	{
		$$ = PARSER->stack().push(rdo::Factory<RDOValue>::create());
	}
	| RDO_Parent RDO_IDENTIF
	{
		$$ = $2;
	}
	| RDO_Parent error
	{
		PARSER->error().error(@1, _T("Ошибка в имени родительской точки"));
	}
	;

dpt_prior_begin
	: RDO_Decision_point RDO_IDENTIF_COLON RDO_prior dpt_prior_parent dpt_prior_trace
	{
		//! @todo а где признак трассировки для prior ?
		LPRDOValue pName       = PARSER->stack().pop<RDOValue>($2);
		LPRDOValue pParentName = PARSER->stack().pop<RDOValue>($4);
		ASSERT(pName);

		LPRDODPTPrior pDPTPrior;
		if (pParentName->defined())
		{
			LPRDODPTPrior  pParentDPTPrior  = PARSER->findDPTPrior (pParentName->value().getIdentificator());
			LPRDODPTSearch pParentDPTSearch = PARSER->findDPTSearch(pParentName->value().getIdentificator());
			LPRDODPTSome   pParentDPTSome   = PARSER->findDPTSome  (pParentName->value().getIdentificator());
			if (!pParentDPTPrior && !pParentDPTSome && !pParentDPTSearch)
			{
				PARSER->error().error(@1, rdo::format(_T("Не найдена родитеская точка %s"), pParentName->value().getIdentificator().c_str()));
			}
			if (pParentDPTSearch)
			{
				PARSER->error().error(@5, @1, _T("Точка принятия решений типа search может содержать лишь активности типа rule и не может быть указана в качестве родительской точки"));
			}
			else if (pParentDPTPrior)
			{
				LPILogic pParentLogic = pParentDPTPrior->getLogic();
				ASSERT(pParentLogic);
				pDPTPrior = rdo::Factory<RDODPTPrior>::create(pName->src_info(), pParentLogic);
			}
			else if (pParentDPTSome)
			{
				LPILogic pParentLogic = pParentDPTSome->getLogic();
				ASSERT(pParentLogic);
				pDPTPrior = rdo::Factory<RDODPTPrior>::create(pName->src_info(), pParentLogic);
			}
		}
		else
		{
			pDPTPrior = rdo::Factory<RDODPTPrior>::create(pName->src_info());
		}
		ASSERT(pDPTPrior);
		$$ = PARSER->stack().push(pDPTPrior);
	}
	;

dpt_prior_condition
	: dpt_prior_begin RDO_Condition fun_logic
	{
		LPRDODPTPrior pDPTPrior = PARSER->stack().pop<RDODPTPrior>($1);
		ASSERT(pDPTPrior);
		pDPTPrior->setCondition(PARSER->stack().pop<RDOFUNLogic>($3));
		$$ = PARSER->stack().push(pDPTPrior);
	}
	| dpt_prior_begin RDO_Condition RDO_NoCheck
	{
		LPRDODPTPrior pDPTPrior = PARSER->stack().pop<RDODPTPrior>($1);
		ASSERT(pDPTPrior);
		pDPTPrior->setCondition();
		$$ = PARSER->stack().push(pDPTPrior);
	}
	| dpt_prior_begin RDO_Condition error
	{
		PARSER->error().error(@2, @3, _T("После ключевого слова $Condition ожидается условие запуска точки"));
	}
	| dpt_prior_begin
	{
		LPRDODPTPrior pDPTPrior = PARSER->stack().pop<RDODPTPrior>($1);
		ASSERT(pDPTPrior);
		pDPTPrior->setCondition();
		$$ = PARSER->stack().push(pDPTPrior);
	}
	;

dpt_prior_prior
	: dpt_prior_condition
	| dpt_prior_condition RDO_Priority fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm);
		if (!PARSER->getLastDPTPrior()->setPrior(pArithm))
		{
			PARSER->error().error(@3, _T("Точка принятия решений пока не может иметь приоритет"));
		}
	}
	| dpt_prior_condition RDO_Priority error
	{
		PARSER->error().error(@1, @2, _T("Ошибка описания приоритета точки принятия решений"))
	}
	| dpt_some_condition error
	{
		PARSER->error().error(@1, @2, _T("Ожидается ключевое слово $Priority"))
	}
	;

dpt_prior_name
	: RDO_IDENTIF_COLON RDO_IDENTIF
	{
		LPRDODPTPrior pDPTPrior = PARSER->getLastDPTPrior();
		ASSERT(pDPTPrior);
		LPRDOValue pName    = PARSER->stack().pop<RDOValue>($1);
		LPRDOValue pPattern = PARSER->stack().pop<RDOValue>($2);
		ASSERT(pName   );
		ASSERT(pPattern);
		LPRDODPTActivity pActivity = pDPTPrior->addNewActivity(pName->src_info(), pPattern->src_info());
		ASSERT(pActivity);
		$$ = PARSER->stack().push(pActivity);
	}
	| RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@1, @2, _T("Ожидается имя образца"));
	}
	| RDO_IDENTIF
	{
		PARSER->error().error(@1, _T("Ожидается ':'"));
	}
	| error
	{
		PARSER->error().error(@1, _T("Ожидается имя активности"));
	}
	;

dpt_prior_descr_keyb
	: /* empty */
	| dpt_prior_descr_keyb '+' RDO_STRING_CONST
	{
		LPRDODPTActivityHotKey pActivityHotKey = PARSER->getLastDPTPrior()->getLastActivity();
		ASSERT(pActivityHotKey);
		tstring key = PARSER->stack().pop<RDOValue>($3)->value().getString();
		pActivityHotKey->addHotKey(key, @3);
	}
	| RDO_STRING_CONST
	{
		LPRDODPTActivityHotKey pActivityHotKey = PARSER->getLastDPTPrior()->getLastActivity();
		ASSERT(pActivityHotKey);
		tstring key = PARSER->stack().pop<RDOValue>($1)->value().getString();
		pActivityHotKey->addHotKey(key, @1);
	}
	;

dpt_prior_descr_param
	: /* empty */
	| dpt_prior_descr_param '*'
	{
		LPRDOValue pValue = rdo::Factory<RDOValue>::create(RDOParserSrcInfo(@2, _T("*")));
		ASSERT(pValue);
		PARSER->getLastDPTPrior()->getLastActivity()->addParam(pValue);
	}
	| dpt_prior_descr_param fun_arithm
	{
		LPRDOValue pConstant = PARSER->stack().pop<RDOFUNArithm>($2)->expression()->constant();
		if (!pConstant || !pConstant->defined())
		{
			PARSER->error().error(@2, _T("Параметр может быть только константой"));
		}
		LPRDOValue pValue = rdo::Factory<RDOValue>::create(pConstant);
		ASSERT(pValue);
		PARSER->getLastDPTPrior()->getLastActivity()->addParam(pValue);
	}
	| dpt_prior_descr_param error
	{
		PARSER->error().error(@1, @2, _T("Ошибка описания параметра образца"))
	}
	;

dpt_prior_activ_prior
	: /* empty */
	| RDO_CF '=' fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm);
		if (!PARSER->getLastDPTPrior()->getLastActivity()->setPrior(pArithm))
		{
			PARSER->error().error(@3, _T("Активность не может иметь приоритет"));
		}
	}
	| RDO_CF '=' error
	{
		PARSER->error().error(@1, @2, _T("Ошибка описания приоритета активности"))
	}
	| RDO_CF error
	{
		PARSER->error().error(@1, @2, _T("Ошибка: ожидается знак равенства"))
	}
	;

dpt_prior_activity
	: /* empty */
	| dpt_prior_activity dpt_prior_name dpt_prior_descr_keyb dpt_prior_descr_param dpt_prior_activ_prior
	{
		LPRDODPTActivity pActivity = PARSER->stack().pop<RDODPTActivity>($2);
		ASSERT(pActivity);
		pActivity->endParam(@3);
		$$ = PARSER->stack().push(pActivity);
	}
	;

dpt_prior_header
	: dpt_prior_prior RDO_Activities dpt_prior_activity
	{
		LPRDODPTActivity pActivity = PARSER->stack().pop<RDODPTActivity>($3);
		ASSERT(pActivity);
	}
	| dpt_prior_prior error
	{
		PARSER->error().error(@1, @2, _T("Ожидается ключевое слово $Activities"));
	}
	;

dpt_prior_end
	: dpt_prior_header RDO_End
	{
		LPRDODPTPrior pDPTPrior = PARSER->stack().pop<RDODPTPrior>($1);
		ASSERT(pDPTPrior);
		pDPTPrior->end();
		$$ = PARSER->stack().push(pDPTPrior);
	}
	| dpt_prior_header
	{
		PARSER->error().error(@1, _T("Ожидается ключевое слово $End"));
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

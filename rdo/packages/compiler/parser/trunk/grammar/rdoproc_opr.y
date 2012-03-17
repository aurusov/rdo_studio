/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoproc_opr.y
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      24.03.2008
  \brief     Синтаксис процессов. based on rdodpt_opr.y
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
#include "utils/rdotypes.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/rdodpt.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdorss.h"
#include "simulator/runtime/calc/calc_array.h"
#include "simulator/runtime/calc/calc_process.h"
#include "simulator/runtime/calc/function/calc_function_system.h"
#include "simulator/runtime/rdo_res_type_i.h"
#include "simulator/compiler/mbuilder/rdo_resources.h"
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

%start prc_main

%%

// --------------------------------------------------------------------------------
// -------------------- General part
// --------------------------------------------------------------------------------
prc_main
	: /* empty */
	| prc_main dpt_process_end
	;

// --------------------------------------------------------------------------------
// -------------------- Process
// --------------------------------------------------------------------------------
dpt_process
	: dpt_process_header dpt_process_input
	;

dpt_process_header
	: dpt_process_begin dpt_process_condition dpt_process_prior
	;

dpt_process_begin
	: RDO_Process RDO_IDENTIF RDO_IDENTIF
	{
		LPRDOPROCProcess pProcess = PARSER->getLastPROCProcess();
		if (pProcess && !pProcess->closed())
		{
			PARSER->error().error(pProcess->src_info(), _T("Незакрыт предыдущий блок $Process"));
		}

		tstring transactName = PARSER->stack().pop<RDOValue>($3)->value().getIdentificator();
		LPRDORTPResType transactType = PARSER->findRTPResType(transactName);

		tstring procName = PARSER->stack().pop<RDOValue>($2)->value().getIdentificator();
		pProcess = rdo::Factory<RDOPROCProcess>::create(RDOParserSrcInfo(@1, @3, procName), procName, transactType);
		ASSERT(pProcess);
	}
	| RDO_Process error
	{
		PARSER->error().error(@2, @2, _T("Ошибка в процессе!"));
	}
	;

dpt_process_condition
	: /* empty */
	{
		LPRDOPROCProcess pProcess = PARSER->getLastPROCProcess();
		ASSERT(pProcess);
		pProcess->setCondition();
	}
	| RDO_Condition fun_logic
	{
		LPRDOPROCProcess pProcess = PARSER->getLastPROCProcess();
		ASSERT(pProcess);
		pProcess->setCondition(PARSER->stack().pop<RDOFUNLogic>($2));
	}
	| RDO_Condition RDO_NoCheck
	{
		LPRDOPROCProcess pProcess = PARSER->getLastPROCProcess();
		ASSERT(pProcess);
		pProcess->setCondition();
	}
	| RDO_Condition error
	{
		PARSER->error().error(@2, @2, _T("После ключевого слова $Condition ожидается условие активации процесса"));
	}
	| error
	{
		PARSER->error().error(@1, _T("Ожидается ключевое слово $Condition"));
	}
	;

dpt_process_prior
	: /* empty */
	| RDO_Priority fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		if (!PARSER->getLastPROCProcess()->setPrior(pArithm))
		{
			PARSER->error().error(@2, _T("Процесс не может иметь приоритет"));
		}
	}
	| RDO_Priority error
	{
		PARSER->error().error(@1, @2, _T("Ошибка описания приоритета процесса"))
	}
	| error
	{
		PARSER->error().error(@1, @1, _T("Ожидается ключевое слово $Priority"))
	}
	;

dpt_process_input
	: /* empty */
	| dpt_process_input dpt_process_line
	;

dpt_process_line
	: RDO_IDENTIF
	{
		PARSER->error().error(@1, rdo::format(_T("Неизвестный оператор '%s'"), PARSER->stack().pop<RDOValue>($1)->value().getIdentificator().c_str()));
	}
	| planning_statement
	{
		rdoRuntime::LPRDOCalc pCalc = PARSER->stack().pop<rdoRuntime::RDOCalc>($1);
		ASSERT(pCalc);
		LPRDOPROCOperator pBlock = rdo::Factory<RDOPROCAssign>::create(
			PARSER->getLastPROCProcess(),
			_T("Event planning from process"),
			pCalc
		);
		ASSERT(pBlock);
		$$ = PARSER->stack().push(pBlock);
	}
	| RDO_GENERATE fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);

		rdoRuntime::LPRDOCalc pCalcTime = pArithm->createCalc();
		ASSERT(pCalcTime);

		LPRDOPROCProcess pProc = PARSER->getLastPROCProcess();
		ASSERT(pProc);

		LPRDORTPResType pParserType = pProc->getTransacType();
		ASSERT(pParserType);
		
		rdoRuntime::LPIResourceType pType = pParserType->getRuntimeResType();
		ASSERT(pType);

		rbool permanentFlag = pParserType->isPermanent();
		rbool traceFlag     = true;

		std::vector<rdoRuntime::RDOValue> paramList;
		//! \error вместо настоящих значений параметров транзакта просто 0
		paramList.push_back(rdoRuntime::RDOValue(0.0));

		rdoRuntime::LPRDOCalcCreateResource pCreateAndGoOnTransactCalc = rdo::Factory<rdoRuntime::RDOCalcCreateResource>::create(
			pType,
			paramList,
			traceFlag,
			permanentFlag
		);
		ASSERT(pCreateAndGoOnTransactCalc);

		LPRDOPROCOperator pBlock = rdo::Factory<RDOPROCGenerate>::create(
			pProc,
			_T("GENERATE"),
			pCalcTime,
			pCreateAndGoOnTransactCalc
		);
		ASSERT(pBlock);

		$$ = PARSER->stack().push(pBlock);
	}
	| RDO_GENERATE error
	{
		PARSER->error().error(@2, _T("Ошибка в арифметическом выражении"));
	}
	| RDO_TERMINATE dpt_term_param
	{
		LPRDOPROCOperator pBlock = PARSER->stack().pop<RDOPROCOperator>($2);
		ASSERT(pBlock);
		$$ = PARSER->stack().push(pBlock);
	}
	| RDO_TERMINATE error
	{
		PARSER->error().error(@1, _T("Ошибка в параметре оператора TERMINATE"));
	}
	| RDO_ADVANCE fun_arithm
	{
		LPRDOPROCOperator pBlock = rdo::Factory<RDOPROCAdvance>::create(PARSER->getLastPROCProcess(), _T("ADVANCE"), PARSER->stack().pop<RDOFUNArithm>($2)->createCalc());
		ASSERT(pBlock);
		$$ = PARSER->stack().push(pBlock);
	}
	| RDO_ADVANCE error
	{
		PARSER->error().error(@2, _T("Ошибка в арифметическом выражении"));
	}
	| RDO_QUEUE dpt_queue_param
	{
		TRACE(_T("QUEUE dpt_queue_param\n"));
		LPRDOPROCQueue pQueue = PARSER->stack().pop<RDOPROCQueue>($2);
		ASSERT(pQueue);
		pQueue->createRuntime();
		$$ = PARSER->stack().push(pQueue);
	}
	| RDO_QUEUE error
	{
		PARSER->error().error(@1, _T("Ожидается имя ресурса для сбора статистики по очереди"));
	}
	| RDO_DEPART dpt_depart_param
	{
		TRACE(_T("DEPART dpt_depart_param\n"));
		LPRDOPROCDepart pDepart = PARSER->stack().pop<RDOPROCDepart>($2);
		ASSERT(pDepart);
		pDepart->createRuntime();
		$$ = PARSER->stack().push(pDepart);
	}
	| RDO_DEPART error
	{
		PARSER->error().error(@1, _T("Ожидается имя ресурса для сбора статистики по очереди"));
	}
	| RDO_SEIZE dpt_seize_param
	{
		TRACE(_T("SEIZE dpt_seize_param\n"));
		LPRDOPROCSeize pSeize = PARSER->stack().pop<RDOPROCSeize>($2);
		ASSERT(pSeize);
		pSeize->createRuntime();
		$$ = PARSER->stack().push(pSeize);
	}
	| RDO_SEIZE error
	{
		PARSER->error().error(@1, rdo::format(_T("Ожидается список ресурсов, объединяемых в блок, через запятую")));
	}
	| RDO_RELEASE dpt_release_param
	{
		TRACE(_T("RELEASE dpt_release_param\n"));
		LPRDOPROCRelease pRelease = PARSER->stack().pop<RDOPROCRelease>($2);
		ASSERT(pRelease);
		pRelease->createRuntime();
		$$ = PARSER->stack().push(pRelease);
	}
	| RDO_RELEASE error
	{
		PARSER->error().error(@1, rdo::format(_T("Ожидается список ресурсов, объединяемых в блок, через запятую")));
	}
	| RDO_ASSIGN dpt_assign_param
	{
	}
	| RDO_ASSIGN error
	{
		PARSER->error().error(@1, rdo::format(_T("Ожидается строка изменения параметра")));
	}
	;

planning_statement
	: RDO_IDENTIF '.' RDO_Planning '(' arithm_list ')'';'
	{
		tstring           eventName   = PARSER->stack().pop<RDOValue>($1)->value().getIdentificator();
		LPArithmContainer pArithmList = PARSER->stack().pop<ArithmContainer>($5);

		LPRDOEvent pEvent = PARSER->findEvent(eventName);
		if (!pEvent)
		{
			PARSER->error().error(@1, rdo::format(_T("Попытка запланировать неизвестное событие: %s"), eventName.c_str()));
		}

		ArithmContainer::Container::const_iterator arithmIt = pArithmList->getContainer().begin();
		if (arithmIt == pArithmList->getContainer().end())
		{
			PARSER->error().error(@1, rdo::format(_T("Не указано время планирования события: %s"), eventName.c_str()));
		}

		LPRDOFUNArithm pTimeArithm = *arithmIt;
		ASSERT(pTimeArithm);
		++arithmIt;

		LPArithmContainer pParamList = rdo::Factory<ArithmContainer>::create();
		ASSERT(pParamList);

		while (arithmIt != pArithmList->getContainer().end())
		{
			pParamList->addItem(*arithmIt);
			++arithmIt;
		}

		pEvent->setParamList(pParamList);

		rdoRuntime::LPRDOCalc pCalcTime = pTimeArithm->createCalc(NULL);
		ASSERT(pCalcTime);

		rdoRuntime::LPRDOCalcEventPlan pCalc = rdo::Factory<rdoRuntime::RDOCalcEventPlan>::create(pCalcTime);
		ASSERT(pCalc);
		pEvent->attachCalc(pCalc);

		$$ = PARSER->stack().push(pCalc);
	}
	| RDO_IDENTIF '.' RDO_Planning '(' arithm_list ')' error
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
	| RDO_IDENTIF '.' RDO_Planning '(' arithm_list error
	{
		PARSER->error().error(@6, _T("Ожидается закрывающая скобка"));
	}
	;

stopping_statement
	: RDO_IDENTIF '.' RDO_Stopping '(' ')' ';'
	{
		tstring           eventName   = PARSER->stack().pop<RDOValue>($1)->value().getIdentificator();
		LPRDOEvent        pEvent      = PARSER->findEvent(eventName);
		if (!pEvent)
		{
			PARSER->error().error(@1, rdo::format(_T("Попытка остановить неизвестное событие: %s"), eventName.c_str()));
		}

		rdoRuntime::LPRDOCalcEventStop pCalc = rdo::Factory<rdoRuntime::RDOCalcEventStop>::create();
		ASSERT(pCalc);
		pEvent->attachCalc(pCalc);

		$$ = PARSER->stack().push(pCalc);
	}
	| RDO_IDENTIF '.' RDO_Stopping '(' ')' error
	{
		PARSER->error().error(@4, _T("Не найден символ окончания инструкции - точка с запятой"));
	}
	;

dpt_queue_param
	: RDO_IDENTIF
	{
		tstring res_name = PARSER->stack().pop<RDOValue>($1)->value().getIdentificator();
		TRACE1(_T("%s _good\n"), res_name.c_str());
		LPRDOPROCQueue pQueue = rdo::Factory<RDOPROCQueue>::create(PARSER->getLastPROCProcess(), _T("QUEUE"));
		ASSERT(pQueue);
		pQueue->setResource(res_name);
		$$ = PARSER->stack().push(pQueue);
	}
	| RDO_IDENTIF error
	{
		PARSER->error().error(@1, _T("Ошибка в миени очереди"))
	}
	;

dpt_depart_param
	: RDO_IDENTIF
	{
		tstring res_name = PARSER->stack().pop<RDOValue>($1)->value().getIdentificator();
		TRACE1(_T("%s _good\n"), res_name.c_str());
		LPRDOPROCDepart pDepart = rdo::Factory<RDOPROCDepart>::create(PARSER->getLastPROCProcess(), _T("DEPART"));
		ASSERT(pDepart);
		pDepart->setResource(res_name);
		$$ = PARSER->stack().push(pDepart);
	}
	| RDO_IDENTIF error 
	{
		PARSER->error().error(@1, _T("Ошибка в имени ресурса"))
	}
	;

dpt_term_param
	: /* empty */
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(rdoRuntime::RDOValue(0));
		LPRDOPROCOperator pBlock = rdo::Factory<RDOPROCTerminate>::create(PARSER->getLastPROCProcess(), _T("TERMINATE"), pCalc);
		ASSERT(pBlock);
		$$ = PARSER->stack().push(pBlock);
	}
	| fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($1);
		ASSERT(pArithm);
		if (pArithm->createCalc()->calcValue(RUNTIME).type()->typeID() == rdoRuntime::RDOType::t_int)
		{
			rdoRuntime::LPRDOCalc pCalc = pArithm->createCalc(NULL);
			LPRDOPROCOperator pBlock = rdo::Factory<RDOPROCTerminate>::create(PARSER->getLastPROCProcess(), _T("TERMINATE"), pCalc);
			ASSERT(pBlock);
			$$ = PARSER->stack().push(pBlock);
		}
		else
		{
			PARSER->error().error(@1, _T("Ошибка, для оператора TERMINATE можно использовать только арифметические выражения целого типа"));
		}
	}
	| fun_arithm error
	{
		PARSER->error().error(@1, _T("Ошибка, после оператора TERMINATE может быть указано только арифметическое выражение целого типа"))
	}
	;

dpt_seize_param
	: RDO_IDENTIF
	{
		tstring        res_name = PARSER->stack().pop<RDOValue>($1)->value().getIdentificator().c_str();
		LPRDOPROCSeize pSeize   = rdo::Factory<RDOPROCSeize>::create(PARSER->getLastPROCProcess(), _T("SEIZE"));
		ASSERT(pSeize);
		pSeize->addResource(res_name);
		$$ = PARSER->stack().push(pSeize);
	}
	| dpt_seize_param ',' RDO_IDENTIF
	{
		LPRDOPROCSeize pSeize = PARSER->stack().pop<RDOPROCSeize>($1);
		ASSERT(pSeize);
		tstring res_name = PARSER->stack().pop<RDOValue>($3)->value().getIdentificator().c_str();
		pSeize->addResource(res_name);
		$$ = PARSER->stack().push(pSeize);
	}
	| dpt_seize_param error
	{
		PARSER->error().error(@1, _T("Ошибка в имени ресурса"));
	}
	;

dpt_release_param
	: RDO_IDENTIF
	{
		tstring          res_name = PARSER->stack().pop<RDOValue>($1)->value().getIdentificator().c_str();
		LPRDOPROCRelease pRelease = rdo::Factory<RDOPROCRelease>::create(PARSER->getLastPROCProcess(), _T("RELEASE"));
		ASSERT(pRelease);
		pRelease->addResource(res_name);
		$$ = PARSER->stack().push(pRelease);
	}
	| dpt_release_param ',' RDO_IDENTIF
	{
		LPRDOPROCRelease pRelease = PARSER->stack().pop<RDOPROCRelease>($1);
		ASSERT(pRelease);
		tstring res_name = PARSER->stack().pop<RDOValue>($3)->value().getIdentificator().c_str();
		pRelease->addResource(res_name);
		$$ = PARSER->stack().push(pRelease);
	}
	| dpt_release_param error
	{
		PARSER->error().error(@1, _T("Ошибка в имени ресурса"));
	}
	;

dpt_assign_param
	: RDO_IDENTIF '.' RDO_IDENTIF '=' fun_arithm
	{
		tstring res   = PARSER->stack().pop<RDOValue>($1)->value().getIdentificator();
		tstring param = PARSER->stack().pop<RDOValue>($3)->value().getIdentificator();
		rdoMBuilder::RDOResType rtp;

		// Получили список всех ресурсов
		rdoMBuilder::RDOResourceList rssList(PARSER);

		// Если ресурс существует, берем его тип и проверяем наличие параметра
		if (rssList[res].exist())
		{
			rtp = rssList[res].getType();
			if(!rtp.m_params[param].exist())
			{
				PARSER->error().error(@1, rdo::format(_T("Ссылка на неизвестный параметр ресурса: %s.%s"), res.c_str(), param.c_str()));
			}
		
			LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($5);
			ASSERT(pArithm);

			LPRDORSSResource pResource = PARSER->findRSSResource(res);
			ASSERT(pResource);

			LPRDORTPResType pResType = pResource->getType();
			LPRDORTPParam   pParam   = pResType->findRTPParam(param);
			pArithm->checkParamType(pParam->getTypeInfo());

			ruint res = pResource->getID();
			ruint par = rtp.m_params[param].id();

			rdoRuntime::LPRDOCalc pCalc = pArithm->createCalc(pParam->getTypeInfo());
			ASSERT(pCalc);

			rdoRuntime::LPRDOCalcProcAssign pAssignCalc = rdo::Factory<rdoRuntime::RDOCalcProcAssign>::create(pCalc, res, par);
			ASSERT(pAssignCalc);

			LPRDOPROCOperator pBlock = rdo::Factory<RDOPROCAssign>::create(PARSER->getLastPROCProcess(), _T("ASSIGN"), pAssignCalc);
			ASSERT(pBlock);
			$$ = PARSER->stack().push(pBlock);
		}
		else
		{
			PARSER->error().error(@1, rdo::format(_T("Ссылка на неизвестный ресурс: %s"), res.c_str()));
		}
	}
	;

dpt_process_end
	: dpt_process RDO_End
	{
		PARSER->getLastPROCProcess()->end();
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

		rdoRuntime::LPRDOCalc pCalc;
		if (pArithm->typeInfo()->type().object_dynamic_cast<RDOArrayType>())
		{
			pCalc = rdo::Factory<rdoRuntime::RDOCalcArraySize>::create(pArithm->calc());
			ASSERT(pCalc);
		}
		else
		{
			PARSER->error().error(@1, rdo::format(_T("'%s' не является массивом."), pValue->value().getIdentificator().c_str()));
		}

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(rdo::Factory<RDOType__int>::create(), RDOParserSrcInfo(@1));
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

		if (pArithm->typeInfo()->type().object_dynamic_cast<RDOArrayType>())
		{
			PARSER->error().error(@1, rdo::format(_T("'%s' не является массивом."), pValue->value().getIdentificator().c_str()));
		}

		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcArrayItem>::create(pArithm->calc(), pArithmInd->calc());
		ASSERT(pCalc);

		LPRDOType pType = pArithm->typeInfo()->type();
		ASSERT(pType);
		LPRDOArrayType pArrayType = pType.object_dynamic_cast<RDOArrayType>();
		ASSERT(pArrayType);

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

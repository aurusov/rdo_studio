/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoproc_rtp.y
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      24.03.2008
  \brief     Сбор типов транзактов процесса
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
#include "simulator/runtime/process/rdoprocess.h"

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

%start prc_rtp_main

%%

// --------------------------------------------------------------------------------
// -------------------- General part
// --------------------------------------------------------------------------------
prc_rtp_main
	: /* empty */
	| prc_rtp_main RDO_Process RDO_IDENTIF RDO_IDENTIF error RDO_End
	{
		tstring rtp_name       = PARSER->stack().pop<RDOValue>($4)->value().getIdentificator();
		tstring rtp_param_name = _T("Время_создания");

		// Получили список всех типов ресурсов
		rdoMBuilder::RDOResTypeList rtpList(PARSER);
		// Найти тип ресурса, если его нет, то создать
		if (!rtpList[rtp_name].exist())
		{
			// Создадим тип ресурса-транзакта
			rdoMBuilder::RDOResType rtp(rtp_name, rdoMBuilder::RDOResType::rt_temporary);
			// Добавим параметр Время_создания
			rdoMBuilder::RDOResType::Param param(rtp_param_name, rdo::Factory<RDOType__real>::create());
			rtp.m_params.append(param);
			// Добавим тип ресурса
			if (!rtpList.append(rtp))
			{
				PARSER->error().error(@2, rdo::format(_T("Ошибка создания типа ресурса: %s"), rtp_name.c_str()));
			}
			LPRDORTPResType pResType = PARSER->findRTPResType(rtp_name);
			ASSERT(pResType);
			pResType->setType(RDORTPResType::procTran);
		}
		else
		{
			// Тип найден, проверим его на наличие вещественного параметра
			CREF(rdoMBuilder::RDOResType) rtp = rtpList[rtp_name];
			if (!rtp.m_params[rtp_param_name].exist())
			{
				PARSER->error().error(rtp.src_info(), rdo::format(_T("У типа ресурса '%s' нет требуемого параметра '%s'"), rtp.name().c_str(), rtp_param_name.c_str()));
			}
			// Параметр есть, надо проверить на тип
			if (rtp.m_params[rtp_param_name].typeID() != rdoRuntime::RDOType::t_real)
			{
				PARSER->error().error(rtp.src_info(), rdo::format(_T("У типа ресурса '%s' параметр '%s' не является вещественным типом"), rtp.name().c_str(), rtp_param_name.c_str()));
			}
			// Тип ресурса подходит в качестве типа транзакта
			LPRDORTPResType pResType = PARSER->findRTPResType(rtp_name);
			pResType->setType(RDORTPResType::procTran);
		}
	}
	;

%%

CLOSE_RDO_PARSER_NAMESPACE

/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoproc_rss.y
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      24.03.2008
  \brief     Сбор ресурсов из процессов. based on rdodpt_rss.y
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
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/rdodpt.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdorss.h"
#include "simulator/compiler/parser/rdopmd.h"

#include "simulator/runtime/process/rdoprocess.h"

#include "simulator/compiler/mbuilder/rdo_resources.h"
#include "simulator/compiler/mbuilder/process.h"
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

%start prc_rss_main

%%

// --------------------------------------------------------------------------------
// -------------------- General part
// --------------------------------------------------------------------------------
prc_rss_main
	: /* empty */
	| prc_rss_main dpt_process_end
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
	: RDO_Process
	;

dpt_process_condition
	: /* empty */
	| RDO_Condition error
	;

dpt_process_prior
	: /* empty */
	| RDO_Priority error
	;

dpt_process_input
	: /* empty */
	| dpt_process_input dpt_process_line
	| dpt_process_input error dpt_process_line
	;

dpt_process_line
	: RDO_IDENTIF
	| RDO_GENERATE
	| RDO_TERMINATE
	| RDO_ADVANCE
	| RDO_ASSIGN
	| RDO_QUEUE dpt_queue_param
	| RDO_QUEUE error
	{
		PARSER->error().error(@1, rdo::format("Ожидается имя ресурса для сбора статистики по очереди"));
	}
	| RDO_DEPART dpt_depart_param
	| RDO_DEPART error
	{
		PARSER->error().error(@1, rdo::format("Ожидается имя ресурса для сбора статистики по очереди"));
	}
	| RDO_SEIZE dpt_seize_param
	| RDO_SEIZE error
	{
		PARSER->error().error(@1, rdo::format("Ожидается список ресурсов, объединяемых в блок, через запятую"));
	}
	| RDO_RELEASE dpt_release_param
	| RDO_RELEASE error
	{
		PARSER->error().error(@1, rdo::format("Ожидается список ресурсов, объединяемых в блок, через запятую"));
	}
	;

dpt_queue_param
	: RDO_IDENTIF
	{
		//! Имя ресурса
		tstring                res_name = PARSER->stack().pop<RDOValue>($1)->value().getIdentificator().c_str();
		CREF(RDOParserSrcInfo) info     = @1;
		// Получили список всех ресурсов
		rdo::compiler::mbuilder::RDOResourceList rssList(PARSER);
		//! Получили список всех типов ресурсов
		rdo::compiler::mbuilder::RDOResTypeList rtpList(PARSER);
		rdo::compiler::mbuilder::RDOResType rtp;
		tstring rtp_name = "QDEPART";
		tstring q_name   = "Очередь_" + res_name;
		//! Если ресурс существует, берем его тип и проверяем
		if (rssList[res_name].exist())
		{
			rtp = rssList[res_name].getType();
			rdo::compiler::mbuilder::BlockForQueue::checkType(rtp, info);
			LPRDOPMDWatchPar pResult = rdo::Factory<RDOPMDWatchPar>::create(RDOParserSrcInfo(q_name));
			ASSERT(pResult);
			pResult->init(false, RDOParserSrcInfo(res_name), RDOParserSrcInfo(tstring("длина_очереди")));
		}
		else
		{
			//! Если тип "QDEPART" существует
			if (rtpList[rtp_name].exist())
			{
				rdo::compiler::mbuilder::RDOResType rtp_ = rtpList[rtp_name];
				if (rdo::compiler::mbuilder::BlockForQueue::checkType(rtp_, info))
				{
					rdo::compiler::mbuilder::BlockForQueue::createRes(rtp_, res_name);
					LPRDOPMDWatchPar pResult = rdo::Factory<RDOPMDWatchPar>::create(RDOParserSrcInfo(q_name));
					ASSERT(pResult);
					pResult->init(false, RDOParserSrcInfo(res_name), RDOParserSrcInfo(tstring("длина_очереди")));
				}
			}
			else
			{
				rdo::compiler::mbuilder::RDOResType rtp_ = rdo::compiler::mbuilder::BlockForQueue::createType(rtp_name, info);
				if (rdo::compiler::mbuilder::BlockForQueue::checkType(rtp_, info))
				{
					rdo::compiler::mbuilder::BlockForQueue::createRes(rtp_, res_name);
					LPRDOPMDWatchPar pResult = rdo::Factory<RDOPMDWatchPar>::create(RDOParserSrcInfo(q_name));
					ASSERT(pResult);
					pResult->init(false, RDOParserSrcInfo(res_name), RDOParserSrcInfo(tstring("длина_очереди")));
				}
			}
		}
	}
	| RDO_IDENTIF error
	{
		PARSER->error().error(@1, "Ошибка в имени ресурса");
	}
	;

dpt_depart_param
	: RDO_IDENTIF
	{
		//! Имя ресурса
		tstring                res_name = PARSER->stack().pop<RDOValue>($1)->value().getIdentificator().c_str();
		CREF(RDOParserSrcInfo) info     = @1;
		//! Получили список всех ресурсов
		rdo::compiler::mbuilder::RDOResourceList rssList(PARSER);
		//! Получили список всех типов ресурсов
		rdo::compiler::mbuilder::RDOResTypeList rtpList(PARSER);
		rdo::compiler::mbuilder::RDOResType rtp;
		tstring rtp_name = "QDEPART";
		//! Если ресурс существует, берем его тип и проверяем
		if (rssList[res_name].exist())
		{
			rtp = rssList[res_name].getType();
			rdo::compiler::mbuilder::BlockForQueue::checkType(rtp, info);
		}
		else
		{
			if (rtpList[rtp_name].exist())
			{
				rdo::compiler::mbuilder::RDOResType rtp_ = rtpList[rtp_name];
				if (rdo::compiler::mbuilder::BlockForQueue::checkType(rtp_, info))
				{
					rdo::compiler::mbuilder::BlockForQueue::createRes(rtp_, res_name);
				}
			}
			else
			{
				rdo::compiler::mbuilder::RDOResType rtp_ = rdo::compiler::mbuilder::BlockForQueue::createType(rtp_name, info);
				if (rdo::compiler::mbuilder::BlockForQueue::checkType(rtp_, info))
				{
					rdo::compiler::mbuilder::BlockForQueue::createRes(rtp_, res_name);
				}
			}
		}
	}
	| RDO_IDENTIF error
	{
		PARSER->error().error(@1, "Ошибка в имени ресурса");
	}
	;

dpt_seize_param
	: RDO_IDENTIF
	{
		//! Имя ресурса
		tstring                res_name = PARSER->stack().pop<RDOValue>($1)->value().getIdentificator().c_str();
		CREF(RDOParserSrcInfo) info     = @1;
		//! Получили список всех ресурсов
		rdo::compiler::mbuilder::RDOResourceList rssList(PARSER);
		rdo::compiler::mbuilder::RDOResType rtp;
		//! Если ресурс существует, берем его тип и проверяем
		if (rssList[res_name].exist())
		{
			rtp = rssList[res_name].getType();
			if (rdo::compiler::mbuilder::BlockForSeize::checkType(rtp, info))
			{
				//! \todo принятие решения об изменении типа должно происходить в rdoproc_rtp.y
				if (!rssList[res_name].checkParserResourceType<RDORSSResource>(PARSER))
				{
					//здесь нужно переделать тип ресурса, а не ресурс
					rdo::compiler::mbuilder::BlockForSeize::reobjectRes(rtp, res_name);
				}
			}
		}
		else
		{
			//! Ресурс не найден, сформировать имя типа по имени ресурса
			//! Сформировать имя типа по имени ресурса
			tstring rtp_name(RDOPROCProcess::s_name_prefix + res_name + RDOPROCProcess::s_name_sufix);
			//! Получили список всех типов ресурсов
			rdo::compiler::mbuilder::RDOResTypeList rtpList(PARSER);
			//! Нашли тип ресурса
			if (rtpList[rtp_name].exist())
			{
				rdo::compiler::mbuilder::RDOResType rtp_ = rtpList[rtp_name];
				if (rdo::compiler::mbuilder::BlockForSeize::checkType(rtp_, info))
				{
					rdo::compiler::mbuilder::BlockForSeize::createRes(rtp_, res_name);
				}
			}
			else
			{
				rtp = rdo::compiler::mbuilder::BlockForSeize::createType(rtp_name, info);
				rdo::compiler::mbuilder::BlockForSeize::createRes(rtp, res_name);
			}
		}
	}
	| dpt_seize_param ',' RDO_IDENTIF
	{
		//! Имя ресурса
		tstring                res_name = PARSER->stack().pop<RDOValue>($3)->value().getIdentificator().c_str();
		CREF(RDOParserSrcInfo) info     = @3;
		//! Получили список всех ресурсов
		rdo::compiler::mbuilder::RDOResourceList rssList(PARSER);
		rdo::compiler::mbuilder::RDOResType rtp;
		//! Если ресурс существует, берем его тип и проверяем
		if (rssList[res_name].exist())
		{
			rtp = rssList[res_name].getType();
			if (rdo::compiler::mbuilder::BlockForSeize::checkType(rtp, info))
			{
				//! \todo принятие решения об изменении типа должно происходить в rdoproc_rtp.y
				if (!rssList[res_name].checkParserResourceType<RDORSSResource>(PARSER))
				{
					rdo::compiler::mbuilder::BlockForSeize::reobjectRes(rtp, res_name);
				}
			}
		}
		else
		{
			//! Ресурс не найден, сформировать имя типа по имени ресурса
			//! Сформировать имя типа по имени ресурса
			tstring rtp_name(RDOPROCProcess::s_name_prefix + res_name + RDOPROCProcess::s_name_sufix);
			//! Получили список всех типов ресурсов
			rdo::compiler::mbuilder::RDOResTypeList rtpList(PARSER);
			//! Нашли тип ресурса
			if (rtpList[rtp_name].exist())
			{
				rdo::compiler::mbuilder::RDOResType rtp_ = rtpList[rtp_name];
				if (rdo::compiler::mbuilder::BlockForSeize::checkType(rtp_, info))
				{
					rdo::compiler::mbuilder::BlockForSeize::createRes(rtp_, res_name);
				}
			}
			else
			{
				rtp = rdo::compiler::mbuilder::BlockForSeize::createType(rtp_name, info);
				rdo::compiler::mbuilder::BlockForSeize::createRes(rtp, res_name);
			}
		}
	}
	| dpt_seize_param error
	{
		PARSER->error().error(@1, "Ошибка в имени ресурса");
	}
	;

dpt_release_param
	: RDO_IDENTIF
	{
		//! Имя ресурса
		tstring                res_name = PARSER->stack().pop<RDOValue>($1)->value().getIdentificator().c_str();
		CREF(RDOParserSrcInfo) info     = @1;
		//! Получили список всех ресурсов
		rdo::compiler::mbuilder::RDOResourceList rssList(PARSER);
		rdo::compiler::mbuilder::RDOResType rtp;
		if (rssList[res_name].exist())
		{
			rtp = rssList[res_name].getType();
			if (rdo::compiler::mbuilder::BlockForSeize::checkType(rtp, info))
			{
				//! \todo принятие решения об изменении типа должно происходить в rdoproc_rtp.y
				if (!rssList[res_name].checkParserResourceType<RDORSSResource>(PARSER))
				{
					rdo::compiler::mbuilder::BlockForSeize::reobjectRes(rtp, res_name);
				}
			}
		}
		else
		{
			//! Ресурс не найден, сформировать имя типа по имени ресурса
			//! Сформировать имя типа по имени ресурса
			tstring rtp_name(RDOPROCProcess::s_name_prefix + res_name + RDOPROCProcess::s_name_sufix);
			//! Получили список всех типов ресурсов
			rdo::compiler::mbuilder::RDOResTypeList rtpList(PARSER);
			//! Нашли тип ресурса
			if (rtpList[rtp_name].exist())
			{
				rdo::compiler::mbuilder::RDOResType rtp_ = rtpList[rtp_name];
				if (rdo::compiler::mbuilder::BlockForSeize::checkType(rtp_, info))
				{
					rdo::compiler::mbuilder::BlockForSeize::createRes(rtp_, res_name);
				}
			}
			else
			{
				rtp = rdo::compiler::mbuilder::BlockForSeize::createType(rtp_name, info);
				rdo::compiler::mbuilder::BlockForSeize::createRes(rtp, res_name);
			}
		}
	}
	| dpt_release_param ',' RDO_IDENTIF
	{
		//! Имя ресурса
		tstring                res_name = PARSER->stack().pop<RDOValue>($3)->value().getIdentificator().c_str();
		CREF(RDOParserSrcInfo) info     = @3;
		//! Получили список всех ресурсов
		rdo::compiler::mbuilder::RDOResourceList rssList(PARSER);
		rdo::compiler::mbuilder::RDOResType rtp;
		//! Если ресурс существует берем его тип и проверяем
		if (rssList[res_name].exist())
		{
			rtp = rssList[res_name].getType();
			if (rdo::compiler::mbuilder::BlockForSeize::checkType(rtp, info))
			{
				//! \todo принятие решения об изменении типа должно происходить в rdoproc_rtp.y
				if (!rssList[res_name].checkParserResourceType<RDORSSResource>(PARSER))
				{
					rdo::compiler::mbuilder::BlockForSeize::reobjectRes(rtp, res_name);
				}
			}
		}
		else
		{
			//! Ресурс не найден, сформировать имя типа по имени ресурса
			//! Сформировать имя типа по имени ресурса
			tstring rtp_name(RDOPROCProcess::s_name_prefix + res_name + RDOPROCProcess::s_name_sufix);
			//! Получили список всех типов ресурсов
			rdo::compiler::mbuilder::RDOResTypeList rtpList(PARSER);
			//! Нашли тип ресурса
			if (rtpList[rtp_name].exist())
			{
				rdo::compiler::mbuilder::RDOResType rtp_ = rtpList[rtp_name];
				if (rdo::compiler::mbuilder::BlockForSeize::checkType(rtp_, info))
				{
					rdo::compiler::mbuilder::BlockForSeize::createRes(rtp_, res_name);
				}
			}
			else
			{
				rtp = rdo::compiler::mbuilder::BlockForSeize::createType(rtp_name, info);
				rdo::compiler::mbuilder::BlockForSeize::createRes(rtp, res_name);
			}
		}
	}
	| dpt_release_param error
	{
		PARSER->error().error(@1, "Ошибка в имени ресурса");
	}
	;

dpt_process_end
	: dpt_process RDO_End
	;

%%

CLOSE_RDO_PARSER_NAMESPACE

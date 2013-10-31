/*!
  \copyright (c) RDO-Team, 2011
  \file      rdorss.y
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     Синтаксис ресурсов
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
#include "simulator/compiler/parser/rdorss.h"
#include "simulator/compiler/parser/rdortp.h"
// --------------------------------------------------------------------------------

#define PARSER  LEXER->parser()
#define RUNTIME PARSER->runtime()

OPEN_RDO_PARSER_NAMESPACE
%}

%left RDO_IDENTIF
%left ':'

%start rss_main

%%

rss_main
	: /* empty */
	| rss_resources_begin rss_resources rss_resources_end
	| rss_resources_begin rss_resources
	{
		PARSER->error().error(@2, "После описания всех ресурсов ожидается ключевое слово $End");
	}
	| error
	{
		if (!PARSER->isHaveKWResources())
		{
			PARSER->error().error(@1, "Ожидается ключевое слово $Resources");
		}
		else
		{
			if (PARSER->isHaveKWResourcesEnd())
			{
				PARSER->error().error(@1, "Ресурсы уже определены");
			}
			else
			{
				PARSER->error().error(@1, "Неизвестная ошибка");
			}
		}
	}
	;

rss_resources_begin
	: RDO_Resources
	{
		PARSER->setHaveKWResources(true);
	}
	;

rss_resources_end
	: RDO_End
	{
		PARSER->setHaveKWResourcesEnd(true);
	}
	;

rss_resources
	: /* empty */
	| rss_resources rss_res_descr
	;

rss_res_descr
	: rss_res_type rss_trace rss_values
	{
		LPRDORSSResource pResource = PARSER->stack().pop<RDORSSResource>($1);
		ASSERT(pResource);
		if (!pResource->defined())
		{
			PARSER->error().error(@3, rdo::format("Заданы не все параметры ресурса: %s", pResource->name().c_str()));
		}
		pResource->setTrace($2 != 0);
		pResource->end();
	}
	;

rss_res_type
	: RDO_IDENTIF_COLON RDO_IDENTIF
	{
		LPRDOValue pName = PARSER->stack().pop<RDOValue>($1);
		LPRDOValue pType = PARSER->stack().pop<RDOValue>($2);
		ASSERT(pName);
		ASSERT(pType);

		LPRDORTPResType pResType = PARSER->findRTPResType(pType->value().getIdentificator());
		if (!pResType)
		{
			PARSER->error().error(@2, rdo::format("Неизвестный тип ресурса: %s", pType->value().getIdentificator().c_str()));
		}
		LPRDORSSResource pResourceExist = PARSER->findRSSResource(pName->value().getIdentificator());
		if (pResourceExist)
		{
			PARSER->error().push_only(pName->src_info(), rdo::format("Ресурс '%s' уже существует", pName->value().getIdentificator().c_str()));
			PARSER->error().push_only(pResourceExist->src_info(), "См. первое определение");
			PARSER->error().push_done();
		}
		LPRDORSSResource pResource = pResType->createRes(PARSER, pName->src_info());
		$$ = PARSER->stack().push(pResource);
	}
	| RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@2, "Ожидается тип ресурса");
	}
	| ':'
	{
		PARSER->error().error(@1, "Перед двоеточием ожидается имя ресурса");
	}
	| error
	{
		PARSER->error().error(@1, "Ожидается имя ресурса");
	}
	;

rss_trace
	: /* empty */  {$$ = 0;}
	| RDO_trace	   {$$ = 1;}
	| RDO_no_trace {$$ = 0;}
	;

rss_values
	: /* empty */
	| rss_values rss_value
	;

rss_value
	: '*'               {PARSER->getLastRSSResource()->addParam(rdo::Factory<RDOValue>::create(RDOParserSrcInfo(@1, "*")));}
	| '#'               {PARSER->getLastRSSResource()->addParam(rdo::Factory<RDOValue>::create(RDOParserSrcInfo(@1, "#")));}
	| RDO_INT_CONST     {PARSER->getLastRSSResource()->addParam(PARSER->stack().pop<RDOValue>($1));}
	| RDO_REAL_CONST    {PARSER->getLastRSSResource()->addParam(PARSER->stack().pop<RDOValue>($1));}
	| RDO_BOOL_CONST    {PARSER->getLastRSSResource()->addParam(PARSER->stack().pop<RDOValue>($1));}
	| RDO_STRING_CONST  {PARSER->getLastRSSResource()->addParam(PARSER->stack().pop<RDOValue>($1));}
	| RDO_IDENTIF       {PARSER->getLastRSSResource()->addParam(PARSER->stack().pop<RDOValue>($1));}
	| param_array_value {PARSER->getLastRSSResource()->addParam(PARSER->stack().pop<RDOValue>($1));}
	| error
	{
		PARSER->error().error(@1, rdo::format("Неправильное значение параметра: %s", LEXER->YYText()));
	}
	;

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

%%

CLOSE_RDO_PARSER_NAMESPACE

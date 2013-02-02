/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoevn.y
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \authors   Поподьянец Евгений (kurt.gigacore@gmail.com)
  \date      12.06.2010
  \brief     Синтаксис событий
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
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/compiler/parser/rdopat.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/type/range.h"
#include "simulator/compiler/parser/rdo_array.h"
#include "simulator/compiler/parser/src/function/local_variable/local_variable.h"
#include "simulator/compiler/parser/src/function/local_variable/local_variable_context.h"
#include "simulator/compiler/parser/type/such_as.h"
#include "simulator/compiler/parser/context/context_type.h"
#include "simulator/compiler/parser/context/memory.h"
#include "simulator/compiler/parser/context/statement.h"

#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/calc/calc_event.h"
#include "simulator/runtime/calc/calc_watch.h"
#include "simulator/runtime/calc/calc_process.h"
#include "simulator/runtime/calc/calc_array.h"
#include "simulator/runtime/calc/procedural/calc_locvar.h"
#include "simulator/runtime/calc/procedural/calc_statement.h"
#include "simulator/runtime/calc/procedural/calc_braces.h"
#include "simulator/runtime/calc/procedural/calc_range.h"
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

pat_main
	: /* empty */
	| pat_main pat_pattern
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($2);
		ASSERT(pPattern);
	}
	| error
	{
		PARSER->error().error(@1, _T("Неизвестная ошибка"));
	}
	;

pat_header
	: RDO_Pattern RDO_IDENTIF_COLON RDO_event pat_trace
	{
		LPRDOValue pName = PARSER->stack().pop<RDOValue>($2);
		ASSERT(pName);
		LPRDOPATPattern pPattern = rdo::Factory<RDOPatternEvent>::create(pName->src_info(), $4 != 0);
		ASSERT(pPattern);
		$$ = PARSER->stack().push(pPattern);
	}
	| RDO_Pattern error
	{
		PARSER->error().error(@2, _T("Ожидается имя образца"));
	}
	| RDO_Pattern RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@2, @3, _T("Ожидается тип образца"));
	}
	;

pat_trace
	: /* empty */	{ $$ = 0; }
	| RDO_trace		{ $$ = 1; }
	| RDO_no_trace	{ $$ = 0; }
	;

pat_params_begin
	: pat_header RDO_Parameters
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		$$ = PARSER->stack().push(pPattern);
	}
	;

pat_params
	: pat_params_begin RDO_IDENTIF_COLON param_type param_value_default
	{
		LPRDOPATPattern pPattern   = PARSER->stack().pop<RDOPATPattern>($1);
		LPRDOValue      pParamName = PARSER->stack().pop<RDOValue>($2);
		LPTypeInfo      pType      = PARSER->stack().pop<TypeInfo>($3);
		ASSERT(pPattern  );
		ASSERT(pParamName);
		ASSERT(pType     );

		LPRDOParam pParam = rdo::Factory<RDOParam>::create(pParamName->src_info(), pType, PARSER->stack().pop<RDOValue>($4));
		ASSERT(pParam);
		pPattern->add(pParam);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params RDO_IDENTIF_COLON param_type param_value_default
	{
		LPRDOPATPattern pPattern   = PARSER->stack().pop<RDOPATPattern>($1);
		LPRDOValue      pParamName = PARSER->stack().pop<RDOValue>($2);
		LPTypeInfo      pType      = PARSER->stack().pop<TypeInfo>($3);
		ASSERT(pPattern  );
		ASSERT(pParamName);
		ASSERT(pType     );

		LPRDOParam pParam = rdo::Factory<RDOParam>::create(pParamName->src_info(), pType, PARSER->stack().pop<RDOValue>($4));
		ASSERT(pParam);
		pPattern->add(pParam);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params_begin error
	{
		if (@1.m_last_line != @2.m_last_line)
		{
			PARSER->error().error(@2, _T("Ожидается имя параметра образца"));
		}
		else
		{
			PARSER->error().error(@2, rdo::format(_T("Ожидается имя параметра образца, найдено: %s"), LEXER->YYText()));
		}
	}
	| pat_params_begin RDO_IDENTIF error
	{
		if (@2.m_last_line != @3.m_last_line)
		{
			PARSER->error().error(@2, @3, _T("Ожидается двоеточие"));
		}
		else
		{
			PARSER->error().error(@2, @3, rdo::format(_T("Ожидается двоеточие, найдено: %s"), LEXER->YYText()));
		}
	}
	| pat_params_begin RDO_IDENTIF_COLON error
	{
		if (@2.m_last_line != @3.m_last_line)
		{
			PARSER->error().error(@2, @3, _T("Ожидается тип параметра образца"));
		}
		else
		{
			PARSER->error().error(@2, @3, rdo::format(_T("Ожидается тип параметра образца, найдено: %s"), LEXER->YYText()));
		}
	}
	| pat_params error
	{
		if (@1.m_last_line != @2.m_last_line)
		{
			PARSER->error().error(@2, _T("Ожидается имя параметра образца"));
		}
		else
		{
			PARSER->error().error(@2, rdo::format(_T("Ожидается имя параметра образца, найдено: %s"), LEXER->YYText()));
		}
	}
	| pat_params RDO_IDENTIF error
	{
		if (@2.m_last_line != @3.m_last_line)
		{
			PARSER->error().error(@2, @3, _T("Ожидается двоеточие"));
		}
		else
		{
			PARSER->error().error(@2, @3, rdo::format(_T("Ожидается двоеточие, найдено: %s"), LEXER->YYText()));
		}
	}
	| pat_params RDO_IDENTIF_COLON error
	{
		if (@2.m_last_line != @3.m_last_line)
		{
			PARSER->error().error(@2, @3, _T("Ожидается тип параметра образца"));
		}
		else
		{
			PARSER->error().error(@2, @3, rdo::format(_T("Ожидается тип параметра образца, найдено: %s"), LEXER->YYText()));
		}
	}
	;

pat_params_end
	: pat_params RDO_Relevant_resources
	{
		$$ = $1;
	}
	| pat_header RDO_Relevant_resources
	{
		$$ = $1;
	}
	| pat_header error
	{
		PARSER->error().error(@2, _T("Ожидается ключевое слово $Relevant_resources"));
	}
	;

pat_rel_res
	: pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF pat_conv pat_conv
	{
		// проверено для ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				LPRDOValue pRelName  = PARSER->stack().pop<RDOValue>($2);
				LPRDOValue pTypeName = PARSER->stack().pop<RDOValue>($3);
				ASSERT(pRelName );
				ASSERT(pTypeName);

				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(pRelName->src_info(), pTypeName->src_info(), (rdo::runtime::RDOResource::ConvertStatus)$4, (rdo::runtime::RDOResource::ConvertStatus)$5, @4, @5);
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@5, _T("У события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@5, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF pat_conv pat_conv
	{
		// проверено для ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				LPRDOValue pRelName  = PARSER->stack().pop<RDOValue>($2);
				LPRDOValue pTypeName = PARSER->stack().pop<RDOValue>($3);
				ASSERT(pRelName );
				ASSERT(pTypeName);

				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(pRelName->src_info(), pTypeName->src_info(), (rdo::runtime::RDOResource::ConvertStatus)$4, (rdo::runtime::RDOResource::ConvertStatus)$5, @4, @5);
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@5, _T("У события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@5, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF pat_conv
	{
		// проверено для ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@4, rdo::format(_T("Помимо статуса конвертора начала (%s), ожидается статус конвертора конца, потому что у операции есть событие конца"), RDOPATPattern::StatusToStr((rdo::runtime::RDOResource::ConvertStatus)$4).c_str()));
				break;
			}
			case RDOPATPattern::PT_Event: 
			case RDOPATPattern::PT_Rule :
			{
				LPRDOValue pRelName  = PARSER->stack().pop<RDOValue>($2);
				LPRDOValue pTypeName = PARSER->stack().pop<RDOValue>($3);
				ASSERT(pRelName );
				ASSERT(pTypeName);

				pPattern->addRelRes(pRelName->src_info(), pTypeName->src_info(), (rdo::runtime::RDOResource::ConvertStatus)$4, @4);
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF pat_conv
	{
		// проверено для ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@4, rdo::format(_T("Помимо статуса конвертора начала (%s), ожидается статус конвертора конца, потому что у операции есть событие конца"), RDOPATPattern::StatusToStr((rdo::runtime::RDOResource::ConvertStatus)$4).c_str()));
				break;
			}
			case RDOPATPattern::PT_Event: 
			case RDOPATPattern::PT_Rule :
			{
				LPRDOValue pRelName  = PARSER->stack().pop<RDOValue>($2);
				LPRDOValue pTypeName = PARSER->stack().pop<RDOValue>($3);
				ASSERT(pRelName );
				ASSERT(pTypeName);

				pPattern->addRelRes(pRelName->src_info(), pTypeName->src_info(), (rdo::runtime::RDOResource::ConvertStatus)$4, @4);
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange pat_conv
	{
		// проверено для ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				LPRDOValue pRelName  = PARSER->stack().pop<RDOValue>($2);
				LPRDOValue pTypeName = PARSER->stack().pop<RDOValue>($3);
				ASSERT(pRelName );
				ASSERT(pTypeName);

				YYLTYPE convertor_pos = @3;
				convertor_pos.m_first_line = convertor_pos.m_last_line;
				convertor_pos.m_first_pos  = convertor_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::CS_NoChange).length();
				convertor_pos.m_first_seek = convertor_pos.m_last_seek - RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(pRelName->src_info(), pTypeName->src_info(), rdo::runtime::RDOResource::CS_NoChange, (rdo::runtime::RDOResource::ConvertStatus)$4, convertor_pos, @4);
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@4, _T("У события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@4, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange pat_conv
	{
		// проверено для ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				LPRDOValue pRelName  = PARSER->stack().pop<RDOValue>($2);
				LPRDOValue pTypeName = PARSER->stack().pop<RDOValue>($3);
				ASSERT(pRelName );
				ASSERT(pTypeName);

				YYLTYPE convertor_pos = @3;
				convertor_pos.m_first_line = convertor_pos.m_last_line;
				convertor_pos.m_first_pos  = convertor_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::CS_NoChange).length();
				convertor_pos.m_first_seek = convertor_pos.m_last_seek - RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(pRelName->src_info(), pTypeName->src_info(), rdo::runtime::RDOResource::CS_NoChange, (rdo::runtime::RDOResource::ConvertStatus)$4, convertor_pos, @4);
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@4, _T("У события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@4, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange_NoChange
	{
		// проверено для ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				LPRDOValue pRelName  = PARSER->stack().pop<RDOValue>($2);
				LPRDOValue pTypeName = PARSER->stack().pop<RDOValue>($3);
				ASSERT(pRelName );
				ASSERT(pTypeName);

				YYLTYPE convertor_begin_pos = @3;
				tstring str = LEXER->YYText();
				rdo::toLower(str);
				tstring::size_type first_nochange = str.find(_T("nochange"));
				ruint i = 0;
				for (;;)
				{
					if (str[i] == '\n')
					{
						convertor_begin_pos.m_first_line++;
						convertor_begin_pos.m_first_seek++;
						convertor_begin_pos.m_first_pos = 0;
					}
					else if (str[i] != '\r')
					{
						convertor_begin_pos.m_first_pos++;
						convertor_begin_pos.m_first_seek++;
					}
					i++;
					if (i == first_nochange)
						break;
				}
				convertor_begin_pos.m_last_line = convertor_begin_pos.m_first_line;
				convertor_begin_pos.m_last_pos  = convertor_begin_pos.m_first_pos  + RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::CS_NoChange).length();
				convertor_begin_pos.m_last_seek = convertor_begin_pos.m_first_seek + RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::CS_NoChange).length();
				YYLTYPE convertor_end_pos = @3;
				convertor_end_pos.m_first_line = convertor_end_pos.m_last_line;
				convertor_end_pos.m_first_pos  = convertor_end_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::CS_NoChange).length();
				convertor_end_pos.m_first_seek = convertor_end_pos.m_last_seek - RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(pRelName->src_info(), pTypeName->src_info(), rdo::runtime::RDOResource::CS_NoChange, rdo::runtime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos);
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@3, _T("У события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@3, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange_NoChange
	{
		// проверено для ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				LPRDOValue pRelName  = PARSER->stack().pop<RDOValue>($2);
				LPRDOValue pTypeName = PARSER->stack().pop<RDOValue>($3);
				ASSERT(pRelName );
				ASSERT(pTypeName);

				YYLTYPE convertor_begin_pos = @3;
				tstring str = LEXER->YYText();
				rdo::toLower(str);
				tstring::size_type first_nochange = str.find(_T("nochange"));
				ruint i = 0;
				for (;;)
				{
					if (str[i] == '\n')
					{
						convertor_begin_pos.m_first_line++;
						convertor_begin_pos.m_first_seek++;
						convertor_begin_pos.m_first_pos = 0;
					}
					else if (str[i] != '\r')
					{
						convertor_begin_pos.m_first_pos++;
						convertor_begin_pos.m_first_seek++;
					}
					i++;
					if (i == first_nochange)
						break;
				}
				convertor_begin_pos.m_last_line = convertor_begin_pos.m_first_line;
				convertor_begin_pos.m_last_pos  = convertor_begin_pos.m_first_pos  + RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::CS_NoChange).length();
				convertor_begin_pos.m_last_seek = convertor_begin_pos.m_first_seek + RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::CS_NoChange).length();
				YYLTYPE convertor_end_pos = @3;
				convertor_end_pos.m_first_line = convertor_end_pos.m_last_line;
				convertor_end_pos.m_first_pos  = convertor_end_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::CS_NoChange).length();
				convertor_end_pos.m_first_seek = convertor_end_pos.m_last_seek - RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(pRelName->src_info(), pTypeName->src_info(), rdo::runtime::RDOResource::CS_NoChange, rdo::runtime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos);
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@3, _T("У события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@3, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange
	{
		// проверено для ie,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@3, rdo::format(_T("Помимо статуса конвертора начала (%s), ожидается статус конвертора конца, потому что у операции есть событие конца"), RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::CS_NoChange).c_str()));
				break;
			}
			case RDOPATPattern::PT_Event: 
			case RDOPATPattern::PT_Rule :
			{
				LPRDOValue pRelName  = PARSER->stack().pop<RDOValue>($2);
				LPRDOValue pTypeName = PARSER->stack().pop<RDOValue>($3);
				ASSERT(pRelName );
				ASSERT(pTypeName);

				YYLTYPE convertor_pos = @3;
				convertor_pos.m_first_line = convertor_pos.m_last_line;
				convertor_pos.m_first_pos  = convertor_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::CS_NoChange).length();
				convertor_pos.m_first_seek = convertor_pos.m_last_seek - RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::CS_NoChange).length();
				pPattern->addRelRes(pRelName->src_info(), pTypeName->src_info(), rdo::runtime::RDOResource::CS_NoChange, convertor_pos);
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange
	{
		// проверено для ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@3, rdo::format(_T("Помимо статуса конвертора начала (%s), ожидается статус конвертора конца, потому что у операции есть событие конца"), RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::CS_NoChange).c_str()));
				break;
			}
			case RDOPATPattern::PT_Event: 
			case RDOPATPattern::PT_Rule :
			{
				LPRDOValue pRelName  = PARSER->stack().pop<RDOValue>($2);
				LPRDOValue pTypeName = PARSER->stack().pop<RDOValue>($3);
				ASSERT(pRelName );
				ASSERT(pTypeName);

				YYLTYPE convertor_pos = @3;
				convertor_pos.m_first_line = convertor_pos.m_last_line;
				convertor_pos.m_first_pos  = convertor_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::CS_NoChange).length();
				convertor_pos.m_first_seek = convertor_pos.m_last_seek - RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::CS_NoChange).length();
				pPattern->addRelRes(pRelName->src_info(), pTypeName->src_info(), rdo::runtime::RDOResource::CS_NoChange, convertor_pos);
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF RDO_IDENTIF_NoChange
	{
		// проверено для ie,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				LPRDOValue pRelName  = PARSER->stack().pop<RDOValue>($2);
				LPRDOValue pTypeName = PARSER->stack().pop<RDOValue>($3);
				ASSERT(pRelName );
				ASSERT(pTypeName);

				tstring convert_begin = PARSER->stack().pop<RDOValue>($4)->value().getIdentificator();
				YYLTYPE convertor_begin_pos = @4;
				convertor_begin_pos.m_last_line = convertor_begin_pos.m_first_line;
				convertor_begin_pos.m_last_pos  = convertor_begin_pos.m_first_pos  + convert_begin.length();
				convertor_begin_pos.m_last_seek = convertor_begin_pos.m_first_seek + convert_begin.length();
				YYLTYPE convertor_end_pos = @4;
				convertor_end_pos.m_first_line = convertor_end_pos.m_last_line;
				convertor_end_pos.m_first_pos  = convertor_end_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::CS_NoChange).length();
				convertor_end_pos.m_first_seek = convertor_end_pos.m_last_seek - RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(pRelName->src_info(), pTypeName->src_info(), pPattern->StrToStatus(convert_begin, convertor_begin_pos), rdo::runtime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos);
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@4, _T("У события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@4, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF RDO_IDENTIF_NoChange
	{
		// проверено для ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				LPRDOValue pRelName  = PARSER->stack().pop<RDOValue>($2);
				LPRDOValue pTypeName = PARSER->stack().pop<RDOValue>($3);
				ASSERT(pRelName );
				ASSERT(pTypeName);

				tstring convert_begin = PARSER->stack().pop<RDOValue>($4)->value().getIdentificator();
				YYLTYPE convertor_begin_pos = @4;
				convertor_begin_pos.m_last_line = convertor_begin_pos.m_first_line;
				convertor_begin_pos.m_last_pos  = convertor_begin_pos.m_first_pos  + convert_begin.length();
				convertor_begin_pos.m_last_seek = convertor_begin_pos.m_first_seek + convert_begin.length();
				YYLTYPE convertor_end_pos = @4;
				convertor_end_pos.m_first_line = convertor_end_pos.m_last_line;
				convertor_end_pos.m_first_pos  = convertor_end_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::CS_NoChange).length();
				convertor_end_pos.m_first_seek = convertor_end_pos.m_last_seek - RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(pRelName->src_info(), pTypeName->src_info(), pPattern->StrToStatus(convert_begin, convertor_begin_pos), rdo::runtime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos);
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@4, _T("У события нет события конца, а значит и второго статуса конвертора"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@4, _T("У продукционного правила нет события конца, а значит и второго статуса конвертора"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params_end error
	{
		PARSER->error().error(@2, _T("Ошибка в описании релевантных ресурсов"));
	}
	| pat_rel_res error
	{
		PARSER->error().error(@2, _T("Ошибка в описании релевантных ресурсов"));
	}
	| pat_params_end RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@2, @3, _T("Ожидается описатель (имя типа или ресурса)"));
	}
	| pat_rel_res RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@2, @3, _T("Ожидается описатель (имя типа или ресурса)"));
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF error
	{
		if (PARSER->getLastPATPattern()->isHaveConvertEnd())
		{
			PARSER->error().error(@3, @4, _T("Ожидается статус конвертора начала"));
		}
		else
		{
			PARSER->error().error(@3, @4, _T("Ожидается статус конвертора"));
		}
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF error
	{
		if (PARSER->getLastPATPattern()->isHaveConvertEnd())
		{
			PARSER->error().error(@3, @4, _T("Ожидается статус конвертора начала"));
		}
		else
		{
			PARSER->error().error(@3, @4, _T("Ожидается статус конвертора"));
		}
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF pat_conv error
	{
		switch (PARSER->getLastPATPattern()->getType())
		{
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@5, _T("Ожидается способ выбора (first/with_min/with_max) или $Body"));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@5, _T("Ожидается способ выбора (first/with_min/with_max) или $Body"));
				break;
			}
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@4, @5, rdo::format(_T("Ожидается статус конвертора конца, найдено: %s"), LEXER->YYText()));
				break;
			}
		}
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF pat_conv error
	{
		switch (PARSER->getLastPATPattern()->getType())
		{
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@5, _T("Ожидается способ выбора (first/with_min/with_max) или $Body"));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@5, _T("Ожидается способ выбора (first/with_min/with_max) или $Body"));
				break;
			}
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@4, @5, rdo::format(_T("Ожидается статус конвертора конца, найдено: %s"), LEXER->YYText()));
				break;
			}
		}
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange error
	{
		switch (PARSER->getLastPATPattern()->getType())
		{
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@4, _T("Ожидается способ выбора (first/with_min/with_max) или $Body"));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@4, _T("Ожидается способ выбора (first/with_min/with_max) или $Body"));
				break;
			}
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@3, @4, rdo::format(_T("Ожидается статус конвертора конца, найдено: %s"), LEXER->YYText()));
				break;
			}
		}
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange error
	{
		switch (PARSER->getLastPATPattern()->getType())
		{
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@4, _T("Ожидается способ выбора (first/with_min/with_max) или $Body"));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@4, _T("Ожидается способ выбора (first/with_min/with_max) или $Body"));
				break;
			}
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@3, @4, rdo::format(_T("Ожидается статус конвертора конца, найдено: %s"), LEXER->YYText()));
				break;
			}
		}
	}
	;

pat_conv
	: RDO_Keep			{ $$ = rdo::runtime::RDOResource::CS_Keep;     }
	| RDO_Create		{ $$ = rdo::runtime::RDOResource::CS_Create;   }
	| RDO_Erase			{ $$ = rdo::runtime::RDOResource::CS_Erase;    }
	| RDO_NonExist		{ $$ = rdo::runtime::RDOResource::CS_NonExist; }
	;

pat_common_choice
	: pat_rel_res
	| pat_rel_res RDO_first
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		if (pPattern->getType() == RDOPATPattern::PT_Event)
		{
			PARSER->error().error(@2, _T("В событиях не используется способ выбора релевантных ресурсов"));
		}
		else
		{
			pPattern->setCommonChoiceFirst();
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_with_min fun_arithm
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		if (pPattern->getType() == RDOPATPattern::PT_Event)
		{
			PARSER->error().error(@2, _T("В событиях не используется способ выбора релевантных ресурсов"));
		}
		else
		{
			LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($3);
			ASSERT(pArithm);
			pArithm->setSrcPos (@2, @3);
			pArithm->setSrcText(_T("with_min ") + pArithm->src_text());
			pPattern->setCommonChoiceWithMin(pArithm);
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_with_max fun_arithm
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		if ( pPattern->getType() == RDOPATPattern::PT_Event)
		{
			PARSER->error().error(@2, _T("В событиях не используется способ выбора релевантных ресурсов"));
		}
		else
		{
			LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($3);
			ASSERT(pArithm);
			pArithm->setSrcPos (@2, @3);
			pArithm->setSrcText(_T("with_max ") + pArithm->src_text());
			pPattern->setCommonChoiceWithMax(pArithm);
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_with_min error
	{
		PARSER->error().error(@3, _T("Ошибка в арифметическом выражении"));
	}
	| pat_rel_res RDO_with_max error
	{
		PARSER->error().error(@3, _T("Ошибка в арифметическом выражении"));
	}
	;

pat_time
	: pat_common_choice RDO_Body
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_common_choice error
	{
		PARSER->error().error(@1, @2, _T("Ожидается ключевое слово $Body"));
	}
	;

pat_body
	: pat_time RDO_IDENTIF_RELRES
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		tstring         name     = PARSER->stack().pop<RDOValue>($2)->value().getIdentificator();
		pPattern->addRelResBody(RDOParserSrcInfo(@2, name));
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_convert RDO_IDENTIF_RELRES
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		tstring         name     = PARSER->stack().pop<RDOValue>($2)->value().getIdentificator();
		pPattern->addRelResBody(RDOParserSrcInfo(@2, name));
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_time error
	{
		tstring str(LEXER->YYText());
		PARSER->error().error(@2, rdo::format(_T("Неизвестный релевантный ресурс: %s"), str.c_str()));
	}
	| pat_convert error
	{
		tstring str(LEXER->YYText());
		PARSER->error().error(@2, rdo::format(_T("Неизвестный релевантный ресурс: %s"), str.c_str()));
	}
	;

pat_res_usage
	: pat_body pat_choice pat_order
	{
		LPRDOPATChoiceFrom pChoiceFrom = PARSER->stack().pop<RDOPATChoiceFrom>($2);
		ASSERT(pChoiceFrom);
		pChoiceFrom->setSrcPos(@2);

		LPRDOPATChoiceOrder pChoiceOrder = PARSER->stack().pop<RDOPATChoiceOrder>($3);
		pChoiceOrder->setSrcPos(@3);

		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		pPattern->addRelResUsage(pChoiceFrom, pChoiceOrder);
		$$ = PARSER->stack().push(pPattern);
	}
	;

pat_choice
	: /* empty */
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::choiceEmpty;
		LPRDOPATChoiceFrom pChoiceFrom = rdo::Factory<RDOPATChoiceFrom>::create(RDOParserSrcInfo(_T("Choice NoCheck")), RDOPATChoiceFrom::ch_empty);
		ASSERT(pChoiceFrom);
		$$ = PARSER->stack().push(pChoiceFrom);
	}
	| pat_choice_nocheck
	{
		LPRDOPATChoiceFrom pChoiceFrom = rdo::Factory<RDOPATChoiceFrom>::create(RDOParserSrcInfo(_T("Choice NoCheck")), RDOPATChoiceFrom::ch_nocheck);
		ASSERT(pChoiceFrom);
		$$ = PARSER->stack().push(pChoiceFrom);
	}
	| pat_choice_from fun_logic
	{
		LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		LPRDOPATChoiceFrom pChoiceFrom = rdo::Factory<RDOPATChoiceFrom>::create(RDOParserSrcInfo(_T("Choice from ") + pLogic->src_text()), RDOPATChoiceFrom::ch_from, pLogic);
		ASSERT(pChoiceFrom);
		$$ = PARSER->stack().push(pChoiceFrom);
	}
	| pat_choice_from error
	{
		PARSER->error().error(@2, _T("Ошибка в логическом выражении"));
	}
	;

pat_choice_nocheck
	: RDO_Choice RDO_NoCheck
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::choiceNoCheck;
	}
	;

pat_choice_from
	: RDO_Choice RDO_from
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::choiceFrom;
	}
	;

pat_order
	: /* empty */
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::choiceOrderEmpty;
		LPRDOPATChoiceOrder pChoiceOrder = rdo::Factory<RDOPATChoiceOrder>::create(RDOParserSrcInfo(), rdo::runtime::RDOSelectResourceCalc::order_empty);
		ASSERT(pChoiceOrder);
		$$ = PARSER->stack().push(pChoiceOrder);
	}
	| pat_choice_first
	{
		LPRDOPATChoiceOrder pChoiceOrder = rdo::Factory<RDOPATChoiceOrder>::create(RDOParserSrcInfo(_T("first")), rdo::runtime::RDOSelectResourceCalc::order_first);
		ASSERT(pChoiceOrder);
		$$ = PARSER->stack().push(pChoiceOrder);
	}
	| pat_choice_with_min fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		LPRDOPATChoiceOrder pChoiceOrder = rdo::Factory<RDOPATChoiceOrder>::create(RDOParserSrcInfo(_T("with_min ") + pArithm->src_text()), rdo::runtime::RDOSelectResourceCalc::order_with_min, pArithm);
		ASSERT(pChoiceOrder);
		$$ = PARSER->stack().push(pChoiceOrder);
	}
	| pat_choice_with_max fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		LPRDOPATChoiceOrder pChoiceOrder = rdo::Factory<RDOPATChoiceOrder>::create(RDOParserSrcInfo(_T("with_max ") + pArithm->src_text()), rdo::runtime::RDOSelectResourceCalc::order_with_max, pArithm);
		ASSERT(pChoiceOrder);
		$$ = PARSER->stack().push(pChoiceOrder);
	}
	| pat_choice_with_min error
	{
		PARSER->error().error(@2, _T("Ошибка в арифметическом выражении"));
	}
	| pat_choice_with_max error
	{
		PARSER->error().error(@2, _T("Ошибка в арифметическом выражении"));
	}
	;

pat_choice_first
	: RDO_first
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::choiceOrderFirst;
	}
	;

pat_choice_with_min
	: RDO_with_min
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::choiceOrderWithMin;
	}
	;

pat_choice_with_max
	: RDO_with_max
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::choiceOrderWithMax;
	}
	;

pat_convert
	: pat_res_usage
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		LPRDORelevantResource rel_res  = pPattern->m_pCurrRelRes;
		tstring str;
		if (rel_res->m_pChoiceOrder->m_type != rdo::runtime::RDOSelectResourceCalc::order_empty)
		{
			str = _T("Сразу после ключевого слова ") + rel_res->m_pChoiceOrder->asString();
		}
		else if (rel_res->m_pChoiceFrom->m_type != RDOPATChoiceFrom::ch_empty)
		{
			str = _T("Сразу после условия выбора");
		}
		else
		{
			str = _T("Сразу после имени");
		}
		if (rel_res->m_statusBegin != rdo::runtime::RDOResource::CS_NoChange && rel_res->m_statusBegin != rdo::runtime::RDOResource::CS_Erase && rel_res->m_statusBegin != rdo::runtime::RDOResource::CS_NonExist)
		{
			switch (pPattern->getType())
			{
				case RDOPATPattern::PT_Event:
				{
					PARSER->error().error(@1, rdo::format(_T("%s ожидается ключевое слово Convert_event для релевантного ресурса '%s', т.к. его статус '%s', но найдено: %s"), str.c_str(), rel_res->name().c_str(), RDOPATPattern::StatusToStr(rel_res->m_statusBegin).c_str(), LEXER->YYText()));
					break;
				}
				case RDOPATPattern::PT_Rule:
				{
					PARSER->error().error(@1, rdo::format(_T("%s ожидается ключевое слово Convert_rule для релевантного ресурса '%s', т.к. его статус '%s', но найдено: %s"), str.c_str(), rel_res->name().c_str(), RDOPATPattern::StatusToStr(rel_res->m_statusBegin).c_str(), LEXER->YYText()));
					break;
				}
				case RDOPATPattern::PT_Operation:
				case RDOPATPattern::PT_Keyboard :
				{
					PARSER->error().error(@1, rdo::format(_T("%s ожидается ключевое слово Convert_begin для релевантного ресурса '%s', т.к. его статус '%s', но найдено: %s"), str.c_str(), rel_res->name().c_str(), RDOPATPattern::StatusToStr(rel_res->m_statusBegin).c_str(), LEXER->YYText()));
					break;
				}
			}
		}
		if (rel_res->m_statusEnd != rdo::runtime::RDOResource::CS_NoChange && rel_res->m_statusEnd != rdo::runtime::RDOResource::CS_Erase && rel_res->m_statusEnd != rdo::runtime::RDOResource::CS_NonExist)
		{
			switch (pPattern->getType())
			{
				case RDOPATPattern::PT_Event:
				case RDOPATPattern::PT_Rule:
				{
					PARSER->error().error(@1, _T("Внутренняя ошибка"));
					break;
				}
				case RDOPATPattern::PT_Operation:
				case RDOPATPattern::PT_Keyboard :
				{
					PARSER->error().error(@1, rdo::format(_T("%s ожидается ключевое слово Convert_end для релевантного ресурса '%s', т.к. его статус '%s', но найдено: %s"), str.c_str(), rel_res->name().c_str(), RDOPATPattern::StatusToStr(rel_res->m_statusBegin).c_str(), LEXER->YYText()));
					break;
				}
			}
		}

		$$ = PARSER->stack().push(pPattern);
	}
	| pat_res_usage convert_begin pat_trace statement_list
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		if (pPattern->getType() != RDOPATPattern::PT_Operation && pPattern->getType() != RDOPATPattern::PT_Keyboard)
		{
			tstring type = _T("");
			switch (pPattern->getType())
			{
			case RDOPATPattern::PT_Event: type = _T("событии");               break;
			case RDOPATPattern::PT_Rule : type = _T("продукционном правиле"); break;
			default                     : break;
			}
			PARSER->error().error(@2, rdo::format(_T("Ключевое слово Convert_begin может быть использовано в обыкновенной или клавиатурной операции, но не в %s '%s'"), type.c_str(), pPattern->name().c_str()));
		}

		LPExpression pExpressionConvertBody = PARSER->stack().pop<Expression>($4);
		ASSERT(pExpressionConvertBody);

		rdo::runtime::LPRDOCalcStatementList pCalcStatementList = pExpressionConvertBody->calc().object_dynamic_cast<rdo::runtime::RDOCalcStatementList>();
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

		LPExpression pExpressionConvert = rdo::Factory<Expression>::create(pExpressionConvertBody->typeInfo(), pCalcBaseStatementList, pCalcStatementList->srcInfo());
		ASSERT(pExpressionConvert);

		pPattern.object_static_cast<RDOPatternOperation>()->addRelResConvertBeginEnd($3 != 0, pExpressionConvert, false, NULL, @2, @2, @3, @3);

		RDOParser::s_parser()->contextStack()->pop<ContextReturnable>();

		$$ = PARSER->stack().push(pPattern);
	}
	| pat_res_usage convert_end pat_trace statement_list
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		if (pPattern->getType() != RDOPATPattern::PT_Operation && pPattern->getType() != RDOPATPattern::PT_Keyboard)
		{
			tstring type = _T("");
			switch (pPattern->getType())
			{
			case RDOPATPattern::PT_Event: type = _T("событии");               break;
			case RDOPATPattern::PT_Rule : type = _T("продукционном правиле"); break;
			default                     : break;
			}
			PARSER->error().error(@2, rdo::format(_T("Ключевое слово Convert_end может быть использовано в обыкновенной и клавиатурной операции, но не в %s '%s'"), type.c_str(), pPattern->name().c_str()));
		}

		LPExpression pExpressionConvertBody = PARSER->stack().pop<Expression>($4);
		ASSERT(pExpressionConvertBody);

		rdo::runtime::LPRDOCalcStatementList pCalcStatementList = pExpressionConvertBody->calc().object_dynamic_cast<rdo::runtime::RDOCalcStatementList>();
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

		LPExpression pExpressionConvert = rdo::Factory<Expression>::create(pExpressionConvertBody->typeInfo(), pCalcBaseStatementList, pCalcStatementList->srcInfo());
		ASSERT(pExpressionConvert);

		pPattern.object_static_cast<RDOPatternOperation>()->addRelResConvertBeginEnd(false, NULL, $3 != 0, pExpressionConvert, @2, @2, @3, @3);

		RDOParser::s_parser()->contextStack()->pop<ContextReturnable>();

		$$ = PARSER->stack().push(pPattern);
	}
	| pat_res_usage convert_begin pat_trace statement_list convert_end pat_trace statement_list
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		if (pPattern->getType() != RDOPATPattern::PT_Operation && pPattern->getType() != RDOPATPattern::PT_Keyboard)
		{
			tstring type = _T("");
			switch (pPattern->getType())
			{
			case RDOPATPattern::PT_Event: type = _T("событии");               break;
			case RDOPATPattern::PT_Rule : type = _T("продукционном правиле"); break;
			default                     : break;
			}
			PARSER->error().error(@2, rdo::format(_T("Ключевые слова Convert_begin и Convert_end могут быть использованы в обыкновенной и клавиатурной операции, но не в %s '%s'"), type.c_str(), pPattern->name().c_str()));
		}

		LPExpression pExpressionConvertBodyBegin = PARSER->stack().pop<Expression>($4);
		ASSERT(pExpressionConvertBodyBegin);
		LPExpression pExpressionConvertBodyEnd = PARSER->stack().pop<Expression>($7);
		ASSERT(pExpressionConvertBodyEnd);

		rdo::runtime::LPRDOCalcStatementList pCalcStatementListBegin = pExpressionConvertBodyBegin->calc().object_dynamic_cast<rdo::runtime::RDOCalcStatementList>();
		ASSERT(pCalcStatementListBegin);
		rdo::runtime::LPRDOCalcStatementList pCalcStatementListEnd = pExpressionConvertBodyEnd->calc().object_dynamic_cast<rdo::runtime::RDOCalcStatementList>();
		ASSERT(pCalcStatementListEnd);

		rdo::runtime::LPRDOCalcBaseStatementList pCalcBaseStatementListBegin = rdo::Factory<rdo::runtime::RDOCalcBaseStatementList>::create();
		ASSERT(pCalcBaseStatementListBegin);

		rdo::runtime::LPRDOCalcBaseStatementList pCalcBaseStatementListEnd = rdo::Factory<rdo::runtime::RDOCalcBaseStatementList>::create();
		ASSERT(pCalcBaseStatementListEnd);

		rdo::runtime::LPRDOCalcOpenBrace pCalcOpenBrace = rdo::Factory<rdo::runtime::RDOCalcOpenBrace>::create();
		ASSERT(pCalcOpenBrace);

		rdo::runtime::LPRDOCalcCloseBrace pCalcCloseBrace = rdo::Factory<rdo::runtime::RDOCalcCloseBrace>::create();
		ASSERT(pCalcCloseBrace);

		pCalcBaseStatementListBegin->addCalcStatement(pCalcOpenBrace);
		pCalcBaseStatementListBegin->addCalcStatement(pCalcStatementListBegin);
		pCalcBaseStatementListBegin->addCalcStatement(pCalcCloseBrace);

		pCalcBaseStatementListEnd->addCalcStatement(pCalcOpenBrace);
		pCalcBaseStatementListEnd->addCalcStatement(pCalcStatementListEnd);
		pCalcBaseStatementListEnd->addCalcStatement(pCalcCloseBrace);

		LPExpression pExpressionConvertBegin =  rdo::Factory<Expression>::create(pExpressionConvertBodyBegin->typeInfo(),pCalcBaseStatementListBegin,pCalcBaseStatementListBegin->srcInfo());
		ASSERT(pExpressionConvertBegin);

		LPExpression pExpressionConvertEnd =  rdo::Factory<Expression>::create(pExpressionConvertBodyEnd->typeInfo(),pCalcBaseStatementListEnd,pCalcBaseStatementListEnd->srcInfo());
		ASSERT(pExpressionConvertEnd);

		pPattern.object_static_cast<RDOPatternOperation>()->addRelResConvertBeginEnd(
			$3 != 0,
			pExpressionConvertBegin ,
			$6 != 0                 ,
			pExpressionConvertEnd   ,
			@2, @5, @3, @6          );


		RDOParser::s_parser()->contextStack()->pop<ContextReturnable>();
		RDOParser::s_parser()->contextStack()->pop<ContextReturnable>();

		$$ = PARSER->stack().push(pPattern);
	}
	| pat_res_usage convert_rule pat_trace statement_list
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		if (pPattern->getType() != RDOPATPattern::PT_Rule)
		{
			tstring type = _T("");
			switch (pPattern->getType())
			{
			case RDOPATPattern::PT_Event    : type = _T("событии");               break;
			case RDOPATPattern::PT_Operation: type = _T("операции");              break;
			case RDOPATPattern::PT_Keyboard : type = _T("клавиатурной операции"); break;
			default                         : break;
			}
			PARSER->error().error(@2, rdo::format(_T("Ключевое слово Convert_rule может быть использовано в продукционном правиле, но не в %s '%s'"), type.c_str(), pPattern->name().c_str()));
		}

		LPRDORelevantResource pRelRes = pPattern->m_pCurrRelRes;
		ASSERT(pRelRes);

		LPExpression pExpressionConvertBody = PARSER->stack().pop<Expression>($4);
		ASSERT(pExpressionConvertBody);

		rdo::runtime::LPRDOCalcStatementList pCalcStatementList = pExpressionConvertBody->calc().object_dynamic_cast<rdo::runtime::RDOCalcStatementList>();
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

		LPExpression pExpressionConvert = rdo::Factory<Expression>::create(pExpressionConvertBody->typeInfo(), pCalcBaseStatementList, pCalcStatementList->srcInfo());
		ASSERT(pExpressionConvert);

		pPattern->addRelResConvert($3 != 0, pExpressionConvert, @2, @3, pRelRes->m_statusBegin);

		RDOParser::s_parser()->contextStack()->pop<ContextReturnable>();

		$$ = PARSER->stack().push(pPattern);
	}
	| pat_res_usage convert_event pat_trace statement_list
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);

		if (pPattern->getType() != RDOPATPattern::PT_Event)
		{
			tstring type = _T("");
			switch (pPattern->getType())
			{
			case RDOPATPattern::PT_Rule     : type = _T("продукционном правиле"); break;
			case RDOPATPattern::PT_Operation: type = _T("операции");              break;
			case RDOPATPattern::PT_Keyboard : type = _T("клавиатурной операции"); break;
			default                         : break;
			}
			PARSER->error().error(@2, rdo::format(_T("Ключевое слово Convert_event может быть использовано в событии, но не в %s '%s'"), type.c_str(), pPattern->name().c_str()));
		}

		LPExpression pExpressionConvertBody = PARSER->stack().pop<Expression>($4);
		ASSERT(pExpressionConvertBody);

		rdo::runtime::LPRDOCalcStatementList pCalcStatementList = pExpressionConvertBody->calc().object_dynamic_cast<rdo::runtime::RDOCalcStatementList>();
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

		LPExpression pExpressionConvert = rdo::Factory<Expression>::create(pExpressionConvertBody->typeInfo(), pCalcBaseStatementList, pCalcStatementList->srcInfo());
		ASSERT(pExpressionConvert);

		ASSERT(pPattern->m_pCurrRelRes);
		pPattern->addRelResConvert($3 != 0, pExpressionConvert, @2, @3, pPattern->m_pCurrRelRes->m_statusBegin);

		RDOParser::s_parser()->contextStack()->pop<ContextReturnable>();

		$$ = PARSER->stack().push(pPattern);
	}
	;

convert_rule
	: RDO_Convert_rule
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::convertBegin;

		LPContextReturnable pContextReturnable = rdo::Factory<ContextReturnable>::create();
		ASSERT(pContextReturnable);

		PARSER->contextStack()->push(pContextReturnable);
	}
	;

convert_event
	: RDO_Convert_event
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::convertBegin;

		LPContextReturnable pContextReturnable = rdo::Factory<ContextReturnable>::create();
		ASSERT(pContextReturnable);

		PARSER->contextStack()->push(pContextReturnable);
	}
	;

convert_begin
	: RDO_Convert_begin
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::convertBegin;

		LPContextReturnable pContextReturnable = rdo::Factory<ContextReturnable>::create();
		ASSERT(pContextReturnable);

		PARSER->contextStack()->push(pContextReturnable);
	}
	;

convert_end
	:	RDO_Convert_end
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::convertEnd;

		LPContextReturnable pContextReturnable = rdo::Factory<ContextReturnable>::create();
		ASSERT(pContextReturnable);

		PARSER->contextStack()->push(pContextReturnable);
	}
	;

pat_pattern
	: pat_convert RDO_End
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		pPattern->end();
		$$ = PARSER->stack().push(pPattern);
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Список statement'ов для паттернов
// --------------------------------------------------------------------------------
nochange_statement
	: RDO_IDENTIF_NoChange ';'
	{
		rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOCalcNoChange>::create();
		ASSERT(pCalc);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__void>(RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		$$ = PARSER->stack().push(pExpression);
	}
	| RDO_IDENTIF_NoChange error
	{
		PARSER->error().error(@2, _T("Не найден символ окончания инструкции - точка с запятой"));
	}
	;

stopping_statement
	: RDO_IDENTIF '.' RDO_Stopping '(' ')' ';'
	{
		tstring    eventName = PARSER->stack().pop<RDOValue>($1)->value().getIdentificator();
		LPRDOEvent pEvent    = PARSER->findEvent(eventName);
		if (!pEvent)
		{
			PARSER->error().error(@1, rdo::format(_T("Попытка остановить неизвестное событие: %s"), eventName.c_str()));
		}

		rdo::runtime::LPRDOCalcEventStop pCalc = rdo::Factory<rdo::runtime::RDOCalcEventStop>::create();
		pCalc->setSrcInfo(RDOParserSrcInfo(@1, @6, rdo::format(_T("Остановка события %s"), eventName.c_str())));
		ASSERT(pCalc);
		pEvent->attachCalc(pCalc);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__void>(RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		$$ = PARSER->stack().push(pExpression);
	}
	| RDO_IDENTIF '.' RDO_Stopping '(' ')' error
	{
		PARSER->error().error(@4, _T("Не найден символ окончания инструкции - точка с запятой"));
	}
	;

planning_statement
	: RDO_IDENTIF '.' RDO_Planning '(' expression ')' ';'
	{
		tstring           eventName   = PARSER->stack().pop<RDOValue>($1)->value().getIdentificator();
		LPExpression      pExpression = PARSER->stack().pop<Expression>($5);
		ASSERT(pExpression);

		LPRDOEvent pEvent = PARSER->findEvent(eventName);
		if (!pEvent)
		{
			PARSER->error().error(@1, rdo::format(_T("Попытка запланировать неизвестное событие: %s"), eventName.c_str()));
		}
		rdo::runtime::LPRDOCalcEventPlan pCalc = pEvent->prepareEvent(pExpression);
		pCalc->setSrcInfo(RDOParserSrcInfo(@1, @7, rdo::format(_T("Планирование события %s в момент времени %s"), eventName.c_str(), pExpression->src_info().src_text().c_str())));
		ASSERT(pCalc);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__void>(RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPExpression pEventExpression = rdo::Factory<Expression>::create(pType, pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		LPIBaseOperation pBaseOperation = pEvent->getRuntimeEvent();
		ASSERT(pBaseOperation);

		pCalc->setEvent(pBaseOperation);
		pEvent->setInitCalc(pCalc);
	}
	| RDO_IDENTIF '.' RDO_Planning '(' expression ')' error
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
	| RDO_IDENTIF '.' RDO_Planning '(' expression error
	{
		PARSER->error().error(@6, _T("Ожидается закрывающая скобка"));
	}
	;

process_input_statement
	: RDO_IDENTIF '.' RDO_ProcessStart '(' RDO_IDENTIF_RELRES ')' ';'
	{
		tstring          processName = PARSER->stack().pop<RDOValue>($1)->value().getIdentificator();
		LPRDOPROCProcess pProcess    = PARSER->findPROCProcess(processName);
		if (!pProcess)
		{
			PARSER->error().error(@1, rdo::format(_T("Попытка запустить неизвестный процесс: %s"), processName.c_str()));
		}

		LPIPROCBlock pBlock = (*(pProcess->getBlockList().begin()))->getRuntimeBlock();
		ASSERT(pBlock);

		tstring relResName = PARSER->stack().pop<RDOValue>($5)->value().getIdentificator();

		LPRDOPATPattern pPattern = PARSER->getLastPATPattern();
		ASSERT(pPattern);
		/*из-за использования RDO_IDENTIF_RELRES findRelevantResource() всегда находит ресурс*/
		LPRDORelevantResource pRelRes = pPattern->findRelevantResource(relResName);
		tstring relResTypeName = pRelRes->getType()->name();

		if (!pProcess->checkTransactType(relResTypeName))
		{
			PARSER->error().error(@1, rdo::format(_T("Процесс %s ожидает в качестве транзактов ресурсы типа %s, а не %s"), processName.c_str(), _T("true_resTypeName"), relResTypeName.c_str()));
		}

		rdo::runtime::LPRDOCalcProcessControl pCalc = rdo::Factory<rdo::runtime::RDOCalcProcessControl>::create(pBlock, pRelRes->m_relResID);
		ASSERT(pCalc);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__void>(RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		$$ = PARSER->stack().push(pExpression);
	}
	| RDO_IDENTIF '.' RDO_ProcessStart '(' error ')' ';'
	{
		PARSER->error().error(@5, _T("В качестве транзакта процессу можно передавать только релеватный ресурс"));
	}
	;

watch_start
	: RDO_IDENTIF '.' RDO_WatchStart '(' ')' ';'
	{
		tstring          name         = PARSER->stack().pop<RDOValue>($1)->value().getIdentificator();
		LPRDOResultGroup pResultGroup = PARSER->findResultGroup(name);
		if (!pResultGroup)
		{
			PARSER->error().error(@1, rdo::format(_T("Неизвестная группа показателей: %s"), name.c_str()));
		}

		rdo::runtime::LPRDOCalcWatchGroupStart pCalc = rdo::Factory<rdo::runtime::RDOCalcWatchGroupStart>::create(pResultGroup->getRuntime());
		ASSERT(pCalc);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__void>(RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		$$ = PARSER->stack().push(pExpression);
	}
	;

watch_stop
	: RDO_IDENTIF '.' RDO_WatchStop '(' ')' ';'
	{
		tstring          name         = PARSER->stack().pop<RDOValue>($1)->value().getIdentificator();
		LPRDOResultGroup pResultGroup = PARSER->findResultGroup(name);
		if (!pResultGroup)
		{
			PARSER->error().error(@1, rdo::format(_T("Неизвестная группа показателей: %s"), name.c_str()));
		}

		rdo::runtime::LPRDOCalcWatchGroupStop pCalc = rdo::Factory<rdo::runtime::RDOCalcWatchGroupStop>::create(pResultGroup->getRuntime());
		ASSERT(pCalc);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__void>(RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

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
	| if_else_statement
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
	| nochange_statement
//	| member_statement ';'
	| process_input_statement
	| stopping_statement
	| planning_statement
	| watch_start
	| watch_stop
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
			LPRDORelevantResource pRelRes = PARSER->getLastPATPattern()->m_pCurrRelRes;
			ASSERT(pRelRes);
			LPRDORTPParam pParam = pRelRes->getType()->findRTPParam(paramName);
			if (!pParam)
			{
				PARSER->error().error(@1, rdo::format(_T("Неизвестный параметр: %s"), paramName.c_str()));
			}

			pLeftArithmType = pParam->getTypeInfo();

			switch (equalType)
			{
				case rdo::runtime::ET_INCR:
				{
					pCalc = rdo::Factory<rdo::runtime::RDOSetRelResParamCalc<rdo::runtime::ET_INCR> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName));
					break;
				}
				case rdo::runtime::ET_DECR:
				{
					pCalc = rdo::Factory<rdo::runtime::RDOSetRelResParamCalc<rdo::runtime::ET_DECR> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName));
					break;
				}
				default:
				{
					NEVER_REACH_HERE;
				}
			}
			ASSERT(pCalc);

			//! Проверка на диапазон
			LPRDOTypeIntRange pTypeIntRange = pParam->getTypeInfo()->type().object_dynamic_cast<RDOTypeIntRange>();
			if (pTypeIntRange)
			{
				pCalc = rdo::Factory<rdo::runtime::RDOCalcCheckRange>::create(pTypeIntRange->range()->getMin()->value(), pTypeIntRange->range()->getMax()->value(), pCalc);
			}

			LPRDOTypeRealRange pTypeRealRange = pParam->getTypeInfo()->type().object_dynamic_cast<RDOTypeRealRange>();
			if (pTypeRealRange)
			{
				pCalc = rdo::Factory<rdo::runtime::RDOCalcCheckRange>::create(pTypeRealRange->range()->getMin()->value(), pTypeRealRange->range()->getMax()->value(), pCalc);
			}
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
	| RDO_IDENTIF param_equal_type expression
	{
		LPRDOValue              pParamName       = PARSER->stack().pop<RDOValue>($1);
		tstring                 paramName        = pParamName->value().getIdentificator();
		rdo::runtime::EqualType equalType        = static_cast<rdo::runtime::EqualType>($2);
		LPExpression            pRightExpression = PARSER->stack().pop<Expression>($3);
		ASSERT(pRightExpression);
		LPContext pContext = PARSER->context();
		ASSERT(pContext);
		LPContextMemory pContextMemory = pContext->cast<ContextMemory>();
		ASSERT(pContextMemory);
		LPLocalVariableListStack pLocalVariableListStack = pContextMemory->getLocalMemory();
		ASSERT(pLocalVariableListStack);
		LPLocalVariable pLocalVariable = pLocalVariableListStack->findLocalVariable(paramName);
		rdo::runtime::LPRDOCalc pCalc;
		rdo::runtime::LPRDOCalc pCalcRight;
		LPTypeInfo pLeftExpressionType;
		LPExpression pExpressionLeft;
		if (pLocalVariable)
		{
			pLeftExpressionType = pLocalVariable->getTypeInfo();

			pCalcRight = pRightExpression->calc();
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
			LPRDORelevantResource pRelRes = PARSER->getLastPATPattern()->m_pCurrRelRes;
			ASSERT(pRelRes);
			LPRDORTPParam pParam = pRelRes->getType()->findRTPParam(paramName);
			if (!pParam)
			{
				PARSER->error().error(@1, rdo::format(_T("Неизвестный параметр: %s"), paramName.c_str()));
			}

			pLeftExpressionType = pParam->getTypeInfo();

			pCalcRight = pRightExpression->calc();
			
			switch (equalType)
			{
				case rdo::runtime::ET_NOCHANGE:
				{
					break;
				}
				case rdo::runtime::ET_EQUAL:
				{
					pCalc = rdo::Factory<rdo::runtime::RDOSetRelResParamCalc<rdo::runtime::ET_EQUAL> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pCalcRight);
					ASSERT(pCalc);
					pCalc->setSrcInfo(RDOParserSrcInfo(@1, rdo::format(_T("%s.%s"), pRelRes->src_text().c_str(), paramName.c_str())));

					pExpressionLeft = rdo::Factory<Expression>::create(
						pParam->getTypeInfo(),
						pCalc,
						pCalc->srcInfo()
					);
					ASSERT(pExpressionLeft)

					pRelRes->getParamSetList().insert(pParam);
					break;
				}
				case rdo::runtime::ET_PLUS:
				{
					pCalc = rdo::Factory<rdo::runtime::RDOSetRelResParamCalc<rdo::runtime::ET_PLUS> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pCalcRight);
					break;
				}
				case rdo::runtime::ET_MINUS:
				{
					pCalc = rdo::Factory<rdo::runtime::RDOSetRelResParamCalc<rdo::runtime::ET_MINUS> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pCalcRight);
					break;
				}
				case rdo::runtime::ET_MULTIPLY:
				{
					pCalc = rdo::Factory<rdo::runtime::RDOSetRelResParamCalc<rdo::runtime::ET_MULTIPLY> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pCalcRight);
					break;
				}
				case rdo::runtime::ET_DIVIDE:
				{
					pCalc = rdo::Factory<rdo::runtime::RDOSetRelResParamCalc<rdo::runtime::ET_DIVIDE> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pCalcRight);
					break;
				}
				default:
				{
					NEVER_REACH_HERE;
				}
			}
			ASSERT(pCalc);

			//! Проверка на диапазон
			LPRDOTypeIntRange pTypeIntRange = pParam->getTypeInfo()->type().object_dynamic_cast<RDOTypeIntRange>();
			if (pTypeIntRange)
			{
				pCalc = rdo::Factory<rdo::runtime::RDOCalcCheckRange>::create(pTypeIntRange->range()->getMin()->value(), pTypeIntRange->range()->getMax()->value(), pCalc);
			}

			LPRDOTypeRealRange pTypeRealRange = pParam->getTypeInfo()->type().object_dynamic_cast<RDOTypeRealRange>();
			if (pTypeRealRange)
			{
				pCalc = rdo::Factory<rdo::runtime::RDOCalcCheckRange>::create(pTypeRealRange->range()->getMin()->value(), pTypeRealRange->range()->getMax()->value(), pCalc);
			}
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

		$$ = PARSER->stack().push(pExpressionLeft);
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
			LPRDORelevantResource pRelRes = PARSER->getLastPATPattern()->m_pCurrRelRes;
			ASSERT(pRelRes);
			LPRDORTPParam pParam = pRelRes->getType()->findRTPParam(paramName);
			ASSERT(pParam);

			pCalc = rdo::Factory<rdo::runtime::RDOSetRelResParamCalc<rdo::runtime::ET_EQUAL> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pArrayItemCalc);
			ASSERT(pCalc);
			pCalc->setSrcInfo(RDOParserSrcInfo(@1, rdo::format(_T("%s.%s"), pRelRes->src_text().c_str(), paramName.c_str())));
		}

		LPExpression pExpression = rdo::Factory<Expression>::create(pArrayArithm->typeInfo(), pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		$$ = PARSER->stack().push(pExpression);
	}
	;

local_variable_declaration
	: local_variable_declaration_context init_declaration_list
	{
		LPTypeContext pTypeContext = PARSER->context()->cast<TypeContext>();
		ASSERT(pTypeContext);

		LPTypeInfo pType = pTypeContext->getTypeInfo();
		ASSERT(pType);

		LPContextLocalVariable pContextLocalVariable = PARSER->context().object_dynamic_cast<ContextLocalVariable>();
		ASSERT(pContextLocalVariable);

		LPExpression pExpression = rdo::Factory<Expression>::create(
			pType, pContextLocalVariable->calc(), RDOParserSrcInfo(@1, @2)
		);
		ASSERT(pExpression);

		PARSER->contextStack()->pop<ContextLocalVariable>();
		PARSER->contextStack()->pop<TypeContext>();

		$$ = PARSER->stack().push(pExpression);
	}
	;

local_variable_declaration_context
	: type_declaration_context
	{
		LPContextMemory pContextMemory = PARSER->context()->cast<ContextMemory>();
		ASSERT(pContextMemory);

		LPLocalVariableListStack pLocalVariableListStack = pContextMemory->getLocalMemory();
		ASSERT(pLocalVariableListStack);
		LPLocalVariableList pLocalVariableList = pLocalVariableListStack->top();
		ASSERT(pLocalVariableList);

		LPContextLocalVariable pContextLocalVariable = rdo::Factory<ContextLocalVariable>::create(
			boost::bind(&LocalVariableList::append, pLocalVariableList.get(), _1)
		);
		ASSERT(pContextLocalVariable);
		PARSER->contextStack()->push(pContextLocalVariable);
	}
	;

type_declaration_context
	: type_declaration
	{
		LPTypeInfo pType = PARSER->stack().pop<TypeInfo>($1);
		ASSERT(pType);

		LPContext pTypeContext = rdo::Factory<TypeContext>::create(pType);
		ASSERT(pTypeContext);
		PARSER->contextStack()->push(pTypeContext);
	}
	;

type_declaration
	: RDO_integer
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__int>(RDOParserSrcInfo(@1));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_real
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__real>(RDOParserSrcInfo(@1));
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
		LPTypeInfo pTypeSuchAs = PARSER->stack().pop<TypeInfo>($1);
		ASSERT(pTypeSuchAs);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pTypeSuchAs->type(), RDOParserSrcInfo(@1));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	;

init_declaration_list
	: init_declaration_list_item
	| init_declaration_list ',' init_declaration_list_item
	;

init_declaration_list_item
	: RDO_IDENTIF init_declaration_value
	{
		LPRDOValue pVariableName = PARSER->stack().pop<RDOValue>($1);
		ASSERT(pVariableName);

		LPExpression pExpression = PARSER->stack().pop<Expression>($2);
		ASSERT(pExpression);

		pExpression->setSrcInfo(RDOParserSrcInfo(@1, @2, pVariableName->src_text()));

		LPLocalVariable pLocalVariable = rdo::Factory<LocalVariable>::create(pVariableName, pExpression);
		ASSERT(pLocalVariable);

		LPContextLocalVariable pContextLocalVariable = PARSER->context().object_dynamic_cast<ContextLocalVariable>();
		ASSERT(pContextLocalVariable);
		pContextLocalVariable->pushLocalVariable(pLocalVariable);
	}
	;

init_declaration_value
	: /* empty */
	{
		LPTypeContext pTypeContext = PARSER->context()->cast<TypeContext>();
		ASSERT(pTypeContext);

		LPTypeInfo pTypeInfo = pTypeContext->getTypeInfo();
		ASSERT(pTypeInfo);

		LPExpression pExpression = rdo::Factory<Expression>::create(
			pTypeInfo,
			rdo::Factory<rdo::runtime::RDOCalcConst>::create(pTypeInfo->type()->get_default()),
			RDOParserSrcInfo()
		);
		ASSERT(pExpression);
		$$ = PARSER->stack().push(pExpression);
	}
	| '=' fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm->expression());
	}
	;

if_else_statement
	: if_statement
	{
		PARSER->contextStack()->pop<ContextReturnable>();
	}
	| if_statement RDO_else statement
	{
		LPExpression pExpression = PARSER->stack().pop<Expression>($1);
		ASSERT(pExpression);

		rdo::runtime::LPRDOCalcIf pCalc = pExpression->calc().object_dynamic_cast<rdo::runtime::RDOCalcIf>();
		ASSERT(pCalc);

		if (!pCalc->hasElse())
		{
			LPExpression pExpressionStatement = PARSER->stack().pop<Expression>($3);
			ASSERT(pExpressionStatement);

			rdo::runtime::LPRDOCalc pCalcStatement = pExpressionStatement->calc();
			ASSERT(pCalcStatement);

			pCalc->setElseStatement(pCalcStatement);

			LPContextReturnable pContextReturnable = PARSER->context()->cast<ContextReturnable>();
			ASSERT(pContextReturnable);
			pContextReturnable->addChildContext();

			$$ = PARSER->stack().push(pExpression);
		}
		else
		{
			PARSER->error().error(@2, rdo::format(_T("С одним If нельзя использовать больше одного Else")));
		}
	}
	;

if_statement
	: if_header statement
	{
		LPExpression pExpression = PARSER->stack().pop<Expression>($1);
		ASSERT(pExpression);

		LPExpression pExpressionStatement = PARSER->stack().pop<Expression>($2);
		ASSERT(pExpressionStatement);

		rdo::runtime::LPRDOCalcIf pCalc = pExpression->calc().object_dynamic_cast<rdo::runtime::RDOCalcIf>();
		ASSERT(pCalc);

		rdo::runtime::LPRDOCalc pCalcStatement = pExpressionStatement->calc();
		ASSERT(pCalcStatement);

		pCalc->setThenStatement(pCalcStatement);

		LPContextReturnable pContextReturnable = PARSER->context()->cast<ContextReturnable>();
		ASSERT(pContextReturnable);
		pContextReturnable->addChildContext();

		$$ = PARSER->stack().push(pExpression);
	}
	;

if_header
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

		PARSER->contextStack()->pop<ContextBreakable>();

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
			PARSER->error().error(src_info, _T("Не указано значение по умолчанию"));
		}
		else
		{
			PARSER->error().error(src_info, _T("Неверное значение по умолчанию"));
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
	: RDO_REAL_CONST                     { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($1))); }
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
	| RDO_IDENTIF '[' fun_arithm ']' '.' RDO_IDENTIF
	{
		LPRDOValue pArrayValue = PARSER->stack().pop<RDOValue>($1);
		ASSERT(pArrayValue);

		LPRDOFUNArithm pArrayArithm = RDOFUNArithm::generateByIdentificator(pArrayValue);
		ASSERT(pArrayArithm);

		LPRDOArrayType pArrayType = pArrayArithm->typeInfo()->type().object_dynamic_cast<RDOArrayType>();
		if (!pArrayType)
		{
			PARSER->error().error(@1, rdo::format(_T("'%s' не является массивом")
				, pArrayValue->value().getIdentificator().c_str())
			);
		}

		LPRDORTPResType pResType = pArrayType->getItemType()->type().object_dynamic_cast<RDORTPResType>();
		if (!pResType)
		{
			PARSER->error().error(@1, rdo::format(_T("'%s' не является массивом ресурсов")
				, pArrayValue->value().getIdentificator().c_str())
			);
		}

		LPRDOFUNArithm pArrayIndex = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArrayIndex);

		LPRDOValue pParamName = PARSER->stack().pop<RDOValue>($6);
		ASSERT(pParamName);

		rsint paramIndex = pResType->getRTPParamNumber(pParamName->value().getAsString());

		if (paramIndex == RDORTPResType::UNDEFINED_PARAM)
		{
			PARSER->error().error(@6, rdo::format(_T("'%s' не является параметром ресурса '%s'")
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
// -------------------- Замена арифметическх выражений, Экспрешены
// --------------------------------------------------------------------------------
expression
	: RDO_INT_CONST                      { $$ = PARSER->stack().push(ExpressionGenerator::generateByConst(PARSER->stack().pop<RDOValue>($1))); }
	| expression ',' expression
	{
		LPExpression pFirstEpression    = PARSER->stack().pop<Expression>($1);
		LPExpression pSecondExpression  = PARSER->stack().pop<Expression>($3);
		ASSERT (pFirstEpression  );
		ASSERT (pSecondExpression);
		LPExpressionList pExpressionList = pFirstEpression.object_dynamic_cast<ExpressionList>();
		if(pExpressionList)
		{
			pExpressionList->addItem(pSecondExpression);
		}
		else
		{
			pExpressionList = rdo::Factory<ExpressionList>::create();
			pExpressionList->addItem(pFirstEpression  );
			pExpressionList->addItem(pSecondExpression);
		}
		
		$$ = PARSER->stack().push(pExpressionList);
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
	| fun_select_body '.' RDO_Select_Array '(' ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		pSelect->setSrcPos(@1, @5);
		RDOParserSrcInfo arrayInfo(@3, @5, _T("Array()"));
		LPRDOFUNArithm pArithm = pSelect->createFunSelectArray(arrayInfo);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
	| fun_select_body '.' RDO_Select_Array error
	{
		PARSER->error().error(@3, _T("Ожидается октрывающаяся скобка"));
	}
	| fun_select_body '.' RDO_Select_Array '(' error
	{
		PARSER->error().error(@4, _T("Ожидается закрывающаяся скобка"));
	}
	;

%%

CLOSE_RDO_PARSER_NAMESPACE

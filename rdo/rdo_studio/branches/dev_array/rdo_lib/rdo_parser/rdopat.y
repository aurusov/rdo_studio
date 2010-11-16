/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdopat.y
 * author   : Александ Барс, Урусов Андрей, Лущан Дмитрий
 * date     : 20.02.2003
 * bref     : Синтаксис образцов активностей
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

%token RDO_Model_name
%token RDO_Resource_file
%token RDO_OprIev_file
%token RDO_Frame_file
%token RDO_Statistic_file
%token RDO_Results_file
%token RDO_Trace_file
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
#include "rdo_lib/rdo_parser/rdopat.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_parser/rdo_type_range.h"
#include "rdo_lib/rdo_parser/rdo_array.h"
#include "rdo_lib/rdo_parser/local_variable.h"
#include "rdo_lib/rdo_parser/context/type.h"
#include "rdo_lib/rdo_parser/context/pattern.h"

#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/calc_event_plan.h"
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

pat_main
	: /* empty */
	| pat_main pat_pattern
	| error
	{
		PARSER->error().error(@1, _T("Неизвестная ошибка"));
	}
	;

pat_header
	: RDO_Pattern RDO_IDENTIF_COLON RDO_operation pat_trace 
	{
		PTR(RDOValue) name = P_RDOVALUE($2);
		LPRDOPATPattern pPattern = rdo::Factory<RDOPatternOperation>::create(name->src_info(), $4 != 0);
		ASSERT(pPattern);
		$$ = PARSER->stack().push(pPattern);
	}
	| RDO_Pattern RDO_IDENTIF_COLON RDO_rule pat_trace
	{
		PTR(RDOValue) name = P_RDOVALUE($2);
		LPRDOPATPattern pPattern = rdo::Factory<RDOPatternRule>::create(name->src_info(), $4 != 0);
		ASSERT(pPattern);
		$$ = PARSER->stack().push(pPattern);
	}
	| RDO_Pattern RDO_IDENTIF_COLON RDO_keyboard pat_trace
	{
		PTR(RDOValue) name = P_RDOVALUE($2);
		LPRDOPATPattern pPattern = rdo::Factory<RDOPatternKeyboard>::create(name->src_info(), $4 != 0);
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
	: pat_params_begin RDO_IDENTIF_COLON param_type
	{
		LPRDOPATPattern          pPattern   = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		PTR(RDOValue)            param_name = P_RDOVALUE($2);
		LPRDOTypeParam           param_type = PARSER->stack().pop<RDOTypeParam>($3);
		LPRDOFUNFunctionParam    pParam     = rdo::Factory<RDOFUNFunctionParam>::create(param_name->src_info(), param_type);
		pPattern->add(pParam);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params RDO_IDENTIF_COLON param_type
	{
		LPRDOPATPattern          pPattern   = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		PTR(RDOValue)            param_name = P_RDOVALUE($2);
		LPRDOTypeParam           param_type = PARSER->stack().pop<RDOTypeParam>($3);
		LPRDOFUNFunctionParam    pParam     = rdo::Factory<RDOFUNFunctionParam>::create(param_name->src_info(), param_type);
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
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(rel_name->src_info(), type_name->src_info(), (rdoRuntime::RDOResource::ConvertStatus)$4, (rdoRuntime::RDOResource::ConvertStatus)$5, @4, @5);
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
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(rel_name->src_info(), type_name->src_info(), (rdoRuntime::RDOResource::ConvertStatus)$4, (rdoRuntime::RDOResource::ConvertStatus)$5, @4, @5);
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
				PARSER->error().error(@4, rdo::format(_T("Помимо статуса конвертора начала (%s), ожидается статус конвертора конца, потому что у операции есть событие конца"), RDOPATPattern::StatusToStr((rdoRuntime::RDOResource::ConvertStatus)$4).c_str()));
				break;
			}
			case RDOPATPattern::PT_Event: 
			case RDOPATPattern::PT_Rule :
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				pPattern->addRelRes(rel_name->src_info(), type_name->src_info(), (rdoRuntime::RDOResource::ConvertStatus)$4, @4);
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
				PARSER->error().error(@4, rdo::format(_T("Помимо статуса конвертора начала (%s), ожидается статус конвертора конца, потому что у операции есть событие конца"), RDOPATPattern::StatusToStr((rdoRuntime::RDOResource::ConvertStatus)$4).c_str()));
				break;
			}
			case RDOPATPattern::PT_Event: 
			case RDOPATPattern::PT_Rule :
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				pPattern->addRelRes(rel_name->src_info(), type_name->src_info(), (rdoRuntime::RDOResource::ConvertStatus)$4, @4);
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
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				YYLTYPE convertor_pos = @3;
				convertor_pos.m_first_line = convertor_pos.m_last_line;
				convertor_pos.m_first_pos  = convertor_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_pos.m_first_seek = convertor_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, (rdoRuntime::RDOResource::ConvertStatus)$4, convertor_pos, @4);
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
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				YYLTYPE convertor_pos = @3;
				convertor_pos.m_first_line = convertor_pos.m_last_line;
				convertor_pos.m_first_pos  = convertor_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_pos.m_first_seek = convertor_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, (rdoRuntime::RDOResource::ConvertStatus)$4, convertor_pos, @4);
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
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				YYLTYPE convertor_begin_pos = @3;
				tstring str = LEXER->YYText();
				rdo::toLower(str);
				tstring::size_type first_nochange = str.find(_T("nochange"));
				int i = 0;
				while (true)
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
				convertor_begin_pos.m_last_pos  = convertor_begin_pos.m_first_pos  + RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_begin_pos.m_last_seek = convertor_begin_pos.m_first_seek + RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				YYLTYPE convertor_end_pos = @3;
				convertor_end_pos.m_first_line = convertor_end_pos.m_last_line;
				convertor_end_pos.m_first_pos  = convertor_end_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_end_pos.m_first_seek = convertor_end_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, rdoRuntime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos);
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
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				YYLTYPE convertor_begin_pos = @3;
				tstring str = LEXER->YYText();
				rdo::toLower(str);
				tstring::size_type first_nochange = str.find(_T("nochange"));
				int i = 0;
				while (true)
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
				convertor_begin_pos.m_last_pos  = convertor_begin_pos.m_first_pos  + RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_begin_pos.m_last_seek = convertor_begin_pos.m_first_seek + RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				YYLTYPE convertor_end_pos = @3;
				convertor_end_pos.m_first_line = convertor_end_pos.m_last_line;
				convertor_end_pos.m_first_pos  = convertor_end_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_end_pos.m_first_seek = convertor_end_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, rdoRuntime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos);
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
				PARSER->error().error(@3, rdo::format(_T("Помимо статуса конвертора начала (%s), ожидается статус конвертора конца, потому что у операции есть событие конца"), RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).c_str()));
				break;
			}
			case RDOPATPattern::PT_Event: 
			case RDOPATPattern::PT_Rule :
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				YYLTYPE convertor_pos = @3;
				convertor_pos.m_first_line = convertor_pos.m_last_line;
				convertor_pos.m_first_pos  = convertor_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_pos.m_first_seek = convertor_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, convertor_pos);
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
				PARSER->error().error(@3, rdo::format(_T("Помимо статуса конвертора начала (%s), ожидается статус конвертора конца, потому что у операции есть событие конца"), RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).c_str()));
				break;
			}
			case RDOPATPattern::PT_Event: 
			case RDOPATPattern::PT_Rule :
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				YYLTYPE convertor_pos = @3;
				convertor_pos.m_first_line = convertor_pos.m_last_line;
				convertor_pos.m_first_pos  = convertor_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_pos.m_first_seek = convertor_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, convertor_pos);
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
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				tstring convert_begin = RDOVALUE($4)->getIdentificator();
				YYLTYPE convertor_begin_pos = @4;
				convertor_begin_pos.m_last_line = convertor_begin_pos.m_first_line;
				convertor_begin_pos.m_last_pos  = convertor_begin_pos.m_first_pos  + convert_begin.length();
				convertor_begin_pos.m_last_seek = convertor_begin_pos.m_first_seek + convert_begin.length();
				YYLTYPE convertor_end_pos = @4;
				convertor_end_pos.m_first_line = convertor_end_pos.m_last_line;
				convertor_end_pos.m_first_pos  = convertor_end_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_end_pos.m_first_seek = convertor_end_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(rel_name->src_info(), type_name->src_info(), pPattern->StrToStatus(convert_begin, convertor_begin_pos), rdoRuntime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos);
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
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				tstring convert_begin = RDOVALUE($4)->getIdentificator();
				YYLTYPE convertor_begin_pos = @4;
				convertor_begin_pos.m_last_line = convertor_begin_pos.m_first_line;
				convertor_begin_pos.m_last_pos  = convertor_begin_pos.m_first_pos  + convert_begin.length();
				convertor_begin_pos.m_last_seek = convertor_begin_pos.m_first_seek + convert_begin.length();
				YYLTYPE convertor_end_pos = @4;
				convertor_end_pos.m_first_line = convertor_end_pos.m_last_line;
				convertor_end_pos.m_first_pos  = convertor_end_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_end_pos.m_first_seek = convertor_end_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(rel_name->src_info(), type_name->src_info(), pPattern->StrToStatus(convert_begin, convertor_begin_pos), rdoRuntime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos);
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
	: RDO_Keep			{ $$ = rdoRuntime::RDOResource::CS_Keep;     }
	| RDO_Create		{ $$ = rdoRuntime::RDOResource::CS_Create;   }
	| RDO_Erase			{ $$ = rdoRuntime::RDOResource::CS_Erase;    }
	| RDO_NonExist		{ $$ = rdoRuntime::RDOResource::CS_NonExist; }
	;

pat_common_choice
	: pat_rel_res
	| pat_rel_res RDO_first
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		pPattern->setCommonChoiceFirst();
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_with_min fun_arithm
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm);
		pArithm->setSrcPos (@2, @3);
		pArithm->setSrcText(_T("with_min ") + pArithm->src_text());
		pPattern->setCommonChoiceWithMin(pArithm);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_with_max fun_arithm
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm);
		pArithm->setSrcPos (@2, @3);
		pArithm->setSrcText(_T("with_max ") + pArithm->src_text());
		pPattern->setCommonChoiceWithMax(pArithm);
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
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@2, _T("Перед $Body пропущено ключевое слово $Time"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_common_choice RDO_Time '=' fun_arithm RDO_Body
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@2, _T("Поле $Time не используется в событии"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@2, _T("Поле $Time не используется в продукционном правиле"));
				break;
			}
		}
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($4);
		ASSERT(pArithm);
		pArithm->setSrcPos (@2, @4);
		pArithm->setSrcText(_T("$Time = ") + pArithm->src_text());
		pPattern->setTime(pArithm);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_common_choice RDO_Time '=' fun_arithm error
	{
		PARSER->error().error(@4, @5, _T("Ожидается ключевое слово $Body"));
	}
	| pat_common_choice RDO_Time '=' error
	{
		PARSER->error().error(@4, _T("Ошибка в арифметическом выражении"));
	}
	| pat_common_choice RDO_Time error
	{
		PARSER->error().error(@2, @3, _T("После ключевого слова $Time ожидается знак равенства"));
	}
	| pat_common_choice error
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@2, rdo::format(_T("Ожидается $Body, найдено: %s"), LEXER->YYText()));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@2, rdo::format(_T("Ожидается $Body, найдено: %s"), LEXER->YYText()));
				break;
			}
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@2, rdo::format(_T("Ожидается $Time, найдено: %s"), LEXER->YYText()));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	;

pat_body
	: pat_time RDO_IDENTIF_RELRES
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		tstring         name     = RDOVALUE($2)->getIdentificator();
		pPattern->addRelResBody(RDOParserSrcInfo(@2, name));
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_convert RDO_IDENTIF_RELRES
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		tstring         name    = RDOVALUE($2)->getIdentificator();
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
		LPRDOPATChoiceOrder pChoiceOrder = rdo::Factory<RDOPATChoiceOrder>::create(RDOParserSrcInfo(), rdoRuntime::RDOSelectResourceCalc::order_empty);
		ASSERT(pChoiceOrder);
		$$ = PARSER->stack().push(pChoiceOrder);
	}
	| pat_choice_first
	{
		LPRDOPATChoiceOrder pChoiceOrder = rdo::Factory<RDOPATChoiceOrder>::create(RDOParserSrcInfo(_T("first")), rdoRuntime::RDOSelectResourceCalc::order_first);
		ASSERT(pChoiceOrder);
		$$ = PARSER->stack().push(pChoiceOrder);
	}
	| pat_choice_with_min fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		LPRDOPATChoiceOrder pChoiceOrder = rdo::Factory<RDOPATChoiceOrder>::create(RDOParserSrcInfo(_T("with_min ") + pArithm->src_text()), rdoRuntime::RDOSelectResourceCalc::order_with_min, pArithm);
		ASSERT(pChoiceOrder);
		$$ = PARSER->stack().push(pChoiceOrder);
	}
	| pat_choice_with_max fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		LPRDOPATChoiceOrder pChoiceOrder = rdo::Factory<RDOPATChoiceOrder>::create(RDOParserSrcInfo(_T("with_max ") + pArithm->src_text()), rdoRuntime::RDOSelectResourceCalc::order_with_max, pArithm);
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
		if (rel_res->m_pChoiceOrder->m_type != rdoRuntime::RDOSelectResourceCalc::order_empty)
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
		if (rel_res->m_statusBegin != rdoRuntime::RDOResource::CS_NoChange && rel_res->m_statusBegin != rdoRuntime::RDOResource::CS_Erase && rel_res->m_statusBegin != rdoRuntime::RDOResource::CS_NonExist)
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
		if (rel_res->m_statusEnd != rdoRuntime::RDOResource::CS_NoChange && rel_res->m_statusEnd != rdoRuntime::RDOResource::CS_Erase && rel_res->m_statusEnd != rdoRuntime::RDOResource::CS_NonExist)
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
	| pat_res_usage convert_begin pat_trace pat_convert_cmd
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		if (pPattern->getType() != RDOPATPattern::PT_Operation && pPattern->getType() != RDOPATPattern::PT_Keyboard)
		{
			tstring type = _T("");
			switch (pPattern->getType())
			{
				case RDOPATPattern::PT_Rule:
				{
					type = _T("продукционном правиле");
					break;
				}
			}
			PARSER->error().error(@2, rdo::format(_T("Ключевое слово Convert_begin может быть использовано в обыкновенной или клавиатурной операции, но не в %s '%s'"), type.c_str(), pPattern->name().c_str()));
		}
		LPConvertCmdList pCmdList = PARSER->stack().pop<ConvertCmdList>($4);
		pPattern.object_static_cast<RDOPatternOperation>()->addRelResConvertBeginEnd($3 != 0, pCmdList, false, NULL, @2, @2, @3, @3);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_res_usage convert_end pat_trace pat_convert_cmd
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		if (pPattern->getType() != RDOPATPattern::PT_Operation && pPattern->getType() != RDOPATPattern::PT_Keyboard)
		{
			tstring type = _T("");
			switch (pPattern->getType())
			{
				case RDOPATPattern::PT_Rule:
				{
					type = _T("продукционном правиле");
					break;
				}
			}
			PARSER->error().error(@2, rdo::format(_T("Ключевое слово Convert_end может быть использовано в обыкновенной и клавиатурной операции, но не в %s '%s'"), type.c_str(), pPattern->name().c_str()));
		}
		LPConvertCmdList pCmdList = PARSER->stack().pop<ConvertCmdList>($4);
		pPattern.object_static_cast<RDOPatternOperation>()->addRelResConvertBeginEnd(false, NULL, $3 != 0, pCmdList, @2, @2, @3, @3);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_res_usage convert_begin pat_trace pat_convert_cmd convert_end pat_trace pat_convert_cmd
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		if (pPattern->getType() != RDOPATPattern::PT_Operation && pPattern->getType() != RDOPATPattern::PT_Keyboard)
		{
			tstring type = _T("");
			switch (pPattern->getType())
			{
				case RDOPATPattern::PT_Rule:
				{
					type = _T("продукционном правиле");
					break;
				}
			}
			PARSER->error().error(@2, rdo::format(_T("Ключевые слова Convert_begin и Convert_end могут быть использованы в обыкновенной и клавиатурной операции, но не в %s '%s'"), type.c_str(), pPattern->name().c_str()));
		}
		LPConvertCmdList pCmdListBegin = PARSER->stack().pop<ConvertCmdList>($4);
		LPConvertCmdList pCmdListEnd   = PARSER->stack().pop<ConvertCmdList>($7);
		pPattern.object_static_cast<RDOPatternOperation>()->addRelResConvertBeginEnd($3 != 0, pCmdListBegin, $6 != 0, pCmdListEnd, @2, @5, @3, @6);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_res_usage convert_rule pat_trace pat_convert_cmd
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		if (pPattern->getType() != RDOPATPattern::PT_Rule)
		{
			tstring type = _T("");
			switch (pPattern->getType())
			{
				case RDOPATPattern::PT_Operation:
				{
					type = _T("операции");
					break;
				}
				case RDOPATPattern::PT_Keyboard :
				{
					type = _T("клавиатурной операции");
					break;
				}
			}
			PARSER->error().error(@2, rdo::format(_T("Ключевое слово Convert_rule может быть использовано в продукционном правиле, но не в %s '%s'"), type.c_str(), pPattern->name().c_str()));
		}
		LPConvertCmdList pCmdList = PARSER->stack().pop<ConvertCmdList>($4);
		ASSERT(pPattern->m_pCurrRelRes);
		pPattern->addRelResConvert($3 != 0, pCmdList, @2, @3, pPattern->m_pCurrRelRes->m_statusBegin);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_res_usage convert_event pat_trace pat_convert_cmd
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		tstring type = _T("");
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Rule     :
			{
				type = _T("продукционном правиле");
				break;
			}
			case RDOPATPattern::PT_Operation:
			{
				type = _T("операции");
				break;
			}
			case RDOPATPattern::PT_Keyboard :
			{
				type = _T("клавиатурной операции");
				break;
			}
		}
		PARSER->error().error(@2, rdo::format(_T("Ключевое слово Convert_event может быть использовано в событии, но не в %s '%s'"), type.c_str(), pPattern->name().c_str()));

		LPConvertCmdList pCmdList = PARSER->stack().pop<ConvertCmdList>($4);
		ASSERT(pPattern->m_pCurrRelRes);
		pPattern->addRelResConvert($3 != 0, pCmdList, @2, @3, pPattern->m_pCurrRelRes->m_statusBegin);
		$$ = PARSER->stack().push(pPattern);
	}
	;

convert_rule
	: RDO_Convert_rule
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::convertBegin;
	}
	;

convert_event
	: RDO_Convert_event
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::convertBegin;
	}
	;

convert_begin
	: RDO_Convert_begin
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::convertBegin;
	}
	;

convert_end
	:	RDO_Convert_end
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::convertEnd;
	}
	;

pat_convert_cmd
	: /* empty */
	{
		LPConvertCmdList pCmdList = rdo::Factory<ConvertCmdList>::create();
		LPRDORelevantResource pRelRes = PARSER->getLastPATPattern()->m_pCurrRelRes;
		ASSERT(pRelRes);
		pRelRes->getParamSetList().reset();
		$$ = PARSER->stack().push(pCmdList);
	}
	| pat_convert_cmd statement
	{
		LPConvertCmdList      pCmdList = PARSER->stack().pop<ConvertCmdList>($1);
		rdoRuntime::LPRDOCalc pCalc    = PARSER->stack().pop<rdoRuntime::RDOCalc>($2);
		pCmdList->insertCommand(pCalc);
		$$ = PARSER->stack().push(pCmdList);
	}
	| pat_convert_cmd statement_old_style
	{
		LPConvertCmdList pCmdList = PARSER->stack().pop<ConvertCmdList>($1);
		YYLTYPE          statement_pos = @2;
		LPCorrection     pCorrection = new Correction(++statement_pos.m_last_line, ++statement_pos.m_last_pos, Semicolon);
		pCmdList->insertCorrection(pCorrection);
		$$ = PARSER->stack().push(pCmdList);
		PARSER->error().error(@2, rdo::format(_T("В позиции Ln %i Col %i не хватает точки с запятой"), statement_pos.m_last_line, statement_pos.m_last_pos));
	}
	;

statement_old_style
	: nochange_statement_old_style
	| equal_statement_old_style
	;

nochange_statement_old_style
	: RDO_IDENTIF_NoChange
	;

equal_statement_old_style
	: RDO_IDENTIF increment_or_decrement_type
	{
		$$ = $2;
	}
	| RDO_IDENTIF param_equal_type fun_arithm
	{
		$$ = $3;
	}
	;

statement
	: empty_statement
	| nochange_statement
	| equal_statement
	| stopping_statement
	| planning_statement
	| local_variable_declaration
	| if_statement
	| '{' statement_list '}'
	{
		rdoRuntime::LPRDOCalcList pCalcList = PARSER->stack().pop<rdoRuntime::RDOCalcList>($2);
		ASSERT(pCalcList);
		rdoRuntime::LPRDOCalc pCalc = pCalcList;
		ASSERT(pCalc);
		$$ = PARSER->stack().push(pCalc);
	}
	;

statement_list
	: /* empty */
	{
		rdoRuntime::LPRDOCalcList pCalcList = rdo::Factory<rdoRuntime::RDOCalcList>::create();
		ASSERT(pCalcList);
		$$ = PARSER->stack().push(pCalcList);
	}
	| statement_list statement
	{
		rdoRuntime::LPRDOCalcList pCalcList = PARSER->stack().pop<rdoRuntime::RDOCalcList>($1);
		ASSERT(pCalcList);

		rdoRuntime::LPRDOCalc     pCalc     = PARSER->stack().pop<rdoRuntime::RDOCalc>($2);
		ASSERT(pCalc);

		pCalcList->addCalc(pCalc);

		$$ = PARSER->stack().push(pCalcList);
	}
	;

empty_statement
	: ';'
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcNoChange>::create();
		ASSERT(pCalc);
		$$ = PARSER->stack().push(pCalc);
	}
	| error ';'
	{
		PARSER->error().error(@1, _T("Ошибка в инструкции"));
	}
	;

nochange_statement
	: RDO_IDENTIF_NoChange ';'
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcNoChange>::create();
		ASSERT(pCalc);
		$$ = PARSER->stack().push(pCalc);
	}
	| RDO_IDENTIF_NoChange error
	{
		PARSER->error().error(@2, _T("Не найден символ окончания инструкции - точка с запятой"));
	}
	;

equal_statement
	: RDO_IDENTIF increment_or_decrement_type ';'
	{
		tstring                paramName = RDOVALUE($1)->getIdentificator();
		rdoRuntime::EqualType  equalType = static_cast<rdoRuntime::EqualType>($2);
		LPRDORelevantResource  pRelRes   = PARSER->getLastPATPattern()->m_pCurrRelRes;
		ASSERT(pRelRes);
		LPRDORTPParam param = pRelRes->getType()->findRTPParam(paramName);
		if (!param)
		{
			PARSER->error().error(@1, rdo::format(_T("Неизвестный параметр: %s"), paramName.c_str()));
		}
		rdoRuntime::LPRDOCalc pCalc;
		switch (equalType)
		{
			case rdoRuntime::ET_INCR:
			{
				pCalc = rdo::Factory<rdoRuntime::RDOSetRelParamCalc<rdoRuntime::ET_INCR> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName));
				break;
			}
			case rdoRuntime::ET_DECR:
			{
				pCalc = rdo::Factory<rdoRuntime::RDOSetRelParamCalc<rdoRuntime::ET_DECR> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName));
				break;
			}
			default:
			{
				NEVER_REACH_HERE;
			}
		}
		ASSERT(pCalc);
		//! Проверка на диапазон
		//! TODO: проверить работоспособность
		if (dynamic_cast<PTR(RDOTypeIntRange)>(param->getParamType().get()))
		{
			LPRDOTypeIntRange pRange = param->getParamType()->type().object_static_cast<RDOTypeIntRange>();
			pCalc = rdo::Factory<rdoRuntime::RDOSetRelParamDiapCalc>::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pRange->range()->getMin().value(), pRange->range()->getMax().value(), pCalc);
		}
		else if (dynamic_cast<PTR(RDOTypeRealRange)>(param->getParamType().get()))
		{
			LPRDOTypeRealRange pRange = param->getParamType()->type().object_static_cast<RDOTypeRealRange>();
			pCalc = rdo::Factory<rdoRuntime::RDOSetRelParamDiapCalc>::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pRange->range()->getMin().value(), pRange->range()->getMax().value(), pCalc);
		}
		tstring oprStr;
		switch (equalType)
		{
			case rdoRuntime::ET_INCR:
			{
				oprStr = _T("++");
				break;
			}
			case rdoRuntime::ET_DECR:
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
		pCalc->setSrcText(rdo::format(_T("%s %s %s"), paramName.c_str(), oprStr.c_str()));
		pCalc->setSrcPos (@1.m_first_line, @1.m_first_pos, @2.m_last_line, @2.m_last_pos);

		$$ = PARSER->stack().push(pCalc);
	}
	| RDO_IDENTIF param_equal_type fun_arithm ';'
	{
		tstring               paramName    = RDOVALUE($1)->getIdentificator();
		rdoRuntime::EqualType equalType    = static_cast<rdoRuntime::EqualType>($2);
		LPRDOFUNArithm        pRightArithm = PARSER->stack().pop<RDOFUNArithm>($3);
		LPRDORelevantResource pRelRes      = PARSER->getLastPATPattern()->m_pCurrRelRes;
		ASSERT(pRelRes);		
		LPContext pContext = PARSER->context();
		ASSERT(pContext);
		LPRDORTPParam param = pRelRes->getType()->findRTPParam(paramName);
		if (!param)
		{
			PARSER->error().error(@1, rdo::format(_T("Неизвестный параметр: %s"), paramName.c_str()));
		}
		rdoRuntime::LPRDOCalc pCalcRight = pRightArithm->createCalc(param->getParamType().get());
		rdoRuntime::LPRDOCalc pCalc;
		switch (equalType)
		{
			case rdoRuntime::ET_NOCHANGE:
			{
				break;
			}
			case rdoRuntime::ET_EQUAL:
			{
				pCalc = rdo::Factory<rdoRuntime::RDOSetRelParamCalc<rdoRuntime::ET_EQUAL> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pCalcRight);
				pRelRes->getParamSetList().insert(param);
				break;
			}
			case rdoRuntime::ET_PLUS:
			{
				pCalc = rdo::Factory<rdoRuntime::RDOSetRelParamCalc<rdoRuntime::ET_PLUS> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pCalcRight);
				break;
			}
			case rdoRuntime::ET_MINUS:
			{
				pCalc = rdo::Factory<rdoRuntime::RDOSetRelParamCalc<rdoRuntime::ET_MINUS> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pCalcRight);
				break;
			}
			case rdoRuntime::ET_MULTIPLY:
			{
				pCalc = rdo::Factory<rdoRuntime::RDOSetRelParamCalc<rdoRuntime::ET_MULTIPLY> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pCalcRight);
				break;
			}
			case rdoRuntime::ET_DIVIDE:
			{
				pCalc = rdo::Factory<rdoRuntime::RDOSetRelParamCalc<rdoRuntime::ET_DIVIDE> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pCalcRight);
				break;
			}
			default:
			{
				NEVER_REACH_HERE;
			}
		}
		ASSERT(pCalc);
		//! Проверка на диапазон
		//! TODO: проверить работоспособность
		if (dynamic_cast<PTR(RDOTypeIntRange)>(param->getParamType().get()))
		{
			LPRDOTypeIntRange pRange = param->getParamType()->type().object_static_cast<RDOTypeIntRange>();
			pCalc = rdo::Factory<rdoRuntime::RDOSetRelParamDiapCalc>::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pRange->range()->getMin().value(), pRange->range()->getMax().value(), pCalc);
		}
		else if (dynamic_cast<PTR(RDOTypeRealRange)>(param->getParamType().get()))
		{
			LPRDOTypeRealRange pRange = param->getParamType()->type().object_static_cast<RDOTypeRealRange>();
			pCalc = rdo::Factory<rdoRuntime::RDOSetRelParamDiapCalc>::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pRange->range()->getMin().value(), pRange->range()->getMax().value(), pCalc);
		}
		tstring oprStr;
		switch (equalType)
		{
			case rdoRuntime::ET_EQUAL:
			{
				oprStr = _T("=");
				break;
			}
			case rdoRuntime::ET_PLUS:
			{
				oprStr = _T("+=");
				break;
			}
			case rdoRuntime::ET_MINUS:
			{
				oprStr = _T("-=");
				break;
			}
			case rdoRuntime::ET_MULTIPLY:
			{
				oprStr = _T("*=");
				break;
			}
			case rdoRuntime::ET_DIVIDE:
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
		pCalc->setSrcText(rdo::format(_T("%s %s %s"), paramName.c_str(), oprStr.c_str(), pCalcRight->src_text().c_str()));
		pCalc->setSrcPos (@1.m_first_line, @1.m_first_pos, @3.m_last_line, @3.m_last_pos);

		$$ = PARSER->stack().push(pCalc);
	}
	| RDO_IDENTIF param_equal_type error
	{
		PARSER->error().error(@3, _T("Ошибка в арифметическом выражении"));
	}
	| RDO_IDENTIF param_equal_type fun_arithm error
	{
		PARSER->error().error(@4, _T("Не найден символ окончания инструкции - точка с запятой"));
	}
	| RDO_IDENTIF error fun_arithm
	{
		PARSER->error().error(@2, _T("Ошибка в операторе присваивания"));
	}
	;

stopping_statement
	: RDO_IDENTIF '.' RDO_Stopping ';'
	{
		tstring           eventName   = RDOVALUE($1)->getIdentificator();
		LPRDOEvent        pEvent      = PARSER->findEvent(eventName);
		if (!pEvent)
		{
			PARSER->error().error(@1, rdo::format(_T("Попытка запланировать неизвестное событие: %s"), eventName.c_str()));
		}

		rdoRuntime::LPRDOCalcEventStop pCalc = rdo::Factory<rdoRuntime::RDOCalcEventStop>::create();
		ASSERT(pCalc);
		pEvent->attachCalc(pCalc);

		$$ = PARSER->stack().push(pCalc);
	}
	| RDO_IDENTIF '.' RDO_Stopping error
	{
		PARSER->error().error(@4, _T("Не найден символ окончания инструкции - точка с запятой"));
	}
	;

planning_statement
	: RDO_IDENTIF '.' RDO_Planning '(' fun_arithm event_descr_param ')' ';'
	{
		tstring        eventName   = RDOVALUE($1)->getIdentificator();
		LPRDOFUNArithm pTimeArithm = PARSER->stack().pop<RDOFUNArithm>($5);
		LPRDOEvent     pEvent      = PARSER->findEvent(eventName);
		if (!pEvent)
		{
			PARSER->error().error(@1, rdo::format(_T("Попытка запланировать неизвестное событие: %s"), eventName.c_str()));
		}

		rdoRuntime::LPRDOCalc pCalcTime = pTimeArithm->createCalc(NULL);
		ASSERT(pCalcTime);

		rdoRuntime::LPRDOCalcEventPlan pCalc = rdo::Factory<rdoRuntime::RDOCalcEventPlan>::create(pCalcTime);
		ASSERT(pCalc);
		pEvent->attachCalc(pCalc);

		$$ = PARSER->stack().push(pCalc);
	}
	| RDO_IDENTIF '.' RDO_Planning '(' fun_arithm event_descr_param ')' error
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
	| RDO_IDENTIF '.' RDO_Planning '(' fun_arithm event_descr_param error
	{
		PARSER->error().error(@6, _T("Ожидается закрывающая скобка"));
	}
	;

event_descr_param
	: /* empty */
	| event_descr_param ',' '*'
	{
		PARSER->error().error(@1, @2, "Планировать события с параметрами пока нельзя")
	}
	| event_descr_param ',' fun_arithm
	{
		PARSER->error().error(@1, @2, "Планировать события с параметрами пока нельзя")
	}
	| event_descr_param ',' error
	{
		PARSER->error().error(@1, @2, "Ошибка описания параметра события")
	}
	;

if_statement
	: RDO_if '(' fun_logic ')' statement
	{
		LPRDOFUNLogic pCondition = PARSER->stack().pop<RDOFUNLogic>($3);
		ASSERT(pCondition);

		rdoRuntime::LPRDOCalc pConditionCalc = pCondition->getCalc();
		ASSERT(pConditionCalc);

		rdoRuntime::LPRDOCalc pStatementCalc = PARSER->stack().pop<rdoRuntime::RDOCalc>($5);
		ASSERT(pStatementCalc);

		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcIf>::create(pConditionCalc, pStatementCalc);
		ASSERT(pCalc);

		$$ = PARSER->stack().push(pCalc);
	}
	| RDO_if '(' fun_logic ')' statement RDO_else statement
	{
		LPRDOFUNLogic pCondition = PARSER->stack().pop<RDOFUNLogic>($3);
		ASSERT(pCondition);
		
		rdoRuntime::LPRDOCalc pConditionCalc = pCondition->getCalc();
		ASSERT(pConditionCalc);

		rdoRuntime::LPRDOCalc pIfStatementCalc = PARSER->stack().pop<rdoRuntime::RDOCalc>($5);
		ASSERT(pIfStatementCalc);

		rdoRuntime::LPRDOCalc pElseStatementCalc = PARSER->stack().pop<rdoRuntime::RDOCalc>($7);
		ASSERT(pElseStatementCalc);

		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcIfElse>::create(pConditionCalc, pIfStatementCalc, pElseStatementCalc);
		ASSERT(pCalc);
		
		$$ = PARSER->stack().push(pCalc);
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

increment_or_decrement_type
	: RDO_IncrEqual
	{
		$$ = rdoRuntime::ET_INCR;
	}
	| RDO_DecrEqual
	{
		$$ = rdoRuntime::ET_DECR;
	}
	;

param_equal_type
	: RDO_set
	{
		$$ = rdoRuntime::ET_EQUAL;
	}
	| '='
	{
		$$ = rdoRuntime::ET_EQUAL;
	}
	| RDO_PlusEqual
	{
		$$ = rdoRuntime::ET_PLUS;
	}
	| RDO_MinusEqual
	{
		$$ = rdoRuntime::ET_MINUS;
	}
	| RDO_MultiplyEqual
	{
		$$ = rdoRuntime::ET_MULTIPLY;
	}
	| RDO_DivideEqual
	{
		$$ = rdoRuntime::ET_DIVIDE;
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

// ----------------------------------------------------------------------------
// ---------- Описание типа параметра
// ----------------------------------------------------------------------------
param_type
	: RDO_integer param_type_range param_value_default
	{
		LPRDOTypeRangeRange pRange = PARSER->stack().pop<RDOTypeRangeRange>($2);
		LPRDOTypeParam pType;
		if (pRange)
		{
			if (pRange->getMin().typeID() != rdoRuntime::RDOType::t_int ||
			    pRange->getMax().typeID() != rdoRuntime::RDOType::t_int)
			{
				PARSER->error().error(@2, _T("Диапазон целого типа должен быть целочисленным"));
			}
			LPRDOTypeIntRange pIntRange = rdo::Factory<RDOTypeIntRange>::create(pRange);
			ASSERT(pIntRange);
			pType = rdo::Factory<RDOTypeParam>::create(pIntRange, RDOVALUE($3), RDOParserSrcInfo(@1, @3));
		}
		else
		{
			pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__int>::create(), RDOVALUE($3), RDOParserSrcInfo(@1, @3));
		}
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_real param_type_range param_value_default
	{
		LPRDOTypeRangeRange pRange = PARSER->stack().pop<RDOTypeRangeRange>($2);
		LPRDOTypeParam pType;
		if (pRange)
		{
			LPRDOTypeRealRange pRealRange = rdo::Factory<RDOTypeRealRange>::create(pRange);
			ASSERT(pRealRange);
			pType = rdo::Factory<RDOTypeParam>::create(pRealRange, RDOVALUE($3), RDOParserSrcInfo(@1, @3));
		}
		else
		{
			pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__real>::create(), RDOVALUE($3), RDOParserSrcInfo(@1, @3));
		}
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_string param_value_default
	{
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__string>::create(), RDOVALUE($2), RDOParserSrcInfo(@1, @2));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_array param_value_default
	{
		LEXER->array_cnt_rst();
		LPRDOArrayType pArray = PARSER->stack().pop<RDOArrayType>($1);
		ASSERT(pArray);
		LPRDOTypeParam pType  = rdo::Factory<RDOTypeParam>::create(pArray, RDOVALUE($2), RDOParserSrcInfo(@1, @2));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_bool param_value_default
	{
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__bool>::create(), RDOVALUE($2), RDOParserSrcInfo(@1, @2));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_enum param_value_default
	{
		LEXER->enumReset();
		LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
		ASSERT(pEnum);
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(pEnum, RDOVALUE($2), RDOParserSrcInfo(@1, @2));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_such_as param_value_default
	{
		LPRDOTypeParam pTypeSuchAs = PARSER->stack().pop<RDOTypeParam>($1);
		ASSERT(pTypeSuchAs);
		RDOValue default = RDOVALUE($2);
		if (!default.defined())
		{
			if (pTypeSuchAs->default().defined())
			{
				default = pTypeSuchAs->default();
			}
		}
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(pTypeSuchAs->type(), default, RDOParserSrcInfo(@1, @2));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	;
	//| RDO_integer error
	//{
	//	PARSER->error().error(@2, _T("Ошибка после ключевого слова integer. Возможно, не хватает значения по-умолчанию."));
	//}
	//| RDO_real error
	//{
	//	PARSER->error().error(@2, _T("Ошибка после ключевого слова real. Возможно, не хватает значения по-умолчанию."));
	//}
	//| param_type_enum error
	//{
	//	PARSER->error().error(@2, _T("Ошибка после перечислимого типа. Возможно, не хватает значения по-умолчанию."));
	//}

param_type_range
	: /* empty */
	{
		$$ = PARSER->stack().push<RDOTypeRangeRange>(LPRDOTypeRangeRange());
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = PARSER->stack().push(pRange);
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = PARSER->stack().push(pRange);
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = PARSER->stack().push(pRange);
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
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
		pEnum->add(RDOVALUE($1));
		LEXER->enumBegin();
		$$ = PARSER->stack().push(pEnum);
	}
	| param_type_enum_list ',' RDO_IDENTIF
	{
		if (!LEXER->enumEmpty())
		{
			LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
			ASSERT(pEnum);
			pEnum->add(RDOVALUE($3));
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
			pEnum->add(RDOVALUE($2));
			$$ = PARSER->stack().push(pEnum);
			PARSER->error().warning(@1, rdo::format(_T("Пропущена запятая перед: %s"), RDOVALUE($2)->getIdentificator().c_str()));
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
		tstring type  = RDOVALUE($2)->getIdentificator();
		tstring param = RDOVALUE($4)->getIdentificator();
		LPRDORTPResType pResType = PARSER->findRTPResType(type);
		if (!pResType)
		{
			PARSER->error().error(@2, rdo::format(_T("Ссылка на неизвестный тип ресурса: %s"), type.c_str()));
		}
		LPRDORTPParam pParam = pResType->findRTPParam(param);
		if (!pParam)
		{
			PARSER->error().error(@4, rdo::format(_T("Ссылка на неизвестный параметр ресурса: %s.%s"), type.c_str(), param.c_str()));
		}
		$$ = PARSER->stack().push(pParam->getParamType());
	}
	| RDO_such_as RDO_IDENTIF
	{
		tstring constName = RDOVALUE($2)->getIdentificator();
		LPRDOFUNConstant pConstant = PARSER->findFUNConstant(constName);
		if (!pConstant)
		{
			PARSER->error().error(@2, rdo::format(_T("Ссылка на несуществующую константу: %s"), constName.c_str()));
		}
		$$ = PARSER->stack().push(pConstant->getType());
	}
	| RDO_such_as RDO_IDENTIF '.' error
	{
		tstring type = RDOVALUE($2)->getIdentificator();
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

param_value_default
	: /* empty */
	{
		$$ = (int)PARSER->addValue(new rdoParse::RDOValue());
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

param_type_array
	: RDO_array '<' param_type '>'
	{
		LPRDOTypeParam pParamType = PARSER->stack().pop<RDOTypeParam>($3);
		ASSERT(pParamType);
		LPRDOArrayType pArray = rdo::Factory<RDOArrayType>::create(pParamType->type(), RDOParserSrcInfo(@1, @4));
		$$ = PARSER->stack().push(pArray);
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
	| fun_arithm '=' fun_arithm
	{
		YYLTYPE          equal_pos = @2;
		LPCorrection     pCorrection = new Correction(++equal_pos.m_last_line, ++equal_pos.m_last_pos, Equality);
		PARSER->error().error(@2, rdo::format(_T("В позиции Ln %i Col %i не хватает знака равенства"), equal_pos.m_last_line, equal_pos.m_last_pos));
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

// ----------------------------------------------------------------------------
// ---------- Локальная переменная
// ----------------------------------------------------------------------------
local_variable_declaration
	: type_declaration init_declaration_list ';'
	{
		LPContext pContext = PARSER->context();
		ASSERT(pContext);

		LPContextPattern pContextPattern = pContext.object_static_cast<ContextPattern>();
		ASSERT(pContextPattern);

		LPLocalVariableList pLocalVariableList = pContextPattern->getLocalMemory();
		ASSERT(pLocalVariableList);

		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcNoChange>::create();
		ASSERT(pCalc);
		$$ = PARSER->stack().push(pCalc);
	}
	;

type_declaration
	: RDO_integer
	{
		LPRDOType pBaseType = rdo::Factory<RDOType__int>::create();
		ASSERT(pBaseType);

		LPRDOTypeParam pType;
		pType = rdo::Factory<RDOTypeParam>::create(pBaseType, rdoParse::RDOValue(pBaseType, RDOParserSrcInfo(@1)), RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPContext pTypeContext = rdo::Factory<TypeContext>::create(pType);
		ASSERT(pTypeContext);

		PARSER->contextStack().push(pTypeContext);
	}
	//| RDO_real
	//{
	//	LPRDOTypeParam pType;
	//	pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__real>::create(), rdoParse::RDOValue(), RDOParserSrcInfo(@1));
	//	ASSERT(pType);
	//	
	//	LPContext pContext = PARSER->context();
	//	ASSERT(pContext);

	//	LPLocalVariableType pLocalVariableType = pContext->getLocalVariableType();
	//	ASSERT(pLocalVariableType);

	//	pLocalVariableType->rememberType(pType);
	//	$$ = PARSER->stack().push(pType);
	//}
	//| RDO_string
	//{
	//	LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__string>::create(), rdoParse::RDOValue(), RDOParserSrcInfo(@1));
	//	ASSERT(pType);
	//	
	//	LPContext pContext = PARSER->context();
	//	ASSERT(pContext);

	//	LPLocalVariableType pLocalVariableType = pContext->getLocalVariableType();
	//	ASSERT(pLocalVariableType);

	//	pLocalVariableType->rememberType(pType);
	//	$$ = PARSER->stack().push(pType);
	//}
	//| param_type_array
	//{
	//	LEXER->array_cnt_rst();
	//	LPRDOArrayType pArray = PARSER->stack().pop<RDOArrayType>($1);
	//	ASSERT(pArray);
	//	LPRDOTypeParam pType  = rdo::Factory<RDOTypeParam>::create(pArray, rdoParse::RDOValue(), RDOParserSrcInfo(@1));
	//	ASSERT(pType);

	//	LPContext pContext = PARSER->context();
	//	ASSERT(pContext);

	//	LPLocalVariableType pLocalVariableType = pContext->getLocalVariableType();
	//	ASSERT(pLocalVariableType);

	//	pLocalVariableType->rememberType(pType);
	//	$$ = PARSER->stack().push(pType);
	//}
	//| RDO_bool
	//{
	//	LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__bool>::create(), rdoParse::RDOValue(), RDOParserSrcInfo(@1));
	//	ASSERT(pType);
	//	
	//	LPContext pContext = PARSER->context();
	//	ASSERT(pContext);

	//	LPLocalVariableType pLocalVariableType = pContext->getLocalVariableType();
	//	ASSERT(pLocalVariableType);

	//	pLocalVariableType->rememberType(pType);
	//	$$ = PARSER->stack().push(pType);
	//}
	//| param_type_enum
	//{
	//	LEXER->enumReset();
	//	LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
	//	ASSERT(pEnum);
	//	LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(pEnum, rdoParse::RDOValue(), RDOParserSrcInfo(@1));
	//	ASSERT(pType);
	//	
	//	LPContext pContext = PARSER->context();
	//	ASSERT(pContext);

	//	LPLocalVariableType pLocalVariableType = pContext->getLocalVariableType();
	//	ASSERT(pLocalVariableType);

	//	pLocalVariableType->rememberType(pType);
	//	$$ = PARSER->stack().push(pType);
	//}
	;

init_declaration_list
	: init_declaration
	{
		LPLocalVariable pLocalVariable = PARSER->stack().pop<LocalVariable>($1);
		ASSERT(pLocalVariable);

		LPContext pContext = PARSER->context();
		ASSERT(pContext);

		LPContextPattern pContextPattern = pContext.object_static_cast<ContextPattern>();
		ASSERT(pContextPattern);

		LPLocalVariableList pLocalVariableList = pContextPattern->getLocalMemory();
		ASSERT(pLocalVariableList);

		pLocalVariableList->append(pLocalVariable);
	}
	| init_declaration_list ',' init_declaration
	{
		LPContext pContext = PARSER->context();
		ASSERT(pContext);

		LPContextPattern pContextPattern = pContext.object_static_cast<ContextPattern>();
		ASSERT(pContextPattern);

		LPLocalVariableList pLocalVariableList = pContextPattern->getLocalMemory();
		ASSERT(pLocalVariableList);

		LPLocalVariable pLocalVariable = PARSER->stack().pop<LocalVariable>($3);
		ASSERT(pLocalVariable);

		pLocalVariableList->append(pLocalVariable);
	}
	;

init_declaration
	: RDO_IDENTIF
	{
		LPContext pContext = PARSER->context();
		ASSERT(pContext);

		LPTypeContext pTypeContext = pContext.object_static_cast<TypeContext>();
		ASSERT(pTypeContext);

		LPRDOTypeParam pParam = pTypeContext->getType();
		ASSERT(pParam);

		LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(rdoParse::RDOValue(pParam->type(), RDOParserSrcInfo(@1)));
		ASSERT(pArithm);

		rdoRuntime::LPRDOCalc pCalc = pArithm->createCalc(pParam);
		ASSERT(pCalc);

		LPLocalVariable pLocalVariable = rdo::Factory<LocalVariable>::create(RDOVALUE($1), pCalc);
		ASSERT(pLocalVariable);

		$$ = PARSER->stack().push(pLocalVariable);
	}
	| RDO_IDENTIF '=' fun_arithm ';'
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm);

		LPContext pContext = PARSER->context();
		ASSERT(pContext);

		LPTypeContext pTypeContext = pContext.object_static_cast<TypeContext>();
		ASSERT(pTypeContext);

		LPRDOTypeParam pParam = pTypeContext->getType();
		ASSERT(pParam);

		rdoRuntime::LPRDOCalc pCalc = pArithm->createCalc(pParam);
		ASSERT(pCalc);

		LPLocalVariable pLocalVariable = rdo::Factory<LocalVariable>::create(RDOVALUE($1), pCalc);
		ASSERT(pLocalVariable);

		$$ = PARSER->stack().push(pLocalVariable);
	}
	;

%%

CLOSE_RDO_PARSER_NAMESPACE

/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoevn_preparse.y
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      12.06.2010
  \brief     Предварительный поиск названий событий
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
#include "simulator/compiler/parser/rdopat.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/type/range.h"
#include "simulator/runtime/rdotrace.h"
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
	: /* empty */          {   }
	| pat_main pat_pattern {@1;}
	| error                {   }
	;

pat_header
	: RDO_Pattern RDO_IDENTIF_COLON RDO_event pat_trace
	{
		LPRDOValue pName = PARSER->stack().pop<RDOValue>($2);
		ASSERT(pName);
		LPRDOEvent pEvent = rdo::Factory<RDOEvent>::create(pName->value().getIdentificator(), true);
		ASSERT(pEvent);
		PARSER->insertEvent(pEvent);
	}
	| RDO_Pattern RDO_IDENTIF_COLON error                         {}
	| RDO_Pattern error                                           {}
	;

pat_trace
	: /* empty */  {}
	| RDO_trace    {}
	| RDO_no_trace {}
	;

pat_params_begin
	: pat_header RDO_Parameters {}
	;

pat_params
	: pat_params_begin RDO_IDENTIF_COLON type_declaration
	| pat_params       RDO_IDENTIF_COLON type_declaration
	| pat_params_begin error
	| pat_params_begin RDO_IDENTIF       error
	| pat_params_begin RDO_IDENTIF_COLON error
	| pat_params error
	| pat_params RDO_IDENTIF       error
	| pat_params RDO_IDENTIF_COLON error
	;

pat_params_end
	: pat_params RDO_Relevant_resources {}
	| pat_header RDO_Relevant_resources {}
	| pat_header error                  {}
	;

pat_rel_res
	: pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF pat_conv pat_conv    {}
	| pat_rel_res    RDO_IDENTIF_COLON RDO_IDENTIF pat_conv pat_conv    {}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF pat_conv             {}
	| pat_rel_res    RDO_IDENTIF_COLON RDO_IDENTIF pat_conv             {}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange pat_conv    {}
	| pat_rel_res    RDO_IDENTIF_COLON RDO_IDENTIF_NoChange pat_conv    {}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange_NoChange    {}
	| pat_rel_res    RDO_IDENTIF_COLON RDO_IDENTIF_NoChange_NoChange    {}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange             {}
	| pat_rel_res    RDO_IDENTIF_COLON RDO_IDENTIF_NoChange             {}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF RDO_IDENTIF_NoChange {}
	| pat_rel_res    RDO_IDENTIF_COLON RDO_IDENTIF RDO_IDENTIF_NoChange {}
	| pat_params_end error                                              {}
	| pat_rel_res    error                                              {}
	| pat_params_end RDO_IDENTIF_COLON error                            {}
	| pat_rel_res    RDO_IDENTIF_COLON error                            {}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF error                {}
	| pat_rel_res    RDO_IDENTIF_COLON RDO_IDENTIF error                {}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF pat_conv error       {}
	| pat_rel_res    RDO_IDENTIF_COLON RDO_IDENTIF pat_conv error       {}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange error       {}
	| pat_rel_res    RDO_IDENTIF_COLON RDO_IDENTIF_NoChange error       {}
	;

pat_conv
	: RDO_Keep     {}
	| RDO_Create   {}
	| RDO_Erase    {}
	| RDO_NonExist {}
	;

pat_common_choice
	: pat_rel_res
	| pat_rel_res RDO_first               {}
	| pat_rel_res RDO_with_min fun_arithm {}
	| pat_rel_res RDO_with_max fun_arithm {}
	| pat_rel_res RDO_with_min error      {}
	| pat_rel_res RDO_with_max error      {}
	;

pat_time
	: pat_common_choice RDO_Body                         {}
	| pat_common_choice RDO_Time '=' fun_arithm RDO_Body {}
	| pat_common_choice RDO_Time '=' fun_arithm error    {}
	| pat_common_choice RDO_Time '=' error               {}
	| pat_common_choice RDO_Time error                   {}
	| pat_common_choice error                            {}
	;

pat_body
	: pat_time    RDO_IDENTIF_RELRES {}
	| pat_convert RDO_IDENTIF_RELRES {}
	| pat_time    error              {}
	| pat_convert error              {}
	;

pat_res_usage
	: pat_body pat_choice pat_order {}
	;

pat_choice
	: /* empty */               {}
	| pat_choice_nocheck        {}
	| pat_choice_from fun_logic {}
	| pat_choice_from error     {}
	;

pat_choice_nocheck
	: RDO_Choice RDO_NoCheck    {}
	;

pat_choice_from
	: RDO_Choice RDO_from       {}
	;

pat_order
	: /* empty */                    {}
	| pat_choice_first               {}
	| pat_choice_with_min fun_arithm {}
	| pat_choice_with_max fun_arithm {}
	| pat_choice_with_min error      {}
	| pat_choice_with_max error      {}
	;

pat_choice_first
	: RDO_first    {}
	;

pat_choice_with_min
	: RDO_with_min {}
	;

pat_choice_with_max
	: RDO_with_max {}
	;

pat_convert
	: pat_res_usage                                                                               {}
	| pat_res_usage convert_begin pat_trace pat_convert_cmd                                       {}
	| pat_res_usage                                         convert_end pat_trace pat_convert_cmd {}
	| pat_res_usage convert_begin pat_trace pat_convert_cmd convert_end pat_trace pat_convert_cmd {}
	| pat_res_usage convert_rule  pat_trace pat_convert_cmd                                       {}
	| pat_res_usage convert_event pat_trace pat_convert_cmd                                       {}
	;

convert_rule
	: RDO_Convert_rule  {}
	;

convert_event
	: RDO_Convert_event {}
	;

convert_begin
	: RDO_Convert_begin {}
	;

convert_end
	: RDO_Convert_end   {}
	;

pat_convert_cmd
	: /* empty */                                             {}
	| pat_convert_cmd RDO_IDENTIF param_equal_type fun_arithm {}
	| pat_convert_cmd RDO_IDENTIF param_equal_type error      {}
	| pat_convert_cmd RDO_IDENTIF_NoChange                    {}
	;

param_equal_type
	: RDO_set           {}
	| '='               {}
	| RDO_PlusEqual     {}
	| RDO_MinusEqual    {}
	| RDO_MultiplyEqual {}
	| RDO_DivideEqual   {}
	;

pat_pattern
	: pat_convert RDO_End {}
	;

// --------------------------------------------------------------------------------
// -------------------- Описание типа параметра
// --------------------------------------------------------------------------------
type_declaration_context
	: type_declaration
	;

type_declaration
	: RDO_integer param_type_range {}
	| RDO_real    param_type_range {}
	| RDO_string                   {}
	| type_declaration_array       {}
	| RDO_bool                     {}
	| type_declaration_enum        {}
	| RDO_IDENTIF                  {}
	| type_declaration_such_as     {}
	;

param_type_range
	: /* empty */
	| '[' RDO_INT_CONST  RDO_dblpoint RDO_INT_CONST  ']'
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST ']'
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST  ']'
	| '[' RDO_INT_CONST  RDO_dblpoint RDO_REAL_CONST ']'
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST error
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST  error
	| '[' RDO_INT_CONST  RDO_dblpoint RDO_REAL_CONST error
	| '[' RDO_INT_CONST  RDO_dblpoint RDO_INT_CONST  error
	| '[' RDO_REAL_CONST RDO_dblpoint error
	| '[' RDO_INT_CONST  RDO_dblpoint error
	| '[' error
	;

type_declaration_enum
	: '(' param_type_enum_list ')'   {}
	| '(' param_type_enum_list error {}
	;

param_type_enum_list
	: RDO_IDENTIF                             {}
	| param_type_enum_list ',' RDO_IDENTIF    {}
	| param_type_enum_list     RDO_IDENTIF    {}
	| param_type_enum_list ',' RDO_INT_CONST  {}
	| param_type_enum_list ',' RDO_REAL_CONST {}
	| param_type_enum_list     RDO_INT_CONST  {}
	| param_type_enum_list     RDO_REAL_CONST {}
	| RDO_INT_CONST                           {}
	| RDO_REAL_CONST                          {}
	;

type_declaration_such_as
	: RDO_such_as RDO_IDENTIF '.' RDO_IDENTIF {}
	| RDO_such_as RDO_IDENTIF                 {}
	| RDO_such_as RDO_IDENTIF '.' error       {}
	| RDO_such_as error                       {}
	;

type_declaration_array
	: RDO_array '<' type_declaration '>' {}
	;

// --------------------------------------------------------------------------------
// -------------------- Общие составные токены для всех объектов РДО
// --------------------------------------------------------------------------------
// -------------------- Описание переменной
// --------------------------------------------------------------------------------
param_value
	: RDO_INT_CONST     {}
	| RDO_REAL_CONST    {}
	| RDO_STRING_CONST  {}
	| RDO_IDENTIF       {}
	| RDO_BOOL_CONST    {}
	| param_array_value {}
	;

param_array_value
	: '[' array_item ']'   {}
	| '[' array_item error {}
	;

array_item
	: param_value                {}
	| array_item ',' param_value {}
	| array_item     param_value {}
	;

param_value_default
	: /* empty */     {}
	| '=' param_value {}
	| '=' error       {}
	;

// --------------------------------------------------------------------------------
// -------------------- Логические выражения
// --------------------------------------------------------------------------------
fun_logic_eq
	: RDO_eq {}
	;

fun_logic
	: fun_arithm  fun_logic_eq  fun_arithm   {}
	| fun_arithm  RDO_neq       fun_arithm   {}
	| fun_arithm  '<'           fun_arithm   {}
	| fun_arithm  '>'           fun_arithm   {}
	| fun_arithm  RDO_leq       fun_arithm   {}
	| fun_arithm  RDO_geq       fun_arithm   {}
	| fun_logic   RDO_and       fun_logic    {}
	| fun_logic   RDO_or        fun_logic    {}
	| fun_arithm                             {}
	| fun_group                              {}
	| fun_select_logic                       {}
	| '[' fun_logic ']'                      {}
	| '(' fun_logic ')'                      {}
	| RDO_not fun_logic                      {}
	| '[' fun_logic error                    {}
	| '(' fun_logic error                    {}
	;

// --------------------------------------------------------------------------------
// -------------------- Арифметические выражения
// --------------------------------------------------------------------------------
fun_arithm
	: RDO_INT_CONST                      {}
	| RDO_REAL_CONST                     {}
	| RDO_BOOL_CONST                     {}
	| RDO_STRING_CONST                   {}
	| param_array_value                  {}
	| RDO_IDENTIF                        {}
	| RDO_IDENTIF_RELRES                 {}
	| RDO_IDENTIF '.' RDO_IDENTIF        {}
	| RDO_IDENTIF_RELRES '.' RDO_IDENTIF {}
	| '*'                                {}
	| fun_arithm '+' fun_arithm          {}
	| fun_arithm '-' fun_arithm          {}
	| fun_arithm '*' fun_arithm          {}
	| fun_arithm '/' fun_arithm          {}
	| fun_arithm_func_call               {}
	| fun_select_arithm                  {}
	| '(' fun_arithm ')'                 {}
	| '-' fun_arithm %prec RDO_UMINUS    {}
	| RDO_IDENTIF '.' RDO_Size
	| RDO_IDENTIF '[' fun_arithm ']' '.' RDO_IDENTIF {}
	| RDO_IDENTIF '[' fun_arithm ']'     {}
	;

// --------------------------------------------------------------------------------
// -------------------- Функции и последовательности
// --------------------------------------------------------------------------------
fun_arithm_func_call
	: RDO_IDENTIF '(' arithm_list ')' {};
	
arithm_list
	: /* empty */
	| arithm_list_body {};
	
arithm_list_body
	: fun_arithm                      {};
	| arithm_list_body ',' fun_arithm {};
	| arithm_list_body ',' error      {};

// --------------------------------------------------------------------------------
// -------------------- Групповые выражения
// --------------------------------------------------------------------------------
fun_group_keyword
	: RDO_Exist       {}
	| RDO_Not_Exist   {}
	| RDO_For_All     {}
	| RDO_Not_For_All {}
	;

fun_group_header
	: fun_group_keyword '(' RDO_IDENTIF_COLON {}
	| fun_group_keyword '(' error             {}
	| fun_group_keyword error                 {}
	;

fun_group
	: fun_group_header fun_logic ')'     {}
	| fun_group_header RDO_NoCheck ')'   {}
	| fun_group_header fun_logic error   {}
	| fun_group_header RDO_NoCheck error {}
	| fun_group_header error             {}
	;

// --------------------------------------------------------------------------------
// -------------------- Select
// --------------------------------------------------------------------------------
fun_select_header
	: RDO_Select '(' RDO_IDENTIF_COLON {}
	| RDO_Select '(' error             {}
	| RDO_Select error                 {}
	;

fun_select_body
	: fun_select_header fun_logic ')'     {}
	| fun_select_header RDO_NoCheck ')'   {}
	| fun_select_header fun_logic error   {}
	| fun_select_header RDO_NoCheck error {}
	| fun_select_header error             {}
	;

fun_select_keyword
	: RDO_Exist       {}
	| RDO_Not_Exist   {}
	| RDO_For_All     {}
	| RDO_Not_For_All {}
	;

fun_select_logic
	: fun_select_body '.' fun_select_keyword '(' fun_logic ')' {}
	| fun_select_body '.' fun_select_keyword '(' error         {}
	| fun_select_body '.' fun_select_keyword error             {}
	| fun_select_body '.' RDO_Empty '(' ')'                    {}
	| fun_select_body '.' RDO_Empty '(' error                  {}
	| fun_select_body '.' RDO_Empty error                      {}
	| fun_select_body '.' error                                {}
	| fun_select_body error                                    {}
	;

fun_select_arithm
	: fun_select_body '.' RDO_Size '(' ')'           {}
	| fun_select_body '.' RDO_Size error             {}
	| fun_select_body '.' RDO_Size '(' error         {}
	| fun_select_body '.' RDO_Select_Array '(' ')'   {}
	| fun_select_body '.' RDO_Select_Array error     {}
	| fun_select_body '.' RDO_Select_Array '(' error {}
	;

%%

CLOSE_RDO_PARSER_NAMESPACE

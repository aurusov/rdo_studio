/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdopat.y
 * author   : Александ Барс, Урусов Андрей, Лущан Дмитри
 * date     : 
 * bref     : 
 * indent   : 4T
 */

%{
#define YYPARSE_PARAM lexer
#define YYLEX_PARAM lexer
%}

%pure-parser

%token RDO_Resource_type				257
%token RDO_permanent					258
%token RDO_Parameters					259
%token RDO_integer						260
%token RDO_real							261
%token RDO_End							262
%token RDO_temporary					263
%token RDO_IDENTIF						264
%token RDO_INT_CONST					265
%token RDO_REAL_CONST					266
%token RDO_such_as						267
%token RDO_dblpoint						268
%token RDO_Resources					269
%token RDO_trace						270
%token RDO_no_trace						271
%token RDO_IDENTIF_COLON				272
%token RDO_Constant						273
%token RDO_Body							274
%token RDO_Function						275
%token RDO_Type							276
%token RDO_algorithmic					277
%token RDO_table						278
%token RDO_list							279
%token RDO_Exist						281
%token RDO_Not_Exist					282
%token RDO_For_All						283
%token RDO_Not_For_All					284
%token RDO_neq							285
%token RDO_leq							286
%token RDO_geq							287
%token RDO_NoCheck						288
%token RDO_Calculate_if					289
%token RDO_or							290
%token RDO_and							291
%token RDO_Sequence						292
%token RDO_uniform						293
%token RDO_exponential					294
%token RDO_normal						295
%token RDO_by_hist						296
%token RDO_enumerative					297

%token RDO_Pattern						298
%token RDO_operation					299
%token RDO_event						379
%token RDO_irregular_event				300
%token RDO_rule							301
%token RDO_keyboard						302
%token RDO_Relevant_resources			303
%token RDO_Keep							304
%token RDO_Create						305
%token RDO_Erase						306
%token RDO_NonExist						307
%token RDO_IDENTIF_NoChange				308
%token RDO_Time							309
%token RDO_Choice						310
%token RDO_from							311
%token RDO_first						312
%token RDO_Convert_begin				313
%token RDO_Convert_end					314
%token RDO_Convert_rule					315
%token RDO_Convert_event				316
%token RDO_with_max						317
%token RDO_with_min						318
%token RDO_set							319
%token RDO_IDENTIF_NoChange_NoChange	320
%token RDO_Operations					321
	
%token RDO_Results						322
%token RDO_watch_par					323
%token RDO_watch_state					324
%token RDO_watch_quant					325
%token RDO_watch_value					326
%token RDO_get_value					327

%token RDO_Model_name					328
%token RDO_Resource_file				329
%token RDO_OprIev_file					330
%token RDO_Frame_file					331
%token RDO_Statistic_file				332
%token RDO_Results_file					333
%token RDO_Trace_file					334
%token RDO_Show_mode					335
%token RDO_Frame_number					336
%token RDO_Show_rate					337
%token RDO_Run_StartTime				338
%token RDO_Trace_StartTime				339
%token RDO_Trace_EndTime				340
%token RDO_Terminate_if					341
%token RDO_Break_point					342
%token RDO_Seed							343
%token RDO_NoShow						344
%token RDO_Monitor						345
%token RDO_Animation					346
%token RDO_NoChange						347

%token RDO_Decision_point				348
%token RDO_search						349
%token RDO_trace_stat					350
%token RDO_trace_tops					351
%token RDO_trace_all					352
%token RDO_Condition					353
%token RDO_Term_condition				354
%token RDO_Evaluate_by					355
%token RDO_Compare_tops					356
%token RDO_NO							357
%token RDO_YES							358
%token RDO_Activities					359
%token RDO_value_before					360
%token RDO_value_after					361
%token RDO_some							362
%token RDO_Process						363
%token RDO_SEIZE						364
%token RDO_GENERATE						365
%token RDO_TERMINATE					366
%token RDO_ADVANCE						367
%token RDO_RELEASE						368
%token RDO_if							369
%token RDO_result						370
%token RDO_CF							371
%token RDO_Priority						372
%token RDO_prior						373
%token RDO_Parent						374
%token RDO_PlusEqual					375
%token RDO_MinusEqual					376
%token RDO_MultiplyEqual				377
%token RDO_DivideEqual					378

%token RDO_Frame						400
%token RDO_Show_if						401
%token RDO_Back_picture					402
%token RDO_Show							403
%token RDO_frm_cell						404
%token RDO_text							405
%token RDO_bitmap						406
%token RDO_s_bmp						407
%token RDO_rect							408
%token RDO_r_rect						409
%token RDO_line							410
%token RDO_ellipse						411
%token RDO_triang						412
%token RDO_active						413
%token RDO_ruler						414
%token RDO_space						415
%token RDO_color_transparent			416
%token RDO_color_last					417
%token RDO_color_white					418
%token RDO_color_black					419
%token RDO_color_red					420
%token RDO_color_green					421
%token RDO_color_blue					422
%token RDO_color_cyan					423
%token RDO_color_magenta				424
%token RDO_color_yellow					425
%token RDO_color_gray					426

%token RDO_IDENTIF_RELRES				427
%token RDO_typedef						428
%token RDO_enum							429

%token RDO_STRING_CONST					430
%token RDO_STRING_CONST_BAD				431
%token RDO_IDENTIF_BAD					432
%token RDO_Select						433
%token RDO_Size							434
%token RDO_Empty						435
%token RDO_not							436
%token RDO_UMINUS						437
%token RDO_string						438
%token RDO_bool							439
%token RDO_BOOL_CONST					440
%token RDO_Fuzzy						441
%token RDO_Fuzzy_Term					442
%token RDO_eq							443
%token RDO_External_Model				444
%token RDO_QUEUE						445
%token RDO_DEPART						446
%token RDO_ASSIGN						447


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
#include "rdo_lib/rdo_runtime/rdotrace.h"
// ===============================================================================

#define PARSER  LEXER->parser()
#define RUNTIME PARSER->runtime()

#define P_RDOVALUE(A) reinterpret_cast<PTR(RDOValue)>(A)
#define P_ARITHM(A)   reinterpret_cast<PTR(RDOFUNArithm)>(A)
#define P_LOGIC(A)    reinterpret_cast<PTR(RDOFUNLogic)>(A)

#define RDOVALUE(A)   (*P_RDOVALUE(A))
#define ARITHM(A)     (*P_ARITHM(A))
#define LOGIC(A)      (*P_LOGIC(A))

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
	: /* empty */	       {}
	| pat_main pat_pattern {@1;}
	| error                {}
	;

pat_header
	: RDO_Pattern RDO_IDENTIF_COLON RDO_operation       pat_trace {}
	| RDO_Pattern RDO_IDENTIF_COLON RDO_irregular_event pat_trace {}
	| RDO_Pattern RDO_IDENTIF_COLON RDO_event           pat_trace
	{
		LPRDOEvent pEvent = rdo::Factory<RDOEvent>::create(RDOVALUE($2)->getIdentificator());
		ASSERT(pEvent);
	}
	| RDO_Pattern RDO_IDENTIF_COLON RDO_rule            pat_trace {}
	| RDO_Pattern RDO_IDENTIF_COLON RDO_keyboard        pat_trace {}
	| RDO_Pattern RDO_IDENTIF_COLON error                         {}
	| RDO_Pattern error                                           {}
	;

pat_trace
	: /* empty */	{}
	| RDO_trace		{}
	| RDO_no_trace	{}
	;

pat_params_begin
	: pat_header RDO_Parameters {}
	;

pat_params
	: pat_params_begin RDO_IDENTIF_COLON param_type {}
	| pat_params RDO_IDENTIF_COLON param_type       {}
	| pat_params_begin error                        {}
	| pat_params_begin RDO_IDENTIF error            {}
	| pat_params_begin RDO_IDENTIF_COLON error      {}
	| pat_params error                              {}
	| pat_params RDO_IDENTIF error                  {}
	| pat_params RDO_IDENTIF_COLON error            {}
	;

pat_params_end
	: pat_params RDO_Relevant_resources {}
	| pat_header RDO_Relevant_resources {}
	| pat_header error                  {}
	;

pat_rel_res
	: pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF pat_conv pat_conv    {}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF pat_conv pat_conv       {}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF pat_conv             {}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF pat_conv                {}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange pat_conv    {}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange pat_conv       {}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange_NoChange    {}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange_NoChange       {}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange             {}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange                {}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF RDO_IDENTIF_NoChange {}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF RDO_IDENTIF_NoChange    {}
	| pat_params_end error                                              {}
	| pat_rel_res error                                                 {}
	| pat_params_end RDO_IDENTIF_COLON error                            {}
	| pat_rel_res RDO_IDENTIF_COLON error                               {}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF error                {}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF error                   {}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF pat_conv error       {}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF pat_conv error          {}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange error       {}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange error          {}
	;

pat_conv
	: RDO_Keep			{}
	| RDO_Create		{}
	| RDO_Erase			{}
	| RDO_NonExist		{}
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
	: pat_time RDO_IDENTIF_RELRES    {}
	| pat_convert RDO_IDENTIF_RELRES {}
	| pat_time error                 {}
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
	| pat_res_usage pat_convert_cmd convert_end pat_trace pat_convert_cmd                         {}
	| pat_res_usage convert_begin pat_trace pat_convert_cmd convert_end pat_trace pat_convert_cmd {}
	| pat_res_usage convert_rule pat_trace pat_convert_cmd                                        {}
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

// ----------------------------------------------------------------------------
// ---------- Описание типа параметра
// ----------------------------------------------------------------------------
param_type
	: RDO_integer param_type_range param_value_default {}
	| RDO_real param_type_range param_value_default    {}
	| RDO_string param_value_default                   {}
	| RDO_bool param_value_default                     {}
	| param_type_enum param_value_default              {}
	| param_type_such_as param_value_default           {}
	;

param_type_range
	: /* empty */                                          {}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST ']'     {}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST ']'   {}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST ']'    {}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST ']'    {}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST error {}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST error  {}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST error  {}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST error   {}
	| '[' RDO_REAL_CONST RDO_dblpoint error                {}
	| '[' RDO_INT_CONST RDO_dblpoint error                 {}
	| '[' error                                            {}
	;

param_type_enum
	: '(' param_type_enum_list ')'   {}
	| '(' param_type_enum_list error {}
	;

param_type_enum_list
	: RDO_IDENTIF                             {}
	| param_type_enum_list ',' RDO_IDENTIF    {}
	| param_type_enum_list RDO_IDENTIF        {}
	| param_type_enum_list ',' RDO_INT_CONST  {}
	| param_type_enum_list ',' RDO_REAL_CONST {}
	| param_type_enum_list RDO_INT_CONST      {}
	| param_type_enum_list RDO_REAL_CONST     {}
	| RDO_INT_CONST                           {}
	| RDO_REAL_CONST                          {}
	;

param_type_such_as
	: RDO_such_as RDO_IDENTIF '.' RDO_IDENTIF {}
	| RDO_such_as RDO_IDENTIF                 {}
	| RDO_such_as RDO_IDENTIF '.' error       {}
	| RDO_such_as error                       {}
	;

param_value_default
	: /* empty */         {}
	| '=' RDO_INT_CONST   {}
	| '=' RDO_REAL_CONST  {}
	| '=' RDO_STRING_CONST{}
	| '=' RDO_IDENTIF     {}
	| '=' RDO_BOOL_CONST  {}
	| '=' error           {}
	;

// ----------------------------------------------------------------------------
// ---------- Логические выражения
// ----------------------------------------------------------------------------
fun_logic_eq
	: '='    {}
	| RDO_eq {}
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

// ----------------------------------------------------------------------------
// ---------- Арифметические выражения
// ----------------------------------------------------------------------------
fun_arithm
	: RDO_INT_CONST                      {}
	| RDO_REAL_CONST                     {}
	| RDO_BOOL_CONST                     {}
	| RDO_STRING_CONST                   {}
	| RDO_IDENTIF                        {}
	| RDO_IDENTIF '.' RDO_IDENTIF        {}
	| RDO_IDENTIF_RELRES '.' RDO_IDENTIF {}
	| fun_arithm '+' fun_arithm		     {}
	| fun_arithm '-' fun_arithm		     {}
	| fun_arithm '*' fun_arithm		     {}
	| fun_arithm '/' fun_arithm		     {}
	| fun_arithm_func_call               {}
	| fun_select_arithm                  {}
	| '(' fun_arithm ')'                 {}
	| '-' fun_arithm %prec RDO_UMINUS    {}
	;

// ----------------------------------------------------------------------------
// ---------- Функции и последовательности
// ----------------------------------------------------------------------------
fun_arithm_func_call
	: RDO_IDENTIF '(' ')'                           {}
	| RDO_IDENTIF '(' fun_arithm_func_call_pars ')' {}
	| RDO_IDENTIF '(' error                         {}
	;

fun_arithm_func_call_pars
	: fun_arithm                               {}
	| fun_arithm_func_call_pars ',' fun_arithm {}
	| fun_arithm_func_call_pars error          {}
	| fun_arithm_func_call_pars ',' error      {}
	;

// ----------------------------------------------------------------------------
// ---------- Групповые выражения
// ----------------------------------------------------------------------------
fun_group_keyword
	: RDO_Exist			{}
	| RDO_Not_Exist		{}
	| RDO_For_All		{}
	| RDO_Not_For_All	{}
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

// ----------------------------------------------------------------------------
// ---------- Select
// ----------------------------------------------------------------------------
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
	: RDO_Exist			{}
	| RDO_Not_Exist		{}
	| RDO_For_All		{}
	| RDO_Not_For_All	{}
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
	: fun_select_body '.' RDO_Size '(' ')'   {}
	| fun_select_body '.' RDO_Size error     {}
	| fun_select_body '.' RDO_Size '(' error {}
	;

%%

CLOSE_RDO_PARSER_NAMESPACE

/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoproc_rss.y
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
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
%token RDO_CF							370
%token RDO_Priority						371
%token RDO_prior						372
%token RDO_Parent						373

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
// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdoparser_lexer.h"
#include "converter/smr2rdox/rdofun.h"
#include "converter/smr2rdox/rdodpt.h"
#include "converter/smr2rdox/rdortp.h"
#include "converter/smr2rdox/rdorss.h"
#include "converter/smr2rdox/rdopmd.h"

#include "simulator/runtime/process/rdoprocess.h"
// --------------------------------------------------------------------------------

#define CONVERTER LEXER->converter()
#define RUNTIME   CONVERTER->runtime()

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE
%}

%left RDO_or
%left RDO_and
%left '+' '-'
%left '*' '/'
%left RDO_not
%left RDO_UMINUS

%start dptrtp_main

%%

// --------------------------------------------------------------------------------
// -------------------- General part
// --------------------------------------------------------------------------------
dptrtp_main:
	| dptrtp_main RDO_Decision_point error RDO_End /* заглушка для $Decision_point */
	| dptrtp_main RDO_Activities error RDO_End     /* заглушка для $Activities     */
	| dptrtp_main dpt_process_end
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
		CONVERTER->error().error(@1, rdo::format("Ожидается имя ресурса для сбора статистики по очереди"));
	}
	| RDO_DEPART dpt_depart_param
	| RDO_DEPART error
	{
		CONVERTER->error().error(@1, rdo::format("Ожидается имя ресурса для сбора статистики по очереди"));
	}
	| RDO_SEIZE dpt_seize_param
	| RDO_SEIZE error
	{
		CONVERTER->error().error(@1, rdo::format("Ожидается список ресурсов, объединяемых в блок, через запятую"));
	}
	| RDO_RELEASE dpt_release_param
	| RDO_RELEASE error
	{
		CONVERTER->error().error(@1, rdo::format("Ожидается список ресурсов, объединяемых в блок, через запятую"));
	}
	;

dpt_queue_param
	: RDO_IDENTIF
	{
	}
	| RDO_IDENTIF error
	{
		CONVERTER->error().error(@1, "Ошибка в имени ресурса");
	}
	;

dpt_depart_param
	: RDO_IDENTIF
	{
	}
	| RDO_IDENTIF error
	{
		CONVERTER->error().error(@1, "Ошибка в имени ресурса");
	}
	;

dpt_seize_param
	: RDO_IDENTIF
	{
	}
	| dpt_seize_param ',' RDO_IDENTIF
	{
	}
	| dpt_seize_param error
	{
		CONVERTER->error().error(@1, "Ошибка в имени ресурса");
	}
	;

dpt_release_param
	: RDO_IDENTIF
	{
	}
	| dpt_release_param ',' RDO_IDENTIF
	{
	}
	| dpt_release_param error
	{
		CONVERTER->error().error(@1, "Ошибка в имени ресурса");
	}
	;

dpt_process_end
	: dpt_process RDO_End
	;

%%

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

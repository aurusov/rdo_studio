%token Resource_type 257
%token permanent 		258
%token Parameters 	259
%token integer 		260
%token real 			261
%token End 				262
%token temporary 		263
%token IDENTIF 		264
%token INT_CONST 		265
%token REAL_CONST 	266
%token such_as 		267
%token dblpoint 		268
%token Resources 		269
%token trace_keyword	270
%token no_trace 		271
%token IDENTIF_COLON	272
%token Constant		273
%token Body				274
%token Function_keyword		275
%token Type_keyword	276
%token algorithmic   277
%token table_keyword	278
%token list_keyword	279
%token Calculate_if	280
%token Exist			281
%token Not_Exist		282
%token For_All			283
%token Not_For_All	284
%token neq				285
%token leq				286
%token geq				287
%token NoCheck			288
%token Calculate_if	289
%token or_keyword		290
%token and_keyword	291
%token Sequence		292
%token uniform			293
%token exponential	294
%token normal_keyword	295
%token by_hist			296
%token enumerative	297

%token Pattern			298
%token operation		299
%token irregular_event	300
%token rule_keyword	301
%token keyboard		302
%token Relevant_resources	303
%token Keep				304
%token Create_keyword	305
%token Erase			306
%token NonExist		307
%token IDENTIF_NoChange	308
%token Time				309
%token Choice			310
%token from_keyword	311
%token first_keyword	312
%token Convert_begin	313
%token Convert_end	314
%token Convert_rule	315
%token Convert_event	316
%token with_max		317
%token with_min		318
%token IDENTIF_set	319
%token IDENTIF_NoChange_NoChange	320
%token Operations		321

%token Results			322
%token watch_par		323
%token watch_state	324
%token watch_quant	325
%token watch_value	326
%token get_value		327

%token Model_name			328
%token Resource_file		329
%token OprIev_file		330
%token Frame_file			331
%token Statistic_file	332
%token Results_file		333
%token Trace_file			334
%token Show_mode			335
%token Frame_number		336
%token Show_rate			337
%token Run_StartTime		338
%token Trace_StartTime	339
%token Trace_EndTime		340
%token Terminate_if		341
%token Break_point		342
%token Seed					343
%token NoShow				344
%token Monitor 			345
%token Animation			346
%token NoChange			347

%token Decision_point	348
%token search_keyword	349
%token trace_stat			350
%token trace_tops			351	
%token trace_all			352
%token Condition_keyword	353	
%token Term_condition	354
%token Evaluate_by		355
%token Compare_tops		356
%token NO					357
%token YES					358
%token Activities			359
%token value_before		360
%token value_after		361
%token some					362

%token Frame				400
%token Show_if				401
%token Back_picture		402
%token Show					403
%token frm_cell			404
%token text					405
%token transparent		406
%token bitmap				407
%token s_bmp				408
%token rect_keyword		409
%token r_rect				410
%token line					411
%token ellipse				412
%token triang				413
%token active				414
%token QUOTED_IDENTIF	415

%{
#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoparselex.h"
#include "rdoparser.h"
#include "rdosmr.h"

namespace rdoParse 
{
%}

%start smr_descr

%%

smr_model: Model_name '=' IDENTIF		{ $$ = (int)(new RDOSMR((string *)$3)); };

smr_descr: smr_model
		|	smr_descr Resource_file		'=' IDENTIF			{ ((RDOSMR *)$1)->setValue("Resource_file",	&RDOSMR::resourceFileName,	(string *)$4); $$ = $1; }
		|	smr_descr OprIev_file		'=' IDENTIF			{ ((RDOSMR *)$1)->setValue("OprIev_file",		&RDOSMR::oprIevFileName,	(string *)$4); $$ = $1; }
		|	smr_descr Frame_file			'=' IDENTIF			{ ((RDOSMR *)$1)->setValue("Frame_file",		&RDOSMR::frameFileName,		(string *)$4); $$ = $1; }
		|	smr_descr Statistic_file	'=' IDENTIF			{ ((RDOSMR *)$1)->setValue("Statistic_file",	&RDOSMR::statisticFileName,(string *)$4); $$ = $1; }
		|	smr_descr Results_file		'=' IDENTIF			{ ((RDOSMR *)$1)->setValue("Results_file",	&RDOSMR::resultsFileName,	(string *)$4); $$ = $1; }
		|	smr_descr Trace_file			'=' IDENTIF			{ ((RDOSMR *)$1)->setValue("Trace_file",		&RDOSMR::traceFileName,		(string *)$4); $$ = $1; }
		|	smr_descr Show_mode			'=' smr_show_mode	{ ((RDOSMR *)$1)->setShowMode((ShowMode)$4); $$ = $1; }
		|	smr_descr Frame_number		'=' INT_CONST		{ ((RDOSMR *)$1)->setFrameNumber($4); $$ = $1; }
		|	smr_descr Show_rate			'=' REAL_CONST		{ ((RDOSMR *)$1)->setValue("Show_rate",		&RDOSMR::showRate,			(double *)$4); $$ = $1; }
		|	smr_descr Run_StartTime		'=' REAL_CONST		{ ((RDOSMR *)$1)->setValue("Run_StartTime",	&RDOSMR::runStartTime,		(double *)$4); $$ = $1; }
		|	smr_descr Trace_StartTime	'=' REAL_CONST		{ ((RDOSMR *)$1)->setValue("Trace_StartTime",&RDOSMR::traceStartTime,	(double *)$4); $$ = $1; }
		|	smr_descr Trace_EndTime		'=' REAL_CONST		{ ((RDOSMR *)$1)->setValue("Trace_EndTime",	&RDOSMR::traceEndTime,		(double *)$4); $$ = $1; };

smr_show_mode:		NoShow		{ $$ = SM_NoShow;		}
					|	Monitor 		{ $$ = SM_Monitor;	}
					|	Animation	{ $$ = SM_Animation; };

%%

}
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

#include "rdoparser.h"
#include "rdoparselex.h"
#include "rdortp.h"

namespace rdoParse 
{
%}

%%

rtp_list:
         | rtp_list rtp_res_type;

rtp_res_type_hdr:	Resource_type IDENTIF_COLON rtp_vid_res {
	string *name = (string *)$2;
	if(currParser->findRTPResType(name))
		currParser->error(("Second appearance of the same resource type : " + *(name)).c_str());

	RDORTPResType *res = new RDORTPResType(name, $3 != 0, currParser->resourceTypeCounter++);
	currParser->allRTPResType.push_back(res);
	currParser->lastRTPResType = res;
	$$ = (int)res;
};

rtp_res_type:  rtp_res_type_hdr Parameters rtp_body End;

rtp_vid_res: permanent	{ $$ = 1; }
         | temporary		{ $$ = 0; };

rtp_body:			
         |  rtp_body rtp_param_desc		{
							RDORTPParamDesc *param = (RDORTPParamDesc*)$2;
							currParser->lastRTPResType->add(param);
						};


rtp_param_desc: IDENTIF_COLON rtp_param_type	{
						string *name = (string *)$1;
						RDORTPResParam *parType = (RDORTPResParam *)$2;
						RDORTPParamDesc *param = new RDORTPParamDesc(name, parType);
						currParser->allRTPParamDesc.push_back(param);
						$$ = (int)param;
					};
         
rtp_enum_default_val:	{	$$ = (int)(new RDORTPEnumDefVal()); }
         | '=' IDENTIF	{	$$ = (int)(new RDORTPEnumDefVal((string *)$2)); };

rtp_real_default_val:	{	$$ = (int)(new RDORTPRealDefVal());	}
         | '=' REAL_CONST {$$ = (int)(new RDORTPRealDefVal(*((double *)$2)));	};

rtp_int_default_val:	{	$$ = (int)(new RDORTPIntDefVal());	}
         | '=' INT_CONST	{	$$ = (int)(new RDORTPIntDefVal($2)); };

rtp_param_type: integer rtp_int_diap rtp_int_default_val  { 
                                                RDORTPIntDiap *diap = (RDORTPIntDiap *)$2;
                                                RDORTPIntDefVal *dv = (RDORTPIntDefVal *)$3;
                                                RDORTPIntResParam *rp = new RDORTPIntResParam(diap, dv);
                                                $$ = (int)rp;
															}
                   | real rtp_real_diap  rtp_real_default_val		{ 
                                                RDORTPRealDiap *diap = (RDORTPRealDiap *)$2;
                                                RDORTPRealDefVal *dv = (RDORTPRealDefVal *)$3;
                                                RDORTPRealResParam *rp = new RDORTPRealResParam(diap, dv);
                                                $$ = (int)rp;
															}

                   | rtp_enum rtp_enum_default_val		{ 
                                                RDORTPEnum *enu = (RDORTPEnum *)$1;
                                                RDORTPEnumDefVal *dv = (RDORTPEnumDefVal *)$2;
																if(dv->exist)
																	enu->findValue(dv->value);	 // if no value - Syntax exception will be thrown
                                                RDORTPEnumResParam *rp = new RDORTPEnumResParam(enu, dv);
                                                $$ = (int)rp;
															}

                   | rtp_such_as		{
											RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
											$$ = (int)desc->getType()->constructSuchAs();
										}
                   | rtp_such_as	'=' INT_CONST	{
											RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
											$$ = (int)desc->getType()->constructSuchAs((int)$3);
										}
                   | rtp_such_as	'=' REAL_CONST	{
											RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
											$$ = (int)desc->getType()->constructSuchAs((double *)$3);
										}
                   | rtp_such_as	'=' IDENTIF	{
											RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
											$$ = (int)desc->getType()->constructSuchAs((string *)$3);
										};


rtp_int_diap:	{
						RDORTPIntDiap *diap = new RDORTPIntDiap();
						$$ = (int)diap;
					}
         | '[' INT_CONST dblpoint INT_CONST ']' {
						RDORTPIntDiap *diap = new RDORTPIntDiap($2, $4);
						$$ = (int)diap;
					}
					;

rtp_real_diap: {
						RDORTPRealDiap *diap = new RDORTPRealDiap();
						$$ = (int)diap;
					}
         | '[' REAL_CONST dblpoint REAL_CONST ']'	{
						double min = *((double *)$2);
						double max = *((double *)$4);
						RDORTPRealDiap *diap = new RDORTPRealDiap(min, max);
						$$ = (int)diap;
					}
					;


rtp_enum:   '(' rtp_enum_list ')'	{ $$ = $2; };
         
rtp_enum_list:  IDENTIF		{
							RDORTPEnum *enu = new RDORTPEnum((string *)$1);
							$$ = (int)enu;
						}
         | rtp_enum_list ',' IDENTIF	{
							RDORTPEnum *enu = (RDORTPEnum *)$1;
							enu->add((string *)$3);
							$$ = (int)enu;
						};


rtp_such_as:   such_as IDENTIF '.' IDENTIF {
							string *type = (string *)$2;
							string *param = (string *)$4;
							const RDORTPResType *const rt = currParser->findRTPResType(type);
							if(!rt)
								currParser->error(("Invalid resource type in such_as: " + *type).c_str());

							const RDORTPParamDesc *const rp = rt->findRTPParam(param);
							if(!rp)
								currParser->error(("Invalid resource parameter in such_as: " + *param).c_str());
								
							$$ = (int)rp;
						};
	

%%

}
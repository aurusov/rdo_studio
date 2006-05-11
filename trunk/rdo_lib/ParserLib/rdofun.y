%{
#define YYPARSE_PARAM lexer
#define YYLEX_PARAM lexer
%}

%pure-parser

%token Resource_type		257
%token permanent			258
%token Parameters			259
%token integer				260
%token real					261
%token End					262
%token temporary			263
%token IDENTIF				264
%token INT_CONST			265
%token REAL_CONST			266
%token such_as				267
%token dblpoint				268
%token Resources			269
%token trace_keyword		270
%token no_trace				271
%token IDENTIF_COLON		272
%token Constant				273
%token Body					274
%token Function_keyword		275
%token Type_keyword			276
%token algorithmic			277
%token table_keyword		278
%token list_keyword			279
%token Calculate_if			280
%token Exist				281
%token Not_Exist			282
%token For_All				283
%token Not_For_All			284
%token neq					285
%token leq					286
%token geq					287
%token NoCheck				288
%token Calculate_if			289
%token or_keyword			290
%token and_keyword			291
%token Sequence				292
%token uniform				293
%token exponential			294
%token normal_keyword		295
%token by_hist				296
%token enumerative			297

%token Pattern				298
%token operation			299
%token irregular_event		300
%token rule_keyword			301
%token keyboard				302
%token Relevant_resources	303
%token Keep					304
%token Create_keyword		305
%token Erase				306
%token NonExist				307
%token IDENTIF_NoChange		308
%token Time					309
%token Choice				310
%token from_keyword			311
%token first_keyword		312
%token Convert_begin		313
%token Convert_end			314
%token Convert_rule			315
%token Convert_event		316
%token with_max				317
%token with_min				318
%token IDENTIF_set			319
%token IDENTIF_NoChange_NoChange	320
%token Operations			321
	
%token Results				322
%token watch_par			323
%token watch_state			324
%token watch_quant			325
%token watch_value			326
%token get_value			327

%token Model_name			328
%token Resource_file		329
%token OprIev_file			330
%token Frame_file			331
%token Statistic_file		332
%token Results_file			333
%token Trace_file			334
%token Show_mode			335
%token Frame_number			336
%token Show_rate			337
%token Run_StartTime		338
%token Trace_StartTime		339
%token Trace_EndTime		340
%token Terminate_if			341
%token Break_point			342
%token Seed					343
%token NoShow				344
%token Monitor				345
%token Animation			346
%token NoChange				347

%token Decision_point		348
%token search_keyword		349
%token trace_stat			350
%token trace_tops			351
%token trace_all			352
%token Condition_keyword	353
%token Term_condition		354
%token Evaluate_by			355
%token Compare_tops			356
%token NO					357
%token YES					358
%token Activities			359
%token value_before			360
%token value_after			361
%token some					362
%token Process				363
%token SEIZE				364

%token Frame				400
%token Show_if				401
%token Back_picture			402
%token Show					403
%token frm_cell				404
%token text					405
%token transparent			406
%token bitmap				407
%token s_bmp				408
%token rect_keyword			409
%token r_rect				410
%token line					411
%token ellipse				412
%token triang				413
%token active				414
%token QUOTED_IDENTIF		415

%{
#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoparser.h"
#include "rdortp.h"
#include "rdofun.h"

namespace rdoParse 
{
%}

%left or_keyword
%left and_keyword
%left '+' '-'
%left '*' '/'

%%

fun_list:	fun_consts fun_func_seq;

fun_consts:	
			| Constant fun_const_body End;

fun_const_body:			
         |  fun_const_body fun_const_param_desc		{
							RDORTPParamDesc *cons = (RDORTPParamDesc*)$2;
							currParser->addConstant(cons);
							@$;
						};


fun_const_param_desc: IDENTIF_COLON fun_const_param_type	{
						std::string *name = (std::string *)$1;
						RDORTPResParam *parType = (RDORTPResParam *)$2;
						RDORTPParamDesc *param = new RDORTPParamDesc(name, parType);
						currParser->allRTPParamDesc.push_back(param);
						if(!parType->dv->exist)
							currParser->error("Constant must have value");
							
						$$ = (int)param;
					};
         
fun_const_enum_default_val:	'=' IDENTIF	{
						std::string *val = (std::string *)$2;
						RDORTPEnumDefVal *dv = new RDORTPEnumDefVal(val);
						$$ = (int)dv;	  
					};


fun_const_real_default_val:	'=' REAL_CONST	{
						double val = *((double *)$2);
						RDORTPRealDefVal *dv = new RDORTPRealDefVal(val);
						$$ = (int)dv;
					};

fun_const_int_default_val:	'=' INT_CONST	{
						RDORTPIntDefVal *dv = new RDORTPIntDefVal($2);
						$$ = (int)dv;
					}
					;

fun_const_param_type: integer fun_const_int_diap fun_const_int_default_val  { 
                                                RDORTPIntDiap *diap = (RDORTPIntDiap *)$2;
                                                RDORTPIntDefVal *dv = (RDORTPIntDefVal *)$3;
                                                RDORTPIntResParam *rp = new RDORTPIntResParam(diap, dv);
                                                $$ = (int)rp;
															}
                   | integer fun_const_int_diap   { 
                                                RDORTPIntDiap *diap = (RDORTPIntDiap *)$2;
                                                RDORTPIntDefVal *dv = new RDORTPIntDefVal();
                                                RDORTPIntResParam *rp = new RDORTPIntResParam(diap, dv);
                                                $$ = (int)rp;
															}
                   | real fun_const_real_diap  fun_const_real_default_val		{ 
                                                RDORTPRealDiap *diap = (RDORTPRealDiap *)$2;
                                                RDORTPRealDefVal *dv = (RDORTPRealDefVal *)$3;
                                                RDORTPRealResParam *rp = new RDORTPRealResParam(diap, dv);
                                                $$ = (int)rp;
															}
                   | real fun_const_real_diap  { 
                                                RDORTPRealDiap *diap = (RDORTPRealDiap *)$2;
                                                RDORTPRealDefVal *dv = new RDORTPRealDefVal();
                                                RDORTPRealResParam *rp = new RDORTPRealResParam(diap, dv);
                                                $$ = (int)rp;
															}
                   | fun_const_enum fun_const_enum_default_val		{ 
                                                RDORTPEnum *enu = (RDORTPEnum *)$1;
                                                RDORTPEnumDefVal *dv = (RDORTPEnumDefVal *)$2;
																enu->findValue(dv->value);	 // if no value - Syntax exception will be thrown
                                                RDORTPEnumResParam *rp = new RDORTPEnumResParam(enu, dv);
                                                $$ = (int)rp;
															}
                   | fun_const_enum				{ 
                                                RDORTPEnum *enu = (RDORTPEnum *)$1;
                                                RDORTPEnumDefVal *dv = new RDORTPEnumDefVal();
                                                RDORTPEnumResParam *rp = new RDORTPEnumResParam(enu, dv);
                                                $$ = (int)rp;
															}

                   | fun_const_such_as		{
											RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
											$$ = (int)desc->getType()->constructSuchAs();
										}
                   | fun_const_such_as	fun_const_int_default_val	{
											RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
											RDORTPIntDefVal *dv = (RDORTPIntDefVal *)$2;
											$$ = (int)desc->getType()->constructSuchAs((int)dv->val);
										}
                   | fun_const_such_as	fun_const_real_default_val	{
											RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
											RDORTPRealDefVal *dv = (RDORTPRealDefVal *)$2;
											if(!dv->exist)
												$$ = (int)desc->getType()->constructSuchAs();
											else
												$$ = (int)desc->getType()->constructSuchAs((double *)&(dv->val));
										}
                   | fun_const_such_as	fun_const_enum_default_val	{
											RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
											RDORTPEnumDefVal *dv = (RDORTPEnumDefVal *)$2;
											if(!dv->exist)
												$$ = (int)desc->getType()->constructSuchAs();
											else
												$$ = (int)desc->getType()->constructSuchAs((std::string *)dv->value);
										};


fun_const_int_diap:	{
						RDORTPIntDiap *diap = new RDORTPIntDiap();
						$$ = (int)diap;
					}
         | '[' INT_CONST dblpoint INT_CONST ']' {
						RDORTPIntDiap *diap = new RDORTPIntDiap($2, $4);
						$$ = (int)diap;
					}
					;

fun_const_real_diap: {
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


fun_const_enum:   '(' fun_const_enum_list ')'	{ $$ = $2; };
         
fun_const_enum_list:  IDENTIF		{
							RDORTPEnum *enu = new RDORTPEnum((std::string *)$1);
							$$ = (int)enu;
						}
         | fun_const_enum_list ',' IDENTIF	{
							RDORTPEnum *enu = (RDORTPEnum *)$1;
							enu->add((std::string *)$3);
							$$ = (int)enu;
						};


fun_const_such_as:   such_as IDENTIF '.' IDENTIF {
							std::string *type = (std::string *)$2;
							std::string *param = (std::string *)$4;
							const RDORTPResType *const rt = currParser->findRTPResType(type);
							if(!rt)
								currParser->error("Invalid resource type in such_as: " + *type);

							const RDORTPParamDesc *const rp = rt->findRTPParam(param);
							if(!rp)
								currParser->error("Invalid resource parameter in such_as: " + *param);
								
							$$ = (int)rp;
						} 
					| such_as IDENTIF {
							std::string *constName = (std::string *)$2;
							const RDOFUNConstant *const cons = currParser->findFUNConst(constName);
							if(!cons)
								currParser->error("Invalid constant reference: " + *constName);
								
							$$ = (int)(cons->descr);
						}; 



fun_func_seq: 
			| fun_func_seq fun_func_descr
			| fun_func_seq fun_seq_descr;

fun_func_descr:	fun_func_header fun_func_footer;

fun_func_header:	Function_keyword IDENTIF_COLON fun_const_param_type	{
	std::string *name = (std::string *)$2;
	if(currParser->findFunction(name))
		currParser->error(("Second appearance of the same function: " + *(name)).c_str());

	RDORTPResParam *retType = (RDORTPResParam *)$3;
	RDOFUNFunction *fun = new RDOFUNFunction(name, retType);
	currParser->allFUNFunctions.push_back(fun);
	currParser->lastFUNFunction = fun;
	$$ = (int)fun;
};


fun_func_footer:	Type_keyword '=' algorithmic Parameters fun_func_params Body fun_func_algorithmic_body End {
	RDOFUNFunction *currFunc = currParser->lastFUNFunction;
	currFunc->createAlgorithmicCalc();
}
			| Type_keyword '=' list_keyword Parameters fun_func_params Body fun_func_list_body End {
	RDOFUNFunction *currFunc = currParser->lastFUNFunction;
	currFunc->createListCalc();
}
			| Type_keyword '=' table_keyword Parameters fun_func_params Body fun_func_list_body End {
	RDOFUNFunction *currFunc = currParser->lastFUNFunction;
	currFunc->createTableCalc();
};

fun_func_params:	
			|	fun_func_params IDENTIF_COLON fun_const_param_type	{
	std::string *name = (std::string *)$2;
	RDORTPResParam *type = (RDORTPResParam *)$3;
	RDOFUNFunctionParam *param = new RDOFUNFunctionParam(name, type);
	currParser->lastFUNFunction->add(param);	// the function must check uniquness of parameters names
	$$ = (int)param;
};

fun_func_list_body:
			| fun_func_list_body fun_func_list_value;

fun_func_list_value:	'='	{
	RDOFUNFunctionListElementEq *value = new RDOFUNFunctionListElementEq();
	currParser->lastFUNFunction->add(value);
	$$ = (int)value;
}
			| fun_std_value	{ $$ = $1; };

fun_std_value: IDENTIF	{
	RDOFUNFunctionListElementItentif *value = new RDOFUNFunctionListElementItentif((std::string *)$1);
	currParser->lastFUNFunction->add(value);
	$$ = (int)value;
}
			| REAL_CONST	{
	RDOFUNFunctionListElementReal *value = new RDOFUNFunctionListElementReal((double *)$1);
	currParser->lastFUNFunction->add(value);
	$$ = (int)value;
}
			| INT_CONST		{
	RDOFUNFunctionListElementInt *value = new RDOFUNFunctionListElementInt((int)$1);
	currParser->lastFUNFunction->add(value);
	$$ = (int)value;
};

fun_func_algorithmic_body: 
			| fun_func_algorithmic_body fun_func_algorithmic_calc_if { currParser->lastFUNFunction->add((RDOFUNCalculateIf*)$2); };

fun_func_algorithmic_calc_if:	Calculate_if fun_logic 	IDENTIF '=' fun_arithm { $$ = (int)(new RDOFUNCalculateIf((RDOFUNLogic *)$2, (std::string *)$3, (RDOFUNArithm *)$5)); };

fun_logic: fun_arithm '=' fun_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 == *(RDOFUNArithm *)$3); }
			| fun_arithm neq fun_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 != *(RDOFUNArithm *)$3); }
			| fun_arithm '<' fun_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 <  *(RDOFUNArithm *)$3); }
			| fun_arithm '>' fun_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 >  *(RDOFUNArithm *)$3); }
			| fun_arithm leq fun_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 <= *(RDOFUNArithm *)$3); }
			| fun_arithm geq fun_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 >= *(RDOFUNArithm *)$3); }
			| fun_logic and_keyword fun_logic  { $$ = (int)(*(RDOFUNLogic *)$1 && *(RDOFUNLogic *)$3); }
			| fun_logic or_keyword fun_logic	  { $$ = (int)(*(RDOFUNLogic *)$1 || *(RDOFUNLogic *)$3); }
			| '[' fun_logic ']'				{ $$ = $2; }
			| fun_group							{ $$ = $1; };

			
fun_arithm: fun_arithm '+' fun_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 + *(RDOFUNArithm *)$3); }
			|	fun_arithm '-' fun_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 - *(RDOFUNArithm *)$3); }
			|	fun_arithm '*' fun_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 * *(RDOFUNArithm *)$3); }
			|	fun_arithm '/' fun_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 / *(RDOFUNArithm *)$3); }
			|	'(' fun_arithm ')'			{ $$ = $2; }
			|	fun_arithm_func_call
			|	IDENTIF '.' IDENTIF			{ $$ = (int)(new RDOFUNArithm((std::string *)$1, (std::string *)$3)); }
			|	INT_CONST						{ $$ = (int)(new RDOFUNArithm((int)$1)); }
			|	REAL_CONST						{ $$ = (int)(new RDOFUNArithm((double*)$1)); }
			|	IDENTIF							{ $$ = (int)(new RDOFUNArithm((std::string *)$1)); };

fun_arithm_func_call:	IDENTIF '(' fun_arithm_func_call_pars ')' { $$ = (int)((RDOFUNParams *)$3)->createCall((std::string *)$1) };

fun_arithm_func_call_pars:								{ $$ = (int)(new RDOFUNParams()); };
			| fun_arithm_func_call_pars fun_arithm	{ $$ = (int)(((RDOFUNParams *)$1)->addParameter((RDOFUNArithm *)$2)); };
			| fun_arithm_func_call_pars ',' fun_arithm	{ $$ = (int)(((RDOFUNParams *)$1)->addParameter((RDOFUNArithm *)$3)); };


fun_group_keyword:	Exist			{ $$ = 1; }
						|	Not_Exist	{ $$ = 2; }
						|	For_All		{ $$ = 3; }
						|	Not_For_All	{ $$ = 4; };

fun_group_header:	fun_group_keyword '(' IDENTIF_COLON { $$ = (int)(new RDOFUNGroup($1, (std::string *)$3)); };

fun_group:	fun_group_header fun_logic ')'		{ $$ = (int)(((RDOFUNGroup *)$1)->createFunLogin((RDOFUNLogic *)$2)); }
					|	fun_group_header NoCheck ')'	{ $$ = (int)(((RDOFUNGroup *)$1)->createFunLogin()); };





fun_seq_descr:	fun_seq_uniform
			| fun_seq_exponential
			| fun_seq_normal
			| fun_seq_by_hist
			| fun_seq_enumerative;

fun_seq_header:	Sequence IDENTIF_COLON fun_const_param_type Type_keyword '=' { $$ = (int)(new RDOFUNSequenceHeader((std::string *)$2, (RDORTPResParam *)$3)); };

fun_seq_uniform:	fun_seq_header uniform End	{ $$ = (int)(new RDOFUNSequenceUniform((RDOFUNSequenceHeader *)$1)); }
			| fun_seq_header uniform INT_CONST End { $$ = (int)(new RDOFUNSequenceUniform((RDOFUNSequenceHeader *)$1, $3)); };

fun_seq_exponential:	fun_seq_header exponential End { $$ = (int)(new RDOFUNSequenceExponential((RDOFUNSequenceHeader *)$1)); }
			| fun_seq_header exponential INT_CONST End { $$ = (int)(new RDOFUNSequenceExponential((RDOFUNSequenceHeader *)$1, $3)); };

fun_seq_normal:	fun_seq_header normal_keyword End { $$ = (int)(new RDOFUNSequenceNormal((RDOFUNSequenceHeader *)$1)); }
			| fun_seq_header normal_keyword INT_CONST End { $$ = (int)(new RDOFUNSequenceNormal((RDOFUNSequenceHeader *)$1, $3)); };


fun_seq_by_hist_header:	fun_seq_header by_hist Body { $$ = (int)(new RDOFUNSequenceByHistHeader((RDOFUNSequenceHeader *)$1)); }
			| fun_seq_header by_hist INT_CONST Body	 { $$ = (int)(new RDOFUNSequenceByHistHeader((RDOFUNSequenceHeader *)$1, $3)); };

fun_seq_by_hist_body_int:	fun_seq_by_hist_header INT_CONST INT_CONST REAL_CONST		{ $$ = (int)(new RDOFUNSequenceByHistInt((RDOFUNSequenceByHistHeader *)$1, $2, $3, (double*)$4)); } 
			| fun_seq_by_hist_body_int INT_CONST INT_CONST REAL_CONST						{ ((RDOFUNSequenceByHistInt *)$1)->addInt($2, $3, (double*)$4); $$ = $1; } ;

fun_seq_by_hist_body_real:	fun_seq_by_hist_header REAL_CONST REAL_CONST REAL_CONST	{ $$ = (int)(new RDOFUNSequenceByHistReal((RDOFUNSequenceByHistHeader *)$1, (double*)$2, (double*)$3, (double*)$4)); } 
			| fun_seq_by_hist_body_real REAL_CONST REAL_CONST REAL_CONST					{ ((RDOFUNSequenceByHistReal *)$1)->addReal((double*)$2, (double*)$3, (double*)$4); $$ = $1; };

fun_seq_by_hist_body_enum:	fun_seq_by_hist_header IDENTIF REAL_CONST						{ $$ = (int)(new RDOFUNSequenceByHistEnum((RDOFUNSequenceByHistHeader *)$1, (std::string*)$2, (double*)$3)); } 
			| fun_seq_by_hist_body_enum IDENTIF REAL_CONST										{ ((RDOFUNSequenceByHistEnum *)$1)->addEnum((std::string*)$2, (double*)$3); $$ = $1; };

fun_seq_by_hist:	fun_seq_by_hist_body_int End	{ ((RDOFUNSequenceByHist *)$1)->createCalcs(); }
			| fun_seq_by_hist_body_real End			{ ((RDOFUNSequenceByHist *)$1)->createCalcs(); }
			| fun_seq_by_hist_body_enum End			{ ((RDOFUNSequenceByHist *)$1)->createCalcs(); };


fun_seq_enumerative_header:	fun_seq_header enumerative Body { $$ = (int)(new RDOFUNSequenceEnumerativeHeader((RDOFUNSequenceHeader *)$1)); }     
			| fun_seq_header enumerative INT_CONST Body			  { $$ = (int)(new RDOFUNSequenceEnumerativeHeader((RDOFUNSequenceHeader *)$1, $3)); };

fun_seq_enumerative_body_int:	fun_seq_enumerative_header INT_CONST		{ $$ = (int)(new RDOFUNSequenceEnumerativeInt((RDOFUNSequenceEnumerativeHeader *)$1, $2)); }                    
			| fun_seq_enumerative_body_int INT_CONST 								{ ((RDOFUNSequenceEnumerativeInt *)$1)->addInt($2); $$ = $1; } ;                                           
																								                                                                                                                       
fun_seq_enumerative_body_real:	fun_seq_enumerative_header REAL_CONST 	{ $$ = (int)(new RDOFUNSequenceEnumerativeReal((RDOFUNSequenceEnumerativeHeader *)$1, (double*)$2)); } 
			| fun_seq_enumerative_body_real REAL_CONST 							{ ((RDOFUNSequenceEnumerativeReal *)$1)->addReal((double*)$2); $$ = $1; };                        
																								                                                                                                                       
fun_seq_enumerative_body_enum:	fun_seq_enumerative_header IDENTIF 		{ $$ = (int)(new RDOFUNSequenceEnumerativeEnum((RDOFUNSequenceEnumerativeHeader *)$1, (std::string*)$2)); }         
			| fun_seq_enumerative_body_enum IDENTIF 								{ ((RDOFUNSequenceEnumerativeEnum *)$1)->addEnum((std::string*)$2); $$ = $1; };                                

fun_seq_enumerative:	fun_seq_enumerative_body_int End		{ ((RDOFUNSequenceEnumerative *)$1)->createCalcs(); }  
			| fun_seq_enumerative_body_real End					{ ((RDOFUNSequenceEnumerative *)$1)->createCalcs(); }  
			| fun_seq_enumerative_body_enum End 				{ ((RDOFUNSequenceEnumerative *)$1)->createCalcs(); }; 





%%

}
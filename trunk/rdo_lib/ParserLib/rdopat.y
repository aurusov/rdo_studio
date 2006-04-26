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
%token SIEZE				364

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
#include "rdoparselex.h"
#include "rdopat.h"
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

pat_list:	
			| pat_list pat_pattern;


pat_header:	Pattern IDENTIF_COLON operation			pat_trace	{ $$ = (int)(new RDOPATPatternOperation(	(string *)$2, $4 != 0, currParser->patternCounter++)); }
			|	Pattern IDENTIF_COLON irregular_event	pat_trace	{ $$ = (int)(new RDOPATPatternEvent(		(string *)$2, $4 != 0, currParser->patternCounter++)); }
			|	Pattern IDENTIF_COLON rule_keyword		pat_trace	{ $$ = (int)(new RDOPATPatternRule(			(string *)$2, $4 != 0, currParser->patternCounter++)); }
			|	Pattern IDENTIF_COLON keyboard			pat_trace	{ $$ = (int)(new RDOPATPatternKeyboard(	(string *)$2, $4 != 0, currParser->patternCounter++)); };

pat_trace:						{ $$ = 0; }
			| trace_keyword	{ $$ = 1; }
			| no_trace			{ $$ = 0; };


pat_params_begin:	pat_header Parameters	{ $$ = $1; };

pat_params:	pat_params_begin IDENTIF_COLON pat_type	{ ((RDOPATPattern *)$1)->add(new RDOFUNFunctionParam((string *)$2, (RDORTPResParam *)$3)); $$ = $1; }
			| pat_params IDENTIF_COLON pat_type				{ ((RDOPATPattern *)$1)->add(new RDOFUNFunctionParam((string *)$2, (RDORTPResParam *)$3)); $$ = $1; };	

pat_params_end: pat_params Relevant_resources	{ $$ = $1; }
			| 		 pat_header Relevant_resources	{ $$ = $1; };


pat_rel_res:	pat_params_end IDENTIF_COLON IDENTIF pat_conv pat_conv	{ ((RDOPATPattern *)$1)->addRelRes((string *)$2, (string *)$3, (ConvertStatus)$4,	(ConvertStatus)$5);	$$ = $1; }
			|		pat_rel_res		IDENTIF_COLON IDENTIF pat_conv pat_conv	{ ((RDOPATPattern *)$1)->addRelRes((string *)$2, (string *)$3, (ConvertStatus)$4,	(ConvertStatus)$5);	$$ = $1; }
			|		pat_params_end IDENTIF_COLON IDENTIF pat_conv 				{ ((RDOPATPattern *)$1)->addRelRes((string *)$2, (string *)$3, (ConvertStatus)$4);								$$ = $1; }
			|		pat_rel_res		IDENTIF_COLON IDENTIF pat_conv				{ ((RDOPATPattern *)$1)->addRelRes((string *)$2, (string *)$3, (ConvertStatus)$4);								$$ = $1; }
			|		pat_params_end IDENTIF_COLON IDENTIF_NoChange pat_conv	{ ((RDOPATPattern *)$1)->addRelRes((string *)$2, (string *)$3, CS_NoChange,			(ConvertStatus)$4);	$$ = $1; }
			|		pat_rel_res		IDENTIF_COLON IDENTIF_NoChange pat_conv	{ ((RDOPATPattern *)$1)->addRelRes((string *)$2, (string *)$3, CS_NoChange,			(ConvertStatus)$4);	$$ = $1; }
			|		pat_params_end IDENTIF_COLON IDENTIF_NoChange_NoChange	{ ((RDOPATPattern *)$1)->addRelRes((string *)$2, (string *)$3, CS_NoChange,			CS_NoChange);			$$ = $1; }
			|		pat_rel_res		IDENTIF_COLON IDENTIF_NoChange_NoChange	{ ((RDOPATPattern *)$1)->addRelRes((string *)$2, (string *)$3, CS_NoChange,			CS_NoChange);			$$ = $1; }
			|		pat_params_end IDENTIF_COLON IDENTIF_NoChange 				{ ((RDOPATPattern *)$1)->addRelRes((string *)$2, (string *)$3, CS_NoChange);										$$ = $1; }
			|		pat_rel_res		IDENTIF_COLON IDENTIF_NoChange				{ ((RDOPATPattern *)$1)->addRelRes((string *)$2, (string *)$3, CS_NoChange);										$$ = $1; }
			|		pat_params_end IDENTIF_COLON IDENTIF IDENTIF_NoChange 	{ ((RDOPATPattern *)$1)->addRelRes((string *)$2, (string *)$3, (string *)$4);										$$ = $1; }
			|		pat_rel_res		IDENTIF_COLON IDENTIF IDENTIF_NoChange 	{ ((RDOPATPattern *)$1)->addRelRes((string *)$2, (string *)$3, (string *)$4);										$$ = $1; };

pat_conv:	Keep				{ $$ = CS_Keep;		}
			|	Create_keyword	{ $$ = CS_Create;		}
			|  Erase				{ $$ = CS_Erase;		}
			|  NonExist			{ $$ = CS_NonExist;	};

pat_common_choice: pat_rel_res
			| pat_rel_res first_keyword			{	((RDOPATPattern *)$1)->setCommonChoiceFirst(); $$ = $1; }
			| pat_rel_res with_min pat_arithm	{	((RDOPATPattern *)$1)->setCommonChoiceWithMin((RDOFUNArithm *)$3); $$ = $1; }
			| pat_rel_res with_max pat_arithm	{	((RDOPATPattern *)$1)->setCommonChoiceWithMax((RDOFUNArithm *)$3); $$ = $1; };

pat_time:	pat_common_choice	Body							{	$$ = $1; }
			| pat_common_choice Time '=' pat_arithm Body {	((RDOPATPattern *)$1)->setTime((RDOFUNArithm *)$4); $$ = $1; };


pat_body:	pat_time	IDENTIF		{	((RDOPATPattern *)$1)->addRelResBody((string *)$2); $$ = $1; }
			|	pat_convert IDENTIF	{	((RDOPATPattern *)$1)->addRelResBody((string *)$2); $$ = $1; };

pat_res_usage: pat_body	pat_choice pat_first	{	((RDOPATPattern *)$1)->addRelResUsage((RDOPATChoice *)$2, (RDOPATFirst *)$3); $$ = $1; };

pat_choice: 										{	$$ = (int) new RDOPATChoice(); }
			|	Choice NoCheck						{	$$ = (int) new RDOPATChoice(1); }
			|	Choice from_keyword pat_logic {	$$ = (int) new RDOPATChoice((RDOFUNLogic *)$3); };

pat_first:									{	$$ = (int) new RDOPATFirst(); }                  
			|	first_keyword				{	$$ = (int) new RDOPATFirst(1); }                 
			|	with_min pat_arithm		{	$$ = (int) new RDOPATFirst(true, (RDOFUNArithm *)$2); }
			|	with_max pat_arithm		{	$$ = (int) new RDOPATFirst(false, (RDOFUNArithm *)$2); };

pat_convert:  pat_res_usage	{	((RDOPATPattern *)$1)->addRelResConvert(); $$ = $1; }
			|	pat_res_usage Convert_begin pat_trace pat_params_set	
					{	((RDOPATPattern *)$1)->addRelResConvertBegin($3 != 0, (RDOPATParamsSet *)$4); $$ = $1; }
			|	pat_res_usage Convert_end		pat_trace pat_params_set
					{	((RDOPATPattern *)$1)->addRelResConvertEnd($3 != 0, (RDOPATParamsSet *)$4); $$ = $1; }
			|	pat_res_usage Convert_begin	pat_trace pat_params_set Convert_end pat_trace pat_params_set
					{	((RDOPATPattern *)$1)->addRelResConvertBeginEnd($3 != 0, (RDOPATParamsSet *)$4, $6 != 0, (RDOPATParamsSet *)$7); $$ = $1; }
			|	pat_res_usage Convert_rule	pat_trace pat_params_set
					{	((RDOPATPattern *)$1)->addRelResConvertRule($3 != 0, (RDOPATParamsSet *)$4); $$ = $1; }
			|	pat_res_usage Convert_event	pat_trace pat_params_set
					{	((RDOPATPattern *)$1)->addRelResConvertEvent($3 != 0, (RDOPATParamsSet *)$4); $$ = $1; };

pat_params_set:												{  $$ = (int) new RDOPATParamsSet(); }
			|	pat_params_set IDENTIF_set pat_arithm	{	((RDOPATParamsSet *)$1)->addIdentif((string *)$2, (RDOFUNArithm *)$3); $$ = $1;}
			|	pat_params_set IDENTIF_NoChange			{	((RDOPATParamsSet *)$1)->addIdentif((string *)$2); $$ = $1;};

pat_pattern:	pat_convert		End {	((RDOPATPattern *)$1)->end(); $$ = $1;}
			|		pat_time			End {	((RDOPATPattern *)$1)->end(); $$ = $1;};


							 


pat_type: integer fun_const_int_diap fun_const_int_default_val  { 
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
							$$ = (int)desc->getType()->constructSuchAs((string *)dv->value);
					};


fun_const_enum_default_val:	'=' IDENTIF	{
						string *val = (string *)$2;
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
							RDORTPEnum *enu = new RDORTPEnum((string *)$1);
							$$ = (int)enu;
						}
         | fun_const_enum_list ',' IDENTIF	{
							RDORTPEnum *enu = (RDORTPEnum *)$1;
							enu->add((string *)$3);
							$$ = (int)enu;
						};


fun_const_such_as:   such_as IDENTIF '.' IDENTIF {
							string *type = (string *)$2;
							string *param = (string *)$4;
							const RDORTPResType *const rt = currParser->findRTPResType(type);
							if(!rt)
								currParser->error(("Invalid resource type in such_as: " + *type).c_str());

							const RDORTPParamDesc *const rp = rt->findRTPParam(param);
							if(!rp)
								currParser->error(("Invalid resource parameter in such_as: " + *param).c_str());
								
							$$ = (int)rp;
						} 
					| such_as IDENTIF {
							string *constName = (string *)$2;
							const RDOFUNConstant *const cons = currParser->findFUNConst(constName);
							if(!cons)
								currParser->error(("Invalid constant reference: " + *constName).c_str());
								
							$$ = (int)(cons->descr);
						}; 



pat_logic: pat_arithm '=' pat_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 == *(RDOFUNArithm *)$3); }
			| pat_arithm neq pat_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 != *(RDOFUNArithm *)$3); }
			| pat_arithm '<' pat_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 <  *(RDOFUNArithm *)$3); }
			| pat_arithm '>' pat_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 >  *(RDOFUNArithm *)$3); }
			| pat_arithm leq pat_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 <= *(RDOFUNArithm *)$3); }
			| pat_arithm geq pat_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 >= *(RDOFUNArithm *)$3); }
			| pat_logic and_keyword pat_logic  { $$ = (int)(*(RDOFUNLogic *)$1 && *(RDOFUNLogic *)$3); }
			| pat_logic or_keyword pat_logic	  { $$ = (int)(*(RDOFUNLogic *)$1 || *(RDOFUNLogic *)$3); }
			| '[' pat_logic ']'				{ $$ = $2; }
			| fun_group							{ $$ = $1; };

			
pat_arithm: pat_arithm '+' pat_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 + *(RDOFUNArithm *)$3); }
			|	pat_arithm '-' pat_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 - *(RDOFUNArithm *)$3); }
			|	pat_arithm '*' pat_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 * *(RDOFUNArithm *)$3); }
			|	pat_arithm '/' pat_arithm	{ $$ = (int)(*(RDOFUNArithm *)$1 / *(RDOFUNArithm *)$3); }
			|	'(' pat_arithm ')'			{ $$ = $2; }
			|	pat_arithm_func_call
			|	IDENTIF '.' IDENTIF			{ $$ = (int)(new RDOFUNArithm((string *)$1, (string *)$3)); }
			|	INT_CONST						{ $$ = (int)(new RDOFUNArithm((int)$1)); }
			|	REAL_CONST						{ $$ = (int)(new RDOFUNArithm((double*)$1)); }
			|	IDENTIF							{ $$ = (int)(new RDOFUNArithm((string *)$1)); };

pat_arithm_func_call:	IDENTIF '(' pat_arithm_func_call_pars ')' { $$ = (int)((RDOFUNParams *)$3)->createCall((string *)$1) };

pat_arithm_func_call_pars:								{ $$ = (int)(new RDOFUNParams()); };
			| pat_arithm_func_call_pars pat_arithm	{ $$ = (int)(((RDOFUNParams *)$1)->addParameter((RDOFUNArithm *)$2)); };
			| pat_arithm_func_call_pars ',' pat_arithm	{ $$ = (int)(((RDOFUNParams *)$1)->addParameter((RDOFUNArithm *)$3)); };


fun_group_keyword:	Exist			{ $$ = 1; }
						|	Not_Exist	{ $$ = 2; }
						|	For_All		{ $$ = 3; }
						|	Not_For_All	{ $$ = 4; };

fun_group_header:	fun_group_keyword '(' IDENTIF_COLON { $$ = (int)(new RDOFUNGroup($1, (string *)$3)); };

fun_group:	fun_group_header pat_logic ')'		{ $$ = (int)(((RDOFUNGroup *)$1)->createFunLogin((RDOFUNLogic *)$2)); }
					|	fun_group_header NoCheck ')'	{ $$ = (int)(((RDOFUNGroup *)$1)->createFunLogin()); };






%%

}
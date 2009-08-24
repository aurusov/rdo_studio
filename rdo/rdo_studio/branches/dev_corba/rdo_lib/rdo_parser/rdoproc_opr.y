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
%token RDO_IDENTIF_set					319
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
%token RDO_Priority                     372

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
#include "pch.h"

#include "rdoparser.h"
#include "rdoparser_lexer.h"
#include "rdofun.h"
#include "rdodpt.h"
#include "rdortp.h"
#include "rdorss.h"

#define PARSER  reinterpret_cast<rdoParse::RDOLexer*>(lexer)->m_parser
#define RUNTIME PARSER->runtime()

namespace rdoParse 
{
%}

%left RDO_or
%left RDO_and
%left '+' '-'
%left '*' '/'
%left RDO_not
%left RDO_UMINUS

%start dptrtp_main

%%

/* ///////////////////////  GENERAL PART ///////////////////////////// */
dptrtp_main:
	| dptrtp_main RDO_Decision_point error RDO_End /* заглушка для $Decision_point */
	| dptrtp_main RDO_Activities error RDO_End     /* заглушка для $Activities     */
	| dptrtp_main dpt_process_end;

/* ///////////////////////  PROCESS ///////////////////////////// */

dpt_process:		dpt_process_begin dpt_process_input;

dpt_process_begin:	RDO_Process 
					{
						RDOPROCProcess* proc = PARSER->getLastPROCProcess();
						if ( proc && !proc->closed() ) {
							PARSER->error( "Незакрыт предыдущий блок $Process" );
						}
						proc = new RDOPROCProcess( PARSER, "Process" );
						@$ = @1;
					};

dpt_process_input:	/* empty */
					| dpt_process_input dpt_process_line;

dpt_process_line:	RDO_IDENTIF	
					{
						PARSER->error( rdo::format("Неизвестный оператор '%s'", reinterpret_cast<RDOValue*>($1)->value().getIdentificator().c_str()) );
					}
					| RDO_GENERATE fun_arithm 
					{
						int time = ((RDOFUNArithm*)$2)->createCalc()->calcValue(RUNTIME).getInt();
						std::string rtp_name       = "Транзакты";
						std::string rtp_param_name = "Время_создания";

						// Получили список всех типов ресурсов
						rdoMBuilder::RDOResTypeList rtpList( PARSER );
						// Найти тип ресурса, если его нет, то создать
							if ( !rtpList[rtp_name].exist() )
							{
								// Создадим тип ресурса
								rdoMBuilder::RDOResType rtp(rtp_name);
								// Добавим параметр Время_создания
								rtp.m_params.append( rdoMBuilder::RDOResType::Param(rtp_param_name, rdoRuntime::RDOType::t_real) );
								// Добавим тип ресурса
								if ( !rtpList.append( rtp ) )
								{
									PARSER->error( @2, rdo::format("Ошибка создания типа ресурса: %s", rtp_name.c_str()) );
								}
								rdoRuntime::RDOPROCTransact::typeID = rtp.id();
							}
							else
							{
								// Тип найден, проверим его на наличие вещественного параметра
								const rdoMBuilder::RDOResType& rtp = rtpList[rtp_name];
								if ( !rtp.m_params[rtp_param_name].exist() ) {
									PARSER->error( rdo::format( "У типа ресурса '%s' нет требуемого параметра '%s'", rtp.name().c_str(), rtp_param_name.c_str() ) );
								}
								// Параметр есть, надо проверить на тип
								if ( rtp.m_params[rtp_param_name].typeID() != rdoRuntime::RDOType::t_real ) {
									PARSER->error( rdo::format( "У типа ресурса '%s' параметр '%s' не является перечислимым типом", rtp.name().c_str(), rtp_param_name.c_str() ) );
								}
								rdoRuntime::RDOPROCTransact::typeID = rtp.id();
							}
						RDOPROCGenerate* generate = new RDOPROCGenerate( PARSER->getLastPROCProcess(), "GENERATE", ((RDOFUNArithm*)$2)->createCalc() );
						$$ = int(generate);
					}
					| RDO_GENERATE fun_arithm error 
					{
						PARSER->error( @2, "Ошибка в арифметическом выражении" );
					}
					| RDO_TERMINATE dpt_term_param
					{
					}
					| RDO_TERMINATE error
					{
						PARSER->error( @1, "Ошибка в параметре оператора TERMINATE" );
					}
					| RDO_ADVANCE fun_arithm 
					{
						RDOPROCAdvance* advance = new RDOPROCAdvance( PARSER->getLastPROCProcess(), "ADVANCE", ((RDOFUNArithm*)$2)->createCalc() );
						$$ = int(advance);
					}
					| RDO_ADVANCE fun_arithm error 
					{
						PARSER->error( @2, "Ошибка в арифметическом выражении" );
					}
					| RDO_QUEUE dpt_queue_param 
					{
						TRACE("QUEUE dpt_queue_param\n");
						RDOPROCQueue* queue  = reinterpret_cast<RDOPROCQueue*>($2);
						queue->create_runtime_Queue( PARSER );
					}
					| RDO_QUEUE error 
					{
						PARSER->error( @1, "Ожидается имя ресурса для сбора статистики по очереди" );
					}
					| RDO_DEPART dpt_depart_param 
					{
						TRACE("DEPART dpt_depart_param\n");
						RDOPROCDepart* depart  = reinterpret_cast<RDOPROCDepart*>($2);
						depart->create_runtime_Depart( PARSER );
					}
					| RDO_DEPART error 
					{
						PARSER->error( @1, "Ожидается имя ресурса для сбора статистики по очереди" );
					}
				/*	| RDO_SEIZE dpt_seize_param 
					{
						TRACE("SEIZE dpt_seize_param\n");
						RDOPROCSeize* seize  = reinterpret_cast<RDOPROCSeize*>($2);
						seize->create_runtime_Seize( PARSER );
					}
					| RDO_SEIZE error				
					{
						PARSER->error(@1, rdo::format("Ожидается имя занимаемого ресурса"));
					}
					| RDO_RELEASE dpt_release_param 
					{
						TRACE("RELEASE dpt_release_param\n");
						RDOPROCRelease* release  = reinterpret_cast<RDOPROCRelease*>($2);
						release->create_runtime_Release( PARSER );
					}
					| RDO_RELEASE error				
					{
						PARSER->error(@1, rdo::format("Ожидается имя освобождаемого ресурса"));
					}*/
					| RDO_SEIZE dpt_seize_param 
					{
						TRACE("SEIZE dpt_seize_param\n");
						RDOPROCSeize* seize  = reinterpret_cast<RDOPROCSeize*>($2);
						seize->create_runtime_Seize( PARSER );	
					}
					| RDO_SEIZE error				
					{
						PARSER->error(@1, rdo::format("Ожидается список ресурсов, объединяемых в блок, через запятую"));
					}
					| RDO_RELEASE dpt_release_param 
					{
						TRACE("RELEASE dpt_release_param\n");
						RDOPROCRelease* release  = reinterpret_cast<RDOPROCRelease*>($2);
						release->create_runtime_Release( PARSER );	
					}
					| RDO_RELEASE error				
					{
						PARSER->error(@1, rdo::format("Ожидается список ресурсов, объединяемых в блок, через запятую"));
					}
					| RDO_ASSIGN dpt_assign_param	  { 	}
					| RDO_ASSIGN error				  { PARSER->error(@1, rdo::format("Ожидается строка изменения параметра"));								};



dpt_queue_param:	RDO_IDENTIF 
					{
						std::string res_name = reinterpret_cast<RDOValue*>($1)->value().getIdentificator();
						TRACE1(_T("%s _good\n"), res_name.c_str());
						RDOPROCQueue* queue = new RDOPROCQueue( PARSER->getLastPROCProcess(), "QUEUE" );
						queue->add_Queue_Resource( res_name );
						$$ = int( queue );
					}
					| RDO_IDENTIF error 
                    {
						PARSER->error( @1, "Ошибка в миени очереди" )
					};     
dpt_depart_param:	RDO_IDENTIF 
					{
						std::string res_name = reinterpret_cast<RDOValue*>($1)->value().getIdentificator();
						TRACE1(_T("%s _good\n"), res_name.c_str());
						RDOPROCDepart* depart = new RDOPROCDepart( PARSER->getLastPROCProcess(), "DEPART" );
						depart->add_Depart_Resource( res_name );
						$$ = int( depart );
					}
					| RDO_IDENTIF error 
                    {
						PARSER->error( @1, "Ошибка в имени ресурса" )
					};     
/*dpt_seize_param:    RDO_IDENTIF 
					{
						std::string res_name = reinterpret_cast<RDOValue*>($1)->value().getIdentificator();
						TRACE1(_T("%s _good\n"), res_name.c_str());
						RDOPROCSeize* seize = new RDOPROCSeize( PARSER->getLastPROCProcess(), "SEIZE");
						seize->add_Seize_Resource(res_name);
						$$ = (int)seize;
					}
                    | RDO_IDENTIF error 
                    {
						PARSER->error( @1, "Ошибка в миени ресурса" )
					};     
dpt_release_param:  RDO_IDENTIF 
					{
						std::string res_name = reinterpret_cast<RDOValue*>($1)->value().getIdentificator();
						TRACE1(_T("%s _good\n"), res_name.c_str());
						RDOPROCRelease* release = new RDOPROCRelease( PARSER->getLastPROCProcess(), "RELEASE");
						release->add_Release_Resource(res_name);
						$$ = (int)release;
					}
					| RDO_IDENTIF error 
					{	
						PARSER->error( @1, "Ошибка в миени ресурса" )
					};	*/				
dpt_term_param:		//empty 
					{
						RDOPROCTerminate* terminate = new RDOPROCTerminate( PARSER->getLastPROCProcess(), "TERMINATE", 0 );
						$$ = int(terminate);					
					}   
					| fun_arithm 
					{
						if(((RDOFUNArithm*)$1)->createCalc()->calcValue(RUNTIME).type().typeID()==rdoRuntime::RDOType::t_int)
						{
							int term = ((RDOFUNArithm*)$1)->createCalc()->calcValue(RUNTIME).getInt();
							RDOPROCTerminate* terminate = new RDOPROCTerminate( PARSER->getLastPROCProcess(), "TERMINATE", term );
							$$ = int(terminate);					
						}
						else
						{
							PARSER->error( @1, "Ошибка, для оператора TERMINATE можно использовать только целое значение" );
						}
					}
					| fun_arithm  error 
					{	
						PARSER->error( @1, "Ошибка, после оператора TERMINATE может быть указано только одно целое положительное число" )
					};	
dpt_seize_param:	RDO_IDENTIF
					{
						std::string res_name = reinterpret_cast<RDOValue*>($1)->value().getIdentificator().c_str();
						RDOPROCSeize* seize = new RDOPROCSeize( PARSER->getLastPROCProcess(), "SEIZE");
						seize->add_Seize_Resourse(res_name);
						$$ = (int)seize;
					}
					| dpt_seize_param ',' RDO_IDENTIF
					{
						RDOPROCSeize* seize  = reinterpret_cast<RDOPROCSeize*>($1);
						std::string res_name = reinterpret_cast<RDOValue*>($3)->value().getIdentificator().c_str();
						seize->add_Seize_Resourse(res_name);
						$$ = $1;
					}
					| dpt_seize_param error
					{
						PARSER->error( @1, "Ошибка в имени ресурса" );
					};
dpt_release_param:	RDO_IDENTIF
					{	
						std::string res_name = reinterpret_cast<RDOValue*>($1)->value().getIdentificator().c_str();
						RDOPROCRelease* release = new RDOPROCRelease( PARSER->getLastPROCProcess(), "RELEASE");
						release->add_Release_Resourse(res_name);
						$$ = (int)release;
					}
					| dpt_release_param ',' RDO_IDENTIF
					{
						RDOPROCRelease* release  = reinterpret_cast<RDOPROCRelease*>($1);
						std::string res_name = reinterpret_cast<RDOValue*>($3)->value().getIdentificator().c_str();
						release->add_Release_Resourse(res_name);
						$$ = $1;
					}
					| dpt_release_param error
					{
						PARSER->error( @1, "Ошибка в имени ресурса" );
					};		
					
dpt_assign_param:	RDO_IDENTIF '.' RDO_IDENTIF '=' fun_arithm
					{
						std::string res = reinterpret_cast<RDOValue*>($1)->value().getIdentificator();
						std::string param = reinterpret_cast<RDOValue*>($3)->value().getIdentificator();
						const RDOParserSrcInfo& info = @1;
						rdoMBuilder::RDOResType rtp;
						// Получили список всех ресурсов
						rdoMBuilder::RDOResourceList rssList( PARSER );
						// Если ресурс существует, берем его тип и проверяем наличие параметра
						if (rssList[res].exist())
						{
							rtp = rssList[res].getType();
							if( !rtp.m_params[param].exist() )
							{
								PARSER->error( @1, rdo::format("Ссылка на неизвестный параметр ресурса: %s.%s", res.c_str(), param.c_str()) );
							}
						
							RDOFUNArithm*   arithm     = reinterpret_cast<RDOFUNArithm*>($5);
							if ( arithm ) 
							{
								const RDORSSResource* rs = PARSER->findRSSResource( res );
								const RDORTPResType* rt = rs->getType();
								const RDORTPParam* pr = rt->findRTPParam( param );
								pr->getType()->checkParamType( arithm );
								RDOPROCAssign* assign = new RDOPROCAssign( PARSER->getLastPROCProcess(), "ASSIGN", arithm->createCalc( pr->getType() ), rs->getID(), rtp.m_params[param].id() );
								$$ = int(assign);
							}

						}
						else
						{
							PARSER->error( @1, rdo::format("Ссылка на неизвестный ресурс: %s", res.c_str()) );
						}
					};

dpt_process_end:	dpt_process RDO_End	
					{
						PARSER->getLastPROCProcess()->end();
					};

// ----------------------------------------------------------------------------
// ---------- Логические выражения
// ----------------------------------------------------------------------------
fun_logic_eq: '='    { $1 = RDO_eq; }
			| RDO_eq { $1 = RDO_eq; };

fun_logic:	  fun_arithm  fun_logic_eq  fun_arithm   { $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) == *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_arithm  RDO_neq       fun_arithm   { $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) != *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_arithm  '<'           fun_arithm   { $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) <  *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_arithm  '>'           fun_arithm   { $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) >  *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_arithm  RDO_leq       fun_arithm   { $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) <= *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_arithm  RDO_geq       fun_arithm   { $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) >= *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_logic   RDO_and       fun_logic    { $$ = (int)(*reinterpret_cast<RDOFUNLogic*>($1) && *reinterpret_cast<RDOFUNLogic*>($3));   }
			| fun_logic   RDO_or        fun_logic    { $$ = (int)(*reinterpret_cast<RDOFUNLogic*>($1) || *reinterpret_cast<RDOFUNLogic*>($3));   }
			| fun_arithm                             { $$ = (int)new RDOFUNLogic( *reinterpret_cast<RDOFUNArithm*>($1) );                        }
			| fun_group
			| fun_select_logic
			| '[' fun_logic ']'
			{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setSrcPos( @1, @3 );
				logic->setSrcText( "[" + logic->src_text() + "]" );
				$$ = $2;
			}
			| '(' fun_logic ')'
			{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setSrcPos( @1, @3 );
				logic->setSrcText( "(" + logic->src_text() + ")" );
				$$ = $2;
			}
			| RDO_not fun_logic
			{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				RDOFUNLogic* logic_not = logic->operator_not();
				logic_not->setSrcPos( @1, @2 );
				logic_not->setSrcText( "not " + logic->src_text() );
				$$ = (int)logic_not;
			}
			| '[' fun_logic error {
				PARSER->error( @2, "Ожидается закрывающаяся скобка" );
			}
			| '(' fun_logic error {
				PARSER->error( @2, "Ожидается закрывающаяся скобка" );
			};

// ----------------------------------------------------------------------------
// ---------- Арифметические выражения
// ----------------------------------------------------------------------------
fun_arithm:	  RDO_INT_CONST                 { $$ = (int)new RDOFUNArithm( PARSER, *reinterpret_cast<RDOValue*>($1) ); }
			| RDO_REAL_CONST                { $$ = (int)new RDOFUNArithm( PARSER, *reinterpret_cast<RDOValue*>($1) ); }
			| RDO_BOOL_CONST                { $$ = (int)new RDOFUNArithm( PARSER, *reinterpret_cast<RDOValue*>($1) ); }
			| RDO_STRING_CONST              { $$ = (int)new RDOFUNArithm( PARSER, *reinterpret_cast<RDOValue*>($1) ); }
			| RDO_IDENTIF                   { $$ = (int)new RDOFUNArithm( PARSER, *reinterpret_cast<RDOValue*>($1) ); }
			| RDO_IDENTIF '.' RDO_IDENTIF   { $$ = (int)new RDOFUNArithm( PARSER, *reinterpret_cast<RDOValue*>($1), *reinterpret_cast<RDOValue*>($3) ); }
			| fun_arithm '+' fun_arithm		{ $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) + *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_arithm '-' fun_arithm		{ $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) - *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_arithm '*' fun_arithm		{ $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) * *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_arithm '/' fun_arithm		{ $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) / *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_arithm_func_call
			| fun_select_arithm
			| '(' fun_arithm ')'
			{
				RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($2);
				arithm->setSrcPos( @1, @3 );
				arithm->setSrcText( "(" + arithm->src_text() + ")" );
				$$ = $2;
			}
			| '-' fun_arithm %prec RDO_UMINUS
			{
				RDOParserSrcInfo info;
				info.setSrcPos( @1, @2 );
				info.setSrcText( "-" + reinterpret_cast<RDOFUNArithm*>($2)->src_text() );
				$$ = (int)new RDOFUNArithm( PARSER, RDOValue(reinterpret_cast<RDOFUNArithm*>($2)->type(), info), new rdoRuntime::RDOCalcUMinus( RUNTIME, reinterpret_cast<RDOFUNArithm*>($2)->createCalc() ) );
			};

// ----------------------------------------------------------------------------
// ---------- Функции и последовательности
// ----------------------------------------------------------------------------
fun_arithm_func_call:	RDO_IDENTIF '(' ')' {
							RDOFUNParams* fun = new RDOFUNParams( PARSER );
							std::string fun_name = reinterpret_cast<RDOValue*>($1)->value().getIdentificator();
							fun->funseq_name.setSrcInfo( RDOParserSrcInfo(@1, fun_name) );
							fun->setSrcPos( @1, @3 );
							fun->setSrcText( fun_name + "()" );
							RDOFUNArithm* arithm = fun->createCall( fun_name );
							$$ = (int)arithm;
						}
						| RDO_IDENTIF '(' fun_arithm_func_call_pars ')' {
							RDOFUNParams* fun    = reinterpret_cast<RDOFUNParams*>($3);
							std::string fun_name = reinterpret_cast<RDOValue*>($1)->value().getIdentificator();
							fun->funseq_name.setSrcInfo( RDOParserSrcInfo(@1, fun_name) );
							fun->setSrcPos( @1, @4 );
							fun->setSrcText( fun_name + "(" + fun->src_text() + ")" );
							RDOFUNArithm* arithm = fun->createCall( fun_name );
							$$ = (int)arithm;
						}
						| RDO_IDENTIF '(' error {
							PARSER->error( @3, "Ошибка в параметрах функции" );
						};

fun_arithm_func_call_pars:	fun_arithm {
								RDOFUNParams* fun = new RDOFUNParams( PARSER );
								RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($1);
								fun->setSrcText( arithm->src_text() );
								fun->addParameter( arithm );
								$$ = (int)fun;
							}
							| fun_arithm_func_call_pars ',' fun_arithm {
								RDOFUNParams* fun    = reinterpret_cast<RDOFUNParams*>($1);
								RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($3);
								fun->setSrcText( fun->src_text() + ", " + arithm->src_text() );
								fun->addParameter( arithm );
								$$ = (int)fun;
							}
							| fun_arithm_func_call_pars error {
								PARSER->error( @2, "Ошибка в арифметическом выражении" );
							}
							| fun_arithm_func_call_pars ',' error {
								PARSER->error( @3, "Ошибка в арифметическом выражении" );
							};

// ----------------------------------------------------------------------------
// ---------- Групповые выражения
// ----------------------------------------------------------------------------
fun_group_keyword:	RDO_Exist			{ $$ = RDOFUNGroupLogic::fgt_exist;     }
					| RDO_Not_Exist		{ $$ = RDOFUNGroupLogic::fgt_notexist;  }
					| RDO_For_All		{ $$ = RDOFUNGroupLogic::fgt_forall;    }
					| RDO_Not_For_All	{ $$ = RDOFUNGroupLogic::fgt_notforall; };

fun_group_header:	fun_group_keyword '(' RDO_IDENTIF_COLON {
						RDOValue* type_name = reinterpret_cast<RDOValue*>($3);
						$$ = (int)(new RDOFUNGroupLogic( PARSER, (RDOFUNGroupLogic::FunGroupType)$1, type_name->src_info() ));
					}
					| fun_group_keyword '(' error {
						PARSER->error( @3, "Ожидается имя типа" );
					}
					| fun_group_keyword error {
						PARSER->error( @1, "После имени функции ожидается октрывающаяся скобка" );
					};

fun_group:			fun_group_header fun_logic ')' {
						RDOFUNGroupLogic* groupfun = reinterpret_cast<RDOFUNGroupLogic*>($1);
						groupfun->setSrcPos( @1, @3 );
						$$ = (int)groupfun->createFunLogic( reinterpret_cast<RDOFUNLogic*>($2) );
					}
					| fun_group_header RDO_NoCheck ')' {
						RDOFUNGroupLogic* groupfun = reinterpret_cast<RDOFUNGroupLogic*>($1);
						groupfun->setSrcPos( @1, @3 );
						RDOFUNLogic* trueLogic = new RDOFUNLogic( groupfun, new rdoRuntime::RDOCalcConst( RUNTIME, 1 ) );
						trueLogic->setSrcPos( @2 );
						trueLogic->setSrcText( "NoCheck" );
						$$ = (int)groupfun->createFunLogic( trueLogic );
					}
					| fun_group_header fun_logic error {
						PARSER->error( @2, "Ожидается закрывающаяся скобка" );
					}
					| fun_group_header RDO_NoCheck error {
						PARSER->error( @2, "Ожидается закрывающаяся скобка" );
					}
					| fun_group_header error {
						PARSER->error( @1, @2, "Ошибка в логическом выражении" )
					};

// ----------------------------------------------------------------------------
// ---------- Select
// ----------------------------------------------------------------------------
fun_select_header:	RDO_Select '(' RDO_IDENTIF_COLON {
						RDOValue* type_name  = reinterpret_cast<RDOValue*>($3);
						RDOFUNSelect* select = new RDOFUNSelect( PARSER, type_name->src_info() );
						select->setSrcText( "Select(" + type_name->value().getIdentificator() + ": " );
						$$ = (int)select;
					}
					| RDO_Select '(' error {
						PARSER->error( @3, "Ожидается имя типа" );
					}
					| RDO_Select error {
						PARSER->error( @1, "Ожидается октрывающаяся скобка" );
					};

fun_select_body:	fun_select_header fun_logic ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						RDOFUNLogic*  flogic = reinterpret_cast<RDOFUNLogic*>($2);
						select->setSrcText( select->src_text() + flogic->src_text() + ")" );
						select->initSelect( flogic );
					}
					| fun_select_header RDO_NoCheck ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						RDOParserSrcInfo logic_info(@2, "NoCheck");
						select->setSrcText( select->src_text() + logic_info.src_text() + ")" );
						rdoRuntime::RDOCalcConst* calc_nocheck = new rdoRuntime::RDOCalcConst( RUNTIME, 1 );
						RDOFUNLogic* flogic = new RDOFUNLogic( select, calc_nocheck, true );
						flogic->setSrcInfo( logic_info );
						select->initSelect( flogic );
					}
					| fun_select_header fun_logic error {
						PARSER->error( @2, "Ожидается закрывающаяся скобка" );
					}
					| fun_select_header RDO_NoCheck error {
						PARSER->error( @2, "Ожидается закрывающаяся скобка" );
					}
					| fun_select_header error {
						PARSER->error( @1, @2, "Ошибка в логическом выражении" )
					};

fun_select_keyword:	RDO_Exist			{ $$ = RDOFUNGroupLogic::fgt_exist;     }
					| RDO_Not_Exist		{ $$ = RDOFUNGroupLogic::fgt_notexist;  }
					| RDO_For_All		{ $$ = RDOFUNGroupLogic::fgt_forall;    }
					| RDO_Not_For_All	{ $$ = RDOFUNGroupLogic::fgt_notforall; };

fun_select_logic:	fun_select_body '.' fun_select_keyword '(' fun_logic ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcPos( @1, @6 );
						RDOFUNLogic* logic = select->createFunSelectGroup( (RDOFUNGroupLogic::FunGroupType)$3, reinterpret_cast<RDOFUNLogic*>($5) );
						$$ = (int)logic;
					}
					| fun_select_body '.' fun_select_keyword '(' error {
						PARSER->error( @4, @5, "Ошибка в логическом выражении" )
					}
					| fun_select_body '.' fun_select_keyword error {
						PARSER->error( @3, "Ожидается октрывающаяся скобка" );
					}
					| fun_select_body '.' RDO_Empty '(' ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcPos( @1, @5 );
						RDOParserSrcInfo empty_info(@3, @5, "Empty()");
						RDOFUNLogic* logic = select->createFunSelectEmpty( empty_info );
						$$ = (int)logic;
					}
					| fun_select_body '.' RDO_Empty '(' error {
						PARSER->error( @4, "Ожидается закрывающаяся скобка" );
					}
					| fun_select_body '.' RDO_Empty error {
						PARSER->error( @3, "Ожидается октрывающаяся скобка" );
					}
					| fun_select_body '.' error {
						PARSER->error( @2, @3, "Ожидается метод списка ресурсов" );
					}
					| fun_select_body error {
						PARSER->error( @1, "Ожидается '.' (точка) для вызова метода списка ресурсов" );
					};

fun_select_arithm:	fun_select_body '.' RDO_Size '(' ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcPos( @1, @5 );
						RDOParserSrcInfo size_info(@3, @5, "Size()");
						RDOFUNArithm* arithm = select->createFunSelectSize( size_info );
						$$ = (int)arithm;
					}
					| fun_select_body '.' RDO_Size error {
						PARSER->error( @3, "Ожидается октрывающаяся скобка" );
					}
					| fun_select_body '.' RDO_Size '(' error {
						PARSER->error( @4, "Ожидается закрывающаяся скобка" );
					};

%%

}

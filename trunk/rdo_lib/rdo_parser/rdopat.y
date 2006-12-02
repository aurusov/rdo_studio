%{
#define YYPARSE_PARAM lexer
#define YYLEX_PARAM lexer
%}

%pure-parser

%token Resource_type		257
%token permanent_kw			258
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
%token operation_kw			299
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
%token Select				418
%token Size_kw				419
%token Empty_kw				420
%token not_keyword			421

%{
#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoparser.h"
#include "rdoparser_rdo.h"
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
%left not_keyword

%%

pat_list:	
			| pat_list pat_pattern;
			| error {
				parser->lexer_loc_set( &(@1) );
				parser->error( "Ожидается ключевое слово $Pattern" );
			};

pat_header:	  Pattern IDENTIF_COLON operation_kw    pat_trace { $$ = (int)(new RDOPATPatternOperation( (std::string *)$2, $4 != 0 )); }
			| Pattern IDENTIF_COLON irregular_event pat_trace { $$ = (int)(new RDOPATPatternEvent(     (std::string *)$2, $4 != 0 )); }
			| Pattern IDENTIF_COLON rule_keyword    pat_trace { $$ = (int)(new RDOPATPatternRule(      (std::string *)$2, $4 != 0 )); }
			| Pattern IDENTIF_COLON keyboard        pat_trace { $$ = (int)(new RDOPATPatternKeyboard(  (std::string *)$2, $4 != 0 )); };
			| Pattern error {
				parser->lexer_loc_set( &(@2) );
				parser->error( "Ожидается имя образца" );
			}
			| Pattern IDENTIF_COLON error {
				parser->lexer_loc_set( &(@2), &(@3) );
				parser->error( "Ожидается тип образца" );
//			}
//			| Pattern IDENTIF_COLON irregular_event error {
//				parser->lexer_loc_set( &(@3), &(@4) );
//				parser->error( "Ожидается признак трассировки" );
//				parser->error( "Ожидается признак трассировки, описание параметров или релевантных ресурсов образца" );
			};

pat_trace:	/* empty */		{ $$ = 0; }
			| trace_keyword	{ $$ = 1; }
			| no_trace		{ $$ = 0; };

pat_params_begin: pat_header Parameters { $$ = $1; };

pat_params:	pat_params_begin IDENTIF_COLON fun_param_type {
				((RDOPATPattern *)$1)->add(new RDOFUNFunctionParam((std::string *)$2, (RDORTPResParam *)$3));
				$$ = $1;
			}
			| pat_params IDENTIF_COLON fun_param_type {
				((RDOPATPattern *)$1)->add(new RDOFUNFunctionParam((std::string *)$2, (RDORTPResParam *)$3));
				$$ = $1;
			}
			| pat_params_begin error {
				parser->lexer_loc_set( &(@1), &(@2) );
				if ( @1.last_line != @2.last_line ) {
					parser->error( "Ожидается имя параметра образца" );
				} else {
					parser->error( rdo::format("Ожидается имя параметра образца, найдено: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				}
			}
			| pat_params_begin IDENTIF error {
				parser->lexer_loc_set( &(@2), &(@3) );
				if ( @2.last_line != @3.last_line ) {
					parser->error( "Ожидается двоеточие" );
				} else {
					parser->error( rdo::format("Ожидается двоеточие, найдено: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				}
			}
			| pat_params_begin IDENTIF_COLON error {
				parser->lexer_loc_set( &(@2), &(@3) );
				if ( @2.last_line != @3.last_line ) {
					parser->error( "Ожидается тип параметра образца" );
				} else {
					parser->error( rdo::format("Ожидается тип параметра образца, найдено: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				}
			}
			| pat_params error {
				parser->lexer_loc_set( &(@2) );
				parser->error( "Ожидается имя параметра образца" );
			}
			| pat_params IDENTIF_COLON error {
				parser->lexer_loc_set( &(@2), &(@3) );
				parser->error( "Ожидается тип параметра образца" );
			};

pat_params_end:	pat_params Relevant_resources   { $$ = $1; }
				| pat_header Relevant_resources { $$ = $1; }
				| pat_header error {
					parser->lexer_loc_set( @2.first_line, @2.first_column );
					parser->error( "Ожидается ключевое слово $Relevant_resources" );
				};

pat_rel_res:	pat_params_end   IDENTIF_COLON IDENTIF pat_conv pat_conv { parser->lexer_loc_backup(); parser->lexer_loc_set( &(@5) ); ((RDOPATPattern *)$1)->addRelRes((std::string *)$2, (std::string *)$3, (RDOPATPattern::ConvertStatus)$4, (RDOPATPattern::ConvertStatus)$5); parser->lexer_loc_restore(); ((RDOPATPattern *)$1)->setRelResPos((std::string *)$2, &(@2)); $$ = $1; }
				| pat_rel_res    IDENTIF_COLON IDENTIF pat_conv pat_conv { parser->lexer_loc_backup(); parser->lexer_loc_set( &(@5) ); ((RDOPATPattern *)$1)->addRelRes((std::string *)$2, (std::string *)$3, (RDOPATPattern::ConvertStatus)$4, (RDOPATPattern::ConvertStatus)$5); parser->lexer_loc_restore(); ((RDOPATPattern *)$1)->setRelResPos((std::string *)$2, &(@2)); $$ = $1; }
				| pat_params_end IDENTIF_COLON IDENTIF pat_conv          { parser->lexer_loc_backup(); parser->lexer_loc_set( &(@4) ); ((RDOPATPattern *)$1)->addRelRes((std::string *)$2, (std::string *)$3, (RDOPATPattern::ConvertStatus)$4);                                   parser->lexer_loc_restore(); ((RDOPATPattern *)$1)->setRelResPos((std::string *)$2, &(@2)); $$ = $1; }
				| pat_rel_res    IDENTIF_COLON IDENTIF pat_conv          { parser->lexer_loc_backup(); parser->lexer_loc_set( &(@4) ); ((RDOPATPattern *)$1)->addRelRes((std::string *)$2, (std::string *)$3, (RDOPATPattern::ConvertStatus)$4);                                   parser->lexer_loc_restore(); ((RDOPATPattern *)$1)->setRelResPos((std::string *)$2, &(@2)); $$ = $1; }
				| pat_params_end IDENTIF_COLON IDENTIF_NoChange pat_conv { parser->lexer_loc_backup(); parser->lexer_loc_set( &(@4) ); ((RDOPATPattern *)$1)->addRelRes((std::string *)$2, (std::string *)$3, RDOPATPattern::CS_NoChange,       (RDOPATPattern::ConvertStatus)$4); parser->lexer_loc_restore(); ((RDOPATPattern *)$1)->setRelResPos((std::string *)$2, &(@2)); $$ = $1; }
				| pat_rel_res    IDENTIF_COLON IDENTIF_NoChange pat_conv { parser->lexer_loc_backup(); parser->lexer_loc_set( &(@4) ); ((RDOPATPattern *)$1)->addRelRes((std::string *)$2, (std::string *)$3, RDOPATPattern::CS_NoChange,       (RDOPATPattern::ConvertStatus)$4); parser->lexer_loc_restore(); ((RDOPATPattern *)$1)->setRelResPos((std::string *)$2, &(@2)); $$ = $1; }
				| pat_params_end IDENTIF_COLON IDENTIF_NoChange_NoChange { parser->lexer_loc_backup(); parser->lexer_loc_set( &(@3) ); ((RDOPATPattern *)$1)->addRelRes((std::string *)$2, (std::string *)$3, RDOPATPattern::CS_NoChange,       RDOPATPattern::CS_NoChange);       parser->lexer_loc_restore(); ((RDOPATPattern *)$1)->setRelResPos((std::string *)$2, &(@2)); $$ = $1; }
				| pat_rel_res    IDENTIF_COLON IDENTIF_NoChange_NoChange { parser->lexer_loc_backup(); parser->lexer_loc_set( &(@3) ); ((RDOPATPattern *)$1)->addRelRes((std::string *)$2, (std::string *)$3, RDOPATPattern::CS_NoChange,       RDOPATPattern::CS_NoChange);       parser->lexer_loc_restore(); ((RDOPATPattern *)$1)->setRelResPos((std::string *)$2, &(@2)); $$ = $1; }
				| pat_params_end IDENTIF_COLON IDENTIF_NoChange          { parser->lexer_loc_backup(); parser->lexer_loc_set( &(@3) ); ((RDOPATPattern *)$1)->addRelRes((std::string *)$2, (std::string *)$3, RDOPATPattern::CS_NoChange);                                         parser->lexer_loc_restore(); ((RDOPATPattern *)$1)->setRelResPos((std::string *)$2, &(@2)); $$ = $1; }
				| pat_rel_res    IDENTIF_COLON IDENTIF_NoChange          { parser->lexer_loc_backup(); parser->lexer_loc_set( &(@3) ); ((RDOPATPattern *)$1)->addRelRes((std::string *)$2, (std::string *)$3, RDOPATPattern::CS_NoChange);                                         parser->lexer_loc_restore(); ((RDOPATPattern *)$1)->setRelResPos((std::string *)$2, &(@2)); $$ = $1; }
				| pat_params_end IDENTIF_COLON IDENTIF IDENTIF_NoChange  { parser->lexer_loc_backup(); parser->lexer_loc_set( &(@4) ); ((RDOPATPattern *)$1)->addRelRes((std::string *)$2, (std::string *)$3, (std::string *)$4);                                                  parser->lexer_loc_restore(); ((RDOPATPattern *)$1)->setRelResPos((std::string *)$2, &(@2)); $$ = $1; }
				| pat_rel_res    IDENTIF_COLON IDENTIF IDENTIF_NoChange  { parser->lexer_loc_backup(); parser->lexer_loc_set( &(@4) ); ((RDOPATPattern *)$1)->addRelRes((std::string *)$2, (std::string *)$3, (std::string *)$4);                                                  parser->lexer_loc_restore(); ((RDOPATPattern *)$1)->setRelResPos((std::string *)$2, &(@2)); $$ = $1; }
				| pat_params_end error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "Ошибка в описании релевантных ресурсов" );
				}
				| pat_rel_res error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "Ошибка в описании релевантных ресурсов" );
				}
				| pat_params_end IDENTIF_COLON error {
					parser->lexer_loc_set( &(@2), &(@3) );
					parser->error( "Ожидатеся описатель (имя типа или ресурса)" );
				}
				| pat_rel_res IDENTIF_COLON error {
					parser->lexer_loc_set( &(@2), &(@3) );
					parser->error( "Ожидатеся описатель (имя типа или ресурса)" );
				}
				| pat_params_end IDENTIF_COLON IDENTIF error {
					parser->lexer_loc_set( &(@3), &(@4) );
					if ( parser->getLastPATPattern()->isHaveConvertEnd() ) {
						parser->error( "Ожидатеся статус конвертора начала" );
					} else {
						parser->error( "Ожидатеся статус конвертора" );
					}
				}
				| pat_rel_res IDENTIF_COLON IDENTIF error {
					parser->lexer_loc_set( &(@3), &(@4) );
					if ( parser->getLastPATPattern()->isHaveConvertEnd() ) {
						parser->error( "Ожидатеся статус конвертора начала" );
					} else {
						parser->error( "Ожидатеся статус конвертора" );
					}
				}
				| pat_params_end IDENTIF_COLON IDENTIF pat_conv error {
					switch ( parser->getLastPATPattern()->getPatType() ) {
						case RDOPATPattern::PT_Rule: {
							parser->lexer_loc_set( &(@5) );
							parser->error( "Ожидатеся способ выбора или ключевое слово $Body" );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->lexer_loc_set( &(@5) );
							parser->error( "Ожидатеся способ выбора или ключевое слово $Time" );
							break;
						}
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							parser->lexer_loc_set( &(@4), &(@5) );
							parser->error( "Ожидатеся статус конвертора конца" );
							break;
						}
					}
				}
				| pat_rel_res IDENTIF_COLON IDENTIF pat_conv error {
					switch ( parser->getLastPATPattern()->getPatType() ) {
						case RDOPATPattern::PT_Rule: {
							parser->lexer_loc_set( &(@5) );
							parser->error( "Ожидатеся способ выбора или ключевое слово $Body" );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->lexer_loc_set( &(@5) );
							parser->error( "Ожидатеся способ выбора или ключевое слово $Time" );
							break;
						}
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							parser->lexer_loc_set( &(@4), &(@5) );
							parser->error( "Ожидатеся статус конвертора конца" );
							break;
						}
					}
				}
				| pat_params_end IDENTIF_COLON IDENTIF_NoChange error {
					switch ( parser->getLastPATPattern()->getPatType() ) {
						case RDOPATPattern::PT_Rule: {
							parser->lexer_loc_set( &(@4) );
							parser->error( "Ожидатеся способ выбора или ключевое слово $Body" );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->lexer_loc_set( &(@4) );
							parser->error( "Ожидатеся способ выбора или ключевое слово $Time" );
							break;
						}
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							parser->lexer_loc_set( &(@3), &(@4) );
							parser->error( "Ожидатеся статус конвертора конца" );
							break;
						}
					}
				}
				| pat_rel_res IDENTIF_COLON IDENTIF_NoChange error {
					switch ( parser->getLastPATPattern()->getPatType() ) {
						case RDOPATPattern::PT_Rule: {
							parser->lexer_loc_set( &(@4) );
							parser->error( "Ожидатеся способ выбора или ключевое слово $Body" );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->lexer_loc_set( &(@4) );
							parser->error( "Ожидатеся способ выбора или ключевое слово $Time" );
							break;
						}
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							parser->lexer_loc_set( &(@3), &(@4) );
							parser->error( "Ожидатеся статус конвертора конца" );
							break;
						}
					}
				};

pat_conv:	Keep				{ $$ = RDOPATPattern::CS_Keep;     }
			| Create_keyword	{ $$ = RDOPATPattern::CS_Create;   }
			| Erase				{ $$ = RDOPATPattern::CS_Erase;    }
			| NonExist			{ $$ = RDOPATPattern::CS_NonExist; };

pat_common_choice:	pat_rel_res
					| pat_rel_res first_keyword {
						((RDOPATPattern *)$1)->setCommonChoiceWithMax( new RDOFUNArithm(1) );
						$$ = $1;
//						parser->lexer_loc_set( &(@2) );
//						parser->error( "Перед $Body необходимо использовать 'with_max(1)' вместо 'first'" );
//						((RDOPATPattern *)$1)->setCommonChoiceFirst(); $$ = $1;
					}
					| pat_rel_res with_min fun_arithm {
						((RDOPATPattern *)$1)->setCommonChoiceWithMin((RDOFUNArithm *)$3);
						$$ = $1;
					}
					| pat_rel_res with_max fun_arithm {
						((RDOPATPattern *)$1)->setCommonChoiceWithMax((RDOFUNArithm *)$3);
						$$ = $1;
					}
					| pat_rel_res with_min error {
						parser->lexer_loc_set( &(@2), &(@3) );
						parser->error( "Ожидается арифметическое выражение" );
					}
					| pat_rel_res with_max error {
						parser->lexer_loc_set( &(@2), &(@3) );
						parser->error( "Ожидается арифметическое выражение" );
					};

pat_time:	pat_common_choice Body {
				$$ = $1;
				if ( ((RDOPATPattern *)$1)->needTime() ) {
					parser->lexer_loc_set( @2.first_line, @2.first_column );
					parser->error( "Перед $Body пропущено ключевое слово $Time" );
				}
			}
			| pat_common_choice Time '=' fun_arithm Body {
				((RDOPATPattern *)$1)->setTime((RDOFUNArithm *)$4);
				$$ = $1;
			}
			| pat_common_choice Time '=' fun_arithm error {
				parser->lexer_loc_set( @5.first_line, @5.first_column );
				parser->error( "Ожидается ключевое слово $Body" );
			};

pat_body:	pat_time IDENTIF {
				parser->lexer_loc_backup();
				parser->lexer_loc_set( &(@2) );
				((RDOPATPattern *)$1)->addRelResBody((std::string *)$2);
				parser->lexer_loc_restore();
				$$ = $1;
			}
			| pat_convert IDENTIF {
				parser->lexer_loc_backup();
				parser->lexer_loc_set( &(@2) );
				((RDOPATPattern *)$1)->addRelResBody((std::string *)$2);
				parser->lexer_loc_restore();
				$$ = $1;
			}
			| pat_time error {
				parser->lexer_loc_set( &(@2) );
				std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
				parser->error( rdo::format("Неизвестный релевантный ресурс: %s", str.c_str()) );
			}
			| pat_convert error {
				parser->lexer_loc_set( &(@2) );
				std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
				parser->error( rdo::format("Неизвестный релевантный ресурс: %s", str.c_str()) );
			};

pat_res_usage:	pat_body pat_choice pat_first {
					parser->lexer_loc_backup();
					parser->lexer_loc_set( @3.last_line, @3.last_column );
					((RDOPATPattern *)$1)->addRelResUsage((RDOPATChoice *)$2, (RDOPATSelectType *)$3);
					parser->lexer_loc_restore();
					$$ = $1;
				};

pat_choice: /* empty */ {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceEmpty;
				$$= (int) new RDOPATChoice( RDOPATChoice::ch_empty );
			}
			| pat_choice_nocheck {
				$$ = (int) new RDOPATChoice( RDOPATChoice::ch_nocheck );
			}
			| pat_choice_from fun_logic {
				$$ = (int) new RDOPATChoice( RDOPATChoice::ch_from, (RDOFUNLogic*)$2 );
			};

pat_choice_nocheck: Choice NoCheck {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceNoCheck;
			};

pat_choice_from: Choice from_keyword {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceFrom;
			};

pat_first:	/* empty */ {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceOrderEmpty;
				$$ = (int) new RDOPATSelectType( RDOPATSelectType::st_empty );
			}
			| pat_choice_first {
				$$ = (int) new RDOPATSelectType( RDOPATSelectType::st_first );
			}
			| pat_choice_with_min fun_arithm {
				$$ = (int) new RDOPATSelectType( RDOPATSelectType::st_with_min, (RDOFUNArithm *)$2 );
			}
			| pat_choice_with_max fun_arithm {
				$$ = (int) new RDOPATSelectType( RDOPATSelectType::st_with_max, (RDOFUNArithm *)$2 );
			}
			| pat_choice_with_min error {
				parser->lexer_loc_set( &(@1), &(@2) );
				parser->error( "Ожидается арифметическое выражение" );
			}
			| pat_choice_with_max error {
				parser->lexer_loc_set( &(@1), &(@2) );
				parser->error( "Ожидается арифметическое выражение" );
			};

pat_choice_first: first_keyword {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceOrderFirst;
			};

pat_choice_with_min: with_min {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceOrderWithMin;
			};

pat_choice_with_max: with_max {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceOrderWithMax;
			};

pat_convert:	pat_res_usage {
					parser->lexer_loc_backup();
					parser->lexer_loc_set( @1.last_line, @1.last_column );
					((RDOPATPattern *)$1)->addRelResConvert();
					parser->lexer_loc_restore();
					$$ = $1;
				}
				| pat_res_usage convert_begin pat_trace pat_params_set {
					((RDOPATPattern *)$1)->addRelResConvertBegin($3 != 0, (RDOPATParamsSet *)$4);
					$$ = $1;
				}
				| pat_res_usage convert_end pat_trace pat_params_set {
					((RDOPATPattern *)$1)->addRelResConvertEnd($3 != 0, (RDOPATParamsSet *)$4);
					$$ = $1;
				}
				| pat_res_usage convert_begin pat_trace pat_params_set convert_end pat_trace pat_params_set {
					((RDOPATPattern *)$1)->addRelResConvertBeginEnd($3 != 0, (RDOPATParamsSet *)$4, $6 != 0, (RDOPATParamsSet *)$7);
					$$ = $1;
				}
				| pat_res_usage convert_rule pat_trace pat_params_set {
					((RDOPATPattern *)$1)->addRelResConvertRule($3 != 0, (RDOPATParamsSet *)$4);
					$$ = $1;
				}
				| pat_res_usage convert_event pat_trace pat_params_set {
					((RDOPATPattern *)$1)->addRelResConvertEvent($3 != 0, (RDOPATParamsSet *)$4);
					$$ = $1;
				};

convert_rule:	Convert_rule {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::convertBegin;
			};

convert_event:	Convert_event {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::convertBegin;
			};

convert_begin:	Convert_begin {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::convertBegin;
			};

convert_end:	Convert_end {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::convertEnd;
			};

pat_params_set:												{  $$ = (int) new RDOPATParamsSet(); }
			|	pat_params_set IDENTIF_set fun_arithm	{	((RDOPATParamsSet *)$1)->addIdentif((std::string *)$2, (RDOFUNArithm *)$3); $$ = $1;}
			|	pat_params_set IDENTIF_NoChange			{	((RDOPATParamsSet *)$1)->addIdentif((std::string *)$2); $$ = $1;};

pat_pattern:	pat_convert End { ((RDOPATPattern *)$1)->end(); $$ = $1; };
//				| pat_time  End { ((RDOPATPattern *)$1)->end(); $$ = $1; };

// ----------------------------------------------------------------------------
// ---------- Описание параметров
// ----------------------------------------------------------------------------
fun_param_type: integer fun_int_diap fun_int_default_val {
					RDORTPIntDiap *diap = (RDORTPIntDiap *)$2;
					RDORTPIntDefVal *dv = (RDORTPIntDefVal *)$3;
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@3) );
					RDORTPIntResParam *rp = new RDORTPIntResParam(diap, dv);
					parser->lexer_loc_restore();
					$$ = (int)rp;
				}
				| integer fun_int_diap {
					RDORTPIntDiap *diap = (RDORTPIntDiap *)$2;
					RDORTPIntDefVal *dv = new RDORTPIntDefVal();
					RDORTPIntResParam *rp = new RDORTPIntResParam(diap, dv);
					$$ = (int)rp;
				}
				| real fun_real_diap fun_real_default_val {
					RDORTPRealDiap *diap = (RDORTPRealDiap *)$2;
					RDORTPRealDefVal *dv = (RDORTPRealDefVal *)$3;
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@3) );
					RDORTPRealResParam *rp = new RDORTPRealResParam(diap, dv);
					parser->lexer_loc_restore();
					$$ = (int)rp;
				}
				| real fun_real_diap {
					RDORTPRealDiap *diap = (RDORTPRealDiap *)$2;
					RDORTPRealDefVal *dv = new RDORTPRealDefVal();
					RDORTPRealResParam *rp = new RDORTPRealResParam(diap, dv);
					$$ = (int)rp;
				}
				| fun_enum fun_enum_default_val {
					reinterpret_cast<RDOLexerFUN*>(lexer)->enum_param_cnt = 0;
					RDORTPEnum *enu = (RDORTPEnum *)$1;
					RDORTPEnumDefVal *dv = (RDORTPEnumDefVal *)$2;
					enu->findValue(dv->value);	 // if no value - Syntax exception will be thrown
					RDORTPEnumResParam *rp = new RDORTPEnumResParam(enu, dv);
					$$ = (int)rp;
				}
				| fun_enum {
					reinterpret_cast<RDOLexerFUN*>(lexer)->enum_param_cnt = 0;
					RDORTPEnum *enu = (RDORTPEnum *)$1;
					RDORTPEnumDefVal *dv = new RDORTPEnumDefVal();
					RDORTPEnumResParam *rp = new RDORTPEnumResParam(enu, dv);
					$$ = (int)rp;
				}
				| fun_such_as {
					RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
					$$ = (int)desc->getType()->constructSuchAs();
				}
				| fun_such_as fun_int_default_val {
					RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
					RDORTPIntDefVal *dv = (RDORTPIntDefVal *)$2;
					$$ = (int)desc->getType()->constructSuchAs((int)dv->val);
				}
				| fun_such_as	fun_real_default_val {
					RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
					RDORTPRealDefVal *dv = (RDORTPRealDefVal *)$2;
					if(!dv->exist)
						$$ = (int)desc->getType()->constructSuchAs();
					else
						$$ = (int)desc->getType()->constructSuchAs((double *)&(dv->val));
				}
				| fun_such_as fun_enum_default_val {
					RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
					RDORTPEnumDefVal *dv = (RDORTPEnumDefVal *)$2;
					if ( !dv->exist ) {
						$$ = (int)desc->getType()->constructSuchAs();
					} else {
						parser->lexer_loc_backup();
						parser->lexer_loc_set( &(@2) );
						$$ = (int)desc->getType()->constructSuchAs((std::string *)dv->value);
						parser->lexer_loc_restore();
					}
				}
				| fun_such_as '=' error {
					parser->error( "Ожидается зачение по-умолчанию" );
				}
				| fun_such_as error {
					parser->error( "Ожидается окончание описания ссылки, например, зачение по-умолчанию" );
				}
				| integer error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "Ошибка после ключевого слова integer. Возможно, не хватает значения по-умолчанию." );
				}
				| real error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "Ошибка после ключевого слова real. Возможно, не хватает значения по-умолчанию." );
				}
				| fun_enum error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "Ошибка после перечислимого типа. Возможно, не хватает значения по-умолчанию." );
				};

fun_int_diap:	/* empty */ {
					RDORTPIntDiap *diap = new RDORTPIntDiap();
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint INT_CONST ']' {
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@2) );
					RDORTPIntDiap *diap = new RDORTPIntDiap($2, $4);
					parser->lexer_loc_restore();
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint REAL_CONST {
					parser->lexer_loc_set( &(@2) );
					parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' REAL_CONST dblpoint INT_CONST {
					parser->lexer_loc_set( &(@2) );
					parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' INT_CONST dblpoint REAL_CONST {
					parser->lexer_loc_set( &(@4) );
					parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' INT_CONST dblpoint INT_CONST error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "Диапазон задан неверно" );
				}
				| '[' INT_CONST dblpoint error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "Диапазон задан неверно" );
				}
				| '[' error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "Диапазон задан неверно" );
				};

fun_real_diap: /* empty */ {
					RDORTPRealDiap *diap = new RDORTPRealDiap();
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint REAL_CONST ']' {
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@2) );
					double min = *((double *)$2);
					double max = *((double *)$4);
					RDORTPRealDiap *diap = new RDORTPRealDiap(min, max);
					parser->lexer_loc_restore();
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint INT_CONST ']' {
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@2) );
					double min = *((double *)$2);
					double max = $4;
					RDORTPRealDiap *diap = new RDORTPRealDiap(min, max);
					parser->lexer_loc_restore();
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint REAL_CONST ']' {
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@2) );
					double min = $2;
					double max = *((double *)$4);
					RDORTPRealDiap *diap = new RDORTPRealDiap(min, max);
					parser->lexer_loc_restore();
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint INT_CONST ']' {
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@2) );
					double min = $2;
					double max = $4;
					RDORTPRealDiap *diap = new RDORTPRealDiap(min, max);
					parser->lexer_loc_restore();
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint REAL_CONST error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "Диапазон задан неверно" );
				}
				| '[' REAL_CONST dblpoint INT_CONST error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "Диапазон задан неверно" );
				}
				| '[' INT_CONST dblpoint REAL_CONST error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "Диапазон задан неверно" );
				}
				| '[' INT_CONST dblpoint INT_CONST error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "Диапазон задан неверно" );
				}
				| '[' REAL_CONST dblpoint error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "Диапазон задан неверно" );
				}
				| '[' INT_CONST dblpoint error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "Диапазон задан неверно" );
				}
				| '[' error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "Диапазон задан неверно" );
				};

fun_enum:	'(' fun_enum_item ')' {
				$$ = $2;
			}
			| '(' fun_enum_item {
				parser->lexer_loc_set( &(@2) );
				parser->error( "Перечисление должно заканчиваться скобкой" );
			};

fun_enum_item:	IDENTIF {
					RDORTPEnum *enu = new RDORTPEnum((std::string *)$1);
					$$ = (int)enu;
					reinterpret_cast<RDOLexerFUN*>(lexer)->enum_param_cnt = 1;
				}
				| fun_enum_item ',' IDENTIF {
					if ( reinterpret_cast<RDOLexerFUN*>(lexer)->enum_param_cnt >= 1 ) {
						parser->lexer_loc_backup();
						parser->lexer_loc_set( &(@3) );
						RDORTPEnum *enu = (RDORTPEnum *)$1;
						enu->add((std::string *)$3);
						parser->lexer_loc_restore();
						$$ = (int)enu;
					} else {
						parser->error( "Ошибка в описании значений перечислимого типа" );
					}
				}
				| fun_enum_item IDENTIF {
					if ( reinterpret_cast<RDOLexerFUN*>(lexer)->enum_param_cnt >= 1 ) {
						parser->error( rdo::format("Пропущена запятая перед: %s", ((std::string*)$2)->c_str()) );
					} else {
						parser->error( "Ошибка в описании значений перечислимого типа" );
					}
				}
				| fun_enum_item error {
					std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
					if ( str.empty() ) {
						parser->lexer_loc_set( &(@1) );
						parser->error( "Ошибка в описании значений перечислимого типа" );
					} else {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdo::format( "Неверное значение перечислимого типа: %s", str.c_str() ) );
					}
				}
				| fun_enum_item ',' INT_CONST {
					parser->lexer_loc_set( &(@3) );
					parser->error( "Значение перечислимого типа не может начинаться с цифры" );
				}
				| fun_enum_item ',' REAL_CONST {
					parser->lexer_loc_set( &(@3) );
					parser->error( "Значение перечислимого типа не может начинаться с цифры" );
				}
				| INT_CONST {
					parser->lexer_loc_set( &(@1) );
					parser->error( "Значение перечислимого типа не может начинаться с цифры" );
				}
				| REAL_CONST {
					parser->lexer_loc_set( &(@1) );
					parser->error( "Значение перечислимого типа не может начинаться с цифры" );
				}
				| error {
					parser->lexer_loc_set( &(@1) );
					parser->error( "Ошибка в описании значений перечислимого типа" );
				};

fun_such_as:	such_as IDENTIF '.' IDENTIF {
					std::string* type = (std::string *)$2;
					std::string* param = (std::string *)$4;
					const RDORTPResType *const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type->c_str() );
					}
					const RDORTPParamDesc *const rp = rt->findRTPParam( param );
					if ( !rp ) {
						parser->lexer_loc_set( &(@4) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_PARAM, type->c_str(), param->c_str() );
					}
					$$ = (int)rp;
				}
				| such_as IDENTIF {
					std::string *constName = (std::string *)$2;
					const RDOFUNConstant *const cons = parser->findFUNConst(constName);
					if ( !cons ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdo::format("Ссылка на несужествующую константу: %s", constName->c_str()) );
					}
					$$ = (int)(cons->descr);
				}
				| such_as IDENTIF '.' {
					std::string* type = (std::string *)$2;
					const RDORTPResType *const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type->c_str() );
					} else {
						parser->lexer_loc_set( &(@3) );
						parser->error( "Не указан параметр" );
					}
				}
				| such_as IDENTIF '.' error {
					std::string* type = (std::string *)$2;
					const RDORTPResType *const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type->c_str() );
					} else {
						parser->lexer_loc_set( &(@4) );
						parser->error( "Ошибка при указании параметра" );
					}
				}
				| such_as IDENTIF error {
					std::string* type = (std::string *)$2;
					const RDORTPResType *const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type->c_str() );
					} else {
						parser->lexer_loc_set( &(@2) );
						parser->error( "После имени типа должен быть указан параметр через точку" );
					}
				}
				| such_as error {
					if ( @1.last_line == @2.first_line ) {
						parser->lexer_loc_set( @2.first_line, @2.first_column );
					} else {
						parser->lexer_loc_set( &(@1) );
					}
					parser->error( "После ключевого слова such_as необходимо указать тип и парамтер ресурса для ссылки" );
				};

fun_int_default_val:	'=' INT_CONST {
						RDORTPIntDefVal *dv = new RDORTPIntDefVal($2);
						$$ = (int)dv;
					}
					| '=' REAL_CONST {
						// Целое число инициализируется вещественным: %f
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_DEFVAULT_INT_AS_REAL, *(double*)$2 );
					}
					| '=' {
						parser->lexer_loc_set( &(@1) );
						parser->error( "Не указано значение по-умолчанию целого типа" );
					}
					| '=' error {
						parser->lexer_loc_set( &(@2) );
						parser->error( "Неверное значение по-умолчанию целого типа" );
					};

fun_real_default_val:	'=' REAL_CONST {
						$$ = (int)(new RDORTPRealDefVal(*((double *)$2)));
					}
					| '=' INT_CONST {
						$$ = (int)(new RDORTPRealDefVal($2));
					}
					| '=' {
						parser->lexer_loc_set( &(@1) );
						parser->error( "Не указано значение по-умолчанию вещественного типа" );
					}
					| '=' error {
						parser->lexer_loc_set( &(@2) );
						parser->error( "Неверное значение по-умолчанию вещественного типа" );
					};

fun_enum_default_val:	'=' IDENTIF {
						std::string *val = (std::string *)$2;
						RDORTPEnumDefVal* dv = new RDORTPEnumDefVal(val);
						$$ = (int)dv;
					}
					| '=' {
						parser->lexer_loc_set( &(@1) );
						parser->error( "Не указано значение по-умолчанию перечислимого типа" );
					}
					| '=' error {
						parser->lexer_loc_set( &(@2) );
						parser->error( "Неверное значение по-умолчанию перечислимого типа" );
					};

// ----------------------------------------------------------------------------
// ---------- Логические выражения
// ----------------------------------------------------------------------------
fun_logic: fun_arithm '=' fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 == *(RDOFUNArithm *)$3); }
			| fun_arithm neq fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 != *(RDOFUNArithm *)$3); }
			| fun_arithm '<' fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 <  *(RDOFUNArithm *)$3); }
			| fun_arithm '>' fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 >  *(RDOFUNArithm *)$3); }
			| fun_arithm leq fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 <= *(RDOFUNArithm *)$3); }
			| fun_arithm geq fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 >= *(RDOFUNArithm *)$3); }
			| fun_logic and_keyword fun_logic	{ $$ = (int)(*(RDOFUNLogic *)$1 && *(RDOFUNLogic *)$3);   }
			| fun_logic or_keyword fun_logic	{ $$ = (int)(*(RDOFUNLogic *)$1 || *(RDOFUNLogic *)$3);   }
			| '[' fun_logic ']'					{ $$ = $2; }
			| '(' fun_logic ')'					{ $$ = $2; }
			| '[' fun_logic error {
				parser->lexer_loc_set( @2.last_line, @2.last_column );
				parser->error( "Ожидается закрывающаяся скобка" );
			}
			| '(' fun_logic error {
				parser->lexer_loc_set( @2.last_line, @2.last_column );
				parser->error( "Ожидается закрывающаяся скобка" );
			}
			| not_keyword fun_logic				{ $$ = (int)((RDOFUNLogic *)$2)->operator_not();          }
			| fun_group							{ $$ = $1; }
			| fun_select_logic
			| error								{
				parser->lexer_loc_set( &(@1) );
				parser->error( "Ошибка в логическом выражении" );
			};

// ----------------------------------------------------------------------------
// ---------- Арифметические выражения
// ----------------------------------------------------------------------------
fun_arithm: fun_arithm '+' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 + *(RDOFUNArithm *)$3); }
			| fun_arithm '-' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 - *(RDOFUNArithm *)$3); }
			| fun_arithm '*' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 * *(RDOFUNArithm *)$3); }
			| fun_arithm '/' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 / *(RDOFUNArithm *)$3); }
			| '(' fun_arithm ')'			{ $$ = $2; }
			| fun_arithm_func_call
			| fun_select_arithm
			| IDENTIF '.' IDENTIF			{
				parser->lexer_loc_backup();
				parser->lexer_loc_set( &(@3) );
				$$ = (int)(new RDOFUNArithm((std::string *)$1, (std::string *)$3));
				parser->lexer_loc_restore();
			}
			| INT_CONST						{ $$ = (int)(new RDOFUNArithm((int)$1));                              }
			| REAL_CONST					{ $$ = (int)(new RDOFUNArithm((double*)$1));                          }
			| IDENTIF						{ $$ = (int)(new RDOFUNArithm((std::string *)$1));                    }
			| error							{
				parser->lexer_loc_set( &(@1) );
				parser->error( "Ошибка в арифметическом выражении" );
			};

fun_arithm_func_call:	IDENTIF '(' fun_arithm_func_call_pars ')' {
							parser->lexer_loc_backup();
							parser->lexer_loc_set( &(@1) );
							$$ = (int)((RDOFUNParams *)$3)->createCall((std::string *)$1);
							parser->lexer_loc_restore();
						}
						| IDENTIF '(' error {
							parser->lexer_loc_set( &(@3) );
							parser->error( "Ошибка в параметрах функции" );
						};

fun_arithm_func_call_pars:	/* empty */ {
								$$ = (int)(new RDOFUNParams());
							}
							| fun_arithm_func_call_pars fun_arithm {
								$$ = (int)(((RDOFUNParams *)$1)->addParameter((RDOFUNArithm *)$2));
							}
							| fun_arithm_func_call_pars ',' fun_arithm {
								$$ = (int)(((RDOFUNParams *)$1)->addParameter((RDOFUNArithm *)$3));
							};

// ----------------------------------------------------------------------------
// ---------- Групповые выражения
// ----------------------------------------------------------------------------
fun_group_keyword:	Exist			{ $$ = 1; }
					| Not_Exist		{ $$ = 2; }
					| For_All		{ $$ = 3; }
					| Not_For_All	{ $$ = 4; };

fun_group_header:	fun_group_keyword '(' IDENTIF_COLON {
						parser->lexer_loc_backup();
						parser->lexer_loc_set( @3.first_line, @3.first_column + ((std::string*)$3)->length() );
						$$ = (int)(new RDOFUNGroupLogic($1, (std::string *)$3));
						parser->lexer_loc_restore();
					}
					| fun_group_keyword '(' error {
						parser->lexer_loc_set( &(@3) );
						parser->error( "Ожидается имя типа" );
					}
					| fun_group_keyword error {
						parser->lexer_loc_set( &(@1) );
						parser->error( "Ожидается октрывающаяся скобка" );
					};

fun_group:			fun_group_header fun_logic ')' {
						$$ = (int)(((RDOFUNGroupLogic *)$1)->createFunLogic((RDOFUNLogic *)$2));
					}
					| fun_group_header NoCheck ')' {
						$$ = (int)(((RDOFUNGroupLogic *)$1)->createFunLogic());
					}
					| fun_group_header fun_logic error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "Ожидается закрывающаяся скобка" );
					}
					| fun_group_header NoCheck error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "Ожидается закрывающаяся скобка" );
					};

fun_select_header:	Select '(' IDENTIF_COLON {
						parser->lexer_loc_backup();
						parser->lexer_loc_set( @3.first_line, @3.first_column + ((std::string*)$3)->length() );
						RDOFUNSelect* select = new RDOFUNSelect((std::string*)$3);
						parser->lexer_loc_restore();
						$$ = (int)select;
					}
					| Select '(' error {
						parser->lexer_loc_set( &(@3) );
						parser->error( "Ожидается имя типа" );
					}
					| Select error {
						parser->lexer_loc_set( &(@1) );
						parser->error( "Ожидается октрывающаяся скобка" );
					};

fun_select_body:	fun_select_header fun_logic ')' {
						((RDOFUNSelect*)$1)->createFunSelect((RDOFUNLogic *)$2);
						$$ = $1;
					}
					| fun_select_header NoCheck ')' {
						((RDOFUNSelect*)$1)->createFunSelect();
						$$ = $1;
					}
					| fun_select_header fun_logic error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "Ожидается закрывающаяся скобка" );
					}
					| fun_select_header NoCheck error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "Ожидается закрывающаяся скобка" );
					};

fun_select_keyword:	Exist			{ $$ = 1; }
					| Not_Exist		{ $$ = 2; }
					| For_All		{ $$ = 3; }
					| Not_For_All	{ $$ = 4; };

fun_select_logic:	fun_select_body '.' fun_select_keyword '(' fun_logic ')' {
						$$ = (int)(((RDOFUNSelect *)$1)->createFunSelect($3, (RDOFUNLogic *)$5));
					}
					| fun_select_body '.' Empty_kw '(' ')' {
						$$ = (int)(((RDOFUNSelect *)$1)->createFunSelectEmpty());
					}
					| fun_select_body error {
						parser->lexer_loc_set( &(@1) );
						parser->error( "Ожидается '.' (точка) для вызова метода списка ресурсов" );
					}
					| fun_select_body '.' error {
						parser->lexer_loc_set( &(@2), &(@3) );
						parser->error( "Ожидается метод списка ресурсов" );
					}
					| fun_select_body '.' fun_select_keyword error {
						parser->lexer_loc_set( &(@3) );
						parser->error( "Ожидается октрывающаяся скобка" );
					}
					| fun_select_body '.' Empty_kw error {
						parser->lexer_loc_set( &(@3) );
						parser->error( "Ожидается октрывающаяся скобка" );
					}
					| fun_select_body '.' fun_select_keyword '(' error ')' {
						parser->lexer_loc_set( @5.first_line, @5.first_column );
						parser->error( "Ошибка в логическом выражении" );
					}
					| fun_select_body '.' Empty_kw '(' error {
						parser->lexer_loc_set( &(@4) );
						parser->error( "Ожидается закрывающаяся скобка" );
					};

fun_select_arithm:	fun_select_body '.' Size_kw '(' ')' {
						$$ = (int)(((RDOFUNSelect *)$1)->createFunSelectSize());
					}
					| fun_select_body error {
						parser->lexer_loc_set( &(@1) );
						parser->error( "Ожидается '.' (точка) для вызова метода списка ресурсов" );
					}
					| fun_select_body '.' error {
						parser->lexer_loc_set( &(@2), &(@3) );
						parser->error( "Ожидается метод списка ресурсов: Size()" );
					}
					| fun_select_body '.' Size_kw error {
						parser->lexer_loc_set( &(@3) );
						parser->error( "Ожидается октрывающаяся скобка" );
					}
					| fun_select_body '.' Size_kw '(' error {
						parser->lexer_loc_set( &(@4) );
						parser->error( "Ожидается закрывающаяся скобка" );
					};

%%

}

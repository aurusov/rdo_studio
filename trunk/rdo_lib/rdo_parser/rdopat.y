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
%token temporary_kw			263
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
%token if_keyword			369
%token result_keyword		370

%token Frame				400
%token Show_if				401
%token Back_picture			402
%token Show					403
%token frm_cell				404
%token text					405
%token bitmap				406
%token s_bmp				407
%token rect_keyword			408
%token r_rect				409
%token line					410
%token ellipse				411
%token triang				412
%token active				413
%token ruler				414
%token space_kw				415
%token color_transparent_kw	416
%token color_last_kw		417
%token color_white_kw		418
%token color_black_kw		419
%token color_red_kw			420
%token color_green_kw		421
%token color_blue_kw		422
%token color_cyan_kw		423
%token color_magenta_kw		424
%token color_yellow_kw		425
%token color_gray_kw		426

%token QUOTED_IDENTIF		430
%token QUOTED_IDENTIF_BAD	431
%token IDENTIF_BAD			432
%token Select				433
%token Size_kw				434
%token Empty_kw				435
%token not_keyword			436
%token UMINUS				437

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
#include <rdotrace.h>

namespace rdoParse 
{
%}

%left or_keyword
%left and_keyword
%left '+' '-'
%left '*' '/'
%left not_keyword
%left UMINUS

%%

pat_main:
			| pat_main pat_pattern;
			| error {
				parser->error( @1, "Неизвестная ошибка" );
			};

pat_header:	Pattern IDENTIF_COLON operation_kw pat_trace {
				std::string name = *reinterpret_cast<std::string*>($2);
				$$ = (int)(new RDOPATPatternOperation( parser, RDOParserSrcInfo( @2, name, RDOParserSrcInfo::psi_align_bytext ), $4 != 0 ));
			}
			| Pattern IDENTIF_COLON irregular_event pat_trace {
				std::string name = *reinterpret_cast<std::string*>($2);
				$$ = (int)(new RDOPATPatternEvent( parser, RDOParserSrcInfo( @2, name, RDOParserSrcInfo::psi_align_bytext ), $4 != 0 ));
			}
			| Pattern IDENTIF_COLON rule_keyword pat_trace {
				std::string name = *reinterpret_cast<std::string*>($2);
				$$ = (int)(new RDOPATPatternRule( parser, RDOParserSrcInfo( @2, name, RDOParserSrcInfo::psi_align_bytext ), $4 != 0 ));
			}
			| Pattern IDENTIF_COLON keyboard pat_trace {
				std::string name = *reinterpret_cast<std::string*>($2);
				$$ = (int)(new RDOPATPatternKeyboard( parser, RDOParserSrcInfo( @2, name, RDOParserSrcInfo::psi_align_bytext ), $4 != 0 ));
			};
			| Pattern error {
				parser->error( @2, "Ожидается имя образца" );
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

pat_params:	pat_params_begin IDENTIF_COLON param_type {
				RDOPATPattern*   pattern    = reinterpret_cast<RDOPATPattern*>($1);
				std::string      param_name = *reinterpret_cast<std::string*>($2);
				RDORTPParamType* param_type = reinterpret_cast<RDORTPParamType*>($3);
				RDOFUNFunctionParam* param = new RDOFUNFunctionParam( pattern, RDOParserSrcInfo( @2, param_name, RDOParserSrcInfo::psi_align_bytext ), param_type );
				pattern->add( param );
				if ( param_type->getType() == RDORTPParamType::pt_enum ) {
					static_cast<RDORTPEnumParamType*>(param_type)->enum_name = rdo::format( "%s.%s", pattern->getName().c_str(), param_name.c_str() );
				}
			}
			| pat_params IDENTIF_COLON param_type {
				RDOPATPattern*   pattern    = reinterpret_cast<RDOPATPattern*>($1);
				std::string      param_name = *reinterpret_cast<std::string*>($2);
				RDORTPParamType* param_type = reinterpret_cast<RDORTPParamType*>($3);
				RDOFUNFunctionParam* param = new RDOFUNFunctionParam( pattern, RDOParserSrcInfo( @2, param_name, RDOParserSrcInfo::psi_align_bytext ), param_type );
				pattern->add( param );
				if ( param_type->getType() == RDORTPParamType::pt_enum ) {
					static_cast<RDORTPEnumParamType*>(param_type)->enum_name = rdo::format( "%s.%s", pattern->getName().c_str(), param_name.c_str() );
				}
			}
			| pat_params_begin error {
				if ( @1.last_line != @2.last_line ) {
					parser->error( @2, "Ожидается имя параметра образца" );
				} else {
					parser->error( @2, rdo::format("Ожидается имя параметра образца, найдено: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
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
				if ( @1.last_line != @2.last_line ) {
					parser->error( @2, "Ожидается имя параметра образца" );
				} else {
					parser->error( @2, rdo::format("Ожидается имя параметра образца, найдено: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				}
			}
			| pat_params IDENTIF error {
				parser->lexer_loc_set( &(@2), &(@3) );
				if ( @2.last_line != @3.last_line ) {
					parser->error( "Ожидается двоеточие" );
				} else {
					parser->error( rdo::format("Ожидается двоеточие, найдено: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				}
			}
			| pat_params IDENTIF_COLON error {
				parser->lexer_loc_set( &(@2), &(@3) );
				if ( @2.last_line != @3.last_line ) {
					parser->error( "Ожидается тип параметра образца" );
				} else {
					parser->error( rdo::format("Ожидается тип параметра образца, найдено: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				}
			};

pat_params_end:	pat_params Relevant_resources   { $$ = $1; }
				| pat_header Relevant_resources { $$ = $1; }
				| pat_header error {
					parser->error( @2, "Ожидается ключевое слово $Relevant_resources" );
				};

pat_rel_res:	pat_params_end IDENTIF_COLON IDENTIF pat_conv pat_conv {
					// проверено для ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string rel_name  = *reinterpret_cast<std::string*>($2);
							std::string type_name = *reinterpret_cast<std::string*>($3);
							static_cast<RDOPATPatternOperation*>(pattern)->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(@3, type_name), (rdoRuntime::RDOResourceTrace::ConvertStatus)$4, (rdoRuntime::RDOResourceTrace::ConvertStatus)$5, @4, @5 );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->error( @5, rdo::format("У нерегулярного события нет события конца, а значит и второго статуса конвертора") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							parser->error( @5, rdo::format("У продукционного правила нет события конца, а значит и второго статуса конвертора") );
							break;
						}
					}
				}
				| pat_rel_res IDENTIF_COLON IDENTIF pat_conv pat_conv {
					// проверено для ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string rel_name  = *reinterpret_cast<std::string*>($2);
							std::string type_name = *reinterpret_cast<std::string*>($3);
							static_cast<RDOPATPatternOperation*>(pattern)->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(@3, type_name), (rdoRuntime::RDOResourceTrace::ConvertStatus)$4, (rdoRuntime::RDOResourceTrace::ConvertStatus)$5, @4, @5 );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->error( @5, rdo::format("У нерегулярного события нет события конца, а значит и второго статуса конвертора") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							parser->error( @5, rdo::format("У продукционного правила нет события конца, а значит и второго статуса конвертора") );
							break;
						}
					}
				}
				| pat_params_end IDENTIF_COLON IDENTIF pat_conv {
					// проверено для ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							parser->error( @4, rdo::format("Помимо статуса конвертора начала (%s), ожидается статус конвертора конца, потому что у операции есть событие конца", RDOPATPattern::StatusToStr((rdoRuntime::RDOResourceTrace::ConvertStatus)$4).c_str()) );
							break;
						}
						case RDOPATPattern::PT_IE  : 
						case RDOPATPattern::PT_Rule: {
							std::string rel_name  = *reinterpret_cast<std::string*>($2);
							std::string type_name = *reinterpret_cast<std::string*>($3);
							pattern->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(@3, type_name), (rdoRuntime::RDOResourceTrace::ConvertStatus)$4, @4 );
							break;
						}
					}
				}
				| pat_rel_res IDENTIF_COLON IDENTIF pat_conv {
					// проверено для ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							parser->error( @4, rdo::format("Помимо статуса конвертора начала (%s), ожидается статус конвертора конца, потому что у операции есть событие конца", RDOPATPattern::StatusToStr((rdoRuntime::RDOResourceTrace::ConvertStatus)$4).c_str()) );
							break;
						}
						case RDOPATPattern::PT_IE  : 
						case RDOPATPattern::PT_Rule: {
							std::string rel_name  = *reinterpret_cast<std::string*>($2);
							std::string type_name = *reinterpret_cast<std::string*>($3);
							pattern->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(@3, type_name), (rdoRuntime::RDOResourceTrace::ConvertStatus)$4, @4 );
							break;
						}
					}
				}
				| pat_params_end IDENTIF_COLON IDENTIF_NoChange pat_conv {
					// проверено для ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string rel_name  = *reinterpret_cast<std::string*>($2);
							std::string type_name = *reinterpret_cast<std::string*>($3);
							YYLTYPE type_pos = @3;
							type_pos.last_line   = type_pos.first_line;
							type_pos.last_column = type_pos.first_column + type_name.length();
							YYLTYPE convertor_pos = @3;
							convertor_pos.first_line   = convertor_pos.last_line;
							convertor_pos.first_column = convertor_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).length();
							static_cast<RDOPATPatternOperation*>(pattern)->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(type_pos, type_name), rdoRuntime::RDOResourceTrace::CS_NoChange, (rdoRuntime::RDOResourceTrace::ConvertStatus)$4, convertor_pos, @4 );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->error( @4, rdo::format("У нерегулярного события нет события конца, а значит и второго статуса конвертора") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							parser->error( @4, rdo::format("У продукционного правила нет события конца, а значит и второго статуса конвертора") );
							break;
						}
					}
				}
				| pat_rel_res IDENTIF_COLON IDENTIF_NoChange pat_conv {
					// проверено для ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string rel_name  = *reinterpret_cast<std::string*>($2);
							std::string type_name = *reinterpret_cast<std::string*>($3);
							YYLTYPE type_pos = @3;
							type_pos.last_line   = type_pos.first_line;
							type_pos.last_column = type_pos.first_column + type_name.length();
							YYLTYPE convertor_pos = @3;
							convertor_pos.first_line   = convertor_pos.last_line;
							convertor_pos.first_column = convertor_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).length();
							static_cast<RDOPATPatternOperation*>(pattern)->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(type_pos, type_name), rdoRuntime::RDOResourceTrace::CS_NoChange, (rdoRuntime::RDOResourceTrace::ConvertStatus)$4, convertor_pos, @4 );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->error( @4, rdo::format("У нерегулярного события нет события конца, а значит и второго статуса конвертора") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							parser->error( @4, rdo::format("У продукционного правила нет события конца, а значит и второго статуса конвертора") );
							break;
						}
					}
				}
				| pat_params_end IDENTIF_COLON IDENTIF_NoChange_NoChange {
					// проверено для ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string rel_name  = *reinterpret_cast<std::string*>($2);
							std::string type_name = *reinterpret_cast<std::string*>($3);
							YYLTYPE type_pos = @3;
							type_pos.last_line   = type_pos.first_line;
							type_pos.last_column = type_pos.first_column + type_name.length();
							YYLTYPE convertor_begin_pos = @3;
							std::string str = reinterpret_cast<RDOLexer*>(lexer)->YYText();
							rdo::toLower( str );
							std::string::size_type first_nochange = str.find( "nochange" );
							int i = 0;
							while ( true ) {
								if ( str[i] == '\n' ) {
									convertor_begin_pos.first_line++;
									convertor_begin_pos.first_column = 0;
								} else if ( str[i] != '\r' ) {
									convertor_begin_pos.first_column++;
								}
								i++;
								if ( i == first_nochange ) break;
							}
							convertor_begin_pos.last_line   = convertor_begin_pos.first_line;
							convertor_begin_pos.last_column = convertor_begin_pos.first_column + RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).length();
							YYLTYPE convertor_end_pos = @3;
							convertor_end_pos.first_line   = convertor_end_pos.last_line;
							convertor_end_pos.first_column = convertor_end_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).length();
							static_cast<RDOPATPatternOperation*>(pattern)->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(type_pos, type_name), rdoRuntime::RDOResourceTrace::CS_NoChange, rdoRuntime::RDOResourceTrace::CS_NoChange, convertor_begin_pos, convertor_end_pos );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->error( @3, rdo::format("У нерегулярного события нет события конца, а значит и второго статуса конвертора") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							parser->error( @3, rdo::format("У продукционного правила нет события конца, а значит и второго статуса конвертора") );
							break;
						}
					}
				}
				| pat_rel_res IDENTIF_COLON IDENTIF_NoChange_NoChange {
					// проверено для ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string rel_name  = *reinterpret_cast<std::string*>($2);
							std::string type_name = *reinterpret_cast<std::string*>($3);
							YYLTYPE type_pos = @3;
							type_pos.last_line   = type_pos.first_line;
							type_pos.last_column = type_pos.first_column + type_name.length();
							YYLTYPE convertor_begin_pos = @3;
							std::string str = reinterpret_cast<RDOLexer*>(lexer)->YYText();
							rdo::toLower( str );
							std::string::size_type first_nochange = str.find( "nochange" );
							int i = 0;
							while ( true ) {
								if ( str[i] == '\n' ) {
									convertor_begin_pos.first_line++;
									convertor_begin_pos.first_column = 0;
								} else if ( str[i] != '\r' ) {
									convertor_begin_pos.first_column++;
								}
								i++;
								if ( i == first_nochange ) break;
							}
							convertor_begin_pos.last_line   = convertor_begin_pos.first_line;
							convertor_begin_pos.last_column = convertor_begin_pos.first_column + RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).length();
							YYLTYPE convertor_end_pos = @3;
							convertor_end_pos.first_line   = convertor_end_pos.last_line;
							convertor_end_pos.first_column = convertor_end_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).length();
							static_cast<RDOPATPatternOperation*>(pattern)->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(type_pos, type_name), rdoRuntime::RDOResourceTrace::CS_NoChange, rdoRuntime::RDOResourceTrace::CS_NoChange, convertor_begin_pos, convertor_end_pos );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->error( @3, rdo::format("У нерегулярного события нет события конца, а значит и второго статуса конвертора") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							parser->error( @3, rdo::format("У продукционного правила нет события конца, а значит и второго статуса конвертора") );
							break;
						}
					}
				}
				| pat_params_end IDENTIF_COLON IDENTIF_NoChange {
					// проверено для ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							parser->error( @3, rdo::format("Помимо статуса конвертора начала (%s), ожидается статус конвертора конца, потому что у операции есть событие конца", RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).c_str()) );
							break;
						}
						case RDOPATPattern::PT_IE  : 
						case RDOPATPattern::PT_Rule: {
							std::string rel_name  = *reinterpret_cast<std::string*>($2);
							std::string type_name = *reinterpret_cast<std::string*>($3);
							YYLTYPE type_pos = @3;
							type_pos.last_line   = type_pos.first_line;
							type_pos.last_column = type_pos.first_column + type_name.length();
							YYLTYPE convertor_pos = @3;
							convertor_pos.first_line   = convertor_pos.last_line;
							convertor_pos.first_column = convertor_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).length();
							pattern->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(type_pos, type_name), rdoRuntime::RDOResourceTrace::CS_NoChange, convertor_pos );
							break;
						}
					}
				}
				| pat_rel_res IDENTIF_COLON IDENTIF_NoChange {
					// проверено для ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							parser->error( @3, rdo::format("Помимо статуса конвертора начала (%s), ожидается статус конвертора конца, потому что у операции есть событие конца", RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).c_str()) );
							break;
						}
						case RDOPATPattern::PT_IE  : 
						case RDOPATPattern::PT_Rule: {
							std::string rel_name  = *reinterpret_cast<std::string*>($2);
							std::string type_name = *reinterpret_cast<std::string*>($3);
							YYLTYPE type_pos = @3;
							type_pos.last_line   = type_pos.first_line;
							type_pos.last_column = type_pos.first_column + type_name.length();
							YYLTYPE convertor_pos = @3;
							convertor_pos.first_line   = convertor_pos.last_line;
							convertor_pos.first_column = convertor_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).length();
							pattern->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(type_pos, type_name), rdoRuntime::RDOResourceTrace::CS_NoChange, convertor_pos );
							break;
						}
					}
				}
				| pat_params_end IDENTIF_COLON IDENTIF IDENTIF_NoChange {
					// проверено для ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string  rel_name      = *reinterpret_cast<std::string*>($2);
							std::string  type_name     = *reinterpret_cast<std::string*>($3);
							std::string* convert_begin = reinterpret_cast<std::string*>($4);
							YYLTYPE convertor_begin_pos = @4;
							convertor_begin_pos.last_line   = convertor_begin_pos.first_line;
							convertor_begin_pos.last_column = convertor_begin_pos.first_column + convert_begin->length();
							YYLTYPE convertor_end_pos = @4;
							convertor_end_pos.first_line   = convertor_end_pos.last_line;
							convertor_end_pos.first_column = convertor_end_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).length();
							static_cast<RDOPATPatternOperation*>(pattern)->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(@3, type_name), pattern->StrToStatus( *convert_begin, convertor_begin_pos ), rdoRuntime::RDOResourceTrace::CS_NoChange, convertor_begin_pos, convertor_end_pos );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->error( @4, rdo::format("У нерегулярного события нет события конца, а значит и второго статуса конвертора") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							parser->error( @4, rdo::format("У продукционного правила нет события конца, а значит и второго статуса конвертора") );
							break;
						}
					}
				}
				| pat_rel_res IDENTIF_COLON IDENTIF IDENTIF_NoChange {
					// проверено для ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string  rel_name      = *reinterpret_cast<std::string*>($2);
							std::string  type_name     = *reinterpret_cast<std::string*>($3);
							std::string* convert_begin = reinterpret_cast<std::string*>($4);
							YYLTYPE convertor_begin_pos = @4;
							convertor_begin_pos.last_line   = convertor_begin_pos.first_line;
							convertor_begin_pos.last_column = convertor_begin_pos.first_column + convert_begin->length();
							YYLTYPE convertor_end_pos = @4;
							convertor_end_pos.first_line   = convertor_end_pos.last_line;
							convertor_end_pos.first_column = convertor_end_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).length();
							static_cast<RDOPATPatternOperation*>(pattern)->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(@3, type_name), pattern->StrToStatus( *convert_begin, convertor_begin_pos ), rdoRuntime::RDOResourceTrace::CS_NoChange, convertor_begin_pos, convertor_end_pos );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->error( @4, rdo::format("У нерегулярного события нет события конца, а значит и второго статуса конвертора") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							parser->error( @4, rdo::format("У продукционного правила нет события конца, а значит и второго статуса конвертора") );
							break;
						}
					}
				}
				| pat_params_end error {
					parser->error( @2, "Ошибка в описании релевантных ресурсов" );
				}
				| pat_rel_res error {
					parser->error( @2, "Ошибка в описании релевантных ресурсов" );
				}
				| pat_params_end IDENTIF_COLON error {
					parser->lexer_loc_set( &(@2), &(@3) );
					parser->error( "Ожидается описатель (имя типа или ресурса)" );
				}
				| pat_rel_res IDENTIF_COLON error {
					parser->lexer_loc_set( &(@2), &(@3) );
					parser->error( "Ожидается описатель (имя типа или ресурса)" );
				}
				| pat_params_end IDENTIF_COLON IDENTIF error {
					parser->lexer_loc_set( &(@3), &(@4) );
					if ( parser->getLastPATPattern()->isHaveConvertEnd() ) {
						parser->error( "Ожидается статус конвертора начала" );
					} else {
						parser->error( "Ожидается статус конвертора" );
					}
				}
				| pat_rel_res IDENTIF_COLON IDENTIF error {
					parser->lexer_loc_set( &(@3), &(@4) );
					if ( parser->getLastPATPattern()->isHaveConvertEnd() ) {
						parser->error( "Ожидается статус конвертора начала" );
					} else {
						parser->error( "Ожидается статус конвертора" );
					}
				}
				| pat_params_end IDENTIF_COLON IDENTIF pat_conv error {
					switch ( parser->getLastPATPattern()->getType() ) {
						case RDOPATPattern::PT_Rule: {
							parser->error( @5, "Ожидается способ выбора (first/with_min/with_max) или $Body" );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->error( @5, "Ожидается способ выбора (first/with_min/with_max) или $Time" );
							break;
						}
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							parser->error( @4, @5, rdo::format("Ожидается статус конвертора конца, найдено: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
							break;
						}
					}
				}
				| pat_rel_res IDENTIF_COLON IDENTIF pat_conv error {
					switch ( parser->getLastPATPattern()->getType() ) {
						case RDOPATPattern::PT_Rule: {
							parser->error( @5, "Ожидается способ выбора (first/with_min/with_max) или $Body" );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->error( @5, "Ожидается способ выбора (first/with_min/with_max) или $Time" );
							break;
						}
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							parser->error( @4, @5, rdo::format("Ожидается статус конвертора конца, найдено: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
						}
					}
				}
				| pat_params_end IDENTIF_COLON IDENTIF_NoChange error {
					switch ( parser->getLastPATPattern()->getType() ) {
						case RDOPATPattern::PT_Rule: {
							parser->error( @4, "Ожидается способ выбора (first/with_min/with_max) или $Body" );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->error( @4, "Ожидается способ выбора (first/with_min/with_max) или $Time" );
							break;
						}
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							parser->error( @3, @4, rdo::format("Ожидается статус конвертора конца, найдено: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
						}
					}
				}
				| pat_rel_res IDENTIF_COLON IDENTIF_NoChange error {
					switch ( parser->getLastPATPattern()->getType() ) {
						case RDOPATPattern::PT_Rule: {
							parser->error( @4, "Ожидается способ выбора (first/with_min/with_max) или $Body" );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->error( @4, "Ожидается способ выбора (first/with_min/with_max) или $Time" );
							break;
						}
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							parser->error( @3, @4, rdo::format("Ожидается статус конвертора конца, найдено: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
							break;
						}
					}
				};

pat_conv:	Keep				{ $$ = rdoRuntime::RDOResourceTrace::CS_Keep;     }
			| Create_keyword	{ $$ = rdoRuntime::RDOResourceTrace::CS_Create;   }
			| Erase				{ $$ = rdoRuntime::RDOResourceTrace::CS_Erase;    }
			| NonExist			{ $$ = rdoRuntime::RDOResourceTrace::CS_NonExist; };

pat_common_choice:	pat_rel_res
					| pat_rel_res first_keyword {
						RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
						if ( pattern->getType() == RDOPATPattern::PT_IE ) {
							parser->error( @2, "В нерегулярном событии не используется способ выбора релевантных ресурсов" );
						} else {
//							pattern->setCommonChoiceFirst();
							RDOFUNArithm* arithm = new RDOFUNArithm( parser, 1, @2 );
							arithm->setSrcPos( @2 );
							arithm->setSrcText( "first" );
							pattern->setCommonChoiceWithMax( arithm );
						}
					}
					| pat_rel_res with_min fun_arithm {
						RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
						if ( pattern->getType() == RDOPATPattern::PT_IE ) {
							parser->error( @2, "В нерегулярном событии не используется способ выбора релевантных ресурсов" );
						} else {
							RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($3);
							arithm->setSrcPos( @2, @3 );
							arithm->setSrcText( "with_min " + arithm->src_text() );
							pattern->setCommonChoiceWithMin( arithm );
						}
					}
					| pat_rel_res with_max fun_arithm {
						RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
						if ( pattern->getType() == RDOPATPattern::PT_IE ) {
							parser->error( @2, "В нерегулярном событии не используется способ выбора релевантных ресурсов" );
						} else {
							RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($3);
							arithm->setSrcPos( @2, @3 );
							arithm->setSrcText( "with_max " + arithm->src_text() );
							pattern->setCommonChoiceWithMax( arithm );
						}
					};

pat_time:	pat_common_choice Body {
				RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
				switch ( pattern->getType() ) {
					case RDOPATPattern::PT_IE       :
					case RDOPATPattern::PT_Operation:
					case RDOPATPattern::PT_Keyboard : {
						parser->error( @2, "Перед $Body пропущено ключевое слово $Time" );
						break;
					}
				}
			}
			| pat_common_choice Time '=' fun_arithm Body {
				RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
				RDOFUNArithm*  arithm  = reinterpret_cast<RDOFUNArithm*>($4);
				arithm->setSrcPos( @2, @4 );
				arithm->setSrcText( "$Time = " + arithm->src_text() );
				pattern->setTime( arithm );
			}
			| pat_common_choice error {
				RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
				switch ( pattern->getType() ) {
					case RDOPATPattern::PT_Rule: {
						parser->error( @2, rdo::format("Ожидается $Body, найдено: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
						break;
					}
					case RDOPATPattern::PT_IE       :
					case RDOPATPattern::PT_Operation:
					case RDOPATPattern::PT_Keyboard : {
						parser->error( @2, rdo::format("Ожидается $Time, найдено: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
						break;
					}
				}
			};

pat_body:	pat_time IDENTIF {
				RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
				std::string    name    = *reinterpret_cast<std::string*>($2);
				pattern->addRelResBody( RDOParserSrcInfo( @2, name ) );
			}
			| pat_convert IDENTIF {
				RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
				std::string    name    = *reinterpret_cast<std::string*>($2);
				pattern->addRelResBody( RDOParserSrcInfo( @2, name ) );
			}
			| pat_time error {
				std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
				parser->error( @2, rdo::format("Неизвестный релевантный ресурс: %s", str.c_str()) );
			}
			| pat_convert error {
				std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
				parser->error( @2, rdo::format("Неизвестный релевантный ресурс: %s", str.c_str()) );
			};

pat_res_usage:	pat_body pat_choice pat_order {
					RDOPATChoiceFrom* choice_from = reinterpret_cast<RDOPATChoiceFrom*>($2);
					choice_from->setSrcPos( @2 );
					RDOPATChoiceOrder* choice_order = reinterpret_cast<RDOPATChoiceOrder*>($3);
					choice_order->setSrcPos( @3 );
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					pattern->addRelResUsage( choice_from, choice_order );
				};

pat_choice: /* empty */ {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceEmpty;
				$$ = (int) new RDOPATChoiceFrom( parser->getLastPATPattern()->currRelRes, RDOParserSrcInfo( "Choice NoCheck" ), RDOPATChoiceFrom::ch_empty );
			}
			| pat_choice_nocheck {
				$$ = (int) new RDOPATChoiceFrom( parser->getLastPATPattern()->currRelRes, RDOParserSrcInfo( "Choice NoCheck" ), RDOPATChoiceFrom::ch_nocheck );
			}
			| pat_choice_from fun_logic {
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				$$ = (int) new RDOPATChoiceFrom( parser->getLastPATPattern()->currRelRes, RDOParserSrcInfo( "Choice from " + logic->src_text() ), RDOPATChoiceFrom::ch_from, logic );
			};

pat_choice_nocheck: Choice NoCheck {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceNoCheck;
			};

pat_choice_from: Choice from_keyword {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceFrom;
			};

pat_order:	/* empty */ {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceOrderEmpty;
				$$ = (int) new RDOPATChoiceOrder( parser->getLastPATPattern()->currRelRes, RDOParserSrcInfo(), rdoRuntime::RDOSelectResourceCalc::order_empty );
			}
			| pat_choice_first {
				$$ = (int) new RDOPATChoiceOrder( parser->getLastPATPattern()->currRelRes, RDOParserSrcInfo( "first" ), rdoRuntime::RDOSelectResourceCalc::order_first );
			}
			| pat_choice_with_min fun_arithm {
				RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($2);
				$$ = (int) new RDOPATChoiceOrder( parser->getLastPATPattern()->currRelRes, RDOParserSrcInfo( "with_min " + arithm->src_text() ), rdoRuntime::RDOSelectResourceCalc::order_with_min, arithm );
			}
			| pat_choice_with_max fun_arithm {
				RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($2);
				$$ = (int) new RDOPATChoiceOrder( parser->getLastPATPattern()->currRelRes, RDOParserSrcInfo( "with_max " + arithm->src_text() ), rdoRuntime::RDOSelectResourceCalc::order_with_max, arithm );
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
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					RDORelevantResource* rel_res = pattern->currRelRes;
					std::string str;
					if ( rel_res->choice_order->type != rdoRuntime::RDOSelectResourceCalc::order_empty ) {
						str = "Сразу после ключевого слова " + rel_res->choice_order->asString();
					} else if ( rel_res->choice_from->type != RDOPATChoiceFrom::ch_empty ) {
						str = "Сразу после условия выбора";
					} else {
						str = "Сразу после имени";
					}
					if ( rel_res->begin != rdoRuntime::RDOResourceTrace::CS_NoChange && rel_res->begin != rdoRuntime::RDOResourceTrace::CS_Erase && rel_res->begin != rdoRuntime::RDOResourceTrace::CS_NonExist ) {
						switch ( pattern->getType() ) {
							case RDOPATPattern::PT_IE       : parser->error( @1, rdo::format("%s ожидается ключевое слово Convert_event для релевантного ресурса '%s', т.к. его статус '%s', но найдено: %s", str.c_str(), rel_res->getName().c_str(), RDOPATPattern::StatusToStr(rel_res->begin).c_str(), reinterpret_cast<RDOLexer*>(lexer)->YYText()) ); break;
							case RDOPATPattern::PT_Rule     : parser->error( @1, rdo::format("%s ожидается ключевое слово Convert_rule для релевантного ресурса '%s', т.к. его статус '%s', но найдено: %s", str.c_str(), rel_res->getName().c_str(), RDOPATPattern::StatusToStr(rel_res->begin).c_str(), reinterpret_cast<RDOLexer*>(lexer)->YYText()) ); break;
							case RDOPATPattern::PT_Operation:
							case RDOPATPattern::PT_Keyboard : parser->error( @1, rdo::format("%s ожидается ключевое слово Convert_begin для релевантного ресурса '%s', т.к. его статус '%s', но найдено: %s", str.c_str(), rel_res->getName().c_str(), RDOPATPattern::StatusToStr(rel_res->begin).c_str(), reinterpret_cast<RDOLexer*>(lexer)->YYText()) ); break;
						}
//						parser->error( "Converter needed for \"" + *rel_res->getName() + "\" relevant resource in pattern \"" + getName() + "\"" );
					}
					if ( rel_res->end != rdoRuntime::RDOResourceTrace::CS_NoChange && rel_res->end != rdoRuntime::RDOResourceTrace::CS_Erase && rel_res->end != rdoRuntime::RDOResourceTrace::CS_NonExist ) {
						switch ( pattern->getType() ) {
							case RDOPATPattern::PT_IE       : parser->error( @1, "Внутренняя ошибка" ); break;
							case RDOPATPattern::PT_Rule     : parser->error( @1, "Внутренняя ошибка" ); break;
							case RDOPATPattern::PT_Operation:
							case RDOPATPattern::PT_Keyboard : parser->error( @1, rdo::format("%s ожидается ключевое слово Convert_end для релевантного ресурса '%s', т.к. его статус '%s', но найдено: %s", str.c_str(), rel_res->getName().c_str(), RDOPATPattern::StatusToStr(rel_res->begin).c_str(), reinterpret_cast<RDOLexer*>(lexer)->YYText()) ); break;
						}
					}
				}
				| pat_res_usage convert_begin pat_trace pat_params_set {
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					if ( pattern->getType() != RDOPATPattern::PT_Operation && pattern->getType() != RDOPATPattern::PT_Keyboard ) {
						std::string type = "";
						switch ( pattern->getType() ) {
							case RDOPATPattern::PT_IE       : type = "нерегулярном событии"; break;
							case RDOPATPattern::PT_Rule     : type = "продукционном правиле"; break;
						}
						parser->error( @2, rdo::format("Ключевое слово Convert_begin может быть использовано в обыкновенной или клавиатурной операции, но не в %s '%s'", type.c_str(), pattern->getName().c_str()) );
					}
					RDOPATParamSet* par_set = reinterpret_cast<RDOPATParamSet*>($4);
					par_set->setSrcPos( @4 );
					static_cast<RDOPATPatternOperation*>(pattern)->addRelResConvertBeginEnd( $3 != 0, par_set, false, NULL, @2, @2, @3, @3 );
				}
				| pat_res_usage pat_params_set convert_end pat_trace pat_params_set {
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					if ( pattern->getType() != RDOPATPattern::PT_Operation && pattern->getType() != RDOPATPattern::PT_Keyboard ) {
						std::string type = "";
						switch ( pattern->getType() ) {
							case RDOPATPattern::PT_IE       : type = "нерегулярном событии"; break;
							case RDOPATPattern::PT_Rule     : type = "продукционном правиле"; break;
						}
						parser->error( @3, rdo::format("Ключевое слово Convert_end может быть использовано в обыкновенной и клавиатурной операции, но не в %s '%s'", type.c_str(), pattern->getName().c_str()) );
					}
					pattern->currRelRes->deleteParamSetBegin();
					RDOPATParamSet* par_set = reinterpret_cast<RDOPATParamSet*>($5);
					par_set->setSrcPos( @5 );
					static_cast<RDOPATPatternOperation*>(pattern)->addRelResConvertBeginEnd( false, NULL, $4 != 0, par_set, @3, @3, @4, @4 );
				}
				| pat_res_usage convert_begin pat_trace pat_params_set convert_end pat_trace pat_params_set {
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					if ( pattern->getType() != RDOPATPattern::PT_Operation && pattern->getType() != RDOPATPattern::PT_Keyboard ) {
						std::string type = "";
						switch ( pattern->getType() ) {
							case RDOPATPattern::PT_IE       : type = "нерегулярном событии"; break;
							case RDOPATPattern::PT_Rule     : type = "продукционном правиле"; break;
						}
						parser->error( @2, rdo::format("Ключевые слова Convert_begin и Convert_end могут быть использованы в обыкновенной и клавиатурной операции, но не в %s '%s'", type.c_str(), pattern->getName().c_str()) );
					}
					RDOPATParamSet* par_set_begin = reinterpret_cast<RDOPATParamSet*>($4);
					par_set_begin->setSrcPos( @4 );
					RDOPATParamSet* par_set_end = reinterpret_cast<RDOPATParamSet*>($7);
					par_set_end->setSrcPos( @7 );
					static_cast<RDOPATPatternOperation*>(pattern)->addRelResConvertBeginEnd( $3 != 0, par_set_begin, $6 != 0, par_set_end, @2, @5, @3, @6 );
				}
				| pat_res_usage convert_rule pat_trace pat_params_set {
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					if ( pattern->getType() != RDOPATPattern::PT_Rule ) {
						std::string type = "";
						switch ( pattern->getType() ) {
							case RDOPATPattern::PT_IE       : type = "нерегулярном событии"; break;
							case RDOPATPattern::PT_Operation: type = "операции"; break;
							case RDOPATPattern::PT_Keyboard : type = "клавиатурной операции"; break;
						}
						parser->error( @2, rdo::format("Ключевое слово Convert_rule может быть использовано в продукционном правиле, но не в %s '%s'", type.c_str(), pattern->getName().c_str()) );
					}
					RDOPATParamSet* par_set = reinterpret_cast<RDOPATParamSet*>($4);
					par_set->setSrcPos( @4 );
					pattern->addRelResConvert( $3 != 0, par_set, @2, @3 );
				}
				| pat_res_usage convert_event pat_trace pat_params_set {
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					if ( pattern->getType() != RDOPATPattern::PT_IE ) {
						std::string type = "";
						switch ( pattern->getType() ) {
							case RDOPATPattern::PT_Rule     : type = "продукционном правиле"; break;
							case RDOPATPattern::PT_Operation: type = "операции"; break;
							case RDOPATPattern::PT_Keyboard : type = "клавиатурной операции"; break;
						}
						parser->error( @2, rdo::format("Ключевое слово Convert_event может быть использовано в нерегулярном событии, но не в %s '%s'", type.c_str(), pattern->getName().c_str()) );
					}
					RDOPATParamSet* par_set = reinterpret_cast<RDOPATParamSet*>($4);
					par_set->setSrcPos( @4 );
					pattern->addRelResConvert( $3 != 0, par_set, @2, @3 );
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

pat_params_set:	/* empty */	{
					RDOPATParamSet* par_set = parser->getLastPATPattern()->currRelRes->createParamSet();
					YYLTYPE pos = @0;
					pos.first_line   = pos.last_line;
					pos.first_column = pos.last_column;
					par_set->setSrcPos( pos );
					$$ = (int)par_set;
				}
				|	pat_params_set IDENTIF_set fun_arithm	{
					RDOPATParamSet* param_set  = reinterpret_cast<RDOPATParamSet*>($1);
					RDOFUNArithm*   arithm     = reinterpret_cast<RDOFUNArithm*>($3);
					std::string     param_name = *reinterpret_cast<std::string*>($2);
					YYLTYPE param_name_pos = @2;
					param_name_pos.last_line   = param_name_pos.first_line;
					param_name_pos.last_column = param_name_pos.first_column + param_name.length();
					param_set->addSet( param_name, param_name_pos, arithm );
				}
				|	pat_params_set IDENTIF_NoChange			{
					RDOPATParamSet* param_set  = reinterpret_cast<RDOPATParamSet*>($1);
					std::string     param_name = *reinterpret_cast<std::string*>($2);
					YYLTYPE param_name_pos = @2;
					param_name_pos.last_line   = param_name_pos.first_line;
					param_name_pos.last_column = param_name_pos.first_column + param_name.length();
					param_set->addSet( param_name, param_name_pos );
				};

pat_pattern:	pat_convert End {
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					pattern->end();
				};
//				| pat_time  End { ((RDOPATPattern *)$1)->end(); $$ = $1; };

// ----------------------------------------------------------------------------
// ---------- Описание типа параметра
// ----------------------------------------------------------------------------
param_type:		integer param_int_diap param_int_default_val {
					RDORTPIntDiap* diap = reinterpret_cast<RDORTPIntDiap*>($2);
					RDORTPIntDefVal* dv = reinterpret_cast<RDORTPIntDefVal*>($3);
					RDORTPIntParamType* rp = new RDORTPIntParamType( parser->getLastParsingObject(), diap, dv, RDOParserSrcInfo( @1, @3 ) );
					$$ = (int)rp;
				}
				| real param_real_diap param_real_default_val {
					RDORTPRealDiap* diap = reinterpret_cast<RDORTPRealDiap*>($2);
					RDORTPRealDefVal* dv = reinterpret_cast<RDORTPRealDefVal*>($3);
					RDORTPRealParamType* rp = new RDORTPRealParamType( parser->getLastParsingObject(), diap, dv, RDOParserSrcInfo( @1, @3 ) );
					$$ = (int)rp;
				}
				| param_enum param_enum_default_val {
					reinterpret_cast<RDOLexer*>(lexer)->enum_param_cnt = 0;
					RDORTPEnum* enu      = reinterpret_cast<RDORTPEnum*>($1);
					RDORTPEnumDefVal* dv = reinterpret_cast<RDORTPEnumDefVal*>($2);
					if ( dv->isExist() ) {
						enu->findEnumValueWithThrow( dv->src_pos(), dv->getEnumValue() ); // Если не найдено, то будет сообщение об ошибке, т.е. throw
					}
					RDORTPEnumParamType* rp = new RDORTPEnumParamType( parser->getLastParsingObject(), enu, dv, RDOParserSrcInfo( @1, @2 ) );
					$$ = (int)rp;
				}
				| param_such_as {
					RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->getName() + "." : "") + param->getName() );
					$$ = (int)param->getType()->constructSuchAs( src_info );
				}
				| param_such_as '=' INT_CONST {
					RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1, @3 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->getName() + "." : "") + param->getName() );
					$$ = (int)param->getType()->constructSuchAs( (int)$3, src_info, RDOParserSrcInfo( @3 ) );
				}
				| param_such_as '=' REAL_CONST {
					RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1, @3 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->getName() + "." : "") + param->getName() );
					$$ = (int)param->getType()->constructSuchAs( *(double*)$3, src_info, RDOParserSrcInfo( @3 ) );
				}
				| param_such_as '=' IDENTIF {
					RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1, @3 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->getName() + "." : "") + param->getName() );
					$$ = (int)param->getType()->constructSuchAs( *(std::string*)$3, src_info, RDOParserSrcInfo( @3 ) );
				}
				| param_such_as '=' error {
					parser->error( "Ожидается зачение по-умолчанию" );
				}
				| param_such_as error {
					parser->error( "Ожидается окончание описания параметра-ссылки, например, зачение по-умолчанию" );
				};
/*
				| integer error {
					parser->error( @2, "Ошибка после ключевого слова integer. Возможно, не хватает значения по-умолчанию." );
//					parser->error( rdoSimulator::RDOSyntaxError::RTP_WAITING_FOR_INT_PARAM_END );
				}
				| real error {
					parser->error( @2, "Ошибка после ключевого слова real. Возможно, не хватает значения по-умолчанию." );
//					parser->error( rdoSimulator::RDOSyntaxError::RTP_WAITING_FOR_REAL_PARAM_END );
				}
				| param_enum error {
					parser->error( @2, "Ошибка после перечислимого типа. Возможно, не хватает значения по-умолчанию." );
//					parser->error( rdoSimulator::RDOSyntaxError::RTP_WAITING_FOR_ENUM_PARAM_END );
				};
*/
param_int_diap:	/* empty */ {
					YYLTYPE pos = @0;
					pos.first_line   = pos.last_line;
					pos.first_column = pos.last_column;
					RDORTPIntDiap* diap = new RDORTPIntDiap( parser, pos );
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint INT_CONST ']' {
					RDORTPIntDiap* diap = new RDORTPIntDiap( parser, $2, $4, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint REAL_CONST {
					parser->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' REAL_CONST dblpoint INT_CONST {
					parser->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' INT_CONST dblpoint REAL_CONST {
					parser->error( @4, rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' INT_CONST dblpoint INT_CONST error {
					parser->error( @4, "Диапазон задан неверно" );
				}
				| '[' INT_CONST dblpoint error {
					parser->error( @4, "Диапазон задан неверно" );
				}
				| '[' error {
					parser->error( @2, "Диапазон задан неверно" );
//					parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_RANGE );
				};

param_real_diap:	/* empty */ {
					YYLTYPE pos = @0;
					pos.first_line   = pos.last_line;
					pos.first_column = pos.last_column;
					RDORTPRealDiap* diap = new RDORTPRealDiap(parser, pos );
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint REAL_CONST ']' {
					double min = *reinterpret_cast<double*>($2);
					double max = *reinterpret_cast<double*>($4);
					RDORTPRealDiap* diap = new RDORTPRealDiap( parser, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint INT_CONST ']' {
					double min = *reinterpret_cast<double*>($2);
					double max = $4;
					RDORTPRealDiap* diap = new RDORTPRealDiap( parser, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint REAL_CONST ']' {
					double min = $2;
					double max = *reinterpret_cast<double*>($4);
					RDORTPRealDiap* diap = new RDORTPRealDiap( parser, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint INT_CONST ']' {
					double min = $2;
					double max = $4;
					RDORTPRealDiap* diap = new RDORTPRealDiap( parser, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint REAL_CONST error {
					parser->error( @4, "Диапазон задан неверно" );
				}
				| '[' REAL_CONST dblpoint INT_CONST error {
					parser->error( @4, "Диапазон задан неверно" );
				}
				| '[' INT_CONST dblpoint REAL_CONST error {
					parser->error( @4, "Диапазон задан неверно" );
				}
				| '[' INT_CONST dblpoint INT_CONST error {
					parser->error( @4, "Диапазон задан неверно" );
				}
				| '[' REAL_CONST dblpoint error {
					parser->error( @4, "Диапазон задан неверно" );
				}
				| '[' INT_CONST dblpoint error {
					parser->error( @4, "Диапазон задан неверно" );
				}
				| '[' error {
					parser->error( @2, "Диапазон задан неверно" );
//					parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_RANGE );
				};

param_int_default_val:	/* empty */ {
						YYLTYPE pos = @0;
						pos.first_line   = pos.last_line;
						pos.first_column = pos.last_column;
						$$ = (int)(new RDORTPIntDefVal(parser, pos));
					}
					| '=' INT_CONST {
						$$ = (int)(new RDORTPIntDefVal(parser, $2, RDOParserSrcInfo( @1, @2 )));
					}
					| '=' REAL_CONST {
						// Целое число инициализируется вещественным: %f
						parser->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_DEFVAULT_INT_AS_REAL, *(double*)$2 );
					}
					| '=' {
						parser->error( @1, "Не указано значение по-умолчанию для целого типа" );
					}
					| '=' error {
						parser->error( @2, "Неверное значение по-умолчанию для целого типа" );
					};

param_real_default_val:	/* empty */ {
						YYLTYPE pos = @0;
						pos.first_line   = pos.last_line;
						pos.first_column = pos.last_column;
						$$ = (int)(new RDORTPRealDefVal(parser, pos));
					}
					| '=' REAL_CONST {
						$$ = (int)(new RDORTPRealDefVal(parser, *((double *)$2), RDOParserSrcInfo( @1, @2 )));
					}
					| '=' INT_CONST {
						$$ = (int)(new RDORTPRealDefVal(parser, $2, RDOParserSrcInfo( @1, @2 )));
					}
					| '=' {
						parser->error( @1, "Не указано значение по-умолчанию для вещественного типа" );
					}
					| '=' error {
						parser->error( @2, "Неверное значение по-умолчанию для вещественного типа" );
					};

param_enum:	'(' param_enum_list ')' {
				RDORTPEnum* enu = reinterpret_cast<RDORTPEnum*>($2);
				enu->setSrcPos( @1, @3 );
				enu->setSrcText( enu->src_text() + ")" );
				$$ = $2;
			}
			| '(' param_enum_list {
				parser->error( @2, "Перечисление должно заканчиваться скобкой" );
			};

param_enum_list: IDENTIF {
					RDORTPEnum* enu = new RDORTPEnum( parser->getLastParsingObject(), *(std::string *)$1 );
					std::string* first = reinterpret_cast<std::string*>($1);
					enu->setSrcText( "(" + *first );
					reinterpret_cast<RDOLexer*>(lexer)->enum_param_cnt = 1;
					$$ = (int)enu;
				}
				| param_enum_list ',' IDENTIF {
					if ( reinterpret_cast<RDOLexer*>(lexer)->enum_param_cnt >= 1 ) {
						RDORTPEnum* enu  = reinterpret_cast<RDORTPEnum*>($1);
						std::string next = *reinterpret_cast<std::string*>($3);
						enu->add( next, @3 );
						enu->setSrcText( enu->src_text() + ", " + next );
						$$ = (int)enu;
					} else {
						parser->error( "Ошибка в описании значений перечислимого типа" );
					}
				}
				| param_enum_list IDENTIF {
					if ( reinterpret_cast<RDOLexer*>(lexer)->enum_param_cnt >= 1 ) {
						parser->error( rdo::format("Пропущена запятая перед: %s", ((std::string*)$2)->c_str()) );
					} else {
						parser->error( "Ошибка в описании значений перечислимого типа" );
					}
				}
				| param_enum_list error {
					std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
					if ( str.empty() ) {
						parser->error( @1, "Ошибка в описании значений перечислимого типа" );
					} else {
						parser->error( @2, rdo::format( "Неверное значение перечислимого типа: %s", str.c_str() ) );
					}
				}
				| param_enum_list ',' INT_CONST {
					parser->error( @3, "Значение перечислимого типа не может начинаться с цифры" );
				}
				| param_enum_list ',' REAL_CONST {
					parser->error( @3, "Значение перечислимого типа не может начинаться с цифры" );
				}
				| INT_CONST {
					parser->error( @1, "Значение перечислимого типа не может начинаться с цифры" );
				}
				| REAL_CONST {
					parser->error( @1, "Значение перечислимого типа не может начинаться с цифры" );
				}
				| error {
					parser->error( @1, "Ошибка в описании значений перечислимого типа" );
				};

param_enum_default_val:	/* empty */ {
						YYLTYPE pos = @0;
						pos.first_line   = pos.last_line;
						pos.first_column = pos.last_column;
						$$ = (int)(new RDORTPEnumDefVal(parser, pos));
					}
					| '=' IDENTIF {
						$$ = (int)(new RDORTPEnumDefVal( parser, *(std::string*)$2, RDOParserSrcInfo( @1, @2 ) ));
					}
					| '=' {
						parser->error( @1, "Не указано значение по-умолчанию для перечислимого типа" );
					}
					| '=' error {
						parser->error( @2, "Неверное значение по-умолчанию для перечислимого типа" );
					};

param_such_as:	such_as IDENTIF '.' IDENTIF {
					std::string type  = *reinterpret_cast<std::string*>($2);
					std::string param = *reinterpret_cast<std::string*>($4);
					const RDORTPResType* const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					}
					const RDORTPParam* const rp = rt->findRTPParam( param );
					if ( !rp ) {
						parser->error( @4, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_PARAM, type.c_str(), param.c_str() );
					}
					$$ = (int)rp;
				}
				| such_as IDENTIF {
					std::string constName = *reinterpret_cast<std::string*>($2);
					const RDOFUNConstant* const cons = parser->findFUNConst( constName );
					if ( !cons ) {
						parser->error( @2, rdo::format("Ссылка на несуществующую константу: %s", constName.c_str()) );
					}
					$$ = (int)(cons->getDescr());
				}
				| such_as IDENTIF '.' {
					std::string type = *reinterpret_cast<std::string*>($2);
					const RDORTPResType* const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					} else {
						parser->error( @3, "Не указан параметер" );
					}
				}
				| such_as IDENTIF '.' error {
					std::string type = *reinterpret_cast<std::string*>($2);
					const RDORTPResType* const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					} else {
						parser->error( @4, "Ошибка при указании параметра" );
					}
				}
				| such_as IDENTIF error {
					std::string type = *reinterpret_cast<std::string*>($2);
					const RDORTPResType* const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					} else {
						parser->error( @2, "После имени типа должен быть указан параметер через точку" );
					}
				}
				| such_as error {
					parser->error( @2, "После ключевого слова such_as необходимо указать тип и параметер ресурса для ссылки" );
				};
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// ---------- Логические выражения
// ----------------------------------------------------------------------------
fun_logic:	fun_arithm '=' fun_arithm         { $$ = (int)(*(RDOFUNArithm *)$1 == *(RDOFUNArithm *)$3); }
			| fun_arithm neq fun_arithm       { $$ = (int)(*(RDOFUNArithm *)$1 != *(RDOFUNArithm *)$3); }
			| fun_arithm '<' fun_arithm       { $$ = (int)(*(RDOFUNArithm *)$1 <  *(RDOFUNArithm *)$3); }
			| fun_arithm '>' fun_arithm       { $$ = (int)(*(RDOFUNArithm *)$1 >  *(RDOFUNArithm *)$3); }
			| fun_arithm leq fun_arithm       { $$ = (int)(*(RDOFUNArithm *)$1 <= *(RDOFUNArithm *)$3); }
			| fun_arithm geq fun_arithm       { $$ = (int)(*(RDOFUNArithm *)$1 >= *(RDOFUNArithm *)$3); }
			| fun_logic and_keyword fun_logic { $$ = (int)(*(RDOFUNLogic *)$1 && *(RDOFUNLogic *)$3);   }
			| fun_logic or_keyword fun_logic  { $$ = (int)(*(RDOFUNLogic *)$1 || *(RDOFUNLogic *)$3);   }
			| '[' fun_logic ']' {
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setSrcPos( @1, @3 );
				logic->setSrcText( "[" + logic->src_text() + "]" );
				$$ = $2;
			}
			| '(' fun_logic ')' {
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setSrcPos( @1, @3 );
				logic->setSrcText( "(" + logic->src_text() + ")" );
				$$ = $2;
			}
			| not_keyword fun_logic {
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				RDOFUNLogic* logic_not = logic->operator_not();
				logic_not->setSrcPos( @1, @2 );
				logic_not->setSrcText( "not " + logic->src_text() );
				$$ = (int)logic_not;
			}
			| fun_group {
			}
			| fun_select_logic {
			}
			| '[' fun_logic error {
				parser->error( @2, "Ожидается закрывающаяся скобка" );
			}
			| '(' fun_logic error {
				parser->error( @2, "Ожидается закрывающаяся скобка" );
			}
			| error {
				parser->error( @1, "Ошибка в логическом выражении" );
			};

// ----------------------------------------------------------------------------
// ---------- Арифметические выражения
// ----------------------------------------------------------------------------
fun_arithm: fun_arithm '+' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 + *(RDOFUNArithm *)$3); }
			| fun_arithm '-' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 - *(RDOFUNArithm *)$3); }
			| fun_arithm '*' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 * *(RDOFUNArithm *)$3); }
			| fun_arithm '/' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 / *(RDOFUNArithm *)$3); }
			| '(' fun_arithm ')' {
				RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($2);
				arithm->setSrcPos( @1, @3 );
				arithm->setSrcText( "(" + arithm->src_text() + ")" );
				$$ = $2;
			}
			| fun_arithm_func_call {
			}
			| fun_select_arithm {
			}
			| IDENTIF '.' IDENTIF {
				$$ = (int)new RDOFUNArithm( parser, RDOParserSrcInfo( @1, *reinterpret_cast<std::string*>($1) ), RDOParserSrcInfo( @3, *reinterpret_cast<std::string*>($3) ) );
			}
			| INT_CONST                   { $$ = (int)new RDOFUNArithm( parser, (int)$1, RDOParserSrcInfo( @1, reinterpret_cast<RDOLexer*>(lexer)->YYText() ) );     }
			| REAL_CONST                  { $$ = (int)new RDOFUNArithm( parser, (double*)$1, RDOParserSrcInfo( @1, reinterpret_cast<RDOLexer*>(lexer)->YYText() ) ); }
			| IDENTIF                     { $$ = (int)new RDOFUNArithm( parser, *(std::string*)$1, @1 );                                                             }
			| '-' fun_arithm %prec UMINUS {
				RDOParserSrcInfo info;
				info.setSrcPos( @1, @2 );
				info.setSrcText( "-" + reinterpret_cast<RDOFUNArithm*>($2)->src_text() );
				$$ = (int)new RDOFUNArithm( parser, reinterpret_cast<RDOFUNArithm*>($2)->getType(), new rdoRuntime::RDOCalcUMinus( parser->runtime, reinterpret_cast<RDOFUNArithm*>($2)->createCalc() ), info );
			}
			| error {
				if ( @1.first_line = @1.last_line ) {
					std::string str = reinterpret_cast<RDOLexer*>(lexer)->YYText();
					if ( str.length() == 1 && str.find_first_of("!#`~@$%^&|:(),=[].*><+-/\\") != std::string::npos ) {
						parser->error( @1, rdo::format("Неизвестный символ: %s", str.c_str()) );
					} else {
						parser->error( @1, rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
					}
				} else {
					parser->error( @1, "Ошибка в арифметическом выражении" );
				}
			};

// ----------------------------------------------------------------------------
// ---------- Функции и последовательности
// ----------------------------------------------------------------------------
fun_arithm_func_call:	IDENTIF '(' fun_arithm_func_call_pars ')' {
							RDOFUNParams* fun    = reinterpret_cast<RDOFUNParams*>($3);
							std::string fun_name = *reinterpret_cast<std::string*>($1);
							fun->funseq_name.setSrcInfo( RDOParserSrcInfo(@1, fun_name) );
							fun->setSrcPos( @1, @4 );
							fun->setSrcText( fun_name + "(" + fun->src_text() + ")" );
							RDOFUNArithm* arithm = fun->createCall( fun_name );
							$$ = (int)arithm;
						}
						| IDENTIF '(' error {
							parser->error( @3, "Ошибка в параметрах функции" );
						};

fun_arithm_func_call_pars:	/* empty */ {
								RDOFUNParams* fun = new RDOFUNParams( parser );
								$$ = (int)fun;
							}
							| fun_arithm_func_call_pars fun_arithm {
								RDOFUNParams* fun    = reinterpret_cast<RDOFUNParams*>($1);
								RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($2);
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
							};

// ----------------------------------------------------------------------------
// ---------- Групповые выражения
// ----------------------------------------------------------------------------
fun_group_keyword:	Exist			{ $$ = 1; }
					| Not_Exist		{ $$ = 2; }
					| For_All		{ $$ = 3; }
					| Not_For_All	{ $$ = 4; };

fun_group_header:	fun_group_keyword '(' IDENTIF_COLON {
						std::string type_name = *reinterpret_cast<std::string*>($3);
						$$ = (int)(new RDOFUNGroupLogic( parser, $1, RDOParserSrcInfo(@3, type_name, RDOParserSrcInfo::psi_align_bytext) ));
					}
					| fun_group_keyword '(' error {
						parser->error( @3, "Ожидается имя типа" );
					}
					| fun_group_keyword error {
						parser->error( @1, "После имени функции ожидается октрывающаяся скобка" );
					};

fun_group:			fun_group_header fun_logic ')' {
						RDOFUNGroupLogic* groupfun = reinterpret_cast<RDOFUNGroupLogic*>($1);
						groupfun->setSrcPos( @1, @3 );
						$$ = (int)groupfun->createFunLogic( reinterpret_cast<RDOFUNLogic*>($2) );
					}
					| fun_group_header NoCheck ')' {
						RDOFUNGroupLogic* groupfun = reinterpret_cast<RDOFUNGroupLogic*>($1);
						groupfun->setSrcPos( @1, @3 );
						RDOFUNLogic* trueLogic = new RDOFUNLogic( groupfun, new rdoRuntime::RDOCalcConst( parser->runtime, 1 ) );
						trueLogic->setSrcPos( @2 );
						trueLogic->setSrcText( "NoCheck" );
						$$ = (int)groupfun->createFunLogic( trueLogic );
					}
					| fun_group_header fun_logic error {
						parser->error( @2, "Ожидается закрывающаяся скобка" );
					}
					| fun_group_header NoCheck error {
						parser->error( @2, "Ожидается закрывающаяся скобка" );
					};

// ----------------------------------------------------------------------------
// ---------- Select
// ----------------------------------------------------------------------------
fun_select_header:	Select '(' IDENTIF_COLON {
						std::string type_name = *reinterpret_cast<std::string*>($3);
						RDOFUNSelect* select = new RDOFUNSelect( parser, RDOParserSrcInfo(@3, type_name, RDOParserSrcInfo::psi_align_bytext) );
						select->setSrcText( "Select(" + type_name + ": " );
						$$ = (int)select;
					}
					| Select '(' error {
						parser->error( @3, "Ожидается имя типа" );
					}
					| Select error {
						parser->error( @1, "Ожидается октрывающаяся скобка" );
					};

fun_select_body:	fun_select_header fun_logic ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						RDOFUNLogic*  flogic = reinterpret_cast<RDOFUNLogic*>($2);
						select->setSrcText( select->src_text() + flogic->src_text() + ")" );
						select->initSelect( flogic );
					}
					| fun_select_header NoCheck ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						RDOParserSrcInfo logic_info(@2, "NoCheck");
						select->setSrcText( select->src_text() + logic_info.src_text() + ")" );
						rdoRuntime::RDOCalcConst* calc_nocheck = new rdoRuntime::RDOCalcConst( parser->runtime, 1 );
						RDOFUNLogic* flogic = new RDOFUNLogic( select, calc_nocheck, true );
						flogic->setSrcInfo( logic_info );
						select->initSelect( flogic );
					}
					| fun_select_header fun_logic error {
						parser->error( @2, "Ожидается закрывающаяся скобка" );
					}
					| fun_select_header NoCheck error {
						parser->error( @2, "Ожидается закрывающаяся скобка" );
					};

fun_select_keyword:	Exist			{ $$ = 1; }
					| Not_Exist		{ $$ = 2; }
					| For_All		{ $$ = 3; }
					| Not_For_All	{ $$ = 4; };

fun_select_logic:	fun_select_body '.' fun_select_keyword '(' fun_logic ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcPos( @1, @6 );
						RDOFUNLogic* logic = select->createFunSelectGroup( $3, reinterpret_cast<RDOFUNLogic*>($5) );
						$$ = (int)logic;
					}
					| fun_select_body '.' Empty_kw '(' ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcPos( @1, @5 );
						RDOParserSrcInfo empty_info(@3, @5, "Empty()");
						RDOFUNLogic* logic = select->createFunSelectEmpty( empty_info );
						$$ = (int)logic;
					}
					| fun_select_body error {
						parser->error( @1, "Ожидается '.' (точка) для вызова метода списка ресурсов" );
					}
					| fun_select_body '.' error {
						parser->error( @2, @3, "Ожидается метод списка ресурсов" );
					}
					| fun_select_body '.' fun_select_keyword error {
						parser->error( @3, "Ожидается октрывающаяся скобка" );
					}
					| fun_select_body '.' Empty_kw error {
						parser->error( @3, "Ожидается октрывающаяся скобка" );
					}
					| fun_select_body '.' fun_select_keyword '(' error ')' {
						parser->error( @5, "Ошибка в логическом выражении" );
					}
					| fun_select_body '.' Empty_kw '(' error {
						parser->error( @4, "Ожидается закрывающаяся скобка" );
					};

fun_select_arithm:	fun_select_body '.' Size_kw '(' ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcPos( @1, @5 );
						RDOParserSrcInfo size_info(@3, @5, "Size()");
						RDOFUNArithm* arithm = select->createFunSelectSize( size_info );
						$$ = (int)arithm;
					}
					| fun_select_body error {
						parser->error( @1, "Ожидается '.' (точка) для вызова метода списка ресурсов" );
					}
					| fun_select_body '.' error {
						parser->error( @2, @3, "Ожидается метод списка ресурсов: Size()" );
					}
					| fun_select_body '.' Size_kw error {
						parser->error( @3, "Ожидается октрывающаяся скобка" );
					}
					| fun_select_body '.' Size_kw '(' error {
						parser->error( @4, "Ожидается закрывающаяся скобка" );
					};

%%

}

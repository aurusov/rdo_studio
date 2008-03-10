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
#include "rdoparser_lexer.h"
#include "rdorss.h"
#include "rdortp.h"

#define PARSER reinterpret_cast<rdoParse::RDOLexer*>(lexer)->m_parser

namespace rdoParse 
{
%}

%left IDENTIF
%left ':'

%start rss_main

%%

rss_main:	/* empty */
		| rss_resources_begin rss_resources rss_resources_end
		| rss_resources_begin rss_resources {
			PARSER->error( @2, "После описания всех ресурсов ожидается ключевое слово $End" );
		}
		| error {
			if ( !PARSER->isHaveKWResources() ) {
				PARSER->error( @1, "Ожидается ключевое слово $Resources" );
			} else if ( PARSER->isHaveKWResourcesEnd() ) {
				PARSER->error( @1, "Ресурсы уже определены" );
			} else {
				PARSER->error( @1, rdoSimulator::RDOSyntaxError::UNKNOWN );
			}
		};

rss_resources_begin:	Resources {
			PARSER->setHaveKWResources( true );
		};

rss_resources_end:		End {
			PARSER->setHaveKWResourcesEnd( true );
		};

rss_resources:	/* empty */
		| rss_resources rss_res_descr;

rss_res_descr:	rss_res_type rss_trace rss_start_vals {
					RDORSSResource* res = reinterpret_cast<RDORSSResource*>($1);
					if ( res->currParam != res->getType()->getParams().end() ) {
						PARSER->error( @3, rdo::format("Заданы не все параметры ресурса: %s", res->getName().c_str()) );
					}
					res->setTrace( $2 != 0 );
				};

rss_res_type:	IDENTIF_COLON IDENTIF {
					std::string name = *reinterpret_cast<std::string*>($1);
					std::string type = *reinterpret_cast<std::string*>($2);
					const RDORTPResType* const resType = PARSER->findRTPResType( type );
					if ( !resType ) {
						PARSER->error( @2, rdo::format("Неизвестный тип ресурса: %s", type.c_str()) );
//						PARSER->error(("Invalid resource type: " + *type).c_str());
					}
					RDOParserSrcInfo src_info(@1, name, RDOParserSrcInfo::psi_align_bytext);
					const RDORSSResource* res = PARSER->findRSSResource( name );
					if ( res ) {
						PARSER->error_push_only( src_info, rdo::format( "Ресурс '%s' уже существует", name.c_str()) );
						PARSER->error_push_only( res->src_info(), "См. первое определение" );
						PARSER->error_push_done();
//						PARSER->error( ("Double resource name: " + *name).c_str() );
					}
					$$ = (int)new RDORSSResource( PARSER, src_info, resType );
				}
				| IDENTIF_COLON error {
					PARSER->error( @2, "Ожидается тип ресурса" );
				}
				| ':' {
					PARSER->error( @1, "Перед двоеточием ожидается имя ресурса" );
				}
				| error {
					PARSER->error( @1, "Ожидается имя ресурса" );
				};

rss_trace:	/* empty */		{ $$ = 0; }
			| trace_keyword	{ $$ = 1; }
			| no_trace		{ $$ = 0; };

rss_start_vals:	/* empty */
			| rss_start_vals rss_value;

rss_value:	'*' {
				if ( PARSER->getLastRSSResource()->currParam == PARSER->getLastRSSResource()->getType()->getParams().end() ) {
					PARSER->error_push_only( @1, "Слишком много параметров" );
					PARSER->error_push_only( PARSER->getLastRSSResource()->getType()->src_info(), "См. тип ресурса" );
					PARSER->error_push_done();
//					PARSER->error( "Too many parameters" );
				}
				try {
					rdoRuntime::RDOValue val = (*(PARSER->getLastRSSResource()->currParam))->getType()->getParamDefaultValue( @1 );
					PARSER->getLastRSSResource()->addValue( val );
					PARSER->getLastRSSResource()->currParam++;
				} catch ( RDOSyntaxException& err ) {
					PARSER->error_modify( rdo::format("Для параметра '%s': %s", (*(PARSER->getLastRSSResource()->currParam))->getName().c_str(), err.mess.c_str()) );
//					throw RDOSyntaxException( err.mess + " for parameter " + (*(PARSER->getLastRSSResource()->currParam))->getName().c_str() );
				}
			}
			| IDENTIF {
				if ( PARSER->getLastRSSResource()->currParam == PARSER->getLastRSSResource()->getType()->getParams().end() ) {
					PARSER->error_push_only( @1, rdo::format("Слишком много параметров. Лишний параметр: %s", ((std::string*)$1)->c_str()) );
					PARSER->error_push_only( PARSER->getLastRSSResource()->getType()->src_info(), "См. тип ресурса" );
					PARSER->error_push_done();
				}
				try {
					rdoRuntime::RDOValue val = (*(PARSER->getLastRSSResource()->currParam))->getType()->getRSSEnumValue(*(std::string *)$1, @1);
					PARSER->getLastRSSResource()->addValue( val );
					PARSER->getLastRSSResource()->currParam++;
				} catch( RDOSyntaxException& err ) {
					PARSER->error_modify( rdo::format("Для параметра '%s': %s", (*(PARSER->getLastRSSResource()->currParam))->getName().c_str(), err.mess.c_str()) );
				}
			}
			| INT_CONST {
				if ( PARSER->getLastRSSResource()->currParam == PARSER->getLastRSSResource()->getType()->getParams().end() ) {
					PARSER->error_push_only( @1, rdo::format("Слишком много параметров. Лишний параметр: %d", $1) );
					PARSER->error_push_only( PARSER->getLastRSSResource()->getType()->src_info(), "См. тип ресурса" );
					PARSER->error_push_done();
				}
				try {
					rdoRuntime::RDOValue val = (*(PARSER->getLastRSSResource()->currParam))->getType()->getRSSIntValue($1, @1);
					PARSER->getLastRSSResource()->addValue( val );
					PARSER->getLastRSSResource()->currParam++;
				} catch( RDOSyntaxException& err ) {
					PARSER->error_modify( rdo::format("Для параметра '%s': %s", (*(PARSER->getLastRSSResource()->currParam))->getName().c_str(), err.mess.c_str()) );
				}
			}
			| REAL_CONST {
				if ( PARSER->getLastRSSResource()->currParam == PARSER->getLastRSSResource()->getType()->getParams().end() ) {
					PARSER->error_push_only( @1, rdo::format("Слишком много параметров. Лишний параметр: %f", *((double*)$1)) );
					PARSER->error_push_only( PARSER->getLastRSSResource()->getType()->src_info(), "См. тип ресурса" );
					PARSER->error_push_done();
				}
				try {
					rdoRuntime::RDOValue val = (*(PARSER->getLastRSSResource()->currParam))->getType()->getRSSRealValue(*(double *)$1, @1);
					PARSER->getLastRSSResource()->addValue( val );
					PARSER->getLastRSSResource()->currParam++;
				} catch ( RDOSyntaxException& err ) {
					PARSER->error_modify( rdo::format("Для параметра '%s': %s", (*(PARSER->getLastRSSResource()->currParam))->getName().c_str(), err.mess.c_str()) );
				}
			}
			| error {
				PARSER->error( @1, rdo::format("Неправильное значение параметра: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
			};

%%

}

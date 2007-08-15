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
%token GENERATE				365
%token TERMINATE			366
%token ADVANCE				367
%token RELEASE				368

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
%token UMINUS				422

%{
#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoparser.h"
#include "rdoparser_rdo.h"
#include "rdofun.h"
#include "rdodpt.h"
#include "rdortp.h"
#include "rdorss.h"

namespace rdoParse 
{
%}

%left or_keyword
%left and_keyword
%left '+' '-'
%left '*' '/'
%left not_keyword
%left UMINUS

%start dptrtp_main

%%

/* ///////////////////////  GENERAL PART ///////////////////////////// */
dptrtp_main:
	| dptrtp_main Decision_point error End /* заглушка для $Decision_point */
	| dptrtp_main Activities error End     /* заглушка для $Activities     */
	| dptrtp_main dpt_process_end;

/* ///////////////////////  PROCESS ///////////////////////////// */

dpt_process:		dpt_process_begin dpt_process_input;

dpt_process_begin:	Process;

dpt_process_input:	/* empty */
					| dpt_process_input dpt_process_line;

dpt_process_line:	IDENTIF	{
						parser->error( rdo::format("Неизвестный оператор '%s'", ((std::string *)$1)->c_str()) );
					}
					| GENERATE fun_arithm {

	RDOPROCTransact::makeRTP( parser );

//	new RDOPROCTransact();
/*
	std::string* rtp_trans_param_name   = parser->registerName( "Время_создания" );
	std::string* rel_res_name = parser->registerName( "Транзакт" );
	std::string* ie_name      = parser->registerName( "PAT_GENERATE" );
	std::string* uniform_name = parser->registerName( "Равномерный" );
	std::string* time_now     = parser->registerName( "Time_now" );
	std::string* opr_name     = parser->registerName( "OPR_GENERATE" );

	// Создадим последовательность
	RDORTPRealResParam*    uniform_real_param            = new RDORTPRealResParam( new RDORTPRealDiap(), new RDORTPRealDefVal(0) );
	RDOFUNSequence::RDOFUNSequenceHeader*  uniform_seq_h = new RDOFUNSequence::RDOFUNSequenceHeader( uniform_name, uniform_real_param );
	RDOFUNSequenceUniform* uniform_seq                   = new RDOFUNSequenceUniform( uniform_seq_h, 123456789 );

	RDOPATPatternEvent* ie = new RDOPATPatternEvent( ie_name, true );
//	ie->addRelRes( rel_res_name, const_cast<std::string*>(transact_type->getName()), RDOPATPattern::CS_Create );

	RDOFUNParams* uniform_params = new RDOFUNParams();
	uniform_params->addParameter( new RDOFUNArithm( 0.25 ) );
	uniform_params->addParameter( new RDOFUNArithm( 0.75 ) );
	ie->setTime( const_cast<RDOFUNArithm*>(uniform_params->createCall( uniform_name )) );

	RDOPATParamsSet* generate_pat_params = new RDOPATParamsSet();
	generate_pat_params->addIdentif( rtp_trans_param_name, new RDOFUNArithm( time_now ) );
	ie->addRelResBody( rel_res_name );
	ie->addRelResConvertEvent( true, generate_pat_params );
	ie->end();

	// Создадим активность
	RDODPTFreeActivity* ie_opr = new RDODPTFreeActivity(opr_name, ie_name);
	ie_opr->end();
*/
					}
					| TERMINATE {
					}
					| ADVANCE fun_arithm {
					}
					| RELEASE IDENTIF {
					}
					| SEIZE {
	@$.first_column = @1.first_column;
	@$.first_line   = @1.first_line;
	@$.last_column  = @1.last_column;
	@$.last_line    = @1.first_column;
	parser->error( std::string(_T("Ожидается имя ресурса")).c_str() );
}

					| SEIZE IDENTIF {

	std::string res_name       = *(std::string*)$2;
	std::string rtp_param_name = "Состояние";
	std::string rtp_state_free = "Свободен";
	std::string rtp_state_buzy = "Занят";

	// Найти ресурс, если его нет, то создать
	RDORSSResource* res = const_cast<RDORSSResource*>(parser->findRSSResource( res_name ));
	if ( !res ) {

		// Сформировать имя типа по имени ресурса
		std::string res_type_name( RDOPROCProcess::name_prefix + res_name + RDOPROCProcess::name_sufix );

		// Найти тип ресурса, если его нет, то создать тип и положить в него параметр
		RDORTPParam* rtp_param = NULL;
		RDORTPResType* res_type = const_cast<RDORTPResType*>(parser->findRTPResType( res_type_name ));
		if ( !res_type ) {
			// Создадим тип ресурса
			res_type = new RDORTPResType( parser, res_type_name, true );

			// Создадим параметр перечислимого типа
			RDORTPEnum* state_enum = new RDORTPEnum( res_type, rtp_state_free );
			state_enum->add( rtp_state_buzy, @2 );

			RDORTPEnumDefVal* state_default = new RDORTPEnumDefVal( rtp_state_free );

			RDORTPEnumParamType* rtp_param_enum = new RDORTPEnumParamType( res_type, state_enum, state_default, RDOParserSrcInfo( @2 ) );
			rtp_param_enum->enum_name = rdo::format( "%s.%s", res_type_name.c_str(), rtp_param_name.c_str() );

			rtp_param = new RDORTPParam( res_type, rtp_param_name, rtp_param_enum, RDOParserSrcInfo( @2 ) );
			res_type->addParam( rtp_param );
		} else {
			// Тип найден, проверить на наличие перечислимого параметра
			rtp_param = const_cast<RDORTPParam*>(res_type->findRTPParam( rtp_param_name ));
			if ( rtp_param ) {
				// Параметр Состояние есть, надо проверить, чтобы в нем были значения Свободен и Занят
				// Для начала проверим тип параметра
				if ( rtp_param->getType()->getType() == RDORTPParamType::pt_enum ) {
					// Теперь проверим сами значения
					try {
						rtp_param->getType()->getRSSEnumValue( rtp_state_free );
						rtp_param->getType()->getRSSEnumValue( rtp_state_buzy );
					} catch ( RDOSyntaxException& ) {
						parser->error( rdo::format( "У типа ресурса '%s' перечислимый параметр '%s' должен иметь как минимум два обязательных значения: %s и %s", res_type->getName().c_str(), rtp_param_name.c_str(), rtp_state_free.c_str(), rtp_state_buzy.c_str() ) );
					}
				} else {
					// Параметр Состояние есть, но он не перечислимого типа
					parser->error( rdo::format( "У типа ресурса '%s' параметр '%s' не является перечислимым типом", res_type->getName().c_str(), rtp_param_name.c_str() ) );
				}
			} else {
				parser->error( rdo::format( "У типа ресурса '%s' нет параметра перечислимого типа '%s'", res_type->getName().c_str(), rtp_param_name.c_str() ) );
			}
		}

		// Создать ресурс
		RDORSSResource* res = new RDORSSResource( parser, res_name, res_type );
		res->setTrace( true );

		// Пропишем значения параметров перечислимого типа по-умолчанию
		rdoRuntime::RDOValue state_val = rtp_param->getType()->getRSSDefaultValue();
		res->addValue( state_val );
		res->currParam++;

		// Пропишем значения параметров (всех) для созданного ресурса. Берутся как значения по-умолчанию
//		const std::vector<const RDORTPParam *>& res_params = res->getType()->getParams();
//		res->currParam = res_params.begin();
//		while ( res->currParam != res_params.end() ) {
//			RDOValue res_param_val = (*res->currParam)->getType()->getRSSDefaultValue();
//			res->addValue( res_param_val );
//			res->currParam++;
//		}

	} else {
		// Ресурс найден, найдем для него тип
		RDORTPResType* res_type = const_cast<RDORTPResType*>(parser->findRTPResType( res->getType()->getName() ));
		if ( !res_type ) {
			// Тип не найден, выдай сообщение об ошибке
			parser->error( rdo::format( "Для ресурса %s не наден тип %s", res_name.c_str(), res_type->getName().c_str() ) );
		}
		RDORTPParam* rtp_param = const_cast<RDORTPParam*>(res_type->findRTPParam( rtp_param_name ));
		if ( !rtp_param ) {
			// Не найден перечислимый параметр Состояние, выдай сообщение об ошибке
			parser->error( rdo::format( "Для типа ресурса %s не наден параметр перечислимого типа %s", res_type->getName().c_str(), rtp_param_name.c_str() ) );
		}
		// Параметр Состояние есть, надо проверить, чтобы в нем были значения Свободен и Занят
		// Для начала проверим тип параметра
		if ( rtp_param->getType()->getType() == RDORTPParamType::pt_enum ) {
			// Теперь проверим сами значения
			try {
				rtp_param->getType()->getRSSEnumValue( rtp_state_free );
				rtp_param->getType()->getRSSEnumValue( rtp_state_buzy );
			} catch ( RDOSyntaxException& ) {
				parser->error( rdo::format( "У типа ресурса '%s' перечислимый параметр '%s' должен иметь как минимум два обязательных значения: %s и %s", res_type->getName().c_str(), rtp_param_name.c_str(), rtp_state_free.c_str(), rtp_state_buzy.c_str() ) );
			}
		} else {
			// Параметр Состояние есть, но он не перечислимого типа
			parser->error( rdo::format( "У типа ресурса '%s' параметр '%s' не является перечислимым типом", res_type->getName().c_str(), rtp_param_name.c_str() ) );
		}

	}

/*
	std::string* res_name      = (std::string*)$2;
	std::string* res_type_name = parser->registerName( std::string( RDOPROCProcess::name_prefix + *res_name + RDOPROCProcess::name_sufix ).c_str() );

	// Найти тип ресурса, если его нет, то создать
	RDORTPResType* res_type = const_cast<RDORTPResType*>(parser->findRTPResType( res_type_name ));
	if ( !res_type ) {
		res_type = new RDORTPResType( res_type_name, true, parser->resourceTypeCounter++ );
		parser->allRTPResType.push_back( res_type );
		parser->lastRTPResType = res_type;
	}

	// Найти параметр Состояние внутри типа, если его нет, то создать
	std::string* rtp_param_name = parser->registerName( "Состояние" );
	std::string* rtp_state_free = parser->registerName( "Свободен" );
	std::string* rtp_state_bizy = parser->registerName( "Занят" );
	bool param_added = false;
	RDORTPParam* rtp_param = const_cast<RDORTPParam*>(res_type->findRTPParam( rtp_param_name ));
	if ( !rtp_param ) {
		RDORTPEnum* state_enum = new RDORTPEnum( rtp_state_free );
		state_enum->add( rtp_state_bizy );

		RDORTPEnumDefVal* state_default = new RDORTPEnumDefVal( rtp_state_free );

		RDORTPEnumParamType* rtp_param_enum = new RDORTPEnumParamType( state_enum, state_default );

		rtp_param = new RDORTPParam( rtp_param_name, rtp_param_enum );
		parser->lastRTPResType->add( rtp_param );
		param_added = true;
	} else {
		// Параметр Состояние есть, надо проверить, чтобы в нем были значения Свободен и Занят
		// Для начала проверим тип параметра
		if ( rtp_param->getType()->getType() == RDORTPParamType::pt_enum ) {
			// Теперь проверим сами значения
			try {
				rtp_param->getType()->getRSSEnumValue( rtp_state_free );
				rtp_param->getType()->getRSSEnumValue( rtp_state_bizy );
			} catch ( RDOSyntaxException& ) {
				parser->error( std::string(_T("У типа ресурса \"") + *res_type_name + "\" перечислимый параметр \"" + *rtp_param_name + "\" должен иметь как минимум два обязательных значения: " + *rtp_state_free + " и " + *rtp_state_bizy ).c_str() );
			}
		} else {
			// Параметр Состояние есть, но он не перечислимого типа
			parser->error( std::string(_T("У типа ресурса \"") + *res_type_name + "\" параметр \"" + *rtp_param_name + "\" не является перечислимым типом" ).c_str() );
		}
	}

	// Найти ресурс, если его нет, то создать
	RDORSSResource* res = const_cast<RDORSSResource*>(parser->findRSSResource( res_name ));
	if ( !res ) {
		res = new RDORSSResource( res_name, res_type, parser->resourceCounter++ );
		res->setTrace( true );
		parser->lastRSSResource = res;
		parser->allRSSResource.push_back( res );

		// Пропишем значения параметров для созданного ресурса. Берутся как значения по-умолчанию
		const std::vector<const RDORTPParam *>& res_params = res->getType()->getParams();
		res->currParam = res_params.begin();
		while ( res->currParam != res_params.end() ) {
			RDOValue res_param_val = (*res->currParam)->getType()->getRSSDefaultValue();
			res->addValue( res_param_val );
			res->currParam++;
		}
	} else if ( param_added ) {
		RDOValue state_val = rtp_param->getType()->getRSSDefaultValue();
		res->addValue( state_val );
		res->currParam++;
	} else {
		TRACE( "q" );
	}
*/

};

dpt_process_end:	dpt_process End	{
					};

fun_arithm: fun_arithm '+' fun_arithm {
			}
			| fun_arithm '-' fun_arithm {
			}
			| fun_arithm '*' fun_arithm {
			}
			| fun_arithm '/' fun_arithm {
			}
			| '(' fun_arithm ')' {
			}
			| IDENTIF '(' fun_arithm_func_call_pars ')' {
			};
			| IDENTIF '.' IDENTIF {
			}
			| INT_CONST {
			}
			| REAL_CONST {
			}
			| IDENTIF {
			};

fun_arithm_func_call_pars:	/* empty */	{
			}
			| fun_arithm_func_call_pars fun_arithm		{
			}
			| fun_arithm_func_call_pars ',' fun_arithm	{
			};

%%

}

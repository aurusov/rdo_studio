#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdopat.h"
#include "rdoparser.h"
#include "rdofun.h"
#include "rdorss.h"
#include "rdortp.h"
#include "rdopatrtime.h"
#include "rdoruntime.h"
#include "rdoparser_lexer.h"

namespace rdoParse 
{

int patlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void paterror( char* mes )
{
}

RDOPATPattern::RDOPATPattern( const std::string* const _name, const bool _trace ):
	name( _name ),
	commonChoice( NULL ),
	patRuntime( NULL ),
	currRelRes( NULL ),
	trace( _trace ),
	useCommonChoice( false )
{
	if ( parser->findPattern( name ) ) {
		parser->error( "Pattern with name: " + *name + " already exist" );
	}
	parser->insertPATPattern( this );
}

	std::string RDOPATPattern::StatusToStr( RDOResourceTrace::ConvertStatus value )
{
	switch ( value ) {
		case RDOResourceTrace::CS_Keep    : return "Keep";
		case RDOResourceTrace::CS_Create  : return "Create";
		case RDOResourceTrace::CS_Erase   : return "Erase";
		case RDOResourceTrace::CS_NonExist: return "NonExist";
		case RDOResourceTrace::CS_NoChange: return "NoChange";
	}
	return ""; // unreachable code
}

RDOResourceTrace::ConvertStatus RDOPATPattern::StrToStatus( const std::string& value )
{
	if ( value == "Keep" ) {
		return RDOResourceTrace::CS_Keep;
	} else if ( value == "Create" ) {
		return RDOResourceTrace::CS_Create;
	} else if ( value == "Erase" ) {
		return RDOResourceTrace::CS_Erase;
	} else if ( value == "NonExist" ) {
		return RDOResourceTrace::CS_NonExist;
	} else if ( value == "NoChange" ) {
		return RDOResourceTrace::CS_NoChange;
	} else {
		parser->error( "Wrong converter status: " + value );
	}
	return RDOResourceTrace::CS_Keep; // unreachable code
}

void RDOPATPattern::rel_res_insert( RDORelevantResource* rel_res )
{
/*
	if ( res->isDirect() ) {
		const RDORSSResource* rss = static_cast<RDORelevantResourceDirect*>(res)->getResource();
		parser->tmp_rss.push_back( rss );
	} else {
		const RDORTPResType* rtp = res->getType();
		if ( rtp->isPermanent() ) {
			int rss_cnt = 0;
			std::vector< RDORSSResource* >::const_iterator it = parser->getRSSResources().begin();
			while ( it != parser->getRSSResources().end() ) {
				if ( (*it)->getType() == rtp ) rss_cnt++;
				it++;
			}
			int rel_cnt = 0;
			std::vector< RDORelevantResource* >::const_iterator it_rel = relRes.begin();
			while ( it_rel != relRes.end() ) {
				if ( (*it_rel)->getType() == rtp && !(*it_rel)->isDirect() ) rel_cnt++;
				it_rel++;
			}
			if ( rel_cnt >= rss_cnt ) {
				parser->error( rdo::format( "Не хватает свободных ресурсов типа: %s", rtp->getName()->c_str() ) );
			}
		}
	}
*/
	patRuntime->addBeginConvertStatus( rel_res->begin );
	relRes.push_back( rel_res );
}

void RDOPATPatternOperation::rel_res_insert( RDORelevantResource* rel_res )
{
	RDOPATPattern::rel_res_insert( rel_res );
	static_cast<rdoRuntime::RDOOperationRuntime*>(patRuntime)->addEndConvertStatus( rel_res->end );
}

void RDOPATPattern::testGoodForSearchActivity() const
{
	parser->error("Only RULEs can be used in search activity");
}

void RDOPATPattern::testGoodForSomeActivity() const
{
	parser->error("Only RULEs and OPERATIONs can be used in some activity");
}

void RDOPATPattern::testGoodForFreeActivity() const
{
	parser->error("Only IEs and KEYBOARD OPERATIONs can be used in free activity");
}

std::string RDOPATPattern::getPatternId() const
{ 
	return patRuntime->getPatternId(); 
}

int RDOPATPattern::writeModelStructure() const
{
	parser->modelStructure << getPatternId() << " " << *getName() << " " << getModelStructureLetter() << " " << relRes.size();
	for(int i = 0; i < relRes.size(); i++)
		parser->modelStructure << " " << relRes.at(i)->getType()->getNumber();

	parser->modelStructure << std::endl;
	return 0;
}

void RDOPATPatternRule::testGoodForSearchActivity() const
{
	for ( std::vector< RDORelevantResource* >::const_iterator i = rel_res_begin(); i != rel_res_end(); i++ ) {
		if ( ((*i)->begin == RDOResourceTrace::CS_Create) || ((*i)->begin == RDOResourceTrace::CS_Erase) ) {
			parser->error( "Rule: " + *getName() + " Cannot be used in search activity because of bad converter status" );
		}
	}
}

const RDOFUNFunctionParam *RDOPATPattern::findPATPatternParam(const std::string *const paramName) const
{
	std::vector<RDOFUNFunctionParam *>::const_iterator it = 
		std::find_if(params.begin(), params.end(), compareName<RDOFUNFunctionParam>(paramName));

	if(it == params.end())
		return NULL;

	return (*it);
}

const RDORelevantResource *RDOPATPattern::findRelevantResource(const std::string *const resName) const
{
	std::vector<RDORelevantResource *>::const_iterator it = 
		std::find_if(relRes.begin(), relRes.end(), compareName<RDORelevantResource>(resName));

	if(it == relRes.end())
		return NULL;

	return (*it);
}

int RDOPATPattern::findPATPatternParamNum(const std::string *const paramName)	const
{
	std::vector<RDOFUNFunctionParam *>::const_iterator it = 
		std::find_if(params.begin(), params.end(), compareName<RDOFUNFunctionParam>(paramName));

	if(it == params.end())
		return -1;

	return it - params.begin();
}

int RDOPATPattern::findRelevantResourceNum(const std::string *const resName) const
{
	std::vector<RDORelevantResource *>::const_iterator it = 
		std::find_if(relRes.begin(), relRes.end(), compareName<RDORelevantResource>(resName));

	if(it == relRes.end())
		return -1;

	return it - relRes.begin();
}

void RDOPATPattern::add(RDOFUNFunctionParam *const _param) 
{ 
	if(findPATPatternParam(_param->getName()))
		parser->error("Second appearance of the same parameter name: " + *(_param->getName()));

	params.push_back(_param); 
}

void RDOPATPattern::addRelRes( std::string* relName, std::string* resName, RDOResourceTrace::ConvertStatus beg, RDOResourceTrace::ConvertStatus end )
{
	parser->error( "Необходим только один конвертор" );
//	parser->error( "Needed 1 converter status for this pattern type" );
}

void RDOPATPattern::addRelRes( std::string* relName, std::string* resName, RDOResourceTrace::ConvertStatus beg )
{
	parser->error( "Ожидатеся статус конвертора конца" );
//	parser->error( "Needed 2 converter statuses for this pattern type" );
}

void RDOPATPattern::addRelRes( std::string* relName, std::string* resName, std::string* convBeg )
{
	parser->error( "Необходим только один конвертор" );
//	parser->error( "Needed 1 converter status for this pattern type" );
}

void RDOPATPattern::setRelResPos( std::string* relRes, YYLTYPE* pos )
{
	rel_res_pos[ *relRes ] = *pos;
	if ( pos->first_line == pos->last_line ) {
		rel_res_pos[ *relRes ].last_column = rel_res_pos[ *relRes ].first_column + relRes->length();
	}
}

void RDOPATPatternOperation::addRelRes( std::string* relName, std::string* resName, std::string* convBeg )
{
	addRelRes( relName, resName, RDOPATPattern::StrToStatus( *convBeg ), RDOResourceTrace::CS_NoChange );
}

void RDOPATPatternOperation::addRelRes( std::string* relName, std::string* resName, RDOResourceTrace::ConvertStatus beg, RDOResourceTrace::ConvertStatus end )
{
	switch ( beg ) {
		case RDOResourceTrace::CS_Keep:
			if ( end != RDOResourceTrace::CS_Keep && end != RDOResourceTrace::CS_Erase && end != RDOResourceTrace::CS_NoChange ) {
				parser->error( rdo::format("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()) );
//				parser->error( "Wrong second converter status" );
			}
			break;
		case RDOResourceTrace::CS_Create:
			if ( end != RDOResourceTrace::CS_Keep && end != RDOResourceTrace::CS_Erase && end != RDOResourceTrace::CS_NoChange ) {
				parser->error( rdo::format("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()) );
//				parser->error( "Wrong second converter status" );
			}
			break;
		case RDOResourceTrace::CS_Erase:
			if ( end != RDOResourceTrace::CS_NonExist ) {
				parser->error( rdo::format("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()) );
//				parser->error( "Wrong second converter status" );
			}
			break;
		case RDOResourceTrace::CS_NonExist:
			if ( end != RDOResourceTrace::CS_Create ) {
				parser->error( rdo::format("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()) );
//				parser->error( "Wrong second converter status" );
			}
			break;
		case RDOResourceTrace::CS_NoChange:
			if ( end != RDOResourceTrace::CS_Keep && end != RDOResourceTrace::CS_Erase && end != RDOResourceTrace::CS_NoChange ) {
				parser->error( rdo::format("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()) );
//				parser->error( "Wrong second converter status" );
			}
			break;
	};

	RDORelevantResource* relRes     = NULL;
	const RDORSSResource* const res = parser->findRSSResource( resName );
	if ( res ) {
		if ( beg == RDOResourceTrace::CS_Create ) {
			parser->error( rdo::format("Статус конвертора '%s' можно использовать, если описателем является тип ресурса, но не ресурс", RDOPATPattern::StatusToStr(beg).c_str()) );
//			parser->error( "Cannot use Create status for resource, only for resource type" );
		}
		if ( end == RDOResourceTrace::CS_Create ) {
			parser->error( rdo::format("Статус конвертора '%s' можно использовать, если описателем является тип ресурса, но не ресурс", RDOPATPattern::StatusToStr(end).c_str()) );
//			parser->error( "Cannot use Create status for resource, only for resource type" );
		}
		if ( res->getType()->isPermanent() ) {
//			if ( beg == RDOResourceTrace::CS_Create || beg == RDOResourceTrace::CS_Erase || beg == RDOResourceTrace::CS_NonExist )
//				parser->error( "Wrong first converter status for resource of permanent type" );
//			if ( end == RDOResourceTrace::CS_Create || end == RDOResourceTrace::CS_Erase || end == RDOResourceTrace::CS_NonExist )
//				parser->error( "Wrong second converter status for resource of permanent type" );
			if ( beg == RDOResourceTrace::CS_Create || beg == RDOResourceTrace::CS_Erase || beg == RDOResourceTrace::CS_NonExist ) {
				parser->error( rdo::format("Недопустимый статус конвертора начала для постоянного ресурса: %s", RDOPATPattern::StatusToStr(beg).c_str()) );
			}
			if ( end == RDOResourceTrace::CS_Create || end == RDOResourceTrace::CS_Erase || end == RDOResourceTrace::CS_NonExist ) {
				parser->error( rdo::format("Недопустимый статус конвертора конца для постоянного ресурса: %s", RDOPATPattern::StatusToStr(end).c_str()) );
			}
		}
		relRes = new RDORelevantResourceDirect( relName, rel_res_count(), res, beg, end );
		rel_res_insert( relRes );

	} else {
		const RDORTPResType* const type = parser->findRTPResType( resName );
		if ( !type ) {
			parser->error( rdo::format("Неизвестный тип ресурса или ресурс: %s", resName->c_str()) );
//			parser->error( "Unknown resource name or type: " + *resName );
		}
		if ( type->isPermanent() ) {
			if ( beg == RDOResourceTrace::CS_Create || beg == RDOResourceTrace::CS_Erase || beg == RDOResourceTrace::CS_NonExist ) {
				parser->error( rdo::format("Недопустимый статус конвертора начала для постоянного ресурса: %s", RDOPATPattern::StatusToStr(beg).c_str()) );
			}
			if ( end == RDOResourceTrace::CS_Create || end == RDOResourceTrace::CS_Erase || end == RDOResourceTrace::CS_NonExist ) {
				parser->error( rdo::format("Недопустимый статус конвертора конца для постоянного ресурса: %s", RDOPATPattern::StatusToStr(end).c_str()) );
			}
		}
		relRes = new RDORelevantResourceByType( relName, rel_res_count(), type, beg, end );
		rel_res_insert( relRes );
	}
	if ( relRes->begin == RDOResourceTrace::CS_Erase ) {
		patRuntime->addBeginEraseCalc( new rdoRuntime::RDOCalcEraseRes( relRes->rel_res_id, *relRes->getName() ) );
	}
	if ( relRes->end == RDOResourceTrace::CS_Erase ) {
		static_cast<rdoRuntime::RDOOperationRuntime*>(patRuntime)->addEndEraseCalc( new rdoRuntime::RDOCalcEraseRes( relRes->rel_res_id, *relRes->getName() ) );
	}
}

void RDOPATPatternRule::addRelRes( std::string* relName, std::string* resName, RDOResourceTrace::ConvertStatus beg )
{
	if ( beg == RDOResourceTrace::CS_NonExist ) {
		parser->error( rdo::format("Нельзя использовать статус конвертора '%s' в образце типа %s", RDOPATPattern::StatusToStr(beg).c_str(), getPatTypeStr().c_str()) );
//		parser->error( "Cannot use NonExist status in rule" );
	}

	RDORelevantResource* relRes     = NULL;
	const RDORSSResource* const res = parser->findRSSResource( resName );
	if ( res ) {
		if ( beg == RDOResourceTrace::CS_Create ) {
			parser->error( rdo::format("Статус конвертора '%s' можно использовать, если описателем является тип ресурса, но не ресурс", RDOPATPattern::StatusToStr(beg).c_str()) );
//			parser->error( "Cannot use Create status for resource, only for resource type" );
		}
		if ( res->getType()->isPermanent() ) {
			if ( beg == RDOResourceTrace::CS_Create || beg == RDOResourceTrace::CS_Erase || beg == RDOResourceTrace::CS_NonExist ) {
				parser->error( rdo::format("Недопустимый статус конвертора начала для постоянного ресурса: %s", RDOPATPattern::StatusToStr(beg).c_str()) );
//				parser->error( "Wrong converter status for resource of permanent type" );
			}
		}
		relRes = new RDORelevantResourceDirect( relName, rel_res_count(), res, beg );
		rel_res_insert( relRes );

	} else {
		const RDORTPResType* const type = parser->findRTPResType( resName );
		if ( !type ) {
			parser->error( rdo::format("Неизвестный тип ресурса или ресурс: %s", resName->c_str()) );
//			parser->error( "Unknown resource name or type: " + *resName );
		}
		relRes = new RDORelevantResourceByType( relName, rel_res_count(), type, beg );
		rel_res_insert( relRes );
	}
	if ( relRes->begin == RDOResourceTrace::CS_Erase ) {
		patRuntime->addBeginEraseCalc( new rdoRuntime::RDOCalcEraseRes( relRes->rel_res_id, *relRes->getName() ) );
	}
}

void RDOPATPatternEvent::addRelRes( std::string* relName, std::string* resName, RDOResourceTrace::ConvertStatus beg )
{
	if ( beg == RDOResourceTrace::CS_NonExist || beg == RDOResourceTrace::CS_NoChange ) {
		parser->error( rdo::format("Статусы конверторов NonExist и NoChange не могут быть использованы в обраце типа %s", getPatTypeStr().c_str()) );
	}

	RDORelevantResource* relRes     = NULL;
	const RDORSSResource* const res = parser->findRSSResource( resName );
	if ( res ) {
		if ( beg == RDOResourceTrace::CS_Create ) {
			parser->error( rdo::format("Статус конвертора '%s' можно использовать, если описателем является тип ресурса, но не ресурс", RDOPATPattern::StatusToStr(beg).c_str()) );
//			parser->error( "Cannot use Create status for resource, only for resource type" );
		}
		if ( res->getType()->isPermanent() ) {
			if ( beg == RDOResourceTrace::CS_Create || beg == RDOResourceTrace::CS_Erase || beg == RDOResourceTrace::CS_NonExist ) {
				parser->error( rdo::format("Недопустимый статус конвертора начала для постоянного ресурса: %s", RDOPATPattern::StatusToStr(beg).c_str()) );
//				parser->error( "Wrong converter status for resource of permanent type" );
			}
		}
		RDORelevantResourceDirect* rd = new RDORelevantResourceDirect( relName, rel_res_count(), res, beg );
		relRes                        = rd;
		rel_res_insert( relRes );
		patRuntime->addChoiceFromCalc( rd->createSelectEmptyResourceCalc() );

	} else {
		const RDORTPResType* const type = parser->findRTPResType( resName );
		if ( !type ) {
			parser->error( rdo::format("Неизвестный тип ресурса или ресурс: %s", resName->c_str()) );
//			parser->error( "Unknown resource name or type: " + *resName );
		}
		if ( beg == RDOResourceTrace::CS_Keep || beg == RDOResourceTrace::CS_Erase ) {
			parser->error( rdo::format("Статусы конверторов Keep и Erase могут быть использованы в обраце типа %s с описателем в виде ресурса, но не типа ресурса", getPatTypeStr().c_str()) );
//			parser->error( "Can use Keep and Erase status with resource name only (not with resource type) in irregular event" );
		}
		relRes = new RDORelevantResourceByType( relName, rel_res_count(), type, beg );
		rel_res_insert( relRes );
	}
	if ( relRes->begin == RDOResourceTrace::CS_Erase ) {
		patRuntime->addBeginEraseCalc( new rdoRuntime::RDOCalcEraseRes( relRes->rel_res_id, *relRes->getName() ) );
	}
}

void RDOPATPattern::setCommonChoiceFirst()
{
	useCommonChoice = true;
	commonChoice    = NULL;
}

void RDOPATPattern::setCommonChoiceWithMin( RDOFUNArithm* arithm )
{
	useCommonChoice  = true;
	useCommonWithMax = false;
	commonChoice     = arithm;
}

void RDOPATPattern::setCommonChoiceWithMax( RDOFUNArithm* arithm )
{
	useCommonChoice  = true;
	useCommonWithMax = true;
	commonChoice     = arithm;
}

void RDOPATPatternRule::setTime( RDOFUNArithm* arithm )
{ 
	parser->error( rdo::format("Поле $Time не используется в образце типа %s", getPatTypeStr().c_str()) );
//	parser->error( "Rule must have no $Time field" );
}

void RDOPATPattern::setTime( RDOFUNArithm* arithm )
{ 
	patRuntime->setTime( arithm->createCalc(NULL) );
//	time = arithm; 
}

void RDOPATPattern::addRelResBody( std::string* resName )
{ 
	std::vector< RDORelevantResource* >::const_iterator it = std::find_if( relRes.begin(), relRes.end(), compareName<RDORelevantResource>(resName) );
	if ( it == relRes.end() ) {
		parser->error( rdo::format("Неизвестный релевантный ресурс: %s", resName->c_str()) );
//		parser->error( "Name of relevant resource expected instead of: " + *resName );
	}
	currRelRes = (*it);
	if ( currRelRes->alreadyHaveConverter ) {
		parser->error( rdo::format("Релевантный ресурс уже используется: %s", resName->c_str()) );
//		parser->error( "\"" + *resName + "\" relevant resource has converter block already" );
	}
	currRelRes->alreadyHaveConverter = true;
}

void RDOPATPattern::addRelResUsage( RDOPATChoice* choice, RDOPATSelectType* order_sort )
{
	if ( useCommonChoice && order_sort->type != RDOPATSelectType::st_empty ) {
		parser->error( "Нельзя указать способ выбора релевантного ресурса, т.к. используется единый для всех релевантных ресурсов способ, указанный до ключевого слова $Body" );
//		parser->error( "Cannot use both common choice and choice for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"" );
	}

	if ( !useCommonChoice && order_sort->type == RDOPATSelectType::st_empty ) {
		if ( (currRelRes->begin != RDOResourceTrace::CS_Create) && (currRelRes->end != RDOResourceTrace::CS_Create) ) {
			order_sort->type = RDOPATSelectType::st_first;
		}
	}

	if ( (currRelRes->begin == RDOResourceTrace::CS_Create) || (currRelRes->end == RDOResourceTrace::CS_Create) ) {
		if ( choice->type != RDOPATChoice::ch_empty ) {
			parser->error( "Релевантный ресурс создается, для него нельзя использовать 'Choice'. Ожидается конвертор" );
//			parser->error( "Cannot use choice when create \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"" );
		}
		if ( order_sort->type != RDOPATSelectType::st_empty ) {
			parser->error( rdo::format("Релевантный ресурс создается, для него нельзя использовать способ выбора '%s'. Ожидается конвертор", order_sort->asString().c_str()) );
		}
	}

	currRelRes->choice     = choice;
	currRelRes->order_sort = order_sort;
}

void RDOPATPatternEvent::addRelResUsage( RDOPATChoice* choice, RDOPATSelectType* order_sort )
{
	if ( order_sort->type != RDOPATSelectType::st_empty ) {
		parser->error( rdo::format("Для релевантных ресурсов нерегулярного события нельзя использовать правило выбора '%s'", order_sort->asString().c_str()) );
	}
	if ( choice->type != RDOPATChoice::ch_empty ) {
		parser->error( "Релевантные ресурсы нерегулярного события нельзя выбирать с помощью Choice from или Choice NoCheck" );
//		parser->error( "Unexpected choice for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"" );
	}
	currRelRes->choice     = choice;
	currRelRes->order_sort = order_sort;
}

void RDOPATPattern::end()
{
	int size = relRes.size();
	for ( int i = 0; i < size; i++ ) {
		RDORelevantResource* currRelRes = relRes.at( i );
		if ( !currRelRes->alreadyHaveConverter ) {
			parser->lexer_loc_backup();
			parser->lexer_loc_set( &rel_res_pos[ *currRelRes->getName() ] );
			parser->error( rdo::format("Релевантный ресурс '%s' не используется в образце '%s'", currRelRes->getName()->c_str(), getName()->c_str()) );
			parser->lexer_loc_restore();
		}
//		patRuntime->addChoiceFromCalc( currRelRes->createSelectEmptyResourceCalc() );
	}

	if(useCommonChoice)
	{
		// first
		// Работает неправильно, а как обыкновенный first
		if(commonChoice == NULL)	// first
		{
//			int size = relRes.size();
			for(int i = 0; i < size; i++)
			{
				rdoRuntime::RDOCalc *calc = relRes.at(i)->createSelectFirstResourceChoiceCalc();
				patRuntime->addChoiceFromCalc(calc);
			}
			return;
		}

		// with_min / with_max
		std::vector< rdoRuntime::RDOSelectResourceCommon* > resSelectors;
		for ( int i = 0; i < size; i++ ) {
			if ( relRes.at(i)->begin == RDOResourceTrace::CS_Keep || relRes.at(i)->begin == RDOResourceTrace::CS_Erase || relRes.at(i)->begin == RDOResourceTrace::CS_NoChange ) {
				resSelectors.push_back( relRes.at(i)->createSelectResourceCommonChoiceCalc() );
			}
		}

		patRuntime->addChoiceFromCalc(new rdoRuntime::RDOSelectResourceCommonCalc(resSelectors, useCommonWithMax, commonChoice->createCalc()));
//		parser->error("RDOPATPattern::end not implemented yet for not \"first\" conditions in common choice");
	} else {
		for( int i = 0; i < size; i++ ) {
			rdoRuntime::RDOCalc *calc = relRes.at(i)->createSelectResourceChoiceCalc();
			patRuntime->addChoiceFromCalc(calc);
		}
	}
}

void RDOPATPattern::addRelResConvert()
{
	if ( currRelRes->begin != RDOResourceTrace::CS_NoChange && currRelRes->begin != RDOResourceTrace::CS_Erase && currRelRes->begin != RDOResourceTrace::CS_NonExist ) {
		parser->error( "Ожидается конвертор релевантного ресурса" );
//		parser->error( "Converter needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"" );
	}

	if ( currRelRes->end != RDOResourceTrace::CS_NoChange && currRelRes->end != RDOResourceTrace::CS_Erase && currRelRes->end != RDOResourceTrace::CS_NonExist ) {
		parser->error( "Converter end needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"" );
	}
}

RDOPATPatternOperation::RDOPATPatternOperation( std::string* _name, bool _trace ):
	RDOPATPattern( _name, _trace )
{ 
//	parser->runTime->addRuntimeOperation((RDOOperationRuntime *)(patRuntime = new RDOOperationRuntime(parser->runTime, _trace)));
	patRuntime = new rdoRuntime::RDOOperationRuntime( parser->runTime, _trace );
	patRuntime->setPatternId( parser->getPAT_id() );
}

RDOPATPatternOperation::RDOPATPatternOperation( bool _trace, std::string* _name ):
	RDOPATPattern( _name, _trace )
{
}

RDOPATPatternKeyboard::RDOPATPatternKeyboard( std::string* _name, bool _trace ):
	RDOPATPatternOperation( _trace, _name )
{
	patRuntime = new rdoRuntime::RDOKeyboardRuntime(parser->runTime, _trace); 
	patRuntime->setPatternId( parser->getPAT_id() );
}
 
RDOPATPatternEvent::RDOPATPatternEvent( std::string* _name, bool _trace ):
	RDOPATPattern( _name, _trace )
{ 
//	parser->runTime->addRuntimeIE((RDOIERuntime *)(patRuntime = new RDOIERuntime(parser->runTime, _trace))); 
	patRuntime = new rdoRuntime::RDOIERuntime( parser->runTime, _trace ); 
	patRuntime->setPatternId( parser->getPAT_id() );
}

RDOPATPatternRule::RDOPATPatternRule( std::string* _name, bool _trace ):
	RDOPATPattern( _name, _trace )
{ 
//	parser->runTime->addRuntimeRule((RDORuleRuntime *)(patRuntime = new RDORuleRuntime(parser->runTime, _trace))); 
	patRuntime = new rdoRuntime::RDORuleRuntime( parser->runTime, _trace );
	patRuntime->setPatternId( parser->getPAT_id() );
}

rdoRuntime::RDOCalc* RDORelevantResourceDirect::createSelectEmptyResourceCalc()
{
	return new rdoRuntime::RDOSelectResourceDirectCalc( rel_res_id, res->getNumber(), NULL, NULL );
}

rdoRuntime::RDOCalc *RDORelevantResourceDirect::createSelectFirstResourceChoiceCalc()
{
	return new rdoRuntime::RDOSelectResourceDirectCalc( rel_res_id, res->getNumber(), choice, NULL );
}

rdoRuntime::RDOCalc *RDORelevantResourceDirect::createSelectResourceChoiceCalc()
{
	return new rdoRuntime::RDOSelectResourceDirectCalc( rel_res_id, res->getNumber(), choice, order_sort );
}

rdoRuntime::RDOSelectResourceCommon *RDORelevantResourceDirect::createSelectResourceCommonChoiceCalc()
{
	return new rdoRuntime::RDOSelectResourceDirectCommonCalc( rel_res_id, res->getNumber(), choice, NULL );
}

rdoRuntime::RDOCalc* RDORelevantResourceByType::createSelectEmptyResourceCalc()
{
	if ( (begin != RDOResourceTrace::CS_Create) && (end != RDOResourceTrace::CS_Create) ) {
		return new rdoRuntime::RDOSelectResourceByTypeCalc( rel_res_id, type->getNumber(), NULL, NULL );
	} else {
		if ( begin == RDOResourceTrace::CS_NonExist ) {
			return new rdoRuntime::RDOSelectResourceNonExistCalc( rel_res_id );
		} else {
			return new rdoRuntime::RDOCalcConst(1);
		}
	}
}

rdoRuntime::RDOCalc *RDORelevantResourceByType::createSelectFirstResourceChoiceCalc()
{
	if ( (begin != RDOResourceTrace::CS_Create) && (end != RDOResourceTrace::CS_Create) ) {
		return new rdoRuntime::RDOSelectResourceByTypeCalc( rel_res_id, type->getNumber(), choice, NULL );
	} else {
		if ( begin == RDOResourceTrace::CS_NonExist ) {
			return new rdoRuntime::RDOSelectResourceNonExistCalc( rel_res_id );
		} else {
			return new rdoRuntime::RDOCalcConst(1);
		}
	}
}

rdoRuntime::RDOCalc *RDORelevantResourceByType::createSelectResourceChoiceCalc()
{
	if ( (begin != RDOResourceTrace::CS_Create) && (end != RDOResourceTrace::CS_Create) ) {
		return new rdoRuntime::RDOSelectResourceByTypeCalc( rel_res_id, type->getNumber(), choice, order_sort );
	} else {
		if ( begin == RDOResourceTrace::CS_NonExist ) {
			return new rdoRuntime::RDOSelectResourceNonExistCalc( rel_res_id );
		} else {
			return new rdoRuntime::RDOCalcConst(1);
		}
	}
}

rdoRuntime::RDOSelectResourceCommon *RDORelevantResourceByType::createSelectResourceCommonChoiceCalc()
{
	return new rdoRuntime::RDOSelectResourceByTypeCommonCalc( rel_res_id, type->getNumber(), choice, NULL );
}

void RDOPATParamsSet::checkParamsNumbers( RDORelevantResource* currRelRes )
{
	int size = paramArithms.size();
	for ( int i = 0; i < size; i++ ) {
		int parNumb = currRelRes->getType()->getRTPParamNumber( paramNames.at(i) );
		if ( parNumb == -1 ) {
			parser->error( rdo::format("Неизвестный параметр ресурса: %s", paramNames.at(i)->c_str()) );
//			parser->error("Wrong resource parameter name: " + *paramNames.at(i));
		}
		if ( std::find(paramNumbs.begin(), paramNumbs.end(), parNumb) != paramNumbs.end() ) {
			parser->error( rdo::format("Параметр ресурса уже используется: %s", paramNames.at(i)->c_str()) );
//			parser->error("Second appearence of the same resource parameter name: " + *paramNames.at(i));
		}
		paramNumbs.push_back(parNumb);
	}
}

void RDOPATParamsSet::checkParamsNumbers( std::string* paramName, const YYLTYPE& param_name_pos )
{
	if ( parser->getLastPATPattern() && parser->getLastPATPattern()->currRelRes ) {
		const RDORTPResType* res_type = parser->getLastPATPattern()->currRelRes->getType();
		const RDORTPParamDesc* param = res_type->findRTPParam( paramName );
		if ( !param ) {
			parser->lexer_loc_set( param_name_pos.last_line, param_name_pos.last_column );
			parser->error( rdo::format("Неизвестный параметр ресурса: %s", paramName->c_str()) );
		}
		std::vector< std::string* >::const_iterator it = paramNames.begin();
		while ( it != paramNames.end() ) {
			if ( **it == *paramName ) {
				parser->lexer_loc_set( param_name_pos.last_line, param_name_pos.last_column );
				parser->error( rdo::format("Параметр ресурса уже используется: %s", paramName->c_str()) );
			}
			it++;
		}
	} else {
		parser->error( "Внутренняя ошибка: не найден текущий реленвантный ресурс" );
	}
}

void RDOPATParamsSet::addIdentif( std::string* paramName, RDOFUNArithm* paramArithm, const YYLTYPE& param_name_pos )
{
	checkParamsNumbers( paramName, param_name_pos );
	const RDORTPResType* res_type = parser->getLastPATPattern()->currRelRes->getType();
	const RDORTPParamDesc* param = res_type->findRTPParam( paramName );
	param->getType()->checkParamType( paramArithm );
	paramNames.push_back( paramName );
	paramArithms.push_back( paramArithm );
}

void RDOPATParamsSet::addIdentif( std::string* paramName, const YYLTYPE& param_name_pos )
{
	checkParamsNumbers( paramName, param_name_pos );
	paramNames.push_back( paramName );
	paramArithms.push_back( NULL );
}

void RDOPATPatternOperation::addRelResConvertBegin( bool trace, RDOPATParamsSet* parSet )
{
	parSet->checkParamsNumbers(currRelRes);
	if ( currRelRes->begin == RDOResourceTrace::CS_NoChange || currRelRes->begin == RDOResourceTrace::CS_NonExist ) {
		parser->error( rdo::format("Не требуется конвертор начала для релевантного ресурса '%s', т.к. его статус: %s", currRelRes->getName()->c_str(), RDOPATPattern::StatusToStr(currRelRes->begin).c_str()) );
	}
//	if ( currRelRes->begin == RDOResourceTrace::CS_NoChange || currRelRes->begin == RDOResourceTrace::CS_Erase || currRelRes->begin == RDOResourceTrace::CS_NonExist )
//		parser->error("Converter begin not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->end != RDOResourceTrace::CS_NoChange && currRelRes->end != RDOResourceTrace::CS_Erase && currRelRes->end != RDOResourceTrace::CS_NonExist)
		parser->error("Converter end needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->begin == RDOResourceTrace::CS_Create)
	{
		if(currRelRes->getType()->getParams().size() != parSet->paramNumbs.size())
			parser->error("Must define all parameters when create new resource: \"" + *currRelRes->getName() + "\" in pattern \"" + *getName() + "\" in convert begin");

		rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOCalcCreateEmptyResource(currRelRes->getType()->getNumber(), trace, currRelRes->rel_res_id, currRelRes->getType()->getParams().size());
		patRuntime->addBeginCalc(calc);
	}

	int size = parSet->paramArithms.size();
	for(int i = 0; i < size; i++)
	{
		int parNumb = parSet->paramNumbs.at(i);
		RDOFUNArithm *currArithm = parSet->paramArithms.at(i);
		if(currArithm)	// if NULL - "NoChange" 
		{
			rdoRuntime::RDOCalc *rightValue = currArithm->createCalc(currRelRes->getType()->getParams().at(parNumb)->getType());
			rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOSetRelParamCalc(currRelRes->rel_res_id, parNumb, rightValue);
			patRuntime->addBeginCalc(calc);
		}
	}
}

void RDOPATPatternOperation::addRelResConvertEnd( bool trace, RDOPATParamsSet* parSet )
{
	parSet->checkParamsNumbers( currRelRes );
	if ( currRelRes->end == RDOResourceTrace::CS_NoChange || currRelRes->end == RDOResourceTrace::CS_NonExist || currRelRes->begin == RDOResourceTrace::CS_Erase ) {
		parser->error( rdo::format("Не требуется конвертор конца для релевантного ресурса '%s', т.к. его статус: %s", currRelRes->getName()->c_str(), RDOPATPattern::StatusToStr(currRelRes->end).c_str()) );
	}
//	if ( currRelRes->end == RDOResourceTrace::CS_NoChange || currRelRes->end == RDOResourceTrace::CS_Erase || currRelRes->end == RDOResourceTrace::CS_NonExist || currRelRes->begin == RDOResourceTrace::CS_Erase ) {
//		parser->error( "Converter end not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"" );

	if ( currRelRes->begin != RDOResourceTrace::CS_NoChange && currRelRes->begin != RDOResourceTrace::CS_Erase && currRelRes->begin != RDOResourceTrace::CS_NonExist ) {
		parser->error( rdo::format("Требуется конвертор начала для релевантного ресурса '%s', т.к. его статус: %s", currRelRes->getName()->c_str(), RDOPATPattern::StatusToStr(currRelRes->begin).c_str()) );
	}
//	if ( currRelRes->begin != RDOResourceTrace::CS_NoChange && currRelRes->begin != RDOResourceTrace::CS_Erase && currRelRes->begin != RDOResourceTrace::CS_NonExist )
//		parser->error("Converter begin needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->end == RDOResourceTrace::CS_Create)
	{
		if(currRelRes->getType()->getParams().size() != parSet->paramNumbs.size())
			parser->error("Must define all parameters when create new resource: \"" + *currRelRes->getName() + "\" in pattern \"" + *getName() + "\" in convert end");

		rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOCalcCreateEmptyResource(currRelRes->getType()->getNumber(), trace, currRelRes->rel_res_id, currRelRes->getType()->getParams().size());
		static_cast<rdoRuntime::RDOOperationRuntime*>(patRuntime)->addEndCalc( calc );
	}

	int size = parSet->paramArithms.size();
	for(int i = 0; i < size; i++)
	{
		int parNumb = parSet->paramNumbs.at(i);
		RDOFUNArithm *currArithm = parSet->paramArithms.at(i);
		if(currArithm)	// if NULL - "NoChange" 
		{
			rdoRuntime::RDOCalc *rightValue = currArithm->createCalc(currRelRes->getType()->getParams().at(parNumb)->getType());
			rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOSetRelParamCalc(currRelRes->rel_res_id, parNumb, rightValue);
			static_cast<rdoRuntime::RDOOperationRuntime*>(patRuntime)->addEndCalc(calc);
		}
	}
}

void RDOPATPatternOperation::addRelResConvertBeginEnd(bool trace, RDOPATParamsSet *parSet, bool trace2, RDOPATParamsSet *parSet2)
{
	parSet->checkParamsNumbers(currRelRes);
	parSet2->checkParamsNumbers(currRelRes);
	if ( currRelRes->begin == RDOResourceTrace::CS_NoChange || currRelRes->begin == RDOResourceTrace::CS_NonExist ) {
		parser->error( rdo::format("Не требуется конвертор начала для релевантного ресурса '%s', т.к. его статус: %s", currRelRes->getName()->c_str(), RDOPATPattern::StatusToStr(currRelRes->begin).c_str()) );
	}
//	if ( currRelRes->begin == RDOResourceTrace::CS_NoChange || currRelRes->begin == RDOResourceTrace::CS_Erase || currRelRes->begin == RDOResourceTrace::CS_NonExist )
//		parser->error("Converter begin not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if ( currRelRes->end == RDOResourceTrace::CS_NoChange || currRelRes->end == RDOResourceTrace::CS_NonExist ) {
		parser->error( rdo::format("Не требуется конвертор конца для релевантного ресурса '%s', т.к. его статус: %s", currRelRes->getName()->c_str(), RDOPATPattern::StatusToStr(currRelRes->end).c_str()) );
	}
//	if (currRelRes->end == RDOResourceTrace::CS_NoChange || currRelRes->end == RDOResourceTrace::CS_Erase || currRelRes->end == RDOResourceTrace::CS_NonExist )
//		parser->error("Converter end not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->begin == RDOResourceTrace::CS_Create)
	{
		if(currRelRes->getType()->getParams().size() != parSet->paramNumbs.size())
			parser->error("Must define all parameters when create new resource: \"" + *currRelRes->getName() + "\" in pattern \"" + *getName() + "\" in convert begin");

		rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOCalcCreateEmptyResource(currRelRes->getType()->getNumber(), trace, currRelRes->rel_res_id, currRelRes->getType()->getParams().size());
		patRuntime->addBeginCalc(calc);
	}

	int size = parSet->paramArithms.size();
	for(int i = 0; i < size; i++)
	{
		int parNumb = parSet->paramNumbs.at(i);
		RDOFUNArithm *currArithm = parSet->paramArithms.at(i);
		if(currArithm)	// if NULL - "NoChange" 
		{
			rdoRuntime::RDOCalc *rightValue = currArithm->createCalc(currRelRes->getType()->getParams().at(parNumb)->getType());
			rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOSetRelParamCalc(currRelRes->rel_res_id, parNumb, rightValue);
			patRuntime->addBeginCalc(calc);
		}
	}

	if(currRelRes->end == RDOResourceTrace::CS_Create)
	{
		if(currRelRes->getType()->getParams().size() != parSet2->paramNumbs.size())
			parser->error("Must define all parameters when create new resource: \"" + *currRelRes->getName() + "\" in pattern \"" + *getName() + "\" in convert end");

		rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOCalcCreateEmptyResource(currRelRes->getType()->getNumber(), trace2, currRelRes->rel_res_id, currRelRes->getType()->getParams().size());
		static_cast<rdoRuntime::RDOOperationRuntime*>(patRuntime)->addEndCalc(calc);
	}

	size = parSet2->paramArithms.size();
	for(i = 0; i < size; i++)
	{
		int parNumb = parSet2->paramNumbs.at(i);
		RDOFUNArithm *currArithm = parSet2->paramArithms.at(i);
		if(currArithm)	// if NULL - "NoChange" 
		{
			rdoRuntime::RDOCalc *rightValue = currArithm->createCalc(currRelRes->getType()->getParams().at(parNumb)->getType());
			rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOSetRelParamCalc(currRelRes->rel_res_id, parNumb, rightValue);
			static_cast<rdoRuntime::RDOOperationRuntime*>(patRuntime)->addEndCalc(calc);
		}
	}
}

void RDOPATPatternRule::addRelResConvertRule(bool trace, RDOPATParamsSet *parSet)
{
	parSet->checkParamsNumbers(currRelRes);
	if(currRelRes->begin == RDOResourceTrace::CS_NoChange || currRelRes->begin == RDOResourceTrace::CS_Erase || currRelRes->begin == RDOResourceTrace::CS_NonExist)
		parser->error("Converter not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->begin == RDOResourceTrace::CS_Create)
	{
		if(currRelRes->getType()->getParams().size() != parSet->paramNumbs.size())
			parser->error("Must define all parameters when create new resource: \"" + *currRelRes->getName() + "\" in pattern \"" + *getName() + "\" in converter");

		rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOCalcCreateEmptyResource(currRelRes->getType()->getNumber(), trace, currRelRes->rel_res_id, currRelRes->getType()->getParams().size());
		patRuntime->addBeginCalc(calc);
	}

	int size = parSet->paramArithms.size();
	for(int i = 0; i < size; i++)
	{
		int parNumb = parSet->paramNumbs.at(i);
		RDOFUNArithm *currArithm = parSet->paramArithms.at(i);
		if(currArithm)	// if NULL - "NoChange" 
		{
			rdoRuntime::RDOCalc *rightValue = currArithm->createCalc(currRelRes->getType()->getParams().at(parNumb)->getType());
			rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOSetRelParamCalc(currRelRes->rel_res_id, parNumb, rightValue);
			patRuntime->addBeginCalc(calc);
		}
	}
}

void RDOPATPatternEvent::addRelResConvertEvent(bool trace, RDOPATParamsSet *parSet)
{
	parSet->checkParamsNumbers(currRelRes);
	if(currRelRes->begin == RDOResourceTrace::CS_NoChange || currRelRes->begin == RDOResourceTrace::CS_Erase || currRelRes->begin == RDOResourceTrace::CS_NonExist)
		parser->error("Converter not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->begin == RDOResourceTrace::CS_Create)
	{
		if(currRelRes->getType()->getParams().size() != parSet->paramNumbs.size())
			parser->error("Must define all parameters when create new resource: \"" + *currRelRes->getName() + "\" in pattern \"" + *getName() + "\" in converter");

		rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOCalcCreateEmptyResource(currRelRes->getType()->getNumber(), trace, currRelRes->rel_res_id, currRelRes->getType()->getParams().size());
		patRuntime->addBeginCalc(calc);
	}

	int size = parSet->paramArithms.size();
	for(int i = 0; i < size; i++)
	{
		int parNumb = parSet->paramNumbs.at(i);
		RDOFUNArithm *currArithm = parSet->paramArithms.at(i);
		if(currArithm)	// if NULL - "NoChange" 
		{
			rdoRuntime::RDOCalc *rightValue = currArithm->createCalc(currRelRes->getType()->getParams().at(parNumb)->getType());
			rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOSetRelParamCalc(currRelRes->rel_res_id, parNumb, rightValue);
			patRuntime->addBeginCalc(calc);
		}
	}
}

void RDOPATPattern::addRelResConvertBegin(bool trace, RDOPATParamsSet *parSet)
{
	parser->error("Unexpected lexem ConvertBegin in pattern \"" + *getName() + "\"");
}

void RDOPATPattern::addRelResConvertEnd(bool trace, RDOPATParamsSet *parSet)
{
	parser->error("Unexpected lexem ConvertEnd in pattern \"" + *getName() + "\"");
}

void RDOPATPattern::addRelResConvertBeginEnd(bool trace, RDOPATParamsSet *parSet,bool trace2, RDOPATParamsSet *parSet2)
{
	parser->error("Unexpected lexem ConvertBegin in pattern \"" + *getName() + "\"");
}

void RDOPATPattern::addRelResConvertRule(bool trace, RDOPATParamsSet *parSet)
{
	parser->error("Unexpected lexem ConvertRule in pattern \"" + *getName() + "\"");
}

void RDOPATPattern::addRelResConvertEvent(bool trace, RDOPATParamsSet *parSet)
{
	parser->error("Unexpected lexem ConvertEvent in pattern \"" + *getName() + "\"");
}

const RDORTPResType *const RDORelevantResourceDirect::getType() const 
{ 
	return res->getType(); 
}

}		// namespace rdoParse 

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

std::string RDOPATPattern::StatusToStr( ConvertStatus value )
{
	switch ( value ) {
		case CS_Keep    : return "Keep";
		case CS_Create  : return "Create";
		case CS_Erase   : return "Erase";
		case CS_NonExist: return "NonExist";
		case CS_NoChange: return "NoChange";
	}
	return ""; // unreachable code
}

RDOPATPattern::ConvertStatus RDOPATPattern::StrToStatus( const std::string& value )
{
	if ( value == "Keep" ) {
		return CS_Keep;
	} else if ( value == "Create" ) {
		return CS_Create;
	} else if ( value == "Erase" ) {
		return CS_Erase;
	} else if ( value == "NonExist" ) {
		return CS_NonExist;
	} else if ( value == "NoChange" ) {
		return CS_NoChange;
	} else {
		parser->error( "Wrong converter status: " + value );
	}
	return CS_Keep; // unreachable code
}

void RDOPATPattern::res_insert( RDORelevantResource* res )
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
	relRes.push_back( res );
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
	for ( std::vector< RDORelevantResource* >::const_iterator i = res_begin(); i != res_end(); i++ ) {
		if ( ((*i)->begin == CS_Create) || ((*i)->begin == CS_Erase) ) {
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

void RDOPATPattern::addRelRes( std::string* relName, std::string* res, ConvertStatus beg, ConvertStatus end )
{
	parser->error( "Необходим только один конвертор" );
//	parser->error( "Needed 1 converter status for this pattern type" );
}

void RDOPATPattern::addRelRes( std::string* relName, std::string* res, ConvertStatus beg)
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
	addRelRes( relName, resName, RDOPATPattern::StrToStatus( *convBeg ), CS_NoChange );
}

void RDOPATPatternOperation::addRelRes( std::string* relName, std::string* resName, ConvertStatus beg, ConvertStatus end )
{
	switch ( beg ) {
	case CS_Keep:
		if ( end != CS_Keep && end != CS_Erase && end != CS_NoChange )
			parser->error( "Wrong second converter status" );
		break;
	case CS_Create:
		if ( end != CS_Keep && end != CS_Erase && end != CS_NoChange )
			parser->error( "Wrong second converter status" );
		break;
	case CS_Erase:
		if ( end != CS_NonExist )
			parser->error( "Wrong second converter status" );
		break;
	case CS_NonExist:
		if ( end != CS_Create )
			parser->error( "Wrong second converter status" );
		break;
	case CS_NoChange:
		if ( end != CS_Keep && end != CS_Erase && end != CS_NoChange )
			parser->error( "Wrong second converter status" );
		break;
	};

	const RDORSSResource* const res = parser->findRSSResource( resName );

	if ( res ) {
		if ( beg == CS_Create ) {
			parser->error("Cannot use Create status for resource, only for resource type");
		}
		if ( end == CS_Create ) {
			parser->error("Cannot use Create status for resource, only for resource type");
		}
		if ( res->getType()->isPermanent() ) {
//			if ( beg == CS_Create || beg == CS_Erase || beg == CS_NonExist )
//				parser->error("Wrong first converter status for resource of permanent type");
//			if ( end == CS_Create || end == CS_Erase || end == CS_NonExist )
//				parser->error("Wrong second converter status for resource of permanent type");
			if ( beg == CS_Create || beg == CS_Erase || beg == CS_NonExist ) {
				parser->error( rdo::format("Недопустимый статус конвертора начала для постоянного ресурса: %s", RDOPATPattern::StatusToStr(beg).c_str()) );
			}
			if ( end == CS_Create || end == CS_Erase || end == CS_NonExist ) {
				parser->error( rdo::format("Недопустимый статус конвертора конца для постоянного ресурса: %s", RDOPATPattern::StatusToStr(end).c_str()) );
			}
		}
		res_insert( new RDORelevantResourceDirect(relName, res_count(), res, beg, end) );

	} else {
		const RDORTPResType* const type = parser->findRTPResType( resName );
		if ( !type ) {
			parser->error( "Unknown resource name or type: " + *resName );
		}
		if ( type->isPermanent() ) {
			if ( beg == CS_Create || beg == CS_Erase || beg == CS_NonExist ) {
				parser->error( rdo::format("Недопустимый статус конвертора начала для постоянного ресурса: %s", RDOPATPattern::StatusToStr(beg).c_str()) );
			}
			if ( end == CS_Create || end == CS_Erase || end == CS_NonExist ) {
				parser->error( rdo::format("Недопустимый статус конвертора конца для постоянного ресурса: %s", RDOPATPattern::StatusToStr(end).c_str()) );
			}
		}
		res_insert( new RDORelevantResourceByType( relName, res_count(), type, beg, end ) );
	}
}

void RDOPATPatternRule::addRelRes(std::string *relName, std::string *resName, ConvertStatus beg)
{
	if(beg == CS_NonExist)
		parser->error("Cannot use NonExist status in rule");

	const RDORSSResource *const res = parser->findRSSResource(resName);
	if(res)
	{
		if(beg == CS_Create)
			parser->error("Cannot use Create status for resource, only for resource type");

		if(res->getType()->isPermanent())
		{
			if(beg == CS_Create || beg == CS_Erase || beg == CS_NonExist)
				parser->error("Wrong converter status for resource of permanent type");
		}

		res_insert(new RDORelevantResourceDirect(relName, res_count(), res, beg));
		return;
	}

	const RDORTPResType *const type = parser->findRTPResType(resName);
	if(!type)
		parser->error("Unknown resource name or type: " + *resName);

	res_insert(new RDORelevantResourceByType(relName, res_count(), type, beg));
}

void RDOPATPatternEvent::addRelRes(std::string *relName, std::string *resName, ConvertStatus beg)
{
	const RDORSSResource *const res = parser->findRSSResource(resName);
	if(res)
	{
		if(beg == CS_Create)
			parser->error("Cannot use Create status for resource, only for resource type");

		if(res->getType()->isPermanent())
		{
			if(beg == CS_Create || beg == CS_Erase || beg == CS_NonExist)
				parser->error("Wrong converter status for resource of permanent type");
		}

		RDORelevantResourceDirect *rd = new RDORelevantResourceDirect(relName, res_count(), res, beg);
		res_insert( rd );
		patRuntime->addChoiceFromCalc( rd->createSelectEmptyResourceCalc() );

		return;
	}

	const RDORTPResType *const type = parser->findRTPResType(resName);
	if(!type)
		parser->error("Unknown resource name or type: " + *resName);

	if(beg == CS_Keep || beg == CS_Erase)
		parser->error("Can use Keep and Erase status with resource name only (not with resource type) in irregular event");

	RDORelevantResourceByType *rt = new RDORelevantResourceByType(relName, res_count(), type, beg);
	res_insert( rt );
}

void RDOPATPattern::setCommonChoiceFirst() {	useCommonChoice = true; commonChoice = NULL; }
void RDOPATPattern::setCommonChoiceWithMin(RDOFUNArithm *arithm) { useCommonChoice = true; useCommonWithMax = false; commonChoice = arithm; }
void RDOPATPattern::setCommonChoiceWithMax(RDOFUNArithm *arithm) { useCommonChoice = true; useCommonWithMax = true; commonChoice = arithm; }
void RDOPATPatternRule::setTime(RDOFUNArithm *arithm) 
{ 
	parser->error("Rule must have no $Time field"); 
}

void RDOPATPattern::setTime(RDOFUNArithm *arithm) 
{ 
	patRuntime->setTime(arithm->createCalc(NULL));
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

void RDOPATPattern::addRelResUsage( RDOPATChoice* choice, RDOPATSelectType* first )
{
	if ( useCommonChoice && first->type != RDOPATSelectType::st_empty ) {
		parser->error( "Нельзя указать способ выбора релевантного ресурса, т.к. используется единый для всех релевантных ресурсов способ, указанный до ключевого слова $Body" );
//		parser->error( "Cannot use both common choice and choice for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"" );
	}

	if ( !useCommonChoice && first->type == RDOPATSelectType::st_empty ) {
		if ( (currRelRes->begin != CS_Create) && (currRelRes->end != CS_Create) ) {
			parser->error( "Ожидается способ выбора релевантного ресурса" );
//			parser->error( "Must use either common choice either choice for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"" );
		}
	}

	if ( first->type != RDOPATSelectType::st_empty || choice->type != RDOPATChoice::ch_empty ) {
		if ( (currRelRes->begin == CS_Create) || (currRelRes->end == CS_Create) ) {
			parser->error( "Релевантный ресурс создается, для него нельзя использовать 'Choice' и способ выбора. Ожидается конвертор" );
//			parser->error( "Cannot use choice when create \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"" );
		}
	}

	currRelRes->choice = choice;
	currRelRes->first  = first;
}

void RDOPATPatternEvent::addRelResUsage( RDOPATChoice* choice, RDOPATSelectType* first )
{
	if ( first->type != RDOPATSelectType::st_empty || choice->type != RDOPATChoice::ch_empty ) {
		parser->error("Unexpected choice for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");
	}
	currRelRes->choice = choice;
	currRelRes->first  = first;
}

void RDOPATPattern::end()
{
	int size = relRes.size();
	for ( int i = 0; i < size; i++ ) {
		RDORelevantResource* currRelRes = relRes.at( i );
//		if ( !currRelRes->choice ) {
		if ( !currRelRes->alreadyHaveConverter ) {
			parser->lexer_loc_backup();
			parser->lexer_loc_set( &rel_res_pos[ *currRelRes->getName() ] );
			parser->warning( rdo::format("Релевантный ресурс '%s' не используется в образце '%s'", currRelRes->getName()->c_str(), getName()->c_str()) );
			parser->lexer_loc_restore();
		}
		patRuntime->addChoiceFromCalc( currRelRes->createSelectEmptyResourceCalc() );
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
			if ( relRes.at(i)->begin == CS_Keep || relRes.at(i)->begin == CS_Erase || relRes.at(i)->begin == CS_NoChange ) {
				resSelectors.push_back( relRes.at(i)->createSelectResourceCommonChoiceCalc() );
			}
		}

		patRuntime->addChoiceFromCalc(new rdoRuntime::RDOSelectResourceCommonCalc(resSelectors, useCommonWithMax, commonChoice->createCalc()));
//		parser->error("RDOPATPattern::end not implemented yet for not \"first\" conditions in common choice");
	}
	else
	{
//		int size = relRes.size();
		for(int i = 0; i < size; i++)
		{
			rdoRuntime::RDOCalc *calc = relRes.at(i)->createSelectResourceChoiceCalc();
			patRuntime->addChoiceFromCalc(calc);
		}
		return;
	}
}

void RDOPATPattern::addRelResConvert()
{
	if ( currRelRes->begin != CS_NoChange && currRelRes->begin != CS_Erase && currRelRes->begin != CS_NonExist ) {
		parser->error( "Ожидается конвертор релевантного ресурса" );
//		parser->error( "Converter needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"" );
	}

	if ( currRelRes->end != CS_NoChange && currRelRes->end != CS_Erase && currRelRes->end != CS_NonExist ) {
		parser->error( "Converter end needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"" );
	}

	if(currRelRes->begin == CS_Erase)
		patRuntime->addBeginCalc(new rdoRuntime::RDOCalcEraseRes(currRelRes->numberOfResource));

	if(currRelRes->end == CS_Erase)
		patRuntime->addEndCalc(new rdoRuntime::RDOCalcEraseRes(currRelRes->numberOfResource));
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

rdoRuntime::RDOCalc *RDORelevantResourceDirect::createSelectEmptyResourceCalc()
{
	return new rdoRuntime::RDOSelectResourceDirectCalc( numberOfResource, res->getNumber(), NULL, NULL );
}

rdoRuntime::RDOCalc *RDORelevantResourceDirect::createSelectFirstResourceChoiceCalc()
{
	return new rdoRuntime::RDOSelectResourceDirectCalc( numberOfResource, res->getNumber(), choice, NULL );
}

rdoRuntime::RDOCalc *RDORelevantResourceDirect::createSelectResourceChoiceCalc()
{
	return new rdoRuntime::RDOSelectResourceDirectCalc( numberOfResource, res->getNumber(), choice, first );
}

rdoRuntime::RDOSelectResourceCommon *RDORelevantResourceDirect::createSelectResourceCommonChoiceCalc()
{
	return new rdoRuntime::RDOSelectResourceDirectCommonCalc( numberOfResource, res->getNumber(), choice, NULL );
}

rdoRuntime::RDOCalc *RDORelevantResourceByType::createSelectEmptyResourceCalc()
{
	if ( (begin != RDOPATPattern::CS_Create) && (end != RDOPATPattern::CS_Create) ) {
		return new rdoRuntime::RDOSelectResourceByTypeCalc( numberOfResource, type->getNumber(), NULL, NULL );
	} else {
		return new rdoRuntime::RDOCalcConst(1);
	}
}

rdoRuntime::RDOCalc *RDORelevantResourceByType::createSelectFirstResourceChoiceCalc()
{
	if ( (begin != RDOPATPattern::CS_Create) && (end != RDOPATPattern::CS_Create) ) {
		return new rdoRuntime::RDOSelectResourceByTypeCalc( numberOfResource, type->getNumber(), choice, NULL );
	} else {
		return new rdoRuntime::RDOCalcConst(1);
	}
}

rdoRuntime::RDOCalc *RDORelevantResourceByType::createSelectResourceChoiceCalc()
{
	if ( (begin != RDOPATPattern::CS_Create) && (end != RDOPATPattern::CS_Create) ) {
		return new rdoRuntime::RDOSelectResourceByTypeCalc( numberOfResource, type->getNumber(), choice, first );
	} else {
		return new rdoRuntime::RDOCalcConst(1);
	}
}

rdoRuntime::RDOSelectResourceCommon *RDORelevantResourceByType::createSelectResourceCommonChoiceCalc()
{
	return new rdoRuntime::RDOSelectResourceByTypeCommonCalc( numberOfResource, type->getNumber(), choice, NULL );
}

void RDOPATParamsSet::checkParamsNumbers(RDORelevantResource *currRelRes)
{
	int size = paramArithms.size();
	for(int i = 0; i < size; i++)
	{
		int parNumb = currRelRes->getType()->getRTPParamNumber(paramNames.at(i));
		if(parNumb == -1)
			parser->error("Wrong resource parameter name: " + *paramNames.at(i));

		if(std::find(paramNumbs.begin(), paramNumbs.end(), parNumb) != paramNumbs.end())
			parser->error("Second appearence of the same resource parameter name: " + *paramNames.at(i));

		paramNumbs.push_back(parNumb);
	}
}

void RDOPATParamsSet::addIdentif(std::string *paramName, RDOFUNArithm *paramArithm)
{
	paramNames.push_back(paramName);
	paramArithms.push_back(paramArithm);
}

void RDOPATParamsSet::addIdentif(std::string *paramName)
{
	paramNames.push_back(paramName);
	paramArithms.push_back(NULL);
}

void RDOPATPatternOperation::addRelResConvertBegin(bool trace, RDOPATParamsSet *parSet)
{
	parSet->checkParamsNumbers(currRelRes);
	if(currRelRes->begin == CS_NoChange || currRelRes->begin == CS_Erase || currRelRes->begin == CS_NonExist)
		parser->error("Converter Begin not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->end != CS_NoChange && currRelRes->end != CS_Erase && currRelRes->end != CS_NonExist)
		parser->error("Converter end needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->end == CS_Erase)
		patRuntime->addEndCalc(new rdoRuntime::RDOCalcEraseRes(currRelRes->numberOfResource));

	if(currRelRes->begin == CS_Create)
	{
		if(currRelRes->getType()->getParams().size() != parSet->paramNumbs.size())
			parser->error("Must define all parameters when create new resource: \"" + *currRelRes->getName() + "\" in pattern \"" + *getName() + "\" in convert begin");

		rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOCalcCreateEmptyResource(currRelRes->getType()->getNumber(), trace, currRelRes->numberOfResource, currRelRes->getType()->getParams().size());
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
			rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOSetRelParamCalc(currRelRes->numberOfResource, parNumb, rightValue);
			patRuntime->addBeginCalc(calc);
		}
	}
}

void RDOPATPatternOperation::addRelResConvertEnd(bool trace, RDOPATParamsSet *parSet)
{
	parSet->checkParamsNumbers(currRelRes);
	if(currRelRes->end == CS_NoChange || currRelRes->end == CS_Erase || currRelRes->end == CS_NonExist || currRelRes->begin == CS_Erase)
		parser->error("Converter end not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->begin != CS_NoChange && currRelRes->begin != CS_Erase && currRelRes->begin != CS_NonExist)
		parser->error("Converter begin needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->end == CS_Create)
	{
		if(currRelRes->getType()->getParams().size() != parSet->paramNumbs.size())
			parser->error("Must define all parameters when create new resource: \"" + *currRelRes->getName() + "\" in pattern \"" + *getName() + "\" in convert end");

		rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOCalcCreateEmptyResource(currRelRes->getType()->getNumber(), trace, currRelRes->numberOfResource, currRelRes->getType()->getParams().size());
		patRuntime->addEndCalc(calc);
	}

	int size = parSet->paramArithms.size();
	for(int i = 0; i < size; i++)
	{
		int parNumb = parSet->paramNumbs.at(i);
		RDOFUNArithm *currArithm = parSet->paramArithms.at(i);
		if(currArithm)	// if NULL - "NoChange" 
		{
			rdoRuntime::RDOCalc *rightValue = currArithm->createCalc(currRelRes->getType()->getParams().at(parNumb)->getType());
			rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOSetRelParamCalc(currRelRes->numberOfResource, parNumb, rightValue);
			patRuntime->addEndCalc(calc);
		}
	}
}

void RDOPATPatternOperation::addRelResConvertBeginEnd(bool trace, RDOPATParamsSet *parSet, bool trace2, RDOPATParamsSet *parSet2)
{
	parSet->checkParamsNumbers(currRelRes);
	parSet2->checkParamsNumbers(currRelRes);
	if(currRelRes->begin == CS_NoChange || currRelRes->begin == CS_Erase || currRelRes->begin == CS_NonExist)
		parser->error("Converter begin not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->end == CS_NoChange || currRelRes->end == CS_Erase || currRelRes->end == CS_NonExist)
		parser->error("Converter end not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->begin == CS_Create)
	{
		if(currRelRes->getType()->getParams().size() != parSet->paramNumbs.size())
			parser->error("Must define all parameters when create new resource: \"" + *currRelRes->getName() + "\" in pattern \"" + *getName() + "\" in convert begin");

		rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOCalcCreateEmptyResource(currRelRes->getType()->getNumber(), trace, currRelRes->numberOfResource, currRelRes->getType()->getParams().size());
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
			rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOSetRelParamCalc(currRelRes->numberOfResource, parNumb, rightValue);
			patRuntime->addBeginCalc(calc);
		}
	}

	if(currRelRes->end == CS_Create)
	{
		if(currRelRes->getType()->getParams().size() != parSet2->paramNumbs.size())
			parser->error("Must define all parameters when create new resource: \"" + *currRelRes->getName() + "\" in pattern \"" + *getName() + "\" in convert end");

		rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOCalcCreateEmptyResource(currRelRes->getType()->getNumber(), trace2, currRelRes->numberOfResource, currRelRes->getType()->getParams().size());
		patRuntime->addEndCalc(calc);
	}

	size = parSet2->paramArithms.size();
	for(i = 0; i < size; i++)
	{
		int parNumb = parSet2->paramNumbs.at(i);
		RDOFUNArithm *currArithm = parSet2->paramArithms.at(i);
		if(currArithm)	// if NULL - "NoChange" 
		{
			rdoRuntime::RDOCalc *rightValue = currArithm->createCalc(currRelRes->getType()->getParams().at(parNumb)->getType());
			rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOSetRelParamCalc(currRelRes->numberOfResource, parNumb, rightValue);
			patRuntime->addEndCalc(calc);
		}
	}
}

void RDOPATPatternRule::addRelResConvertRule(bool trace, RDOPATParamsSet *parSet)
{
	parSet->checkParamsNumbers(currRelRes);
	if(currRelRes->begin == CS_NoChange || currRelRes->begin == CS_Erase || currRelRes->begin == CS_NonExist)
		parser->error("Converter not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->begin == CS_Create)
	{
		if(currRelRes->getType()->getParams().size() != parSet->paramNumbs.size())
			parser->error("Must define all parameters when create new resource: \"" + *currRelRes->getName() + "\" in pattern \"" + *getName() + "\" in converter");

		rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOCalcCreateEmptyResource(currRelRes->getType()->getNumber(), trace, currRelRes->numberOfResource, currRelRes->getType()->getParams().size());
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
			rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOSetRelParamCalc(currRelRes->numberOfResource, parNumb, rightValue);
			patRuntime->addBeginCalc(calc);
		}
	}
}

void RDOPATPatternEvent::addRelResConvertEvent(bool trace, RDOPATParamsSet *parSet)
{
	parSet->checkParamsNumbers(currRelRes);
	if(currRelRes->begin == CS_NoChange || currRelRes->begin == CS_Erase || currRelRes->begin == CS_NonExist)
		parser->error("Converter not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->begin == CS_Create)
	{
		if(currRelRes->getType()->getParams().size() != parSet->paramNumbs.size())
			parser->error("Must define all parameters when create new resource: \"" + *currRelRes->getName() + "\" in pattern \"" + *getName() + "\" in converter");

		rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOCalcCreateEmptyResource(currRelRes->getType()->getNumber(), trace, currRelRes->numberOfResource, currRelRes->getType()->getParams().size());
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
			rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOSetRelParamCalc(currRelRes->numberOfResource, parNumb, rightValue);
			patRuntime->addBeginCalc(calc);
		}
	}
}

void RDOPATPattern::addRelResConvertBegin(bool trace, RDOPATParamsSet *parSet)
{
	parser->error("Unexoected lexem ConvertBegin in pattern \"" + *getName() + "\"");
}

void RDOPATPattern::addRelResConvertEnd(bool trace, RDOPATParamsSet *parSet)
{
	parser->error("Unexoected lexem ConvertEnd in pattern \"" + *getName() + "\"");
}

void RDOPATPattern::addRelResConvertBeginEnd(bool trace, RDOPATParamsSet *parSet,bool trace2, RDOPATParamsSet *parSet2)
{
	parser->error("Unexoected lexem ConvertBegin in pattern \"" + *getName() + "\"");
}

void RDOPATPattern::addRelResConvertRule(bool trace, RDOPATParamsSet *parSet)
{
	parser->error("Unexoected lexem ConvertRule in pattern \"" + *getName() + "\"");
}

void RDOPATPattern::addRelResConvertEvent(bool trace, RDOPATParamsSet *parSet)
{
	parser->error("Unexoected lexem ConvertEvent in pattern \"" + *getName() + "\"");
}

const RDORTPResType *const RDORelevantResourceDirect::getType() const 
{ 
	return res->getType(); 
}

}		// namespace rdoParse 

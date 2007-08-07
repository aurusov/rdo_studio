#include "pch.h"
#include "rdopat.h"
#include "rdoparser.h"
#include "rdofun.h"
#include "rdorss.h"
#include "rdortp.h"
#include "rdopatrtime.h"
#include "rdoparser_lexer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

// ----------------------------------------------------------------------------
// ---------- RDOPATPattern
// ----------------------------------------------------------------------------
RDOPATPattern::RDOPATPattern( RDOParser* _parser, const std::string* const _name, const bool _trace ):
	RDOParserObject( _parser ),
	name( *_name ),
	commonChoice( NULL ),
	patRuntime( NULL ),
	currRelRes( NULL ),
	current_rel_res_index( 0 ),
	trace( _trace ),
	useCommonChoice( false )
{
	if ( parser->findPattern( name ) ) {
		parser->error( "Pattern with name: " + name + " already exist" );
	}
	parser->insertPATPattern( this );
}

std::string RDOPATPattern::StatusToStr( rdoRuntime::RDOResourceTrace::ConvertStatus value )
{
	switch ( value ) {
		case rdoRuntime::RDOResourceTrace::CS_Keep    : return "Keep";
		case rdoRuntime::RDOResourceTrace::CS_Create  : return "Create";
		case rdoRuntime::RDOResourceTrace::CS_Erase   : return "Erase";
		case rdoRuntime::RDOResourceTrace::CS_NonExist: return "NonExist";
		case rdoRuntime::RDOResourceTrace::CS_NoChange: return "NoChange";
	}
	return ""; // unreachable code
}

rdoRuntime::RDOResourceTrace::ConvertStatus RDOPATPattern::StrToStatus( const std::string& value, const YYLTYPE& convertor_pos )
{
	if ( value == "Keep" || value == "keep" ) {
		return rdoRuntime::RDOResourceTrace::CS_Keep;
	} else if ( value == "Create" || value == "create" ) {
		return rdoRuntime::RDOResourceTrace::CS_Create;
	} else if ( value == "Erase" || value == "erase" ) {
		return rdoRuntime::RDOResourceTrace::CS_Erase;
	} else if ( value == "NonExist" || value == "nonexist" ) {
		return rdoRuntime::RDOResourceTrace::CS_NonExist;
	} else if ( value == "NoChange" || value == "nochange" ) {
		return rdoRuntime::RDOResourceTrace::CS_NoChange;
	} else {
		parser->lexer_loc_set( convertor_pos.last_line, RDOParserSrcInfo::getPosByLength(convertor_pos.first_column, value) );
		parser->error( rdo::format("Неверный статус конвертора: %s", value.c_str()) );
//		parser->error( "Wrong converter status: " + value );
	}
	return rdoRuntime::RDOResourceTrace::CS_Keep; // unreachable code
}

void RDOPATPattern::beforeRelRensert( const std::string& rel_res_name, const YYLTYPE& pos )
{
	if ( findRelevantResource( rel_res_name ) ) {
		parser->lexer_loc_set( pos.last_line, RDOParserSrcInfo::getPosByLength(pos.first_column, rel_res_name) );
		parser->error( rdo::format("Релевантный ресурс '%s' уже определен", rel_res_name.c_str()) );
	}
}

void RDOPATPattern::rel_res_insert( RDORelevantResource* rel_res, const YYLTYPE& pos )
{
	rel_res->setSrcPosAndTextByLength( pos, rel_res->getName() );
	patRuntime->addBeginConvertStatus( rel_res->begin );
	relRes.push_back( rel_res );
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
	parser->modelStructure << getPatternId() << " " << getName() << " " << getModelStructureLetter() << " " << relRes.size();
	for ( int i = 0; i < relRes.size(); i++ ) {
		parser->modelStructure << " " << relRes.at(i)->getType()->getNumber();
	}
	parser->modelStructure << std::endl;
	return 0;
}

const RDOFUNFunctionParam* RDOPATPattern::findPATPatternParam(const std::string *const paramName) const
{
	std::vector<RDOFUNFunctionParam *>::const_iterator it = 
		std::find_if(params.begin(), params.end(), compareName<RDOFUNFunctionParam>(paramName));

	if(it == params.end())
		return NULL;

	return (*it);
}

const RDORelevantResource* RDOPATPattern::findRelevantResource( const std::string& resName ) const
{
	std::vector< RDORelevantResource* >::const_iterator it = std::find_if( relRes.begin(), relRes.end(), compareName2<RDORelevantResource>(resName) );
	return it != relRes.end() ? (*it) : NULL;
}

int RDOPATPattern::findPATPatternParamNum( const std::string* const paramName ) const
{
	std::vector< RDOFUNFunctionParam* >::const_iterator it = std::find_if( params.begin(), params.end(), compareName<RDOFUNFunctionParam>(paramName) );
	return it != params.end() ? it - params.begin() : -1;
}

int RDOPATPattern::findRelevantResourceNum( const std::string& resName ) const
{
	std::vector< RDORelevantResource* >::const_iterator it = std::find_if( relRes.begin(), relRes.end(), compareName2<RDORelevantResource>(resName) );
	return it != relRes.end() ? it - relRes.begin() : -1;
}

void RDOPATPattern::add( RDOFUNFunctionParam* const _param )
{ 
	if ( findPATPatternParam(_param->getName()) ) {
		parser->error("Second appearance of the same parameter name: " + *(_param->getName()));
	}
	params.push_back(_param); 
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

void RDOPATPattern::setTime( RDOFUNArithm* arithm )
{ 
	patRuntime->setTime( arithm->createCalc(NULL) );
//	time = arithm; 
}

void RDOPATPattern::addRelResBody( std::string* resName )
{ 
	std::vector< RDORelevantResource* >::const_iterator it = std::find_if( relRes.begin(), relRes.end(), compareName2<RDORelevantResource>(*resName) );
	if ( it == relRes.end() ) {
		parser->error( rdo::format("Неизвестный релевантный ресурс: %s", resName->c_str()) );
//		parser->error( "Name of relevant resource expected instead of: " + *resName );
	}
	if ( findRelevantResourceNum( *resName ) != current_rel_res_index ) {
		std::string rel_res_waiting = current_rel_res_index < relRes.size() ? relRes[current_rel_res_index]->getName().c_str() : "";
		parser->error( rdo::format("Ожидается описание релевантного ресурса '%s', вместо него найдено описание '%s'", rel_res_waiting.c_str(), resName->c_str()) );
	}
	if ( (*it)->alreadyHaveConverter ) {
		parser->error( rdo::format("Релевантный ресурс уже используется: %s", resName->c_str()) );
//		parser->error( "\"" + *resName + "\" relevant resource has converter block already" );
	}
	currRelRes = (*it);
	currRelRes->alreadyHaveConverter = true;
	current_rel_res_index++;
}

void RDOPATPattern::addRelResUsage( RDOPATChoiceFrom* choice_from, RDOPATChoiceOrder* choice_order )
{
	if ( useCommonChoice && choice_order->type != rdoRuntime::RDOSelectResourceCalc::st_empty ) {
		parser->error( "Нельзя указать способ выбора релевантного ресурса, т.к. используется единый для всех релевантных ресурсов способ, указанный до ключевого слова $Body" );
//		parser->error( "Cannot use both common choice and choice for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"" );
	}

	if ( !useCommonChoice && choice_order->type == rdoRuntime::RDOSelectResourceCalc::st_empty ) {
		if ( (currRelRes->begin != rdoRuntime::RDOResourceTrace::CS_Create) && (currRelRes->end != rdoRuntime::RDOResourceTrace::CS_Create) ) {
			choice_order->type = rdoRuntime::RDOSelectResourceCalc::st_first;
		}
	}

	if ( (currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_Create) || (currRelRes->end == rdoRuntime::RDOResourceTrace::CS_Create) ) {
		if ( choice_from->type != RDOPATChoiceFrom::ch_empty ) {
			parser->error( "Релевантный ресурс создается, для него нельзя использовать 'Choice'. Ожидается конвертор" );
//			parser->error( "Cannot use choice when create \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"" );
		}
		if ( choice_order->type != rdoRuntime::RDOSelectResourceCalc::st_empty ) {
			parser->error( rdo::format("Релевантный ресурс создается, для него нельзя использовать способ выбора '%s'. Ожидается конвертор", choice_order->asString().c_str()) );
		}
	}

	currRelRes->choice_from  = choice_from;
	currRelRes->choice_order = choice_order;
}

void RDOPATPattern::end()
{
	int size = relRes.size();
	for ( int i = 0; i < size; i++ ) {
		RDORelevantResource* currRelRes = relRes.at( i );
		if ( !currRelRes->alreadyHaveConverter ) {
			parser->lexer_loc_backup();
			parser->lexer_loc_set( &currRelRes->getPosAsYY() );
			// TODO: А почему нельзя сделать warning ? Возможно, есть жесткое требование недопустить пустого рел. ресурса.
			parser->error( rdo::format("Релевантный ресурс '%s' не используется в образце '%s'", currRelRes->getName().c_str(), getName().c_str()) );
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
			if ( relRes.at(i)->begin == rdoRuntime::RDOResourceTrace::CS_Keep || relRes.at(i)->begin == rdoRuntime::RDOResourceTrace::CS_Erase || relRes.at(i)->begin == rdoRuntime::RDOResourceTrace::CS_NoChange ) {
				resSelectors.push_back( relRes.at(i)->createSelectResourceCommonChoiceCalc() );
			}
		}

		patRuntime->addChoiceFromCalc(new rdoRuntime::RDOSelectResourceCommonCalc( parser->runTime, resSelectors, useCommonWithMax, commonChoice->createCalc() ));
//		parser->error("RDOPATPattern::end not implemented yet for not \"first\" conditions in common choice");
	} else {
		for ( int i = 0; i < size; i++ ) {
			rdoRuntime::RDOCalc* calc = relRes.at(i)->createSelectResourceChoiceCalc();
			patRuntime->addChoiceFromCalc(calc);
		}
	}
}

void RDOPATPattern::addRelResConvert()
{
	if ( currRelRes->begin != rdoRuntime::RDOResourceTrace::CS_NoChange && currRelRes->begin != rdoRuntime::RDOResourceTrace::CS_Erase && currRelRes->begin != rdoRuntime::RDOResourceTrace::CS_NonExist ) {
		parser->error( "Ожидается конвертор релевантного ресурса" );
//		parser->error( "Converter needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"" );
	}

	if ( currRelRes->end != rdoRuntime::RDOResourceTrace::CS_NoChange && currRelRes->end != rdoRuntime::RDOResourceTrace::CS_Erase && currRelRes->end != rdoRuntime::RDOResourceTrace::CS_NonExist ) {
		parser->error( "Ожидается конвертор релевантного ресурса" );
//		parser->error( "Converter end needed for \"" + currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"" );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPATPatternEvent
// ----------------------------------------------------------------------------
RDOPATPatternEvent::RDOPATPatternEvent( RDOParser* _parser, std::string* _name, bool _trace ):
	RDOPATPattern( _parser, _name, _trace )
{ 
//	parser->runTime->addRuntimeIE((RDOIERuntime *)(patRuntime = new RDOIERuntime(parser->runTime, _trace))); 
	patRuntime = new rdoRuntime::RDOIERuntime( parser->runTime, _trace ); 
	patRuntime->setPatternId( parser->getPAT_id() );
}

void RDOPATPatternEvent::addRelRes( std::string* relName, std::string* typeName, rdoRuntime::RDOResourceTrace::ConvertStatus beg, const YYLTYPE& rel_pos, const YYLTYPE& type_pos, const YYLTYPE& convertor_pos )
{
	beforeRelRensert( *relName, rel_pos );
	if ( beg == rdoRuntime::RDOResourceTrace::CS_NonExist || beg == rdoRuntime::RDOResourceTrace::CS_NoChange ) {
		parser->lexer_loc_set( convertor_pos.last_line, convertor_pos.last_column );
		parser->error( "Статусы конверторов NonExist и NoChange не могут быть использованы в нерегулярном событии" );
	}

	RDORelevantResource* relRes     = NULL;
	const RDORSSResource* const res = parser->findRSSResource( typeName );
	if ( res ) {
		if ( beg == rdoRuntime::RDOResourceTrace::CS_Create ) {
			parser->lexer_loc_set( type_pos.last_line, type_pos.last_column );
			parser->error( rdo::format("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)", relName->c_str(), typeName->c_str()) );
//			parser->error( "Cannot use Create status for resource, only for resource type" );
		}
		if ( beg == rdoRuntime::RDOResourceTrace::CS_Erase ) {
			parser->lexer_loc_set( convertor_pos.last_line, convertor_pos.last_column );
			parser->error( "Удалять ресурсы в нерегулярном событии нельзя" );
		}
		RDORelevantResourceDirect* rd = new RDORelevantResourceDirect( this, relName, rel_res_count(), res, beg );
		relRes                        = rd;
		rel_res_insert( relRes, rel_pos );
		patRuntime->addChoiceFromCalc( rd->createSelectEmptyResourceCalc() );
	} else {
		const RDORTPResType* const type = parser->findRTPResType( typeName );
		if ( !type ) {
			parser->lexer_loc_set( type_pos.last_line, type_pos.last_column );
			parser->error( rdo::format("Неизвестный тип ресурса: %s", typeName->c_str()) );
//			parser->error( "Unknown resource name or type: " + *typeName );
		}
		if ( beg == rdoRuntime::RDOResourceTrace::CS_Create && !type->isTemporary() ) {
			parser->lexer_loc_set( type_pos.last_line, type_pos.last_column );
			parser->error( rdo::format("Тип ресурса '%s' постоянный. Динамически создавать от него ресурсы нельзя", typeName->c_str()) );
		}
		if ( beg == rdoRuntime::RDOResourceTrace::CS_Keep || beg == rdoRuntime::RDOResourceTrace::CS_Erase ) {
			parser->error( "Статусы конверторов Keep и Erase могут быть использованы в нерегулярном событии с описателем в виде ресурса, но не типа ресурса" );
//			parser->error( "Can use Keep and Erase status with resource name only (not with resource type) in irregular event" );
		}
		relRes = new RDORelevantResourceByType( this, relName, rel_res_count(), type, beg );
		rel_res_insert( relRes, rel_pos );
	}
	if ( relRes->begin == rdoRuntime::RDOResourceTrace::CS_Erase ) {
		patRuntime->addBeginEraseCalc( new rdoRuntime::RDOCalcEraseRes( parser->runTime, relRes->rel_res_id, relRes->getName() ) );
	}
}

void RDOPATPatternEvent::addRelResUsage( RDOPATChoiceFrom* choice_from, RDOPATChoiceOrder* choice_order )
{
	if ( choice_from->type != RDOPATChoiceFrom::ch_empty ) {
		parser->error( "Релевантные ресурсы нерегулярного события нельзя выбирать с помощью Choice from или Choice NoCheck" );
//		parser->error( "Unexpected choice for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"" );
	}
	if ( choice_order->type != rdoRuntime::RDOSelectResourceCalc::st_empty ) {
		parser->error( rdo::format("Для релевантных ресурсов нерегулярного события нельзя использовать правило выбора '%s'", choice_order->asString().c_str()) );
	}
	currRelRes->choice_from  = choice_from;
	currRelRes->choice_order = choice_order;
}

void RDOPATPatternEvent::addRelResConvert( bool trace, RDOPATParamSet* parSet, const YYLTYPE& convertor_pos )
{
	parSet->setParamsNumbers();
	if ( currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_NoChange || currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
		parser->lexer_loc_set( convertor_pos.last_line, convertor_pos.last_column );
		parser->error( rdo::format("Для релевантного ресурса '%s' не требуется конвертор, т.к. его статус: %s", currRelRes->getName().c_str(), RDOPATPattern::StatusToStr(currRelRes->begin).c_str()) );
	}

	if ( currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_Create ) {
		if ( currRelRes->getType()->getParams().size() != parSet->paramNumbs.size() ) {
			parser->lexer_loc_set( parSet->src_pos().last_line, parSet->src_pos().last_pos );
			parser->error( rdo::format("При создании ресурса необходимо определить все его параметры"));
//			parser->error("Must define all parameters when create new resource: \"" + currRelRes->getName() + "\" in pattern \"" + getName() + "\" in converter");
		}

		rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOCalcCreateEmptyResource( parser->runTime, currRelRes->getType()->getNumber(), trace, currRelRes->rel_res_id, currRelRes->getType()->getParams().size() );
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
			rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOSetRelParamCalc( parser->runTime, currRelRes->rel_res_id, parNumb, rightValue );
			patRuntime->addBeginCalc(calc);
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPATPatternRule
// ----------------------------------------------------------------------------
RDOPATPatternRule::RDOPATPatternRule( RDOParser* _parser, std::string* _name, bool _trace ):
	RDOPATPattern( _parser, _name, _trace )
{ 
//	parser->runTime->addRuntimeRule((RDORuleRuntime *)(patRuntime = new RDORuleRuntime(parser->runTime, _trace))); 
	patRuntime = new rdoRuntime::RDORuleRuntime( parser->runTime, _trace );
	patRuntime->setPatternId( parser->getPAT_id() );
}

void RDOPATPatternRule::testGoodForSearchActivity() const
{
	for ( std::vector< RDORelevantResource* >::const_iterator i = rel_res_begin(); i != rel_res_end(); i++ ) {
		if ( ((*i)->begin == rdoRuntime::RDOResourceTrace::CS_Create) || ((*i)->begin == rdoRuntime::RDOResourceTrace::CS_Erase) ) {
			parser->error( "Rule: " + getName() + " Cannot be used in search activity because of bad converter status" );
		}
	}
}

void RDOPATPatternRule::setTime( RDOFUNArithm* arithm )
{ 
	parser->error( rdo::format("Поле $Time не используется в образце типа %s", getPatTypeStr().c_str()) );
//	parser->error( "Rule must have no $Time field" );
}

void RDOPATPatternRule::addRelRes( std::string* relName, std::string* typeName, rdoRuntime::RDOResourceTrace::ConvertStatus beg, const YYLTYPE& rel_pos, const YYLTYPE& type_pos, const YYLTYPE& convertor_pos )
{
	beforeRelRensert( *relName, rel_pos );
	if ( beg == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
		parser->lexer_loc_set( convertor_pos.last_line, convertor_pos.last_column );
		parser->error( rdo::format("Нельзя использовать статус конвертора '%s' в продукционном правиле", RDOPATPattern::StatusToStr(beg).c_str()) );
//		parser->error( "Cannot use NonExist status in rule" );
	}

	RDORelevantResource* relRes     = NULL;
	const RDORSSResource* const res = parser->findRSSResource( typeName );
	if ( res ) {
		if ( beg == rdoRuntime::RDOResourceTrace::CS_Create ) {
			parser->lexer_loc_set( type_pos.last_line, type_pos.last_column );
			parser->error( rdo::format("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)", relName->c_str(), typeName->c_str()) );
//			parser->error( "Cannot use Create status for resource, only for resource type" );
		}
		if ( beg == rdoRuntime::RDOResourceTrace::CS_Erase ) {
			parser->lexer_loc_set( convertor_pos.last_line, convertor_pos.last_column );
			parser->error( rdo::format("Недопустимый статус конвертора для ресурса: %s", RDOPATPattern::StatusToStr(beg).c_str()) );
//			parser->error( "Wrong converter status for resource of permanent type" );
		}
		relRes = new RDORelevantResourceDirect( this, relName, rel_res_count(), res, beg );
		rel_res_insert( relRes, rel_pos );

	} else {
		const RDORTPResType* const type = parser->findRTPResType( typeName );
		if ( !type ) {
			parser->lexer_loc_set( type_pos.last_line, type_pos.last_column );
			parser->error( rdo::format("Неизвестный тип ресурса: %s", typeName->c_str()) );
//			parser->error( "Unknown resource name or type: " + *typeName );
		}
		if ( !type->isTemporary() && (beg == rdoRuntime::RDOResourceTrace::CS_Create || beg == rdoRuntime::RDOResourceTrace::CS_Erase ) ) {
			parser->lexer_loc_set( type_pos.last_line, type_pos.last_column );
			parser->error( rdo::format("Для создании или удалении ресурса его тип должен быть временным (temporary), а не постоянным (permanent), как у '%s'", typeName->c_str()) );
		}
		relRes = new RDORelevantResourceByType( this, relName, rel_res_count(), type, beg );
		rel_res_insert( relRes, rel_pos );
	}
	if ( relRes->begin == rdoRuntime::RDOResourceTrace::CS_Erase ) {
		patRuntime->addBeginEraseCalc( new rdoRuntime::RDOCalcEraseRes( parser->runTime, relRes->rel_res_id, relRes->getName() ) );
	}
}

void RDOPATPatternRule::addRelResConvert( bool trace, RDOPATParamSet* parSet, const YYLTYPE& convertor_pos )
{
	parSet->setParamsNumbers();
	if ( currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_NoChange || currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
		parser->lexer_loc_set( convertor_pos.last_line, convertor_pos.last_column );
		parser->error( rdo::format("Для релевантного ресурса '%s' не требуется конвертор, т.к. его статус: %s", currRelRes->getName().c_str(), RDOPATPattern::StatusToStr(currRelRes->begin).c_str()) );
//		parser->error("Converter not needed for \"" + currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"");
	}

	if ( currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_Create ) {
		if ( currRelRes->getType()->getParams().size() != parSet->paramNumbs.size() ) {
			parser->lexer_loc_set( parSet->src_pos().last_line, parSet->src_pos().last_pos );
			parser->error( rdo::format("При создании ресурса необходимо определить все его параметры"));
//			parser->error("Must define all parameters when create new resource: \"" + currRelRes->getName() + "\" in pattern \"" + getName() + "\" in converter");
		}

		rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOCalcCreateEmptyResource( parser->runTime, currRelRes->getType()->getNumber(), trace, currRelRes->rel_res_id, currRelRes->getType()->getParams().size() );
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
			rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOSetRelParamCalc( parser->runTime, currRelRes->rel_res_id, parNumb, rightValue );
			patRuntime->addBeginCalc(calc);
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPATPatternOperation
// ----------------------------------------------------------------------------
RDOPATPatternOperation::RDOPATPatternOperation( RDOParser* _parser, std::string* _name, bool _trace ):
	RDOPATPattern( _parser, _name, _trace )
{ 
//	parser->runTime->addRuntimeOperation((RDOOperationRuntime *)(patRuntime = new RDOOperationRuntime(parser->runTime, _trace)));
	patRuntime = new rdoRuntime::RDOOperationRuntime( parser->runTime, _trace );
	patRuntime->setPatternId( parser->getPAT_id() );
}

RDOPATPatternOperation::RDOPATPatternOperation( RDOParser* _parser, bool _trace, std::string* _name ):
	RDOPATPattern( _parser, _name, _trace )
{
}

void RDOPATPatternOperation::rel_res_insert( RDORelevantResource* rel_res, const YYLTYPE& pos )
{
	RDOPATPattern::rel_res_insert( rel_res, pos );
	static_cast<rdoRuntime::RDOOperationRuntime*>(patRuntime)->addEndConvertStatus( rel_res->end );
}

void RDOPATPatternOperation::addRelRes( std::string* relName, std::string* typeName, rdoRuntime::RDOResourceTrace::ConvertStatus beg, const YYLTYPE& rel_pos, const YYLTYPE& type_pos, const YYLTYPE& convertor_pos )
{
	parser->lexer_loc_set( convertor_pos.last_line, convertor_pos.last_column );
	parser->error( "Внутренняя ошибка парсера" );
}

void RDOPATPatternOperation::addRelRes( std::string* relName, std::string* typeName, rdoRuntime::RDOResourceTrace::ConvertStatus beg, rdoRuntime::RDOResourceTrace::ConvertStatus end, const YYLTYPE& rel_pos, const YYLTYPE& type_pos, const YYLTYPE& convertor_begin_pos, const YYLTYPE& convertor_end_pos )
{
	beforeRelRensert( *relName, rel_pos );
	switch ( beg ) {
		case rdoRuntime::RDOResourceTrace::CS_Keep:
			if ( end != rdoRuntime::RDOResourceTrace::CS_Keep && end != rdoRuntime::RDOResourceTrace::CS_Erase && end != rdoRuntime::RDOResourceTrace::CS_NoChange ) {
				parser->lexer_loc_set( convertor_end_pos.last_line, convertor_end_pos.last_column );
				parser->error( rdo::format("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()) );
//				parser->error( "Wrong second converter status" );
			}
			break;
		case rdoRuntime::RDOResourceTrace::CS_Create:
			if ( end != rdoRuntime::RDOResourceTrace::CS_Keep && end != rdoRuntime::RDOResourceTrace::CS_Erase && end != rdoRuntime::RDOResourceTrace::CS_NoChange ) {
				parser->lexer_loc_set( convertor_end_pos.last_line, convertor_end_pos.last_column );
				parser->error( rdo::format("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()) );
//				parser->error( "Wrong second converter status" );
			}
			break;
		case rdoRuntime::RDOResourceTrace::CS_Erase:
			if ( end != rdoRuntime::RDOResourceTrace::CS_NonExist ) {
				parser->lexer_loc_set( convertor_end_pos.last_line, convertor_end_pos.last_column );
				parser->error( rdo::format("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()) );
//				parser->error( "Wrong second converter status" );
			}
			break;
		case rdoRuntime::RDOResourceTrace::CS_NonExist:
			if ( end != rdoRuntime::RDOResourceTrace::CS_Create ) {
				parser->lexer_loc_set( convertor_end_pos.last_line, convertor_end_pos.last_column );
				parser->error( rdo::format("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()) );
//				parser->error( "Wrong second converter status" );
			}
			break;
		case rdoRuntime::RDOResourceTrace::CS_NoChange:
			if ( end != rdoRuntime::RDOResourceTrace::CS_Keep && end != rdoRuntime::RDOResourceTrace::CS_Erase && end != rdoRuntime::RDOResourceTrace::CS_NoChange ) {
				parser->lexer_loc_set( convertor_end_pos.last_line, convertor_end_pos.last_column );
				parser->error( rdo::format("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()) );
//				parser->error( "Wrong second converter status" );
			}
			break;
	};

	RDORelevantResource* relRes     = NULL;
	const RDORSSResource* const res = parser->findRSSResource( typeName );
	if ( res ) {
		if ( beg == rdoRuntime::RDOResourceTrace::CS_Create ) {
			parser->lexer_loc_set( convertor_begin_pos.last_line, convertor_begin_pos.last_column );
			parser->error( rdo::format("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)", relName->c_str(), typeName->c_str()) );
		}
		if ( end == rdoRuntime::RDOResourceTrace::CS_Create ) {
			parser->lexer_loc_set( convertor_end_pos.last_line, convertor_end_pos.last_column );
			parser->error( rdo::format("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)", relName->c_str(), typeName->c_str()) );
		}
		if ( beg == rdoRuntime::RDOResourceTrace::CS_Erase || beg == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
			parser->lexer_loc_set( convertor_begin_pos.last_line, convertor_begin_pos.last_column );
			parser->error( rdo::format("Недопустимый статус конвертора начала для ресурса: %s", RDOPATPattern::StatusToStr(beg).c_str()) );
		}
		if ( end == rdoRuntime::RDOResourceTrace::CS_Erase || end == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
			parser->lexer_loc_set( convertor_end_pos.last_line, convertor_end_pos.last_column );
			parser->error( rdo::format("Недопустимый статус конвертора конца для ресурса: %s", RDOPATPattern::StatusToStr(end).c_str()) );
		}
		relRes = new RDORelevantResourceDirect( this, relName, rel_res_count(), res, beg, end );
		rel_res_insert( relRes, rel_pos );

	} else {
		const RDORTPResType* const type = parser->findRTPResType( typeName );
		if ( !type ) {
			parser->lexer_loc_set( type_pos.last_line, type_pos.last_column );
			parser->error( rdo::format("Неизвестный тип ресурса: %s", typeName->c_str()) );
//			parser->error( "Unknown resource name or type: " + *typeName );
		}
		if ( type->isPermanent() ) {
			if ( beg == rdoRuntime::RDOResourceTrace::CS_Create || beg == rdoRuntime::RDOResourceTrace::CS_Erase || beg == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
				parser->lexer_loc_set( convertor_begin_pos.last_line, convertor_begin_pos.last_column );
				parser->error( rdo::format("Недопустимый статус конвертора начала для постоянного типа: %s", RDOPATPattern::StatusToStr(beg).c_str()) );
			}
			if ( end == rdoRuntime::RDOResourceTrace::CS_Create || end == rdoRuntime::RDOResourceTrace::CS_Erase || end == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
				parser->lexer_loc_set( convertor_end_pos.last_line, convertor_end_pos.last_column );
				parser->error( rdo::format("Недопустимый статус конвертора конца для постоянного типа: %s", RDOPATPattern::StatusToStr(end).c_str()) );
			}
		}
		relRes = new RDORelevantResourceByType( this, relName, rel_res_count(), type, beg, end );
		rel_res_insert( relRes, rel_pos );
	}
	if ( relRes->begin == rdoRuntime::RDOResourceTrace::CS_Erase ) {
		patRuntime->addBeginEraseCalc( new rdoRuntime::RDOCalcEraseRes( parser->runTime, relRes->rel_res_id, relRes->getName() ) );
	}
	if ( relRes->end == rdoRuntime::RDOResourceTrace::CS_Erase ) {
		static_cast<rdoRuntime::RDOOperationRuntime*>(patRuntime)->addEndEraseCalc( new rdoRuntime::RDOCalcEraseRes( parser->runTime, relRes->rel_res_id, relRes->getName() ) );
	}
}

void RDOPATPatternOperation::addRelResConvert( bool trace, RDOPATParamSet* parSet, const YYLTYPE& convertor_pos )
{
	parSet->setParamsNumbers();
	if ( currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_NoChange || currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
		parser->lexer_loc_set( convertor_pos.last_line, convertor_pos.last_column );
		parser->error( rdo::format("Для релевантного ресурса '%s' не требуется конвертор начала, т.к. его статус: %s", currRelRes->getName().c_str(), RDOPATPattern::StatusToStr(currRelRes->begin).c_str()) );
	}
//	if ( currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_NoChange || currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_Erase || currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_NonExist )
//		parser->error("Converter begin not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"");

	if ( currRelRes->end != rdoRuntime::RDOResourceTrace::CS_NoChange && currRelRes->end != rdoRuntime::RDOResourceTrace::CS_Erase && currRelRes->end != rdoRuntime::RDOResourceTrace::CS_NonExist ) {
		parser->error( rdo::format("Ожидается конвертор конца для релевантного ресурса '%s', т.к. его статус: %s", currRelRes->getName().c_str(), RDOPATPattern::StatusToStr(currRelRes->end).c_str()) );
//		parser->error("Converter end needed for \"" + currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"");
	}

	if ( currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_Create ) {
		if ( currRelRes->getType()->getParams().size() != parSet->paramNumbs.size() ) {
			parser->lexer_loc_set( parSet->src_pos().last_line, parSet->src_pos().last_pos );
			parser->error( rdo::format("При создании ресурса необходимо определить все его параметры"));
//			parser->error("Must define all parameters when create new resource: \"" + currRelRes->getName() + "\" in pattern \"" + getName() + "\" in convert begin");
		}

		rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOCalcCreateEmptyResource( parser->runTime, currRelRes->getType()->getNumber(), trace, currRelRes->rel_res_id, currRelRes->getType()->getParams().size() );
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
			rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOSetRelParamCalc( parser->runTime, currRelRes->rel_res_id, parNumb, rightValue );
			patRuntime->addBeginCalc(calc);
		}
	}
}

void RDOPATPatternOperation::addRelResConvertEnd( bool trace, RDOPATParamSet* parSet, const YYLTYPE& convertor_pos )
{
	parSet->setParamsNumbers();
	if ( currRelRes->end == rdoRuntime::RDOResourceTrace::CS_NoChange || currRelRes->end == rdoRuntime::RDOResourceTrace::CS_NonExist || currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_Erase ) {
		parser->lexer_loc_set( convertor_pos.last_line, convertor_pos.last_column );
		parser->error( rdo::format("Для релевантного ресурса '%s' не требуется конвертор конца, т.к. его статус: %s", currRelRes->getName().c_str(), RDOPATPattern::StatusToStr(currRelRes->begin).c_str()) );
	}
//	if ( currRelRes->end == rdoRuntime::RDOResourceTrace::CS_NoChange || currRelRes->end == rdoRuntime::RDOResourceTrace::CS_Erase || currRelRes->end == rdoRuntime::RDOResourceTrace::CS_NonExist || currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_Erase ) {
//		parser->error( "Converter end not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"" );

	if ( currRelRes->begin != rdoRuntime::RDOResourceTrace::CS_NoChange && currRelRes->begin != rdoRuntime::RDOResourceTrace::CS_Erase && currRelRes->begin != rdoRuntime::RDOResourceTrace::CS_NonExist ) {
		parser->lexer_loc_set( convertor_pos.last_line, convertor_pos.last_column );
		parser->error( rdo::format("Перед конвертором конца ожидается конвертор начала для релевантного ресурса '%s', т.к. его статус: %s", currRelRes->getName().c_str(), RDOPATPattern::StatusToStr(currRelRes->begin).c_str()) );
	}
//	if ( currRelRes->begin != rdoRuntime::RDOResourceTrace::CS_NoChange && currRelRes->begin != rdoRuntime::RDOResourceTrace::CS_Erase && currRelRes->begin != rdoRuntime::RDOResourceTrace::CS_NonExist )
//		parser->error("Converter begin needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"");

	if ( currRelRes->end == rdoRuntime::RDOResourceTrace::CS_Create ) {
		if ( currRelRes->getType()->getParams().size() != parSet->paramNumbs.size() ) {
			parser->lexer_loc_set( parSet->src_pos().last_line, parSet->src_pos().last_pos );
			parser->error( rdo::format("При создании ресурса необходимо определить все его параметры"));
//			parser->error("Must define all parameters when create new resource: \"" + currRelRes->getName() + "\" in pattern \"" + getName() + "\" in convert end");
		}

		rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOCalcCreateEmptyResource( parser->runTime, currRelRes->getType()->getNumber(), trace, currRelRes->rel_res_id, currRelRes->getType()->getParams().size() );
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
			rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOSetRelParamCalc( parser->runTime, currRelRes->rel_res_id, parNumb, rightValue );
			static_cast<rdoRuntime::RDOOperationRuntime*>(patRuntime)->addEndCalc(calc);
		}
	}
}

void RDOPATPatternOperation::addRelResConvertBeginEnd( bool trace, RDOPATParamSet* parSet, bool trace2, RDOPATParamSet* parSet2, const YYLTYPE& convertor_begin_pos, const YYLTYPE& convertor_end_pos )
{
	parSet->setParamsNumbers();
	parSet2->setParamsNumbers();
	if ( currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_NoChange || currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
		parser->lexer_loc_set( convertor_begin_pos.last_line, convertor_begin_pos.last_column );
		parser->error( rdo::format("Для релевантного ресурса '%s' не требуется конвертор начала, т.к. его статус: %s", currRelRes->getName().c_str(), RDOPATPattern::StatusToStr(currRelRes->begin).c_str()) );
	}
//	if ( currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_NoChange || currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_Erase || currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_NonExist )
//		parser->error("Converter begin not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"");

	if ( currRelRes->end == rdoRuntime::RDOResourceTrace::CS_NoChange || currRelRes->end == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
		parser->lexer_loc_set( convertor_end_pos.last_line, convertor_end_pos.last_column );
		parser->error( rdo::format("Для релевантного ресурса '%s' не требуется конвертор конца, т.к. его статус: %s", currRelRes->getName().c_str(), RDOPATPattern::StatusToStr(currRelRes->begin).c_str()) );
	}
//	if (currRelRes->end == rdoRuntime::RDOResourceTrace::CS_NoChange || currRelRes->end == rdoRuntime::RDOResourceTrace::CS_Erase || currRelRes->end == rdoRuntime::RDOResourceTrace::CS_NonExist )
//		parser->error("Converter end not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"");

	if ( currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_Create ) {
		if ( currRelRes->getType()->getParams().size() != parSet->paramNumbs.size() ) {
			parser->lexer_loc_set( parSet->src_pos().last_line, parSet->src_pos().last_pos );
			parser->error( rdo::format("При создании ресурса необходимо определить все его параметры"));
//			parser->error("Must define all parameters when create new resource: \"" + currRelRes->getName() + "\" in pattern \"" + getName() + "\" in convert begin");
		}
		rdoRuntime::RDOCalc* calc = new rdoRuntime::RDOCalcCreateEmptyResource( parser->runTime, currRelRes->getType()->getNumber(), trace, currRelRes->rel_res_id, currRelRes->getType()->getParams().size() );
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
			rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOSetRelParamCalc( parser->runTime, currRelRes->rel_res_id, parNumb, rightValue );
			patRuntime->addBeginCalc(calc);
		}
	}

	if(currRelRes->end == rdoRuntime::RDOResourceTrace::CS_Create)
	{
		if ( currRelRes->getType()->getParams().size() != parSet2->paramNumbs.size() ) {
			parser->lexer_loc_set( parSet2->src_pos().last_line, parSet2->src_pos().last_pos );
			parser->error( rdo::format("При создании ресурса необходимо определить все его параметры"));
//			parser->error("Must define all parameters when create new resource: \"" + currRelRes->getName() + "\" in pattern \"" + getName() + "\" in convert end");
		}

		rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOCalcCreateEmptyResource( parser->runTime, currRelRes->getType()->getNumber(), trace2, currRelRes->rel_res_id, currRelRes->getType()->getParams().size() );
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
			rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOSetRelParamCalc( parser->runTime, currRelRes->rel_res_id, parNumb, rightValue );
			static_cast<rdoRuntime::RDOOperationRuntime*>(patRuntime)->addEndCalc(calc);
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPATPatternKeyboard
// ----------------------------------------------------------------------------
RDOPATPatternKeyboard::RDOPATPatternKeyboard( RDOParser* _parser, std::string* _name, bool _trace ):
	RDOPATPatternOperation( _parser, _trace, _name )
{
	patRuntime = new rdoRuntime::RDOKeyboardRuntime(parser->runTime, _trace); 
	patRuntime->setPatternId( parser->getPAT_id() );
}

// ----------------------------------------------------------------------------
// ---------- RDORelevantResource
// ----------------------------------------------------------------------------
rdoRuntime::RDOCalc* RDORelevantResource::getChoiceCalc() const
{
	if ( choice_from && choice_from->type == rdoParse::RDOPATChoiceFrom::ch_from ) {
		return choice_from->logic->calc;
	}
	return NULL;
}

rdoRuntime::RDOCalc* RDORelevantResource::getSelectCalc() const
{
	if ( choice_order && choice_order->arithm ) {
		return choice_order->arithm->createCalc( NULL );
	}
	return NULL;
}

rdoRuntime::RDOSelectResourceCalc::Type RDORelevantResource::getSelectType() const
{
	return choice_order ? choice_order->type : rdoRuntime::RDOSelectResourceCalc::st_empty;
}

// ----------------------------------------------------------------------------
// ---------- RDORelevantResourceDirect - по имени ресурса
// ----------------------------------------------------------------------------
rdoRuntime::RDOCalc* RDORelevantResourceDirect::createSelectEmptyResourceCalc()
{
	rdoRuntime::RDOSelectResourceDirectCalc* relres_calc = new rdoRuntime::RDOSelectResourceDirectCalc( parser->runTime, rel_res_id, res->getNumber(), NULL, NULL );
	relres_calc->setSrcInfo( src_info() );
	relres_calc->setSrcText( "Предварительный выбор рел. ресурса " + relres_calc->src_text() );
	return relres_calc;
}

rdoRuntime::RDOCalc* RDORelevantResourceDirect::createSelectFirstResourceChoiceCalc()
{
	return new rdoRuntime::RDOSelectResourceDirectCalc( parser->runTime, rel_res_id, res->getNumber(), getChoiceCalc() );
}

rdoRuntime::RDOCalc* RDORelevantResourceDirect::createSelectResourceChoiceCalc()
{
	rdoRuntime::RDOSelectResourceDirectCalc* relres_calc = new rdoRuntime::RDOSelectResourceDirectCalc( parser->runTime, rel_res_id, res->getNumber(), getChoiceCalc(), getSelectCalc(), getSelectType() );
	relres_calc->setSrcInfo( choice_from->src_info() );
	return relres_calc;
}

rdoRuntime::RDOSelectResourceCommon* RDORelevantResourceDirect::createSelectResourceCommonChoiceCalc()
{
	return new rdoRuntime::RDOSelectResourceDirectCommonCalc( parser->runTime, rel_res_id, res->getNumber(), getChoiceCalc() );
}

const RDORTPResType* const RDORelevantResourceDirect::getType() const 
{ 
	return res->getType(); 
}

// ----------------------------------------------------------------------------
// ---------- RDORelevantResourceByType - по имени типа
// ----------------------------------------------------------------------------
rdoRuntime::RDOCalc* RDORelevantResourceByType::createSelectEmptyResourceCalc()
{
	if ( (begin != rdoRuntime::RDOResourceTrace::CS_Create) && (end != rdoRuntime::RDOResourceTrace::CS_Create) ) {
		return new rdoRuntime::RDOSelectResourceByTypeCalc( parser->runTime, rel_res_id, type->getNumber(), NULL, NULL );
	} else {
		if ( begin == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
			return new rdoRuntime::RDOSelectResourceNonExistCalc( parser->runTime, rel_res_id );
		} else {
			return new rdoRuntime::RDOCalcConst( parser->runTime, 1 );
		}
	}
}

rdoRuntime::RDOCalc* RDORelevantResourceByType::createSelectFirstResourceChoiceCalc()
{
	if ( (begin != rdoRuntime::RDOResourceTrace::CS_Create) && (end != rdoRuntime::RDOResourceTrace::CS_Create) ) {
		return new rdoRuntime::RDOSelectResourceByTypeCalc( parser->runTime, rel_res_id, type->getNumber(), getChoiceCalc() );
	} else {
		if ( begin == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
			return new rdoRuntime::RDOSelectResourceNonExistCalc( parser->runTime, rel_res_id );
		} else {
			return new rdoRuntime::RDOCalcConst( parser->runTime, 1 );
		}
	}
}

rdoRuntime::RDOCalc* RDORelevantResourceByType::createSelectResourceChoiceCalc()
{
	if ( (begin != rdoRuntime::RDOResourceTrace::CS_Create) && (end != rdoRuntime::RDOResourceTrace::CS_Create) ) {
		return new rdoRuntime::RDOSelectResourceByTypeCalc( parser->runTime, rel_res_id, type->getNumber(), getChoiceCalc(), getSelectCalc(), getSelectType() );
	} else {
		if ( begin == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
			return new rdoRuntime::RDOSelectResourceNonExistCalc( parser->runTime, rel_res_id );
		} else {
			return new rdoRuntime::RDOCalcConst( parser->runTime, 1 );
		}
	}
}

rdoRuntime::RDOSelectResourceCommon* RDORelevantResourceByType::createSelectResourceCommonChoiceCalc()
{
	return new rdoRuntime::RDOSelectResourceByTypeCommonCalc( parser->runTime, rel_res_id, type->getNumber(), getChoiceCalc() );
}

// ----------------------------------------------------------------------------
// ---------- RDOPATParamSet - все операторы set для одного рел. ресурса
// ----------------------------------------------------------------------------
void RDOPATParamSet::addSet( std::string* paramName, const YYLTYPE& param_name_pos, RDOFUNArithm* paramArithm )
{
	if ( paramNames.empty() ) {
		setSrcPos( param_name_pos );
	}
	YYLTYPE position = getPosAsYY();
	if ( paramArithm ) {
		position.last_line   = paramArithm->src_pos().last_line;
		position.last_column = paramArithm->src_pos().last_pos;
	}
	setSrcPos( position );
	checkParam( paramName );
	if ( paramArithm ) {
		const RDORTPParamDesc* param = rel_res->getType()->findRTPParam( paramName );
		param->getType()->checkParamType( paramArithm );
	}
	paramNames.push_back( paramName );
	paramArithms.push_back( paramArithm );
}

void RDOPATParamSet::checkParam( std::string* paramName )
{
	const RDORTPParamDesc* param = rel_res->getType()->findRTPParam( paramName );
	if ( !param ) {
		parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		parser->error( rdo::format("Неизвестный параметр ресурса: %s", paramName->c_str()) );
	}
	std::vector< std::string* >::const_iterator it = paramNames.begin();
	while ( it != paramNames.end() ) {
		if ( **it == *paramName ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
			parser->error( rdo::format("Параметр ресурса уже используется: %s", paramName->c_str()) );
		}
		it++;
	}
}

void RDOPATParamSet::setParamsNumbers()
{
	int size = paramArithms.size();
	for ( int i = 0; i < size; i++ ) {
		int parNumb = rel_res->getType()->getRTPParamNumber( paramNames.at(i) );
		if ( parNumb == -1 ) {
			parser->error( rdo::format("Неизвестный параметр ресурса: %s", paramNames.at(i)->c_str()) );
//			parser->error("Wrong resource parameter name: " + *paramNames.at(i));
		}
		if ( std::find(paramNumbs.begin(), paramNumbs.end(), parNumb) != paramNumbs.end() ) {
			parser->error( rdo::format("Параметр ресурса уже используется: %s", paramNames.at(i)->c_str()) );
//			parser->error("Second appearence of the same resource parameter name: " + *paramNames.at(i));
		}
		paramNumbs.push_back( parNumb );
	}
}

} // namespace rdoParse

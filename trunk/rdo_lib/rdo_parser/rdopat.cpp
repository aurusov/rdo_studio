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
RDOPATPattern::RDOPATPattern( RDOParser* _parser, const RDOParserSrcInfo& _name_src_info, const bool _trace ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _name_src_info ),
	commonChoice( NULL ),
	patRuntime( NULL ),
	currRelRes( NULL ),
	current_rel_res_index( 0 ),
	trace( _trace ),
	useCommonChoice( false )
{
	const RDOPATPattern* pattern = parser->findPattern( src_info().src_text() );
	if ( pattern ) {
		parser->error_push_only( src_info(), rdo::format("Паттерн '%s' уже существует", getName().c_str()) );
		parser->error( pattern->src_info(), rdo::format("См. первое определение") );
//		parser->error( "Pattern with name: " + name + " already exist" );
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
	parser->error( "Только продукционные правила могут быть использованы в точке принятия решений" );
//	parser->error("Only RULEs can be used in search activity");
}

void RDOPATPattern::testGoodForSomeActivity() const
{
	parser->error( "Только продукционные правила и операции могут быть использованы в точке типа some" );
//	parser->error("Only RULEs and OPERATIONs can be used in some activity");
}

void RDOPATPattern::testGoodForFreeActivity() const
{
	parser->error( "Только нерегулярные события и клавиатурные операции могут быть использованы в списке свободных активностей" );
//	parser->error("Only IEs and KEYBOARD OPERATIONs can be used in free activity");
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

const RDOFUNFunctionParam* RDOPATPattern::findPATPatternParam( const std::string& paramName ) const
{
	std::vector< RDOFUNFunctionParam* >::const_iterator it = std::find_if(params.begin(), params.end(), compareName2<RDOFUNFunctionParam>(paramName));

	if(it == params.end())
		return NULL;

	return (*it);
}

const RDORelevantResource* RDOPATPattern::findRelevantResource( const std::string& resName ) const
{
	std::vector< RDORelevantResource* >::const_iterator it = std::find_if( relRes.begin(), relRes.end(), compareName2<RDORelevantResource>(resName) );
	return it != relRes.end() ? (*it) : NULL;
}

int RDOPATPattern::findPATPatternParamNum( const std::string& paramName ) const
{
	std::vector< RDOFUNFunctionParam* >::const_iterator it = std::find_if( params.begin(), params.end(), compareName2<RDOFUNFunctionParam>(paramName) );
	return it != params.end() ? it - params.begin() : -1;
}

int RDOPATPattern::findRelevantResourceNum( const std::string& resName ) const
{
	std::vector< RDORelevantResource* >::const_iterator it = std::find_if( relRes.begin(), relRes.end(), compareName2<RDORelevantResource>(resName) );
	return it != relRes.end() ? it - relRes.begin() : -1;
}

void RDOPATPattern::add( RDOFUNFunctionParam* const _param )
{
	const RDOFUNFunctionParam* param = findPATPatternParam(_param->getName());
	if ( param ) {
		parser->error_push_only( _param->src_info(), rdo::format("Параметр '%s' уже определен", _param->src_text().c_str()) );
		parser->error( param->src_info(), rdo::format("См. первое определение") );
//		parser->error("Second appearance of the same parameter name: " + _param->getName());
	}
	params.push_back( _param ); 
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

void RDOPATPattern::addRelResBody( const RDOParserSrcInfo& body_name )
{ 
	std::vector< RDORelevantResource* >::const_iterator it = std::find_if( relRes.begin(), relRes.end(), compareName2<RDORelevantResource>(body_name.src_text()) );
	if ( it == relRes.end() ) {
		parser->error( body_name.src_info(), rdo::format("Неизвестный релевантный ресурс: %s", body_name.src_text().c_str()) );
//		parser->error( "Name of relevant resource expected instead of: " + body_name.src_text() );
	}
	if ( findRelevantResourceNum( body_name.src_text() ) != current_rel_res_index ) {
		std::string rel_res_waiting = current_rel_res_index < relRes.size() ? relRes[current_rel_res_index]->getName().c_str() : "";
		parser->error( body_name.src_info(), rdo::format("Ожидается описание релевантного ресурса '%s', вместо него найдено: %s", rel_res_waiting.c_str(), body_name.src_text().c_str()) );
	}
	if ( (*it)->alreadyHaveConverter ) {
		parser->error( body_name.src_info(), rdo::format("Релевантный ресурс уже используется: %s", body_name.src_text().c_str()) );
//		parser->error( "\"" + body_name.src_text() + "\" relevant resource has converter block already" );
	}
	currRelRes = (*it);
	currRelRes->body_name = body_name;
	currRelRes->alreadyHaveConverter = true;
	current_rel_res_index++;
}

void RDOPATPattern::addRelResUsage( RDOPATChoiceFrom* choice_from, RDOPATChoiceOrder* choice_order )
{
	if ( !useCommonChoice && choice_order->type == rdoRuntime::RDOSelectResourceCalc::order_empty ) {
		if ( (currRelRes->begin != rdoRuntime::RDOResourceTrace::CS_Create) && (currRelRes->end != rdoRuntime::RDOResourceTrace::CS_Create) ) {
			choice_order->type = rdoRuntime::RDOSelectResourceCalc::order_first;
		}
	}

	if ( (currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_Create) || (currRelRes->end == rdoRuntime::RDOResourceTrace::CS_Create) ) {
		if ( choice_from->type != RDOPATChoiceFrom::ch_empty ) {
			parser->error( choice_from->src_info(), "Релевантный ресурс создается, для него нельзя использовать Choice from или Choice NoCheck" );
//			parser->error( "Cannot use choice when create \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"" );
		}
		if ( choice_order->type != rdoRuntime::RDOSelectResourceCalc::order_empty ) {
			parser->error( choice_from->src_info(), rdo::format("Релевантный ресурс создается, для него нельзя использовать правило выбора '%s'", choice_order->asString().c_str()) );
		}
	}

	if ( useCommonChoice && choice_order->type != rdoRuntime::RDOSelectResourceCalc::order_empty ) {
		parser->error( choice_from->src_info(), "Нельзя указать способ выбора релевантного ресурса, т.к. используется единый для всех релевантных ресурсов способ, указанный до ключевого слова $Body" );
//		parser->error( "Cannot use both common choice and choice for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"" );
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

	if ( useCommonChoice ) {
		// first
		// Работает неправильно, а как обыкновенный first
//		parser->error("RDOPATPattern::end not implemented yet for not \"first\" conditions in common choice");
		if ( commonChoice == NULL ) { // first
//			int size = relRes.size();
			for( int i = 0; i < size; i++ ) {
				rdoRuntime::RDOCalc* calc = relRes.at(i)->createSelectFirstResourceChoiceCalc();
				patRuntime->addChoiceFromCalc( calc );
			}
			return;
		} else {
			// with_min / with_max
			std::vector< rdoRuntime::RDOSelectResourceCommon* > resSelectors;
			for ( int i = 0; i < size; i++ ) {
				if ( relRes.at(i)->begin == rdoRuntime::RDOResourceTrace::CS_Keep || relRes.at(i)->begin == rdoRuntime::RDOResourceTrace::CS_Erase || relRes.at(i)->begin == rdoRuntime::RDOResourceTrace::CS_NoChange ) {
					resSelectors.push_back( relRes.at(i)->createSelectResourceCommonChoiceCalc() );
				}
			}
			patRuntime->addChoiceFromCalc( new rdoRuntime::RDOSelectResourceCommonCalc( parser->runTime, resSelectors, useCommonWithMax, commonChoice->createCalc() ) );
		}
	} else {
		for ( int i = 0; i < size; i++ ) {
			rdoRuntime::RDOCalc* calc = relRes.at(i)->createSelectResourceChoiceCalc();
			patRuntime->addChoiceFromCalc( calc );
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPATPatternEvent
// ----------------------------------------------------------------------------
RDOPATPatternEvent::RDOPATPatternEvent( RDOParser* _parser, const RDOParserSrcInfo& _name_src_info, bool _trace ):
	RDOPATPattern( _parser, _name_src_info, _trace )
{ 
//	parser->runTime->addRuntimeIE((RDOIERuntime *)(patRuntime = new RDOIERuntime(parser->runTime, _trace))); 
	patRuntime = new rdoRuntime::RDOIERuntime( parser->runTime, _trace ); 
	patRuntime->setPatternId( parser->getPAT_id() );
}

void RDOPATPatternEvent::addRelRes( const std::string& relName, const std::string& typeName, rdoRuntime::RDOResourceTrace::ConvertStatus beg, const YYLTYPE& rel_pos, const YYLTYPE& type_pos, const YYLTYPE& convertor_pos )
{
	beforeRelRensert( relName, rel_pos );
	if ( beg == rdoRuntime::RDOResourceTrace::CS_NonExist || beg == rdoRuntime::RDOResourceTrace::CS_NoChange ) {
		parser->lexer_loc_set( convertor_pos.last_line, convertor_pos.last_column );
		parser->error( "Статусы конверторов NonExist и NoChange не могут быть использованы в нерегулярном событии" );
	}

	RDORelevantResource* relRes     = NULL;
	const RDORSSResource* const res = parser->findRSSResource( typeName );
	if ( res ) {
		if ( beg == rdoRuntime::RDOResourceTrace::CS_Create ) {
			parser->lexer_loc_set( type_pos.last_line, type_pos.last_column );
			parser->error( rdo::format("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)", relName.c_str(), typeName.c_str()) );
		}
		if ( beg == rdoRuntime::RDOResourceTrace::CS_Erase ) {
			parser->lexer_loc_set( convertor_pos.last_line, convertor_pos.last_column );
			parser->error( "Удалять ресурсы в нерегулярном событии нельзя" );
		}
		relRes = new RDORelevantResourceDirect( this, relName, rel_res_count(), res, beg );
		rel_res_insert( relRes, rel_pos );
	} else {
		const RDORTPResType* const type = parser->findRTPResType( typeName );
		if ( !type ) {
			parser->lexer_loc_set( type_pos.last_line, type_pos.last_column );
			parser->error( rdo::format("Неизвестный тип ресурса: %s", typeName.c_str()) );
//			parser->error( "Unknown resource name or type: " + *typeName );
		}
		if ( beg == rdoRuntime::RDOResourceTrace::CS_Create && !type->isTemporary() ) {
			parser->lexer_loc_set( type_pos.last_line, type_pos.last_column );
			parser->error( rdo::format("Тип ресурса '%s' постоянный. Динамически создавать от него ресурсы нельзя", typeName.c_str()) );
		}
		if ( beg == rdoRuntime::RDOResourceTrace::CS_Keep || beg == rdoRuntime::RDOResourceTrace::CS_Erase ) {
			parser->lexer_loc_set( convertor_pos.last_line, convertor_pos.last_column );
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
		parser->error( choice_from->src_info(), "Релевантные ресурсы нерегулярного события нельзя выбирать с помощью Choice from или Choice NoCheck" );
//		parser->error( "Unexpected choice for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"" );
	}
	if ( choice_order->type != rdoRuntime::RDOSelectResourceCalc::order_empty ) {
		parser->error( choice_from->src_info(), rdo::format("Для релевантных ресурсов нерегулярного события нельзя использовать правило выбора '%s'", choice_order->asString().c_str()) );
	}
	currRelRes->choice_from  = choice_from;
	currRelRes->choice_order = choice_order;
}

rdoRuntime::RDOCalc* RDOPATPattern::createRelRes( const RDOPATParamSet* const parSet, bool trace ) const
{
	std::vector< rdoRuntime::RDOValue > params_default;
	std::vector< const RDORTPParam* >::const_iterator it = currRelRes->getType()->getParams().begin();
	while ( it != currRelRes->getType()->getParams().end() ) {
		if ( !(*it)->getType()->dv->isExist() ) {
			params_default.push_back( rdoRuntime::RDOValue(0) );
			bool set_found = false;
			std::vector< RDOPATParamSet::param_set >::const_iterator set_it = parSet->params.begin();
			while ( set_it != parSet->params.end() ) {
				if ( (*it)->getName() == set_it->name && set_it->arithm ) {
					set_found = true;
					break;
				}
				set_it++;
			}
			if ( !set_found ) {
				parser->error( parSet->src_info(), rdo::format("При создании ресурса необходимо определить все его параметры. Не найдено определение параметра: %s", (*it)->getName().c_str()));
			}
		} else {
			params_default.push_back( (*it)->getType()->getParamDefaultValue() );
		}
		it++;
	}
	return new rdoRuntime::RDOCalcCreateEmptyResource( parser->runTime, currRelRes->getType()->getNumber(), trace, params_default, currRelRes->rel_res_id );
}

void RDOPATPatternEvent::addRelResConvert( bool trace, RDOPATParamSet* parSet, const YYLTYPE& convertor_pos, const YYLTYPE& trace_pos )
{
	if ( currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_NoChange || currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
		parser->lexer_loc_set( convertor_pos.last_line, convertor_pos.last_column );
		parser->error( rdo::format("Для релевантного ресурса '%s' не требуется конвертор, т.к. его статус: %s", currRelRes->getName().c_str(), RDOPATPattern::StatusToStr(currRelRes->begin).c_str()) );
	}

	if ( currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_Create ) {
		patRuntime->addBeginCalc( createRelRes( parSet, trace ) );
	} else {
		if ( trace ) {
			parser->lexer_loc_set( trace_pos.last_line, trace_pos.last_column );
			parser->error( "Признак трассировки в данном месте возможен только для создаваемого ресурса" );
		}
	}

	int size = parSet->params.size();
	if ( !size && currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_Keep ) {
		parser->lexer_loc_backup();
		parser->lexer_loc_set( convertor_pos.last_line, convertor_pos.last_column );
		parser->warning( rdo::format("Для релевантного ресурса '%s' указан пустой конвертор (Convert_event), хотя его статус: %s", currRelRes->getName().c_str(), RDOPATPattern::StatusToStr(currRelRes->begin).c_str()) );
		parser->lexer_loc_restore();
	}
	for ( int i = 0; i < size; i++ ) {
		int parNumb = parSet->params.at(i).index;
		RDOFUNArithm* currArithm = parSet->params.at(i).arithm;
		if ( currArithm ) { // if NULL - "NoChange" 
			rdoRuntime::RDOCalc* rightValue = currArithm->createCalc( currRelRes->getType()->getParams().at(parNumb)->getType() );
			rdoRuntime::RDOCalc* calc = new rdoRuntime::RDOSetRelParamCalc( parser->runTime, currRelRes->rel_res_id, parNumb, rightValue );
			calc->setSrcText( parSet->params.at(i).name + " set " + rightValue->src_text() );
			patRuntime->addBeginCalc( calc );
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPATPatternRule
// ----------------------------------------------------------------------------
RDOPATPatternRule::RDOPATPatternRule( RDOParser* _parser, const RDOParserSrcInfo& _name_src_info, bool _trace ):
	RDOPATPattern( _parser, _name_src_info, _trace )
{ 
//	parser->runTime->addRuntimeRule((RDORuleRuntime *)(patRuntime = new RDORuleRuntime(parser->runTime, _trace))); 
	patRuntime = new rdoRuntime::RDORuleRuntime( parser->runTime, _trace );
	patRuntime->setPatternId( parser->getPAT_id() );
}

void RDOPATPatternRule::testGoodForSearchActivity() const
{
	for ( std::vector< RDORelevantResource* >::const_iterator i = rel_res_begin(); i != rel_res_end(); i++ ) {
		if ( ((*i)->begin == rdoRuntime::RDOResourceTrace::CS_Create) || ((*i)->begin == rdoRuntime::RDOResourceTrace::CS_Erase) ) {
// TODO
			parser->error( "Rule: " + getName() + " Cannot be used in search activity because of bad converter status" );
		}
	}
}

void RDOPATPatternRule::setTime( RDOFUNArithm* arithm )
{ 
	parser->error( arithm->src_info(), "Поле $Time не используется в продукционном правиле" );
//	parser->error( "Rule must have no $Time field" );
}

void RDOPATPatternRule::addRelRes( const std::string& relName, const std::string& typeName, rdoRuntime::RDOResourceTrace::ConvertStatus beg, const YYLTYPE& rel_pos, const YYLTYPE& type_pos, const YYLTYPE& convertor_pos )
{
	beforeRelRensert( relName, rel_pos );
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
			parser->error( rdo::format("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)", relName.c_str(), typeName.c_str()) );
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
			parser->error( rdo::format("Неизвестный тип ресурса: %s", typeName.c_str()) );
//			parser->error( "Unknown resource name or type: " + *typeName );
		}
		if ( !type->isTemporary() && (beg == rdoRuntime::RDOResourceTrace::CS_Create || beg == rdoRuntime::RDOResourceTrace::CS_Erase ) ) {
			parser->lexer_loc_set( type_pos.last_line, type_pos.last_column );
			parser->error( rdo::format("Для создании или удалении ресурса его тип должен быть временным (temporary), а не постоянным (permanent), как у '%s'", typeName.c_str()) );
		}
		relRes = new RDORelevantResourceByType( this, relName, rel_res_count(), type, beg );
		rel_res_insert( relRes, rel_pos );
	}
	if ( relRes->begin == rdoRuntime::RDOResourceTrace::CS_Erase ) {
		patRuntime->addBeginEraseCalc( new rdoRuntime::RDOCalcEraseRes( parser->runTime, relRes->rel_res_id, relRes->getName() ) );
	}
}

void RDOPATPatternRule::addRelResConvert( bool trace, RDOPATParamSet* parSet, const YYLTYPE& convertor_pos, const YYLTYPE& trace_pos )
{
	if ( currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_NoChange || currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
		parser->lexer_loc_set( convertor_pos.last_line, convertor_pos.last_column );
		parser->error( rdo::format("Для релевантного ресурса '%s' не требуется конвертор, т.к. его статус: %s", currRelRes->getName().c_str(), RDOPATPattern::StatusToStr(currRelRes->begin).c_str()) );
	}

	if ( currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_Create ) {
		patRuntime->addBeginCalc( createRelRes( parSet, trace ) );
	} else {
		if ( trace ) {
			parser->lexer_loc_set( trace_pos.last_line, trace_pos.last_column );
			parser->error( "Признак трассировки в данном месте возможен только для создаваемого ресурса" );
		}
	}

	int size = parSet->params.size();
	if ( !size && currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_Keep ) {
		parser->lexer_loc_backup();
		parser->lexer_loc_set( convertor_pos.last_line, convertor_pos.last_column );
		parser->warning( rdo::format("Для релевантного ресурса '%s' указан пустой конвертор (Convert_rule), хотя его статус: %s", currRelRes->getName().c_str(), RDOPATPattern::StatusToStr(currRelRes->begin).c_str()) );
		parser->lexer_loc_restore();
	}
	for ( int i = 0; i < size; i++ ) {
		int parNumb = parSet->params.at(i).index;
		RDOFUNArithm* currArithm = parSet->params.at(i).arithm;
		if ( currArithm ) { // if NULL - "NoChange" 
			rdoRuntime::RDOCalc* rightValue = currArithm->createCalc( currRelRes->getType()->getParams().at(parNumb)->getType() );
			rdoRuntime::RDOCalc* calc = new rdoRuntime::RDOSetRelParamCalc( parser->runTime, currRelRes->rel_res_id, parNumb, rightValue );
			calc->setSrcText( parSet->params.at(i).name + " set " + rightValue->src_text() );
			patRuntime->addBeginCalc( calc );
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPATPatternOperation
// ----------------------------------------------------------------------------
RDOPATPatternOperation::RDOPATPatternOperation( RDOParser* _parser, const RDOParserSrcInfo& _name_src_info, bool _trace ):
	RDOPATPattern( _parser, _name_src_info, _trace )
{ 
//	parser->runTime->addRuntimeOperation((RDOOperationRuntime *)(patRuntime = new RDOOperationRuntime(parser->runTime, _trace)));
	patRuntime = new rdoRuntime::RDOOperationRuntime( parser->runTime, _trace );
	patRuntime->setPatternId( parser->getPAT_id() );
}

RDOPATPatternOperation::RDOPATPatternOperation( RDOParser* _parser, bool _trace, const RDOParserSrcInfo& _name_src_info ):
	RDOPATPattern( _parser, _name_src_info, _trace )
{
}

void RDOPATPatternOperation::rel_res_insert( RDORelevantResource* rel_res, const YYLTYPE& pos )
{
	RDOPATPattern::rel_res_insert( rel_res, pos );
	static_cast<rdoRuntime::RDOOperationRuntime*>(patRuntime)->addEndConvertStatus( rel_res->end );
}

void RDOPATPatternOperation::addRelRes( const std::string& relName, const std::string& typeName, rdoRuntime::RDOResourceTrace::ConvertStatus beg, const YYLTYPE& rel_pos, const YYLTYPE& type_pos, const YYLTYPE& convertor_pos )
{
	parser->lexer_loc_set( convertor_pos.last_line, convertor_pos.last_column );
	parser->error( "Внутренняя ошибка парсера" );
}

void RDOPATPatternOperation::addRelRes( const std::string& relName, const std::string& typeName, rdoRuntime::RDOResourceTrace::ConvertStatus beg, rdoRuntime::RDOResourceTrace::ConvertStatus end, const YYLTYPE& rel_pos, const YYLTYPE& type_pos, const YYLTYPE& convertor_begin_pos, const YYLTYPE& convertor_end_pos )
{
	beforeRelRensert( relName, rel_pos );
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
			parser->error( rdo::format("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)", relName.c_str(), typeName.c_str()) );
		}
		if ( end == rdoRuntime::RDOResourceTrace::CS_Create ) {
			parser->lexer_loc_set( convertor_end_pos.last_line, convertor_end_pos.last_column );
			parser->error( rdo::format("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)", relName.c_str(), typeName.c_str()) );
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
			parser->error( rdo::format("Неизвестный тип ресурса: %s", typeName.c_str()) );
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

void RDOPATPatternOperation::addRelResConvert( bool trace, RDOPATParamSet* parSet, const YYLTYPE& convertor_pos, const YYLTYPE& trace_pos )
{
	addRelResConvertBeginEnd( trace, parSet, false, NULL, convertor_pos, convertor_pos, trace_pos, trace_pos );
}

void RDOPATPatternOperation::addRelResConvertBeginEnd( bool trace_begin, RDOPATParamSet* parSet_begin, bool trace_end, RDOPATParamSet* parSet_end, const YYLTYPE& convertor_begin_pos, const YYLTYPE& convertor_end_pos, const YYLTYPE& trace_begin_pos, const YYLTYPE& trace_end_pos )
{
	if ( parSet_begin ) {
		if ( currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_NoChange || currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
			parser->lexer_loc_set( convertor_begin_pos.last_line, convertor_begin_pos.last_column );
			parser->error( rdo::format("Для релевантного ресурса '%s' не требуется конвертор начала (Convert_begin), т.к. его статус: %s", currRelRes->getName().c_str(), RDOPATPattern::StatusToStr(currRelRes->begin).c_str()) );
//			parser->error("Converter begin not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"");
		}
		if ( currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_Create ) {
			patRuntime->addBeginCalc( createRelRes( parSet_begin, trace_begin ) );
		} else {
			if ( trace_begin ) {
				parser->lexer_loc_set( trace_begin_pos.last_line, trace_begin_pos.last_column );
				parser->error( "Признак трассировки в данном месте возможен только для создаваемого ресурса" );
			}
		}

		int size = parSet_begin->params.size();
		if ( !size && currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_Keep ) {
			parser->lexer_loc_backup();
			parser->lexer_loc_set( convertor_begin_pos.last_line, convertor_begin_pos.last_column );
			parser->warning( rdo::format("Для релевантного ресурса '%s' указан пустой конвертор начала (Convert_begin), хотя его статус: %s", currRelRes->getName().c_str(), RDOPATPattern::StatusToStr(currRelRes->begin).c_str()) );
			parser->lexer_loc_restore();
		}
		for ( int i = 0; i < size; i++ ) {
			int parNumb = parSet_begin->params.at(i).index;
			RDOFUNArithm* currArithm = parSet_begin->params.at(i).arithm;
			if ( currArithm ) { // if NULL - "NoChange" 
				rdoRuntime::RDOCalc* rightValue = currArithm->createCalc( currRelRes->getType()->getParams().at(parNumb)->getType() );
				rdoRuntime::RDOCalc* calc = new rdoRuntime::RDOSetRelParamCalc( parser->runTime, currRelRes->rel_res_id, parNumb, rightValue );
				calc->setSrcText( parSet_begin->params.at(i).name + " set " + rightValue->src_text() );
				patRuntime->addBeginCalc( calc );
			}
		}
	}

	if ( parSet_end ) {
		if ( currRelRes->end == rdoRuntime::RDOResourceTrace::CS_NoChange || currRelRes->end == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
			parser->lexer_loc_set( convertor_end_pos.last_line, convertor_end_pos.last_column );
			parser->error( rdo::format("Для релевантного ресурса '%s' не требуется конвертор конца (Convert_end), т.к. его статус: %s", currRelRes->getName().c_str(), RDOPATPattern::StatusToStr(currRelRes->begin).c_str()) );
//			parser->error("Converter end not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"");
		}
		if ( currRelRes->end == rdoRuntime::RDOResourceTrace::CS_Create ) {
			static_cast<rdoRuntime::RDOOperationRuntime*>(patRuntime)->addEndCalc( createRelRes( parSet_end, trace_end ) );
		} else {
			if ( trace_end ) {
				parser->lexer_loc_set( trace_end_pos.last_line, trace_end_pos.last_column );
				parser->error( "Признак трассировки в данном месте возможен только для создаваемого ресурса" );
			}
		}

		int size = parSet_end->params.size();
		if ( !size && currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_Keep ) {
			parser->lexer_loc_backup();
			parser->lexer_loc_set( convertor_end_pos.last_line, convertor_end_pos.last_column );
			parser->warning( rdo::format("Для релевантного ресурса '%s' указан пустой конвертор конца (Convert_end), хотя его статус: %s", currRelRes->getName().c_str(), RDOPATPattern::StatusToStr(currRelRes->begin).c_str()) );
			parser->lexer_loc_restore();
		}
		for ( int i = 0; i < size; i++ ) {
			int parNumb = parSet_end->params.at( i ).index;
			RDOFUNArithm* currArithm = parSet_end->params.at( i ).arithm;
			if ( currArithm ) { // NULL == "NoChange" 
				rdoRuntime::RDOCalc* rightValue = currArithm->createCalc( currRelRes->getType()->getParams().at( parNumb )->getType() );
				rdoRuntime::RDOCalc* calc = new rdoRuntime::RDOSetRelParamCalc( parser->runTime, currRelRes->rel_res_id, parNumb, rightValue );
				calc->setSrcText( parSet_end->params.at(i).name + " set " + rightValue->src_text() );
				static_cast<rdoRuntime::RDOOperationRuntime*>(patRuntime)->addEndCalc( calc );
			}
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPATPatternKeyboard
// ----------------------------------------------------------------------------
RDOPATPatternKeyboard::RDOPATPatternKeyboard( RDOParser* _parser, const RDOParserSrcInfo& _name_src_info, bool _trace ):
	RDOPATPatternOperation( _parser, _trace, _name_src_info )
{
	patRuntime = new rdoRuntime::RDOKeyboardRuntime( parser->runTime, _trace ); 
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
	return choice_order ? choice_order->type : rdoRuntime::RDOSelectResourceCalc::order_empty;
}

RDOPATParamSet* RDORelevantResource::createParamSet()
{
	if ( !param_set_begin ) {
		param_set_begin = new RDOPATParamSet( this );
		return param_set_begin;
	} else if ( !param_set_end ) {
		param_set_end = new RDOPATParamSet( this );
		return param_set_end;
	}
	parser->error( src_info(), "Внутренняя ошибка парсера" );
	return NULL;
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

rdoRuntime::RDOCalc* RDORelevantResourceDirect::createSelectResourceChoiceCalc()
{
	rdoRuntime::RDOSelectResourceDirectCalc* relres_calc = new rdoRuntime::RDOSelectResourceDirectCalc( parser->runTime, rel_res_id, res->getNumber(), getChoiceCalc(), getSelectCalc(), getSelectType() );
	relres_calc->setSrcInfo( choice_from->src_info() );
	return relres_calc;
}

rdoRuntime::RDOCalc* RDORelevantResourceDirect::createSelectFirstResourceChoiceCalc()
{
	return new rdoRuntime::RDOSelectResourceDirectCalc( parser->runTime, rel_res_id, res->getNumber(), getChoiceCalc() );
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

rdoRuntime::RDOSelectResourceCommon* RDORelevantResourceByType::createSelectResourceCommonChoiceCalc()
{
	return new rdoRuntime::RDOSelectResourceByTypeCommonCalc( parser->runTime, rel_res_id, type->getNumber(), getChoiceCalc() );
}

// ----------------------------------------------------------------------------
// ---------- RDOPATParamSet - все операторы set для одного рел. ресурса
// ----------------------------------------------------------------------------
void RDOPATParamSet::addSet( const std::string& paramName, const YYLTYPE& param_name_pos, RDOFUNArithm* paramArithm )
{
	if ( params.empty() ) {
		setSrcText( paramName + (paramArithm ? " set " + paramArithm->src_text() : " NoChange") );
		setSrcPos( param_name_pos );
	} else {
		setSrcText( src_text() + "\n" + paramName + (paramArithm ? " set " + paramArithm->src_text() : " NoChange") );
	}
	const RDORTPParam* param = rel_res->getType()->findRTPParam( paramName );
	if ( !param ) {
		parser->lexer_loc_set( param_name_pos.last_line, param_name_pos.last_column );
		parser->error( rdo::format("Неизвестный параметр: %s", paramName.c_str()) );
	}
	if ( isExist(paramName) ) {
		parser->error( RDOParserSrcInfo(param_name_pos), rdo::format("Параметр '%s' уже используется", paramName.c_str()) );
//		parser->warning( RDOParserSrcInfo(param_name_pos), rdo::format("Параметр '%s' уже изменяется в конверторе. В трассировку попадет последнее значение параметра", paramName.c_str()) );
	}
	if ( paramArithm ) {
		param->getType()->checkParamType( paramArithm );
	}
	params.push_back( param_set( paramName, rel_res->getType()->getRTPParamNumber( paramName ), paramArithm ) );
}

} // namespace rdoParse

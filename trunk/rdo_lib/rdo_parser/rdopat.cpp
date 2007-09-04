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
	const RDOPATPattern* pattern = getParser()->findPattern( src_info().src_text() );
	if ( pattern ) {
		getParser()->error_push_only( src_info(), rdo::format("Паттерн '%s' уже существует", getName().c_str()) );
		getParser()->error_push_only( pattern->src_info(), "См. первое определение" );
		getParser()->error_push_done();
//		getParser()->error( "Pattern with name: " + name + " already exist" );
	}
	getParser()->insertPATPattern( this );
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
		getParser()->error( convertor_pos, rdo::format("Неверный статус конвертора: %s", value.c_str()) );
//		getParser()->error( "Wrong converter status: " + value );
	}
	return rdoRuntime::RDOResourceTrace::CS_Keep; // unreachable code
}

void RDOPATPattern::beforeRelRensert( const RDOParserSrcInfo& rel_info )
{
	if ( findRelevantResource( rel_info.src_text() ) ) {
		getParser()->error( rel_info, rdo::format("Релевантный ресурс '%s' уже определен", rel_info.src_text().c_str()) );
	}
}

void RDOPATPattern::rel_res_insert( RDORelevantResource* rel_res )
{
	patRuntime->addBeginConvertStatus( rel_res->begin );
	relRes.push_back( rel_res );
}

void RDOPATPattern::addRelResConvert( bool trace, RDOPATParamSet* parSet, const YYLTYPE& convertor_pos, const YYLTYPE& trace_pos )
{
	if ( parSet->convert_status == rdoRuntime::RDOResourceTrace::CS_NoChange || parSet->convert_status == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
		getParser()->error( convertor_pos, getErrorMessage_NotNeedConvertor(parSet) );
	}

	if ( parSet->convert_status == rdoRuntime::RDOResourceTrace::CS_Create ) {
		addParamSetCalc( parSet, createRelRes( parSet, trace ) );
	} else {
		if ( trace ) {
			getParser()->error( trace_pos, "Признак трассировки в данном месте возможен только для создаваемого ресурса" );
		}
	}

	int size = parSet->params.size();
	if ( !size && parSet->convert_status == rdoRuntime::RDOResourceTrace::CS_Keep ) {
		getParser()->warning( convertor_pos, getWarningMessage_EmptyConvertor(parSet) );
	}
	for ( int i = 0; i < size; i++ ) {
		int parNumb = parSet->params.at(i).index;
		RDOFUNArithm* currArithm = parSet->params.at(i).arithm;
		if ( currArithm ) { // NULL == NoChange
			const RDORTPParam* param = parSet->getRelRes()->getType()->getParams().at(parNumb);
			rdoRuntime::RDOCalc* rightValue = currArithm->createCalc( param->getType() );
			rdoRuntime::RDOCalc* calc = NULL;
			switch ( param->getType()->getType() ) {
				case RDORTPParamType::pt_int: {
					const RDORTPIntParamType* param_type = static_cast<const RDORTPIntParamType*>(param->getType());
					if ( param_type->diap->isExist() ) {
						calc = new rdoRuntime::RDOSetRelParamDiapCalc( getParser()->runtime, parSet->getRelRes()->rel_res_id, parNumb, rightValue, param_type->diap->min_value, param_type->diap->max_value );
						break;
					}
				}
				case RDORTPParamType::pt_real: {
					const RDORTPRealParamType* param_type = static_cast<const RDORTPRealParamType*>(param->getType());
					if ( param_type->diap->isExist() ) {
						calc = new rdoRuntime::RDOSetRelParamDiapCalc( getParser()->runtime, parSet->getRelRes()->rel_res_id, parNumb, rightValue, param_type->diap->min_value, param_type->diap->max_value );
						break;
					}
				}
			}
			if ( !calc ) {
				calc = new rdoRuntime::RDOSetRelParamCalc( getParser()->runtime, parSet->getRelRes()->rel_res_id, parNumb, rightValue );
			}
			calc->setSrcText( parSet->params.at(i).name + " set " + rightValue->src_text() );
			addParamSetCalc( parSet, calc );
		}
	}
}

void RDOPATPattern::addParamSetCalc( const RDOPATParamSet* const parSet, rdoRuntime::RDOCalc* calc )
{
	patRuntime->addBeginCalc( calc );
}

void RDOPATPattern::testGoodForSearchActivity() const
{
	getParser()->error( "Только продукционные правила могут быть использованы в точке принятия решений" );
//	getParser()->error("Only RULEs can be used in search activity");
}

void RDOPATPattern::testGoodForSomeActivity() const
{
	getParser()->error( "Только продукционные правила и операции могут быть использованы в точке типа some" );
//	getParser()->error("Only RULEs and OPERATIONs can be used in some activity");
}

std::string RDOPATPattern::getPatternId() const
{ 
	return patRuntime->getPatternId(); 
}

int RDOPATPattern::writeModelStructure() const
{
	getParser()->modelStructure << getPatternId() << " " << getName() << " " << getModelStructureLetter() << " " << relRes.size();
	for ( int i = 0; i < relRes.size(); i++ ) {
		getParser()->modelStructure << " " << relRes.at(i)->getType()->getNumber();
	}
	getParser()->modelStructure << std::endl;
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
		getParser()->error_push_only( _param->src_info(), rdo::format("Параметр '%s' уже определен", _param->src_text().c_str()) );
		getParser()->error_push_only( param->src_info(), "См. первое определение" );
		getParser()->error_push_done();
//		getParser()->error("Second appearance of the same parameter name: " + _param->getName());
	}
	params.push_back( _param ); 
}

void RDOPATPattern::setCommonChoiceFirst()
{
	useCommonChoice = true;
	commonChoice    = NULL;
	getParser()->error( "Вызывать нельзя, т.к. в сообщениях об ошибках используется commonChoice" );
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
		getParser()->error( body_name.src_info(), rdo::format("Неизвестный релевантный ресурс: %s", body_name.src_text().c_str()) );
//		getParser()->error( "Name of relevant resource expected instead of: " + body_name.src_text() );
	}
	if ( findRelevantResourceNum( body_name.src_text() ) != current_rel_res_index ) {
		std::string rel_res_waiting = current_rel_res_index < relRes.size() ? relRes[current_rel_res_index]->getName().c_str() : "";
		getParser()->error( body_name.src_info(), rdo::format("Ожидается описание релевантного ресурса '%s', вместо него найдено: %s", rel_res_waiting.c_str(), body_name.src_text().c_str()) );
	}
	if ( (*it)->alreadyHaveConverter ) {
		getParser()->error( body_name.src_info(), rdo::format("Релевантный ресурс уже используется: %s", body_name.src_text().c_str()) );
//		getParser()->error( "\"" + body_name.src_text() + "\" relevant resource has converter block already" );
	}
	currRelRes = (*it);
	currRelRes->body_name = body_name;
	currRelRes->alreadyHaveConverter = true;
	current_rel_res_index++;
}

void RDOPATPattern::addRelResUsage( RDOPATChoiceFrom* choice_from, RDOPATChoiceOrder* choice_order )
{
	if ( !useCommonChoice ) {
		if ( choice_order->type == rdoRuntime::RDOSelectResourceCalc::order_empty ) {
			if ( (currRelRes->begin != rdoRuntime::RDOResourceTrace::CS_Create) && (currRelRes->end != rdoRuntime::RDOResourceTrace::CS_Create) ) {
//				choice_order->type = rdoRuntime::RDOSelectResourceCalc::order_first;
			}
		} else if ( currRelRes->isDirect() ) {
			getParser()->warning( choice_order->src_info(), rdo::format("Правило выбора '%s' релевантного ресурса '%s' не имеет смысла, т.к. релевантный ресурс определен через имя, а не тип, и не может быть связан с каким-либо другим ресурсом",choice_order->src_text().c_str(), currRelRes->getName().c_str()) );
		}
	} else {
		if ( choice_order->type != rdoRuntime::RDOSelectResourceCalc::order_empty ) {
			getParser()->error_push_only( choice_order->src_info(), "Нельзя указать способ выбора релевантного ресурса, т.к. используется единый для всех релевантных ресурсов способ, указанный до ключевого слова $Body" );
			getParser()->error_push_only( commonChoice->src_info(), rdo::format("См. '%s'", commonChoice->src_text().c_str()) );
			getParser()->error_push_done();
//			getParser()->error( "Cannot use both common choice and choice for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"" );
		}
	}

	if ( (currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_Create) || (currRelRes->end == rdoRuntime::RDOResourceTrace::CS_Create) ) {
		if ( choice_from->type != RDOPATChoiceFrom::ch_empty ) {
			getParser()->error( choice_from->src_info(), "Релевантный ресурс создается, для него нельзя использовать Choice from или Choice NoCheck" );
//			getParser()->error( "Cannot use choice when create \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"" );
		}
		if ( choice_order->type != rdoRuntime::RDOSelectResourceCalc::order_empty ) {
			getParser()->error( choice_from->src_info(), rdo::format("Релевантный ресурс создается, для него нельзя использовать правило выбора '%s'", choice_order->asString().c_str()) );
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
			// TODO: А почему нельзя сделать warning ? Возможно, есть жесткое требование недопустить пустого рел. ресурса.
			getParser()->error( currRelRes->src_info(), rdo::format("Релевантный ресурс '%s' не используется в образце '%s'", currRelRes->getName().c_str(), getName().c_str()) );
		}
		patRuntime->addChoiceFromCalc( currRelRes->createSelectEmptyResourceCalc() );
	}

	if ( useCommonChoice ) {
		// first
		// Работает неправильно, а как обыкновенный first
//		getParser()->error("RDOPATPattern::end not implemented yet for not \"first\" conditions in common choice");
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
			patRuntime->addChoiceFromCalc( new rdoRuntime::RDOSelectResourceCommonCalc( getParser()->runtime, resSelectors, useCommonWithMax, commonChoice->createCalc() ) );
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
//	getParser()->runtime->addRuntimeIE((RDOIERuntime *)(patRuntime = new RDOIERuntime(getParser()->runtime, _trace))); 
	patRuntime = new rdoRuntime::RDOIERuntime( getParser()->runtime, _trace ); 
	patRuntime->setPatternId( getParser()->getPAT_id() );
}

void RDOPATPatternEvent::addRelRes( const RDOParserSrcInfo& rel_info, const RDOParserSrcInfo& type_info, rdoRuntime::RDOResourceTrace::ConvertStatus beg, const YYLTYPE& convertor_pos )
{
	beforeRelRensert( rel_info );
	if ( beg == rdoRuntime::RDOResourceTrace::CS_NonExist || beg == rdoRuntime::RDOResourceTrace::CS_NoChange ) {
		getParser()->error( convertor_pos, "Статусы конверторов NonExist и NoChange не могут быть использованы в нерегулярном событии" );
	}

	RDORelevantResource* relRes     = NULL;
	const RDORSSResource* const res = getParser()->findRSSResource( type_info.src_text() );
	if ( res ) {
		if ( beg == rdoRuntime::RDOResourceTrace::CS_Create ) {
			getParser()->error( type_info, rdo::format("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)", rel_info.src_text().c_str(), type_info.src_text().c_str()) );
		}
		if ( beg == rdoRuntime::RDOResourceTrace::CS_Erase ) {
			getParser()->error( convertor_pos, "Удалять ресурсы в нерегулярном событии нельзя" );
		}
		relRes = new RDORelevantResourceDirect( this, rel_info.src_text(), rel_res_count(), res, beg );
		rel_res_insert( relRes );
	} else {
		const RDORTPResType* const type = getParser()->findRTPResType( type_info.src_text() );
		if ( !type ) {
			getParser()->error( type_info, rdo::format("Неизвестный тип ресурса: %s", type_info.src_text().c_str()) );
//			getParser()->error( "Unknown resource name or type: " + *type_info.src_text() );
		}
		if ( beg == rdoRuntime::RDOResourceTrace::CS_Create && !type->isTemporary() ) {
			getParser()->error( type_info, rdo::format("Тип ресурса '%s' постоянный. Динамически создавать от него ресурсы нельзя", type_info.src_text().c_str()) );
		}
		if ( beg == rdoRuntime::RDOResourceTrace::CS_Keep || beg == rdoRuntime::RDOResourceTrace::CS_Erase ) {
			getParser()->error( convertor_pos, "Статусы конверторов Keep и Erase могут быть использованы в нерегулярном событии с описателем в виде ресурса, но не типа ресурса" );
//			getParser()->error( "Can use Keep and Erase status with resource name only (not with resource type) in irregular event" );
		}
		relRes = new RDORelevantResourceByType( this, rel_info.src_text(), rel_res_count(), type, beg );
		rel_res_insert( relRes );
	}
	if ( relRes->begin == rdoRuntime::RDOResourceTrace::CS_Erase ) {
		patRuntime->addBeginEraseCalc( new rdoRuntime::RDOCalcEraseRes( getParser()->runtime, relRes->rel_res_id, relRes->getName() ) );
	}
}

void RDOPATPatternEvent::addRelResUsage( RDOPATChoiceFrom* choice_from, RDOPATChoiceOrder* choice_order )
{
	if ( choice_from->type != RDOPATChoiceFrom::ch_empty ) {
		getParser()->error( choice_from->src_info(), "Релевантные ресурсы нерегулярного события нельзя выбирать с помощью Choice from или Choice NoCheck" );
//		getParser()->error( "Unexpected choice for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"" );
	}
	if ( choice_order->type != rdoRuntime::RDOSelectResourceCalc::order_empty ) {
		getParser()->error( choice_from->src_info(), rdo::format("Для релевантных ресурсов нерегулярного события нельзя использовать правило выбора '%s'", choice_order->asString().c_str()) );
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
				getParser()->error( parSet->src_info(), rdo::format("При создании ресурса необходимо определить все его параметры. Не найдено определение параметра: %s", (*it)->getName().c_str()));
			}
		} else {
			params_default.push_back( (*it)->getType()->getParamDefaultValue( (*it)->getType()->dv->src_info() ) );
		}
		it++;
	}
	return new rdoRuntime::RDOCalcCreateEmptyResource( getParser()->runtime, currRelRes->getType()->getNumber(), trace, params_default, currRelRes->rel_res_id );
}

std::string RDOPATPatternEvent::getErrorMessage_NotNeedConvertor( const RDOPATParamSet* const parSet )
{
	return rdo::format("Для релевантного ресурса '%s' не требуется конвертор (Convert_event), т.к. его статус: %s", parSet->getRelRes()->getName().c_str(), RDOPATPattern::StatusToStr(parSet->convert_status).c_str());
}

std::string RDOPATPatternEvent::getWarningMessage_EmptyConvertor( const RDOPATParamSet* const parSet )
{
	return rdo::format("Для релевантного ресурса '%s' указан пустой конвертор (Convert_event), хотя его статус: %s", parSet->getRelRes()->getName().c_str(), RDOPATPattern::StatusToStr(parSet->convert_status).c_str());
}

// ----------------------------------------------------------------------------
// ---------- RDOPATPatternRule
// ----------------------------------------------------------------------------
RDOPATPatternRule::RDOPATPatternRule( RDOParser* _parser, const RDOParserSrcInfo& _name_src_info, bool _trace ):
	RDOPATPattern( _parser, _name_src_info, _trace )
{ 
//	getParser()->runtime->addRuntimeRule((RDORuleRuntime *)(patRuntime = new RDORuleRuntime(getParser()->runtime, _trace))); 
	patRuntime = new rdoRuntime::RDORuleRuntime( getParser()->runtime, _trace );
	patRuntime->setPatternId( getParser()->getPAT_id() );
}

void RDOPATPatternRule::testGoodForSearchActivity() const
{
	for ( std::vector< RDORelevantResource* >::const_iterator i = rel_res_begin(); i != rel_res_end(); i++ ) {
		if ( ((*i)->begin == rdoRuntime::RDOResourceTrace::CS_Create) || ((*i)->begin == rdoRuntime::RDOResourceTrace::CS_Erase) ) {
// TODO
			getParser()->error( "Rule: " + getName() + " Cannot be used in search activity because of bad converter status" );
		}
	}
}

void RDOPATPatternRule::setTime( RDOFUNArithm* arithm )
{ 
	getParser()->error( arithm->src_info(), "Поле $Time не используется в продукционном правиле" );
//	getParser()->error( "Rule must have no $Time field" );
}

void RDOPATPatternRule::addRelRes( const RDOParserSrcInfo& rel_info, const RDOParserSrcInfo& type_info, rdoRuntime::RDOResourceTrace::ConvertStatus beg, const YYLTYPE& convertor_pos )
{
	beforeRelRensert( rel_info );
	if ( beg == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
		getParser()->error( convertor_pos, rdo::format("Нельзя использовать статус конвертора '%s' в продукционном правиле", RDOPATPattern::StatusToStr(beg).c_str()) );
//		getParser()->error( "Cannot use NonExist status in rule" );
	}

	RDORelevantResource* relRes     = NULL;
	const RDORSSResource* const res = getParser()->findRSSResource( type_info.src_text() );
	if ( res ) {
		if ( beg == rdoRuntime::RDOResourceTrace::CS_Create ) {
			getParser()->error( type_info, rdo::format("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)", rel_info.src_text().c_str(), type_info.src_text().c_str()) );
//			getParser()->error( "Cannot use Create status for resource, only for resource type" );
		}
		if ( beg == rdoRuntime::RDOResourceTrace::CS_Erase ) {
			getParser()->error( convertor_pos, rdo::format("Недопустимый статус конвертора для ресурса: %s", RDOPATPattern::StatusToStr(beg).c_str()) );
//			getParser()->error( "Wrong converter status for resource of permanent type" );
		}
		relRes = new RDORelevantResourceDirect( this, rel_info.src_text(), rel_res_count(), res, beg );
		rel_res_insert( relRes );

	} else {
		const RDORTPResType* const type = getParser()->findRTPResType( type_info.src_text() );
		if ( !type ) {
			getParser()->error( type_info, rdo::format("Неизвестный тип ресурса: %s", type_info.src_text().c_str()) );
//			getParser()->error( "Unknown resource name or type: " + *type_info.src_text() );
		}
		if ( !type->isTemporary() && (beg == rdoRuntime::RDOResourceTrace::CS_Create || beg == rdoRuntime::RDOResourceTrace::CS_Erase ) ) {
			getParser()->error( type_info, rdo::format("Для создании или удалении ресурса его тип должен быть временным (temporary), а не постоянным (permanent), как у '%s'", type_info.src_text().c_str()) );
		}
		relRes = new RDORelevantResourceByType( this, rel_info.src_text(), rel_res_count(), type, beg );
		rel_res_insert( relRes );
	}
	if ( relRes->begin == rdoRuntime::RDOResourceTrace::CS_Erase ) {
		patRuntime->addBeginEraseCalc( new rdoRuntime::RDOCalcEraseRes( getParser()->runtime, relRes->rel_res_id, relRes->getName() ) );
	}
}

std::string RDOPATPatternRule::getErrorMessage_NotNeedConvertor( const RDOPATParamSet* const parSet )
{
	return rdo::format("Для релевантного ресурса '%s' не требуется конвертор (Convert_rule), т.к. его статус: %s", parSet->getRelRes()->getName().c_str(), RDOPATPattern::StatusToStr(parSet->convert_status).c_str());
}

std::string RDOPATPatternRule::getWarningMessage_EmptyConvertor( const RDOPATParamSet* const parSet )
{
	return rdo::format("Для релевантного ресурса '%s' указан пустой конвертор (Convert_rule), хотя его статус: %s", parSet->getRelRes()->getName().c_str(), RDOPATPattern::StatusToStr(parSet->convert_status).c_str());
}

// ----------------------------------------------------------------------------
// ---------- RDOPATPatternOperation
// ----------------------------------------------------------------------------
RDOPATPatternOperation::RDOPATPatternOperation( RDOParser* _parser, const RDOParserSrcInfo& _name_src_info, bool _trace ):
	RDOPATPattern( _parser, _name_src_info, _trace )
{ 
//	getParser()->runtime->addRuntimeOperation((RDOOperationRuntime *)(patRuntime = new RDOOperationRuntime(getParser()->runtime, _trace)));
	patRuntime = new rdoRuntime::RDOOperationRuntime( getParser()->runtime, _trace );
	patRuntime->setPatternId( getParser()->getPAT_id() );
}

RDOPATPatternOperation::RDOPATPatternOperation( RDOParser* _parser, bool _trace, const RDOParserSrcInfo& _name_src_info ):
	RDOPATPattern( _parser, _name_src_info, _trace )
{
}

void RDOPATPatternOperation::rel_res_insert( RDORelevantResource* rel_res )
{
	RDOPATPattern::rel_res_insert( rel_res );
	static_cast<rdoRuntime::RDOOperationRuntime*>(patRuntime)->addEndConvertStatus( rel_res->end );
}

void RDOPATPatternOperation::addRelRes( const RDOParserSrcInfo& rel_info, const RDOParserSrcInfo& type_info, rdoRuntime::RDOResourceTrace::ConvertStatus beg, const YYLTYPE& convertor_pos )
{
	getParser()->error( convertor_pos, "Внутренняя ошибка парсера" );
}

void RDOPATPatternOperation::addRelRes( const RDOParserSrcInfo& rel_info, const RDOParserSrcInfo& type_info, rdoRuntime::RDOResourceTrace::ConvertStatus beg, rdoRuntime::RDOResourceTrace::ConvertStatus end, const YYLTYPE& convertor_begin_pos, const YYLTYPE& convertor_end_pos )
{
	beforeRelRensert( rel_info );
	switch ( beg ) {
		case rdoRuntime::RDOResourceTrace::CS_Keep:
			if ( end != rdoRuntime::RDOResourceTrace::CS_Keep && end != rdoRuntime::RDOResourceTrace::CS_Erase && end != rdoRuntime::RDOResourceTrace::CS_NoChange ) {
				getParser()->error( convertor_end_pos, rdo::format("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()) );
//				getParser()->error( "Wrong second converter status" );
			}
			break;
		case rdoRuntime::RDOResourceTrace::CS_Create:
			if ( end != rdoRuntime::RDOResourceTrace::CS_Keep && end != rdoRuntime::RDOResourceTrace::CS_Erase && end != rdoRuntime::RDOResourceTrace::CS_NoChange ) {
				getParser()->error( convertor_end_pos, rdo::format("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()) );
//				getParser()->error( "Wrong second converter status" );
			}
			break;
		case rdoRuntime::RDOResourceTrace::CS_Erase:
			if ( end != rdoRuntime::RDOResourceTrace::CS_NonExist ) {
				getParser()->error( convertor_end_pos, rdo::format("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()) );
//				getParser()->error( "Wrong second converter status" );
			}
			break;
		case rdoRuntime::RDOResourceTrace::CS_NonExist:
			if ( end != rdoRuntime::RDOResourceTrace::CS_Create ) {
				getParser()->error( convertor_end_pos, rdo::format("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()) );
//				getParser()->error( "Wrong second converter status" );
			}
			break;
		case rdoRuntime::RDOResourceTrace::CS_NoChange:
			if ( end != rdoRuntime::RDOResourceTrace::CS_Keep && end != rdoRuntime::RDOResourceTrace::CS_Erase && end != rdoRuntime::RDOResourceTrace::CS_NoChange ) {
				getParser()->error( convertor_end_pos, rdo::format("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()) );
//				getParser()->error( "Wrong second converter status" );
			}
			break;
	};

	RDORelevantResource* relRes     = NULL;
	const RDORSSResource* const res = getParser()->findRSSResource( type_info.src_text() );
	if ( res ) {
		if ( beg == rdoRuntime::RDOResourceTrace::CS_Create ) {
			getParser()->error( convertor_begin_pos, rdo::format("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)", rel_info.src_text().c_str(), type_info.src_text().c_str()) );
		}
		if ( end == rdoRuntime::RDOResourceTrace::CS_Create ) {
			getParser()->error( convertor_end_pos, rdo::format("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)", rel_info.src_text().c_str(), type_info.src_text().c_str()) );
		}
		if ( beg == rdoRuntime::RDOResourceTrace::CS_Erase || beg == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
			getParser()->error( convertor_begin_pos, rdo::format("Недопустимый статус конвертора начала для ресурса: %s", RDOPATPattern::StatusToStr(beg).c_str()) );
		}
		if ( end == rdoRuntime::RDOResourceTrace::CS_Erase || end == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
			getParser()->error( convertor_end_pos, rdo::format("Недопустимый статус конвертора конца для ресурса: %s", RDOPATPattern::StatusToStr(end).c_str()) );
		}
		relRes = new RDORelevantResourceDirect( this, rel_info.src_text(), rel_res_count(), res, beg, end );
		rel_res_insert( relRes );

	} else {
		const RDORTPResType* const type = getParser()->findRTPResType( type_info.src_text() );
		if ( !type ) {
			getParser()->error( type_info, rdo::format("Неизвестный тип ресурса: %s", type_info.src_text().c_str()) );
//			getParser()->error( "Unknown resource name or type: " + *type_info.src_text() );
		}
		if ( type->isPermanent() ) {
			if ( beg == rdoRuntime::RDOResourceTrace::CS_Create || beg == rdoRuntime::RDOResourceTrace::CS_Erase || beg == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
				getParser()->error( convertor_begin_pos, rdo::format("Недопустимый статус конвертора начала для постоянного типа: %s", RDOPATPattern::StatusToStr(beg).c_str()) );
			}
			if ( end == rdoRuntime::RDOResourceTrace::CS_Create || end == rdoRuntime::RDOResourceTrace::CS_Erase || end == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
				getParser()->error( convertor_end_pos, rdo::format("Недопустимый статус конвертора конца для постоянного типа: %s", RDOPATPattern::StatusToStr(end).c_str()) );
			}
		}
		relRes = new RDORelevantResourceByType( this, rel_info.src_text(), rel_res_count(), type, beg, end );
		rel_res_insert( relRes );
	}
	if ( relRes->begin == rdoRuntime::RDOResourceTrace::CS_Erase ) {
		patRuntime->addBeginEraseCalc( new rdoRuntime::RDOCalcEraseRes( getParser()->runtime, relRes->rel_res_id, relRes->getName() ) );
	}
	if ( relRes->end == rdoRuntime::RDOResourceTrace::CS_Erase ) {
		static_cast<rdoRuntime::RDOOperationRuntime*>(patRuntime)->addEndEraseCalc( new rdoRuntime::RDOCalcEraseRes( getParser()->runtime, relRes->rel_res_id, relRes->getName() ) );
	}
}

void RDOPATPatternOperation::addRelResConvertBeginEnd( bool trace_begin, RDOPATParamSet* parSet_begin, bool trace_end, RDOPATParamSet* parSet_end, const YYLTYPE& convertor_begin_pos, const YYLTYPE& convertor_end_pos, const YYLTYPE& trace_begin_pos, const YYLTYPE& trace_end_pos )
{
	if ( parSet_begin ) {
		addRelResConvert( trace_begin, parSet_begin, convertor_begin_pos, trace_begin_pos );
	}
	if ( parSet_end ) {
		addRelResConvert( trace_end, parSet_end, convertor_end_pos, trace_end_pos );
	}
}

void RDOPATPatternOperation::addParamSetCalc( const RDOPATParamSet* const parSet, rdoRuntime::RDOCalc* calc )
{
	switch ( parSet->getRelRes()->getConvertorType(parSet) ) {
		case convert_begin: patRuntime->addBeginCalc( calc ); return;
		case convert_end  : static_cast<rdoRuntime::RDOOperationRuntime*>(patRuntime)->addEndCalc( calc ); return;
	}
	getParser()->error( "Внутренняя ошибка парсера" );
}

std::string RDOPATPatternOperation::getErrorMessage_NotNeedConvertor( const RDOPATParamSet* const parSet )
{
	switch ( parSet->getRelRes()->getConvertorType(parSet) ) {
		case convert_begin: return rdo::format("Для релевантного ресурса '%s' не требуется конвертор начала (Convert_begin), т.к. его статус: %s", parSet->getRelRes()->getName().c_str(), RDOPATPattern::StatusToStr(parSet->convert_status).c_str()); break;
		case convert_end  : return rdo::format("Для релевантного ресурса '%s' не требуется конвертор конца (Convert_end), т.к. его статус: %s", parSet->getRelRes()->getName().c_str(), RDOPATPattern::StatusToStr(parSet->convert_status).c_str()); break;
	}
	return "Внутренняя ошибка парсера";
}

std::string RDOPATPatternOperation::getWarningMessage_EmptyConvertor( const RDOPATParamSet* const parSet )
{
	switch ( parSet->getRelRes()->getConvertorType(parSet) ) {
		case convert_begin: return rdo::format("Для релевантного ресурса '%s' указан пустой конвертор начала (Convert_begin), хотя его статус: %s", parSet->getRelRes()->getName().c_str(), RDOPATPattern::StatusToStr(parSet->convert_status).c_str()); break;
		case convert_end  : return rdo::format("Для релевантного ресурса '%s' указан пустой конвертор конца (Convert_end), хотя его статус: %s", parSet->getRelRes()->getName().c_str(), RDOPATPattern::StatusToStr(parSet->convert_status).c_str()); break;
	}
	return "Внутренняя ошибка парсера";
}

// ----------------------------------------------------------------------------
// ---------- RDOPATPatternKeyboard
// ----------------------------------------------------------------------------
RDOPATPatternKeyboard::RDOPATPatternKeyboard( RDOParser* _parser, const RDOParserSrcInfo& _name_src_info, bool _trace ):
	RDOPATPatternOperation( _parser, _trace, _name_src_info )
{
	patRuntime = new rdoRuntime::RDOKeyboardRuntime( getParser()->runtime, _trace ); 
	patRuntime->setPatternId( getParser()->getPAT_id() );
}

// ----------------------------------------------------------------------------
// ---------- RDORelevantResource
// ----------------------------------------------------------------------------
RDOPATParamSet* RDORelevantResource::createParamSet()
{
	if ( !param_set_begin ) {
		param_set_begin = new RDOPATParamSet( this, begin );
		return param_set_begin;
	} else if ( !param_set_end ) {
		param_set_end = new RDOPATParamSet( this, end );
		return param_set_end;
	}
	getParser()->error( src_info(), "Внутренняя ошибка парсера" );
	return NULL;
}

void RDORelevantResource::deleteParamSetBegin()
{
	if ( param_set_begin ) {
		delete param_set_begin;
		param_set_begin = NULL;
	}
}

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

// ----------------------------------------------------------------------------
// ---------- RDORelevantResourceDirect - по имени ресурса
// ----------------------------------------------------------------------------
rdoRuntime::RDOCalc* RDORelevantResourceDirect::createSelectEmptyResourceCalc()
{
	rdoRuntime::RDOSelectResourceDirectCalc* calc = new rdoRuntime::RDOSelectResourceDirectCalc( getParser()->runtime, rel_res_id, res->getNumber(), NULL, NULL );
	calc->setSrcInfo( src_info() );
	calc->setSrcText( "Предварительный выбор рел. ресурса " + calc->src_text() );
	return calc;
}

rdoRuntime::RDOCalc* RDORelevantResourceDirect::createSelectResourceChoiceCalc()
{
//	rdoRuntime::RDOSelectResourceDirectCalc* calc = new rdoRuntime::RDOSelectResourceDirectCalc( getParser()->runtime, rel_res_id, res->getNumber(), getChoiceCalc(), NULL, rdoRuntime::RDOSelectResourceCalc::order_empty );
	rdoRuntime::RDOSelectResourceDirectCalc* calc = new rdoRuntime::RDOSelectResourceDirectCalc( getParser()->runtime, rel_res_id, res->getNumber(), getChoiceCalc(), getSelectCalc(), getSelectType() );
	calc->setSrcInfo( choice_from->src_info() );
	return calc;
}

rdoRuntime::RDOCalc* RDORelevantResourceDirect::createSelectFirstResourceChoiceCalc()
{
	return new rdoRuntime::RDOSelectResourceDirectCalc( getParser()->runtime, rel_res_id, res->getNumber(), getChoiceCalc() );
}

rdoRuntime::RDOSelectResourceCommon* RDORelevantResourceDirect::createSelectResourceCommonChoiceCalc()
{
	return new rdoRuntime::RDOSelectResourceDirectCommonCalc( getParser()->runtime, rel_res_id, res->getNumber(), getChoiceCalc() );
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
		rdoRuntime::RDOSelectResourceByTypeCalc* calc = new rdoRuntime::RDOSelectResourceByTypeCalc( getParser()->runtime, rel_res_id, type->getNumber(), NULL, NULL );
		calc->setSrcInfo( src_info() );
		calc->setSrcText( "Предварительный выбор рел. ресурса " + calc->src_text() );
		return calc;
	} else {
		if ( begin == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
			return new rdoRuntime::RDOSelectResourceNonExistCalc( getParser()->runtime, rel_res_id );
		} else {
			rdoRuntime::RDOCalcConst* calc = new rdoRuntime::RDOCalcConst( getParser()->runtime, 1 );
			calc->setSrcInfo( src_info() );
			calc->setSrcText( "Предварительный выбор рел. ресурса перед созданием " + calc->src_text() );
			return calc;
		}
	}
}

rdoRuntime::RDOCalc* RDORelevantResourceByType::createSelectResourceChoiceCalc()
{
	if ( (begin != rdoRuntime::RDOResourceTrace::CS_Create) && (end != rdoRuntime::RDOResourceTrace::CS_Create) ) {
		rdoRuntime::RDOSelectResourceByTypeCalc* calc = new rdoRuntime::RDOSelectResourceByTypeCalc( getParser()->runtime, rel_res_id, type->getNumber(), getChoiceCalc(), getSelectCalc(), getSelectType() );
		calc->setSrcInfo( choice_from->src_info() );
		return calc;
	} else {
		if ( begin == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
			return new rdoRuntime::RDOSelectResourceNonExistCalc( getParser()->runtime, rel_res_id );
		} else {
			rdoRuntime::RDOCalcConst* calc = new rdoRuntime::RDOCalcConst( getParser()->runtime, 1 );
			calc->setSrcInfo( src_info() );
			calc->setSrcText( "Перед созданием рел. ресурса " + calc->src_text() );
			return calc;
		}
	}
}

rdoRuntime::RDOCalc* RDORelevantResourceByType::createSelectFirstResourceChoiceCalc()
{
	if ( (begin != rdoRuntime::RDOResourceTrace::CS_Create) && (end != rdoRuntime::RDOResourceTrace::CS_Create) ) {
		return new rdoRuntime::RDOSelectResourceByTypeCalc( getParser()->runtime, rel_res_id, type->getNumber(), getChoiceCalc() );
	} else {
		if ( begin == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
			return new rdoRuntime::RDOSelectResourceNonExistCalc( getParser()->runtime, rel_res_id );
		} else {
			return new rdoRuntime::RDOCalcConst( getParser()->runtime, 1 );
		}
	}
}

rdoRuntime::RDOSelectResourceCommon* RDORelevantResourceByType::createSelectResourceCommonChoiceCalc()
{
	return new rdoRuntime::RDOSelectResourceByTypeCommonCalc( getParser()->runtime, rel_res_id, type->getNumber(), getChoiceCalc() );
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
	const RDORTPParam* param = getRelRes()->getType()->findRTPParam( paramName );
	if ( !param ) {
		getParser()->error( param_name_pos, rdo::format("Неизвестный параметр: %s", paramName.c_str()) );
	}
	if ( isExist(paramName) ) {
		getParser()->error( RDOParserSrcInfo(param_name_pos), rdo::format("Параметр '%s' уже используется", paramName.c_str()) );
//		getParser()->warning( RDOParserSrcInfo(param_name_pos), rdo::format("Параметр '%s' уже изменяется в конверторе. В трассировку попадет последнее значение параметра", paramName.c_str()) );
	}
	if ( paramArithm ) {
		param->getType()->checkParamType( paramArithm );
	}
	params.push_back( param_set( paramName, getRelRes()->getType()->getRTPParamNumber( paramName ), paramArithm ) );
}

} // namespace rdoParse

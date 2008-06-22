#include "pch.h"
#include "rdofun.h"
#include "rdoparser.h"
#include "rdortp.h"
#include "rdorss.h"
#include "rdopat.h"
#include "rdofrm.h"
#include "rdo_random_distribution.h"
#include "rdodpt.h"
#include "rdoparser_lexer.h"
#include <rdo_runtime.h>
#include <rdoframe.h>
#include <rdocalc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoParse 
{

int funlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void funerror( char* mes )
{
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNDoubleToIntByResult
// ----------------------------------------------------------------------------
void RDOFUNDoubleToIntByResult::initCalc( bool round )
{
	std::vector< rdoRuntime::RDOCalcDoubleToIntByResult* >::iterator it = m_int_or_double.begin();
	while ( it != m_int_or_double.end() ) {
		(*it)->needRound();
		it++;
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNLogic
// ----------------------------------------------------------------------------
RDOFUNLogic::RDOFUNLogic( const RDOParserObject* _parent, rdoRuntime::RDOCalc* _calc, bool hide_warning ):
	RDOParserObject( _parent ),
	RDOParserSrcInfo(),
	m_calc( _calc )
{
	if ( m_calc ) {
		m_calc->setSrcFileType( src_filetype() );
	}
	if ( !hide_warning ) {
		rdoRuntime::RDOCalcConst* calc_const = dynamic_cast<rdoRuntime::RDOCalcConst*>(m_calc);
		if ( calc_const ) {
			if ( calc_const->calcValue( parser()->runtime() ).getBool() ) {
				parser()->warning( calc_const->src_info(), rdo::format("Логическое выражение всегда истинно: %s", calc_const->src_text().c_str()) );
			} else {
				parser()->warning( calc_const->src_info(), rdo::format("Логическое выражение всегда ложно: %s", calc_const->src_text().c_str()) );
			}
		}
	}
}

rdoRuntime::RDOCalc* RDOFUNLogic::createCalc( rdoRuntime::RDOType::ID _id )
{
	if ( _id != rdoRuntime::RDOType::t_real ) {
		m_int_or_double.initCalc( true );
	}
	return m_calc;
}

RDOFUNLogic* RDOFUNLogic::operator &&( const RDOFUNLogic& second )
{
	rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<rdoRuntime::RDOCalcConst*>(m_calc);
	rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<rdoRuntime::RDOCalcConst*>(second.m_calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser()->runtime(), calc1->calcValue( parser()->runtime() ).getBool() && calc2->calcValue( parser()->runtime() ).getBool() );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcAnd::getStaticSrcInfo( calc1, calc2 ) );
	} else {
		newCalc = new rdoRuntime::RDOCalcAnd( parser()->runtime(), m_calc, second.m_calc );
	}
	RDOFUNLogic* logic = new RDOFUNLogic( this, newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	logic->m_int_or_double.insert( m_int_or_double, second.m_int_or_double );
	return logic;
}

RDOFUNLogic* RDOFUNLogic::operator ||( const RDOFUNLogic& second )
{
	rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<rdoRuntime::RDOCalcConst*>(m_calc);
	rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<rdoRuntime::RDOCalcConst*>(second.m_calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser()->runtime(), calc1->calcValue( parser()->runtime() ).getBool() || calc2->calcValue( parser()->runtime() ).getBool() );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcOr::getStaticSrcInfo( calc1, calc2 ) );
	} else {
		newCalc = new rdoRuntime::RDOCalcOr( parser()->runtime(), m_calc, second.m_calc );
	}
	RDOFUNLogic* logic = new RDOFUNLogic( this, newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	logic->m_int_or_double.insert( m_int_or_double, second.m_int_or_double );
	return logic;
}

RDOFUNLogic* RDOFUNLogic::operator_not()
{
	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcNot( parser()->runtime(), m_calc );
	RDOFUNLogic* logic = new RDOFUNLogic( this, newCalc );
	logic->m_int_or_double.insert( m_int_or_double );
	return logic;
}

void RDOFUNLogic::setSrcInfo( const RDOParserSrcInfo& src_info )
{
	RDOParserSrcInfo::setSrcInfo( src_info );
	if ( m_calc ) m_calc->setSrcInfo( src_info );
}

void RDOFUNLogic::setSrcPos( const RDOSrcInfo::Position& _pos )
{
	RDOParserSrcInfo::setSrcPos( _pos );
	if ( m_calc ) m_calc->setSrcPos( _pos );
}

void RDOFUNLogic::setSrcText( const std::string& value )
{
	RDOParserSrcInfo::setSrcText( value );
	if ( m_calc ) m_calc->setSrcText( value );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNArithm
// ----------------------------------------------------------------------------
RDOFUNArithm::RDOFUNArithm( RDOParser* parser, rdoRuntime::RDOType::ID typeID, rdoRuntime::RDOCalc* calc, const RDOParserSrcInfo& src_info ):
	RDOParserObject( parser ),
	m_typeID( typeID ),
	enu( NULL ),
	str( "" ),
	m_calc( calc )
{
	setSrcInfo( src_info );
}

RDOFUNArithm::RDOFUNArithm( const RDOParserObject* parent, rdoRuntime::RDOType::ID typeID, rdoRuntime::RDOCalc* calc, const RDOParserSrcInfo& src_info ):
	RDOParserObject( parent ),
	m_typeID( typeID ),
	enu( NULL ),
	str( "" ),
	m_calc( calc )
{
	setSrcInfo( src_info );
}

RDOFUNArithm::RDOFUNArithm( RDOParser* parser, const RDOParserSrcInfo& res_name_src_info, const RDOParserSrcInfo& par_name_src_info ):
	RDOParserObject( parser ),
	m_typeID( rdoRuntime::RDOType::t_int ),
	enu( NULL ),
	str( "" ),
	m_calc( NULL )
{
	init( res_name_src_info, par_name_src_info );
}

RDOFUNArithm::RDOFUNArithm( const RDOFUNArithm* parent, const RDOParserSrcInfo& res_name_src_info, const RDOParserSrcInfo& par_name_src_info ):
	RDOParserObject( parent ),
	m_typeID( rdoRuntime::RDOType::t_int ),
	enu( NULL ),
	str( "" ),
	m_calc( NULL )
{
	init( res_name_src_info, par_name_src_info );
}

void RDOFUNArithm::init( const RDOParserSrcInfo& res_name_src_info, const RDOParserSrcInfo& par_name_src_info )
{
	setSrcInfo( res_name_src_info, ".", par_name_src_info );
	const RDORSSResource* const res = parser()->findRSSResource( res_name_src_info.src_text() ); 
	if ( res ) {
		// Это ресурс с закладки RSS
		int resNumb = res->getNumber();
		int parNumb = res->getType()->getRTPParamNumber( par_name_src_info.src_text() );
		if ( parNumb == -1 ) {
			parser()->error( par_name_src_info, rdo::format("Неизвестный параметр ресурса: %s", par_name_src_info.src_text().c_str()) );
		}
		if ( res->getType()->isPermanent() ) {
			m_calc = new rdoRuntime::RDOCalcGetResParam( parser()->runtime(), resNumb, parNumb );
		} else if ( res->getType()->isTemporary() && parser()->getFileToParse() == rdoModelObjects::FRM ) {
			m_calc = new rdoRuntime::RDOCalcGetTempResParamFRM( parser()->runtime(), resNumb, parNumb );
		} else {
			parser()->error( res_name_src_info, rdo::format("Нельзя использовать временный ресурс: %s", res_name_src_info.src_text().c_str()) );
//			parser()->error(("Cannot use temporary resource in function: " + *res_name_src_info.src_text()).c_str());
		}
		m_calc->setSrcInfo( src_info() );
		m_typeID = res->getType()->findRTPParam( par_name_src_info.src_text() )->getType()->typeID();
		if ( m_typeID == rdoRuntime::RDOType::t_enum ) {
			enu = ((RDORTPEnumParamType *)res->getType()->findRTPParam( par_name_src_info.src_text() )->getType())->m_enum;
		}
		return;
	}
	// Это не ресурс, но возможно релевантный ресурс или ресурс, внутри групповой функции
	else if ( !parser()->getFUNGroupStack().empty() && parser()->getFUNGroupStack().back()->resType->name() == res_name_src_info.src_text() )
	{
		// Это ресурс внутри групповой функции
		RDOFUNGroup* currGroup = parser()->getFUNGroupStack().back();
		int parNumb = currGroup->resType->getRTPParamNumber( par_name_src_info.src_text() );
		if ( parNumb == -1 ) {
			parser()->error( par_name_src_info, rdo::format("Неизвестный параметр ресурса: %s", par_name_src_info.src_text().c_str()) );
		}
		m_calc = new rdoRuntime::RDOCalcGetGroupResParam( parser()->runtime(), parNumb );
		m_calc->setSrcInfo( src_info() );
		m_typeID = currGroup->resType->findRTPParam( par_name_src_info.src_text() )->getType()->typeID();
		if ( m_typeID == rdoRuntime::RDOType::t_enum ) {
			enu = ((RDORTPEnumParamType *)currGroup->resType->findRTPParam( par_name_src_info.src_text() )->getType())->m_enum;
		}
		return;
	}
	else
	{
		switch ( parser()->getFileToParse() )
		{
			case rdoModelObjects::PAT:
			{
				if ( parser()->getLastPATPattern() && parser()->getLastPATPattern()->findRelevantResource( res_name_src_info.src_text() ) )
				{
					// Это релевантный ресурс где-то в паттерне (with_min-common-choice, $Time, $Body)
					RDOPATPattern* pat = parser()->getLastPATPattern();
					const RDORelevantResource* const rel = pat->findRelevantResource( res_name_src_info.src_text() );
					if ( !pat->currRelRes ) {
						// Внутри with_min-common-choice или $Time
						if ( rel->begin == rdoRuntime::RDOResource::CS_NonExist || rel->begin == rdoRuntime::RDOResource::CS_Create ) {
							parser()->error( res_name_src_info, rdo::format("Релевантный ресурс не может быть использован, т.к. он еще не существует: %s", rel->name().c_str()) );
						}
					} else {
						// Внутри $Body
						// Проверяем использование неинициализированного рел.ресурса (rel) в Choice from другом рел.ресурсе (pat->currRelRes)
						if ( pat->currRelRes->isChoiceFromState() ) {
							if ( !rel->alreadyHaveConverter && !rel->isDirect() ) {
								parser()->error( res_name_src_info, rdo::format("Релевантный ресурс неопределен: %s. Его нельзя использовать в условиях выбора других ресурсов до его собственного Choice from", rel->name().c_str()) );
							}
							if ( rel->begin == rdoRuntime::RDOResource::CS_NonExist ) {
								parser()->error( res_name_src_info, rdo::format("Релевантный ресурс в начале операции не существует (NonExist): %s", rel->name().c_str()) );
							}
							if ( rel->begin == rdoRuntime::RDOResource::CS_Create ) {
								parser()->error( res_name_src_info, rdo::format("Сразу после создания (Create) релевантный ресурс '%s' можно использовать только в конверторах, но не в условии выбора", rel->name().c_str()) );
							}
						}
						// Проверяем использование временного рел.ресурса внутри конвертора другого рел.ресурса
						if ( rel->getType()->isTemporary() ) {
							// В конверторе начала
							if ( pat->currRelRes->currentState == RDORelevantResource::convertBegin ) {
								if ( rel->begin == rdoRuntime::RDOResource::CS_Create && !rel->alreadyHaveConverter ) {
									parser()->error( res_name_src_info, rdo::format("Релевантный ресурс нельзя использовать до его создания (Create): %s", rel->name().c_str()) );
								}
								if ( rel->begin == rdoRuntime::RDOResource::CS_Erase && rel->alreadyHaveConverter ) {
									parser()->error( res_name_src_info, rdo::format("Релевантный ресурс нельзя использовать после удаления (Erase): %s", rel->name().c_str()) );
								}
								if ( rel->begin == rdoRuntime::RDOResource::CS_NonExist ) {
									parser()->error( res_name_src_info, rdo::format("Релевантный ресурс не существует в этом конверторе (NonExist): %s", rel->name().c_str()) );
								}
							}
							// В конверторе конца
							if ( pat->currRelRes->currentState == RDORelevantResource::convertEnd ) {
								if ( rel->end == rdoRuntime::RDOResource::CS_Create && !rel->alreadyHaveConverter ) {
									parser()->error( res_name_src_info, rdo::format("Релевантный ресурс нельзя использовать до его создания (Create): %s", rel->name().c_str()) );
								}
								if ( rel->end == rdoRuntime::RDOResource::CS_Erase && rel->alreadyHaveConverter ) {
									parser()->error( res_name_src_info, rdo::format("Релевантный ресурс нельзя использовать после удаления (Erase): %s", rel->name().c_str()) );
								}
								if ( rel->end == rdoRuntime::RDOResource::CS_NonExist ) {
									parser()->error( res_name_src_info, rdo::format("Релевантный ресурс не существует в этом конверторе (NonExist): %s", rel->name().c_str()) );
								}
							}
						}
						// Проверяем использование еще не инициализированного (только для Create) параметра рел. ресурса в его же конверторе
						const RDORTPParam* param = pat->currRelRes->getType()->findRTPParam( par_name_src_info.src_text() );
						if ( param && pat->currRelRes->name() == res_name_src_info.src_text() ) {
							// В конверторе начала
							if ( pat->currRelRes->currentState == RDORelevantResource::convertBegin && pat->currRelRes->begin == rdoRuntime::RDOResource::CS_Create) {
								if ( !pat->currRelRes->getParamSetBegin()->isExist( par_name_src_info.src_text() ) ) {
									if ( !param->getType()->getDV().isExist() ) {
										parser()->error( par_name_src_info, rdo::format("Параметр '%s' еще не определен, ему необходимо присвоить значение в текущем конверторе или указать значение по-умолчанию в типе ресурса", par_name_src_info.src_text().c_str()) );
									}
								}
							}
							// В конверторе начала
							if ( pat->currRelRes->currentState == RDORelevantResource::convertEnd && pat->currRelRes->end == rdoRuntime::RDOResource::CS_Create) {
								if ( !pat->currRelRes->getParamSetEnd()->isExist( par_name_src_info.src_text() ) ) {
									if ( !param->getType()->getDV().isExist() ) {
										parser()->error( par_name_src_info, rdo::format("Параметр '%s' еще не определен, ему необходимо присвоить значение в текущем конверторе или указать значение по-умолчанию в типе ресурса", par_name_src_info.src_text().c_str()) );
									}
								}
							}
						}
					}
					int relResNumb = pat->findRelevantResourceNum( res_name_src_info.src_text() );
					int parNumb    = rel->getType()->getRTPParamNumber( par_name_src_info.src_text() );
					if ( parNumb == -1 ) {
						parser()->error( par_name_src_info, rdo::format("Неизвестный параметр ресурса: %s", par_name_src_info.src_text().c_str()) );
//							parser()->error( "Unknown resource parameter: " + *par_name_src_info.src_text() );
					}
					m_calc = new rdoRuntime::RDOCalcGetRelevantResParam( parser()->runtime(), relResNumb, parNumb );
					m_calc->setSrcInfo( src_info() );
					m_typeID = rel->getType()->findRTPParam( par_name_src_info.src_text() )->getType()->typeID();
					if ( m_typeID == rdoRuntime::RDOType::t_enum ) {
						enu = ((RDORTPEnumParamType*)rel->getType()->findRTPParam( par_name_src_info.src_text() )->getType())->m_enum;
					}
					return;
				}
				break;
			}
			case rdoModelObjects::DPT:
			{
				if ( parser()->isCurrentDPTSearch() && parser()->getLastDPTSearch()->getLastActivity() )
				{
					const RDOPATPattern* rule = parser()->getLastDPTSearch()->getLastActivity()->pattern();
					if ( rule && rule->findRelevantResource( res_name_src_info.src_text() ) ) {
						// Это ресурс, который используется в DPT (condition, term_condition, evaluate_by, value before, value after)
						const RDORelevantResource* const rel = rule->findRelevantResource( res_name_src_info.src_text() );
						int relResNumb = rule->findRelevantResourceNum( res_name_src_info.src_text() );
						int parNumb    = rel->getType()->getRTPParamNumber( par_name_src_info.src_text() );
						if ( parNumb == -1 ) {
							parser()->error( par_name_src_info, rdo::format("Неизвестный параметр ресурса: %s", par_name_src_info.src_text().c_str()) );
						}

						m_calc = new rdoRuntime::RDOCalcGetRelevantResParam( parser()->runtime(), relResNumb, parNumb );
						m_calc->setSrcInfo( src_info() );
						m_typeID = rel->getType()->findRTPParam( par_name_src_info.src_text() )->getType()->typeID();
						if ( m_typeID == rdoRuntime::RDOType::t_enum ) {
							enu = ((RDORTPEnumParamType *)rel->getType()->findRTPParam( par_name_src_info.src_text() )->getType())->m_enum;
						}
						return;
					}
				}
				break;
			}
			case rdoModelObjects::FRM:
			{
				if ( parser()->getLastFRMFrame() && parser()->getLastFRMFrame()->frame()->getLastShow() && parser()->getLastFRMFrame()->frame()->getLastShow()->isShowIf() )
				{
					m_calc = new rdoRuntime::RDOCalcGetUnknowResParam( parser()->runtime(), res_name_src_info.src_text(), par_name_src_info.src_text() );
					m_calc->setSrcInfo( src_info() );
					return;
				}
				break;
			}
		}
	}
	parser()->error( res_name_src_info, rdo::format("Неизвестный ресурс: %s", res_name_src_info.src_text().c_str()) );
//	parser()->error(("Unknown resource name: " + *res_name_src_info.src_text()).c_str());
}

RDOFUNArithm::RDOFUNArithm( RDOParser* _parser, int value, const RDOParserSrcInfo& src_info ):
	RDOParserObject( _parser ),
	m_typeID( rdoRuntime::RDOType::t_int ),
	enu( NULL ),
	str( "" ),
	m_calc( NULL )
{
	m_calc = new rdoRuntime::RDOCalcConst( parser()->runtime(), value );
	setSrcInfo( src_info );
}

RDOFUNArithm::RDOFUNArithm( const RDOFUNArithm* parent, int value, const RDOParserSrcInfo& src_info ):
	RDOParserObject( parent ),
	m_typeID( rdoRuntime::RDOType::t_int ),
	enu( NULL ),
	str( "" ),
	m_calc( NULL )
{
	m_calc = new rdoRuntime::RDOCalcConst( this->parser()->runtime(), value );
	setSrcInfo( src_info );
}

RDOFUNArithm::RDOFUNArithm( RDOParser* parser, double value, const RDOParserSrcInfo& src_info ):
	RDOParserObject( parser ),
	m_typeID( rdoRuntime::RDOType::t_real ),
	enu( NULL ),
	str( "" ),
	m_calc( NULL )
{
	m_calc = new rdoRuntime::RDOCalcConst( this->parser()->runtime(), value );
	setSrcInfo( src_info );
}

RDOFUNArithm::RDOFUNArithm( const RDOFUNArithm* parent, double value, const RDOParserSrcInfo& src_info ):
	RDOParserObject( parent ),
	m_typeID( rdoRuntime::RDOType::t_real ),
	enu( NULL ),
	str( "" ),
	m_calc( NULL )
{
	m_calc = new rdoRuntime::RDOCalcConst( this->parser()->runtime(), value );
	setSrcInfo( src_info );
}

RDOFUNArithm::RDOFUNArithm( RDOParser* parser, const std::string& value, const YYLTYPE& pos ):
	RDOParserObject( parser ),
	m_typeID( rdoRuntime::RDOType::t_int ),
	enu( NULL ),
	str( "" ),
	m_calc( NULL )
{
	init( value, pos );
}

RDOFUNArithm::RDOFUNArithm( const RDOFUNArithm* parent, const std::string& value, const YYLTYPE& pos ):
	RDOParserObject( parent ),
	m_typeID( rdoRuntime::RDOType::t_int ),
	enu( NULL ),
	str( "" ),
	m_calc( NULL )
{
	init( value, pos );
}

void RDOFUNArithm::init( const std::string& value, const YYLTYPE& pos )
{
	setSrcPos( pos );
	setSrcText( value );

	if ( value == "Time_now" || value == "time_now" || value == "Системное_время" || value == "системное_время" ) {
		m_typeID = rdoRuntime::RDOType::t_real;
		m_calc = new rdoRuntime::RDOCalcGetTimeNow( parser()->runtime() );
		m_calc->setSrcInfo( src_info() );
		return;
	}

	if ( value == "Seconds" || value == "seconds" ) {
		m_typeID = rdoRuntime::RDOType::t_real;
		m_calc = new rdoRuntime::RDOCalcGetSeconds( parser()->runtime() );
		m_calc->setSrcInfo( src_info() );
		return;
	}

	// Ищем параметры паттерна или функции по имени
	const RDOFUNFunctionParam* param = NULL;
	switch ( parser()->getFileToParse() ) {
		case rdoModelObjects::PAT: param = parser()->getLastPATPattern()->findPATPatternParam( value ); break;
		case rdoModelObjects::FUN: param = parser()->getLastFUNFunction()->findFUNFunctionParam( value ); break;
	}

	// Ищем константы по имени
	const RDOFUNConstant* cons = parser()->findFUNConstant( value );

	if ( cons && param ) {
		parser()->error( src_info(), rdo::format("Имя параметра образца совпадает с именем константы: %s", value.c_str()) );
//		parser()->error( "Ambiguity: constant or parameter usage: " + *value + " ?" );
	}

	if ( cons ) {
		m_typeID = cons->getType()->typeID();
		if ( m_typeID == rdoRuntime::RDOType::t_enum ) {
			enu = ((RDORTPEnumParamType *)cons->getType())->m_enum;
		}
		m_calc = new rdoRuntime::RDOCalcGetConst( parser()->runtime(), cons->getNumber() );
		m_calc->setSrcInfo( src_info() );
		return;
	}

	// Ищем последовательность по имени
	const RDOFUNSequence* seq = parser()->findFUNSequence( value );
	if ( seq && param ) {
		parser()->error( src_info(), rdo::format("Имя параметра образца совпадает с именем последовательности: %s", value.c_str()) );
//		parser()->error("Ambiguity: sequence or parameter usage: " + *value + " ?");
	}

	if ( seq ) {
		RDOFUNParams tmp( seq );
		RDOFUNArithm* arithm = tmp.createSeqCall( value );
		arithm->setSrcInfo( src_info() );
		m_typeID = arithm->typeID();
		if ( m_typeID == rdoRuntime::RDOType::t_enum ) {
			enu = arithm->enu;
		}
		m_calc = arithm->m_calc;
		return;
	}

	if ( param ) {
		// Это параметр
		m_typeID = param->getType()->typeID();
		if ( m_typeID == rdoRuntime::RDOType::t_enum ) {
			enu = ((RDORTPEnumParamType *)param->getType())->m_enum;
		}
		switch ( parser()->getFileToParse() ) {
			case rdoModelObjects::PAT: m_calc = new rdoRuntime::RDOCalcPatParam( parser()->runtime(), parser()->getLastPATPattern()->findPATPatternParamNum( value ) ); break;
			case rdoModelObjects::FUN: m_calc = new rdoRuntime::RDOCalcFuncParam( parser()->runtime(), parser()->getLastFUNFunction()->findFUNFunctionParamNum( value ), param->src_info() ); break;
		}
		if ( m_calc ) m_calc->setSrcInfo( src_info() );
		return;
	} else {
		// Возможно, что это значение перечислимого типа
		std::vector< RDORTPEnumParamType* >::const_iterator it = parser()->getRTPEnumParamTypes().begin();
		while ( it != parser()->getRTPEnumParamTypes().end() ) {
			if ( (*it)->m_enum->getEnums().findEnum( value ) != rdoRuntime::RDOEnumType::END ) {
				// Да, это перечислимый тип, только одно и тоже значение может встречаться в разных
				// перечислимых типах, поэтому какой именно из них выбрать - вопрос
				m_typeID = rdoRuntime::RDOType::t_unknow;
				str  = value;
				setSrcText( value );
				return;
			}
			it++;
		}
		parser()->error( pos, rdo::format("Неизвестный идентификатор: %s", value.c_str()) );
//		m_typeID = rdoRuntime::RDOType::t_unknow;
//		str  = value;
//		return;
	}
}

RDOFUNArithm* RDOFUNArithm::operator +( RDOFUNArithm& second )
{
	rdoRuntime::RDOType::ID newTypeID;

	if ( m_typeID == rdoRuntime::RDOType::t_int && second.m_typeID == rdoRuntime::RDOType::t_int ) {
		newTypeID = rdoRuntime::RDOType::t_int;
	} else if ( m_typeID == rdoRuntime::RDOType::t_enum || second.m_typeID == rdoRuntime::RDOType::t_enum ) {
		parser()->error( second, "Нельзя складывать перечислимые данные" );
//		parser()->error("cannot add enumerative types");
	} else if ( m_typeID == rdoRuntime::RDOType::t_unknow || second.m_typeID == rdoRuntime::RDOType::t_unknow ) {
		if ( m_typeID == rdoRuntime::RDOType::t_unknow ) {
			parser()->error( src_info(), rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser()->error( second.src_info(), rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	} else {
		newTypeID = rdoRuntime::RDOType::t_real;
	}
	rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<rdoRuntime::RDOCalcConst*>(m_calc);
	rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<rdoRuntime::RDOCalcConst*>(second.m_calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser()->runtime(), calc1->calcValue( parser()->runtime() ) + calc2->calcValue( parser()->runtime() ) );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcPlus::getStaticSrcInfo(calc1, calc2) );
	} else {
		newCalc = new rdoRuntime::RDOCalcPlus( parser()->runtime(), m_calc, second.m_calc );
	}
	RDOFUNArithm* arithm = new RDOFUNArithm( this, newTypeID, newCalc, newCalc->src_info() );
	arithm->m_int_or_double.insert( m_int_or_double, second.m_int_or_double );
	return arithm;
}

RDOFUNArithm* RDOFUNArithm::operator -( RDOFUNArithm& second )
{
	rdoRuntime::RDOType::ID newTypeID;

	if ( m_typeID == rdoRuntime::RDOType::t_int && second.m_typeID == rdoRuntime::RDOType::t_int ) {
		newTypeID = rdoRuntime::RDOType::t_int;
	} else if ( m_typeID == rdoRuntime::RDOType::t_enum || second.m_typeID == rdoRuntime::RDOType::t_enum ) {
		parser()->error( second.src_info(), "Нельзя вычитать перечислимые данные" );
//		parser()->error("cannot subtract enumerative types");
	} else if ( m_typeID == rdoRuntime::RDOType::t_unknow || second.m_typeID == rdoRuntime::RDOType::t_unknow ) {
		if ( m_typeID == rdoRuntime::RDOType::t_unknow ) {
			parser()->error( src_info(), rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser()->error( second.src_info(), rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	} else {
		newTypeID = rdoRuntime::RDOType::t_real;
	}
	rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<rdoRuntime::RDOCalcConst*>(m_calc);
	rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<rdoRuntime::RDOCalcConst*>(second.m_calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser()->runtime(), calc1->calcValue( parser()->runtime() ) - calc2->calcValue( parser()->runtime() ) );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcMinus::getStaticSrcInfo(calc1, calc2) );
	} else {
		newCalc = new rdoRuntime::RDOCalcMinus( parser()->runtime(), m_calc, second.m_calc );
	}
	RDOFUNArithm* arithm = new RDOFUNArithm( this, newTypeID, newCalc, newCalc->src_info() );
	arithm->m_int_or_double.insert( m_int_or_double, second.m_int_or_double );
	return arithm;
}

RDOFUNArithm* RDOFUNArithm::operator *( RDOFUNArithm& second )
{
	rdoRuntime::RDOType::ID newTypeID;

	if ( m_typeID == rdoRuntime::RDOType::t_int && second.m_typeID == rdoRuntime::RDOType::t_int ) {
		newTypeID = rdoRuntime::RDOType::t_int;
	} else if ( m_typeID == rdoRuntime::RDOType::t_enum || second.m_typeID == rdoRuntime::RDOType::t_enum ) {
		parser()->error( second.src_info(), "Нельзя перемножать перечислимые данные" );
//		parser()->error("cannot multiply enumerative types");
	} else if ( m_typeID == rdoRuntime::RDOType::t_unknow || second.m_typeID == rdoRuntime::RDOType::t_unknow ) {
		if ( m_typeID == rdoRuntime::RDOType::t_unknow ) {
			parser()->error( src_info(), rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser()->error( second.src_info(), rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	} else {
		newTypeID = rdoRuntime::RDOType::t_real;
	}
/*
	const rdoRuntime::RDOCalcConst* calc1 = NULL;
	rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<rdoRuntime::RDOCalcConst*>(second.m_calc);
	bool  right = false;
	if ( calc2 ) {
		calc1 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(m_calc);
		if ( !calc1 && dynamic_cast<const rdoRuntime::RDOCalcBinary*>(m_calc) ) {
			calc1 = dynamic_cast<const rdoRuntime::RDOCalcBinary*>(m_calc)->getRightAsConst();
			right = calc1 ? true : false;
		}
	}
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser()->runtime(), calc1->calcValue( parser()->runtime() ) * calc2->calcValue( parser()->runtime() ) );
		if ( right ) {
			dynamic_cast<rdoRuntime::RDOCalcBinary*>(m_calc)->setRight( newCalc );
			return this;
		}
	} else {
		newCalc = new rdoRuntime::RDOCalcMult( parser()->runtime(), m_calc, second.m_calc );
	}
*/
	rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<rdoRuntime::RDOCalcConst*>(m_calc);
	rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<rdoRuntime::RDOCalcConst*>(second.m_calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser()->runtime(), calc1->calcValue( parser()->runtime() ) * calc2->calcValue( parser()->runtime() ) );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcMult::getStaticSrcInfo(calc1, calc2) );
	} else {
		newCalc = new rdoRuntime::RDOCalcMult( parser()->runtime(), m_calc, second.m_calc );
	}
	RDOFUNArithm* arithm = new RDOFUNArithm( this, newTypeID, newCalc, newCalc->src_info() );
	arithm->m_int_or_double.insert( m_int_or_double, second.m_int_or_double );
	return arithm;
}

RDOFUNArithm* RDOFUNArithm::operator /( RDOFUNArithm& second )
{
	rdoRuntime::RDOType::ID newTypeID = rdoRuntime::RDOType::t_real;

	if ( m_typeID == rdoRuntime::RDOType::t_int && second.m_typeID == rdoRuntime::RDOType::t_int ) {
		newTypeID = rdoRuntime::RDOType::t_int;
	} else if ( m_typeID == rdoRuntime::RDOType::t_enum || second.m_typeID == rdoRuntime::RDOType::t_enum ) {
		parser()->error( second.src_info(), "Нельзя разделить перечислимые данные" );
//		parser()->error("cannot divide enumerative types");
	} else if ( m_typeID == rdoRuntime::RDOType::t_unknow || second.m_typeID == rdoRuntime::RDOType::t_unknow ) {
		if ( m_typeID == rdoRuntime::RDOType::t_unknow ) {
			parser()->error( src_info(), rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser()->error( second.src_info(), rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	} else {
		newTypeID = rdoRuntime::RDOType::t_real;
	}
	rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<rdoRuntime::RDOCalcConst*>(m_calc);
	rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<rdoRuntime::RDOCalcConst*>(second.m_calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser()->runtime(), calc1->calcValue( parser()->runtime() ).getDouble() / calc2->calcValue( parser()->runtime() ).getDouble() );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcDiv::getStaticSrcInfo(calc1, calc2) );
	} else {
		newCalc = new rdoRuntime::RDOCalcDiv( parser()->runtime(), m_calc, second.m_calc );
	}
	// TODO: перевод вещественного в целое при делении. А что делать с умножением и т.д. ?
	// Ответ: с умножением надо делать тоже самое, только непонятно как
	if ( newTypeID == rdoRuntime::RDOType::t_int ) {
		rdoRuntime::RDOCalc* newCalc_div = newCalc;
		newCalc = new rdoRuntime::RDOCalcDoubleToIntByResult( parser()->runtime(), newCalc_div );
		newCalc->setSrcInfo( newCalc_div->src_info() );
	}
	RDOFUNArithm* arithm = new RDOFUNArithm( this, newTypeID, newCalc, newCalc->src_info() );
	if ( newTypeID == rdoRuntime::RDOType::t_int ) {
		arithm->m_int_or_double.push_back( reinterpret_cast<rdoRuntime::RDOCalcDoubleToIntByResult*>(newCalc) );
	}
	arithm->m_int_or_double.insert( m_int_or_double, second.m_int_or_double );
	return arithm;
}

RDOFUNLogic* RDOFUNArithm::operator <( RDOFUNArithm& second )
{
	if ( m_typeID == rdoRuntime::RDOType::t_enum || second.m_typeID == rdoRuntime::RDOType::t_enum ) {
		parser()->error( second.src_info(), "Нельзя сравнивать перечислимые данные" );
//		parser()->error("cannot compare enumerative types");
	} else if ( m_typeID == rdoRuntime::RDOType::t_unknow || second.m_typeID == rdoRuntime::RDOType::t_unknow ) {
		if ( m_typeID == rdoRuntime::RDOType::t_unknow ) {
			parser()->error( src_info(), rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser()->error( second.src_info(), rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	}
	rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<rdoRuntime::RDOCalcConst*>(m_calc);
	rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<rdoRuntime::RDOCalcConst*>(second.m_calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser()->runtime(), calc1->calcValue( parser()->runtime() ) < calc2->calcValue( parser()->runtime() ) );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcIsLess::getStaticSrcInfo(calc1, calc2) );
	} else {
		newCalc = new rdoRuntime::RDOCalcIsLess( parser()->runtime(), m_calc, second.m_calc );
	}
	RDOFUNLogic* logic = new RDOFUNLogic( this, newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	logic->m_int_or_double.insert( m_int_or_double, second.m_int_or_double );
	return logic;
}

RDOFUNLogic* RDOFUNArithm::operator >( RDOFUNArithm& second )
{
	if ( m_typeID == rdoRuntime::RDOType::t_enum || second.m_typeID == rdoRuntime::RDOType::t_enum ) {
		parser()->error( second.src_info(), "Нельзя сравнивать перечислимые данные" );
	} else if ( m_typeID == rdoRuntime::RDOType::t_unknow || second.m_typeID == rdoRuntime::RDOType::t_unknow ) {
		if ( m_typeID == rdoRuntime::RDOType::t_unknow ) {
			parser()->error( src_info(), rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser()->error( second.src_info(), rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	}
	rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<rdoRuntime::RDOCalcConst*>(m_calc);
	rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<rdoRuntime::RDOCalcConst*>(second.m_calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser()->runtime(), calc1->calcValue( parser()->runtime() ) > calc2->calcValue( parser()->runtime() ) );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcIsGreater::getStaticSrcInfo(calc1, calc2) );
	} else {
		newCalc = new rdoRuntime::RDOCalcIsGreater( parser()->runtime(), m_calc, second.m_calc );
	}
	RDOFUNLogic* logic = new RDOFUNLogic( this, newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	logic->m_int_or_double.insert( m_int_or_double, second.m_int_or_double );
	return logic;
}

RDOFUNLogic* RDOFUNArithm::operator <=( RDOFUNArithm& second )
{
	if ( m_typeID == rdoRuntime::RDOType::t_enum || second.m_typeID == rdoRuntime::RDOType::t_enum ) {
		parser()->error( second.src_info(), "Нельзя сравнивать перечислимые данные" );
	} else if ( m_typeID == rdoRuntime::RDOType::t_unknow || second.m_typeID == rdoRuntime::RDOType::t_unknow ) {
		if ( m_typeID == rdoRuntime::RDOType::t_unknow ) {
			parser()->error( src_info(), rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser()->error( second.src_info(), rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	}
	rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<rdoRuntime::RDOCalcConst*>(m_calc);
	rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<rdoRuntime::RDOCalcConst*>(second.m_calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser()->runtime(), calc1->calcValue( parser()->runtime() ) <= calc2->calcValue( parser()->runtime() ) );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcIsLEQ::getStaticSrcInfo(calc1, calc2) );
	} else {
		newCalc = new rdoRuntime::RDOCalcIsLEQ( parser()->runtime(), m_calc, second.m_calc );
	}
	RDOFUNLogic* logic = new RDOFUNLogic( this, newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	return logic;
}

RDOFUNLogic* RDOFUNArithm::operator >=( RDOFUNArithm& second )
{
	if ( m_typeID == rdoRuntime::RDOType::t_enum || second.m_typeID == rdoRuntime::RDOType::t_enum ) {
		parser()->error( second.src_info(), "Нельзя сравнивать перечислимые данные" );
	} else if ( m_typeID == rdoRuntime::RDOType::t_unknow || second.m_typeID == rdoRuntime::RDOType::t_unknow ) {
		if ( m_typeID == rdoRuntime::RDOType::t_unknow ) {
			parser()->error( src_info(), rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser()->error( second.src_info(), rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	}
	rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<rdoRuntime::RDOCalcConst*>(m_calc);
	rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<rdoRuntime::RDOCalcConst*>(second.m_calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser()->runtime(), calc1->calcValue( parser()->runtime() ) >= calc2->calcValue( parser()->runtime() ) );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcIsGEQ::getStaticSrcInfo(calc1, calc2) );
	} else {
		newCalc = new rdoRuntime::RDOCalcIsGEQ( parser()->runtime(), m_calc, second.m_calc );
	}
	RDOFUNLogic* logic = new RDOFUNLogic( this, newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	logic->m_int_or_double.insert( m_int_or_double, second.m_int_or_double );
	return logic;
}

RDOFUNLogic* RDOFUNArithm::operator ==( RDOFUNArithm& second )
{
	if ( m_typeID == rdoRuntime::RDOType::t_enum && second.m_typeID == rdoRuntime::RDOType::t_enum ) {
		if ( enu != second.enu ) {
			parser()->error( second.src_info(), "Нельзя сравнивать разные перечислимые типы" );
//			parser()->error("cannot compare different enumerative types");
		}
	} else if ( m_typeID == rdoRuntime::RDOType::t_enum && second.m_typeID == rdoRuntime::RDOType::t_unknow ) {
		second.m_calc = new rdoRuntime::RDOCalcConst( parser()->runtime(), enu->findEnumValueWithThrow( second.src_info(), second.str ) );
		second.m_calc->setSrcInfo( second.src_info() );
//	} else if ( (m_typeID >= 2) || (second.m_typeID >= 2) ) {
	} else if ( (m_typeID == rdoRuntime::RDOType::t_unknow && second.m_typeID == rdoRuntime::RDOType::t_enum) || (m_typeID == rdoRuntime::RDOType::t_unknow && second.m_typeID == rdoRuntime::RDOType::t_unknow) ) {
		if ( m_typeID == rdoRuntime::RDOType::t_unknow ) {
			parser()->error( src_info(), rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser()->error( second.src_info(), rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
//		parser()->error("cannot compare enumerative m_typeID with nonenumerative type");
	}
	rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<rdoRuntime::RDOCalcConst*>(m_calc);
	rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<rdoRuntime::RDOCalcConst*>(second.m_calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser()->runtime(), calc1->calcValue( parser()->runtime() ) == calc2->calcValue( parser()->runtime() ) );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcIsEqual::getStaticSrcInfo(calc1, calc2) );
	} else {
		newCalc = new rdoRuntime::RDOCalcIsEqual( parser()->runtime(), m_calc, second.m_calc );
	}
	RDOFUNLogic* logic = new RDOFUNLogic( this, newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	logic->m_int_or_double.insert( m_int_or_double, second.m_int_or_double );
	return logic;
}

RDOFUNLogic* RDOFUNArithm::operator !=( RDOFUNArithm& second )
{
	if ( m_typeID == rdoRuntime::RDOType::t_enum && second.m_typeID == rdoRuntime::RDOType::t_enum ) {
		if ( enu != second.enu ) {
			parser()->error( second.src_info(), "Нельзя сравнивать разные перечислимые типы" );
		}
	} else if ( m_typeID == rdoRuntime::RDOType::t_enum && second.m_typeID == rdoRuntime::RDOType::t_unknow ) {
		second.m_calc = new rdoRuntime::RDOCalcConst( parser()->runtime(), enu->findEnumValueWithThrow( second.src_info(), second.str ) );
		second.m_calc->setSrcInfo( second.src_info() );
	} else if ( (m_typeID == rdoRuntime::RDOType::t_unknow && second.m_typeID == rdoRuntime::RDOType::t_enum) || (m_typeID == rdoRuntime::RDOType::t_unknow && second.m_typeID == rdoRuntime::RDOType::t_unknow) ) {
		if ( m_typeID == rdoRuntime::RDOType::t_unknow ) {
			parser()->error( src_info(), rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser()->error( second.src_info(), rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	}
	rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<rdoRuntime::RDOCalcConst*>(m_calc);
	rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<rdoRuntime::RDOCalcConst*>(second.m_calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser()->runtime(), calc1->calcValue( parser()->runtime() ) != calc2->calcValue( parser()->runtime() ) );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcIsNotEqual::getStaticSrcInfo(calc1, calc2) );
	} else {
		newCalc = new rdoRuntime::RDOCalcIsNotEqual( parser()->runtime(), m_calc, second.m_calc );
	}
	RDOFUNLogic* logic = new RDOFUNLogic( this, newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	logic->m_int_or_double.insert( m_int_or_double, second.m_int_or_double );
	return logic;
}

rdoRuntime::RDOCalc* RDOFUNArithm::createCalc( const RDORTPParamType* const forType )
{
	if ( m_typeID != rdoRuntime::RDOType::t_unknow ) {
		if ( forType == NULL ) {
			return m_calc;
		}
		if ( forType->typeID() != rdoRuntime::RDOType::t_int ) {
			if ( forType->typeID() == rdoRuntime::RDOType::t_enum ) {
				m_int_or_double.initCalc( true );
			}
			return m_calc;
		} else {
			m_int_or_double.initCalc( true );
			rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcDoubleToInt( parser()->runtime(), m_calc );
//			rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcInt( parser()->runtime(), m_calc );
			newCalc->setSrcInfo( src_info() );
			return newCalc;
		}
	} else if ( m_typeID == rdoRuntime::RDOType::t_unknow && !forType && !str.empty() ) {
		parser()->error( src_info(), rdo::format( "Неизвестный идентификатор: %s", str.c_str()) );
	}

	if ( !forType ) {
		parser()->error( src_info(), "Неизвестный тип параметра" );
	}

	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcConst( parser()->runtime(), forType->getValue( RDOValue(src_info()) )); // TODO юзали str
	newCalc->setSrcInfo( src_info() );
	return newCalc;
}

void RDOFUNArithm::setSrcInfo( const RDOParserSrcInfo& src_info )
{
	RDOParserSrcInfo::setSrcInfo( src_info );
	if ( m_calc ) m_calc->setSrcInfo( src_info );
}

void RDOFUNArithm::setSrcPos( const RDOSrcInfo::Position& _pos )
{
	RDOParserSrcInfo::setSrcPos( _pos );
	if ( m_calc ) m_calc->setSrcPos( _pos );
}

void RDOFUNArithm::setSrcText( const std::string& value )
{
	RDOParserSrcInfo::setSrcText( value );
	if ( m_calc ) m_calc->setSrcText( src_info().src_text() );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNConstant
// ----------------------------------------------------------------------------
RDOFUNConstant::RDOFUNConstant( RDOParser* _parser, RDOFUNConst* _const ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _const->src_info() ),
	m_const( _const ),
	m_number( _parser->getFUNCONST_id() )
{
	parser()->insertFUNConstant( this );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNParams
// ----------------------------------------------------------------------------
// Параметры, с которыми функция вызывается
// ----------------------------------------------------------------------------
RDOFUNArithm* RDOFUNParams::createCall( const std::string& funName ) const
{
	const RDOFUNFunction* const func = parser()->findFUNFunction( funName );
	if ( !func ) {
		return createSeqCall( funName );
	}

	int nParams = func->getParams().size();
	if ( nParams != params.size() ) {
		parser()->error( src_info(), rdo::format("Неверное количество параметров функции: %s", funName.c_str()) );
//		parser()->error(("Wrong parameters number in function call: " + *funName).c_str());
	}

	rdoRuntime::RDOCalcFunctionCall* funcCall = new rdoRuntime::RDOCalcFunctionCall( parser()->runtime(), func->getFunctionCalc() );
	const_cast<RDOFUNFunction*>(func)->insertPostLinked( funcCall );
	funcCall->setSrcInfo( src_info() );
	for ( int i = 0; i < nParams; i++ ) {
		const RDORTPParamType* const funcParam = func->getParams()[i]->getType();
		RDOFUNArithm* arithm = params[i];
		funcParam->checkParamType( arithm );
		switch ( funcParam->typeID() ) {
			case rdoRuntime::RDOType::t_int: {
				rdoRuntime::RDOCalc* arg = arithm->createCalc( NULL );
				if ( arithm->typeID() == rdoRuntime::RDOType::t_real ) {
					arg = new rdoRuntime::RDOCalcDoubleToInt( parser()->runtime(), arg );
				}
				if ( static_cast<const RDORTPIntParamType*>(funcParam)->getDiap().isExist() ) {
					arg = new rdoRuntime::RDOCalcCheckDiap( parser()->runtime(), static_cast<const RDORTPIntParamType*>(funcParam)->getDiap().getMin(), static_cast<const RDORTPIntParamType*>(funcParam)->getDiap().getMax(), arg );
				}
				funcCall->addParameter( arg );
				break;
			}
			case rdoRuntime::RDOType::t_real: {
				rdoRuntime::RDOCalc* arg = arithm->createCalc( NULL );
				if ( static_cast<const RDORTPRealParamType*>(funcParam)->getDiap().isExist() ) {
					arg = new rdoRuntime::RDOCalcCheckDiap( parser()->runtime(), static_cast<const RDORTPRealParamType*>(funcParam)->getDiap().getMin(), static_cast<const RDORTPRealParamType*>(funcParam)->getDiap().getMax(), arg );
				}
				funcCall->addParameter( arg );
				break;
			}
			default: {
				if ( arithm->typeID() == rdoRuntime::RDOType::t_int || arithm->typeID() == rdoRuntime::RDOType::t_real ) {
					parser()->error( arithm->src_info(), "Неверный тип параметра функции" );
//					parser()->error("wrong parameter type");
				}

				rdoRuntime::RDOCalc* arg;
				if ( arithm->typeID() == rdoRuntime::RDOType::t_enum ) {
					if ( arithm->enu != static_cast<const RDORTPEnumParamType*>(funcParam)->m_enum ) {
						parser()->error( "Перечислимые типы не совпадают" );
//						parser()->error("wrong enumerative parameter type");
					}
					arg = arithm->createCalc( NULL );
				} else {
					arg = new rdoRuntime::RDOCalcConst( parser()->runtime(), static_cast<const RDORTPEnumParamType*>(funcParam)->m_enum->findEnumValueWithThrow( arithm->src_info(), arithm->str ) );
					arg->setSrcInfo( arithm->src_info() );
				}
				funcCall->addParameter( arg );
				break;
			}
		}
	}

	RDOFUNArithm* res = new RDOFUNArithm( this, func->getType()->typeID(), funcCall, src_pos() );
	if ( func->getType()->typeID() == rdoRuntime::RDOType::t_enum ) {
		res->enu = reinterpret_cast<const RDORTPEnumParamType*>(func->getType())->m_enum;
	}
	res->setSrcInfo( src_info() );

	return res;
}

RDOFUNArithm* RDOFUNParams::createSeqCall( const std::string& seqName ) const
{
	const RDOFUNSequence* const seq = parser()->findFUNSequence( seqName );
	if ( !seq ) {
		parser()->error( funseq_name, rdo::format("Неопределенная функция или последовательность: %s", seqName.c_str()) );
//		parser()->error( "Undefined function or sequence: " + *seqName );
	}
	RDOFUNArithm* arithm = seq->createCallCalc( this, src_info() );
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- Последовательности
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequence
// ----------------------------------------------------------------------------
RDOFUNSequence::RDOFUNSequence( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _header->src_info() ),
	header( _header ),
	init_calc( NULL ),
	next_calc( NULL ),
	base( _base )
{
	header->reparent( this );
	parser()->insertFUNSequence( this );
}

void RDOFUNSequence::initResult()
{
	initCalcSrcInfo();
	switch ( header->getType()->typeID() ) {
		case rdoRuntime::RDOType::t_int: {
			next_calc->m_res_real = false;
			if ( static_cast<const RDORTPIntParamType*>(header->getType())->getDiap().isExist() ) {
				next_calc->m_diap     = true;
				next_calc->m_diap_min = static_cast<const RDORTPIntParamType*>(header->getType())->getDiap().getMin();
				next_calc->m_diap_max = static_cast<const RDORTPIntParamType*>(header->getType())->getDiap().getMax();
			}
			break;
		}
		case rdoRuntime::RDOType::t_real: {
			if ( static_cast<const RDORTPRealParamType*>(header->getType())->getDiap().isExist() ) {
				next_calc->m_diap     = true;
				next_calc->m_diap_min = static_cast<const RDORTPRealParamType*>(header->getType())->getDiap().getMin();
				next_calc->m_diap_max = static_cast<const RDORTPRealParamType*>(header->getType())->getDiap().getMax();
			}
			break;
		}
		default: parser()->error( src_info(), "Внутренняя ошибка: обработать все типы RDOValue" );
	}
}

void RDOFUNSequence::initCalcSrcInfo()
{
	if ( init_calc ) {
		init_calc->setSrcInfo( header->src_info() );
		init_calc->setSrcText( "Установка базы последовательности " + init_calc->src_text() );
	}
	next_calc->setSrcInfo( header->src_info() );
}

// ----------------------------------------------------------------------------
// ---------- Датчики случайных чисел
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceUniform
// ----------------------------------------------------------------------------
RDOFUNSequenceUniform::RDOFUNSequenceUniform( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base ):
	RDOFUNSequence( _parser, _header, _base )
{
	if ( header->getType()->typeID() == rdoRuntime::RDOType::t_enum ) {
		parser()->error( src_info(), "Последовательность типа uniform не может возвращять значения перечислимого типа" );
//		parser()->error("Uniform sequence cannot be enumerative type");
	}
}

void RDOFUNSequenceUniform::createCalcs()
{
	rdoRuntime::RandGeneratorUniform* gen = new rdoRuntime::RandGeneratorUniform();
	init_calc = new rdoRuntime::RDOCalcSeqInit( parser()->runtime(), base, gen );
	parser()->runtime()->addInitCalc( init_calc );
	next_calc = new rdoRuntime::RDOCalcSeqNextUniform( parser()->runtime(), gen );
	initResult();
}

RDOFUNArithm* RDOFUNSequenceUniform::createCallCalc( const RDOFUNParams* const param, const RDOParserSrcInfo& _src_info ) const
{
	if ( param->params.size() != 2 ) {
		parser()->error( _src_info, rdo::format("Для равномерного закона распределения '%s' нужно указать два параметра: минимальную и максимальную границы диапазона", name().c_str()) );
//		parser()->error("Wrong parameters number in uniform sequence call: " + *header->name);
	}

	rdoRuntime::RDOCalcFunctionCall* funcCall = new rdoRuntime::RDOCalcFunctionCall( parser()->runtime(), next_calc );
	RDORTPRealParamType* realType = new RDORTPRealParamType( this, new RDORTPRealDiap(parser()), new RDORTPDefVal(parser()) );
	rdoRuntime::RDOCalc* arg1 = param->params[0]->createCalc( realType );
	rdoRuntime::RDOCalc* arg2 = param->params[1]->createCalc( realType );

	funcCall->addParameter( arg1 );
	funcCall->addParameter( arg2 );

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceUniform*>(this), header->getType()->typeID(), funcCall, param->src_pos() );
	arithm->setSrcInfo( _src_info );
	if ( arithm->typeID() == rdoRuntime::RDOType::t_enum ) {
		parser()->error( src_info(), "Внутренняя ошибка парсера" );
//		parser()->error("Internal parser error");
	}
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceExponential
// ----------------------------------------------------------------------------
RDOFUNSequenceExponential::RDOFUNSequenceExponential( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base ):
	RDOFUNSequence( _parser, _header, _base )
{
	if ( header->getType()->typeID() != rdoRuntime::RDOType::t_int && header->getType()->typeID() != rdoRuntime::RDOType::t_real ) {
		parser()->error( _header->getType()->src_info(), rdo::format("Последовательность '%s' может возвращять значения только целого или вещественного типа", src_text().c_str()) );
//		parser()->error("Exponential sequence cannot be enumerative type");
	}
}

void RDOFUNSequenceExponential::createCalcs()
{
	rdoRuntime::RandGeneratorExponential* gen = new rdoRuntime::RandGeneratorExponential();
	init_calc = new rdoRuntime::RDOCalcSeqInit( parser()->runtime(), base, gen );
	parser()->runtime()->addInitCalc( init_calc );
	next_calc = new rdoRuntime::RDOCalcSeqNextExponential( parser()->runtime(), gen );
	initResult();
}

RDOFUNArithm* RDOFUNSequenceExponential::createCallCalc( const RDOFUNParams* const param, const RDOParserSrcInfo& _src_info ) const
{
	if ( param->params.size() != 1 ) {
		parser()->error( _src_info, rdo::format("Для экспоненциального закона распределения '%s' нужно указать один параметр: математическое ожидание", name().c_str()) );
//		parser()->error("Wrong parameters number in exponential sequence call: " + *header->name);
	}

	rdoRuntime::RDOCalcFunctionCall* funcCall = new rdoRuntime::RDOCalcFunctionCall( parser()->runtime(), next_calc );
	RDOFUNArithm *arithm1 = param->params[0];
	RDORTPRealParamType* realType = new RDORTPRealParamType( this, new RDORTPRealDiap(parser()), new RDORTPDefVal(parser()) );
	rdoRuntime::RDOCalc *arg1 = arithm1->createCalc( realType );

	funcCall->addParameter( arg1 );

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceExponential*>(this), header->getType()->typeID(), funcCall, param->src_pos() );
	arithm->setSrcInfo( _src_info );
	if ( arithm->typeID() == rdoRuntime::RDOType::t_enum ) {
		parser()->error( src_info(), "Внутренняя ошибка парсера" );
//		parser()->error("Internal parser error");
	}
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceNormal
// ----------------------------------------------------------------------------
RDOFUNSequenceNormal::RDOFUNSequenceNormal( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base ):
	RDOFUNSequence( _parser, _header, _base )
{
	if ( header->getType()->typeID() == rdoRuntime::RDOType::t_enum ) {
		parser()->error( src_info(), "Последовательность типа normal не может возвращять значения перечислимого типа" );
//		parser()->error("Normal sequence cannot be enumerative type");
	}
}

void RDOFUNSequenceNormal::createCalcs()
{
	rdoRuntime::RandGeneratorNormal* gen = new rdoRuntime::RandGeneratorNormal();
	init_calc = new rdoRuntime::RDOCalcSeqInit( parser()->runtime(), base, gen );
	parser()->runtime()->addInitCalc( init_calc );
	next_calc = new rdoRuntime::RDOCalcSeqNextNormal( parser()->runtime(), gen );
	initResult();
}

RDOFUNArithm* RDOFUNSequenceNormal::createCallCalc( const RDOFUNParams* const param, const RDOParserSrcInfo& _src_info ) const
{
	if ( param->params.size() != 2 ) {
		parser()->error( _src_info, rdo::format("Для нормального закона распределения '%s' нужно указать два параметра: математическое ожидание и среднее квадратическое отклонение", name().c_str()) );
//		parser()->error("Wrong parameters number in normal sequence call: " + *header->name);
	}

	rdoRuntime::RDOCalcFunctionCall *funcCall = new rdoRuntime::RDOCalcFunctionCall(parser()->runtime(), next_calc);
	RDORTPRealParamType* realType = new RDORTPRealParamType( this, new RDORTPRealDiap(parser()), new RDORTPDefVal(parser()) );
	RDOFUNArithm *arithm1 = param->params[0];
	RDOFUNArithm *arithm2 = param->params[1];
	rdoRuntime::RDOCalc *arg1 = arithm1->createCalc( realType );
	rdoRuntime::RDOCalc *arg2 = arithm2->createCalc( realType );

	funcCall->addParameter( arg1 );
	funcCall->addParameter( arg2 );

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceNormal*>(this), header->getType()->typeID(), funcCall, param->src_pos() );
	arithm->setSrcInfo( _src_info );
	if ( arithm->typeID() == rdoRuntime::RDOType::t_enum ) {
		parser()->error( src_info(), "Внутренняя ошибка парсера" );
//		parser()->error("Internal parser error");
	}
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- Гистограмма
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHist
// ----------------------------------------------------------------------------
RDOFUNArithm* RDOFUNSequenceByHist::createCallCalc( const RDOFUNParams* const param, const RDOParserSrcInfo& _src_info ) const
{
	if ( param->params.size() != 0 ) {
		parser()->error( _src_info, rdo::format("Гистограмма '%s' должна вызываться без параметров", name().c_str()) );
//		parser()->error("Wrong parameters number in by_hist sequence call: " + *header->name);
	}

	rdoRuntime::RDOCalcFunctionCall* funcCall = new rdoRuntime::RDOCalcFunctionCall( parser()->runtime(), next_calc );

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceByHist*>(this), header->getType()->typeID(), funcCall, param->src_pos() );
	arithm->setSrcInfo( _src_info );
	if ( arithm->typeID() == rdoRuntime::RDOType::t_enum ) {
		arithm->enu = reinterpret_cast<const RDORTPEnumParamType*>(header->getType())->m_enum;
	}
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHistReal
// ----------------------------------------------------------------------------
void RDOFUNSequenceByHistReal::addReal( rdoRuntime::RDOValue _from, rdoRuntime::RDOValue _to, rdoRuntime::RDOValue _freq, const YYLTYPE& _from_pos, const YYLTYPE& _to_pos, const YYLTYPE& _freq_pos )
{
	if ( !to.empty() && _from != to.back() ) {
		parser()->error( _from_pos, "Начало интервала должно совпадать с окончанием предыдущего" );
	}
	if ( _from > _to ) {
		parser()->error( _to_pos, "Начало интервала должно быть меньше его конца" );
	}
	if ( _freq == 0 ) {
		parser()->warning( _freq_pos, "Относительная вероятность должна быть больше нуля" );
	} else if ( _freq < 0 ) {
		parser()->error( _freq_pos, "Относительная вероятность должна быть больше нуля" );
	}
	header->getType()->checkParamType( _from, RDOParserSrcInfo(_from_pos, rdo::format("%f", _from)) );
	header->getType()->checkParamType( _to, RDOParserSrcInfo(_to_pos, rdo::format("%f", _to)) );
	from.push_back( _from );
	to.push_back( _to );
	freq.push_back( _freq );
}

void RDOFUNSequenceByHistReal::createCalcs()
{
	rdoRuntime::RandGeneratorByHistReal* gen = new rdoRuntime::RandGeneratorByHistReal();
	int size = from.size();
	for ( int i = 0; i < size; i++ ) {
		gen->addValues( from[i].getDouble(), to[i].getDouble(), freq[i].getDouble() );
	}

	init_calc = new rdoRuntime::RDOCalcSeqInit( parser()->runtime(), base, gen );
	parser()->runtime()->addInitCalc( init_calc );
	next_calc = new rdoRuntime::RDOCalcSeqNextByHist( parser()->runtime(), gen );
	initResult();
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHistEnum
// ----------------------------------------------------------------------------
void RDOFUNSequenceByHistEnum::addEnum( const RDOParserSrcInfo& _value_info, rdoRuntime::RDOValue _freq, const YYLTYPE& _freq_pos )
{
	if ( _freq == 0 ) {
		parser()->warning( _freq_pos, "Относительная вероятность должна быть больше нуля" );
	} else if ( _freq < 0 ) {
		parser()->error( _freq_pos, "Относительная вероятность должна быть больше нуля" );
	}
	rdoRuntime::RDOValue enum_id = header->getType()->getValue( RDOValue(_value_info) );
	if ( std::find( val.begin(), val.end(), enum_id ) != val.end() ) {
		parser()->error( _value_info, rdo::format("Перечислимое значение определено дважды: %s", _value_info.src_text().c_str()) );
	}
	header->getType()->checkParamType( enum_id, _value_info );
	val.push_back( enum_id );
	freq.push_back( _freq );
}

void RDOFUNSequenceByHistEnum::createCalcs()
{
	rdoRuntime::RandGeneratorByHistEnum* gen = new rdoRuntime::RandGeneratorByHistEnum();
	int size = val.size();
	for ( int i = 0; i < size; i++ ) {
		gen->addValues( val[i], freq[i].getDouble() );
	}
	init_calc = new rdoRuntime::RDOCalcSeqInit( parser()->runtime(), base, gen );
	parser()->runtime()->addInitCalc( init_calc );
	next_calc = new rdoRuntime::RDOCalcSeqNextByHist( parser()->runtime(), gen );
	initCalcSrcInfo();
}

// ----------------------------------------------------------------------------
// ---------- Перечень значений
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerative
// ----------------------------------------------------------------------------
RDOFUNArithm* RDOFUNSequenceEnumerative::createCallCalc( const RDOFUNParams* const param, const RDOParserSrcInfo& _src_info ) const
{
	if ( param->params.size() != 0 ) {
		parser()->error( _src_info, rdo::format("Перечисление '%s' должно вызываться без параметров", name().c_str()) );
//		parser()->error("Wrong parameters number in enumerative sequence call: " + *header->name);
	}

	rdoRuntime::RDOCalcFunctionCall* funcCall = new rdoRuntime::RDOCalcFunctionCall( parser()->runtime(), next_calc );

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceEnumerative*>(this), header->getType()->typeID(), funcCall, param->src_pos() );
	arithm->setSrcInfo( _src_info );
	if ( arithm->typeID() == rdoRuntime::RDOType::t_enum ) {
		arithm->enu = reinterpret_cast<const RDORTPEnumParamType*>(header->getType())->m_enum;
	}
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerativeInt
// ----------------------------------------------------------------------------
void RDOFUNSequenceEnumerativeInt::addInt( int _val )
{
	val.push_back( _val );
}

void RDOFUNSequenceEnumerativeInt::createCalcs()
{
	rdoRuntime::RandGeneratorEnumerative* gen = new rdoRuntime::RandGeneratorEnumerative();
	int size = val.size();
	for ( int i = 0; i < size; i++ ) {
		gen->addValue( val[i] );
	}
	next_calc = new rdoRuntime::RDOCalcSeqNextByHist( parser()->runtime(), gen );
	initCalcSrcInfo();
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerativeReal
// ----------------------------------------------------------------------------
void RDOFUNSequenceEnumerativeReal::addReal( double _val )
{
	val.push_back( _val );
}

void RDOFUNSequenceEnumerativeReal::createCalcs()
{
	rdoRuntime::RandGeneratorEnumerative* gen = new rdoRuntime::RandGeneratorEnumerative();
	int size = val.size();
	for ( int i = 0; i < size; i++ ) {
		gen->addValue( val[i] );
	}
	next_calc = new rdoRuntime::RDOCalcSeqNextByHist( parser()->runtime(), gen );
	initCalcSrcInfo();
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerativeEnum
// ----------------------------------------------------------------------------
void RDOFUNSequenceEnumerativeEnum::addEnum( const RDOParserSrcInfo& _value_info )
{
	val.push_back( header->getType()->getValue( RDOValue(_value_info) ));
}

void RDOFUNSequenceEnumerativeEnum::createCalcs()
{
	rdoRuntime::RandGeneratorEnumerative* gen = new rdoRuntime::RandGeneratorEnumerative();
	int size = val.size();
	for ( int i = 0; i < size; i++ ) {
		gen->addValue( val[i] );
	}
	next_calc = new rdoRuntime::RDOCalcSeqNextByHist( parser()->runtime(), gen );
	initCalcSrcInfo();
}

// ----------------------------------------------------------------------------
// ---------- Функции
// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElement
// ----------------------------------------------------------------------------
rdoRuntime::RDOCalcIsEqual* RDOFUNFunctionListElement::createIsEqualCalc( const RDORTPParamType* const retType, rdoRuntime::RDOCalcFuncParam* const funcParam, const RDOParserSrcInfo& _src_pos ) const
{
	rdoRuntime::RDOCalcConst* constCalc = createResultCalc( retType, _src_pos );
	return new rdoRuntime::RDOCalcIsEqual( parser()->runtime(), funcParam, constCalc );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementIdentif
// ----------------------------------------------------------------------------
rdoRuntime::RDOCalcConst* RDOFUNFunctionListElementIdentif::createResultCalc( const RDORTPParamType* const retType, const RDOParserSrcInfo& _src_pos ) const
{
	rdoRuntime::RDOCalcConst* const_calc = new rdoRuntime::RDOCalcConst( parser()->runtime(), retType->getValue(RDOValue(RDOParserSrcInfo(_src_pos.getPosAsYY(), src_text()))) );
	const_calc->setSrcInfo( _src_pos );
	return const_calc;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementReal
// ----------------------------------------------------------------------------
rdoRuntime::RDOCalcConst* RDOFUNFunctionListElementReal::createResultCalc( const RDORTPParamType* const retType, const RDOParserSrcInfo& _src_pos ) const
{
	rdoRuntime::RDOCalcConst* const_calc = new rdoRuntime::RDOCalcConst( parser()->runtime(), retType->getValue( RDOValue(value, _src_pos) ) );
	const_calc->setSrcInfo( _src_pos );
	return const_calc;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementInt
// ----------------------------------------------------------------------------
rdoRuntime::RDOCalcConst* RDOFUNFunctionListElementInt::createResultCalc( const RDORTPParamType* const retType, const RDOParserSrcInfo& _src_pos ) const
{
	rdoRuntime::RDOCalcConst* const_calc = new rdoRuntime::RDOCalcConst( parser()->runtime(), retType->getValue( RDOValue(value, _src_pos) ) );
	const_calc->setSrcInfo( _src_pos );
	return const_calc;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementEq
// ----------------------------------------------------------------------------
rdoRuntime::RDOCalcConst* RDOFUNFunctionListElementEq::createResultCalc( const RDORTPParamType* const retType, const RDOParserSrcInfo& _src_pos ) const
{
	parser()->error( _src_pos, "Внутренная ошибка парсера: RDOFUNFunctionListElementEq::createResultCalc" );
//	parser()->error( "Internal parser error, incorrect state" );
	return NULL;	// unreachable code
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNCalculateIf
// ----------------------------------------------------------------------------
RDOFUNCalculateIf::RDOFUNCalculateIf( const RDOParserObject* _parent, RDOFUNLogic* _condition, RDOFUNArithm* _action ):
	RDOParserObject( _parent ),
	condition( _condition ),
	action( _action )
{
	parser()->getLastFUNFunction()->getType()->checkParamType( action );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunction
// ----------------------------------------------------------------------------
RDOFUNFunction::RDOFUNFunction( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDORTPParamType* const _retType ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	retType( _retType ),
	functionCalc( NULL )
{
	parser()->insertFUNFunction( this );
}

RDOFUNFunction::RDOFUNFunction( RDOParser* _parser, const std::string& _name, const RDORTPParamType* const _retType ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _name ),
	retType( _retType ),
	functionCalc( NULL )
{
	parser()->insertFUNFunction( this );
}

void RDOFUNFunction::setFunctionCalc( rdoRuntime::RDOFunCalc* calc )
{
	functionCalc = calc;
	if ( functionCalc->src_empty() )
	{
		functionCalc->setSrcInfo( src_info() );
	}
	std::vector< rdoRuntime::RDOCalcFunctionCall* >::iterator it = post_linked.begin();
	while ( it != post_linked.end() )
	{
		(*it)->setFunctionCalc( getFunctionCalc() );
		it++;
	}
}

const RDOFUNFunctionParam* const RDOFUNFunction::findFUNFunctionParam( const std::string& paramName ) const 
{
	std::vector< const RDOFUNFunctionParam* >::const_iterator it = std::find_if( params.begin(), params.end(), compareName<RDOFUNFunctionParam>(paramName) );
	return it != params.end() ? *it : NULL;
}

int RDOFUNFunction::findFUNFunctionParamNum( const std::string& paramName ) const
{
	std::vector< const RDOFUNFunctionParam* >::const_iterator it = std::find_if( params.begin(), params.end(), compareName<RDOFUNFunctionParam>(paramName) );
	return it != params.end() ? it - params.begin() : -1;
}

void RDOFUNFunction::add( const RDOFUNFunctionParam* const _param )
{ 
	const RDOFUNFunctionParam* const param = findFUNFunctionParam( _param->name() );
	if ( param ) {
		parser()->error_push_only( _param->src_info(), rdo::format("Параметр уже существует: %s", _param->name().c_str()) );
		parser()->error_push_only( param->src_info(), "См. первое определение" );
		parser()->error_push_done();
//		parser()->error("Second appearance of the same parameter name: " + *(_param->name()));
	}
	params.push_back( _param ); 
}

void RDOFUNFunction::add( const RDOFUNFunctionListElement* const _param )
{
	elements.push_back( _param ); 
}

void RDOFUNFunction::add( const RDOFUNCalculateIf* const _calculateIf )
{
	calculateIf.push_back( _calculateIf );
}

void RDOFUNFunction::createListCalc()
{
	if ( !getType()->getDV().isExist() ) {
		parser()->error( getType()->getDV().value().src_info(), rdo::format("Функция '%s' должна иметь значение по-умолчанию", name().c_str()) );
//		parser()->error(("list function " + *name + " must have default result value").c_str());
	}
	if ( params.empty() ) {
		parser()->error( src_info(), rdo::format("Функция '%s' должна иметь параметры, т.к. её тип list", name().c_str()) );
	}
	if ( elements.empty() ) {
		parser()->warning( src_info(), rdo::format("Функция '%s' не содержит списка", name().c_str()) );
	}

	rdoRuntime::RDOCalcConst* default_value_calc = new rdoRuntime::RDOCalcConst( parser()->runtime(), getType()->getDefaultValue( getType()->getDV().value().src_info() ) );
	default_value_calc->setSrcInfo( getType()->getDV().value().src_info() );
	rdoRuntime::RDOFunListCalc* fun_calc = new rdoRuntime::RDOFunListCalc( parser()->runtime(), default_value_calc );
	fun_calc->setSrcInfo( src_info() );
	const RDOFUNFunctionListElement* arg_last = NULL;
	std::vector< const RDOFUNFunctionListElement* >::const_iterator elem_it = elements.begin();
	while ( elem_it != elements.end() ) {
		rdoRuntime::RDOCalc* case_calc = new rdoRuntime::RDOCalcConst( parser()->runtime(), true );
		case_calc->setSrcInfo( (*elem_it)->src_info() );
		int currParam_number = 0;
		std::vector< const RDOFUNFunctionParam* >::const_iterator param_it = params.begin();
		while ( param_it != params.end() ) {
			const RDOFUNFunctionParam* const param = *param_it;
			if ( elem_it == elements.end() ) {
				elem_it--;
				parser()->error( (*elem_it)->src_info(), rdo::format("Ожидается значение для параметра '%s'", param->name().c_str()) );
			}
			const RDOFUNFunctionListElement* elem = *elem_it;
			if ( elem->isEquivalence() ) {
				if ( (param_it + 1) == params.end() ) {
					parser()->error( elem->src_info(), rdo::format("Перед знаком равенства ожидается значение для параметра: %s",param->name().c_str()) );
				} else {
					std::string str = (*param_it)->src_text();
					param_it++;
					while ( param_it != params.end() ) {
						str += ", ";
						str += (*param_it)->src_text();
						param_it++;
					}
					parser()->error( elem->src_info(), rdo::format("Перед знаком равенства ожидаются значения для параметров: %s", str.c_str()) );
				}
			}
			rdoRuntime::RDOCalcFuncParam* funcParam  = new rdoRuntime::RDOCalcFuncParam( parser()->runtime(), currParam_number, param->src_info() );
			rdoRuntime::RDOCalcIsEqual* compare_calc = elem->createIsEqualCalc( param->getType(), funcParam, elem->src_info() );
			rdoRuntime::RDOCalc* and_calc = new rdoRuntime::RDOCalcAnd( parser()->runtime(), case_calc, compare_calc );
			case_calc = and_calc;
			elem_it++;
			param_it++;
			currParam_number++;
		}
		if ( elem_it == elements.end() ) {
			elem_it--;
			param_it--;
			parser()->error( (*elem_it)->src_info(), rdo::format("После значения для параметра '%s' ожидается знак равенства", (*param_it)->name().c_str()) );
		}
		if ( !(*elem_it)->isEquivalence() ) {
			parser()->error( (*elem_it)->src_info(), rdo::format("Слишком много значений, вместо '%s' ожидается знак равенства", (*elem_it)->src_text().c_str()) );
		}
		elem_it++;
		if ( elem_it == elements.end() ) {
			elem_it--;
			parser()->error( (*elem_it)->src_info(), rdo::format("После знака равенства ожидается значение функции '%s'", name().c_str()) );
		}
		rdoRuntime::RDOCalcConst* result_calc = (*elem_it)->createResultCalc( getType(), (*elem_it)->src_info() );
		fun_calc->addCase( case_calc, result_calc );
		elem_it++;
	}
	setFunctionCalc( fun_calc );
}

void RDOFUNFunction::createTableCalc( const YYLTYPE& _elements_pos )
{
	std::vector< const RDOFUNFunctionListElement* >::const_iterator it = elements.begin();
	while ( it != elements.end() ) {
		if ( (*it)->isEquivalence() ) {
			parser()->error( (*it)->src_info(), "Символ '=' недопустим в табличной функции" );
//			parser()->error("\"=\" unexpected in table function");
		}
		it++;
	}
	int param_cnt = params.size();
	int range = 1;
	rdoRuntime::RDOCalc* calc = new rdoRuntime::RDOCalcConst( parser()->runtime(), 0 );
	calc->setSrcInfo( src_info() );
	calc->setSrcText( "0" );
	for ( int currParam = 0; currParam < param_cnt; currParam++ ) {
		const RDOFUNFunctionParam* const param  = params.at(currParam);
		rdoRuntime::RDOCalcFuncParam* funcParam = new rdoRuntime::RDOCalcFuncParam( parser()->runtime(), currParam, param->src_info() );
		rdoRuntime::RDOCalc* val2 = funcParam;
		if ( param->getType()->typeID() != rdoRuntime::RDOType::t_enum ) {
			rdoRuntime::RDOCalcConst* const1 = new rdoRuntime::RDOCalcConst( parser()->runtime(), 1 );
			const1->setSrcInfo( param->src_info() );
			val2 = new rdoRuntime::RDOCalcMinus( parser()->runtime(), val2, const1 );
		}
		rdoRuntime::RDOCalcConst* const2 = new rdoRuntime::RDOCalcConst( parser()->runtime(), range );
		const2->setSrcInfo( param->src_info() );
		rdoRuntime::RDOCalcMult* mult = new rdoRuntime::RDOCalcMultEnumSafe( parser()->runtime(), const2, val2 );
		rdoRuntime::RDOCalcPlus* add = new rdoRuntime::RDOCalcPlusEnumSafe( parser()->runtime(), mult, calc );
		range *= param->getType()->getDiapTableFunc();
		calc = add;
	}

	if ( elements.size() != range ) {
		parser()->error( _elements_pos, rdo::format("Неверное количество элементов табличной функции '%s': %d, требуется %d", name().c_str(), elements.size(), range) );
//		parser()->error(("wrong number of value in table function " + *name).c_str());
	}

	rdoRuntime::RDOFuncTableCalc* fun_calc = new rdoRuntime::RDOFuncTableCalc( parser()->runtime(), calc );
	fun_calc->setSrcInfo( src_info() );
	for ( int currElem = 0; currElem < range; currElem++ ) {
		const RDOFUNFunctionListElement* const elem = elements.at(currElem);
		rdoRuntime::RDOCalcConst* result_calc = elem->createResultCalc( getType(), elem->src_info() );
		fun_calc->addResultCalc( result_calc );
	}
	setFunctionCalc( fun_calc );
}

void RDOFUNFunction::createAlgorithmicCalc( const RDOParserSrcInfo& _body_src_info )
{
	rdoRuntime::RDOFunAlgorithmicCalc* fun_calc = NULL;
	switch ( getType()->typeID() ) {
		case rdoRuntime::RDOType::t_int: {
			if ( static_cast<const RDORTPIntParamType*>(getType())->getDiap().isExist() ) {
				fun_calc = new rdoRuntime::RDOFunAlgorithmicDiapCalc( parser()->runtime(), static_cast<const RDORTPIntParamType*>(getType())->getDiap().getMin(), static_cast<const RDORTPIntParamType*>(getType())->getDiap().getMax() );
			}
			break;
		}
		case rdoRuntime::RDOType::t_real: {
			if ( static_cast<const RDORTPRealParamType*>(getType())->getDiap().isExist() ) {
				fun_calc = new rdoRuntime::RDOFunAlgorithmicDiapCalc( parser()->runtime(), static_cast<const RDORTPRealParamType*>(getType())->getDiap().getMin(), static_cast<const RDORTPRealParamType*>(getType())->getDiap().getMax() );
			}
			break;
		}
		case rdoRuntime::RDOType::t_enum: {
			break;
		}
		default: parser()->error( src_info(), "Внутренняя ошибка: обработать все типы RDOValue" );
	}
	if ( !fun_calc ) {
		fun_calc = new rdoRuntime::RDOFunAlgorithmicCalc( parser()->runtime() );
	}
	fun_calc->setSrcInfo( src_info() );
	bool default_flag = false;
	bool true_const   = false;
	const rdoRuntime::RDOCalcConst* calc_cond_const = NULL;
	int size = calculateIf.size();
	int cnt = 0;
	for ( int i = 0; i < size; i++ ) {
		rdoRuntime::RDOCalc* logic_calc = calculateIf[i]->condition->createCalc( getType()->typeID() );
		rdoRuntime::RDOCalcConst* calc_cond_last = dynamic_cast<rdoRuntime::RDOCalcConst*>(logic_calc);
		if ( true_const ) {
			parser()->warning( calculateIf[i]->condition->src_info(), rdo::format("Условие не используется: %s", calculateIf[i]->condition->src_text().c_str()) );
			parser()->warning( calc_cond_const->src_info(), rdo::format("Последнее рабочее условие функции: %s", calc_cond_const->src_text().c_str()) );
		} else if ( !calc_cond_last || calc_cond_last->calcValue( parser()->runtime() ).getBool() ) {
			// Игнорируем чистые false-условия предыдущей проверкой
			fun_calc->addCalcIf( logic_calc, calculateIf[i]->action->createCalc(getType()) );
			cnt++;
		}
		if ( !default_flag && calc_cond_last && calc_cond_last->calcValue( parser()->runtime() ).getBool() ) {
			true_const   = true;
			default_flag = true;
			calc_cond_const = calc_cond_last;
		}
	}
	if ( !cnt ) {
		parser()->warning( src_info(), rdo::format("Отсутствует тело функции '%s'", name().c_str()) );
	}
	if ( !true_const ) {
		if ( getType()->getDV().isExist() ) {
			rdoRuntime::RDOCalcConst* calc_cond = new rdoRuntime::RDOCalcConst( parser()->runtime(), 1 );
			rdoRuntime::RDOCalcConst* calc_act  = new rdoRuntime::RDOCalcConst( parser()->runtime(), getType()->getDefaultValue( getType()->getDV().value().src_info() ) );
			calc_cond->setSrcInfo( getType()->src_info() );
			calc_act->setSrcInfo( getType()->src_info() );
			fun_calc->addCalcIf( calc_cond, calc_act );
			default_flag = true;
		}
	}
	if ( !default_flag ) {
		rdoRuntime::RDOCalcConst* calc_cond = new rdoRuntime::RDOCalcConst( parser()->runtime(), 1 );
		rdoRuntime::RDOCalcConst* calc_act  = NULL;
		switch ( getType()->typeID() ) {
			case rdoRuntime::RDOType::t_int: {
				if ( static_cast<const RDORTPIntParamType*>(getType())->getDiap().isExist() ) {
					calc_act = new rdoRuntime::RDOCalcConst( parser()->runtime(), static_cast<const RDORTPIntParamType*>(getType())->getDiap().getMin() );
				}
				break;
			}
			case rdoRuntime::RDOType::t_real: {
				if ( static_cast<const RDORTPRealParamType*>(getType())->getDiap().isExist() ) {
					calc_act = new rdoRuntime::RDOCalcConst( parser()->runtime(), static_cast<const RDORTPRealParamType*>(getType())->getDiap().getMin() );
				}
				break;
			}
			case rdoRuntime::RDOType::t_enum: {
				calc_act = new rdoRuntime::RDOCalcConst( parser()->runtime(), static_cast<const RDORTPEnumParamType*>(getType())->m_enum->getFirstValue() );
				break;
			}
			default: parser()->error( src_info(), "Внутренняя ошибка: обработать все типы RDOValue" );
		}
		if ( !calc_act ) {
			calc_act = new rdoRuntime::RDOCalcConst( parser()->runtime(), 0 );
		}
		calc_cond->setSrcInfo( getType()->src_info() );
		calc_act->setSrcInfo( getType()->src_info() );
		fun_calc->addCalcIf( calc_cond, calc_act );
		parser()->warning( src_info(), rdo::format("Для функции '%s' неопределено значение по-умолчанию", name().c_str()) );
	}
	setFunctionCalc( fun_calc );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNGroup
// ----------------------------------------------------------------------------
RDOFUNGroup::RDOFUNGroup( RDOParser* _parser, const RDOParserSrcInfo& _res_info ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _res_info )
{
	init( _res_info );
}

RDOFUNGroup::RDOFUNGroup( const RDOParserObject* _parent, const RDOParserSrcInfo& _res_info ):
	RDOParserObject( _parent ),
	RDOParserSrcInfo( _res_info )
{
	init( _res_info );
}

void RDOFUNGroup::init( const RDOParserSrcInfo& _res_info )
{
	resType = parser()->findRTPResType( _res_info.src_text() );
	if ( !resType ) {
		parser()->error( _res_info, rdo::format("Неизвестный тип ресурса: %s", _res_info.src_text().c_str()) );
	}
	parser()->insertFUNGroup( this );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNGroupLogic
// ----------------------------------------------------------------------------
RDOFUNLogic* RDOFUNGroupLogic::createFunLogic( RDOFUNLogic* cond )
{
	rdoRuntime::RDOFunCalcGroup* calc;
	switch ( funType ) {
		case fgt_exist    : setSrcText( "Exist(" + resType->name() + ": " + cond->src_text() + ")" );     calc = new rdoRuntime::RDOFunCalcExist    ( parser()->runtime(), resType->getNumber(), cond->createCalc() ); break;
		case fgt_notexist : setSrcText( "NotExist(" + resType->name() + ": " + cond->src_text() + ")" );  calc = new rdoRuntime::RDOFunCalcNotExist ( parser()->runtime(), resType->getNumber(), cond->createCalc() ); break;
		case fgt_forall   : setSrcText( "ForAll(" + resType->name() + ": " + cond->src_text() + ")" );    calc = new rdoRuntime::RDOFunCalcForAll   ( parser()->runtime(), resType->getNumber(), cond->createCalc() ); break;
		case fgt_notforall: setSrcText( "NotForAll(" + resType->name() + ": " + cond->src_text() + ")" ); calc = new rdoRuntime::RDOFunCalcNotForAll( parser()->runtime(), resType->getNumber(), cond->createCalc() ); break;
		default: parser()->error( src_info(), "Внутренная ошибка: несуществующий тип функции" );
	}
	parser()->getFUNGroupStack().pop_back();
	RDOFUNLogic* logic = new RDOFUNLogic( this, calc );
	logic->setSrcInfo( src_info() );
	return logic;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSelect
// ----------------------------------------------------------------------------
// Сам Select как выборка по типу и условию
void RDOFUNSelect::initSelect( const RDOFUNLogic* cond )
{
	select = new rdoRuntime::RDOFunCalcSelect( parser()->runtime(), resType->getNumber(), const_cast<RDOFUNLogic*>(cond)->createCalc() );
	select->setSrcInfo( cond->src_info() );
}

// Групповая функция над выборкой Select'а
RDOFUNLogic* RDOFUNSelect::createFunSelectGroup( RDOFUNGroupLogic::FunGroupType funType, RDOFUNLogic* cond )
{
	rdoRuntime::RDOFunCalcSelectBase* calc;
	switch ( funType ) {
		case RDOFUNGroupLogic::fgt_exist    : setSrcText( src_text() + ".Exist(" + cond->src_text() + ")" );     calc = new rdoRuntime::RDOFunCalcSelectExist    ( parser()->runtime(), select, cond->createCalc() ); break;
		case RDOFUNGroupLogic::fgt_notexist : setSrcText( src_text() + ".NotExist(" + cond->src_text() + ")" );  calc = new rdoRuntime::RDOFunCalcSelectNotExist ( parser()->runtime(), select, cond->createCalc() ); break;
		case RDOFUNGroupLogic::fgt_forall   : setSrcText( src_text() + ".ForAll(" + cond->src_text() + ")" );    calc = new rdoRuntime::RDOFunCalcSelectForAll   ( parser()->runtime(), select, cond->createCalc() ); break;
		case RDOFUNGroupLogic::fgt_notforall: setSrcText( src_text() + ".NotForAll(" + cond->src_text() + ")" ); calc = new rdoRuntime::RDOFunCalcSelectNotForAll( parser()->runtime(), select, cond->createCalc() ); break;
		default: parser()->error( "Внутренная ошибка: неизвестный метод для списка ресурсов" );
	}
	parser()->getFUNGroupStack().pop_back();
	RDOFUNLogic* logic = new RDOFUNLogic( this, calc );
	logic->setSrcInfo( src_info() );
	return logic;
}

RDOFUNLogic* RDOFUNSelect::createFunSelectEmpty( const RDOParserSrcInfo& _empty_info )
{
	setSrcText( src_text() + "." + _empty_info.src_text() );
	parser()->getFUNGroupStack().pop_back();
	RDOFUNLogic* logic = new RDOFUNLogic( this, new rdoRuntime::RDOFunCalcSelectEmpty( parser()->runtime(), select ) );
	logic->setSrcInfo( _empty_info );
	return logic;
}

RDOFUNArithm* RDOFUNSelect::createFunSelectSize( const RDOParserSrcInfo& _size_info )
{
	setSrcText( src_text() + "." + _size_info.src_text() );
	parser()->getFUNGroupStack().pop_back();
	RDOFUNArithm* arithm = new RDOFUNArithm( this, rdoRuntime::RDOType::t_int, new rdoRuntime::RDOFunCalcSelectSize( parser()->runtime(), select ), _size_info );
	arithm->setSrcInfo( _size_info );
	return arithm;
}

} // namespace rdoParse

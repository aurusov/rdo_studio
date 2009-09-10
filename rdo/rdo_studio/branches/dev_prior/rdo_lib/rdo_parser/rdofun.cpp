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
void RDOFUNDoubleToIntByResult::roundCalc()
{
	std::vector< rdoRuntime::RDOCalcDoubleToIntByResult* >::iterator it = m_int_or_double.begin();
	while ( it != m_int_or_double.end() )
	{
		(*it)->needRound();
		it++;
	}
}

// ----------------------------------------------------------------------------
// ---------- Набор макросов для генерации логических и арифметических выражений
// ----------------------------------------------------------------------------
#define CREATE_CALC( CALC, OPR ) \
rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<rdoRuntime::RDOCalcConst*>(m_calc); \
rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<rdoRuntime::RDOCalcConst*>(second.m_calc); \
rdoRuntime::RDOCalc* newCalc; \
if ( calc1 && calc2 ) \
{ \
	newCalc = new rdoRuntime::RDOCalcConst( parser()->runtime(), calc1->calcValue( parser()->runtime() ) OPR calc2->calcValue( parser()->runtime() ) ); \
	newCalc->setSrcInfo( rdoRuntime::RDOCalc##CALC::getStaticSrcInfo(calc1, calc2) ); \
} \
else \
{ \
	newCalc = new rdoRuntime::RDOCalc##CALC( parser()->runtime(), m_calc, second.m_calc ); \
}

#define RETURN_LOGIC() \
RDOFUNLogic* logic = new RDOFUNLogic( this, newCalc ); \
logic->setSrcInfo( newCalc->src_info() ); \
logic->m_int_or_double.insert( m_int_or_double, second.m_int_or_double ); \
return logic;

#define GENERATE_LOGIC( CALC, OPR ) \
CREATE_CALC( CALC, OPR ); \
RETURN_LOGIC();

#define CAST_ARITHM_VALUE( OPR, ERROR ) \
try \
{ \
	if ( beforeCastValue(second) == CR_CONTINUE ) \
	{ \
		value().value() OPR second.value().value(); \
	} \
} \
catch ( rdoRuntime::RDOValueException& ) \
{ \
	parser()->error( second, rdo::format(ERROR, type().name().c_str(), second.type().name().c_str()) ); \
}

#define GET_ARITHM_PRE_TYPE() \
const RDOType* newType = getPreType( second );

#define GENERATE_ARITHM_CALC( CALC, OPR, ERROR ) \
CAST_ARITHM_VALUE( OPR, ERROR ) \
GET_ARITHM_PRE_TYPE() \
CREATE_CALC( CALC, OPR )

#define RETURN_ARITHM() \
RDOFUNArithm* arithm = new RDOFUNArithm( this, RDOValue(*newType, newCalc->src_info()), newCalc ); \
arithm->m_int_or_double.insert( m_int_or_double, second.m_int_or_double ); \
return arithm;

#define GENERATE_ARITHM( CALC, OPR, ERROR ) \
GENERATE_ARITHM_CALC( CALC, OPR, ERROR ) \
RETURN_ARITHM()

#define GENERATE_LOGIC_FROM_ARITHM( CALC, OPR, ERROR ) \
GENERATE_ARITHM_CALC( CALC, OPR, ERROR ) \
RETURN_LOGIC()

// ----------------------------------------------------------------------------
// ---------- RDOFUNLogic
// ----------------------------------------------------------------------------
RDOFUNLogic::RDOFUNLogic( const RDOFUNArithm& arithm ):
	RDOParserObject( &arithm ),
	RDOParserSrcInfo( arithm.src_info() ),
	m_calc( NULL )
{
	switch ( arithm.typeID() )
	{
		case rdoRuntime::RDOType::t_bool: m_calc = arithm.calc(); break;
	}
	if ( !m_calc )
	{
		parser()->error( src_info(), rdo::format("Ожидается логическое выражение, найдено: %s", src_text().c_str()) );
	}
}

RDOFUNLogic::RDOFUNLogic( const RDOParserObject* _parent, rdoRuntime::RDOCalc* _calc, bool hide_warning ):
	RDOParserObject( _parent ),
	RDOParserSrcInfo(),
	m_calc( _calc )
{
	if ( m_calc )
	{
		m_calc->setSrcFileType( src_filetype() );
	}
	if ( !hide_warning )
	{
		rdoRuntime::RDOCalcConst* calc_const = dynamic_cast<rdoRuntime::RDOCalcConst*>(m_calc);
		if ( calc_const )
		{
			if ( calc_const->calcValue( parser()->runtime() ).getAsBool() )
			{
				parser()->warning( calc_const->src_info(), rdo::format("Логическое выражение всегда истинно: %s", calc_const->src_text().c_str()) );
			}
			else
			{
				parser()->warning( calc_const->src_info(), rdo::format("Логическое выражение всегда ложно: %s", calc_const->src_text().c_str()) );
			}
		}
	}
}

rdoRuntime::RDOCalc* RDOFUNLogic::getCalc( rdoRuntime::RDOType::TypeID _id )
{
	if ( _id != rdoRuntime::RDOType::t_real )
	{
		m_int_or_double.roundCalc();
	}
	return m_calc;
}

RDOFUNLogic* RDOFUNLogic::operator &&( const RDOFUNLogic& second )
{
	GENERATE_LOGIC( And, && );
}

RDOFUNLogic* RDOFUNLogic::operator ||( const RDOFUNLogic& second )
{
	GENERATE_LOGIC( Or, || );
}

RDOFUNLogic* RDOFUNLogic::operator_not()
{
	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcNot( parser()->runtime(), m_calc );
	RDOFUNLogic* logic = new RDOFUNLogic( this, newCalc );
	logic->setSrcInfo( newCalc->src_info() );
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
RDOFUNArithm::RDOFUNArithm( RDOParser* parser, const RDOValue& value, rdoRuntime::RDOCalc* calc ):
	RDOParserObject( parser ),
	m_value( value ),
	m_calc( calc )
{
	setSrcInfo( m_value.src_info() );
}

RDOFUNArithm::RDOFUNArithm( const RDOParserObject* parent, const RDOValue& value, rdoRuntime::RDOCalc* calc ):
	RDOParserObject( parent ),
	m_value( value ),
	m_calc( calc )
{
	setSrcInfo( m_value.src_info() );
}

RDOFUNArithm::RDOFUNArithm( RDOParser* _parser, const RDOValue& value ):
	RDOParserObject( _parser ),
	m_calc( NULL )
{
	init( value );
}

RDOFUNArithm::RDOFUNArithm( const RDOFUNArithm* parent, const RDOValue& value ):
	RDOParserObject( parent ),
	m_calc( NULL )
{
	init( value );
}

RDOFUNArithm::RDOFUNArithm( RDOParser* parser, const RDOValue& res_name, const RDOValue& par_name ):
	RDOParserObject( parser ),
	m_calc( NULL )
{
	init( res_name, par_name );
}

RDOFUNArithm::RDOFUNArithm( const RDOFUNArithm* parent, const RDOValue& res_name, const RDOValue& par_name ):
	RDOParserObject( parent ),
	m_calc( NULL )
{
	init( res_name, par_name );
}

void RDOFUNArithm::init( const RDOValue& value )
{
	setSrcInfo( value.src_info() );

	if ( value.constant() )
	{
		m_value = value;
		m_calc  = new rdoRuntime::RDOCalcConst( parser()->runtime(), m_value.value() );
		m_calc->setSrcInfo( src_info() );
		return;
	}
	if ( value->getIdentificator() == "Time_now" || value->getIdentificator() == "time_now" || value->getIdentificator() == "Системное_время" || value->getIdentificator() == "системное_время" )
	{
		m_value = g_real;
		m_calc = new rdoRuntime::RDOCalcGetTimeNow( parser()->runtime() );
		m_calc->setSrcInfo( src_info() );
		return;
	}
	if ( value->getIdentificator() == "Terminate_counter" || value->getIdentificator() == "terminate_counter" )
	{
		m_value = g_int;
		m_calc = new rdoRuntime::RDOCalcGetTermNow( parser()->runtime() );
		m_calc->setSrcInfo( src_info() );
		return;
	}
	if ( value->getIdentificator() == "Seconds" || value->getIdentificator() == "seconds" )
	{
		m_value = g_real;
		m_calc = new rdoRuntime::RDOCalcGetSeconds( parser()->runtime() );
		m_calc->setSrcInfo( src_info() );
		return;
	}

	// Ищем параметры паттерна или функции по имени
	const RDOFUNFunctionParam* param = NULL;
	switch ( parser()->getFileToParse() )
	{
		case rdoModelObjects::PAT: param = parser()->getLastPATPattern()->findPATPatternParam( value->getIdentificator() ); break;
		case rdoModelObjects::FUN: param = parser()->getLastFUNFunction()->findFUNFunctionParam( value->getIdentificator() ); break;
	}

	// Ищем константы по имени
	const RDOFUNConstant* cons = parser()->findFUNConstant( value->getIdentificator() );

	if ( cons && param )
	{
		parser()->error( src_info(), rdo::format("Имя параметра образца совпадает с именем константы: %s", value->getIdentificator().c_str()) );
	}

	if ( cons )
	{
		m_value = cons->getType()->type();
		m_calc  = new rdoRuntime::RDOCalcGetConst( parser()->runtime(), cons->getNumber() );
		m_calc->setSrcInfo( src_info() );
		return;
	}

	// Ищем последовательность по имени
	const RDOFUNSequence* seq = parser()->findFUNSequence( value->getIdentificator() );
	if ( seq && param )
	{
		parser()->error( src_info(), rdo::format("Имя параметра образца совпадает с именем последовательности: %s", value->getIdentificator().c_str()) );
	}

	if ( seq )
	{
		RDOFUNParams tmp( seq );
		RDOFUNArithm* arithm = tmp.createSeqCall( value->getIdentificator() );
		arithm->setSrcInfo( src_info() );
		m_value = arithm->value();
		m_calc  = arithm->m_calc;
		return;
	}

	if ( param )
	{
		// Это параметр
		m_value = param->getType()->type();
		switch ( parser()->getFileToParse() ) {
			case rdoModelObjects::PAT: m_calc = new rdoRuntime::RDOCalcPatParam( parser()->runtime(), parser()->getLastPATPattern()->findPATPatternParamNum( value->getIdentificator() ) ); break;
			case rdoModelObjects::FUN: m_calc = new rdoRuntime::RDOCalcFuncParam( parser()->runtime(), parser()->getLastFUNFunction()->findFUNFunctionParamNum( value->getIdentificator() ), param->src_info() ); break;
		}
		if ( m_calc ) m_calc->setSrcInfo( src_info() );
		return;
	}

	// Возможно, что это значение перечислимого типа
	std::vector< RDORTPEnumParamType* >::const_iterator it = parser()->getRTPEnumParamTypes().begin();
	while ( it != parser()->getRTPEnumParamTypes().end() )
	{
		if ( (*it)->m_enum->getEnums().findEnum( value->getIdentificator() ) != rdoRuntime::RDOEnumType::END )
		{
			// Да, это перечислимый тип, только одно и тоже значение может встречаться в разных
			// перечислимых типах, поэтому какой именно из них выбрать - вопрос
			m_value = value;
			return;
		}
		it++;
	}

	parser()->error( value.src_info(), rdo::format("Неизвестный идентификатор: %s", value->getIdentificator().c_str()) );
}

void RDOFUNArithm::init( const RDOValue& res_name, const RDOValue& par_name )
{
	setSrcInfo( res_name.src_info(), ".", par_name.src_info() );
	const RDORSSResource* const res = parser()->findRSSResource( res_name->getIdentificator() ); 
	if ( res )
	{
		// Это ресурс с закладки RSS
		unsigned int parNumb = res->getType()->getRTPParamNumber( par_name->getIdentificator() );
		if ( parNumb == RDORTPResType::UNDEFINED_PARAM )
		{
			parser()->error( par_name.src_info(), rdo::format("Неизвестный параметр ресурса: %s", par_name->getIdentificator().c_str()) );
		}
		if ( res->getType()->isPermanent() )
		{
			m_calc = new rdoRuntime::RDOCalcGetResParam( parser()->runtime(), res->getID(), parNumb );
		}
		else if ( res->getType()->isTemporary() && parser()->getFileToParse() == rdoModelObjects::FRM )
		{
			m_calc = new rdoRuntime::RDOCalcGetTempResParamFRM( parser()->runtime(), res->getID(), parNumb );
		}
		else
		{
			parser()->error( res_name.src_info(), rdo::format("Нельзя использовать временный ресурс: %s", res_name->getIdentificator().c_str()) );
		}
		m_calc->setSrcInfo( src_info() );
		m_value = res->getType()->findRTPParam( par_name->getIdentificator() )->getType()->type();
		return;
	}
	// Это не ресурс, но возможно релевантный ресурс или ресурс, внутри групповой функции
	else if ( !parser()->getFUNGroupStack().empty() && parser()->getFUNGroupStack().back()->resType->name() == res_name->getIdentificator() )
	{
		// Это ресурс внутри групповой функции
		RDOFUNGroup* currGroup = parser()->getFUNGroupStack().back();
		unsigned int parNumb = currGroup->resType->getRTPParamNumber( par_name->getIdentificator() );
		if ( parNumb == RDORTPResType::UNDEFINED_PARAM )
		{
			parser()->error( par_name.src_info(), rdo::format("Неизвестный параметр ресурса: %s", par_name->getIdentificator().c_str()) );
		}
		m_calc = new rdoRuntime::RDOCalcGetGroupResParam( parser()->runtime(), parNumb );
		m_calc->setSrcInfo( src_info() );
		m_value = currGroup->resType->findRTPParam( par_name->getIdentificator() )->getType()->type();
		return;
	}
	else
	{
		switch ( parser()->getFileToParse() )
		{
			case rdoModelObjects::PAT:
			{
				if ( parser()->getLastPATPattern() && parser()->getLastPATPattern()->findRelevantResource( res_name->getIdentificator() ) )
				{
					// Это релевантный ресурс где-то в паттерне (with_min-common-choice, $Time, $Body)
					RDOPATPattern* pat = parser()->getLastPATPattern();
					const RDORelevantResource* const rel = pat->findRelevantResource( res_name->getIdentificator() );
					if ( !pat->currRelRes )
					{
						// Внутри with_min-common-choice или $Time
						if ( rel->begin == rdoRuntime::RDOResource::CS_NonExist || rel->begin == rdoRuntime::RDOResource::CS_Create )
						{
							parser()->error( res_name.src_info(), rdo::format("Релевантный ресурс не может быть использован, т.к. он еще не существует: %s", rel->name().c_str()) );
						}
					}
					else
					{
						// Внутри $Body
						// Проверяем использование неинициализированного рел.ресурса (rel) в Choice from другом рел.ресурсе (pat->currRelRes)
						if ( pat->currRelRes->isChoiceFromState() )
						{
							if ( !rel->alreadyHaveConverter && !rel->isDirect() )
							{
								parser()->error( res_name.src_info(), rdo::format("Релевантный ресурс неопределен: %s. Его нельзя использовать в условиях выбора других ресурсов до его собственного Choice from", rel->name().c_str()) );
							}
							if ( rel->begin == rdoRuntime::RDOResource::CS_NonExist )
							{
								parser()->error( res_name.src_info(), rdo::format("Релевантный ресурс в начале операции не существует (NonExist): %s", rel->name().c_str()) );
							}
							if ( rel->begin == rdoRuntime::RDOResource::CS_Create )
							{
								parser()->error( res_name.src_info(), rdo::format("Сразу после создания (Create) релевантный ресурс '%s' можно использовать только в конверторах, но не в условии выбора", rel->name().c_str()) );
							}
						}
						// Проверяем использование временного рел.ресурса внутри конвертора другого рел.ресурса
						if ( rel->getType()->isTemporary() )
						{
							// В конверторе начала
							if ( pat->currRelRes->currentState == RDORelevantResource::convertBegin )
							{
								if ( rel->begin == rdoRuntime::RDOResource::CS_Create && !rel->alreadyHaveConverter )
								{
									parser()->error( res_name.src_info(), rdo::format("Релевантный ресурс нельзя использовать до его создания (Create): %s", rel->name().c_str()) );
								}
								if ( rel->begin == rdoRuntime::RDOResource::CS_Erase && rel->alreadyHaveConverter )
								{
									parser()->error( res_name.src_info(), rdo::format("Релевантный ресурс нельзя использовать после удаления (Erase): %s", rel->name().c_str()) );
								}
								if ( rel->begin == rdoRuntime::RDOResource::CS_NonExist )
								{
									parser()->error( res_name.src_info(), rdo::format("Релевантный ресурс не существует в этом конверторе (NonExist): %s", rel->name().c_str()) );
								}
							}
							// В конверторе конца
							if ( pat->currRelRes->currentState == RDORelevantResource::convertEnd )
							{
								if ( rel->end == rdoRuntime::RDOResource::CS_Create && !rel->alreadyHaveConverter )
								{
									parser()->error( res_name.src_info(), rdo::format("Релевантный ресурс нельзя использовать до его создания (Create): %s", rel->name().c_str()) );
								}
								if ( rel->end == rdoRuntime::RDOResource::CS_Erase && rel->alreadyHaveConverter )
								{
									parser()->error( res_name.src_info(), rdo::format("Релевантный ресурс нельзя использовать после удаления (Erase): %s", rel->name().c_str()) );
								}
								if ( rel->end == rdoRuntime::RDOResource::CS_NonExist )
								{
									parser()->error( res_name.src_info(), rdo::format("Релевантный ресурс не существует в этом конверторе (NonExist): %s", rel->name().c_str()) );
								}
							}
						}
						// Проверяем использование еще не инициализированного (только для Create) параметра рел. ресурса в его же конверторе
						const RDORTPParam* param = pat->currRelRes->getType()->findRTPParam( par_name->getIdentificator() );
						if ( param && pat->currRelRes->name() == res_name->getIdentificator() )
						{
							// В конверторе начала
							if ( pat->currRelRes->currentState == RDORelevantResource::convertBegin && pat->currRelRes->begin == rdoRuntime::RDOResource::CS_Create)
							{
								if ( !pat->currRelRes->getParamSetBegin()->isExist( par_name->getIdentificator() ) )
								{
									if ( !param->getType()->getDV().isExist() )
									{
										parser()->error( par_name.src_info(), rdo::format("Параметр '%s' еще не определен, ему необходимо присвоить значение в текущем конверторе или указать значение по-умолчанию в типе ресурса", par_name->getIdentificator().c_str()) );
									}
								}
							}
							// В конверторе начала
							if ( pat->currRelRes->currentState == RDORelevantResource::convertEnd && pat->currRelRes->end == rdoRuntime::RDOResource::CS_Create) {
								if ( !pat->currRelRes->getParamSetEnd()->isExist( par_name->getIdentificator() ) ) {
									if ( !param->getType()->getDV().isExist() ) {
										parser()->error( par_name.src_info(), rdo::format("Параметр '%s' еще не определен, ему необходимо присвоить значение в текущем конверторе или указать значение по-умолчанию в типе ресурса", par_name->getIdentificator().c_str()) );
									}
								}
							}
						}
					}
					unsigned int parNumb = rel->getType()->getRTPParamNumber( par_name->getIdentificator() );
					if ( parNumb == RDORTPResType::UNDEFINED_PARAM )
					{
						parser()->error( par_name.src_info(), rdo::format("Неизвестный параметр ресурса: %s", par_name->getIdentificator().c_str()) );
					}
					m_calc = new rdoRuntime::RDOCalcGetRelevantResParam( parser()->runtime(), pat->findRelevantResourceNum( res_name->getIdentificator() ), parNumb );
					m_calc->setSrcInfo( src_info() );
					m_value = rel->getType()->findRTPParam( par_name->getIdentificator() )->getType()->type();
					return;
				}
				break;
			}
			case rdoModelObjects::DPT:
			{
				if ( parser()->isCurrentDPTSearch() && parser()->getLastDPTSearch()->getLastActivity() )
				{
					const RDOPATPattern* rule = parser()->getLastDPTSearch()->getLastActivity()->pattern();
					if ( rule && rule->findRelevantResource( res_name->getIdentificator() ) ) {
						// Это ресурс, который используется в DPT (condition, term_condition, evaluate_by, value before, value after)
						const RDORelevantResource* const rel = rule->findRelevantResource( res_name->getIdentificator() );
						int relResNumb = rule->findRelevantResourceNum( res_name->getIdentificator() );
						unsigned int parNumb = rel->getType()->getRTPParamNumber( par_name->getIdentificator() );
						if ( parNumb == RDORTPResType::UNDEFINED_PARAM )
						{
							parser()->error( par_name.src_info(), rdo::format("Неизвестный параметр ресурса: %s", par_name->getIdentificator().c_str()) );
						}
						m_calc = new rdoRuntime::RDOCalcGetRelevantResParam( parser()->runtime(), relResNumb, parNumb );
						m_calc->setSrcInfo( src_info() );
						m_value = rel->getType()->findRTPParam( par_name->getIdentificator() )->getType()->type();
						return;
					}
				}
				if ( parser()->isCurrentDPTPrior() && parser()->getLastDPTPrior()->getLastActivity() )
				{
					const RDOPATPattern* activity = parser()->getLastDPTPrior()->getLastActivity()->pattern();
					if ( activity && activity->findRelevantResource( res_name->getIdentificator() ) ) {
						// Это ресурс, который используется в выражении приоритета активности DPTPrior
						const RDORelevantResource* const rel = activity->findRelevantResource( res_name->getIdentificator() );
						int relResNumb = activity->findRelevantResourceNum( res_name->getIdentificator() );
						unsigned int parNumb = rel->getType()->getRTPParamNumber( par_name->getIdentificator() );
						if ( parNumb == RDORTPResType::UNDEFINED_PARAM )
						{
							parser()->error( par_name.src_info(), rdo::format("Неизвестный параметр ресурса: %s", par_name->getIdentificator().c_str()) );
						}
						m_calc = new rdoRuntime::RDOCalcGetRelevantResParam( parser()->runtime(), relResNumb, parNumb );
						m_calc->setSrcInfo( src_info() );
						m_value = rel->getType()->findRTPParam( par_name->getIdentificator() )->getType()->type();
						return;
					}
				}
				break;
			}
			case rdoModelObjects::FRM:
			{
				if ( parser()->getLastFRMFrame() && parser()->getLastFRMFrame()->frame()->getLastShow() && parser()->getLastFRMFrame()->frame()->getLastShow()->isShowIf() )
				{
					m_calc = new rdoRuntime::RDOCalcGetUnknowResParam( parser()->runtime(), res_name->getIdentificator(), par_name->getIdentificator() );
					m_calc->setSrcInfo( src_info() );
					return;
				}
				break;
			}
		}
	}
	parser()->error( res_name.src_info(), rdo::format("Неизвестный ресурс: %s", res_name->getIdentificator().c_str()) );
}

RDOFUNArithm::CastResult RDOFUNArithm::beforeCastValue( RDOFUNArithm& second )
{
	if ( typeID() == rdoRuntime::RDOType::t_enum && second.typeID() == rdoRuntime::RDOType::t_identificator )
	{
		second.m_value = RDOValue( enumType().findEnumValueWithThrow(second.src_info(), second.value()->getAsString()), enumType(), second.m_value.src_info() );
		second.m_calc  = new rdoRuntime::RDOCalcConst( parser()->runtime(), second.m_value.value() );
		second.m_calc->setSrcInfo( second.src_info() );
		return CR_DONE;
	}
	else if ( typeID() == rdoRuntime::RDOType::t_identificator && second.typeID() == rdoRuntime::RDOType::t_enum )
	{
		m_value = RDOValue( second.enumType().findEnumValueWithThrow(src_info(), value()->getAsString()), second.enumType(), m_value.src_info() );
		m_calc  = new rdoRuntime::RDOCalcConst( parser()->runtime(), m_value.value() );
		m_calc->setSrcInfo( src_info() );
		return CR_DONE;
	}
	return CR_CONTINUE;
}

const RDOType* RDOFUNArithm::getPreType( const RDOFUNArithm& second )
{
	if ( typeID() == rdoRuntime::RDOType::t_unknow )
	{
		parser()->error( src_info(), rdo::format("Внутренная ошибка парсера: неопределенное значение арифметического выражения: %s", src_text().c_str()) );
	}
	if ( second.typeID() == rdoRuntime::RDOType::t_unknow )
	{
		parser()->error( second.src_info(), rdo::format("Внутренная ошибка парсера: неопределенное значение арифметического выражения: %s", second.src_text().c_str()) );
	}
	if ( typeID() == rdoRuntime::RDOType::t_identificator )
	{
		parser()->error( src_info(), rdo::format("Неизвестный идентификатор: %s", value()->getIdentificator().c_str()) );
	}
	if ( second.typeID() == rdoRuntime::RDOType::t_identificator )
	{
		parser()->error( second.src_info(), rdo::format("Неизвестный идентификатор: %s", second.value()->getIdentificator().c_str()) );
	}

	const RDOType* newType = type().cast( second.type() );
	if ( !newType )
	{
		parser()->error_push_only( src_info(), rdo::format("Несовместимые типы данных: %s и %s", value().type()->asString().c_str(), second.value().type()->asString().c_str()) );
		parser()->error_push_only( value().src_info(), "См. первый тип" );
		parser()->error_push_only( second.value().src_info(), "См. второй тип" );
		parser()->error_push_done();
	}
	return newType;
}

RDOFUNArithm* RDOFUNArithm::operator+ ( RDOFUNArithm& second )
{
	GENERATE_ARITHM( Plus, +, "Ну не мегу я сложить %s и %s" );
}

RDOFUNArithm* RDOFUNArithm::operator- ( RDOFUNArithm& second )
{
	GENERATE_ARITHM( Minus, -, "Нельзя из %s вычесть %s" );
}

RDOFUNArithm* RDOFUNArithm::operator* ( RDOFUNArithm& second )
{
	GENERATE_ARITHM( Mult, *, "Нельзя %s умножить на %s" );
}

RDOFUNArithm* RDOFUNArithm::operator/ ( RDOFUNArithm& second )
{
	GENERATE_ARITHM_CALC( Div, /, "Нельзя %s разделить на %s" );
	// TODO: перевод вещественного в целое при делении. А что делать с умножением и т.д. ?
	// Ответ: с умножением надо делать тоже самое, только непонятно как
	if ( newType->type().typeID() == rdoRuntime::RDOType::t_int )
	{
		rdoRuntime::RDOCalc* newCalc_div = newCalc;
		newCalc = new rdoRuntime::RDOCalcDoubleToIntByResult( parser()->runtime(), newCalc_div );
		newCalc->setSrcInfo( newCalc_div->src_info() );
	}
	RDOFUNArithm* arithm = new RDOFUNArithm( this, RDOValue(*newType, newCalc->src_info()), newCalc );
	if ( newType->type().typeID() == rdoRuntime::RDOType::t_int )
	{
		arithm->m_int_or_double.push_back( reinterpret_cast<rdoRuntime::RDOCalcDoubleToIntByResult*>(newCalc) );
	}
	arithm->m_int_or_double.insert( m_int_or_double, second.m_int_or_double );
	return arithm;
}

RDOFUNLogic* RDOFUNArithm::operator< ( RDOFUNArithm& second )
{
	GENERATE_LOGIC_FROM_ARITHM( IsLess, <, "Нельзя сравнивать %s и %s" );
}

RDOFUNLogic* RDOFUNArithm::operator> ( RDOFUNArithm& second )
{
	GENERATE_LOGIC_FROM_ARITHM( IsGreater, >, "Нельзя сравнивать %s и %s" );
}

RDOFUNLogic* RDOFUNArithm::operator<= ( RDOFUNArithm& second )
{
	GENERATE_LOGIC_FROM_ARITHM( IsLEQ, <=, "Нельзя сравнивать %s и %s" );
}

RDOFUNLogic* RDOFUNArithm::operator>= ( RDOFUNArithm& second )
{
	GENERATE_LOGIC_FROM_ARITHM( IsGEQ, >, "Нельзя сравнивать %s и %s" );
}

RDOFUNLogic* RDOFUNArithm::operator== ( RDOFUNArithm& second )
{
	GENERATE_LOGIC_FROM_ARITHM( IsEqual, ==, "Нельзя сравнивать %s и %s" );
}

RDOFUNLogic* RDOFUNArithm::operator!= ( RDOFUNArithm& second )
{
	GENERATE_LOGIC_FROM_ARITHM( IsNotEqual, !=, "Нельзя сравнивать %s и %s" );
}

rdoRuntime::RDOCalc* RDOFUNArithm::createCalc( const RDORTPParamType* const forType )
{
	if ( typeID() != rdoRuntime::RDOType::t_identificator )
	{
		if ( forType == NULL )
		{
			return m_calc;
		}
		if ( forType->typeID() != rdoRuntime::RDOType::t_int )
		{
			if ( forType->typeID() == rdoRuntime::RDOType::t_enum )
			{
				m_int_or_double.roundCalc();
			}
			return m_calc;
		}
		else
		{
			m_int_or_double.roundCalc();
			rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcDoubleToInt( parser()->runtime(), m_calc );
			newCalc->setSrcInfo( src_info() );
			return newCalc;
		}
	}
	else if ( typeID() == rdoRuntime::RDOType::t_identificator && !forType )
	{
		parser()->error( src_info(), rdo::format( "Неизвестный идентификатор: %s", value()->getAsString().c_str()) );
	}

	if ( !forType )
	{
		parser()->error( src_info(), "Неизвестный тип параметра" );
	}

	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcConst( parser()->runtime(), forType->getValue( RDOValue(src_info()) )); // TODO юзали _m_str
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
				rdoRuntime::RDOCalc* arg = arithm->createCalc();
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
				rdoRuntime::RDOCalc* arg = arithm->createCalc();
				if ( static_cast<const RDORTPRealParamType*>(funcParam)->getDiap().isExist() ) {
					arg = new rdoRuntime::RDOCalcCheckDiap( parser()->runtime(), static_cast<const RDORTPRealParamType*>(funcParam)->getDiap().getMin(), static_cast<const RDORTPRealParamType*>(funcParam)->getDiap().getMax(), arg );
				}
				funcCall->addParameter( arg );
				break;
			}
			default: {
				if ( arithm->typeID() == rdoRuntime::RDOType::t_int || arithm->typeID() == rdoRuntime::RDOType::t_real ) {
					parser()->error( arithm->src_info(), "Неверный тип параметра функции" );
				}

				rdoRuntime::RDOCalc* arg;
				if ( arithm->typeID() == rdoRuntime::RDOType::t_enum ) {
					if ( arithm->enumType() != *static_cast<const RDORTPEnumParamType*>(funcParam)->m_enum ) {
						parser()->error( "Перечислимые типы не совпадают" );
					}
					arg = arithm->createCalc();
				} else {
					arg = new rdoRuntime::RDOCalcConst( parser()->runtime(), static_cast<const RDORTPEnumParamType*>(funcParam)->m_enum->findEnumValueWithThrow( arithm->src_info(), arithm->value()->getAsString() ) );
					arg->setSrcInfo( arithm->src_info() );
				}
				funcCall->addParameter( arg );
				break;
			}
		}
	}

	RDOFUNArithm* res = new RDOFUNArithm( this, RDOValue(func->getType()->type(), src_pos()), funcCall );
	res->setSrcInfo( src_info() );

	return res;
}

RDOFUNArithm* RDOFUNParams::createSeqCall( const std::string& seqName ) const
{
	const RDOFUNSequence* const seq = parser()->findFUNSequence( seqName );
	if ( !seq ) {
		parser()->error( funseq_name, rdo::format("Неопределенная функция или последовательность: %s", seqName.c_str()) );
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
	next_calc->m_res_real = true;
	switch ( header->getType()->typeID() )
	{
		case rdoRuntime::RDOType::t_int:
		{
			if ( static_cast<const RDORTPIntParamType*>(header->getType())->getDiap().isExist() )
			{
				next_calc->m_diap     = true;
				next_calc->m_diap_min = static_cast<const RDORTPIntParamType*>(header->getType())->getDiap().getMin();
				next_calc->m_diap_max = static_cast<const RDORTPIntParamType*>(header->getType())->getDiap().getMax();
			}
			break;
		}
		case rdoRuntime::RDOType::t_real:
		{
			next_calc->m_res_real = true;
			if ( static_cast<const RDORTPRealParamType*>(header->getType())->getDiap().isExist() )
			{
				next_calc->m_diap     = true;
				next_calc->m_diap_min = static_cast<const RDORTPRealParamType*>(header->getType())->getDiap().getMin();
				next_calc->m_diap_max = static_cast<const RDORTPRealParamType*>(header->getType())->getDiap().getMax();
			}
			break;
		}
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
	}

	rdoRuntime::RDOCalcFunctionCall* funcCall = new rdoRuntime::RDOCalcFunctionCall( parser()->runtime(), next_calc );
	RDORTPRealParamType* realType = new RDORTPRealParamType( this, new RDORTPRealDiap(parser()), new RDORTPDefVal(parser()) );
	rdoRuntime::RDOCalc* arg1 = param->params[0]->createCalc( realType );
	rdoRuntime::RDOCalc* arg2 = param->params[1]->createCalc( realType );

	funcCall->addParameter( arg1 );
	funcCall->addParameter( arg2 );

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceUniform*>(this), RDOValue(header->getType()->type(), param->src_pos()), funcCall );
	arithm->setSrcInfo( _src_info );
	if ( arithm->typeID() == rdoRuntime::RDOType::t_enum ) {
		parser()->error( src_info(), "Внутренняя ошибка парсера" );
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
	}

	rdoRuntime::RDOCalcFunctionCall* funcCall = new rdoRuntime::RDOCalcFunctionCall( parser()->runtime(), next_calc );
	RDOFUNArithm *arithm1 = param->params[0];
	RDORTPRealParamType* realType = new RDORTPRealParamType( this, new RDORTPRealDiap(parser()), new RDORTPDefVal(parser()) );
	rdoRuntime::RDOCalc *arg1 = arithm1->createCalc( realType );

	funcCall->addParameter( arg1 );

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceExponential*>(this), RDOValue(header->getType()->type(), param->src_pos()), funcCall );
	arithm->setSrcInfo( _src_info );
	if ( arithm->typeID() == rdoRuntime::RDOType::t_enum ) {
		parser()->error( src_info(), "Внутренняя ошибка парсера" );
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
	}

	rdoRuntime::RDOCalcFunctionCall *funcCall = new rdoRuntime::RDOCalcFunctionCall(parser()->runtime(), next_calc);
	RDORTPRealParamType* realType = new RDORTPRealParamType( this, new RDORTPRealDiap(parser()), new RDORTPDefVal(parser()) );
	RDOFUNArithm *arithm1 = param->params[0];
	RDOFUNArithm *arithm2 = param->params[1];
	rdoRuntime::RDOCalc *arg1 = arithm1->createCalc( realType );
	rdoRuntime::RDOCalc *arg2 = arithm2->createCalc( realType );

	funcCall->addParameter( arg1 );
	funcCall->addParameter( arg2 );

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceNormal*>(this), RDOValue(header->getType()->type(), param->src_pos()), funcCall );
	arithm->setSrcInfo( _src_info );
	if ( arithm->typeID() == rdoRuntime::RDOType::t_enum ) {
		parser()->error( src_info(), "Внутренняя ошибка парсера" );
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
	}

	rdoRuntime::RDOCalcFunctionCall* funcCall = new rdoRuntime::RDOCalcFunctionCall( parser()->runtime(), next_calc );

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceByHist*>(this), RDOValue(header->getType()->type(), param->src_pos()), funcCall );
	arithm->setSrcInfo( _src_info );
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHistReal
// ----------------------------------------------------------------------------
void RDOFUNSequenceByHistReal::addReal( const RDOValue& from, const RDOValue& to, const RDOValue& freq )
{
	if ( !m_to.empty() && from.value() != m_to.back() )
	{
		parser()->error( from, "Начало интервала должно совпадать с окончанием предыдущего" );
	}
	if ( from.value() > to.value() )
	{
		parser()->error( to, "Начало интервала должно быть меньше его конца" );
	}
	if ( freq.value() == 0 )
	{
		parser()->warning( freq, "Относительная вероятность должна быть больше нуля" );
	}
	else if ( freq.value() < 0 )
	{
		parser()->error( freq, "Относительная вероятность должна быть больше нуля" );
	}
	header->getType()->checkValue( from );
	header->getType()->checkValue( to );
	m_from.push_back( from.value() );
	m_to.push_back( to.value() );
	m_freq.push_back( freq.value() );
}

void RDOFUNSequenceByHistReal::createCalcs()
{
	rdoRuntime::RandGeneratorByHistReal* gen = new rdoRuntime::RandGeneratorByHistReal();
	int size = m_from.size();
	for ( int i = 0; i < size; i++ ) {
		gen->addValues( m_from[i].getDouble(), m_to[i].getDouble(), m_freq[i].getDouble() );
	}

	init_calc = new rdoRuntime::RDOCalcSeqInit( parser()->runtime(), base, gen );
	parser()->runtime()->addInitCalc( init_calc );
	next_calc = new rdoRuntime::RDOCalcSeqNextByHist( parser()->runtime(), gen );
	initResult();
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHistEnum
// ----------------------------------------------------------------------------
void RDOFUNSequenceByHistEnum::addEnum( const RDOValue& value, const RDOValue& freq )
{
	if ( freq.value() == 0 )
	{
		parser()->warning( freq, "Относительная вероятность должна быть больше нуля" );
	}
	else if ( freq.value() < 0 )
	{
		parser()->error( freq, "Относительная вероятность должна быть больше нуля" );
	}
	header->getType()->checkValue( value );
	rdoRuntime::RDOValue enum_id = header->getType()->getValue( value );
	if ( std::find( m_values.begin(), m_values.end(), enum_id ) != m_values.end() )
	{
		parser()->error( value, rdo::format("Перечислимое значение определено дважды: %s", value->getIdentificator().c_str()) );
	}
	m_values.push_back( enum_id );
	m_freq.push_back( freq.value() );
}

void RDOFUNSequenceByHistEnum::createCalcs()
{
	rdoRuntime::RandGeneratorByHistEnum* gen = new rdoRuntime::RandGeneratorByHistEnum();
	unsigned int size = m_values.size();
	for ( unsigned int i = 0; i < size; i++ )
	{
		gen->addValues( m_values[i], m_freq[i].getDouble() );
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
	if ( param->params.size() != 0 )
	{
		parser()->error( _src_info, rdo::format("Перечисление '%s' должно вызываться без параметров", name().c_str()) );
	}

	rdoRuntime::RDOCalcFunctionCall* funcCall = new rdoRuntime::RDOCalcFunctionCall( parser()->runtime(), next_calc );

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceEnumerative*>(this), RDOValue(header->getType()->type(), param->src_pos()), funcCall );
	arithm->setSrcInfo( _src_info );
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerative
// ----------------------------------------------------------------------------
void RDOFUNSequenceEnumerative::createCalcs()
{
	rdoRuntime::RandGeneratorEnumerative* gen = new rdoRuntime::RandGeneratorEnumerative();
	for ( unsigned int i = 0; i < m_values.size(); i++ )
	{
		gen->addValue( m_values[i].value() );
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
	}
	if ( params.empty() ) {
		parser()->error( src_info(), rdo::format("Функция '%s' должна иметь параметры, т.к. её тип list", name().c_str()) );
	}
	if ( elements.empty() ) {
		parser()->warning( src_info(), rdo::format("Функция '%s' не содержит списка", name().c_str()) );
	}

	rdoRuntime::RDOCalcConst* default_value_calc = new rdoRuntime::RDOCalcConst( parser()->runtime(), getType()->getDefaultValue( getType()->getDV().value() ) );
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
					std::string _m_str = (*param_it)->src_text();
					param_it++;
					while ( param_it != params.end() ) {
						_m_str += ", ";
						_m_str += (*param_it)->src_text();
						param_it++;
					}
					parser()->error( elem->src_info(), rdo::format("Перед знаком равенства ожидаются значения для параметров: %s", _m_str.c_str()) );
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
	// Фильтр на функцию
	switch ( getType()->typeID() )
	{
		case rdoRuntime::RDOType::t_int:
		{
			if ( static_cast<const RDORTPIntParamType*>(getType())->getDiap().isExist() )
			{
				fun_calc = new rdoRuntime::RDOFunAlgorithmicDiapCalc( parser()->runtime(), static_cast<const RDORTPIntParamType*>(getType())->getDiap().getMin(), static_cast<const RDORTPIntParamType*>(getType())->getDiap().getMax() );
			}
			break;
		}
		case rdoRuntime::RDOType::t_real:
		{
			if ( static_cast<const RDORTPRealParamType*>(getType())->getDiap().isExist() )
			{
				fun_calc = new rdoRuntime::RDOFunAlgorithmicDiapCalc( parser()->runtime(), static_cast<const RDORTPRealParamType*>(getType())->getDiap().getMin(), static_cast<const RDORTPRealParamType*>(getType())->getDiap().getMax() );
			}
			break;
		}
	}
	if ( !fun_calc )
	{
		fun_calc = new rdoRuntime::RDOFunAlgorithmicCalc( parser()->runtime() );
	}
	fun_calc->setSrcInfo( src_info() );
	bool default_flag = false;
	bool true_const   = false;
	const rdoRuntime::RDOCalcConst* calc_cond_const = NULL;
	int size = calculateIf.size();
	int cnt = 0;
	for ( int i = 0; i < size; i++ ) {
		rdoRuntime::RDOCalc* logic_calc = calculateIf[i]->condition->getCalc( getType()->typeID() );
		rdoRuntime::RDOCalcConst* calc_cond_last = dynamic_cast<rdoRuntime::RDOCalcConst*>(logic_calc);
		if ( true_const ) {
			parser()->warning( calculateIf[i]->condition->src_info(), rdo::format("Условие не используется: %s", calculateIf[i]->condition->src_text().c_str()) );
			parser()->warning( calc_cond_const->src_info(), rdo::format("Последнее рабочее условие функции: %s", calc_cond_const->src_text().c_str()) );
		} else if ( !calc_cond_last || calc_cond_last->calcValue( parser()->runtime() ).getAsBool() ) {
			// Игнорируем чистые false-условия предыдущей проверкой
			fun_calc->addCalcIf( logic_calc, calculateIf[i]->action->createCalc(getType()) );
			cnt++;
		}
		if ( !default_flag && calc_cond_last && calc_cond_last->calcValue( parser()->runtime() ).getAsBool() ) {
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
			rdoRuntime::RDOCalcConst* calc_act  = new rdoRuntime::RDOCalcConst( parser()->runtime(), getType()->getDefaultValue( getType()->getDV().value() ) );
			calc_cond->setSrcInfo( getType()->src_info() );
			calc_act->setSrcInfo( getType()->src_info() );
			fun_calc->addCalcIf( calc_cond, calc_act );
			default_flag = true;
		}
	}
	if ( !default_flag ) {
		rdoRuntime::RDOCalcConst* calc_cond = new rdoRuntime::RDOCalcConst( parser()->runtime(), 1 );
		rdoRuntime::RDOCalcConst* calc_act  = NULL;
		// Присвоить автоматическое значение по-умолчанию, если оно не задано в явном виде
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
			case rdoRuntime::RDOType::t_bool: {
				calc_act = new rdoRuntime::RDOCalcConst( parser()->runtime(), rdoRuntime::RDOValue(false) );
				break;
			}
			case rdoRuntime::RDOType::t_enum: {
				calc_act = new rdoRuntime::RDOCalcConst( parser()->runtime(), static_cast<const RDORTPEnumParamType*>(getType())->m_enum->getFirstValue() );
				break;
			}
			case rdoRuntime::RDOType::t_string: {
				calc_act = new rdoRuntime::RDOCalcConst( parser()->runtime(), rdoRuntime::RDOValue("") );
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
		case fgt_exist    : setSrcText( "Exist(" + resType->name() + ": " + cond->src_text() + ")" );     calc = new rdoRuntime::RDOFunCalcExist    ( parser()->runtime(), resType->getNumber(), cond->getCalc() ); break;
		case fgt_notexist : setSrcText( "NotExist(" + resType->name() + ": " + cond->src_text() + ")" );  calc = new rdoRuntime::RDOFunCalcNotExist ( parser()->runtime(), resType->getNumber(), cond->getCalc() ); break;
		case fgt_forall   : setSrcText( "ForAll(" + resType->name() + ": " + cond->src_text() + ")" );    calc = new rdoRuntime::RDOFunCalcForAll   ( parser()->runtime(), resType->getNumber(), cond->getCalc() ); break;
		case fgt_notforall: setSrcText( "NotForAll(" + resType->name() + ": " + cond->src_text() + ")" ); calc = new rdoRuntime::RDOFunCalcNotForAll( parser()->runtime(), resType->getNumber(), cond->getCalc() ); break;
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
	select = new rdoRuntime::RDOFunCalcSelect( parser()->runtime(), resType->getNumber(), const_cast<RDOFUNLogic*>(cond)->getCalc() );
	select->setSrcInfo( cond->src_info() );
}

// Групповая функция над выборкой Select'а
RDOFUNLogic* RDOFUNSelect::createFunSelectGroup( RDOFUNGroupLogic::FunGroupType funType, RDOFUNLogic* cond )
{
	rdoRuntime::RDOFunCalcSelectBase* calc;
	switch ( funType ) {
		case RDOFUNGroupLogic::fgt_exist    : setSrcText( src_text() + ".Exist(" + cond->src_text() + ")" );     calc = new rdoRuntime::RDOFunCalcSelectExist    ( parser()->runtime(), select, cond->getCalc() ); break;
		case RDOFUNGroupLogic::fgt_notexist : setSrcText( src_text() + ".NotExist(" + cond->src_text() + ")" );  calc = new rdoRuntime::RDOFunCalcSelectNotExist ( parser()->runtime(), select, cond->getCalc() ); break;
		case RDOFUNGroupLogic::fgt_forall   : setSrcText( src_text() + ".ForAll(" + cond->src_text() + ")" );    calc = new rdoRuntime::RDOFunCalcSelectForAll   ( parser()->runtime(), select, cond->getCalc() ); break;
		case RDOFUNGroupLogic::fgt_notforall: setSrcText( src_text() + ".NotForAll(" + cond->src_text() + ")" ); calc = new rdoRuntime::RDOFunCalcSelectNotForAll( parser()->runtime(), select, cond->getCalc() ); break;
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
	RDOFUNArithm* arithm = new RDOFUNArithm( this, RDOValue(g_int, _size_info), new rdoRuntime::RDOFunCalcSelectSize( parser()->runtime(), select ) );
	arithm->setSrcInfo( _size_info );
	return arithm;
}

} // namespace rdoParse

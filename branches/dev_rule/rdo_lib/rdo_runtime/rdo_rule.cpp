#include "pch.h"
#include "rdo_rule.h"
#include "rdo_runtime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDORule
// ----------------------------------------------------------------------------
RDORule::RDORule( RDORuntime* runtime, RDOPatternRule* pattern, bool trace, const std::string& name ):
	RDOActivityPattern<RDOPatternRule>( runtime, pattern, trace, name ),
	RDORuntimeContainer( runtime ),
	additionalCondition( NULL )
{
	setTraceID( getRuntime()->getFreeActivityId() );
	haveAdditionalCondition = false;
}

RDORule::RDORule( RDORuntime* runtime, RDOPatternRule* pattern, bool trace, RDOCalc* condition, const std::string& name ):
	RDOActivityPattern<RDOPatternRule>( runtime, pattern, trace, name ),
	RDORuntimeContainer( runtime ),
	additionalCondition( condition )
{
	setTraceID( getRuntime()->getFreeActivityId() );
	haveAdditionalCondition = true;
}

bool RDORule::choiceFrom( RDORuntime* runtime )
{ 
	runtime->setCurrentActivity( this );
	if ( haveAdditionalCondition ) {
		if ( !additionalCondition->calcValue( runtime ).getAsBool() ) {
			return false;
		}
	}
	return m_pattern->choiceFrom( runtime ); 
}

void RDORule::convertRule( RDORuntime* runtime ) 
{ 
	runtime->setCurrentActivity( this );
	m_pattern->convertRule( runtime ); 
}

void RDORule::onAfterRule( RDOSimulator* sim, bool inSearch )
{
	updateConvertStatus( sim, m_pattern->m_convertorStatus );
	if ( !inSearch ) {
		getRuntime()->getTracer()->writeRule( this, getRuntime() );
	}
	m_pattern->convertErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
}

bool RDORule::onCheckCondition( RDOSimulator* sim )
/*
Дата внесения изменений: 02.11.08
Автор: Лущан Дмитрий
Смысл изменений: не запускать патерн (рул), если после его выполнения состояние системы не изменилось	
*/
{
	onBeforeChoiceFrom( sim );
	sim->inc_cnt_choice_from();
	bool result = choiceFrom( static_cast<RDORuntime*>(sim) );
	if ( result )
	{
// Сохраняем состояние системы в копию clone и в ней запускаем соответствующий рул
		RDOSimulator* clone = sim->clone();
		onBeforeRule( clone );
		convertRule( static_cast<RDORuntime*>(clone) );
		onAfterRule( clone );
// Сравниваем состояние системы до запуска рула и после
		if ( *clone == *sim )
		{
			result = false;
		}
		delete clone;
	}
	return result;
}

RDOBaseOperation::BOResult RDORule::onDoOperation( RDOSimulator* sim )
{
	onBeforeRule( sim );
	convertRule( static_cast<RDORuntime*>(sim) );
	onAfterRule( sim );
	return RDOBaseOperation::BOR_done;
}

} // namespace rdoRuntime

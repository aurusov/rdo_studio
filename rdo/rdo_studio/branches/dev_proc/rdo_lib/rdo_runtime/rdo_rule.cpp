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
		if ( !additionalCondition->calcValue( runtime ).getBool() ) {
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
{
	onBeforeChoiceFrom( sim );
	sim->inc_cnt_choice_from();
	return choiceFrom( static_cast<RDORuntime*>(sim) );
}

RDOBaseOperation::BOResult RDORule::onDoOperation( RDOSimulator* sim )
{
	onBeforeRule( sim );
	convertRule( static_cast<RDORuntime*>(sim) );
	onAfterRule( sim );
	return RDOBaseOperation::BOR_done;
}

} // namespace rdoRuntime

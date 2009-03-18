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
RDORule::RDORule( RDORuntime* runtime, RDOPatternRule* pattern, bool trace, const std::string& name )
	: RDOActivityPattern<RDOPatternRule>( runtime, pattern, trace, name )
	, RDORuntimeContainer  (runtime)
	, m_additionalCondition(NULL   )
{
	init();
}

RDORule::RDORule( RDORuntime* runtime, RDOPatternRule* pattern, bool trace, RDOCalc* condition, const std::string& name )
	: RDOActivityPattern<RDOPatternRule>( runtime, pattern, trace, name )
	, RDORuntimeContainer  (runtime  )
	, m_additionalCondition(condition)
{
	init();
}

void RDORule::init()
{
	setTraceID( getRuntime()->getFreeActivityId() );
	m_traceOFF = false;
}

bool RDORule::choiceFrom( RDORuntime* runtime )
{ 
	runtime->setCurrentActivity( this );
	if ( m_additionalCondition && !m_additionalCondition->calcValue( runtime ).getAsBool() )
	{
		return false;
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
	if ( !inSearch )
	{
		trace();
	}
	m_pattern->convertErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
}

void RDORule::trace()
{
	if ( !m_traceOFF )
	{
		getRuntime()->getTracer()->writeRule( this, getRuntime() );
	}
}

bool RDORule::onCheckCondition( RDOSimulator* sim )
{
	onBeforeChoiceFrom( sim );
	sim->inc_cnt_choice_from();
	bool result = choiceFrom( static_cast<RDORuntime*>(sim) );
	if ( result )
	{
		m_traceOFF = true;
		std::auto_ptr<RDOSimulator> clone (sim->clone());
		if (onDoOperation(clone.get()) != RDOBaseOperation::BOR_done)
		{
			//! Реакция на плохой onDoOperation - это вообще-то спортный вопрос
			return false;
		}
		if ( *clone.get() == *sim )
		{
			result = false;
		}
		m_traceOFF = false;
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

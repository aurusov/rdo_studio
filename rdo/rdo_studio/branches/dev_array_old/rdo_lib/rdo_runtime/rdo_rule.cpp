/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_rule.cpp
 * author   : Урусов Андрей, Лущан Дмитрий
 * date     : 18.08.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/pch.h"
#include "rdo_lib/rdo_runtime/rdo_rule.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDORule
// ----------------------------------------------------------------------------
RDORule::RDORule( RDORuntime* runtime, RDOPatternRule* pattern, bool trace, const std::string& name )
	: RDOActivityPattern<RDOPatternRule>( runtime, pattern, trace, name )
	, RDOPatternPrior()
	, RDORuntimeContainer  (runtime)
	, m_additionalCondition(NULL   )
{
	init();
}

RDORule::RDORule( RDORuntime* runtime, RDOPatternRule* pattern, bool trace, CREF(LPRDOCalc) pCondition, const std::string& name )
	: RDOActivityPattern<RDOPatternRule>( runtime, pattern, trace, name )
	, RDOPatternPrior()
	, RDORuntimeContainer  (runtime   )
	, m_additionalCondition(pCondition)
{
	init();
}

void RDORule::init()
{
	setTraceID( getRuntime()->getFreeActivityId() );
	m_traceOFF = false;
}

void RDORule::onBeforeChoiceFrom(PTR(rdoRuntime::RDOSimulator) sim)
{
	setPatternParameters(sim);
}

rbool RDORule::choiceFrom(PTR(rdoRuntime::RDORuntime) runtime)
{ 
	runtime->setCurrentActivity( this );
	if ( m_additionalCondition && !m_additionalCondition->calcValue( runtime ).getAsBool() )
	{
		return false;
	}
	return m_pattern->choiceFrom( runtime ); 
}

void RDORule::onBeforeRule(PTR(rdoRuntime::RDOSimulator) sim)
{}

void RDORule::convertRule(PTR(rdoRuntime::RDORuntime) runtime)
{ 
	runtime->setCurrentActivity( this );
	m_pattern->convertRule( runtime ); 
}

void RDORule::onAfterRule(PTR(rdoRuntime::RDOSimulator) sim, rbool inSearch)
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
		if (onDoOperation(clone.get()) != IBaseOperation::BOR_done)
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

IBaseOperation::BOResult RDORule::onDoOperation( RDOSimulator* sim )
{
	onBeforeRule( sim );
	convertRule( static_cast<RDORuntime*>(sim) );
	onAfterRule( sim, false );
	return IBaseOperation::BOR_done;
}

void                     RDORule::onStart     (PTR(rdoRuntime::RDOSimulator) sim)                  {}
void                     RDORule::onStop      (PTR(rdoRuntime::RDOSimulator) sim)                  {}
void                     RDORule::onMakePlaned(PTR(rdoRuntime::RDOSimulator) sim, PTR(void) param) {}
IBaseOperation::BOResult RDORule::onContinue  (PTR(rdoRuntime::RDOSimulator) sim)                  { return IBaseOperation::BOR_cant_run; }

CLOSE_RDO_RUNTIME_NAMESPACE

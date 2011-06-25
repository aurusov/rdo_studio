/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_rule.cpp
 * author   : Урусов Андрей, Лущан Дмитрий
 * date     : 18.08.2010
 * bref     : Продукционные правила
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
RDORule::RDORule(CREF(LPRDORuntime) pRuntime, RDOPatternRule* pattern, bool trace, const std::string& name)
	: RDOActivityPattern<RDOPatternRule>(pattern, trace, name )
	, RDOPatternPrior()
	, m_pRuntime(pRuntime)
	, m_additionalCondition(NULL   )
{
	init();
}

RDORule::RDORule(CREF(LPRDORuntime) pRuntime, RDOPatternRule* pattern, bool trace, CREF(LPRDOCalc) pCondition, const std::string& name)
	: RDOActivityPattern<RDOPatternRule>(pattern, trace, name )
	, RDOPatternPrior()
	, m_pRuntime(pRuntime)
	, m_additionalCondition(pCondition)
{
	init();
}

void RDORule::init()
{
	setTraceID( m_pRuntime->getFreeActivityId() );
	m_traceOFF = false;
}

void RDORule::onBeforeChoiceFrom(CREF(LPRDORuntime) pRuntime)
{
	setPatternParameters(pRuntime);
}

rbool RDORule::choiceFrom(CREF(LPRDORuntime) pRuntime)
{ 
	pRuntime->setCurrentActivity( this );
	if ( m_additionalCondition && !m_additionalCondition->calcValue( pRuntime ).getAsBool() )
	{
		return false;
	}
	return m_pattern->choiceFrom( pRuntime ); 
}

void RDORule::onBeforeRule(CREF(LPRDORuntime) pRuntime)
{}

void RDORule::convertRule(CREF(LPRDORuntime) pRuntime)
{ 
	pRuntime->setCurrentActivity( this );
	m_pattern->convertRule( pRuntime ); 
}

void RDORule::onAfterRule(CREF(LPRDORuntime) pRuntime, rbool inSearch)
{
	updateConvertStatus( pRuntime, m_pattern->m_convertorStatus );
	if ( !inSearch )
	{
		trace();
	}
	m_pattern->convertErase(pRuntime);
	updateRelRes( pRuntime );
}

void RDORule::trace()
{
	if ( !m_traceOFF )
	{
		m_pRuntime->getTracer()->writeRule( this, m_pRuntime );
	}
}

bool RDORule::onCheckCondition( CREF(LPRDORuntime) pRuntime )
{
	onBeforeChoiceFrom( pRuntime );
	pRuntime->inc_cnt_choice_from();
	bool result = choiceFrom(pRuntime);
	if ( result )
	{
		m_traceOFF = true;
		LPRDORuntime pClone = pRuntime->clone();
		ASSERT(pClone);
		if (onDoOperation(pClone) != IBaseOperation::BOR_done)
		{
			//! Реакция на плохой onDoOperation - это вообще-то спортный вопрос
			return false;
		}
		if (pClone->equal(pRuntime))
		{
			result = false;
		}
		m_traceOFF = false;
	}
	return result;
}

IBaseOperation::BOResult RDORule::onDoOperation( CREF(LPRDORuntime) pRuntime )
{
	onBeforeRule( pRuntime );
	convertRule(pRuntime);
	onAfterRule( pRuntime, false );
	return IBaseOperation::BOR_done;
}

void                     RDORule::onStart     (CREF(LPRDORuntime) pRuntime)                  {}
void                     RDORule::onStop      (CREF(LPRDORuntime) pRuntime)                  {}
void                     RDORule::onMakePlaned(CREF(LPRDORuntime) pRuntime, PTR(void) param) {}
IBaseOperation::BOResult RDORule::onContinue  (CREF(LPRDORuntime) pRuntime)                  { return IBaseOperation::BOR_cant_run; }

CLOSE_RDO_RUNTIME_NAMESPACE

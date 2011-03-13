/*
 * copyright: (c) RDO-Team, 2010
 * filename : event_plan.cpp
 * author   : Ћущан ƒмитрий
 * date     : 26.05.2010
 * bref     : RDOCalc дл€ планировани€, запуска и остановки событий и нерегул€рных событий
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/event_plan.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcEvent
// ----------------------------------------------------------------------------
RDOCalcEvent::RDOCalcEvent()
{}

void RDOCalcEvent::setEvent(CREF(LPIBaseOperation) pEvent)
{
	ASSERT(pEvent);
	m_pEvent = pEvent;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcEventPlan
// ----------------------------------------------------------------------------
RDOCalcEventPlan::RDOCalcEventPlan(CREF(LPRDOCalc) pTimeCalc)
	: m_pTimeCalc(pTimeCalc)
{
	ASSERT(m_pTimeCalc);
}

REF(RDOValue) RDOCalcEventPlan::doCalc(PTR(RDORuntime) runtime)
{
	ASSERT(m_pEvent);
	m_value = m_pTimeCalc->calcValue(runtime);
	runtime->addTimePoint(m_value.getDouble(), m_pEvent);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcEventStop
// ----------------------------------------------------------------------------
RDOCalcEventStop::RDOCalcEventStop()
{}

REF(RDOValue) RDOCalcEventStop::doCalc(PTR(RDORuntime) runtime)
{
	ASSERT(m_pEvent);
	runtime->removeTimePoint(m_pEvent);
	m_value = RDOValue(0);
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

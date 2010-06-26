/*
 * copyright: (c) RDO-Team, 2010
 * filename : calc_event_plan.cpp
 * author   : Лущан Дмитрий
 * date     : 26.05.2010
 * bref     : RDOCalc для планирования и остановки событий
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/calc_event_plan.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcEvent
// ----------------------------------------------------------------------------
RDOCalcEvent::RDOCalcEvent()
{}

void RDOCalcEvent::setEvent(CREF(LPIBaseOperation) event)
{
	ASSERT(event);
	m_event = event;
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
	ASSERT(m_event);
	runtime->addTimePoint(m_pTimeCalc->calcValue(runtime).getDouble(), m_event);
	return m_pTimeCalc->calcValue(runtime);
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcEventStop
// ----------------------------------------------------------------------------
RDOCalcEventStop::RDOCalcEventStop()
{}

REF(RDOValue) RDOCalcEventStop::doCalc(PTR(RDORuntime) runtime)
{
	ASSERT(m_event);
	runtime->removeTimePoint(m_event);
	m_value = RDOValue(0);
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

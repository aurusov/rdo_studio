/*
 * copyright: (c) RDO-Team, 2010
 * filename : calc_event_plan.cpp
 * author   : Лущан Дмитрий
 * date     : 26.05.2010
 * bref     : RDOCalc для планирование событий
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
// ---------- RDOCalcEventPlan
// ----------------------------------------------------------------------------
RDOCalcEventPlan::RDOCalcEventPlan(CREF(LPRDOCalc) pTimeCalc)
	: m_pTimeCalc(pTimeCalc)
{
	ASSERT(m_pTimeCalc);
}

void RDOCalcEventPlan::setEvent(CREF(LPIBaseOperation) event)
{
	ASSERT(event);
	m_event = event;
}

REF(RDOValue) RDOCalcEventPlan::doCalc(PTR(RDORuntime) runtime)
{
	ASSERT(m_event);
	runtime->addTimePoint(m_pTimeCalc->calcValue(runtime).getDouble(), m_event);
	return m_pTimeCalc->calcValue(runtime);
}

CLOSE_RDO_RUNTIME_NAMESPACE

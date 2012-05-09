/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_event.cpp
  \author    Ћущан ƒмитрий (dluschan@rk9.bmstu.ru)
  \date      26.05.2010
  \brief     RDOCalc дл€ планировани€, запуска и остановки событий и нерегул€рных событий
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_event.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcEvent
// --------------------------------------------------------------------------------
RDOCalcEvent::RDOCalcEvent()
{}

void RDOCalcEvent::setEvent(CREF(LPIBaseOperation) pEvent)
{
	ASSERT(pEvent);
	m_pEvent = pEvent;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcEventPlan
// --------------------------------------------------------------------------------
RDOCalcEventPlan::RDOCalcEventPlan(CREF(LPRDOCalc) pTimeCalc)
	: m_pTimeCalc(pTimeCalc)
{
	ASSERT(m_pTimeCalc);
}

REF(RDOValue) RDOCalcEventPlan::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ASSERT(m_pEvent);
	m_value = m_pTimeCalc->calcValue(pRuntime);
	pRuntime->addTimePoint(m_value.getDouble(), m_pEvent);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcEventStop
// --------------------------------------------------------------------------------
RDOCalcEventStop::RDOCalcEventStop()
{}

REF(RDOValue) RDOCalcEventStop::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ASSERT(m_pEvent);
	pRuntime->removeTimePoint(m_pEvent);
	m_value = RDOValue(0);
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

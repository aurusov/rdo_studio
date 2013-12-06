/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_event.cpp
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      26.05.2010
  \brief     RDOCalc для планирования, запуска и остановки событий и нерегулярных событий
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
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

void RDOCalcEvent::setEvent(CREF(LPIEvent) pEvent)
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

RDOValue RDOCalcEventPlan::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ASSERT(m_pEvent);
	RDOValue value = m_pTimeCalc->calcValue(pRuntime);
	pRuntime->addTimePoint(
		value.getDouble(),
		m_pEvent,
		boost::bind(&IEvent::onMakePlaned, m_pEvent.get(), pRuntime)
	);
	return value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcEventStop
// --------------------------------------------------------------------------------
RDOCalcEventStop::RDOCalcEventStop()
{}

RDOValue RDOCalcEventStop::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ASSERT(m_pEvent);
	pRuntime->removeTimePoint(m_pEvent);
	return RDOValue();
}

CLOSE_RDO_RUNTIME_NAMESPACE

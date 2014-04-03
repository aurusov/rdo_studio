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
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_event.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcEvent
// --------------------------------------------------------------------------------
RDOCalcEvent::RDOCalcEvent(const LPIEvent& event)
	: m_pEvent(event)
{
	ASSERT(m_pEvent);
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcEventPlan
// --------------------------------------------------------------------------------
RDOCalcEventPlan::RDOCalcEventPlan(const LPIEvent& event, const LPRDOCalc& pTimeCalc, const std::vector<runtime::LPRDOCalc>& params)
	: RDOCalcEvent(event)
	, m_pTimeCalc (pTimeCalc)
	, m_params    (params)
{
	ASSERT(m_pTimeCalc);
}

RDOValue RDOCalcEventPlan::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue time = m_pTimeCalc->calcValue(pRuntime);

	std::vector<RDOValue> params;
	params.reserve(m_params.size());
	BOOST_FOREACH(const LPRDOCalc& param, m_params)
	{
		params.push_back(param->calcValue(pRuntime));
	}

	pRuntime->addTimePoint(
		time.getDouble(),
		m_pEvent,
		boost::bind(&IEvent::onMakePlaned, m_pEvent.get(), pRuntime, params)
	);
	return time;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcEventStop
// --------------------------------------------------------------------------------
RDOCalcEventStop::RDOCalcEventStop(const LPIEvent& event)
	: RDOCalcEvent(event)
{}

RDOValue RDOCalcEventStop::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->removeTimePoint(m_pEvent);
	return RDOValue();
}

CLOSE_RDO_RUNTIME_NAMESPACE

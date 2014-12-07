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

RDOValue RDOCalcEventPlan::doCalc(const LPRDORuntime& pRuntime)
{
	RDOValue time = m_pTimeCalc->calcValue(pRuntime);

	std::vector<RDOValue> params;
	params.reserve(m_params.size());
	for (const LPRDOCalc& param: m_params)
	{
		params.push_back(param->calcValue(pRuntime));
	}

	pRuntime->addTimePoint(
		time.getDouble(),
		m_pEvent.object_dynamic_cast<IBaseOperation>(),
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

RDOValue RDOCalcEventStop::doCalc(const LPRDORuntime& pRuntime)
{
	pRuntime->removeTimePoint(m_pEvent.object_dynamic_cast<IBaseOperation>());
	return RDOValue();
}

CLOSE_RDO_RUNTIME_NAMESPACE

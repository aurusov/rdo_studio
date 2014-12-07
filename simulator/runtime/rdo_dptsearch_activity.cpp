// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_dptsearch_activity.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearchActivity
// --------------------------------------------------------------------------------
RDODPTSearchActivity::RDODPTSearchActivity(const LPIRule& rule, ValueTime valueTime, const LPRDOCalc& pCost)
	: RDOTraceableObject(false    )
	, m_rule            (rule     )
	, m_pCost           (pCost    )
	, m_valueTime       (valueTime)
{
	LPITrace trace = m_rule.object_dynamic_cast<ITrace>();
	ASSERT(trace);
	setTraceID(trace->getTraceID());
}

LPIRule& RDODPTSearchActivity::rule()
{
	return m_rule;
}

double RDODPTSearchActivity::cost(const LPRDORuntime& pRuntime)
{
	return m_pCost->calcValue(pRuntime).getDouble();
}

IDPTSearchActivity::ValueTime RDODPTSearchActivity::valueTime() const
{
	return m_valueTime;
}

CLOSE_RDO_RUNTIME_NAMESPACE

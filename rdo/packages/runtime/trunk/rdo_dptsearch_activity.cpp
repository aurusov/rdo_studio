/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_dptsearch_activity.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      08.08.2009
  \brief     Активности точки принятия решения DPTSearch
  \indent    4T
*/

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
RDODPTSearchActivity::RDODPTSearchActivity(CREF(LPIRule) rule, ValueTime valueTime, CREF(LPRDOCalc) pCost)
	: RDOTraceableObject(false    )
	, m_rule            (rule     )
	, m_pCost           (pCost    )
	, m_valueTime       (valueTime)
{
	LPITrace trace = m_rule;
	ASSERT(trace);
	setTraceID(trace->getTraceID());
}

REF(LPIRule) RDODPTSearchActivity::rule()
{
	return m_rule;
}

double RDODPTSearchActivity::cost(CREF(LPRDORuntime) pRuntime)
{
	return m_pCost->calcValue(pRuntime).getDouble();
}

IDPTSearchActivity::ValueTime RDODPTSearchActivity::valueTime() const
{
	return m_valueTime;
}

CLOSE_RDO_RUNTIME_NAMESPACE

/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_dptsearch_activity.cpp
 * author   : Урусов Андрей
 * date     : 08.08.2009
 * bref     : 
 * indent   : 4T
 */

// =========================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_lib/rdo_runtime/rdo_dptsearch_activity.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

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

double RDODPTSearchActivity::cost(PTR(RDOSimulator) runtime)
{
	return m_pCost->calcValue(static_cast<PTR(RDORuntime)>(runtime)).getDouble();
}

IDPTSearchActivity::ValueTime RDODPTSearchActivity::valueTime() const
{
	return m_valueTime;
}

CLOSE_RDO_RUNTIME_NAMESPACE

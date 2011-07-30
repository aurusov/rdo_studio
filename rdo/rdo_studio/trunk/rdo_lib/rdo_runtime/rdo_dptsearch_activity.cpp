/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdo_dptsearch_activity.cpp
 * @authors   Урусов Андрей
 * @date      08.08.2009
 * @brief     
 * @indent    4T
 */

// =========================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_lib/rdo_runtime/rdo_dptsearch_activity.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ********************************************************************************

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

double RDODPTSearchActivity::cost(CREF(LPRDORuntime) pRuntime)
{
	return m_pCost->calcValue(pRuntime).getDouble();
}

IDPTSearchActivity::ValueTime RDODPTSearchActivity::valueTime() const
{
	return m_valueTime;
}

CLOSE_RDO_RUNTIME_NAMESPACE

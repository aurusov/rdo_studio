/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      watch.cpp
 * @author    Урусов Андрей
 * @date      04.03.2011
 * @brief     Собираемые показатели
 * @indent    4T
 *********************************************************************************/

// ---------------------------------------------------------------------------- PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/watch.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcWatchGroupStart
// --------------------------------------------------------------------------------
RDOCalcWatchGroupStart::RDOCalcWatchGroupStart(CREF(LPIPokazGroup) pResultGroup)
	: m_pResultGroup(pResultGroup)
{
	m_value = RDOValue(0);
}

RDOCalcWatchGroupStart::~RDOCalcWatchGroupStart()
{}

REF(RDOValue) RDOCalcWatchGroupStart::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ASSERT(m_pResultGroup);
	m_pResultGroup->onStart(pRuntime);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcWatchGroupStop
// --------------------------------------------------------------------------------
RDOCalcWatchGroupStop::RDOCalcWatchGroupStop(CREF(LPIPokazGroup) pResultGroup)
	: m_pResultGroup(pResultGroup)
{
	m_value = RDOValue(0);
}

RDOCalcWatchGroupStop::~RDOCalcWatchGroupStop()
{}

REF(RDOValue) RDOCalcWatchGroupStop::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ASSERT(m_pResultGroup);
	m_pResultGroup->onStop(pRuntime);
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

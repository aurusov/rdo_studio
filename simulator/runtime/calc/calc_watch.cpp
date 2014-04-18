/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_watch.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      04.03.2011
  \brief     Собираемые показатели
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_watch.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcWatchGroupStart
// --------------------------------------------------------------------------------
RDOCalcWatchGroupStart::RDOCalcWatchGroupStart(const LPIResultGroup& pResultGroup)
	: m_pResultGroup(pResultGroup)
{}

RDOCalcWatchGroupStart::~RDOCalcWatchGroupStart()
{}

RDOValue RDOCalcWatchGroupStart::doCalc(const LPRDORuntime& pRuntime)
{
	ASSERT(m_pResultGroup);
	m_pResultGroup->onStart(pRuntime);
	return RDOValue();
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcWatchGroupStop
// --------------------------------------------------------------------------------
RDOCalcWatchGroupStop::RDOCalcWatchGroupStop(const LPIResultGroup& pResultGroup)
	: m_pResultGroup(pResultGroup)
{}

RDOCalcWatchGroupStop::~RDOCalcWatchGroupStop()
{}

RDOValue RDOCalcWatchGroupStop::doCalc(const LPRDORuntime& pRuntime)
{
	ASSERT(m_pResultGroup);
	m_pResultGroup->onStop(pRuntime);
	return RDOValue();
}

CLOSE_RDO_RUNTIME_NAMESPACE

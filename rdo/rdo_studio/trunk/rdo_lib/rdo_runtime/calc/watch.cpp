/*
 * copyright: (c) RDO-Team, 2011
 * filename : watch.cpp
 * author   : Урусов Андрей
 * date     : 04.03.2011
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/watch.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcResultGroupStart
// ----------------------------------------------------------------------------
RDOCalcResultGroupStart::RDOCalcResultGroupStart(CREF(LPIPokazGroup) pResultGroup)
	: m_pResultGroup(pResultGroup)
{
	m_value = RDOValue(0);
}

RDOCalcResultGroupStart::~RDOCalcResultGroupStart()
{}

REF(RDOValue) RDOCalcResultGroupStart::doCalc(PTR(RDORuntime) pRuntime)
{
	ASSERT(m_pResultGroup);
	m_pResultGroup->onStart(pRuntime);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcResultGroupStop
// ----------------------------------------------------------------------------
RDOCalcResultGroupStop::RDOCalcResultGroupStop(CREF(LPIPokazGroup) pResultGroup)
	: m_pResultGroup(pResultGroup)
{
	m_value = RDOValue(0);
}

RDOCalcResultGroupStop::~RDOCalcResultGroupStop()
{}

REF(RDOValue) RDOCalcResultGroupStop::doCalc(PTR(RDORuntime) pRuntime)
{
	ASSERT(m_pResultGroup);
	m_pResultGroup->onStop(pRuntime);
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

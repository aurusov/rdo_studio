/*
 * copyright: (c) RDO-Team, 2011
 * filename : locvar.cpp
 * author   : Чирков Михаил
 * date     : 16.04.11
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/locvar.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateLocalVariable
// ----------------------------------------------------------------------------
RDOCalcCreateLocalVariable::RDOCalcCreateLocalVariable(CREF(tstring) name)
	: m_name(name)
{}

REF(RDOValue) RDOCalcCreateLocalVariable::doCalc(PTR(RDORuntime) pRuntime)
{
	pRuntime->getMemoryStack()->create(m_name);
	m_value = RDOValue();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcInitLocalVariable
// ----------------------------------------------------------------------------
RDOCalcInitLocalVariable::RDOCalcInitLocalVariable(CREF(tstring) name, CREF(LPRDOCalc) pCalc)
	: m_name (name )
	, m_pCalc(pCalc)
{}

REF(RDOValue) RDOCalcInitLocalVariable::doCalc(PTR(RDORuntime) pRuntime)
{
	pRuntime->getMemoryStack()->set(m_name, m_pCalc->calcValue(pRuntime));
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetLocalVariable
// ----------------------------------------------------------------------------
RDOCalcGetLocalVariable::RDOCalcGetLocalVariable(CREF(tstring) name)
	: m_name(name)
{}

REF(RDOValue) RDOCalcGetLocalVariable::doCalc(PTR(RDORuntime) pRuntime)
{
	m_value = pRuntime->getMemoryStack()->get(m_name);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcLocalVariableList
// ----------------------------------------------------------------------------
RDOCalcLocalVariableList::RDOCalcLocalVariableList()
{}

void RDOCalcLocalVariableList::addCalcLocalVariable(CREF(LPRDOCalc) pCalc)
{
	ASSERT(pCalc);
	m_calcLocalVariableList.push_back(pCalc);
}

REF(RDOValue) RDOCalcLocalVariableList::doCalc(PTR(RDORuntime) pRuntime)
{
	STL_FOR_ALL(m_calcLocalVariableList, calc_it)
	{
		(*calc_it)->calcValue(pRuntime);
	}

	m_value = RDOValue(m_calcLocalVariableList.size());
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

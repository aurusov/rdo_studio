/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_locvar.cpp
  \author    Чирков Михаил
  \date      16.04.2011
  \brief     Локальные переменные
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_locvar.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcCreateLocalVariable
// --------------------------------------------------------------------------------
RDOCalcCreateLocalVariable::RDOCalcCreateLocalVariable(CREF(tstring) name, CREF(LPRDOCalc) pValueCalc)
	: m_name      (name      )
	, m_pValueCalc(pValueCalc)
{
	ASSERT(m_pValueCalc);
}

REF(RDOValue) RDOCalcCreateLocalVariable::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = m_pValueCalc->calcValue(pRuntime);
	pRuntime->getMemoryStack()->create(m_name, m_value);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetLocalVariable
// --------------------------------------------------------------------------------
RDOCalcGetLocalVariable::RDOCalcGetLocalVariable(CREF(tstring) name)
	: m_name(name)
{}

REF(RDOValue) RDOCalcGetLocalVariable::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = pRuntime->getMemoryStack()->get(m_name);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcLocalVariableList
// --------------------------------------------------------------------------------
RDOCalcLocalVariableList::RDOCalcLocalVariableList()
{}

void RDOCalcLocalVariableList::addCalcLocalVariable(CREF(LPRDOCalc) pCalc)
{
	ASSERT(pCalc);
	m_calcLocalVariableList.push_back(pCalc);
}

REF(RDOValue) RDOCalcLocalVariableList::doCalc(CREF(LPRDORuntime) pRuntime)
{
	STL_FOR_ALL(m_calcLocalVariableList, calc_it)
	{
		(*calc_it)->calcValue(pRuntime);
	}

	m_value = RDOValue(m_calcLocalVariableList.size());
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

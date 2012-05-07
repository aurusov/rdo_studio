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

void RDOCalcCreateLocalVariable::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pValueCalc->calcValue(pRuntime);
	pRuntime->getMemoryStack()->create(m_name, pRuntime->stack().pop());
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetLocalVariable
// --------------------------------------------------------------------------------
RDOCalcGetLocalVariable::RDOCalcGetLocalVariable(CREF(tstring) name)
	: m_name(name)
{}

void RDOCalcGetLocalVariable::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->stack().push(pRuntime->getMemoryStack()->get(m_name));
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

void RDOCalcLocalVariableList::doCalc(CREF(LPRDORuntime) pRuntime)
{
	STL_FOR_ALL(m_calcLocalVariableList, calc_it)
	{
		(*calc_it)->calcValue(pRuntime);
	}
}

CLOSE_RDO_RUNTIME_NAMESPACE

/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_braces.cpp
  \author    Чирков Михаил
  \date      16.04.2011
  \brief     Список инструкций в стиле C++
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_braces.h"
#include "simulator/runtime/calc/procedural/calc_statement.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcOpenBrace
// --------------------------------------------------------------------------------
RDOCalcOpenBrace::RDOCalcOpenBrace()
{}

REF(RDOValue) RDOCalcOpenBrace::doCalc(CREF(LPRDORuntime) pRuntime)
{
	LPRDOMemory pLocalMemory = rdo::Factory<RDOMemory>::create();
	pRuntime->getMemoryStack()->push(pLocalMemory);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcCloseBrace
// --------------------------------------------------------------------------------
RDOCalcCloseBrace::RDOCalcCloseBrace()
{}

REF(RDOValue) RDOCalcCloseBrace::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->getMemoryStack()->pop();
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFunEnd
// --------------------------------------------------------------------------------
RDOCalcFunEnd::RDOCalcFunEnd()
{}

REF(RDOValue) RDOCalcFunEnd::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->getMemoryStack()->pop();
	pRuntime->setFunBreakFlag(RDORuntime::FBF_NONE);
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

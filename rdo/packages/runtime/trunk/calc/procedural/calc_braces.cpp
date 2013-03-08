/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_braces.cpp
  \author    Чирков Михаил
  \date      16.04.2011
  \brief     Список инструкций в стиле C++
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
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

RDOValue RDOCalcOpenBrace::doCalc(CREF(LPRDORuntime) pRuntime)
{
	LPRDOMemory pLocalMemory = rdo::Factory<RDOMemory>::create();
	pRuntime->getMemoryStack()->push(pLocalMemory);
	return RDOValue();
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcCloseBrace
// --------------------------------------------------------------------------------
RDOCalcCloseBrace::RDOCalcCloseBrace()
{}

RDOValue RDOCalcCloseBrace::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->getMemoryStack()->pop();
	return RDOValue();
}

CLOSE_RDO_RUNTIME_NAMESPACE

/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_function_system.cpp
  \author    Барс Александр
  \date      20.12.2011
  \brief     Системные функции
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/function/calc_function_system.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetTimeNow
// --------------------------------------------------------------------------------
RDOValue RDOCalcGetTimeNow::doCalc(const LPRDORuntime& pRuntime)
{
	return pRuntime->getTimeNow();
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetSeconds
// --------------------------------------------------------------------------------
RDOValue RDOCalcGetSeconds::doCalc(const LPRDORuntime& pRuntime)
{
	return pRuntime->getSeconds();
}

CLOSE_RDO_RUNTIME_NAMESPACE

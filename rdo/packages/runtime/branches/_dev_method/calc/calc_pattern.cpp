/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_pattern.cpp
  \author    Барс Александр
  \date      20.12.2011
  \brief     Калки для паттернов
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_pattern.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcPatParam
// --------------------------------------------------------------------------------
RDOCalcPatParam::RDOCalcPatParam(ruint paramID)
	: m_paramID(paramID)
{}

RDOValue RDOCalcPatParam::doCalc(CREF(LPRDORuntime) pRuntime)
{
	return pRuntime->getPatternParameter(m_paramID);
}

CLOSE_RDO_RUNTIME_NAMESPACE

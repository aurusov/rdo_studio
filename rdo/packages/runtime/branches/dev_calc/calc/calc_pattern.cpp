/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_pattern.cpp
  \author    Барс Александр
  \date      20.12.2011
  \brief     Калки для паттернов
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_pattern.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOSetPatternParamCalc
// --------------------------------------------------------------------------------
RDOSetPatternParamCalc::RDOSetPatternParamCalc(ruint paramID, CREF(LPRDOCalc) pCalc)
	: m_paramID(paramID)
	, m_pCalc  (pCalc  )
{
	ASSERT(m_pCalc);
}

void RDOSetPatternParamCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pCalc->calcValue(pRuntime);
	pRuntime->setPatternParameter(m_paramID, pRuntime->stack().pop());
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcPatParam
// --------------------------------------------------------------------------------
RDOCalcPatParam::RDOCalcPatParam(ruint paramID)
	: m_paramID(paramID)
{}

void RDOCalcPatParam::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->stack().push(pRuntime->getPatternParameter(m_paramID));
}

CLOSE_RDO_RUNTIME_NAMESPACE

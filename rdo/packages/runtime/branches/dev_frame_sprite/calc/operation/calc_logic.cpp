/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_logic.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     Логические операторы
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/operation/calc_logic.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcAnd
// --------------------------------------------------------------------------------
RDOValue RDOCalcAnd::doCalc(CREF(LPRDORuntime) pRuntime)
{
	++OperatorType::getCalcCounter<OperatorType::Type(calc_type)>();

	if (!m_pLeft->calcValue(pRuntime).getAsBool())
		return m_value_false;

	if (!m_pRight->calcValue(pRuntime).getAsBool())
		return m_value_false;

	return m_value_true;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcAnd
// --------------------------------------------------------------------------------
RDOValue RDOCalcOr::doCalc(CREF(LPRDORuntime) pRuntime)
{
	++OperatorType::getCalcCounter<OperatorType::Type(calc_type)>();

	if (m_pLeft->calcValue(pRuntime).getAsBool())
		return m_value_true;

	if (m_pRight->calcValue(pRuntime).getAsBool())
		return m_value_true;

	return m_value_false;
}

CLOSE_RDO_RUNTIME_NAMESPACE

/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_arithm.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     Арифметические операции
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/calc/operation/operation_type.h"
#include "simulator/runtime/calc/operation/calc_arithm.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcDiv
// --------------------------------------------------------------------------------
RDOValue RDOCalcDiv::doCalc(CREF(LPRDORuntime) pRuntime)
{
	++OperatorType::getCalcCounter<OperatorType::Type(calc_type)>();
	RDOValue right = m_pRight->calcValue(pRuntime);
	if (right == 0)
	{
		pRuntime->error().push(_T("Деление на ноль"), srcInfo());
	}
	return m_pLeft->calcValue(pRuntime) / right;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcPlusEnumSafe
// --------------------------------------------------------------------------------
RDOValue RDOCalcPlusEnumSafe::doCalc(CREF(LPRDORuntime) pRuntime)
{
	++OperatorType::getCalcCounter<OperatorType::Type(calc_type)>();
	return m_pLeft->calcValue(pRuntime).getEnumAsInt() + m_pRight->calcValue(pRuntime).getEnumAsInt();
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcMultEnumSafe
// --------------------------------------------------------------------------------
RDOValue RDOCalcMultEnumSafe::doCalc(CREF(LPRDORuntime) pRuntime)
{
	++OperatorType::getCalcCounter<OperatorType::Type(calc_type)>();
	return m_pLeft->calcValue(pRuntime).getEnumAsInt() * m_pRight->calcValue(pRuntime).getEnumAsInt();
}

CLOSE_RDO_RUNTIME_NAMESPACE

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
void RDOCalcDiv::doCalc(CREF(LPRDORuntime) pRuntime)
{
	++OperatorType::getCalcCounter<OperatorType::Type(calc_type)>();
	m_pRight->calcValue(pRuntime);
	RDOValue right = pRuntime->stack().pop();
	if (right == 0)
	{
		pRuntime->error().push(_T("Деление на ноль"), srcInfo());
	}
	m_pLeft->calcValue(pRuntime);
	pRuntime->stack().push(pRuntime->stack().pop() / right);
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcPlusEnumSafe
// --------------------------------------------------------------------------------
void RDOCalcPlusEnumSafe::doCalc(CREF(LPRDORuntime) pRuntime)
{
	++OperatorType::getCalcCounter<OperatorType::Type(calc_type)>();

	m_pLeft->calcValue(pRuntime);
	rsint left = pRuntime->stack().pop().getEnumAsInt();

	m_pRight->calcValue(pRuntime);
	rsint right = pRuntime->stack().pop().getEnumAsInt();

	pRuntime->stack().push(left + right);
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcMultEnumSafe
// --------------------------------------------------------------------------------
void RDOCalcMultEnumSafe::doCalc(CREF(LPRDORuntime) pRuntime)
{
	++OperatorType::getCalcCounter<OperatorType::Type(calc_type)>();

	m_pLeft->calcValue(pRuntime);
	rsint left = pRuntime->stack().pop().getEnumAsInt();

	m_pRight->calcValue(pRuntime);
	rsint right = pRuntime->stack().pop().getEnumAsInt();

	pRuntime->stack().push(left * right);
}

CLOSE_RDO_RUNTIME_NAMESPACE

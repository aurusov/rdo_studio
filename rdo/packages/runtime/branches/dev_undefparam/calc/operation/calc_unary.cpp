/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_unary.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     Унарные операторы
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/operation/calc_unary.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- Унарные операции
// --------------------------------------------------------------------------------
REF(RDOValue) RDOCalcDoubleToIntByResult::doCalc(CREF(LPRDORuntime) pRuntime)
{
	REF(RDOValue) value = m_pOperation->calcValue(pRuntime);
	m_value = m_round
		? value.getInt()
		: value;
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcInt (приведение к целому)
// --------------------------------------------------------------------------------
REF(RDOValue) RDOCalcInt::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue res = m_pOperation->calcValue(pRuntime);
	m_value = res > 0 ? RDOValue((int)(res.getDouble() + 0.5)) : RDOValue((int)(res.getDouble() - 0.5));
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

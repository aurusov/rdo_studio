/*!
  \copyright (c) RDO-Team, 2011
  \file      unary.cpp
  \authors   Барс Александр, Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     Унарные операторы
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/unary.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- Унарные операции
// --------------------------------------------------------------------------------
REF(RDOValue) RDOCalcDoubleToIntByResult::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = m_round ? RDOValue(m_pOperation->calcValue(pRuntime).getInt()) : m_pOperation->calcValue(pRuntime);
	return m_value;
}

REF(RDOValue) RDOCalcCheckDiap::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = m_pOperation->calcValue(pRuntime);
	if (m_value < m_min_value || m_value > m_max_value)
	{
		pRuntime->error(rdo::format(_T("Значение выходит за допустимый диапазон [%s..%s]: %s"), m_min_value.getAsString().c_str(), m_max_value.getAsString().c_str(), m_value.getAsString().c_str()), this);
	}
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

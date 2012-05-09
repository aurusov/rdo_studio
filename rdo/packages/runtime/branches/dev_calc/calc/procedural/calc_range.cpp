/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_range.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      26.12.2011
  \brief     Проверка на диапазон
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_range.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcCheckRange
// --------------------------------------------------------------------------------
RDOCalcCheckRange::RDOCalcCheckRange(CREF(RDOValue) minValue, CREF(RDOValue) maxValue, CREF(LPRDOCalc) pCalc)
	: m_minValue(minValue)
	, m_maxValue(maxValue)
	, m_pCalc   (pCalc   )
{
	if (m_pCalc)
	{
		setSrcInfo(m_pCalc->srcInfo());
	}
}

RDOValue RDOCalcCheckRange::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = m_pCalc->calcValue(pRuntime);
	if (m_value < m_minValue || m_value > m_maxValue)
	{
		pRuntime->error().push(rdo::format(_T("Значение выходит за допустимый диапазон [%s..%s]: %s"), m_minValue.getAsString().c_str(), m_maxValue.getAsString().c_str(), m_value.getAsString().c_str()), srcInfo());
	}
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

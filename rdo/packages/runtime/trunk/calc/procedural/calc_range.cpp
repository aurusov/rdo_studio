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
#include "simulator/runtime/pch/stdpch.h"
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
	RDOValue value = m_pCalc->calcValue(pRuntime);
	if (value < m_minValue || value > m_maxValue)
	{
		pRuntime->error().push(rdo::format("Значение выходит за допустимый диапазон [%s..%s]: %s", m_minValue.getAsString().c_str(), m_maxValue.getAsString().c_str(), value.getAsString().c_str()), srcInfo());
	}
	return value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

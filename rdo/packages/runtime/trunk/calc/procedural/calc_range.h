/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_range.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      26.12.2011
  \brief     Проверка на диапазон
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_PROCEDURAL_RANGE_H_
#define _LIB_RUNTIME_CALC_PROCEDURAL_RANGE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Проверка на диапазон
CALC(RDOCalcCheckRange)
{
DECLARE_FACTORY(RDOCalcCheckRange)
private:
	RDOCalcCheckRange(CREF(RDOValue) minValue, CREF(RDOValue) maxValue, CREF(LPRDOCalc) pCalc);

	RDOValue   m_minValue;
	RDOValue   m_maxValue;
	LPRDOCalc  m_pCalc;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_PROCEDURAL_RANGE_H_

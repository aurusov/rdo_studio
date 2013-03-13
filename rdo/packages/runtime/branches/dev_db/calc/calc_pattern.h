/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_pattern.h
  \author    Барс Александр
  \date      20.12.2011
  \brief     Калки для паттернов
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_PATTERN_H_
#define _LIB_RUNTIME_CALC_PATTERN_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Установка параметра паттерна
CALC(RDOSetPatternParamCalc)
{
DECLARE_FACTORY(RDOSetPatternParamCalc)
private:
	RDOSetPatternParamCalc(ruint paramID, CREF(LPRDOCalc) pCalc);

	ruint      m_paramID;
	LPRDOCalc  m_pCalc;

	DECLARE_ICalc;
};

//! Получение параметра паттерна
CALC(RDOCalcPatParam)
{
DECLARE_FACTORY(RDOCalcPatParam)
private:
	RDOCalcPatParam(ruint paramID);

	ruint  m_paramID;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_PATTERN_H_

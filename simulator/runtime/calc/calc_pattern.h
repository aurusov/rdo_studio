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

//! Получение параметра паттерна
PREDECLARE_POINTER(RDOCalcPatParam);
class RDOCalcPatParam: public RDOCalc
{
DECLARE_FACTORY(RDOCalcPatParam)
private:
	RDOCalcPatParam(std::size_t paramID);

	std::size_t m_paramID;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_PATTERN_H_

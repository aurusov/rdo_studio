/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_function_system.h
  \author    Барс Александр
  \date      20.12.2011
  \brief     Системные функции
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_FUNCTION_SYSTEM_H_
#define _LIB_RUNTIME_CALC_FUNCTION_SYSTEM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Получение текущего модельного времени
CALC(RDOCalcGetTimeNow)
{
DECLARE_FACTORY(RDOCalcGetTimeNow)
DECLARE_ICalc;
};

//! Получение реального времени работы модели
CALC(RDOCalcGetSeconds)
{
DECLARE_FACTORY(RDOCalcGetSeconds)
DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_FUNCTION_SYSTEM_H_

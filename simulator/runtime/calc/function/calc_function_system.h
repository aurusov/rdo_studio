#ifndef _LIB_RUNTIME_CALC_FUNCTION_SYSTEM_H_
#define _LIB_RUNTIME_CALC_FUNCTION_SYSTEM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOCalcGetTimeNow);
class RDOCalcGetTimeNow: public RDOCalc
{
DECLARE_FACTORY(RDOCalcGetTimeNow)
DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcGetSeconds);
class RDOCalcGetSeconds: public RDOCalc
{
DECLARE_FACTORY(RDOCalcGetSeconds)
DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_FUNCTION_SYSTEM_H_

#pragma once

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

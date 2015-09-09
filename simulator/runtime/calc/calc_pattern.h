#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

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

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_pattern.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcPatParam
// --------------------------------------------------------------------------------
RDOCalcPatParam::RDOCalcPatParam(std::size_t paramID)
    : m_paramID(paramID)
{}

RDOValue RDOCalcPatParam::doCalc(const LPRDORuntime& pRuntime)
{
    return pRuntime->getPatternParameter(m_paramID);
}

CLOSE_RDO_RUNTIME_NAMESPACE

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_nop.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcNop
// --------------------------------------------------------------------------------
RDOCalcNop::RDOCalcNop()
{}

RDOCalcNop::~RDOCalcNop()
{}

RDOValue RDOCalcNop::doCalc(const LPRDORuntime& /*pRuntime*/)
{
    return RDOValue();
}

CLOSE_RDO_RUNTIME_NAMESPACE

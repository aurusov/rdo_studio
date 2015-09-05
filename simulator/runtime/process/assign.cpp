// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/process/rdoprocess.h"
#include "simulator/runtime/process/assign.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/resource/calc_relevant.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCAssign
// --------------------------------------------------------------------------------
RDOPROCAssign::RDOPROCAssign(LPIPROCProcess pProcess, const LPRDOCalc& pCalc)
    : RDOPROCBlock(pProcess)
    , pAssignCalc (pCalc   )
{}

bool RDOPROCAssign::onCheckCondition(const LPRDORuntime& /*pRuntime*/)
{
    return !m_transacts.empty();
}

IBaseOperation::ResultCode RDOPROCAssign::onDoOperation(const LPRDORuntime& pRuntime)
{
    pAssignCalc->calcValue(pRuntime);
    TRACE1("%7.1f ASSIGN\n", pRuntime->getCurrentTime());
    m_transacts.front()->next();
    return IBaseOperation::ResultCode::DONE;
}

void RDOPROCAssign::onStart(const LPRDORuntime& /*pRuntime*/)
{}

void RDOPROCAssign::onStop(const LPRDORuntime& /*pRuntime*/)
{}

IBaseOperation::ResultCode RDOPROCAssign::onContinue(const LPRDORuntime& /*pRuntime*/)
{
    return IBaseOperation::ResultCode::CANNOT_RUN;
}

CLOSE_RDO_RUNTIME_NAMESPACE

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdodptrtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearchRuntime
// --------------------------------------------------------------------------------
RDODPTSearchRuntime::RDODPTSearchRuntime(const LPRDORuntime& pRuntime, LPIBaseOperationContainer parent, const LPRDOCalc& _pCondition, const LPRDOCalc& _pTermCondition, const LPRDOCalc& _pEvaluateBy, bool _compTops, RDODPTSearchTrace::TraceFlag _traceFlag)
    : RDODPTSearchTrace(pRuntime, parent)
    , pCondition       (_pCondition     )
    , pTermCondition   (_pTermCondition )
    , pEvaluateBy      (_pEvaluateBy    )
    , compTops         (_compTops       )
{
    setTraceID(pRuntime->getFreeDPTId());
    traceFlag = _traceFlag;
}

bool RDODPTSearchRuntime::onCheckCondition(const LPRDORuntime& pRuntime)
{
    return pCondition->calcValue(pRuntime).getAsBool();
}

bool RDODPTSearchRuntime::TermCondition(const LPRDORuntime& pRuntime)
{
    return pTermCondition->calcValue(pRuntime).getAsBool();
}

double RDODPTSearchRuntime::EvaluateBy(const LPRDORuntime& pRuntime)
{
    return pEvaluateBy->calcValue(pRuntime).getDouble();
}

bool RDODPTSearchRuntime::NeedCompareTops()
{
    return compTops;
}

CLOSE_RDO_RUNTIME_NAMESPACE

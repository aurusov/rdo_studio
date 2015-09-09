#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <sstream>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/searchtrace.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDODPTSearchRuntime: public RDODPTSearchTrace
{
DECLARE_FACTORY(RDODPTSearchRuntime);

private:
    RDODPTSearchRuntime(const LPRDORuntime& pRuntime, LPIBaseOperationContainer parent, const LPRDOCalc& _pCondition, const LPRDOCalc& _pTermCondition, const LPRDOCalc& _pEvaluateBy, bool _compTops, RDODPTSearchTrace::TraceFlag _traceFlag);

    LPRDOCalc pCondition;
    LPRDOCalc pTermCondition;
    LPRDOCalc pEvaluateBy;
    bool compTops;

    virtual bool onCheckCondition(const LPRDORuntime& pRuntime);
    virtual bool TermCondition(const LPRDORuntime& pRuntime);
    virtual double EvaluateBy(const LPRDORuntime& pRuntime);
    virtual bool NeedCompareTops();
};

CLOSE_RDO_RUNTIME_NAMESPACE

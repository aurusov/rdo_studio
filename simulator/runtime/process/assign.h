#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/namespace.h"
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/process/rdoprocess_i.h"
#include "simulator/runtime/process/rdoprocess.h"
#include "simulator/runtime/rdo_logic.h"
#include "simulator/runtime/rdo_res_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOPROCAssign: public RDOPROCBlock
{
DECLARE_FACTORY(RDOPROCAssign);
private:
    RDOPROCAssign(LPIPROCProcess pProcess, const LPRDOCalc& pCalc);

    LPRDOCalc pAssignCalc;

    DECLARE_IBaseOperation;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_logic.h"
#include "simulator/runtime/rdo_priority.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDODPTSome: public RDOLogicSimple, public RDOPatternPrior
{
DECLARE_FACTORY(RDODPTSome);
private:
	RDODPTSome(const LPRDORuntime& pRuntime, LPIBaseOperationContainer parent = NULL);
	virtual ~RDODPTSome();
};

CLOSE_RDO_RUNTIME_NAMESPACE

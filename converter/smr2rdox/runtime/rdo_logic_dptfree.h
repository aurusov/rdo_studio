#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_logic.h"
#include "simulator/runtime/rdo_priority.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDODPTFree: public RDOLogicSimple, public RDOPatternPrior
{
DECLARE_FACTORY(RDODPTFree);
private:
	RDODPTFree(const LPRDORuntime& pRuntime);
	virtual ~RDODPTFree();
};

CLOSE_RDO_RUNTIME_NAMESPACE

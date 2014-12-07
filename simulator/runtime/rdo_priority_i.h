#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOCalc)

CLOSE_RDO_RUNTIME_NAMESPACE

class IPriority: public virtual rdo::counter_reference
{
public:
	virtual rdo::runtime::LPRDOCalc getPrior()                   = 0;
	virtual bool setPrior(const rdo::runtime::LPRDOCalc& pPrior) = 0;
};
DECLARE_POINTER(IPriority)

#define DECLARE_IPriority                       \
	virtual rdo::runtime::LPRDOCalc getPrior(); \
	virtual bool setPrior(const rdo::runtime::LPRDOCalc& pPrior);

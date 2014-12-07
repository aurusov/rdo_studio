#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_simulator.h"
#include "simulator/runtime/result/result_i.h"
// --------------------------------------------------------------------------------

class IResultGroup: public virtual rdo::counter_reference
{
public:
	virtual void onStart (const rdo::runtime::LPRDORuntime& pRuntime) = 0;
	virtual void onStop  (const rdo::runtime::LPRDORuntime& pRuntime) = 0;
	virtual void onAppend(const LPIResult&                  pResult ) = 0;
};
DECLARE_POINTER(IResultGroup)

#define DECLARE_IResultGroup                                   \
	void onStart (const rdo::runtime::LPRDORuntime& pRuntime); \
	void onStop  (const rdo::runtime::LPRDORuntime& pRuntime); \
	void onAppend(const LPIResult&                  pResult );

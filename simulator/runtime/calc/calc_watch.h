#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/result/result_group_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOCalcWatchGroupStart);
class RDOCalcWatchGroupStart: public RDOCalc
{
DECLARE_FACTORY(RDOCalcWatchGroupStart)
protected:
	RDOCalcWatchGroupStart(const LPIResultGroup& pResultGroup);
	virtual ~RDOCalcWatchGroupStart();

	LPIResultGroup m_pResultGroup;

	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcWatchGroupStop);
class RDOCalcWatchGroupStop: public RDOCalc
{
DECLARE_FACTORY(RDOCalcWatchGroupStop)
protected:
	RDOCalcWatchGroupStop(const LPIResultGroup& pResultGroup);
	virtual ~RDOCalcWatchGroupStop();

	LPIResultGroup m_pResultGroup;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

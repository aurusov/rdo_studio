#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_priority_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOPatternPrior: public IPriority
{
protected:
	RDOPatternPrior();
	virtual ~RDOPatternPrior();

private:
	virtual LPRDOCalc getPrior();
	virtual bool setPrior(const LPRDOCalc& pPrior);

	LPRDOCalc m_pPrior;
};

class RDODPTActivityCompare
{
public:
	RDODPTActivityCompare(const LPRDORuntime& pRuntime);
	bool operator()(const LPIBaseOperation& pOpr1, const LPIBaseOperation& pOpr2);

private:
	LPRDORuntime m_pRuntime;
};

CLOSE_RDO_RUNTIME_NAMESPACE

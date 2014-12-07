#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOCalcCheckRange);
class RDOCalcCheckRange: public RDOCalc
{
DECLARE_FACTORY(RDOCalcCheckRange)
private:
	RDOCalcCheckRange(const RDOValue& minValue, const RDOValue& maxValue, const LPRDOCalc& pCalc);

	RDOValue   m_minValue;
	RDOValue   m_maxValue;
	LPRDOCalc  m_pCalc;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#ifndef _LIB_RUNTIME_CALC_PROCEDURAL_RANGE_H_
#define _LIB_RUNTIME_CALC_PROCEDURAL_RANGE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Проверка на диапазон
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

#endif // _LIB_RUNTIME_CALC_PROCEDURAL_RANGE_H_

#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOCalcArraySize);
class RDOCalcArraySize: public RDOCalc
{
DECLARE_FACTORY(RDOCalcArraySize)
private:
	RDOCalcArraySize(const LPRDOCalc& pCalc);

	LPRDOCalc m_pCalc;

	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcArrayItem);
class RDOCalcArrayItem: public RDOCalc
{
DECLARE_FACTORY(RDOCalcArrayItem)
private:
	RDOCalcArrayItem(const LPRDOCalc& pArray, const LPRDOCalc& pArrayInd);

	LPRDOCalc m_pArray;
	LPRDOCalc m_pArrayInd;

	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcSetArrayItem);
class RDOCalcSetArrayItem: public RDOCalc
{
	DECLARE_FACTORY(RDOCalcSetArrayItem)
private:
	RDOCalcSetArrayItem(const LPRDOCalc& pArray, const LPRDOCalc& pArrayInd, const LPRDOCalc& pSetItem);

	LPRDOCalc m_pArray;
	LPRDOCalc m_pArrayInd;
	LPRDOCalc m_pSetItem;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

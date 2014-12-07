#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/function/calc_function.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOFunCalcGroup);
class RDOFunCalcGroup: public RDOFunCalc
{
protected:
	int        m_nResType;
	LPRDOCalc  m_pCondition;

	RDOFunCalcGroup(int nResType, const LPRDOCalc& pCondition);
};

#define DEFINE_CALC_GROUP(CalcName)                                 \
PREDECLARE_POINTER(RDOFunCalc##CalcName);                           \
class RDOFunCalc##CalcName: public RDOFunCalcGroup                  \
{                                                                   \
DECLARE_FACTORY(RDOFunCalc##CalcName)                               \
private:                                                            \
	RDOFunCalc##CalcName(int nResType, const LPRDOCalc& pCondition) \
		: RDOFunCalcGroup(nResType, pCondition)                     \
	{}                                                              \
	DECLARE_ICalc;                                                  \
};

DEFINE_CALC_GROUP(Exist);

DEFINE_CALC_GROUP(NotExist);

DEFINE_CALC_GROUP(ForAll);

DEFINE_CALC_GROUP(NotForAll);

CLOSE_RDO_RUNTIME_NAMESPACE

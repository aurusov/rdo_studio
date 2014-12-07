#ifndef _LIB_RUNTIME_CALC_PROCEDURAL_CONST_H_
#define _LIB_RUNTIME_CALC_PROCEDURAL_CONST_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOCalcConst);
class RDOCalcConst: public RDOCalc
{
DECLARE_FACTORY(RDOCalcConst)
public:
	const RDOValue& getValue() const;

private:
	RDOCalcConst(const RDOValue& value);

	RDOValue m_value;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_PROCEDURAL_CONST_H_

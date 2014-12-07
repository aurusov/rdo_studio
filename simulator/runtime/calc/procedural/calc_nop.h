#ifndef _LIB_RUNTIME_CALC_PROCEDURAL_NOP_H_
#define _LIB_RUNTIME_CALC_PROCEDURAL_NOP_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOCalcNop);
class RDOCalcNop: public RDOCalc
{
DECLARE_FACTORY(RDOCalcNop)
private:
	RDOCalcNop();
	virtual ~RDOCalcNop();

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_PROCEDURAL_NOP_H_

/*
 * copyright: (c) RDO-Team, 2011
 * filename : arithm.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALC_ARITHM_H_
#define _RDOCALC_ARITHM_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/binary.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- Арифметические функции
// ----------------------------------------------------------------------------
typedef RDOCalcBinary<RDOValue, (&RDOValue::operator+)> RDOCalcPlus;  DECLARE_POINTER(RDOCalcPlus);
typedef RDOCalcBinary<RDOValue, (&RDOValue::operator-)> RDOCalcMinus; DECLARE_POINTER(RDOCalcMinus);
typedef RDOCalcBinary<RDOValue, (&RDOValue::operator*)> RDOCalcMult;  DECLARE_POINTER(RDOCalcMult);

class RDOCalcDiv: public RDOCalcBinary<RDOValue, (&RDOValue::operator/)>
{
DECLARE_FACTORY(RDOCalcDiv);
private:
	RDOCalcDiv(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
		: RDOCalcBinary(pLeft, pRight)
	{}
	DECALRE_ICalc;
};
DECLARE_POINTER(RDOCalcDiv);

CALC_SUB(RDOCalcPlusEnumSafe, RDOCalcPlus)
{
DECLARE_FACTORY(RDOCalcPlusEnumSafe)
private:
	RDOCalcPlusEnumSafe(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
		: RDOCalcPlus(pLeft, pRight)
	{}
	DECALRE_ICalc;
};

CALC_SUB(RDOCalcMultEnumSafe, RDOCalcMult)
{
DECLARE_FACTORY(RDOCalcMultEnumSafe)
private:
	RDOCalcMultEnumSafe(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
		: RDOCalcMult(pLeft, pRight)
	{}
	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOCALC_ARITHM_H_

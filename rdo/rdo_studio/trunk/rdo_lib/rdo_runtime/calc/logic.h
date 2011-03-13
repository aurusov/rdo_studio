/*
 * copyright: (c) RDO-Team, 2011
 * filename : logic.h
 * author   : Александ Барс, Урусов Андрей, Дмитрий Лущан
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALC_LOGIC_H_
#define _RDOCALC_LOGIC_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/binary.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- Логические функции
// ----------------------------------------------------------------------------
class RDOCalcAnd: public RDOCalcBinary<RDOValue, (&RDOValue::operator&&)>
{
DECLARE_FACTORY(RDOCalcAnd)
private:
	RDOCalcAnd(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
		: RDOCalcBinary(pLeft, pRight)
	{
		m_value_true  = 1;
		m_value_false = 0;
		setSrcInfo(getStaticSrcInfo(m_pLeft, m_pRight));
	}
	RDOValue m_value_true;
	RDOValue m_value_false;

	DECALRE_ICalc;
};

class RDOCalcOr: public RDOCalcBinary<RDOValue, (&RDOValue::operator||)>
{
DECLARE_FACTORY(RDOCalcOr)
private:
	RDOCalcOr(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
		: RDOCalcBinary(pLeft, pRight)
	{
		m_value_true  = 1;
		m_value_false = 0;
		setSrcInfo(getStaticSrcInfo(m_pLeft, m_pRight));
	}

	RDOValue m_value_true;
	RDOValue m_value_false;

	DECALRE_ICalc;
};

CALC(RDOCalcNot)
{
DECLARE_FACTORY(RDOCalcNot)
private:
	RDOCalcNot(CREF(LPRDOCalc) pCalc)
		: m_pCalc(pCalc)
	{}

	LPRDOCalc m_pCalc;

	DECALRE_ICalc;
};

typedef RDOCalcBinary<rbool, (&RDOValue::operator==)> RDOCalcIsEqual;     DECLARE_POINTER(RDOCalcIsEqual);
typedef RDOCalcBinary<rbool, (&RDOValue::operator!=)> RDOCalcIsNotEqual;  DECLARE_POINTER(RDOCalcIsNotEqual);
typedef RDOCalcBinary<rbool, (&RDOValue::operator< )> RDOCalcIsLess;      DECLARE_POINTER(RDOCalcIsLess);
typedef RDOCalcBinary<rbool, (&RDOValue::operator> )> RDOCalcIsGreater;   DECLARE_POINTER(RDOCalcIsGreater);
typedef RDOCalcBinary<rbool, (&RDOValue::operator<=)> RDOCalcIsLEQ;       DECLARE_POINTER(RDOCalcIsLEQ);
typedef RDOCalcBinary<rbool, (&RDOValue::operator>=)> RDOCalcIsGEQ;       DECLARE_POINTER(RDOCalcIsGEQ);

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOCALC_LOGIC_H_

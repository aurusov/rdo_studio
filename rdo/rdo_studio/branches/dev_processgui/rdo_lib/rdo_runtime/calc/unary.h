/*
 * copyright: (c) RDO-Team, 2011
 * filename : unary.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALC_UNARY_H_
#define _RDOCALC_UNARY_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcUnary
// ----------------------------------------------------------------------------
CALC(RDOCalcUnary)
{
protected:
	RDOCalcUnary(CREF(LPRDOCalc) pOperation)
		: m_pOperation(pOperation)
	{
		if (m_pOperation)
			setSrcInfo(m_pOperation->src_info());
	}

	LPRDOCalc m_pOperation;
};

// ----------------------------------------------------------------------------
// ---------- Унарные операции
// ----------------------------------------------------------------------------
CALC_SUB(RDOCalcUMinus, RDOCalcUnary)
{
DECLARE_FACTORY(RDOCalcUMinus)
private:
	RDOCalcUMinus(CREF(LPRDOCalc) pOper)
		: RDOCalcUnary(pOper)
	{}
	DECALRE_ICalc;
};

CALC_SUB(RDOCalcDoubleToInt, RDOCalcUnary)
{
DECLARE_FACTORY(RDOCalcDoubleToInt)
private:
	RDOCalcDoubleToInt(CREF(LPRDOCalc) pOper)
		: RDOCalcUnary(pOper)
	{}
	DECALRE_ICalc;
};

CALC_SUB(RDOCalcDoubleToIntByResult, RDOCalcUnary)
{
DECLARE_FACTORY(RDOCalcDoubleToIntByResult)
public:
	void needRound()
	{
		m_round = true;
	}

private:
	RDOCalcDoubleToIntByResult(CREF(LPRDOCalc) pOper)
		: RDOCalcUnary(pOper)
		, m_round     (false)
	{}

	rbool m_round;

	DECALRE_ICalc;
};

CALC_SUB(RDOCalcCheckDiap, RDOCalcUnary)
{
DECLARE_FACTORY(RDOCalcCheckDiap)
private:
	RDOCalcCheckDiap(CREF(RDOValue) min_value, CREF(RDOValue) max_value, CREF(LPRDOCalc) pOper)
		: RDOCalcUnary(pOper    )
		, m_min_value (min_value)
		, m_max_value (max_value)
	{}

	RDOValue m_min_value;
	RDOValue m_max_value;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcInt (приведение к целому)
// ----------------------------------------------------------------------------
CALC_SUB(RDOCalcInt, RDOCalcUnary)
{
DECLARE_FACTORY(RDOCalcInt)
private:
	RDOCalcInt(CREF(LPRDOCalc) pOperation)
		: RDOCalcUnary(pOperation)
	{}

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOCALC_UNARY_H_

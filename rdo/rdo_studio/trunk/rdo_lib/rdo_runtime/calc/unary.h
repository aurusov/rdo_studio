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
#include "rdo_lib/rdo_runtime/calc/operation_type.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcUnaryBase
// ----------------------------------------------------------------------------
class RDOCalcUnaryBase: public RDOCalc
{
public:
	template <class T>
	static LPRDOCalc generateCalc(CREF(LPRDOCalc) pUnaryCalc);

protected:
	RDOCalcUnaryBase(CREF(LPRDOCalc) pOperation);

	LPRDOCalc m_pOperation;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcUnary
// ----------------------------------------------------------------------------
template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
class RDOCalcUnary: public RDOCalcUnaryBase
{
friend class rdo::Factory<RDOCalcUnary<ret_type, pOperator, CalcType> >;
public:
	enum { calc_type = CalcType };
	typedef ret_type (RDOValue::*value_operator)() const;

	static RDOSrcInfo     getStaticSrcInfo(CREF(LPRDOCalc) pUnaryCalc);
	static value_operator getOperation    ();

protected:
	RDOCalcUnary(CREF(LPRDOCalc) pOperation);

private:
	REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- Унарные операции
// ----------------------------------------------------------------------------
typedef RDOCalcUnary<RDOValue, (&RDOValue::operator-), OperatorType::OT_ARITHM> RDOCalcUMinus;
typedef RDOCalcUnary<rsint,    (&RDOValue::getInt   ), OperatorType::OT_ARITHM> RDOCalcDoubleToInt;

CALC_SUB(RDOCalcDoubleToIntByResult, RDOCalc)
{
DECLARE_FACTORY(RDOCalcDoubleToIntByResult)
public:
	void needRound()
	{
		m_round = true;
	}

private:
	RDOCalcDoubleToIntByResult(CREF(LPRDOCalc) pOper)
		: m_pOperation(pOper)
		, m_round     (false)
	{}

	rbool     m_round;
	LPRDOCalc m_pOperation;

	DECALRE_ICalc;
};

CALC_SUB(RDOCalcCheckDiap, RDOCalc)
{
DECLARE_FACTORY(RDOCalcCheckDiap)
private:
	RDOCalcCheckDiap(CREF(RDOValue) min_value, CREF(RDOValue) max_value, CREF(LPRDOCalc) pOper)
		: m_pOperation(pOper    )
		, m_min_value (min_value)
		, m_max_value (max_value)
	{}

	RDOValue  m_min_value;
	RDOValue  m_max_value;
	LPRDOCalc m_pOperation;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcInt (приведение к целому)
// ----------------------------------------------------------------------------
CALC_SUB(RDOCalcInt, RDOCalc)
{
DECLARE_FACTORY(RDOCalcInt)
private:
	RDOCalcInt(CREF(LPRDOCalc) pOperation)
		: m_pOperation(pOperation)
	{}

	LPRDOCalc m_pOperation;

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/calc/unary.inl"

#endif //! _RDOCALC_UNARY_H_

#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/operation/operation_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOCalcUnaryBase: public RDOCalc
{
public:
	template <class T>
	static LPRDOCalc generateCalc(const RDOSrcInfo::Position& position, const LPRDOCalc& pUnaryCalc);

protected:
	RDOCalcUnaryBase(const LPRDOCalc& pOperation);

	LPRDOCalc m_pOperation;
};

template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
class RDOCalcUnary: public RDOCalcUnaryBase
{
friend class rdo::Factory<RDOCalcUnary<ret_type, pOperator, CalcType> >;
public:
	enum { calc_type = CalcType };
	typedef ret_type (RDOValue::*value_operator)() const;

	static RDOSrcInfo     getStaticSrcInfo(const RDOSrcInfo::Position& position, const LPRDOCalc& pUnaryCalc);
	static value_operator getOperation    ();

protected:
	RDOCalcUnary(const RDOSrcInfo::Position& position, const LPRDOCalc& pOperation);

private:
	DECLARE_ICalc;
};

typedef RDOCalcUnary<RDOValue, &RDOValue::operator-, OperatorType::OT_ARITHM> RDOCalcUMinus;

typedef RDOCalcUnary<int, &RDOValue::getInt, OperatorType::OT_ARITHM> RDOCalcDoubleToInt;

PREDECLARE_POINTER(RDOCalcDoubleToIntByResult);
class RDOCalcDoubleToIntByResult: public RDOCalc
{
DECLARE_FACTORY(RDOCalcDoubleToIntByResult)
public:
	void needRound();

private:
	RDOCalcDoubleToIntByResult(const LPRDOCalc& pOper);

	bool m_round;
	LPRDOCalc m_pOperation;

	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcInt);
class RDOCalcInt: public RDOCalc
{
DECLARE_FACTORY(RDOCalcInt)
private:
	RDOCalcInt(const LPRDOCalc& pOperation);

	LPRDOCalc m_pOperation;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/operation/calc_unary-inl.h"

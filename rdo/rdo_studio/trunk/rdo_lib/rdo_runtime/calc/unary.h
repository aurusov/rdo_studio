/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      unary.h
 * @authors   Барс Александр, Урусов Андрей
 * @date      
 * @brief     
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_CALC_UNARY_H_
#define _LIB_RUNTIME_CALC_UNARY_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/calc/operation_type.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOCalcUnaryBase
// ********************************************************************************
class RDOCalcUnaryBase: public RDOCalc
{
public:
	template <class T>
	static LPRDOCalc generateCalc(CREF(RDOSrcInfo::Position) position, CREF(LPRDOCalc) pUnaryCalc);

protected:
	RDOCalcUnaryBase(CREF(LPRDOCalc) pOperation);

	LPRDOCalc m_pOperation;
};

// ********************************************************************************
// ******************** RDOCalcUnary
// ********************************************************************************
template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
class RDOCalcUnary: public RDOCalcUnaryBase
{
friend class rdo::Factory<RDOCalcUnary<ret_type, pOperator, CalcType> >;
public:
	enum { calc_type = CalcType };
	typedef ret_type (RDOValue::*value_operator)() const;

	static RDOSrcInfo     getStaticSrcInfo(CREF(RDOSrcInfo::Position) position, CREF(LPRDOCalc) pUnaryCalc);
	static value_operator getOperation    ();

protected:
	RDOCalcUnary(CREF(RDOSrcInfo::Position) position, CREF(LPRDOCalc) pOperation);

private:
	REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime);
};

// ********************************************************************************
// ******************** Унарные операции
// ********************************************************************************
typedef RDOCalcUnary<RDOValue, (&RDOValue::operator-), OperatorType::OT_ARITHM> RDOCalcUMinus;
typedef RDOCalcUnary<rsint,    (&RDOValue::getInt   ), OperatorType::OT_ARITHM> RDOCalcDoubleToInt;

CALC_SUB(RDOCalcDoubleToIntByResult, RDOCalc)
{
DECLARE_FACTORY(RDOCalcDoubleToIntByResult)
public:
	void needRound();

private:
	RDOCalcDoubleToIntByResult(CREF(LPRDOCalc) pOper);

	rbool     m_round;
	LPRDOCalc m_pOperation;

	DECALRE_ICalc;
};

CALC_SUB(RDOCalcCheckDiap, RDOCalc)
{
DECLARE_FACTORY(RDOCalcCheckDiap)
private:
	RDOCalcCheckDiap(CREF(RDOValue) min_value, CREF(RDOValue) max_value, CREF(LPRDOCalc) pOper);

	RDOValue  m_min_value;
	RDOValue  m_max_value;
	LPRDOCalc m_pOperation;

	DECALRE_ICalc;
};

// ********************************************************************************
// ******************** RDOCalcInt (приведение к целому)
// ********************************************************************************
CALC_SUB(RDOCalcInt, RDOCalc)
{
DECLARE_FACTORY(RDOCalcInt)
private:
	RDOCalcInt(CREF(LPRDOCalc) pOperation);

	LPRDOCalc m_pOperation;

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/calc/unary.inl"

#endif // _LIB_RUNTIME_CALC_UNARY_H_

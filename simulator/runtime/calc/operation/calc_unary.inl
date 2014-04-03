/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_unary.inl
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      21.03.2011
  \brief     Унарные операторы
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/operation/operation_name.h"
#include "simulator/runtime/calc/procedural/calc_const.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcUnaryBase
// --------------------------------------------------------------------------------
inline RDOCalcUnaryBase::RDOCalcUnaryBase(CREF(LPRDOCalc) pOperation)
	: m_pOperation(pOperation)
{
	ASSERT(m_pOperation);
}

template <class T>
LPRDOCalc RDOCalcUnaryBase::generateCalc(CREF(RDOSrcInfo::Position) position, CREF(LPRDOCalc) pUnaryCalc)
{
	ASSERT(pUnaryCalc);

	LPRDOCalcConst pConstCalc = pUnaryCalc.object_dynamic_cast<RDOCalcConst>();
	LPRDOCalc pCalc;
	if (pConstCalc)
	{
		typename T::value_operator pOperation = T::getOperation();
		pCalc = rdo::Factory<RDOCalcConst>::create((pConstCalc->calcValue(NULL).*pOperation)());
		pCalc->setSrcInfo(T::getStaticSrcInfo(position, pConstCalc));
	}
	else
	{
		pCalc = rdo::Factory<T>::create(position, pUnaryCalc);
	}
	ASSERT(pCalc);
	return pCalc;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcUnary
// --------------------------------------------------------------------------------
template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
inline RDOCalcUnary<ret_type, pOperator, CalcType>::RDOCalcUnary(CREF(RDOSrcInfo::Position) position, CREF(LPRDOCalc) pOperation)
	: RDOCalcUnaryBase(pOperation)
{
	setSrcInfo(getStaticSrcInfo(position, m_pOperation));
}

template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
inline RDOSrcInfo RDOCalcUnary<ret_type, pOperator, CalcType>::getStaticSrcInfo(CREF(RDOSrcInfo::Position) position, CREF(LPRDOCalc) pUnaryCalc)
{
	RDOSrcInfo srcInfo(pUnaryCalc->srcInfo());
	srcInfo.setSrcPos (position);
	srcInfo.setSrcText(rdo::format("%s(%s)", OperatorName<ret_type (RDOValue::*)() const>::name(pOperator).c_str(), pUnaryCalc->srcInfo().src_text().c_str()));
	return srcInfo;
}

template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
inline typename RDOCalcUnary<ret_type, pOperator, CalcType>::value_operator RDOCalcUnary<ret_type, pOperator, CalcType>::getOperation()
{
	return pOperator;
}

template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
inline RDOValue RDOCalcUnary<ret_type, pOperator, CalcType>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	++OperatorType::getCalcCounter<CalcType>();
	return (m_pOperation->calcValue(pRuntime).*pOperator)();
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcDoubleToIntByResult
// --------------------------------------------------------------------------------
inline void RDOCalcDoubleToIntByResult::needRound()
{
	m_round = true;
}

inline RDOCalcDoubleToIntByResult::RDOCalcDoubleToIntByResult(CREF(LPRDOCalc) pOper)
	: m_round     (false)
	, m_pOperation(pOper)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcInt
// --------------------------------------------------------------------------------
inline RDOCalcInt::RDOCalcInt(CREF(LPRDOCalc) pOperation)
	: m_pOperation(pOperation)
{}

CLOSE_RDO_RUNTIME_NAMESPACE

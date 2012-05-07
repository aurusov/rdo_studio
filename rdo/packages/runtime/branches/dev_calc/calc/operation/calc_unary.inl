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
#include "simulator/runtime/rdo_runtime.h"
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
inline LPRDOCalc RDOCalcUnaryBase::generateCalc(CREF(RDOSrcInfo::Position) position, CREF(LPRDOCalc) pUnaryCalc)
{
	ASSERT(pUnaryCalc);

	LPRDOCalcConst pConstCalc = pUnaryCalc.object_dynamic_cast<RDOCalcConst>();
	LPRDOCalc pCalc;
	if (pConstCalc)
	{
		typename T::value_operator pOperation = T::getOperation();
		pCalc = rdo::Factory<RDOCalcConst>::create((pConstCalc->getValue().*pOperation)());
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
	srcInfo.setSrcText(rdo::format(_T("%s(%s)"), OperatorName<ret_type (RDOValue::*)() const>::name(pOperator).c_str(), pUnaryCalc->srcInfo().src_text().c_str()));
	return srcInfo;
}

template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
inline typename RDOCalcUnary<ret_type, pOperator, CalcType>::value_operator RDOCalcUnary<ret_type, pOperator, CalcType>::getOperation()
{
	return pOperator;
}

template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
inline void RDOCalcUnary<ret_type, pOperator, CalcType>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	++OperatorType::getCalcCounter<CalcType>();
	m_pOperation->calcValue(pRuntime);
	pRuntime->stack().push((pRuntime->stack().pop().*pOperator)());
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcDoubleToIntByResult
// --------------------------------------------------------------------------------
inline void RDOCalcDoubleToIntByResult::needRound()
{
	m_round = true;
}

inline RDOCalcDoubleToIntByResult::RDOCalcDoubleToIntByResult(CREF(LPRDOCalc) pOper)
	: m_pOperation(pOper)
	, m_round     (false)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcInt
// --------------------------------------------------------------------------------
inline RDOCalcInt::RDOCalcInt(CREF(LPRDOCalc) pOperation)
	: m_pOperation(pOperation)
{}

CLOSE_RDO_RUNTIME_NAMESPACE

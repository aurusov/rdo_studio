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
template <typename OperatorType::Type CalcType, typename ret_type, ret_type (RDOValue::*pOperator)() const>
inline RDOCalcUnary<CalcType, ret_type, pOperator>::RDOCalcUnary(CREF(RDOSrcInfo::Position) position, CREF(LPRDOCalc) pOperation)
	: RDOCalcUnaryBase(pOperation)
{
	setSrcInfo(getStaticSrcInfo(position, m_pOperation));
}

template <typename OperatorType::Type CalcType, typename ret_type, ret_type (RDOValue::*pOperator)() const>
inline RDOSrcInfo RDOCalcUnary<CalcType, ret_type, pOperator>::getStaticSrcInfo(CREF(RDOSrcInfo::Position) position, CREF(LPRDOCalc) pUnaryCalc)
{
	RDOSrcInfo src_info(pUnaryCalc->src_info());
	src_info.setSrcPos (position);
	src_info.setSrcText(rdo::format(_T("%s(%s)"), OperatorName<ret_type (RDOValue::*)() const>::name(pOperator).c_str(), pUnaryCalc->src_text().c_str()));
	return src_info;
}

template <typename OperatorType::Type CalcType, typename ret_type, ret_type (RDOValue::*pOperator)() const>
inline typename RDOCalcUnary<CalcType, ret_type, pOperator>::value_operator RDOCalcUnary<CalcType, ret_type, pOperator>::getOperation()
{
	return pOperator;
}

template <typename OperatorType::Type CalcType, typename ret_type, ret_type (RDOValue::*pOperator)() const>
inline REF(RDOValue) RDOCalcUnary<CalcType, ret_type, pOperator>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	++OperatorType::getCalcCounter<CalcType>();
	m_value = (m_pOperation->calcValue(pRuntime).*pOperator)();
	return m_value;
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
// -------------------- RDOCalcCheckDiap
// --------------------------------------------------------------------------------
inline RDOCalcCheckDiap::RDOCalcCheckDiap(CREF(RDOValue) min_value, CREF(RDOValue) max_value, CREF(LPRDOCalc) pOper)
	: m_pOperation(pOper    )
	, m_min_value (min_value)
	, m_max_value (max_value)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcInt
// --------------------------------------------------------------------------------
inline RDOCalcInt::RDOCalcInt(CREF(LPRDOCalc) pOperation)
	: m_pOperation(pOperation)
{}

CLOSE_RDO_RUNTIME_NAMESPACE

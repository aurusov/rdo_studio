/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_binary.inl
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     Бинарные операторы
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/operation/operation_name.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcBinaryBase
// --------------------------------------------------------------------------------
inline RDOCalcBinaryBase::RDOCalcBinaryBase(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
	: m_pLeft (pLeft )
	, m_pRight(pRight)
{
	ASSERT(m_pLeft );
	ASSERT(m_pRight);
}

template <class T>
inline LPRDOCalc RDOCalcBinaryBase::generateCalc(CREF(LPRDOCalc) pFirst, CREF(LPRDOCalc) pSecond)
{
	ASSERT(pFirst );
	ASSERT(pSecond);

	LPRDOCalcConst pConstCalc1 = pFirst.object_dynamic_cast<RDOCalcConst>();
	LPRDOCalcConst pConstCalc2 = pSecond.object_dynamic_cast<RDOCalcConst>();
	LPRDOCalc pCalc;
	if (pConstCalc1 && pConstCalc2)
	{
		typename T::value_operator pOperation = T::getOperation();
		pCalc = rdo::Factory<RDOCalcConst>::create((pConstCalc1->getValue().*pOperation)(pConstCalc2->getValue()));
		pCalc->setSrcInfo(T::getStaticSrcInfo(pConstCalc1, pConstCalc2));
	}
	else
	{
		pCalc = rdo::Factory<T>::create(pFirst, pSecond);
	}
	ASSERT(pCalc);
	return pCalc;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcBinary
// --------------------------------------------------------------------------------
template <typename OperatorType::Type CalcType, typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue)) const>
inline RDOCalcBinary<CalcType, ret_type, pOperator>::RDOCalcBinary(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
	: RDOCalcBinaryBase(pLeft, pRight)
{
	setSrcInfo(getStaticSrcInfo(m_pLeft, m_pRight));
}

template <typename OperatorType::Type CalcType, typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue)) const>
inline LPRDOCalc RDOCalcBinary<CalcType, ret_type, pOperator>::getLeft() const
{
	return m_pLeft;
}

template <typename OperatorType::Type CalcType, typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue)) const>
inline LPRDOCalcConst RDOCalcBinary<CalcType, ret_type, pOperator>::getRightAsConst() const
{
	return m_pRight.object_static_cast<RDOCalcConst>();
}

template <typename OperatorType::Type CalcType, typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue)) const>
inline void RDOCalcBinary<CalcType, ret_type, pOperator>::setRight(CREF(LPRDOCalc) pRight)
{
	m_pRight = pRight;
}

template <typename OperatorType::Type CalcType, typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue)) const>
inline RDOSrcInfo RDOCalcBinary<CalcType, ret_type, pOperator>::getStaticSrcInfo(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
{
	RDOSrcInfo src_info;
	src_info.setSrcInfo(pLeft->src_info(), rdo::format(_T(" %s "), OperatorName<ret_type (RDOValue::*)(CREF(RDOValue)) const>::name(pOperator).c_str()), pRight->src_info());
	return src_info;
}

template <typename OperatorType::Type CalcType, typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue)) const>
inline typename RDOCalcBinary<CalcType, ret_type, pOperator>::value_operator RDOCalcBinary<CalcType, ret_type, pOperator>::getOperation()
{
	return pOperator;
}

template <typename OperatorType::Type CalcType, typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue)) const>
inline REF(RDOValue) RDOCalcBinary<CalcType, ret_type, pOperator>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	++OperatorType::getCalcCounter<CalcType>();
	m_value = (m_pLeft->calcValue(pRuntime).*pOperator)(m_pRight->calcValue(pRuntime));
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

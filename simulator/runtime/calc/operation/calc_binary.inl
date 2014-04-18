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
inline RDOCalcBinaryBase::RDOCalcBinaryBase(const LPRDOCalc& pLeft, const LPRDOCalc& pRight)
	: m_pLeft (pLeft )
	, m_pRight(pRight)
{
	ASSERT(m_pLeft );
	ASSERT(m_pRight);
}

template <class T>
inline LPRDOCalc RDOCalcBinaryBase::generateCalc(const LPRDOCalc& pFirst, const LPRDOCalc& pSecond)
{
	ASSERT(pFirst );
	ASSERT(pSecond);

	LPRDOCalcConst pConstCalc1 = pFirst.object_dynamic_cast<RDOCalcConst>();
	LPRDOCalcConst pConstCalc2 = pSecond.object_dynamic_cast<RDOCalcConst>();
	LPRDOCalc pCalc;
	if (pConstCalc1 && pConstCalc2)
	{
		pCalc = rdo::Factory<RDOCalcConst>::create((pConstCalc1->getValue().*T::caller_type::method())(pConstCalc2->getValue()));
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
// -------------------- BinaryOperatorConstP1
// --------------------------------------------------------------------------------
template <typename ret_type, ret_type (RDOValue::*pMethod)(const RDOValue&) const>
inline typename BinaryOperatorConstP1<ret_type, pMethod>::method_type BinaryOperatorConstP1<ret_type, pMethod>::method()
{
	return pMethod;
}

// --------------------------------------------------------------------------------
// -------------------- BinaryOperatorNonConstP1
// --------------------------------------------------------------------------------
template <typename ret_type, ret_type (RDOValue::*pMethod)(const RDOValue&)>
inline typename BinaryOperatorNonConstP1<ret_type, pMethod>::method_type BinaryOperatorNonConstP1<ret_type, pMethod>::method()
{
	return pMethod;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcBinary
// --------------------------------------------------------------------------------
template <class F, typename OperatorType::Type CalcType>
inline RDOCalcBinary<F, CalcType>::RDOCalcBinary(const LPRDOCalc& pLeft, const LPRDOCalc& pRight)
	: RDOCalcBinaryBase(pLeft, pRight)
{
	setSrcInfo(getStaticSrcInfo(m_pLeft, m_pRight));
}

template <class F, typename OperatorType::Type CalcType>
inline LPRDOCalc RDOCalcBinary<F, CalcType>::getLeft() const
{
	return m_pLeft;
}

template <class F, typename OperatorType::Type CalcType>
inline LPRDOCalcConst RDOCalcBinary<F, CalcType>::getRightAsConst() const
{
	return m_pRight.object_static_cast<RDOCalcConst>();
}

template <class F, typename OperatorType::Type CalcType>
inline void RDOCalcBinary<F, CalcType>::setRight(const LPRDOCalc& pRight)
{
	m_pRight = pRight;
}

template <class F, typename OperatorType::Type CalcType>
inline RDOSrcInfo RDOCalcBinary<F, CalcType>::getStaticSrcInfo(const LPRDOCalc& pLeft, const LPRDOCalc& pRight)
{
	RDOSrcInfo srcInfo;
	srcInfo.setSrcInfo(pLeft->srcInfo(), rdo::format(" %s ", OperatorName<typename F::method_type>::name(F::method()).c_str()), pRight->srcInfo());
	return srcInfo;
}

template <class F, typename OperatorType::Type CalcType>
inline RDOValue RDOCalcBinary<F, CalcType>::doCalc(const LPRDORuntime& pRuntime)
{
	++OperatorType::getCalcCounter<CalcType>();
	return (m_pLeft->calcValue(pRuntime).*F::method())(m_pRight->calcValue(pRuntime));
}

CLOSE_RDO_RUNTIME_NAMESPACE

/*
 * copyright: (c) RDO-Team, 2011
 * filename : binary.inl
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/operation_name.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcBinaryBase
// ----------------------------------------------------------------------------
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
		T::value_operator pOperation = T::getOperation();
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

// ----------------------------------------------------------------------------
// ---------- RDOCalcBinary
// ----------------------------------------------------------------------------
template <typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue) rdovalue) const, typename OperatorType::Type CalcType>
inline RDOCalcBinary<ret_type, pOperator, CalcType>::RDOCalcBinary(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
	: RDOCalcBinaryBase(pLeft, pRight)
{
	setSrcInfo(getStaticSrcInfo(m_pLeft, m_pRight));
}

template <typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue) rdovalue) const, typename OperatorType::Type CalcType>
inline LPRDOCalc RDOCalcBinary<ret_type, pOperator, CalcType>::getLeft() const
{
	return m_pLeft;
}

template <typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue) rdovalue) const, typename OperatorType::Type CalcType>
inline LPRDOCalcConst RDOCalcBinary<ret_type, pOperator, CalcType>::getRightAsConst() const
{
	return m_pRight.object_static_cast<RDOCalcConst>();
}

template <typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue) rdovalue) const, typename OperatorType::Type CalcType>
inline void RDOCalcBinary<ret_type, pOperator, CalcType>::setRight(CREF(LPRDOCalc) pRight)
{
	m_pRight = pRight;
}

template <typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue) rdovalue) const, typename OperatorType::Type CalcType>
inline RDOSrcInfo RDOCalcBinary<ret_type, pOperator, CalcType>::getStaticSrcInfo(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
{
	RDOSrcInfo src_info;
	src_info.setSrcInfo(pLeft->src_info(), rdo::format(_T(" %s "), OperatorName<ret_type (RDOValue::*)(CREF(RDOValue)) const>::name(pOperator).c_str()), pRight->src_info());
	return src_info;
}

template <typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue) rdovalue) const, typename OperatorType::Type CalcType>
inline typename RDOCalcBinary<ret_type, pOperator, CalcType>::value_operator RDOCalcBinary<ret_type, pOperator, CalcType>::getOperation()
{
	return pOperator;
}

template <typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue) rdovalue) const, typename OperatorType::Type CalcType>
inline REF(RDOValue) RDOCalcBinary<ret_type, pOperator, CalcType>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	++OperatorType::getCalcCounter<CalcType>();
	m_value = (m_pLeft->calcValue(pRuntime).*pOperator)(m_pRight->calcValue(pRuntime));
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

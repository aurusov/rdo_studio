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
// ---------- RDOCalcBinary
// ----------------------------------------------------------------------------
template <typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue) rdovalue) const, typename OperatorType::Type CalcType>
inline RDOCalcBinary<ret_type, pOperator, CalcType>::RDOCalcBinary(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
	: m_pLeft (pLeft )
	, m_pRight(pRight)
{
	setSrcInfo(getStaticSrcInfo(m_pLeft, m_pRight));
}

template <typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue) rdovalue) const, typename OperatorType::Type CalcType>
inline RDOSrcInfo RDOCalcBinary<ret_type, pOperator, CalcType>::getStaticSrcInfo(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
{
	RDOSrcInfo src_info;
	src_info.setSrcInfo(pLeft->src_info(), rdo::format(_T(" %s "), OperatorName<ret_type>::name(pOperator).c_str()), pRight->src_info());
	return src_info;
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
inline typename RDOCalcBinary<ret_type, pOperator, CalcType>::value_operator RDOCalcBinary<ret_type, pOperator, CalcType>::getOperation()
{
	return pOperator;
}

template <typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue) rdovalue) const, typename OperatorType::Type CalcType>
inline REF(RDOValue) RDOCalcBinary<ret_type, pOperator, CalcType>::doCalc(PTR(RDORuntime) runtime)
{
	++OperatorType::getCalcCounter<CalcType>();
	m_value = (m_pLeft->calcValue(runtime).*pOperator)(m_pRight->calcValue(runtime));
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

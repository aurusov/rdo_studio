/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      unary.inl
 * @authors   Барс Александр, Урусов Андрей
 * @date      
 * @brief     
 * @indent    4T
 */

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/operation_name.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOCalcUnaryBase
// ********************************************************************************
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
		T::value_operator pOperation = T::getOperation();
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

// ********************************************************************************
// ******************** RDOCalcUnary
// ********************************************************************************
template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
inline RDOCalcUnary<ret_type, pOperator, CalcType>::RDOCalcUnary(CREF(RDOSrcInfo::Position) position, CREF(LPRDOCalc) pOperation)
	: RDOCalcUnaryBase(pOperation)
{
	setSrcInfo(getStaticSrcInfo(position, m_pOperation));
}

template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
inline RDOSrcInfo RDOCalcUnary<ret_type, pOperator, CalcType>::getStaticSrcInfo(CREF(RDOSrcInfo::Position) position, CREF(LPRDOCalc) pUnaryCalc)
{
	RDOSrcInfo src_info(pUnaryCalc->src_info());
	src_info.setSrcPos (position);
	src_info.setSrcText(rdo::format(_T("%s(%s)"), OperatorName<ret_type (RDOValue::*)() const>::name(pOperator).c_str(), pUnaryCalc->src_text().c_str()));
	return src_info;
}

template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
inline typename RDOCalcUnary<ret_type, pOperator, CalcType>::value_operator RDOCalcUnary<ret_type, pOperator, CalcType>::getOperation()
{
	return pOperator;
}

template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
inline REF(RDOValue) RDOCalcUnary<ret_type, pOperator, CalcType>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	++OperatorType::getCalcCounter<CalcType>();
	m_value = (m_pOperation->calcValue(pRuntime).*pOperator)();
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

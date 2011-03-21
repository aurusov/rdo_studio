/*
 * copyright: (c) RDO-Team, 2011
 * filename : unary.inl
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcUnaryBase
// ----------------------------------------------------------------------------
inline RDOCalcUnaryBase::RDOCalcUnaryBase(CREF(LPRDOCalc) pOperation)
	: m_pOperation(pOperation)
{
	ASSERT(m_pOperation);
}

template <class T>
LPRDOCalc RDOCalcUnaryBase::generateCalc(CREF(LPRDOCalc) pUnaryCalc)
{
	ASSERT(pUnaryCalc);

	LPRDOCalcConst pConstCalc = pUnaryCalc.object_dynamic_cast<RDOCalcConst>();
	LPRDOCalc pCalc;
	if (pConstCalc)
	{
		T::value_operator pOperation = T::getOperation();
		pCalc = rdo::Factory<RDOCalcConst>::create((pConstCalc->calcValue(NULL).*pOperation)());
		pCalc->setSrcInfo(pConstCalc->src_info());
	}
	else
	{
		pCalc = rdo::Factory<T>::create(pUnaryCalc);
	}
	ASSERT(pCalc);
	return pCalc;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcUnary
// ----------------------------------------------------------------------------
template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
inline RDOCalcUnary<ret_type, pOperator, CalcType>::RDOCalcUnary(CREF(LPRDOCalc) pOperation)
	: RDOCalcUnaryBase(pOperation)
{
	setSrcInfo(m_pOperation->src_info());
}

template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
inline typename RDOCalcUnary<ret_type, pOperator, CalcType>::value_operator RDOCalcUnary<ret_type, pOperator, CalcType>::getOperation()
{
	return pOperator;
}

template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
inline REF(RDOValue) RDOCalcUnary<ret_type, pOperator, CalcType>::doCalc(PTR(RDORuntime) runtime)
{
	++OperatorType::getCalcCounter<CalcType>();
	m_value = (m_pOperation->calcValue(runtime).*pOperator)();
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

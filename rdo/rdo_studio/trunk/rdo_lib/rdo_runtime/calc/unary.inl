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

template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
inline RDOCalcUnary<ret_type, pOperator, CalcType>::RDOCalcUnary(CREF(LPRDOCalc) pOperation)
	: m_pOperation(pOperation)
{
	if (m_pOperation)
		setSrcInfo(m_pOperation->src_info());
}

template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
inline REF(RDOValue) RDOCalcUnary<ret_type, pOperator, CalcType>::doCalc(PTR(RDORuntime) runtime)
{
	++OperatorType::getCalcCounter<CalcType>();
	m_value = (m_pOperation->calcValue(runtime).*pOperator)();
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

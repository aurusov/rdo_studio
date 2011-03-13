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
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcBinary
// ----------------------------------------------------------------------------
template <typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue) rdovalue) const>
inline RDOCalcBinary<ret_type, pOperator>::RDOCalcBinary(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
	: m_pLeft (pLeft )
	, m_pRight(pRight)
{
	setSrcInfo(getStaticSrcInfo(m_pLeft, m_pRight));
}

template <typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue) rdovalue) const>
inline RDOSrcInfo RDOCalcBinary<ret_type, pOperator>::getStaticSrcInfo(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
{
	RDOSrcInfo src_info;
	src_info.setSrcInfo(pLeft->src_info(), rdo::format(_T(" %s "), OperatorName<ret_type>::name(pOperator).c_str()), pRight->src_info());
	return src_info;
}

template <typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue) rdovalue) const>
inline LPRDOCalc RDOCalcBinary<ret_type, pOperator>::getLeft() const
{
	return m_pLeft;
}

template <typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue) rdovalue) const>
LPRDOCalcConst RDOCalcBinary<ret_type, pOperator>::getRightAsConst() const
{
	return m_pRight.object_static_cast<RDOCalcConst>();
}

template <typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue) rdovalue) const>
void RDOCalcBinary<ret_type, pOperator>::setRight(CREF(LPRDOCalc) pRight)
{
	m_pRight = pRight;
}

template <typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue) rdovalue) const>
REF(RDOValue) RDOCalcBinary<ret_type, pOperator>::doCalc(PTR(RDORuntime) runtime)
{
//		runtime->inc_cnt_calc_logic();
	m_value = (m_pLeft->calcValue(runtime).*pOperator)(m_pRight->calcValue(runtime));
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

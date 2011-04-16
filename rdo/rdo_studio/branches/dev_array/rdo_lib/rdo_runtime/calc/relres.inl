/*
 * copyright: (c) RDO-Team, 2011
 * filename : relres.inl
 * author   : Александ Барс, Урусов Андрей
 * date     : 16.04.11
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOSetRelParamCalc
// ----------------------------------------------------------------------------
template <EqualType equalType>
inline RDOSetRelParamCalc<equalType>::RDOSetRelParamCalc(int relNumb, int parNumb, CREF(LPRDOCalc) pCalc)
	: m_relNumb(relNumb)
	, m_parNumb(parNumb)
	, m_pCalc  (pCalc  )
{
	m_value = 1;
	if (m_pCalc)
	{
		setSrcInfo(m_pCalc->src_info());
	}
}

template <EqualType equalType>
inline RDOSetRelParamCalc<equalType>::~RDOSetRelParamCalc()
{}

template <>
inline REF(RDOValue) RDOSetRelParamCalc<ET_EQUAL>::doCalc(PTR(RDORuntime) runtime)
{
	ruint resID = runtime->getCurrentActivity()->getResByRelRes(m_relNumb);
	runtime->setResParamVal(resID, m_parNumb, m_pCalc->calcValue(runtime));
	return m_value;
}

template <>
inline REF(RDOValue) RDOSetRelParamCalc<ET_PLUS>::doCalc(PTR(RDORuntime) runtime)
{
	ruint resID = runtime->getCurrentActivity()->getResByRelRes(m_relNumb);
	runtime->getResParamValRaw(resID, m_parNumb) += m_pCalc->calcValue(runtime);
	return m_value;
}

template <>
inline REF(RDOValue) RDOSetRelParamCalc<ET_MINUS>::doCalc(PTR(RDORuntime) runtime)
{
	ruint resID = runtime->getCurrentActivity()->getResByRelRes(m_relNumb);
	runtime->getResParamValRaw(resID, m_parNumb) -= m_pCalc->calcValue(runtime);
	return m_value;
}

template <>
inline REF(RDOValue) RDOSetRelParamCalc<ET_MULTIPLY>::doCalc(PTR(RDORuntime) runtime)
{
	ruint resID = runtime->getCurrentActivity()->getResByRelRes(m_relNumb);
	runtime->getResParamValRaw(resID, m_parNumb) *= m_pCalc->calcValue(runtime);
	return m_value;
}

template <>
inline REF(RDOValue) RDOSetRelParamCalc<ET_DIVIDE>::doCalc(PTR(RDORuntime) runtime)
{
	ruint resID = runtime->getCurrentActivity()->getResByRelRes(m_relNumb);
	runtime->getResParamValRaw(resID, m_parNumb) /= m_pCalc->calcValue(runtime);
	return m_value;
}

template <>
inline REF(RDOValue) RDOSetRelParamCalc<ET_INCR>::doCalc(PTR(RDORuntime) runtime)
{
	ruint resID = runtime->getCurrentActivity()->getResByRelRes(m_relNumb);
	runtime->getResParamValRaw(resID, m_parNumb) += RDOValue(1);
	return m_value;
}

template <>
inline REF(RDOValue) RDOSetRelParamCalc<ET_DECR>::doCalc(PTR(RDORuntime) runtime)
{
	ruint resID = runtime->getCurrentActivity()->getResByRelRes(m_relNumb);
	runtime->getResParamValRaw(resID, m_parNumb) -= RDOValue(1);
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

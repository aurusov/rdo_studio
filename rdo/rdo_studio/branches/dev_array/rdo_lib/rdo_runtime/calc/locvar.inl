/*
 * copyright: (c) RDO-Team, 2011
 * filename : locvar.inl
 * author   : Чирков Михаил
 * date     : 16.04.11
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdo_activity.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcSetLocalVariable
// ----------------------------------------------------------------------------
template <EqualType equalType>
inline RDOCalcSetLocalVariable<equalType>::RDOCalcSetLocalVariable(CREF(tstring) name, LPRDOCalc pCalc)
	: m_name (name )
	, m_pCalc(pCalc)
{}

template <EqualType equalType>
inline RDOCalcSetLocalVariable<equalType>::~RDOCalcSetLocalVariable()
{}

template <>
inline REF(RDOValue) RDOCalcSetLocalVariable<ET_EQUAL>::doCalc(PTR(RDORuntime) pRuntime)
{
	pRuntime->getMemoryStack()->set(m_name, m_pCalc->calcValue(pRuntime));
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVariable<ET_PLUS>::doCalc(PTR(RDORuntime) pRuntime)
{
	rdoRuntime::RDOValue pValue = pRuntime->getMemoryStack()->get(m_name) + m_pCalc->calcValue(pRuntime);
	pRuntime->getMemoryStack()->set(m_name, pValue);
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVariable<ET_MINUS>::doCalc(PTR(RDORuntime) pRuntime)
{
	rdoRuntime::RDOValue pValue = pRuntime->getMemoryStack()->get(m_name) - m_pCalc->calcValue(pRuntime);
	pRuntime->getMemoryStack()->set(m_name, pValue);
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVariable<ET_MULTIPLY>::doCalc(PTR(RDORuntime) pRuntime)
{
	rdoRuntime::RDOValue pValue = pRuntime->getMemoryStack()->get(m_name) * m_pCalc->calcValue(pRuntime);
	pRuntime->getMemoryStack()->set(m_name, pValue);
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVariable<ET_DIVIDE>::doCalc(PTR(RDORuntime) pRuntime)
{
	rdoRuntime::RDOValue pValue = pRuntime->getMemoryStack()->get(m_name) / m_pCalc->calcValue(pRuntime);
	pRuntime->getMemoryStack()->set(m_name, pValue);
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVariable<ET_INCR>::doCalc(PTR(RDORuntime) pRuntime)
{
	rdoRuntime::RDOValue pValue = pRuntime->getMemoryStack()->get(m_name) + RDOValue(1);
	pRuntime->getMemoryStack()->set(m_name, pValue);
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVariable<ET_DECR>::doCalc(PTR(RDORuntime) pRuntime)
{
	rdoRuntime::RDOValue pValue = pRuntime->getMemoryStack()->get(m_name) - RDOValue(1);
	pRuntime->getMemoryStack()->set(m_name, pValue);
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

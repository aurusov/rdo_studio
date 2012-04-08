/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_locvar.inl
  \author    Чирков Михаил
  \date      16.04.2011
  \brief     Локальные переменные
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_activity.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcSetLocalVariable
// --------------------------------------------------------------------------------
template <EqualType equalType>
inline RDOCalcSetLocalVariable<equalType>::RDOCalcSetLocalVariable(CREF(tstring) name, LPRDOCalc pCalc)
	: m_name (name )
	, m_pCalc(pCalc)
{}

template <EqualType equalType>
inline RDOCalcSetLocalVariable<equalType>::~RDOCalcSetLocalVariable()
{}

template <>
inline REF(RDOValue) RDOCalcSetLocalVariable<ET_EQUAL>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->getMemoryStack()->set(m_name, m_pCalc->calcValue(pRuntime));
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVariable<ET_PLUS>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue pValue = pRuntime->getMemoryStack()->get(m_name) + m_pCalc->calcValue(pRuntime);
	pRuntime->getMemoryStack()->set(m_name, pValue);
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVariable<ET_MINUS>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue pValue = pRuntime->getMemoryStack()->get(m_name) - m_pCalc->calcValue(pRuntime);
	pRuntime->getMemoryStack()->set(m_name, pValue);
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVariable<ET_MULTIPLY>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue pValue = pRuntime->getMemoryStack()->get(m_name) * m_pCalc->calcValue(pRuntime);
	pRuntime->getMemoryStack()->set(m_name, pValue);
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVariable<ET_DIVIDE>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue pValue = pRuntime->getMemoryStack()->get(m_name) / m_pCalc->calcValue(pRuntime);
	pRuntime->getMemoryStack()->set(m_name, pValue);
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVariable<ET_INCR>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue pValue = pRuntime->getMemoryStack()->get(m_name) + RDOValue(1);
	pRuntime->getMemoryStack()->set(m_name, pValue);
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVariable<ET_DECR>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue pValue = pRuntime->getMemoryStack()->get(m_name) - RDOValue(1);
	pRuntime->getMemoryStack()->set(m_name, pValue);
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

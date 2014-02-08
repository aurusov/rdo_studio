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
template <SetOperationType::Type setOperationType>
inline RDOCalcSetLocalVariable<setOperationType>::RDOCalcSetLocalVariable(CREF(tstring) name, LPRDOCalc pCalc)
	: m_name (name )
	, m_pCalc(pCalc)
{}

template <SetOperationType::Type setOperationType>
inline RDOCalcSetLocalVariable<setOperationType>::~RDOCalcSetLocalVariable()
{}

template <>
inline RDOValue RDOCalcSetLocalVariable<SetOperationType::SET>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->getMemoryStack()->set(m_name, m_pCalc->calcValue(pRuntime));
	return RDOValue();
}

template <>
inline RDOValue RDOCalcSetLocalVariable<SetOperationType::ADDITION>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue value = pRuntime->getMemoryStack()->get(m_name) + m_pCalc->calcValue(pRuntime);
	pRuntime->getMemoryStack()->set(m_name, value);
	return value;
}

template <>
inline RDOValue RDOCalcSetLocalVariable<SetOperationType::SUBTRACTION>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue value = pRuntime->getMemoryStack()->get(m_name) - m_pCalc->calcValue(pRuntime);
	pRuntime->getMemoryStack()->set(m_name, value);
	return value;
}

template <>
inline RDOValue RDOCalcSetLocalVariable<SetOperationType::MULTIPLY>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue value = pRuntime->getMemoryStack()->get(m_name) * m_pCalc->calcValue(pRuntime);
	pRuntime->getMemoryStack()->set(m_name, value);
	return value;
}

template <>
inline RDOValue RDOCalcSetLocalVariable<SetOperationType::DIVIDE>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue value = pRuntime->getMemoryStack()->get(m_name) / m_pCalc->calcValue(pRuntime);
	pRuntime->getMemoryStack()->set(m_name, value);
	return value;
}

template <>
inline RDOValue RDOCalcSetLocalVariable<SetOperationType::INCREMENT>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue value = pRuntime->getMemoryStack()->get(m_name) + RDOValue(1);
	pRuntime->getMemoryStack()->set(m_name, value);
	return value;
}

template <>
inline RDOValue RDOCalcSetLocalVariable<SetOperationType::DECRIMENT>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue value = pRuntime->getMemoryStack()->get(m_name) - RDOValue(1);
	pRuntime->getMemoryStack()->set(m_name, value);
	return value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_relevant.inl
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      16.04.2011
  \brief     RDOCalc для подбора релевантных ресурсов
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_activity.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOSetRelResParamCalc
// --------------------------------------------------------------------------------
template <SetOperationType::Type setOperationType>
inline RDOSetRelResParamCalc<setOperationType>::RDOSetRelResParamCalc(ruint relResID, ruint paramID, CREF(LPRDOCalc) pCalc)
	: m_relResID(relResID)
	, m_paramID (paramID )
	, m_pCalc   (pCalc   )
{
	if (m_pCalc)
	{
		setSrcInfo(m_pCalc->srcInfo());
	}
}

template <SetOperationType::Type setOperationType>
inline RDOSetRelResParamCalc<setOperationType>::~RDOSetRelResParamCalc()
{}

template <>
inline RDOValue RDOSetRelResParamCalc<SetOperationType::SET>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue value = m_pCalc->calcValue(pRuntime);
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResID);
	pRuntime->setResParamVal(resID, m_paramID, value);
	return value;
}

template <>
inline RDOValue RDOSetRelResParamCalc<SetOperationType::ADDITION>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResID);
	return pRuntime->getResParamValRaw(resID, m_paramID) += m_pCalc->calcValue(pRuntime);
}

template <>
inline RDOValue RDOSetRelResParamCalc<SetOperationType::SUBTRACTION>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResID);
	return pRuntime->getResParamValRaw(resID, m_paramID) -= m_pCalc->calcValue(pRuntime);
}

template <>
inline RDOValue RDOSetRelResParamCalc<SetOperationType::MULTIPLY>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResID);
	return pRuntime->getResParamValRaw(resID, m_paramID) *= m_pCalc->calcValue(pRuntime);
}

template <>
inline RDOValue RDOSetRelResParamCalc<SetOperationType::DIVIDE>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResID);
	return pRuntime->getResParamValRaw(resID, m_paramID) /= m_pCalc->calcValue(pRuntime);
}

template <>
inline RDOValue RDOSetRelResParamCalc<SetOperationType::INCREMENT>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResID);
	return pRuntime->getResParamValRaw(resID, m_paramID) += RDOValue(1);
}

template <>
inline RDOValue RDOSetRelResParamCalc<SetOperationType::DECRIMENT>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResID);
	return pRuntime->getResParamValRaw(resID, m_paramID) -= RDOValue(1);
}

CLOSE_RDO_RUNTIME_NAMESPACE

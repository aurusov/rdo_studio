/*
 * @copyright (c) RDO-Team, 2011
 * @file      relres.inl
 * @authors   Александ Барс, Урусов Андрей
 * @date      16.04.11
 * @brief     
 * @indent    4T
 */

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdo_activity.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOSetRelResParamCalc
// ********************************************************************************
template <EqualType equalType>
inline RDOSetRelResParamCalc<equalType>::RDOSetRelResParamCalc(ruint relResID, ruint paramID, CREF(LPRDOCalc) pCalc)
	: m_relResID(relResID)
	, m_paramID (paramID )
	, m_pCalc   (pCalc   )
{
	m_value = 1;
	if (m_pCalc)
	{
		setSrcInfo(m_pCalc->src_info());
	}
}

template <EqualType equalType>
inline RDOSetRelResParamCalc<equalType>::~RDOSetRelResParamCalc()
{}

template <>
inline REF(RDOValue) RDOSetRelResParamCalc<ET_EQUAL>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResID);
	pRuntime->setResParamVal(resID, m_paramID, m_pCalc->calcValue(pRuntime));
	return m_value;
}

template <>
inline REF(RDOValue) RDOSetRelResParamCalc<ET_PLUS>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResID);
	pRuntime->getResParamValRaw(resID, m_paramID) += m_pCalc->calcValue(pRuntime);
	return m_value;
}

template <>
inline REF(RDOValue) RDOSetRelResParamCalc<ET_MINUS>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResID);
	pRuntime->getResParamValRaw(resID, m_paramID) -= m_pCalc->calcValue(pRuntime);
	return m_value;
}

template <>
inline REF(RDOValue) RDOSetRelResParamCalc<ET_MULTIPLY>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResID);
	pRuntime->getResParamValRaw(resID, m_paramID) *= m_pCalc->calcValue(pRuntime);
	return m_value;
}

template <>
inline REF(RDOValue) RDOSetRelResParamCalc<ET_DIVIDE>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResID);
	pRuntime->getResParamValRaw(resID, m_paramID) /= m_pCalc->calcValue(pRuntime);
	return m_value;
}

template <>
inline REF(RDOValue) RDOSetRelResParamCalc<ET_INCR>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResID);
	pRuntime->getResParamValRaw(resID, m_paramID) += RDOValue(1);
	return m_value;
}

template <>
inline REF(RDOValue) RDOSetRelResParamCalc<ET_DECR>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResID);
	pRuntime->getResParamValRaw(resID, m_paramID) -= RDOValue(1);
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

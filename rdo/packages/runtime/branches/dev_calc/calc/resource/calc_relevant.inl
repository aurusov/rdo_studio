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
template <EqualType equalType>
inline RDOSetRelResParamCalc<equalType>::RDOSetRelResParamCalc(ruint relResID, ruint paramID, CREF(LPRDOCalc) pCalc)
	: m_relResID(relResID)
	, m_paramID (paramID )
	, m_pCalc   (pCalc   )
{
	if (m_pCalc)
	{
		setSrcInfo(m_pCalc->srcInfo());
	}
}

template <EqualType equalType>
inline RDOSetRelResParamCalc<equalType>::~RDOSetRelResParamCalc()
{}

template <>
inline void RDOSetRelResParamCalc<ET_EQUAL>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pCalc->calcValue(pRuntime);
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResID);
	RDOValue value = pRuntime->stack().pop();
	pRuntime->setResParamVal(resID, m_paramID, value);
	pRuntime->stack().push(value);
}

template <>
inline void RDOSetRelResParamCalc<ET_PLUS>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResID);
	m_pCalc->calcValue(pRuntime);
	pRuntime->getResParamValRaw(resID, m_paramID) += pRuntime->stack().pop();
}

template <>
inline void RDOSetRelResParamCalc<ET_MINUS>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResID);
	m_pCalc->calcValue(pRuntime);
	pRuntime->getResParamValRaw(resID, m_paramID) -= pRuntime->stack().pop();
}

template <>
inline void RDOSetRelResParamCalc<ET_MULTIPLY>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResID);
	m_pCalc->calcValue(pRuntime);
	pRuntime->getResParamValRaw(resID, m_paramID) *= pRuntime->stack().pop();
}

template <>
inline void RDOSetRelResParamCalc<ET_DIVIDE>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResID);
	m_pCalc->calcValue(pRuntime);
	pRuntime->getResParamValRaw(resID, m_paramID) /= pRuntime->stack().pop();
}

template <>
inline void RDOSetRelResParamCalc<ET_INCR>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResID);
	pRuntime->getResParamValRaw(resID, m_paramID) += RDOValue(1);
}

template <>
inline void RDOSetRelResParamCalc<ET_DECR>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResID);
	pRuntime->getResParamValRaw(resID, m_paramID) -= RDOValue(1);
}

CLOSE_RDO_RUNTIME_NAMESPACE

/*!
  \copyright (c) RDO-Team, 2011
  \file      relres.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      16.04.2011
  \brief     RDOCalc для подбора релевантных ресурсов
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/resource/calc_relevant.h"
#include "simulator/runtime/rdo_res_type_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOGetResourceByRelevantResourceID
// --------------------------------------------------------------------------------
RDOGetResourceByRelevantResourceID::RDOGetResourceByRelevantResourceID(ruint relevantResourceID)
	: m_relevantResourceID(relevantResourceID)
{}

REF(RDOValue) RDOGetResourceByRelevantResourceID::doCalc(CREF(LPRDORuntime) pRuntime)
{
	if (!RDOCalcGetResourceHelper::getResource(pRuntime, pRuntime->getCurrentActivity()->getResByRelRes(m_relevantResourceID), m_value))
	{
		pRuntime->error(_T("Не найден ресурс"), this);
	}
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOSetRelResParamDiapCalc
// --------------------------------------------------------------------------------
RDOSetRelResParamDiapCalc::RDOSetRelResParamDiapCalc(ruint relResID, ruint paramID, CREF(RDOValue) minValue, CREF(RDOValue) maxValue, CREF(LPRDOCalc) pCalc)
	: m_relResID(relResID)
	, m_paramID (paramID )
	, m_pCalc   (pCalc   )
	, m_minValue(minValue)
	, m_maxValue(maxValue)
{
	m_value = true;
	if (m_pCalc)
	{
		setSrcInfo(m_pCalc->src_info());
	}
}

REF(RDOValue) RDOSetRelResParamDiapCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pCalc->calcValue(pRuntime);
	m_value = pRuntime->getResParamVal(pRuntime->getCurrentActivity()->getResByRelRes(m_relResID), m_paramID);
	if (m_value < m_minValue || m_value > m_maxValue)
	{
		pRuntime->error(rdo::format(_T("Значение выходит за допустимый диапазон [%s..%s]: %s"), m_minValue.getAsString().c_str(), m_maxValue.getAsString().c_str(), m_value.getAsString().c_str()), this);
	}
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOEraseResRelCalc
// --------------------------------------------------------------------------------
RDOEraseResRelCalc::RDOEraseResRelCalc(ruint relResID, CREF(tstring) relResName)
	: m_relResID  (relResID  )
	, m_relResName(relResName)
{
	m_value = 1;
}

REF(RDOValue) RDOEraseResRelCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->onEraseRes(pRuntime->getCurrentActivity()->getResByRelRes(m_relResID), this);
	return m_value;
}

CREF(tstring) RDOEraseResRelCalc::getName() const
{
	return m_relResName;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetGroupFunctionResource
// --------------------------------------------------------------------------------
RDOCalcGetGroupFunctionResource::RDOCalcGetGroupFunctionResource()
{}

REF(RDOValue) RDOCalcGetGroupFunctionResource::doCalc(CREF(LPRDORuntime) pRuntime)
{
	LPRDOResource pResource = pRuntime->getGroupFuncRes();
	ASSERT(pResource);

	LPRDOType pType(pResource->getResType());
	ASSERT(pType);

	m_value = RDOValue(pType, pResource);
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

/*
 * copyright: (c) RDO-Team, 2011
 * filename : relres.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 16.04.11
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/relres.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOGetRelResParamCalc
// ----------------------------------------------------------------------------
RDOGetRelResParamCalc::RDOGetRelResParamCalc(ruint relResID, ruint paramID)
	: m_relResID(relResID)
	, m_paramID (paramID )
{}

REF(RDOValue) RDOGetRelResParamCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = pRuntime->getResParamVal(pRuntime->getCurrentActivity()->getResByRelRes(m_relResID), m_paramID);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOSetRelResParamDiapCalc
// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
// ---------- RDOEraseResRelCalc
// ----------------------------------------------------------------------------
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

CLOSE_RDO_RUNTIME_NAMESPACE

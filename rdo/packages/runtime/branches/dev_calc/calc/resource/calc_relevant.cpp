/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_relevant.cpp
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
#include "simulator/runtime/calc/resource/calc_resource.h"
#include "simulator/runtime/rdo_res_type_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOGetResourceByRelevantResourceID
// --------------------------------------------------------------------------------
RDOGetResourceByRelevantResourceID::RDOGetResourceByRelevantResourceID(ruint relevantResourceID)
	: m_relevantResourceID(relevantResourceID)
{}

void RDOGetResourceByRelevantResourceID::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue value;
	if (!RDOCalcGetResourceHelper::getResource(pRuntime, pRuntime->getCurrentActivity()->getResByRelRes(m_relevantResourceID), value))
	{
		pRuntime->error().push(_T("Не найден ресурс"), srcInfo());
	}
	pRuntime->stack().push(value);
}

// --------------------------------------------------------------------------------
// -------------------- RDOEraseResRelCalc
// --------------------------------------------------------------------------------
RDOEraseResRelCalc::RDOEraseResRelCalc(ruint relResID, CREF(tstring) relResName)
	: m_relResID  (relResID  )
	, m_relResName(relResName)
{}

void RDOEraseResRelCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->onEraseRes(pRuntime->getCurrentActivity()->getResByRelRes(m_relResID), this);
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

void RDOCalcGetGroupFunctionResource::doCalc(CREF(LPRDORuntime) pRuntime)
{
	LPRDOResource pResource = pRuntime->getGroupFuncRes();
	ASSERT(pResource);

	LPRDOType pType(pResource->getResType());
	ASSERT(pType);

	pRuntime->stack().push(RDOValue(pType, pResource));
}

CLOSE_RDO_RUNTIME_NAMESPACE

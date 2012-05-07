/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_resource.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      19.12.2011
  \brief     Калки для ресурсов
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/resource/calc_resource.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_res_type.h"
#include "simulator/runtime/rdo_activity_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetResourceHelper
// --------------------------------------------------------------------------------
rbool RDOCalcGetResourceHelper::getResource(CREF(LPRDORuntime) pRuntime, ruint resourceID, REF(RDOValue) result)
{
	LPRDOResource pResource = pRuntime->getResourceByID(resourceID);
	if (!pResource)
		false;

	LPRDOType pType(pResource->getResType());
	ASSERT(pType);

	result = RDOValue(pType, pResource);
	return true;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetResourceByID
// --------------------------------------------------------------------------------
RDOCalcGetResourceByID::RDOCalcGetResourceByID(CREF(ruint) resourceID)
	: m_resourceID(resourceID)
{}

void RDOCalcGetResourceByID::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue value;
	if (!RDOCalcGetResourceHelper::getResource(pRuntime, m_resourceID, value))
	{
		pRuntime->error().push(_T("Не найден ресурс"), srcInfo());
	}
	pRuntime->stack().push(value);
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetResourceParam
// --------------------------------------------------------------------------------
RDOCalcGetResourceParam::RDOCalcGetResourceParam(CREF(LPRDOCalc) pResource, ruint paramID)
	: m_pResource(pResource)
	, m_paramID  (paramID  )
{
	ASSERT(m_pResource);
}

void RDOCalcGetResourceParam::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pResource->calcValue(pRuntime);
	LPRDOResource pResource = pRuntime->stack().pop().getPointerByInterfaceSafety<IResourceType>();
	ASSERT(pResource);
	pRuntime->stack().push(pResource->getParam(m_paramID));
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetUnknowResParam
// --------------------------------------------------------------------------------
RDOCalcGetUnknowResParam::RDOCalcGetUnknowResParam(CREF(tstring) resName, CREF(tstring) parName)
	: m_resName(resName)
	, m_parName(parName)
{}

void RDOCalcGetUnknowResParam::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->error().push(rdo::format(_T("Попытка использовать несуществующий ресурс: %s.%s"), m_resName.c_str(), m_parName.c_str()), srcInfo());
}

// --------------------------------------------------------------------------------
// -------------------- RDOSetResourceParamCalc
// --------------------------------------------------------------------------------
RDOSetResourceParamCalc::RDOSetResourceParamCalc(ruint resourceID, ruint paramID, CREF(LPRDOCalc) pCalc)
	: m_resourceID(resourceID)
	, m_paramID   (paramID   )
	, m_pCalc     (pCalc     )
{
	if (m_pCalc)
	{
		setSrcInfo(m_pCalc->srcInfo());
	}
}

void RDOSetResourceParamCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pCalc->calcValue(pRuntime);
	pRuntime->setResParamVal(m_resourceID, m_paramID, pRuntime->stack().pop());
}

CLOSE_RDO_RUNTIME_NAMESPACE

/*!
  \copyright (c) RDO-Team, 2012
  \file      calc_create_resource.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      10.02.2012
  \brief     RDOCalc для создания ресурсов
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/resource/calc_create_resource.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_activity.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcCreateResource
// --------------------------------------------------------------------------------
RDOCalcCreateResource::RDOCalcCreateResource(
	ruint resourceTypeID,
	const std::vector<LPRDOCalc>& rParamCalcList,
	rbool traceFlag,
	rbool permanentFlag,
	ruint relResID
)
	: m_resourceTypeID(resourceTypeID)
	, m_traceFlag     (traceFlag     )
	, m_permanentFlag (permanentFlag )
	, m_relResID      (relResID      )
{
	m_paramCalcList.insert(m_paramCalcList.begin(), rParamCalcList.begin(), rParamCalcList.end());
	// попытка создавать постоянные ресурсы динамически
	ASSERT(m_relResID == ~0 || (m_relResID != ~0 && !m_permanentFlag));
}

RDOValue RDOCalcCreateResource::doCalc(CREF(LPRDORuntime) pRuntime)
{
	const LPRDOResourceTypeList& resourceType = pRuntime->getResType(m_resourceTypeID);
	std::vector<RDOValue> paramValueList;

	STL_FOR_ALL_CONST(m_paramCalcList, it)
	{
		paramValueList.push_back((*it)->calcValue(pRuntime));
	}

	LPRDOResource pResource = resourceType.interface_cast<IResourceType>()->createRes(pRuntime, pRuntime->getResourceId(), paramValueList, m_traceFlag, m_permanentFlag);
	ASSERT(pResource);

	if (m_relResID != ruint(~0))
	{
		pRuntime->getCurrentActivity()->setRelRes(m_relResID, pResource->getTraceID());
	}

	LPRDOType pType = resourceType;
	ASSERT(pType);

	return RDOValue(pType, pResource);
}

CLOSE_RDO_RUNTIME_NAMESPACE

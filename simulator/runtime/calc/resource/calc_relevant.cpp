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
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/resource/calc_relevant.h"
#include "simulator/runtime/calc/resource/calc_resource.h"
#include "simulator/runtime/rdo_res_type_i.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_activity.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOGetResourceByRelevantResourceID
// --------------------------------------------------------------------------------
RDOGetResourceByRelevantResourceID::RDOGetResourceByRelevantResourceID(std::size_t relevantResourceID)
	: m_relevantResourceID(relevantResourceID)
{}

RDOValue RDOGetResourceByRelevantResourceID::doCalc(const LPRDORuntime& pRuntime)
{
	RDOValue value;
	if (!RDOCalcGetResourceHelper::getResource(pRuntime, pRuntime->getCurrentActivity()->getResByRelRes(m_relevantResourceID), value))
	{
		pRuntime->error().push("Не найден ресурс", srcInfo());
	}
	return value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOEraseResRelCalc
// --------------------------------------------------------------------------------
RDOEraseResRelCalc::RDOEraseResRelCalc(std::size_t relResID, const std::string& relResName)
	: m_relResID  (relResID  )
	, m_relResName(relResName)
{}

RDOValue RDOEraseResRelCalc::doCalc(const LPRDORuntime& pRuntime)
{
	pRuntime->onEraseRes(pRuntime->getCurrentActivity()->getResByRelRes(m_relResID), this);
	return RDOValue();
}

const std::string& RDOEraseResRelCalc::getName() const
{
	return m_relResName;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetGroupFunctionResource
// --------------------------------------------------------------------------------
RDOCalcGetGroupFunctionResource::RDOCalcGetGroupFunctionResource()
{}

RDOValue RDOCalcGetGroupFunctionResource::doCalc(const LPRDORuntime& pRuntime)
{
	LPRDOResource pResource = pRuntime->getGroupFuncRes();
	ASSERT(pResource);

	LPRDOType pType(pResource->getResType());
	ASSERT(pType);

	return RDOValue(pType, pResource);
}

CLOSE_RDO_RUNTIME_NAMESPACE

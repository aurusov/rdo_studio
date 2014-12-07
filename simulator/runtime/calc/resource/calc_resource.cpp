// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
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
bool RDOCalcGetResourceHelper::getResource(const LPRDORuntime& pRuntime, std::size_t resourceID, RDOValue& result)
{
	LPRDOResource pResource = pRuntime->getResourceByID(resourceID);
	if (!pResource)
		return false;

	LPRDOType pType(pResource->getResType());
	ASSERT(pType);

	result = RDOValue(pType, pResource);
	return true;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetResourceByID
// --------------------------------------------------------------------------------
RDOCalcGetResourceByID::RDOCalcGetResourceByID(const std::size_t& resourceID)
	: m_resourceID(resourceID)
{}

RDOValue RDOCalcGetResourceByID::doCalc(const LPRDORuntime& pRuntime)
{
	RDOValue value;
	if (!RDOCalcGetResourceHelper::getResource(pRuntime, m_resourceID, value))
	{
		pRuntime->error().push("Не найден ресурс", srcInfo());
	}
	return value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetResourceParam
// --------------------------------------------------------------------------------
RDOCalcGetResourceParam::RDOCalcGetResourceParam(const LPRDOCalc& pResource, std::size_t paramID)
	: m_pResource(pResource)
	, m_paramID  (paramID  )
{
	ASSERT(m_pResource);
}

RDOValue RDOCalcGetResourceParam::doCalc(const LPRDORuntime& pRuntime)
{
	LPRDOResource pResource = m_pResource->calcValue(pRuntime).getPointerByInterface<IResourceType>();
	ASSERT(pResource);
	return pResource->getParam(m_paramID);
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetUnknowResParam
// --------------------------------------------------------------------------------
RDOCalcGetUnknowResParam::RDOCalcGetUnknowResParam(const std::string& resName, const std::string& parName)
	: m_resName(resName)
	, m_parName(parName)
{}

RDOValue RDOCalcGetUnknowResParam::doCalc(const LPRDORuntime& pRuntime)
{
	pRuntime->error().push(rdo::format("Попытка использовать несуществующий ресурс: %s.%s", m_resName.c_str(), m_parName.c_str()), srcInfo());
	return RDOValue();
}

// --------------------------------------------------------------------------------
// -------------------- RDOSetResourceParamCalc
// --------------------------------------------------------------------------------
RDOSetResourceParamCalc::RDOSetResourceParamCalc(std::size_t resourceID, std::size_t paramID, const LPRDOCalc& pCalc)
	: m_resourceID(resourceID)
	, m_paramID   (paramID   )
	, m_pCalc     (pCalc     )
{
	if (m_pCalc)
	{
		setSrcInfo(m_pCalc->srcInfo());
	}
}

RDOValue RDOSetResourceParamCalc::doCalc(const LPRDORuntime& pRuntime)
{
	pRuntime->setResParamVal(m_resourceID, m_paramID, m_pCalc->calcValue(pRuntime));
	return RDOValue();
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcSetResourceTrace
// --------------------------------------------------------------------------------
RDOCalcSetResourceTrace::RDOCalcSetResourceTrace(const LPRDOCalc& getResource, bool traceValue)
	: m_getResource(getResource)
	, m_traceValue(traceValue)
{}

RDOValue RDOCalcSetResourceTrace::doCalc(const LPRDORuntime& pRuntime)
{
	RDOValue value = 0;
	LPRDOResource pResource = m_getResource->calcValue(pRuntime).getPointerByInterface<IResourceType>();
	ASSERT(pResource);
	pResource->setTrace(m_traceValue);
	return value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

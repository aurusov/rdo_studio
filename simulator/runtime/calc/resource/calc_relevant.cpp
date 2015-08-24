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
    std::size_t resId = pRuntime->getCurrentActivity()->getResByRelRes(m_relResID);
    LPRDOResource pResource = pRuntime->getResourceByID(resId);
    pResource->onDestroy(pRuntime, this);
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

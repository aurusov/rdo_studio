// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOSetResourceParam
// --------------------------------------------------------------------------------
template <SetOperation::Type setOperationType>
inline RDOSetResourceParam<setOperationType>::RDOSetResourceParam(const LPRDOCalc& getResource, const std::size_t paramID, const LPRDOCalc& pCalc)
    : m_getResource (getResource)
    , m_paramID     (paramID )
    , m_pCalc       (pCalc   )
{
    if (m_pCalc)
    {
        setSrcInfo(m_pCalc->srcInfo());
    }
}

template <SetOperation::Type setOperationType>
inline RDOSetResourceParam<setOperationType>::~RDOSetResourceParam()
{}

template <>
inline RDOValue RDOSetResourceParam<SetOperation::Type::NOCHANGE>::doCalc(const LPRDORuntime& pRuntime)
{
    LPRDOResource pResource = m_getResource->calcValue(pRuntime).getPointerByInterface<IResourceType>();
    ASSERT(pResource);
    RDOValue value(true);
    return value;
}

template <>
inline RDOValue RDOSetResourceParam<SetOperation::Type::SET>::doCalc(const LPRDORuntime& pRuntime)
{
    LPRDOResource pResource = m_getResource->calcValue(pRuntime).getPointerByInterface<IResourceType>();
    ASSERT(pResource);
    RDOValue value = m_pCalc->calcValue(pRuntime);
    pResource->setParam(m_paramID, value);
    return value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
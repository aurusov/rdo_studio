// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_resource.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOResource
// --------------------------------------------------------------------------------
RDOResource::RDOResource(const LPRDORuntime& /*pRuntime*/, const ParamList& paramList, LPIResourceType pResType, std::size_t resID, std::size_t typeID, bool trace, bool temporary, bool isNested)
    : RDORuntimeObject   (                                      )
    , RDOTraceableObject (trace, resID, rdo::toString(resID + 1))
    , m_temporary        (temporary                             )
    , m_state            (RDOResource::ConvertStatus::NONE                  )
    , m_type             (typeID                                )
    , m_referenceCount   (0                                     )
    , m_resType          (pResType                              )
    , m_isNested         (isNested                              )
{
    appendParams(paramList.begin(), paramList.end());
}

// TODO копирующий конструктор не используется - нужен ли он?
RDOResource::RDOResource(const LPRDORuntime& /*pRuntime*/, const RDOResource& copy)
    : RDORuntimeObject   (                 )
    , RDOTraceableObject (copy.traceable(), copy.getTraceID(), copy.traceId())
    , m_paramList        (copy.m_paramList )
    , m_temporary        (copy.m_temporary )
    , m_state            (copy.m_state     )
    , m_type             (copy.m_type      )
    , m_referenceCount   (0                )
    , m_resType          (copy.m_resType   )
    , m_typeId           (copy.m_typeId    )
    , m_isNested         (copy.m_isNested    )
{
    appendParams(copy.m_paramList.begin(), copy.m_paramList.end());
// TODO посмотреть history и принять решение о комментарии
//    getRuntime()->incrementResourceIdReference( getTraceID() );
}

RDOResource::~RDOResource()
{}

bool RDOResource::operator== (const RDOResource& other) const
{
    if (m_type != other.m_type)
        return false;

    if (m_paramList.size() != other.m_paramList.size())
        return false;

    int size = m_paramList.size();
    for (int i = 0; i < size; ++i)
    {
        if (m_paramList[i] != other.m_paramList[i])
            return false;
    }
    return true;
}

LPRDOResource RDOResource::clone(const LPRDORuntime& pRuntime) const
{
    const LPRDOResourceTypeList& resourceType = pRuntime->getResType(m_type);
    ASSERT(resourceType);
    LPRDOResource pResource = rdo::Factory<RDOResource>::create(pRuntime, m_paramList, resourceType.interface_cast<IResourceType>(), getTraceID(), m_type, traceable(), m_temporary, m_isNested);
    ASSERT(pResource);
    return pResource;
}

std::string RDOResource::getTypeId()
{
    std::ostringstream str;
    str << m_type;
    return str.str();
}

std::string RDOResource::traceParametersValue()
{
    std::ostringstream str;
    if(m_paramList.size() > 0)
    {
        ParamList::iterator end = m_paramList.end();
        for (ParamList::iterator it = m_paramList.begin();;)
        {
#ifdef RDOSIM_COMPATIBLE
            std::ostringstream _str;
            _str << *it;
            std::string::size_type pos = _str.str().find("e");
            if (pos != std::string::npos)
            {
                std::string __str = _str.str();
                __str.erase(pos + 2, 1);
                str << __str.c_str();
            }
            else
            {
                str << _str.str().c_str();
            }
#else
            str << *it;
#endif
            if (++it == end)
                break;
            str << " ";
        }
    }
    return str.str();
}

std::string RDOResource::traceResourceState(char prefix, const LPRDORuntime& pRuntime)
{
    std::ostringstream res;
    if (traceable() || (prefix != '\0'))
    {
        if (m_state == RDOResource::ConvertStatus::NOCHANGE || m_state == RDOResource::ConvertStatus::NONEXIST)
            return "";

        if (prefix != '\0')
            res << prefix;

        switch (m_state)
        {
        case RDOResource::ConvertStatus::CREATE:
            res << "RC ";
            break;
        case RDOResource::ConvertStatus::ERASE:
            res << "RE "
#ifdef RDOSIM_COMPATIBLE
                << pRuntime->getCurrentTime() << " "
                << traceTypeId()              << " "
                << traceId()                  << std::endl;
            return res.str();
#else
                ;
#endif
            break;
        default:
            res << "RK ";
            break;
        }

        res << pRuntime->getCurrentTime() << " "
            << traceTypeId()              << " "
            << traceId()                  << " "
            << traceParametersValue()     << std::endl;
    }
    return res.str();
}

void RDOResource::setRuntime(const LPRDORuntime& /*pRuntime*/)
{
    // TODO походу надо удалить метод
    NEVER_REACH_HERE;
}

std::string RDOResource::whoAreYou()
{
    return "rdoRes";
}

void RDOResource::makeTemporary(bool value)
{
    m_temporary = value;
}

RDOResource::ConvertStatus RDOResource::getState() const
{
    return m_state;
}

void RDOResource::setState(RDOResource::ConvertStatus value)
{
    m_state = value;
}

bool RDOResource::checkType(std::size_t type) const
{
    return m_type == type;
}

const LPIResourceType& RDOResource::getResType() const
{
    return m_resType;
}

std::size_t RDOResource::getType() const
{
    return m_type;
}

const RDOResource::ParamList& RDOResource::getParamList() const
{
    return m_paramList;
}

const RDOValue& RDOResource::getParam(std::size_t index) const
{
    ASSERT(index < m_paramList.size());
    return m_paramList[index];
}

RDOValue& RDOResource::getParamRaw(std::size_t index)
{
    ASSERT(index < m_paramList.size());
    setState(ConvertStatus::KEEP);
    return m_paramList[index];
}

void RDOResource::setParam(std::size_t index, const RDOValue& value)
{
    ASSERT(index < m_paramList.size());
    setState(ConvertStatus::KEEP);
    m_paramList[index] = value;
}

std::size_t RDOResource::paramsCount() const
{
    return m_paramList.size();
}

void RDOResource::appendParams(const ParamCIt& from_begin, const ParamCIt& from_end)
{
    m_paramList.insert(m_paramList.end(), from_begin, from_end);
}

bool RDOResource::canFree() const
{
    return m_referenceCount == 0;
}

void RDOResource::incRef()
{
    ++m_referenceCount;
}

void RDOResource::decRef()
{
    --m_referenceCount;
}

std::string RDOResource::traceTypeId()
{
    return m_typeId.empty() ? (m_typeId = getTypeId()) : m_typeId;
}

void RDOResource::onDestroy(const LPRDORuntime& pRuntime, const LPRDOEraseResRelCalc& pCalc)
{
    for (auto& param: m_paramList)
    {
        if(param.type().object_dynamic_cast<RDOResourceTypeList>())
        {
            LPRDOResource pNestedResource = param.getPointerByType<RDOResourceTypeList>();
            pNestedResource->onDestroy(pRuntime, pCalc);
        }
    }
    pRuntime->onEraseRes(getTraceID(), pCalc);
}

bool RDOResource::isNested()
{
    return m_isNested;
}

CLOSE_RDO_RUNTIME_NAMESPACE

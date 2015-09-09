// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/tracer_pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/tracer_resource.h"
#include "app/rdo_studio/src/tracer/tracer_resource_type.h"
#include "app/rdo_studio/src/tracer/tracer_values.h"
#include "app/rdo_studio/src/tracer/tracer.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::tracer;

// --------------------------------------------------------------------------------
// -------------------- Param
// --------------------------------------------------------------------------------
Param::Param(const LPResource& pResource)
    : Serie(Kind::PARAM)
    , m_pResource(pResource)
{}

Param::~Param()
{}

const LPResource& Param::getResource() const
{
    return m_pResource;
}

ParamInfo* Param::getParamInfo() const
{
    LPParam pThis(const_cast<Param*>(this));
    int index = m_pResource->getParamIndex(pThis);
    return index != -1
        ? m_pResource->getType()->getParamInfo(index)
        : NULL;
}

void Param::getCaptions(std::vector<std::string>& captions, const int valueCount) const
{
    switch (getParamInfo()->getParamType())
    {
    case ParamInfo::Type::INTEGER:
        Serie::getCaptionsInt(captions, valueCount);
        break;

    case ParamInfo::Type::REAL:
        Serie::getCaptionsDouble(captions, valueCount);
        break;

    case ParamInfo::Type::ENUMERATIVE:
    case ParamInfo::Type::BOOL:
    case ParamInfo::Type::STRING:
    {
        Serie::getCaptions(captions, valueCount);
        int delta = getParamInfo()->getEnumCount();
        m_minValue = 0;
        m_maxValue = delta - 1;
        int real_val_count = valueCount;
        if (delta > real_val_count)
        {
            while ((int)((delta - 1) / (real_val_count - 1)) != ((double)(delta - 1) / (real_val_count - 1)))
                real_val_count--;
        }
        else
        {
            real_val_count = delta;
        }
        int valo = (int)m_minValue;
        int valoffset = real_val_count != 1 ? (delta - 1) / (real_val_count - 1) : 0;
        for (int i = 0; i < real_val_count; i++)
        {
            captions.push_back(getParamInfo()->getEnumValue(valo));
            valo += valoffset;
        }
        break;
    }
    case ParamInfo::Type::ARRAY:
    case ParamInfo::Type::RESOURCE:
            break;
    }
}

// --------------------------------------------------------------------------------
// -------------------- Resource
// --------------------------------------------------------------------------------
Resource::Resource(const LPResourceType& pResType, const QString& name, int id)
    : ChartTreeItem()
    , m_name(name)
    , m_id(id)
    , m_pResourceType(pResType)
    , m_erased(false)
{
    int count = m_pResourceType->getParamsCount();
    for (int i = 0; i < count; i++)
    {
        addParam(rdo::Factory<Param>::create(this));
    }
}

Resource::~Resource()
{}

const QString& Resource::getName() const
{
    return m_name;
}

void Resource::setName(const QString& name)
{
    m_name = name;
}

int Resource::getID() const
{
    return m_id;
}

const LPResourceType& Resource::getType() const
{
    return m_pResourceType;
}

void Resource::addParam(const LPParam& pParam)
{
    ASSERT(pParam);
    pParam->setTitle(m_name + "." + m_pResourceType->getParamInfo(m_paramList.size())->getName());
    m_paramList.push_back(pParam);
}

LPParam Resource::getParam(unsigned int index) const
{
    if (index >= m_paramList.size() || index < 0)
        return LPParam();
    return m_paramList.at(index);
}

int Resource::getParamIndex(const LPParam& pParam) const
{
    int count = m_paramList.size();
    for (int i = 0; i < count; i++)
    {
        if (m_paramList.at(i) == pParam)
        {
            return i;
        }
    }
    return -1;
}

void Resource::setParams(std::string& line, Time* const pTime, const int eventIndex, const bool erasing)
{
    int count = m_paramList.size();
    for (int i = 0; i < count; i++)
    {
        Value* pPrevValue;
        m_paramList.at(i)->getLastValue(pPrevValue);
        std::string nextValue = g_pTracer->getNextValue(line);
        double newValue;
        if (erasing)
        {
            newValue = pPrevValue->getValue();
        }
        else
        {
            switch (m_pResourceType->getParamInfo(i)->getParamType())
            {
            case ParamInfo::Type::BOOL:
                newValue = nextValue == "true" ? 1.0 : 0.0;
                break;

            case ParamInfo::Type::STRING:
                newValue = m_pResourceType->getParamInfo(i)->addStringValue(nextValue);
                break;

            case ParamInfo::Type::RESOURCE:
                newValue = 0;
                break;

            default:
                newValue = boost::lexical_cast<double>(nextValue);
                break;
            }
        }

        Value* pNewValue = new Value(pTime, eventIndex, newValue);
        m_paramList.at(i)->addValue(pNewValue);
    }
}

void Resource::setErased(const bool value)
{
    if (m_erased != value)
    {
        m_erased = value;
    }
}

bool Resource::isErased() const
{
    return m_erased;
}

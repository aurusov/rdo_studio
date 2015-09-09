// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_value.h"
#include "converter/smr2rdox/rdo_type.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOValue
// --------------------------------------------------------------------------------
RDOValue::RDOValue(const int& value, const RDOParserSrcInfo& src_info)
    : RDOParserSrcInfo(src_info)
    , m_value(value)
    , m_type(rdo::Factory<RDOType__INT>::create())
{}

RDOValue::RDOValue(const std::size_t& value, const RDOParserSrcInfo& src_info)
    : RDOParserSrcInfo(src_info)
    , m_value(value)
    , m_type(rdo::Factory<RDOType__INT>::create())
{}

RDOValue::RDOValue(const double& value, const RDOParserSrcInfo& src_info)
    : RDOParserSrcInfo(src_info)
    , m_value(value)
    , m_type(rdo::Factory<RDOType__REAL>::create())
{}

RDOValue::RDOValue(const std::string& value, const RDOParserSrcInfo& src_info)
    : RDOParserSrcInfo(src_info)
    , m_value(value)
    , m_type(rdo::Factory<RDOType__STRING>::create())
{}

RDOValue::RDOValue(const LPRDOValue& pValue)
    : RDOParserSrcInfo(pValue->src_info())
    , m_value         (pValue->m_value   )
    , m_type          (pValue->m_type    )
{}

RDOValue::RDOValue(const rdo::runtime::RDOValue& value, const LPRDOType& type, const RDOParserSrcInfo& src_info)
    : RDOParserSrcInfo(src_info)
    , m_value(value)
    , m_type(type)
{}

RDOValue::RDOValue(const LPRDOType& type, const RDOParserSrcInfo& src_info)
    : RDOParserSrcInfo(src_info)
    , m_value(type->type())
    , m_type(type)
{}

// Для t_identificator известно только имя, но не тип
RDOValue::RDOValue(const RDOParserSrcInfo& src_info)
    : RDOParserSrcInfo(src_info)
    , m_value(rdo::runtime::RDOValue(src_info.src_text(), rdo::runtime::g_IDENTIFICATOR))
    , m_type(rdo::Factory<RDOType__IDENTIFICATOR>::create())
{}

// Неопределенный тип
RDOValue::RDOValue()
    : RDOParserSrcInfo()
    , m_value(rdo::runtime::RDOValue(rdo::runtime::g_UNKNOW.object_parent_cast<rdo::runtime::RDOType>()))
    , m_type(rdo::Factory<RDOType__UNKNOW>::create())
{}

const LPRDOType& RDOValue::type() const
{
    return m_type;
}

rdo::runtime::RDOType::Type RDOValue::typeID() const
{
    return m_type->type()->typeID();
}

const rdo::runtime::RDOValue& RDOValue::value() const
{
    return m_value;
}

const rdo::runtime::RDOValue* RDOValue::operator->() const
{
    return &m_value;
}

bool RDOValue::defined() const
{
    return m_value.typeID() != rdo::runtime::RDOType::Type::UNKNOW;
}

bool RDOValue::constant() const
{
    return
        m_value.typeID() == rdo::runtime::RDOType::Type::INT  ||
        m_value.typeID() == rdo::runtime::RDOType::Type::REAL ||
        m_value.typeID() == rdo::runtime::RDOType::Type::BOOL ||
        m_value.typeID() == rdo::runtime::RDOType::Type::STRING;
}

LPRDOValue RDOValue::getIdentificator(const std::string& identificator)
{
    return rdo::Factory<RDOValue>::create(RDOParserSrcInfo(identificator));
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

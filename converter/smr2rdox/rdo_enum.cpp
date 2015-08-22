// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_enum.h"
#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdoparser_error.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOEnumType
// --------------------------------------------------------------------------------
RDOEnumType::RDOEnumType()
    : RDOType(rdo::runtime::g_UNKNOW)
{
    m_pType = rdo::Factory<rdo::runtime::RDOEnumType>::create();
    ASSERT(m_pType);
}

RDOEnumType::RDOEnumType(const rdo::runtime::LPRDOEnumType& pEnumType)
    : RDOType(rdo::runtime::g_UNKNOW)
{
    rdo::runtime::LPRDOEnumType pType = rdo::Factory<rdo::runtime::RDOEnumType>::create();
    m_pType = pType;
    ASSERT(m_pType);

    rdo::runtime::RDOEnumType::Enums::const_iterator it = pEnumType->begin();
    while (it != pEnumType->end())
    {
        pType->add(*it);
    }
}

RDOEnumType::~RDOEnumType()
{}

std::string RDOEnumType::name() const
{
    std::string str = "(";
    rdo::runtime::RDOEnumType::const_iterator it = getEnums()->begin();
    while (it != getEnums()->end())
    {
        str += *it;
        ++it;
        if (it != getEnums()->end())
        {
            str += ", ";
        }
    }
    str += ")";
    return str;
}

LPRDOType RDOEnumType::type_cast(const LPRDOType& pFrom, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    switch (pFrom->type()->typeID())
    {
        case rdo::runtime::RDOType::Type::ENUM:
        {
            LPRDOEnumType pEnum(const_cast<RDOEnumType*>(this));
            // Это один и тот же тип
            if (pEnum == pFrom)
                return pEnum;

            // Типы разные, сгенерим ошибку
            if (pEnum.compare(pFrom.object_static_cast<RDOEnumType>()))
            {
                rdo::converter::smr2rdox::g_error().push_only(src_info,     "Используются различные перечислимые типы с одинаковыми значениями");
                rdo::converter::smr2rdox::g_error().push_only(to_src_info,   to_src_info.src_text());
                rdo::converter::smr2rdox::g_error().push_only(src_info,     "и");
                rdo::converter::smr2rdox::g_error().push_only(from_src_info, from_src_info.src_text());
                rdo::converter::smr2rdox::g_error().push_only(src_info,     "Возможно, удобнее использовать первый из них как перечислимый, а второй как such_as на него, тогда параметры можно будет сравнивать и присваивать");
            }
            else
            {
                rdo::converter::smr2rdox::g_error().push_only(src_info,     "Несоответствие перечислимых типов");
                rdo::converter::smr2rdox::g_error().push_only(to_src_info,   to_src_info.src_text());
                rdo::converter::smr2rdox::g_error().push_only(src_info,     "и");
                rdo::converter::smr2rdox::g_error().push_only(from_src_info, from_src_info.src_text());
            }
            rdo::converter::smr2rdox::g_error().push_done();
            break;
        }
        case rdo::runtime::RDOType::Type::STRING       :
        case rdo::runtime::RDOType::Type::IDENTIFICATOR:
        {
            if (getEnums()->exist(from_src_info.src_text()))
            {
                LPRDOEnumType pEnum(const_cast<RDOEnumType*>(this));
                return pEnum;
            }
            rdo::converter::smr2rdox::g_error().error(src_info, rdo::format("Значение '%s' не является элементом перечислимого типа %s", from_src_info.src_text().c_str(), to_src_info.src_text().c_str()));
            break;
        }
        default:
            rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format("Ожидается значение перечислимого типа, найдено: %s", from_src_info.src_text().c_str()));
            rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
            rdo::converter::smr2rdox::g_error().push_done();
            break;
    }
    return NULL;
}

LPRDOValue RDOEnumType::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    LPRDOValue pToValue;
    LPRDOEnumType pEnum(const_cast<RDOEnumType*>(this));
    try
    {
        switch (pFrom->typeID())
        {
        case rdo::runtime::RDOType::Type::IDENTIFICATOR:
            pToValue = (getEnums()->findEnum(pFrom->value().getIdentificator()) != rdo::runtime::RDOEnumType::END) ?
                rdo::Factory<RDOValue>::create(rdo::runtime::RDOValue(getEnums(), pFrom->value().getIdentificator()), pEnum, pFrom->src_info()) :
                rdo::Factory<RDOValue>::create(rdo::Factory<RDOType__UNKNOW>::create().object_parent_cast<RDOType>(), pFrom->src_info());
            break;

        case rdo::runtime::RDOType::Type::STRING:
            pToValue = (getEnums()->findEnum(pFrom->value().getAsString()) != rdo::runtime::RDOEnumType::END) ?
                rdo::Factory<RDOValue>::create(rdo::runtime::RDOValue(getEnums(), pFrom->value().getAsString()), pEnum, pFrom->src_info()) :
                rdo::Factory<RDOValue>::create(rdo::Factory<RDOType__UNKNOW>::create().object_parent_cast<RDOType>(), pFrom->src_info());
            break;

        case rdo::runtime::RDOType::Type::ENUM:
            if (m_pType == pFrom->type()->type())
                pToValue = rdo::Factory<RDOValue>::create(pFrom);
            break;

        default:
            break;
        }
    }
    catch (const rdo::runtime::RDOValueException&)
    {}

    if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::Type::UNKNOW)
    {
        rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format("Неверное значение параметра перечислимого типа: %s", pFrom->src_info().src_text().c_str()));
        rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format("Возможные значения: %s", name().c_str()));
        rdo::converter::smr2rdox::g_error().push_done();
    }
    return pToValue;
}

rdo::runtime::LPRDOCalc RDOEnumType::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPRDOType& pType) const
{
    return RDOType::calc_cast(pCalc, pType);
}

LPRDOValue RDOEnumType::get_default() const
{
    LPRDOEnumType pEnum(const_cast<RDOEnumType*>(this));
    return rdo::Factory<RDOValue>::create(rdo::runtime::RDOValue(getEnums(), 0), pEnum, RDOParserSrcInfo());
}

void RDOEnumType::writeModelStructure(std::ostream& stream) const
{
    stream << "E " << getEnums()->getValues().size() << std::endl;
    for (std::size_t i = 0; i < getEnums()->getValues().size(); i++)
    {
        stream << "    " << i << " " << getEnums()->getValues().at(i) << std::endl;
    }
}

void RDOEnumType::add(const LPRDOValue& pNext)
{
    ASSERT(pNext);

    if (getEnums()->findEnum(pNext->value().getAsString()) != rdo::runtime::RDOEnumType::END)
    {
        rdo::converter::smr2rdox::g_error().error(pNext->src_info(), rdo::format("Значение перечислимого типа уже существует: %s", pNext->src_text().c_str()));
    }
    getEnums()->add(pNext->value().getAsString());
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

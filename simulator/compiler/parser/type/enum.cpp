// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/enum.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_error.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOEnumType
// --------------------------------------------------------------------------------
RDOEnumType::RDOEnumType()
{}

RDOEnumType::~RDOEnumType()
{}

std::string RDOEnumType::name() const
{
    std::string str = "(";
    rdo::runtime::RDOEnumType::const_iterator it = begin();
    while (it != end())
    {
        str += *it;
        ++it;
        if (it != end())
        {
            str += ", ";
        }
    }
    str += ")";
    return str;
}

LPIType RDOEnumType::type_cast(const LPIType& from, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    switch (from.object_dynamic_cast<RDOType>()->typeID())
    {
        case rdo::runtime::RDOType::Type::ENUM:
        {
            LPRDOEnumType pEnum(const_cast<RDOEnumType*>(this));
            // Это один и тот же тип
            if (pEnum == from)
                return pEnum;

            // Типы разные, сгенерим ошибку
            if (pEnum.compare(from.object_static_cast<RDOEnumType>()))
            {
                parser::g_error().push_only(src_info,     "Используются различные перечислимые типы с одинаковыми значениями");
                parser::g_error().push_only(to_src_info,   to_src_info.src_text());
                parser::g_error().push_only(src_info,     "и");
                parser::g_error().push_only(from_src_info, from_src_info.src_text());
                parser::g_error().push_only(src_info,     "Возможно, удобнее использовать первый из них как перечислимый, а второй как such_as на него, тогда параметры можно будет сравнивать и присваивать");
            }
            else
            {
                parser::g_error().push_only(src_info,     "Несоответствие перечислимых типов");
                parser::g_error().push_only(to_src_info,   to_src_info.src_text());
                parser::g_error().push_only(src_info,     "и");
                parser::g_error().push_only(from_src_info, from_src_info.src_text());
            }
            parser::g_error().push_done();
            break;
        }
        case rdo::runtime::RDOType::Type::STRING       :
        case rdo::runtime::RDOType::Type::IDENTIFICATOR:
        {
            if (exist(from_src_info.src_text()))
            {
                LPRDOEnumType pEnum(const_cast<RDOEnumType*>(this));
                return pEnum;
            }
            parser::g_error().error(src_info, rdo::format("Значение '%s' не является элементом перечислимого типа %s", from_src_info.src_text().c_str(), to_src_info.src_text().c_str()));
            break;
        }
        default:
            parser::g_error().push_only(src_info,    rdo::format("Ожидается значение перечислимого типа, найдено: %s", from_src_info.src_text().c_str()));
            parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
            parser::g_error().push_done();
            break;
    }
    return NULL;
}

LPRDOValue RDOEnumType::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    ASSERT(pFrom);

    LPRDOValue pToValue;
    LPRDOEnumType pEnum(const_cast<RDOEnumType*>(this));
    try
    {
        switch (pFrom->typeID())
        {
        case rdo::runtime::RDOType::Type::IDENTIFICATOR:
            if (findEnum(pFrom->value().getIdentificator()) != rdo::runtime::RDOEnumType::END)
            {
                LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pEnum, to_src_info);
                ASSERT(pType);
                pToValue = rdo::Factory<RDOValue>::create(rdo::runtime::RDOValue(pEnum, pFrom->value().getIdentificator()), pFrom->src_info(), pType);
                ASSERT(pToValue);
            }
            else
            {
                pToValue = RDOValue::getUnknow(pFrom->src_info());
                ASSERT(pToValue);
            }
            break;

        case rdo::runtime::RDOType::Type::STRING:
            if (findEnum(pFrom->value().getAsString()) != rdo::runtime::RDOEnumType::END)
            {
                LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pEnum, to_src_info);
                ASSERT(pType);
                pToValue = rdo::Factory<RDOValue>::create(rdo::runtime::RDOValue(pEnum, pFrom->value().getAsString()), pFrom->src_info(), pType);
                ASSERT(pToValue);
            }
            else
            {
                pToValue = RDOValue::getUnknow(pFrom->src_info());
                ASSERT(pToValue);
            }
            break;

        case rdo::runtime::RDOType::Type::ENUM:
            if (pEnum == pFrom->typeInfo()->itype())
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
        parser::g_error().push_only(src_info,    rdo::format("Неверное значение параметра перечислимого типа: %s", pFrom->src_info().src_text().c_str()));
        parser::g_error().push_only(to_src_info, rdo::format("Возможные значения: %s", name().c_str()));
        parser::g_error().push_done();
    }
    return pToValue;
}

rdo::runtime::LPRDOCalc RDOEnumType::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPIType& /*pType*/) const
{
    return pCalc;
}

rdo::runtime::RDOValue RDOEnumType::get_default() const
{
    return rdo::runtime::RDOValue(rdo::runtime::LPRDOEnumType(const_cast<RDOEnumType*>(this)), 0);
}

void RDOEnumType::writeModelStructure(std::ostream& stream) const
{
    stream << "E " << getValues().size() << std::endl;
    for (std::size_t i = 0; i < getValues().size(); i++)
    {
        stream << "    " << i << " " << getValues().at(i) << std::endl;
    }
}

void RDOEnumType::add(const LPRDOValue& pNext)
{
    ASSERT(pNext);

    if (findEnum(pNext->value().getAsString()) != rdo::runtime::RDOEnumType::END)
    {
        parser::g_error().error(pNext->src_info(), rdo::format("Значение перечислимого типа уже существует: %s", pNext->src_text().c_str()));
    }
    rdo::runtime::RDOEnumType::add(pNext->value().getAsString());
}

namespace
{

LPExpression contextUnknownEnum(const std::string& enumValue, std::size_t /*index*/, const RDOParserSrcInfo& srcInfo)
{
    LPTypeInfo typeInfo = rdo::Factory<TypeInfo>::delegate<RDOType__IDENTIFICATOR>(srcInfo);
    return rdo::Factory<Expression>::create(
        typeInfo,
        rdo::Factory<rdo::runtime::RDOCalcConst>::create(rdo::runtime::RDOValue(enumValue, typeInfo->type())),
        srcInfo
    );
}

}

Context::LPFindResult RDOEnumType::onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const
{
    if (method == Context::METHOD_GET)
    {
        std::size_t index = findEnum(params.identifier());
        const auto enumValue = getValues()[index];
        return rdo::Factory<FindResult>::create(CreateExpression(boost::bind(&contextUnknownEnum, enumValue, index, srcInfo)));
    }

    return rdo::Factory<FindResult>::create();
}

CLOSE_RDO_PARSER_NAMESPACE

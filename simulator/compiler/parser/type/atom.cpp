// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/atom.h"
#include "simulator/compiler/parser/type/info.h"
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/rdoparser_error.h"
#include "simulator/runtime/calc/operation/calc_unary.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ATOM_TYPE_PARSER
// --------------------------------------------------------------------------------
#define DECLARE_ATOM_TYPE_PARSER(Type, TypeName) \
std::string RDOType__##Type::name() const        \
{                                                \
    return TypeName;                             \
}

DECLARE_ATOM_TYPE_PARSER(UNKNOW,        "unknow"       );
DECLARE_ATOM_TYPE_PARSER(EMPTY,         "void"         );
DECLARE_ATOM_TYPE_PARSER(IDENTIFICATOR, "identificator");
DECLARE_ATOM_TYPE_PARSER(INT,           "integer"      );
DECLARE_ATOM_TYPE_PARSER(REAL,          "real"         );
DECLARE_ATOM_TYPE_PARSER(BOOL,          "bool"         );
DECLARE_ATOM_TYPE_PARSER(STRING,        "string"       );

// --------------------------------------------------------------------------------
// -------------------- RDOType__UNKNOW
// --------------------------------------------------------------------------------
LPIType RDOType__UNKNOW::type_cast(const LPIType& /*pFrom*/, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& /*to_src_info*/, const RDOParserSrcInfo& src_info) const
{
    parser::g_error().error(src_info, rdo::format("Внутренная ошибка парсера. Невозможно преобразовать тип '%s' к неизвестному типу", from_src_info.src_text().c_str()));
    return NULL;
}

LPRDOValue RDOType__UNKNOW::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    ASSERT(pFrom);

    parser::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к неизвестному типу", pFrom->src_info().src_text().c_str()));
    parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
    parser::g_error().push_done();
    return LPRDOValue(NULL);
}

rdo::runtime::LPRDOCalc RDOType__UNKNOW::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPIType& /*pType*/) const
{
    return pCalc;
}

rdo::runtime::RDOValue RDOType__UNKNOW::get_default() const
{
    return rdo::runtime::RDOValue();
}

void RDOType__UNKNOW::writeModelStructure(std::ostream& /*stream*/) const
{
    parser::g_error().error(RDOParserSrcInfo(), "Внутренная ошибка парсера. Невозможно записать неизвестный тип в отчет");
    NEVER_REACH_HERE;
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__EMPTY
// --------------------------------------------------------------------------------
LPIType RDOType__EMPTY::type_cast(const LPIType& pFrom, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& /*to_src_info*/, const RDOParserSrcInfo& src_info) const
{
    if (pFrom.object_dynamic_cast<RDOType>()->typeID() == rdo::runtime::RDOType::Type::EMPTY)
    {
        return rdo::Factory<RDOType__EMPTY>::create();
    }
    parser::g_error().error(src_info, rdo::format("Невозможно преобразовать '%s' к void", from_src_info.src_text().c_str()));
    return NULL;
}

LPRDOValue RDOType__EMPTY::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    ASSERT(pFrom);

    if (pFrom->value().type()->typeID() == rdo::runtime::RDOType::Type::EMPTY)
    {
        return rdo::Factory<RDOValue>::create(pFrom);
    }

    parser::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к void-типу", pFrom->src_info().src_text().c_str()));
    parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
    parser::g_error().push_done();
    return LPRDOValue(NULL);
}

rdo::runtime::LPRDOCalc RDOType__EMPTY::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPIType& /*pType*/) const
{
    return pCalc;
}

rdo::runtime::RDOValue RDOType__EMPTY::get_default() const
{
    return rdo::runtime::RDOValue();
}

void RDOType__EMPTY::writeModelStructure(std::ostream& /*stream*/) const
{
    parser::g_error().error(RDOParserSrcInfo(), "Внутренная ошибка парсера. Невозможно записать void-тип в отчет");
    NEVER_REACH_HERE;
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__INT
// --------------------------------------------------------------------------------
LPIType RDOType__INT::type_cast(const LPIType& pFrom, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    ASSERT(pFrom);

    switch (pFrom.object_dynamic_cast<RDOType>()->typeID())
    {
        case rdo::runtime::RDOType::Type::INT :
            return rdo::Factory<RDOType__INT>::create();
        case rdo::runtime::RDOType::Type::REAL:
            parser::g_error().warning(src_info, rdo::format("Преобразование '%s' в '%s', возможна потеря данных", pFrom->name().c_str(), name().c_str()));
            return rdo::Factory<RDOType__REAL>::create();
        default:
            if (!from_src_info.src_text().empty())
            {
                parser::g_error().push_only(src_info, rdo::format("Ожидается целочисленное значение, найдено: %s", from_src_info.src_text().c_str()));
            }
            else
            {
                parser::g_error().push_only(from_src_info, rdo::format("Ожидается целочисленное значение, найдено: %s", pFrom->name().c_str()));
            }
            parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
            parser::g_error().push_done();
    }
    return NULL;
}

LPRDOValue RDOType__INT::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    ASSERT(pFrom);

    LPRDOValue pToValue;
    try
    {
        LPTypeInfo pType = rdo::Factory<TypeInfo>::create(const_cast<RDOType__INT*>(this), pFrom->src_info());
        ASSERT(pType);
        pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getInt(), pFrom->src_info(), pType);
        ASSERT(pToValue);
    }
    catch (const rdo::runtime::RDOValueException&)
    {}

    if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::Type::UNKNOW)
    {
        parser::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к целому типу", pFrom->src_info().src_text().c_str()));
        parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
        parser::g_error().push_done();
    }
    return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__INT::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPIType& pType) const
{
    if (pType.object_dynamic_cast<RDOType>()->typeID() == rdo::runtime::RDOType::Type::REAL)
    {
        return rdo::Factory<rdo::runtime::RDOCalcDoubleToInt>::create(pCalc->srcInfo().src_pos(), pCalc);
    }
    return pCalc;
}

rdo::runtime::RDOValue RDOType__INT::get_default() const
{
    return rdo::runtime::RDOValue(0);
}

void RDOType__INT::writeModelStructure(std::ostream& stream) const
{
    stream << "I" << std::endl;
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__REAL
// --------------------------------------------------------------------------------
LPIType RDOType__REAL::type_cast(const LPIType& pFrom, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    ASSERT(pFrom);

    switch (pFrom.object_dynamic_cast<RDOType>()->typeID())
    {
        case rdo::runtime::RDOType::Type::INT :
        case rdo::runtime::RDOType::Type::REAL:
            return rdo::Factory<RDOType__REAL>::create();
        default:
            parser::g_error().push_only(src_info,    rdo::format("Ожидается вещественное значение, найдено: %s", from_src_info.src_text().c_str()));
            parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
            parser::g_error().push_done();
    }
    return NULL;
}

LPRDOValue RDOType__REAL::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    ASSERT(pFrom);

    LPRDOValue pToValue;
    try
    {
        LPTypeInfo pType = rdo::Factory<TypeInfo>::create(const_cast<RDOType__REAL*>(this), pFrom->src_info());
        ASSERT(pType);
        pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getDouble(), pFrom->src_info(), pType);
        ASSERT(pToValue);
    }
    catch (const rdo::runtime::RDOValueException&)
    {}

    if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::Type::UNKNOW)
    {
        parser::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к вещественному типу", pFrom->src_info().src_text().c_str()));
        parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
        parser::g_error().push_done();
    }
    return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__REAL::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPIType& /*pType*/) const
{
    return pCalc;
}

rdo::runtime::RDOValue RDOType__REAL::get_default() const
{
    return rdo::runtime::RDOValue(0.0);
}

void RDOType__REAL::writeModelStructure(std::ostream& stream) const
{
    stream << "R" << std::endl;
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__STRING
// --------------------------------------------------------------------------------
LPIType RDOType__STRING::type_cast(const LPIType& pFrom, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    ASSERT(pFrom);

    switch (pFrom.object_dynamic_cast<RDOType>()->typeID())
    {
        case rdo::runtime::RDOType::Type::STRING:
            return rdo::Factory<RDOType__STRING>::create();
        default:
            parser::g_error().push_only(src_info,    rdo::format("Ожидается строковое значение, найдено: %s", from_src_info.src_text().c_str()));
            parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
            parser::g_error().push_done();
    }
    return NULL;
}

LPRDOValue RDOType__STRING::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    ASSERT(pFrom);

    LPRDOValue pToValue;
    try
    {
        LPTypeInfo pType = rdo::Factory<TypeInfo>::create(const_cast<RDOType__STRING*>(this), pFrom->src_info());
        ASSERT(pType);
        pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getString(), pFrom->src_info(), pType);
        ASSERT(pToValue);
    }
    catch (const rdo::runtime::RDOValueException&)
    {}

    if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::Type::UNKNOW)
    {
        parser::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к строковому типу", pFrom->src_info().src_text().c_str()));
        parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
        parser::g_error().push_done();
    }
    return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__STRING::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPIType& /*pType*/) const
{
    return pCalc;
}

rdo::runtime::RDOValue RDOType__STRING::get_default() const
{
    return rdo::runtime::RDOValue("");
}

void RDOType__STRING::writeModelStructure(std::ostream& stream) const
{
    stream << "S" << std::endl;
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__IDENTIFICATOR
// --------------------------------------------------------------------------------
LPIType RDOType__IDENTIFICATOR::type_cast(const LPIType& pFrom, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& /*to_src_info*/, const RDOParserSrcInfo& src_info) const
{
    switch (pFrom.object_dynamic_cast<RDOType>()->typeID())
    {
    case rdo::runtime::RDOType::Type::IDENTIFICATOR:
        return rdo::Factory<RDOType__IDENTIFICATOR>::create();
    default:
        parser::g_error().error(src_info, rdo::format("Внутренная ошибка парсера. Невозможно преобразовать тип '%s' к идентификатору", from_src_info.src_text().c_str()));
    }
    return NULL;
}

LPRDOValue RDOType__IDENTIFICATOR::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    ASSERT(pFrom);

    switch (pFrom->typeID())
    {
    case rdo::runtime::RDOType::Type::IDENTIFICATOR:
        return rdo::Factory<RDOValue>::create(pFrom);
    default:
        parser::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к типу идентификатор", pFrom->src_info().src_text().c_str()));
        parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
        parser::g_error().push_done();
    }
    return LPRDOValue(NULL);
}

rdo::runtime::LPRDOCalc RDOType__IDENTIFICATOR::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPIType& /*pType*/) const
{
    return pCalc;
}

rdo::runtime::RDOValue RDOType__IDENTIFICATOR::get_default() const
{
    return rdo::runtime::RDOValue("", rdo::runtime::g_IDENTIFICATOR);
}

void RDOType__IDENTIFICATOR::writeModelStructure(std::ostream& /*stream*/) const
{
    parser::g_error().error(RDOParserSrcInfo(), "Внутренная ошибка парсера. Невозможно записать тип идектификатор в отчет");
    NEVER_REACH_HERE;
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__BOOL
// --------------------------------------------------------------------------------
LPIType RDOType__BOOL::type_cast(const LPIType& pFrom, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    ASSERT(pFrom);

    switch (pFrom.object_dynamic_cast<RDOType>()->typeID())
    {
        case rdo::runtime::RDOType::Type::BOOL:
            return rdo::Factory<RDOType__BOOL>::create();
        default:
            parser::g_error().push_only(src_info,    rdo::format("Ожидается булевское значение, найдено: %s", from_src_info.src_text().c_str()));
            parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
            parser::g_error().push_done();
    }
    return NULL;
}

LPRDOValue RDOType__BOOL::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    ASSERT(pFrom);

    LPRDOValue pToValue;
    try
    {
        LPTypeInfo pType = rdo::Factory<TypeInfo>::create(const_cast<RDOType__BOOL*>(this), pFrom->src_info());
        ASSERT(pType);
        pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getBool(), pFrom->src_info(), pType);
        ASSERT(pToValue);
    }
    catch (const rdo::runtime::RDOValueException&)
    {}

    if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::Type::UNKNOW)
    {
        parser::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к булевскому типу", pFrom->src_info().src_text().c_str()));
        parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
        parser::g_error().push_done();
    }
    return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__BOOL::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPIType& /*pType*/) const
{
    return pCalc;
}

rdo::runtime::RDOValue RDOType__BOOL::get_default() const
{
    return rdo::runtime::RDOValue(false);
}

void RDOType__BOOL::writeModelStructure(std::ostream& stream) const
{
    stream << "B" << std::endl;
}

CLOSE_RDO_PARSER_NAMESPACE

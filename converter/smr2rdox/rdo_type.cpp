// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_type.h"
#include "converter/smr2rdox/rdo_value.h"
#include "converter/smr2rdox/rdoparser_error.h"
#include "converter/smr2rdox/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ATOM_TYPE_CONVERTER
// --------------------------------------------------------------------------------
#define DECLARE_ATOM_TYPE_PARSER(Type, TypeName) \
std::string RDOType__##Type::name() const        \
{                                                \
    return TypeName;                             \
}

DECLARE_ATOM_TYPE_PARSER(UNKNOW,        "unknow"       );
DECLARE_ATOM_TYPE_PARSER(IDENTIFICATOR, "identificator");
DECLARE_ATOM_TYPE_PARSER(INT,           "integer"      );
DECLARE_ATOM_TYPE_PARSER(REAL,          "real"         );
DECLARE_ATOM_TYPE_PARSER(BOOL,          "bool"         );
DECLARE_ATOM_TYPE_PARSER(STRING,        "string"       );

// --------------------------------------------------------------------------------
// -------------------- RDOType
// --------------------------------------------------------------------------------

// RDOType__UNKNOW
LPRDOType RDOType__UNKNOW::type_cast(const LPRDOType& /*pFrom*/, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& /*to_src_info*/, const RDOParserSrcInfo& src_info) const
{
    rdo::converter::smr2rdox::g_error().error(src_info, rdo::format("Внутренняя ошибка парсера. Невозможно преобразовать тип '%s' к неизвестному типу", from_src_info.src_text().c_str()));
    return NULL;
}

LPRDOValue RDOType__UNKNOW::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к неизвестному типу", pFrom->src_info().src_text().c_str()));
    rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
    rdo::converter::smr2rdox::g_error().push_done();
    return LPRDOValue(NULL);
}

rdo::runtime::LPRDOCalc RDOType__UNKNOW::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPRDOType& pType) const
{
    return parent_type::calc_cast(pCalc, pType);
}

LPRDOValue RDOType__UNKNOW::get_default() const
{
    return rdo::Factory<RDOValue>::create(rdo::runtime::RDOValue(), rdo::Factory<RDOType__UNKNOW>::create(), RDOParserSrcInfo());
}

void RDOType__UNKNOW::writeModelStructure(std::ostream& /*stream*/) const
{
    rdo::converter::smr2rdox::g_error().error(RDOParserSrcInfo(), "Внутренняя ошибка парсера. Невозможно записать неизвестный тип в отчет");
    NEVER_REACH_HERE;
}

// RDOType__INT
LPRDOType RDOType__INT::type_cast(const LPRDOType& pFrom, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    switch (pFrom->type()->typeID())
    {
        case rdo::runtime::RDOType::Type::INT :
            return rdo::Factory<RDOType__INT>::create();
        case rdo::runtime::RDOType::Type::REAL:
            rdo::converter::smr2rdox::g_error().warning(src_info, rdo::format("Преобразование '%s' в '%s', возможна потеря данных", pFrom->name().c_str(), name().c_str()));
            return rdo::Factory<RDOType__REAL>::create();
        default:
            rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format("Ожидается целочисленное значение, найдено: %s", from_src_info.src_text().c_str()));
            rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
            rdo::converter::smr2rdox::g_error().push_done();
    }
    return NULL;
}

LPRDOValue RDOType__INT::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    LPRDOValue pToValue;
    try
    {
        pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getInt(), rdo::Factory<RDOType__INT>::create(), pFrom->src_info());
    }
    catch (const rdo::runtime::RDOValueException&)
    {}

    if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::Type::UNKNOW)
    {
        rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к целому типу", pFrom->src_info().src_text().c_str()));
        rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
        rdo::converter::smr2rdox::g_error().push_done();
    }
    return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__INT::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPRDOType& pType) const
{
    if (pType->typeID() == rdo::runtime::RDOType::Type::REAL)
    {
        return rdo::Factory<rdo::runtime::RDOCalcDoubleToInt>::create(pCalc->srcInfo().src_pos(), pCalc);
    }
    return pCalc;
}

LPRDOValue RDOType__INT::get_default() const
{
    return rdo::Factory<RDOValue>::create(rdo::runtime::RDOValue(0), rdo::Factory<RDOType__INT>::create(), RDOParserSrcInfo());
}

void RDOType__INT::writeModelStructure(std::ostream& stream) const
{
    stream << "I" << std::endl;
}

// RDOType__REAL
LPRDOType RDOType__REAL::type_cast(const LPRDOType& pFrom, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    switch (pFrom->type()->typeID())
    {
        case rdo::runtime::RDOType::Type::INT :
        case rdo::runtime::RDOType::Type::REAL:
            return rdo::Factory<RDOType__REAL>::create();
        default:
            rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format("Ожидается вещественное значение, найдено: %s", from_src_info.src_text().c_str()));
            rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
            rdo::converter::smr2rdox::g_error().push_done();
    }
    return NULL;
}

LPRDOValue RDOType__REAL::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    LPRDOValue pToValue;
    try
    {
        pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getDouble(), rdo::Factory<RDOType__REAL>::create(), pFrom->src_info());
    }
    catch (const rdo::runtime::RDOValueException&)
    {}

    if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::Type::UNKNOW)
    {
        rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к вещественному типу", pFrom->src_info().src_text().c_str()));
        rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
        rdo::converter::smr2rdox::g_error().push_done();
    }
    return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__REAL::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPRDOType& pType) const
{
    return parent_type::calc_cast(pCalc, pType);
}

LPRDOValue RDOType__REAL::get_default() const
{
    return rdo::Factory<RDOValue>::create(rdo::runtime::RDOValue(0.0), rdo::Factory<RDOType__REAL>::create(), RDOParserSrcInfo());
}

void RDOType__REAL::writeModelStructure(std::ostream& stream) const
{
    stream << "R" << std::endl;
}

// RDOType__STRING
LPRDOType RDOType__STRING::type_cast(const LPRDOType& pFrom, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    switch (pFrom->type()->typeID())
    {
        case rdo::runtime::RDOType::Type::STRING:
            return rdo::Factory<RDOType__STRING>::create();
        default:
            rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format("Ожидается строковое значение, найдено: %s", from_src_info.src_text().c_str()));
            rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
            rdo::converter::smr2rdox::g_error().push_done();
    }
    return NULL;
}

LPRDOValue RDOType__STRING::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    LPRDOValue pToValue;
    try
    {
        pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getString(), rdo::Factory<RDOType__STRING>::create(), pFrom->src_info());
    }
    catch (const rdo::runtime::RDOValueException&)
    {}

    if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::Type::UNKNOW)
    {
        rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к строковому типу", pFrom->src_info().src_text().c_str()));
        rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
        rdo::converter::smr2rdox::g_error().push_done();
    }
    return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__STRING::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPRDOType& pType) const
{
    return parent_type::calc_cast(pCalc, pType);
}

LPRDOValue RDOType__STRING::get_default() const
{
    return rdo::Factory<RDOValue>::create(rdo::runtime::RDOValue(""), rdo::Factory<RDOType__STRING>::create(), RDOParserSrcInfo());
}

void RDOType__STRING::writeModelStructure(std::ostream& stream) const
{
    stream << "S" << std::endl;
}

// RDOType__IDENTIFICATOR
LPRDOType RDOType__IDENTIFICATOR::type_cast(const LPRDOType& /*pFrom*/, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& /*to_src_info*/, const RDOParserSrcInfo& src_info) const
{
    rdo::converter::smr2rdox::g_error().error(src_info, rdo::format("Внутренняя ошибка парсера. Невозможно преобразовать тип '%s' к идентификатору", from_src_info.src_text().c_str()));
    return NULL;
}

LPRDOValue RDOType__IDENTIFICATOR::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к типу идентификатор", pFrom->src_info().src_text().c_str()));
    rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
    rdo::converter::smr2rdox::g_error().push_done();
    return LPRDOValue(NULL);
}

rdo::runtime::LPRDOCalc RDOType__IDENTIFICATOR::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPRDOType& pType) const
{
    return parent_type::calc_cast(pCalc, pType);
}

LPRDOValue RDOType__IDENTIFICATOR::get_default() const
{
    return rdo::Factory<RDOValue>::create(rdo::runtime::RDOValue("", rdo::runtime::g_IDENTIFICATOR), rdo::Factory<RDOType__IDENTIFICATOR>::create(), RDOParserSrcInfo());
}

void RDOType__IDENTIFICATOR::writeModelStructure(std::ostream& /*stream*/) const
{
    rdo::converter::smr2rdox::g_error().error(RDOParserSrcInfo(), "Внутренняя ошибка парсера. Невозможно записать тип идектификатор в отчет");
    NEVER_REACH_HERE;
}

// RDOType__BOOL
LPRDOType RDOType__BOOL::type_cast(const LPRDOType& pFrom, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    switch (pFrom->type()->typeID())
    {
        case rdo::runtime::RDOType::Type::BOOL:
            return rdo::Factory<RDOType__BOOL>::create();
        default:
            rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format("Ожидается булевское значение, найдено: %s", from_src_info.src_text().c_str()));
            rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
            rdo::converter::smr2rdox::g_error().push_done();
    }
    return NULL;
}

LPRDOValue RDOType__BOOL::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    LPRDOValue pToValue;
    try
    {
        pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getBool(), rdo::Factory<RDOType__BOOL>::create(), pFrom->src_info());
    }
    catch (const rdo::runtime::RDOValueException&)
    {}

    if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::Type::UNKNOW)
    {
        rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к булевскому типу", pFrom->src_info().src_text().c_str()));
        rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
        rdo::converter::smr2rdox::g_error().push_done();
    }
    return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__BOOL::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPRDOType& pType) const
{
    return parent_type::calc_cast(pCalc, pType);
}

LPRDOValue RDOType__BOOL::get_default() const
{
    return rdo::Factory<RDOValue>::create(rdo::runtime::RDOValue(false), rdo::Factory<RDOType__BOOL>::create(), RDOParserSrcInfo());
}

void RDOType__BOOL::writeModelStructure(std::ostream& stream) const
{
    stream << "B" << std::endl;
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

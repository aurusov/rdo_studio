/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_type.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

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
#define DECLARE_ATOM_TYPE_PARSER(Type, TypeName)    \
tstring RDOType__##Type::name() const               \
{                                                   \
	return TypeName;                                \
}

DECLARE_ATOM_TYPE_PARSER(unknow,        "unknow"       );
DECLARE_ATOM_TYPE_PARSER(identificator, "identificator");
DECLARE_ATOM_TYPE_PARSER(int,           "integer"      );
DECLARE_ATOM_TYPE_PARSER(real,          "real"         );
DECLARE_ATOM_TYPE_PARSER(bool,          "bool"         );
DECLARE_ATOM_TYPE_PARSER(string,        "string"       );

// --------------------------------------------------------------------------------
// -------------------- RDOType
// --------------------------------------------------------------------------------

//! RDOType__unknow
LPRDOType RDOType__unknow::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	UNUSED(pFrom      );
	UNUSED(to_src_info);

	rdo::converter::smr2rdox::g_error().error(src_info, rdo::format("Внутренная ошибка парсера. Невозможно преобразовать тип '%s' к неизвестному типу", from_src_info.src_text().c_str()));
	return NULL;
}

LPRDOValue RDOType__unknow::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к неизвестному типу", pFrom->src_info().src_text().c_str()));
	rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
	rdo::converter::smr2rdox::g_error().push_done();
	return LPRDOValue(NULL);
}

rdo::runtime::LPRDOCalc RDOType__unknow::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

LPRDOValue RDOType__unknow::get_default() const
{
	return rdo::Factory<RDOValue>::create(rdo::runtime::RDOValue(), rdo::Factory<RDOType__unknow>::create(), RDOParserSrcInfo());
}

void RDOType__unknow::writeModelStructure(REF(std::ostream) stream) const
{
	UNUSED(stream);

	rdo::converter::smr2rdox::g_error().error(RDOParserSrcInfo(), "Внутренная ошибка парсера. Невозможно записать неизвестный тип в отчет");
	NEVER_REACH_HERE;
}

//! RDOType__int
LPRDOType RDOType__int::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (pFrom->type()->typeID())
	{
		case rdo::runtime::RDOType::t_int :
			return rdo::Factory<RDOType__int>::create();
		case rdo::runtime::RDOType::t_real:
			rdo::converter::smr2rdox::g_error().warning(src_info, rdo::format("Преобразование '%s' в '%s', возможна потеря данных", pFrom->name().c_str(), name().c_str()));
			return rdo::Factory<RDOType__real>::create();
		default:
			rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format("Ожидается целочисленное значение, найдено: %s", from_src_info.src_text().c_str()));
			rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
			rdo::converter::smr2rdox::g_error().push_done();
	}
	return NULL;
}

LPRDOValue RDOType__int::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	LPRDOValue pToValue;
	try
	{
		pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getInt(), rdo::Factory<RDOType__int>::create(), pFrom->src_info());
	}
	catch (CREF(rdo::runtime::RDOValueException))
	{}

	if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::t_unknow)
	{
		rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к целому типу", pFrom->src_info().src_text().c_str()));
		rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
		rdo::converter::smr2rdox::g_error().push_done();
	}
	return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__int::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	if (pType->typeID() == rdo::runtime::RDOType::t_real)
	{
		return rdo::Factory<rdo::runtime::RDOCalcDoubleToInt>::create(pCalc->srcInfo().src_pos(), pCalc);
	}
	return pCalc;
}

LPRDOValue RDOType__int::get_default() const
{
	return rdo::Factory<RDOValue>::create(rdo::runtime::RDOValue(0), rdo::Factory<RDOType__int>::create(), RDOParserSrcInfo());
}

void RDOType__int::writeModelStructure(REF(std::ostream) stream) const
{
	stream << "I" << std::endl;
}

//! RDOType__real
LPRDOType RDOType__real::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (pFrom->type()->typeID())
	{
		case rdo::runtime::RDOType::t_int :
		case rdo::runtime::RDOType::t_real:
			return rdo::Factory<RDOType__real>::create();
		default:
			rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format("Ожидается вещественное значение, найдено: %s", from_src_info.src_text().c_str()));
			rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
			rdo::converter::smr2rdox::g_error().push_done();
	}
	return NULL;
}

LPRDOValue RDOType__real::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	LPRDOValue pToValue;
	try
	{
		pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getDouble(), rdo::Factory<RDOType__real>::create(), pFrom->src_info());
	}
	catch (CREF(rdo::runtime::RDOValueException))
	{}

	if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::t_unknow)
	{
		rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к вещественному типу", pFrom->src_info().src_text().c_str()));
		rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
		rdo::converter::smr2rdox::g_error().push_done();
	}
	return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__real::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

LPRDOValue RDOType__real::get_default() const
{
	return rdo::Factory<RDOValue>::create(rdo::runtime::RDOValue(0.0), rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo());
}

void RDOType__real::writeModelStructure(REF(std::ostream) stream) const
{
	stream << "R" << std::endl;
}

//! RDOType__string
LPRDOType RDOType__string::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (pFrom->type()->typeID())
	{
		case rdo::runtime::RDOType::t_string:
			return rdo::Factory<RDOType__string>::create();
		default:
			rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format("Ожидается строковое значение, найдено: %s", from_src_info.src_text().c_str()));
			rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
			rdo::converter::smr2rdox::g_error().push_done();
	}
	return NULL;
}

LPRDOValue RDOType__string::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	LPRDOValue pToValue;
	try
	{
		pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getString(), rdo::Factory<RDOType__string>::create(), pFrom->src_info());
	}
	catch (CREF(rdo::runtime::RDOValueException))
	{}

	if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::t_unknow)
	{
		rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к строковому типу", pFrom->src_info().src_text().c_str()));
		rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
		rdo::converter::smr2rdox::g_error().push_done();
	}
	return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__string::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

LPRDOValue RDOType__string::get_default() const
{
	return rdo::Factory<RDOValue>::create(rdo::runtime::RDOValue(""), rdo::Factory<RDOType__string>::create(), RDOParserSrcInfo());
}

void RDOType__string::writeModelStructure(REF(std::ostream) stream) const
{
	stream << "S" << std::endl;
}

//! RDOType__identificator
LPRDOType RDOType__identificator::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	UNUSED(pFrom       );
	UNUSED(to_src_info);

	rdo::converter::smr2rdox::g_error().error(src_info, rdo::format("Внутренная ошибка парсера. Невозможно преобразовать тип '%s' к идентификатору", from_src_info.src_text().c_str()));
	return NULL;
}

LPRDOValue RDOType__identificator::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к типу идентификатор", pFrom->src_info().src_text().c_str()));
	rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
	rdo::converter::smr2rdox::g_error().push_done();
	return LPRDOValue(NULL);
}

rdo::runtime::LPRDOCalc RDOType__identificator::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

LPRDOValue RDOType__identificator::get_default() const
{
	return rdo::Factory<RDOValue>::create(rdo::runtime::RDOValue("", rdo::runtime::g_identificator), rdo::Factory<RDOType__identificator>::create(), RDOParserSrcInfo());
}

void RDOType__identificator::writeModelStructure(REF(std::ostream) stream) const
{
	UNUSED(stream);

	rdo::converter::smr2rdox::g_error().error(RDOParserSrcInfo(), "Внутренная ошибка парсера. Невозможно записать тип идектификатор в отчет");
	NEVER_REACH_HERE;
}

//! RDOType__bool
LPRDOType RDOType__bool::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (pFrom->type()->typeID())
	{
		case rdo::runtime::RDOType::t_bool:
			return rdo::Factory<RDOType__bool>::create();
		default:
			rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format("Ожидается булевское значение, найдено: %s", from_src_info.src_text().c_str()));
			rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
			rdo::converter::smr2rdox::g_error().push_done();
	}
	return NULL;
}

LPRDOValue RDOType__bool::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	LPRDOValue pToValue;
	try
	{
		pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getBool(), rdo::Factory<RDOType__bool>::create(), pFrom->src_info());
	}
	catch (CREF(rdo::runtime::RDOValueException))
	{}

	if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::t_unknow)
	{
		rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к булевскому типу", pFrom->src_info().src_text().c_str()));
		rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
		rdo::converter::smr2rdox::g_error().push_done();
	}
	return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__bool::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

LPRDOValue RDOType__bool::get_default() const
{
	return rdo::Factory<RDOValue>::create(rdo::runtime::RDOValue(false), rdo::Factory<RDOType__bool>::create(), RDOParserSrcInfo());
}

void RDOType__bool::writeModelStructure(REF(std::ostream) stream) const
{
	stream << "B" << std::endl;
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

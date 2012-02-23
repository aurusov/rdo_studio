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

OPEN_RDO_CONVERTER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ATOM_TYPE_CONVERTER
// --------------------------------------------------------------------------------
#define DECLARE_ATOM_TYPE_PARSER(Type, TypeName)    \
tstring RDOType__##Type::name() const               \
{                                                   \
	return TypeName;                                \
}

DECLARE_ATOM_TYPE_PARSER(unknow,        _T("unknow")       );
DECLARE_ATOM_TYPE_PARSER(identificator, _T("identificator"));
DECLARE_ATOM_TYPE_PARSER(int,           _T("integer")      );
DECLARE_ATOM_TYPE_PARSER(real,          _T("real")         );
DECLARE_ATOM_TYPE_PARSER(bool,          _T("bool")         );
DECLARE_ATOM_TYPE_PARSER(string,        _T("string")       );

// --------------------------------------------------------------------------------
// -------------------- RDOType
// --------------------------------------------------------------------------------

//! RDOType__unknow
LPRDOType RDOType__unknow::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	UNUSED(from       );
	UNUSED(to_src_info);

	rdoConverter::g_error().error(src_info, rdo::format(_T("Внутренная ошибка парсера. Невозможно преобразовать тип '%s' к неизвестному типу"), from_src_info.src_text().c_str()));
	return NULL;
}

RDOValue RDOType__unknow::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	rdoConverter::g_error().push_only(src_info,    rdo::format(_T("Невозможно преобразовать значение '%s' к неизвестному типу"), from.src_info().src_text().c_str()));
	rdoConverter::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
	rdoConverter::g_error().push_done();
	return RDOValue();
}

rdoRuntime::LPRDOCalc RDOType__unknow::calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

RDOValue RDOType__unknow::get_default() const
{
	return RDOValue(rdoRuntime::RDOValue(), rdo::Factory<RDOType__unknow>::create(), RDOParserSrcInfo());
}

void RDOType__unknow::writeModelStructure(REF(std::ostream) stream) const
{
	UNUSED(stream);

	rdoConverter::g_error().error(RDOParserSrcInfo(), _T("Внутренная ошибка парсера. Невозможно записать неизвестный тип в отчет"));
	NEVER_REACH_HERE;
}

//! RDOType__int
LPRDOType RDOType__int::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from->type()->typeID())
	{
		case rdoRuntime::RDOType::t_int :
			return rdo::Factory<RDOType__int>::create();
		case rdoRuntime::RDOType::t_real:
			rdoConverter::g_error().warning(src_info, rdo::format(_T("Преобразование '%s' в '%s', возможна потеря данных"), from->name().c_str(), name().c_str()));
			return rdo::Factory<RDOType__real>::create();
		default:
			rdoConverter::g_error().push_only(src_info,    rdo::format(_T("Ожидается целочисленное значение, найдено: %s"), from_src_info.src_text().c_str()));
			rdoConverter::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
			rdoConverter::g_error().push_done();
	}
	return NULL;
}

RDOValue RDOType__int::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue;
	try
	{
		toValue = RDOValue(from->getInt(), rdo::Factory<RDOType__int>::create(), from.src_info());
	}
	catch (CREF(rdoRuntime::RDOValueException))
	{}

	if (toValue.typeID() == rdoRuntime::RDOType::t_unknow)
	{
		rdoConverter::g_error().push_only(src_info,    rdo::format(_T("Невозможно преобразовать значение '%s' к целому типу"), from.src_info().src_text().c_str()));
		rdoConverter::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
		rdoConverter::g_error().push_done();
	}
	return toValue;
}

rdoRuntime::LPRDOCalc RDOType__int::calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	if (pType->typeID() == rdoRuntime::RDOType::t_real)
	{
		return rdo::Factory<rdoRuntime::RDOCalcDoubleToInt>::create(pCalc->srcInfo().src_pos(), pCalc);
	}
	return pCalc;
}

RDOValue RDOType__int::get_default() const
{
	return RDOValue(rdoRuntime::RDOValue(0), rdo::Factory<RDOType__int>::create(), RDOParserSrcInfo());
}

void RDOType__int::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("I") << std::endl;
}

//! RDOType__real
LPRDOType RDOType__real::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from->type()->typeID())
	{
		case rdoRuntime::RDOType::t_int :
		case rdoRuntime::RDOType::t_real:
			return rdo::Factory<RDOType__real>::create();
		default:
			rdoConverter::g_error().push_only(src_info,    rdo::format(_T("Ожидается вещественное значение, найдено: %s"), from_src_info.src_text().c_str()));
			rdoConverter::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
			rdoConverter::g_error().push_done();
	}
	return NULL;
}

RDOValue RDOType__real::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue;
	try
	{
		toValue = RDOValue(from->getDouble(), rdo::Factory<RDOType__real>::create(), from.src_info());
	}
	catch (CREF(rdoRuntime::RDOValueException))
	{}

	if (toValue.typeID() == rdoRuntime::RDOType::t_unknow)
	{
		rdoConverter::g_error().push_only(src_info,    rdo::format(_T("Невозможно преобразовать значение '%s' к вещественному типу"), from.src_info().src_text().c_str()));
		rdoConverter::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
		rdoConverter::g_error().push_done();
	}
	return toValue;
}

rdoRuntime::LPRDOCalc RDOType__real::calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

RDOValue RDOType__real::get_default() const
{
	return RDOValue(rdoRuntime::RDOValue(0.0), rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo());
}

void RDOType__real::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("R") << std::endl;
}

//! RDOType__string
LPRDOType RDOType__string::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from->type()->typeID())
	{
		case rdoRuntime::RDOType::t_string:
			return rdo::Factory<RDOType__string>::create();
		default:
			rdoConverter::g_error().push_only(src_info,    rdo::format(_T("Ожидается строковое значение, найдено: %s"), from_src_info.src_text().c_str()));
			rdoConverter::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
			rdoConverter::g_error().push_done();
	}
	return NULL;
}

RDOValue RDOType__string::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue;
	try
	{
		toValue = RDOValue(from->getString(), rdo::Factory<RDOType__string>::create(), from.src_info());
	}
	catch (CREF(rdoRuntime::RDOValueException))
	{}

	if (toValue.typeID() == rdoRuntime::RDOType::t_unknow)
	{
		rdoConverter::g_error().push_only(src_info,    rdo::format(_T("Невозможно преобразовать значение '%s' к строковому типу"), from.src_info().src_text().c_str()));
		rdoConverter::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
		rdoConverter::g_error().push_done();
	}
	return toValue;
}

rdoRuntime::LPRDOCalc RDOType__string::calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

RDOValue RDOType__string::get_default() const
{
	return RDOValue(rdoRuntime::RDOValue(_T("")), rdo::Factory<RDOType__string>::create(), RDOParserSrcInfo());
}

void RDOType__string::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("S") << std::endl;
}

//! RDOType__identificator
LPRDOType RDOType__identificator::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	UNUSED(from       );
	UNUSED(to_src_info);

	rdoConverter::g_error().error(src_info, rdo::format(_T("Внутренная ошибка парсера. Невозможно преобразовать тип '%s' к идентификатору"), from_src_info.src_text().c_str()));
	return NULL;
}

RDOValue RDOType__identificator::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	rdoConverter::g_error().push_only(src_info,    rdo::format(_T("Невозможно преобразовать значение '%s' к типу идентификатор"), from.src_info().src_text().c_str()));
	rdoConverter::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
	rdoConverter::g_error().push_done();
	return RDOValue();
}

rdoRuntime::LPRDOCalc RDOType__identificator::calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

RDOValue RDOType__identificator::get_default() const
{
	return RDOValue(rdoRuntime::RDOValue(_T(""), rdoRuntime::g_identificator), rdo::Factory<RDOType__identificator>::create(), RDOParserSrcInfo());
}

void RDOType__identificator::writeModelStructure(REF(std::ostream) stream) const
{
	UNUSED(stream);

	rdoConverter::g_error().error(RDOParserSrcInfo(), _T("Внутренная ошибка парсера. Невозможно записать тип идектификатор в отчет"));
	NEVER_REACH_HERE;
}

//! RDOType__bool
LPRDOType RDOType__bool::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from->type()->typeID())
	{
		case rdoRuntime::RDOType::t_bool:
			return rdo::Factory<RDOType__bool>::create();
		default:
			rdoConverter::g_error().push_only(src_info,    rdo::format(_T("Ожидается булевское значение, найдено: %s"), from_src_info.src_text().c_str()));
			rdoConverter::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
			rdoConverter::g_error().push_done();
	}
	return NULL;
}

RDOValue RDOType__bool::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue;
	try
	{
		toValue = RDOValue(from->getBool(), rdo::Factory<RDOType__bool>::create(), from.src_info());
	}
	catch (CREF(rdoRuntime::RDOValueException))
	{}

	if (toValue.typeID() == rdoRuntime::RDOType::t_unknow)
	{
		rdoConverter::g_error().push_only(src_info,    rdo::format(_T("Невозможно преобразовать значение '%s' к булевскому типу"), from.src_info().src_text().c_str()));
		rdoConverter::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
		rdoConverter::g_error().push_done();
	}
	return toValue;
}

rdoRuntime::LPRDOCalc RDOType__bool::calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

RDOValue RDOType__bool::get_default() const
{
	return RDOValue(rdoRuntime::RDOValue(false), rdo::Factory<RDOType__bool>::create(), RDOParserSrcInfo());
}

void RDOType__bool::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("B") << std::endl;
}

CLOSE_RDO_CONVERTER_NAMESPACE

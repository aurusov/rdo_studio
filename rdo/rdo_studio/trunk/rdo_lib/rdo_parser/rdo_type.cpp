/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type.cpp
 * author   : Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_type.h"
#include "rdo_lib/rdo_parser/rdo_type_range.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
//#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_runtime/rdo_exception.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- ATOM_TYPE_PARSER
// ----------------------------------------------------------------------------
#define DECLARE_ATOM_TYPE_PARSER(Type)              \
RDOType__##Type g_##Type##_type;                    \
rdo::smart_ptr<RDOType> g_##Type(&g_##Type##_type);

DECLARE_ATOM_TYPE_PARSER(unknow       );
DECLARE_ATOM_TYPE_PARSER(identificator);
DECLARE_ATOM_TYPE_PARSER(int          );
DECLARE_ATOM_TYPE_PARSER(real         );
DECLARE_ATOM_TYPE_PARSER(bool         );
DECLARE_ATOM_TYPE_PARSER(string       );

// ----------------------------------------------------------------------------
// ---------- RDOType
// ----------------------------------------------------------------------------
LPRDOType RDOType::type_cast_throw(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	LPRDOType toType = type_cast(from);
	if (!toType)
	{
		rdoParse::g_error().push_only(src_info, rdo::format(_T("Несовместимые типы данных: %s и %s"), name().c_str(), from->name().c_str()));
		rdoParse::g_error().push_only(to_src_info,   _T("См. первый тип"));
		rdoParse::g_error().push_only(from_src_info, _T("См. второй тип"));
		rdoParse::g_error().push_done();
	}
	return toType;
}

LPRDOType RDOType::getTypeByID(rdoRuntime::RDOType::TypeID typeID)
{
	switch (typeID)
	{
		case rdoRuntime::RDOType::t_unknow       : return g_unknow;
		case rdoRuntime::RDOType::t_int          : return g_int;
		case rdoRuntime::RDOType::t_real         : return g_real;
		case rdoRuntime::RDOType::t_bool         : return g_bool;
		case rdoRuntime::RDOType::t_string       : return g_string;
		case rdoRuntime::RDOType::t_identificator: return g_identificator;
	}
	throw rdoRuntime::RDOTypeException();
}

//! RDOType__unknow
LPRDOType RDOType__unknow::type_cast(CREF(LPRDOType) from) const
{
	return NULL;
}

RDOValue RDOType__unknow::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	rdoParse::g_error().push_only(src_info,    rdo::format(_T("Невозможно преобразовать значение '%s' к неизвестному типу"), from.src_info().src_text().c_str()));
	rdoParse::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
	rdoParse::g_error().push_done();
	return RDOValue();
}

void RDOType__unknow::writeModelStructure(REF(std::ostream) stream) const
{
	NEVER_REACH_HERE;
}

//! RDOType__int
LPRDOType RDOType__int::type_cast(CREF(LPRDOType) from) const
{
	switch (from->type().typeID())
	{
		case rdoRuntime::RDOType::t_int : return g_int;
		case rdoRuntime::RDOType::t_real: return g_real;
	}
	return NULL;
}

RDOValue RDOType__int::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue;
	try
	{
		toValue = RDOValue(from->getInt(), from.src_info());
	}
	catch (CREF(rdoRuntime::RDOValueException))
	{}

	if (toValue.typeID() == rdoRuntime::RDOType::t_unknow)
	{
		rdoParse::g_error().push_only(src_info,    rdo::format(_T("Невозможно преобразовать значение '%s' к целому типу"), from.src_info().src_text().c_str()));
		rdoParse::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
		rdoParse::g_error().push_done();
	}
	return toValue;
}

void RDOType__int::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("I") << std::endl;
}

//! RDOType__real
LPRDOType RDOType__real::type_cast(CREF(LPRDOType) from) const
{
	switch (from->type().typeID())
	{
		case rdoRuntime::RDOType::t_int :
		case rdoRuntime::RDOType::t_real: return g_real;
	}
	return NULL;
}

RDOValue RDOType__real::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue;
	try
	{
		toValue = RDOValue(from->getDouble(), from.src_info());
	}
	catch (CREF(rdoRuntime::RDOValueException))
	{}

	if (toValue.typeID() == rdoRuntime::RDOType::t_unknow)
	{
		rdoParse::g_error().push_only(src_info,    rdo::format(_T("Невозможно преобразовать значение '%s' к вещественному типу"), from.src_info().src_text().c_str()));
		rdoParse::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
		rdoParse::g_error().push_done();
	}
	return toValue;
}

void RDOType__real::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("R") << std::endl;
}

//! RDOType__string
LPRDOType RDOType__string::type_cast(CREF(LPRDOType) from) const
{
	switch (from->type().typeID())
	{
		case rdoRuntime::RDOType::t_string: return g_string;
	}
	return NULL;
}

RDOValue RDOType__string::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue;
	try
	{
		toValue = RDOValue(from->getString(), from.src_info());
	}
	catch (CREF(rdoRuntime::RDOValueException))
	{}

	if (toValue.typeID() == rdoRuntime::RDOType::t_unknow)
	{
		rdoParse::g_error().push_only(src_info,    rdo::format(_T("Невозможно преобразовать значение '%s' к строковому типу"), from.src_info().src_text().c_str()));
		rdoParse::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
		rdoParse::g_error().push_done();
	}
	return toValue;
}

void RDOType__string::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("S") << std::endl;
}

//! RDOType__identificator
LPRDOType RDOType__identificator::type_cast(CREF(LPRDOType) from) const
{
	return NULL;
}

RDOValue RDOType__identificator::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	rdoParse::g_error().push_only(src_info,    rdo::format(_T("Невозможно преобразовать значение '%s' к типу идентификатор"), from.src_info().src_text().c_str()));
	rdoParse::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
	rdoParse::g_error().push_done();
	return RDOValue();
}

void RDOType__identificator::writeModelStructure(REF(std::ostream) stream) const
{
	NEVER_REACH_HERE;
}

//! RDOType__bool
LPRDOType RDOType__bool::type_cast(CREF(LPRDOType) from) const
{
	switch (from->type().typeID())
	{
		case rdoRuntime::RDOType::t_bool: return g_bool;
	}
	return NULL;
}

RDOValue RDOType__bool::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue;
	try
	{
		toValue = RDOValue(from->getBool(), from.src_info());
	}
	catch (CREF(rdoRuntime::RDOValueException))
	{}

	if (toValue.typeID() == rdoRuntime::RDOType::t_unknow)
	{
		rdoParse::g_error().push_only(src_info,    rdo::format(_T("Невозможно преобразовать значение '%s' к строковому типу"), from.src_info().src_text().c_str()));
		rdoParse::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
		rdoParse::g_error().push_done();
	}
	return toValue;
}

void RDOType__bool::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("B") << std::endl;
}

CLOSE_RDO_PARSER_NAMESPACE

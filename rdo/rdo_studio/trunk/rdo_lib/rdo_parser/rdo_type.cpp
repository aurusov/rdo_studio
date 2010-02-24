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
//#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_runtime/rdo_exception.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOType
// ----------------------------------------------------------------------------
CPTR(RDOType) RDOType::type_cast_throw(CREF(RDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	CPTR(RDOType) toType = type_cast(from);
	if (!toType)
	{
		rdoParse::g_error().push_only(src_info, rdo::format(_T("Несовместимые типы данных: %s и %s"), name().c_str(), from.name().c_str()));
		rdoParse::g_error().push_only(to_src_info,   _T("См. первый тип"));
		rdoParse::g_error().push_only(from_src_info, _T("См. второй тип"));
		rdoParse::g_error().push_done();
	}
	return toType;
}

CREF(RDOType) RDOType::getTypeByID(rdoRuntime::RDOType::TypeID typeID)
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
CPTR(RDOType) RDOType__unknow::type_cast(CREF(RDOType) from) const
{
	return NULL;
}

rdoRuntime::RDOValue RDOType__unknow::value_cast(CREF(rdoRuntime::RDOValue) from) const
{
	throw rdoRuntime::RDOTypeException();
}

void RDOType__unknow::writeModelStructure(REF(std::ostream) stream) const
{
	NEVER_REACH_HERE;
}

//! RDOType__int
CPTR(RDOType) RDOType__int::type_cast(CREF(RDOType) from) const
{
	switch (from->typeID())
	{
		case rdoRuntime::RDOType::t_int : return &g_int;
		case rdoRuntime::RDOType::t_real: return &g_real;
	}
	return NULL;
}

rdoRuntime::RDOValue RDOType__int::value_cast(CREF(rdoRuntime::RDOValue) from) const
{
	return from.getInt();
}

void RDOType__int::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("I") << std::endl;
}

//! RDOType__real
CPTR(RDOType) RDOType__real::type_cast(CREF(RDOType) from) const
{
	switch (from->typeID())
	{
		case rdoRuntime::RDOType::t_int :
		case rdoRuntime::RDOType::t_real: return &g_real;
	}
	return NULL;
}

rdoRuntime::RDOValue RDOType__real::value_cast(CREF(rdoRuntime::RDOValue) from) const
{
	return from.getDouble();
}

void RDOType__real::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("R") << std::endl;
}

//! RDOType__string
CPTR(RDOType) RDOType__string::type_cast(CREF(RDOType) from) const
{
	switch (from->typeID())
	{
		case rdoRuntime::RDOType::t_string: return &g_string;
	}
	return NULL;
}

rdoRuntime::RDOValue RDOType__string::value_cast(CREF(rdoRuntime::RDOValue) from) const
{
	return from.getString();
}

void RDOType__string::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("S") << std::endl;
}

//! RDOType__identificator
CPTR(RDOType) RDOType__identificator::type_cast(CREF(RDOType) from) const
{
	return NULL;
}

rdoRuntime::RDOValue RDOType__identificator::value_cast(CREF(rdoRuntime::RDOValue) from) const
{
	throw rdoRuntime::RDOTypeException();
}

void RDOType__identificator::writeModelStructure(REF(std::ostream) stream) const
{
	NEVER_REACH_HERE;
}

//! RDOType__bool
CPTR(RDOType) RDOType__bool::type_cast(CREF(RDOType) from) const
{
	switch (from->typeID())
	{
		case rdoRuntime::RDOType::t_bool: return &g_bool;
	}
	return NULL;
}

rdoRuntime::RDOValue RDOType__bool::value_cast(CREF(rdoRuntime::RDOValue) from) const
{
	return from.getBool();
}

void RDOType__bool::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("B") << std::endl;
}

// ----------------------------------------------------------------------------
// ---------- ATOM_TYPE_PARSER
// ----------------------------------------------------------------------------
#define DECLARE_ATOM_TYPE_PARSER(Type) RDOType__##Type g_##Type;

DECLARE_ATOM_TYPE_PARSER(unknow       );
DECLARE_ATOM_TYPE_PARSER(identificator);
DECLARE_ATOM_TYPE_PARSER(int          );
DECLARE_ATOM_TYPE_PARSER(real         );
DECLARE_ATOM_TYPE_PARSER(bool         );
DECLARE_ATOM_TYPE_PARSER(string       );

CLOSE_RDO_PARSER_NAMESPACE

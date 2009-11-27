#include "pch.h"
#include "rdoparser.h"
#include "rdoparser_type.h"
#include <rdo_exception.h>

namespace rdoParse {

// ----------------------------------------------------------------------------
// ---------- RDOType
// ----------------------------------------------------------------------------
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
CPTR(RDOType) RDOType__unknow::cast(CREF(RDOType) toType) const
{
	return NULL;
}

rdoRuntime::RDOValue RDOType__unknow::cast(CREF(rdoRuntime::RDOValue) from) const
{
	throw rdoRuntime::RDOTypeException();
}

//! RDOType__int
CPTR(RDOType) RDOType__int::cast(CREF(RDOType) toType) const
{
	switch (toType->typeID())
	{
		case rdoRuntime::RDOType::t_int : return &g_int;
		case rdoRuntime::RDOType::t_real: return &g_real;
	}
	return NULL;
}

rdoRuntime::RDOValue RDOType__int::cast(CREF(rdoRuntime::RDOValue) from) const
{
	return from.getInt();
}

//! RDOType__real
CPTR(RDOType) RDOType__real::cast(CREF(RDOType) toType) const
{
	switch (toType->typeID())
	{
		case rdoRuntime::RDOType::t_int :
		case rdoRuntime::RDOType::t_real: return &g_real;
	}
	return NULL;
}

rdoRuntime::RDOValue RDOType__real::cast(CREF(rdoRuntime::RDOValue) from) const
{
	return from.getDouble();
}

//! RDOType__string
CPTR(RDOType) RDOType__string::cast(CREF(RDOType) toType) const
{
	switch (toType->typeID())
	{
		case rdoRuntime::RDOType::t_string: return &g_string;
	}
	return NULL;
}

rdoRuntime::RDOValue RDOType__string::cast(CREF(rdoRuntime::RDOValue) from) const
{
	return from.getString();
}

//! RDOType__identificator
CPTR(RDOType) RDOType__identificator::cast(CREF(RDOType) toType) const
{
	return NULL;
}

rdoRuntime::RDOValue RDOType__identificator::cast(CREF(rdoRuntime::RDOValue) from) const
{
	throw rdoRuntime::RDOTypeException();
}

//! RDOType__bool
CPTR(RDOType) RDOType__bool::cast(CREF(RDOType) toType) const
{
	switch (toType->typeID())
	{
		case rdoRuntime::RDOType::t_bool: return &g_bool;
	}
	return NULL;
}

rdoRuntime::RDOValue RDOType__bool::cast(CREF(rdoRuntime::RDOValue) from) const
{
	return from.getBool();
}

// ----------------------------------------------------------------------------
// ---------- ATOM_TYPE_PARSER
// ----------------------------------------------------------------------------
#define DECLARE_ATOM_TYPE_PARSER( Class ) RDOType__##Class g_##Class;

DECLARE_ATOM_TYPE_PARSER( unknow        );
DECLARE_ATOM_TYPE_PARSER( identificator );
DECLARE_ATOM_TYPE_PARSER( int           );
DECLARE_ATOM_TYPE_PARSER( real          );
DECLARE_ATOM_TYPE_PARSER( bool          );
DECLARE_ATOM_TYPE_PARSER( string        );

} // namespace rdoParse

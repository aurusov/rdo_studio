#include "pch.h"
#include "rdo_type.h"
#include "rdo_value.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOType
// ----------------------------------------------------------------------------
CREF(RDOType) RDOType::getTypeByID(RDOType::TypeID typeID)
{
	switch (typeID)
	{
		case t_unknow       : return g_unknow;
		case t_identificator: return g_identificator;
		case t_int          : return g_int;
		case t_real         : return g_real;
		case t_bool         : return g_bool;
		case t_string       : return g_string;
	}
	throw RDOTypeException();
}

//! RDOType__unknow
rdoRuntime::RDOValue RDOType__unknow::cast(CREF(rdoRuntime::RDOValue) from) const
{
	throw rdoRuntime::RDOTypeException();
}

//! RDOType__int
rdoRuntime::RDOValue RDOType__int::cast(CREF(rdoRuntime::RDOValue) from) const
{
	return from.getInt();
}

//! RDOType__real
rdoRuntime::RDOValue RDOType__real::cast(CREF(rdoRuntime::RDOValue) from) const
{
	return from.getDouble();
}

//! RDOType__string
rdoRuntime::RDOValue RDOType__string::cast(CREF(rdoRuntime::RDOValue) from) const
{
	return from.getString();
}

//! RDOType__identificator
rdoRuntime::RDOValue RDOType__identificator::cast(CREF(rdoRuntime::RDOValue) from) const
{
	throw rdoRuntime::RDOTypeException();
}

//! RDOType__bool
rdoRuntime::RDOValue RDOType__bool::cast(CREF(rdoRuntime::RDOValue) from) const
{
	return from.getBool();
}

// ----------------------------------------------------------------------------
// ---------- ATOM_TYPE
// ----------------------------------------------------------------------------
#define DECLARE_ATOM_TYPE(Class) RDOType__##Class g_##Class;

DECLARE_ATOM_TYPE(unknow       );
DECLARE_ATOM_TYPE(identificator);
DECLARE_ATOM_TYPE(int          );
DECLARE_ATOM_TYPE(real         );
DECLARE_ATOM_TYPE(bool         );
DECLARE_ATOM_TYPE(string       );

} // namespace rdoRuntime

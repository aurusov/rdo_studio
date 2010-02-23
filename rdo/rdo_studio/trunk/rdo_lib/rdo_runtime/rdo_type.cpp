/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type.cpp
 * author   : Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_type.h"
#include "rdo_lib/rdo_runtime/rdo_value.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

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

// ----------------------------------------------------------------------------
// ---------- ATOM_TYPE
// ----------------------------------------------------------------------------
#define DECLARE_ATOM_TYPE(Type) RDOType__##Type g_##Type;

DECLARE_ATOM_TYPE(unknow       );
DECLARE_ATOM_TYPE(identificator);
DECLARE_ATOM_TYPE(int          );
DECLARE_ATOM_TYPE(real         );
DECLARE_ATOM_TYPE(bool         );
DECLARE_ATOM_TYPE(string       );

CLOSE_RDO_RUNTIME_NAMESPACE

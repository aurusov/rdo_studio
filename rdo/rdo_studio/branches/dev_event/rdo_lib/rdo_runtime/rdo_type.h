/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type.h
 * author   : Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_TYPE_H_
#define _RDO_TYPE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdomacros.h"
#include "rdo_common/rdotypes.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

class RDOValue;

// ----------------------------------------------------------------------------
// ---------- RDOType
// ----------------------------------------------------------------------------
class RDOType
{
public:
	enum TypeID
	{
		t_unknow = 0,
		t_identificator,
		t_int,
		t_real,
		t_bool,
		t_string,
		t_enum,
		t_fuzzy
	};

	RDOType(TypeID typeID);

	TypeID  typeID() const;

private:
	TypeID  m_typeID;
};

// ----------------------------------------------------------------------------
// ---------- ATOM_TYPE
// ----------------------------------------------------------------------------
#define DEFINE_ATOM_TYPE(Type)        \
class RDOType__##Type: public RDOType \
{                                     \
public:                               \
	RDOType__##Type()                 \
		: RDOType(t_##Type)           \
	{}                                \
};                                    \
extern RDOType__##Type g_##Type;

DEFINE_ATOM_TYPE(unknow       );
DEFINE_ATOM_TYPE(identificator);
DEFINE_ATOM_TYPE(int          );
DEFINE_ATOM_TYPE(real         );
DEFINE_ATOM_TYPE(bool         );
DEFINE_ATOM_TYPE(string       );

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/rdo_type.inl"

#endif //! _RDO_TYPE_H_

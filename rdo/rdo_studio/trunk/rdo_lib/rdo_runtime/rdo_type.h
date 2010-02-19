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

	TypeID  typeID     () const;
	rbool   operator!= (CREF(RDOType) type);

	virtual tstring  asString  ()                    const = 0;
	virtual RDOValue value_cast(CREF(RDOValue) from) const = 0;

	static CREF(RDOType) getTypeByID(TypeID typeID);

private:
	TypeID  m_typeID;
};

// ----------------------------------------------------------------------------
// ---------- ATOM_TYPE
// ----------------------------------------------------------------------------
#define DEFINE_ATOM_TYPE(Class, ClassName)                                       \
class RDOType__##Class: public RDOType                                           \
{                                                                                \
public:                                                                          \
	RDOType__##Class()                                                           \
		: RDOType(t_##Class)                                                     \
	{}                                                                           \
	virtual tstring  asString  ()                    const { return ClassName; } \
	virtual RDOValue value_cast(CREF(RDOValue) from) const;                      \
};                                                                               \
extern RDOType__##Class g_##Class;

DEFINE_ATOM_TYPE(unknow,        _T("unknow")       );
DEFINE_ATOM_TYPE(identificator, _T("identificator"));
DEFINE_ATOM_TYPE(int,           _T("integer")      );
DEFINE_ATOM_TYPE(real,          _T("real")         );
DEFINE_ATOM_TYPE(bool,          _T("bool")         );
DEFINE_ATOM_TYPE(string,        _T("string")       );

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/rdo_type.inl"

#endif //! _RDO_TYPE_H_

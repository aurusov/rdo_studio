// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_type.h"
#include "utils/src/smart_ptr/factory/factory.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOType
// --------------------------------------------------------------------------------
RDOType::RDOType(TypeID typeID)
	: m_typeID(typeID)
{}

RDOType::~RDOType()
{}

RDOType::TypeID RDOType::typeID() const
{
	return m_typeID;
}

/// @todo внимание макрос!

#define DEFINE_ATOM_TYPE_DEFAULT_VALUE(Type) rdo::intrusive_ptr<RDOType__##Type> g_##Type = rdo::Factory<RDOType__##Type>::create();

DEFINE_ATOM_TYPE_DEFAULT_VALUE(unknow);

DEFINE_ATOM_TYPE_DEFAULT_VALUE(void);

DEFINE_ATOM_TYPE_DEFAULT_VALUE(identificator);

DEFINE_ATOM_TYPE_DEFAULT_VALUE(int);

DEFINE_ATOM_TYPE_DEFAULT_VALUE(real);

DEFINE_ATOM_TYPE_DEFAULT_VALUE(bool);

DEFINE_ATOM_TYPE_DEFAULT_VALUE(string);

CLOSE_RDO_RUNTIME_NAMESPACE

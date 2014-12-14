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
RDOType::RDOType(Type typeID)
    : m_typeID(typeID)
{}

RDOType::~RDOType()
{}

RDOType::Type RDOType::typeID() const
{
    return m_typeID;
}

// TODO внимание макрос!

#define DEFINE_ATOM_TYPE_DEFAULT_VALUE(Type) rdo::intrusive_ptr<RDOType__##Type> g_##Type = rdo::Factory<RDOType__##Type>::create();

DEFINE_ATOM_TYPE_DEFAULT_VALUE(UNKNOW);
DEFINE_ATOM_TYPE_DEFAULT_VALUE(EMPTY);
DEFINE_ATOM_TYPE_DEFAULT_VALUE(IDENTIFICATOR);
DEFINE_ATOM_TYPE_DEFAULT_VALUE(INT);
DEFINE_ATOM_TYPE_DEFAULT_VALUE(REAL);
DEFINE_ATOM_TYPE_DEFAULT_VALUE(BOOL);
DEFINE_ATOM_TYPE_DEFAULT_VALUE(STRING);

CLOSE_RDO_RUNTIME_NAMESPACE

#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOValue;

PREDECLARE_POINTER(RDOType);
class RDOType: public virtual rdo::counter_reference
{
public:
    enum class Type
    {
        UNKNOW = 0,
        EMPTY,
        IDENTIFICATOR,
        INT,
        REAL,
        BOOL,
        STRING,
        ENUM,
        POINTER
    };

    RDOType(Type typeID);
    virtual ~RDOType();

    Type typeID() const;

private:
    Type  m_typeID;
};

// TODO внимание макрос!
#define DEFINE_ATOM_TYPE(TypeName)        \
class RDOType__##TypeName: public RDOType \
{                                         \
public:                                   \
    RDOType__##TypeName()                 \
        : RDOType(Type::TypeName)         \
    {}                                    \
};                                        \
extern rdo::intrusive_ptr<RDOType__##TypeName> g_##TypeName;

DEFINE_ATOM_TYPE(UNKNOW);
DEFINE_ATOM_TYPE(EMPTY);
DEFINE_ATOM_TYPE(IDENTIFICATOR);
DEFINE_ATOM_TYPE(INT);
DEFINE_ATOM_TYPE(REAL);
DEFINE_ATOM_TYPE(BOOL);
DEFINE_ATOM_TYPE(STRING);

CLOSE_RDO_RUNTIME_NAMESPACE

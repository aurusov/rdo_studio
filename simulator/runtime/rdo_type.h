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
        enum TypeID
    {
        t_unknow = 0,
        t_void,
        t_identificator,
        t_int,
        t_real,
        t_bool,
        t_string,
        t_enum,
        t_pointer
    };

    RDOType(TypeID typeID);
    virtual ~RDOType();

    TypeID  typeID() const;

private:
    TypeID  m_typeID;
};

// TODO внимание макрос!
#define DEFINE_ATOM_TYPE(Type)        \
class RDOType__##Type: public RDOType \
{                                     \
public:                               \
    RDOType__##Type()                 \
        : RDOType(t_##Type)           \
    {}                                \
};                                    \
extern rdo::intrusive_ptr<RDOType__##Type> g_##Type;

DEFINE_ATOM_TYPE(unknow);

DEFINE_ATOM_TYPE(void);

DEFINE_ATOM_TYPE(identificator);

DEFINE_ATOM_TYPE(int);

DEFINE_ATOM_TYPE(real);

DEFINE_ATOM_TYPE(bool);

DEFINE_ATOM_TYPE(string);

CLOSE_RDO_RUNTIME_NAMESPACE

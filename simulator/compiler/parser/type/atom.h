#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/rdo_model_i.h"
#include "simulator/compiler/parser/type/type_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ATOM_TYPE_PARSER
// --------------------------------------------------------------------------------
#define DEFINE_ATOM_TYPE_PARSER(Type)      \
class RDOType__##Type                      \
    : public rdo::runtime::RDOType__##Type \
    , public IType                         \
    , public IModelStructure               \
{                                          \
public:                                    \
    DECLARE_IType;                         \
    DECLARE_IModelStructure;               \
};                                         \
DECLARE_POINTER(RDOType__##Type);

DEFINE_ATOM_TYPE_PARSER(UNKNOW);
DEFINE_ATOM_TYPE_PARSER(IDENTIFICATOR);
DEFINE_ATOM_TYPE_PARSER(INT);
DEFINE_ATOM_TYPE_PARSER(REAL);
DEFINE_ATOM_TYPE_PARSER(BOOL);
DEFINE_ATOM_TYPE_PARSER(STRING);
DEFINE_ATOM_TYPE_PARSER(EMPTY);

CLOSE_RDO_PARSER_NAMESPACE

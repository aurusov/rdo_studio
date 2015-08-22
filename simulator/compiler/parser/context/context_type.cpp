// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/context/context_type.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- TypeContext
// --------------------------------------------------------------------------------
TypeContext::TypeContext(const LPTypeInfo& pType)
    : m_pType(pType)
{}

const LPTypeInfo& TypeContext::getTypeInfo() const
{
    return m_pType;
}

CLOSE_RDO_PARSER_NAMESPACE

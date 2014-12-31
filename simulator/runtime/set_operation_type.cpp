// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/set_operation_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

std::string SetOperation::toString(Type type)
{
    switch (type)
    {
    case Type::NOCHANGE   : return std::string();
    case Type::SET        : return "=";
    case Type::ADDITION   : return "+=";
    case Type::SUBTRACTION: return "-=";
    case Type::MULTIPLY   : return "*=";
    case Type::DIVIDE     : return "/=";
    case Type::INCREMENT  : return "++";
    case Type::DECRIMENT  : return "--";
    default: NEVER_REACH_HERE; return std::string();
    }
}

CLOSE_RDO_RUNTIME_NAMESPACE

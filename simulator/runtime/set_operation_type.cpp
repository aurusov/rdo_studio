// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/set_operation_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

std::string SetOperationType::toString(Type type)
{
    switch (type)
    {
    case NOCHANGE   : return "";
    case SET        : return "=";
    case ADDITION   : return "+=";
    case SUBTRACTION: return "-=";
    case MULTIPLY   : return "*=";
    case DIVIDE     : return "/=";
    case INCREMENT  : return "++";
    case DECRIMENT  : return "--";
    default: NEVER_REACH_HERE; return "";
    }
}

CLOSE_RDO_RUNTIME_NAMESPACE

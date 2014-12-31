#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

enum class RunTimeMode
{
    MAX_SPEED,
    JUMP,
    SYNC,
    PAUSE,
    BREAKPOINT
};

CLOSE_RDO_RUNTIME_NAMESPACE

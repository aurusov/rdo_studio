#ifndef _LIB_RUNTIME_NAMESPACE_MODE_H_
#define _LIB_RUNTIME_NAMESPACE_MODE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

enum RunTimeMode
{
	RTM_MaxSpeed,
	RTM_Jump,
	RTM_Sync,
	RTM_Pause,
	RTM_BreakPoint
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_NAMESPACE_MODE_H_

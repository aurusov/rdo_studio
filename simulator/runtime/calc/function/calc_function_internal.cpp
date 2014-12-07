// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <math.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/function/calc_function_internal.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

double GetParam<double>::getParam(const LPRDORuntime& pRuntime, std::size_t paramNumber)
{
	return pRuntime->getFuncArgument(paramNumber).getDouble();
}

int GetParam<int>::getParam(const LPRDORuntime& pRuntime, std::size_t paramNumber)
{
	return pRuntime->getFuncArgument(paramNumber).getInt();
}


CLOSE_RDO_RUNTIME_NAMESPACE

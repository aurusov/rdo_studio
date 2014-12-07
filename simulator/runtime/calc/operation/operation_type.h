#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class OperatorType
{
public:
	enum Type
	{
		OT_ARITHM,
		OT_LOGIC
	};

	template <Type>
	static std::size_t& getCalcCounter();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/operation/operation_type-inl.h"

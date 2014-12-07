#ifndef _LIB_RUNTIME_CALC_OPERATION_TYPE_H_
#define _LIB_RUNTIME_CALC_OPERATION_TYPE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class OperatorType
{
public:
	//! Тип операции в РДО
	enum Type
	{
		OT_ARITHM, // Арифметическое выражение
		OT_LOGIC   // Логическое выражение
	};

	template <Type>
	static std::size_t& getCalcCounter();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/operation/operation_type-inl.h"

#endif // _LIB_RUNTIME_CALC_OPERATION_TYPE_H_

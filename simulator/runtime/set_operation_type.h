#ifndef _LIB_RUNTIME_SET_OPERATION_TYPE_H_
#define _LIB_RUNTIME_SET_OPERATION_TYPE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

struct SetOperationType
{
	enum Type
	{
		NOCHANGE,
		SET,
		ADDITION,
		SUBTRACTION,
		MULTIPLY,
		DIVIDE,
		INCREMENT,
		DECRIMENT
	};

	static std::string toString(Type type);
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_SET_OPERATION_TYPE_H_

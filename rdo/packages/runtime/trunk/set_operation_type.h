/*!
  \copyright (c) RDO-Team, 2011
  \file      set_operation_type.h
  \authors   Чирков Михаил
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      03.12.2010
  \brief     Типы операторов присваивания
  \indent    4T
*/

#ifndef _LIB_RUNTIME_SET_OPERATION_TYPE_H_
#define _LIB_RUNTIME_SET_OPERATION_TYPE_H_

// ----------------------------------------------------------------------- INCLUDES
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
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_SET_OPERATION_TYPE_H_

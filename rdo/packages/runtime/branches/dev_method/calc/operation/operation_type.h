/*!
  \copyright (c) RDO-Team, 2011
  \file      operation_type.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     Тип операции: арифметическое или логическое выражение
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_OPERATION_TYPE_H_
#define _LIB_RUNTIME_CALC_OPERATION_TYPE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdotypes.h"
#include "utils/src/common/rdomacros.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   OperatorType
  \brief   Тип выражения
*/
class OperatorType
{
public:
	//! Тип операции в РДО
	enum Type
	{
		OT_ARITHM, //!< Арифметическое выражение
		OT_LOGIC   //!< Логическое выражение
	};

	template <Type>
	static REF(ruint) getCalcCounter();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/operation/operation_type.inl"

#endif // _LIB_RUNTIME_CALC_OPERATION_TYPE_H_

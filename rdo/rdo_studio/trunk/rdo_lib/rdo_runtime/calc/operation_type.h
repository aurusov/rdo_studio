/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      operation_type.h
 * @authors   Урусов Андрей
 * @date      13.03.2011
 * @brief     unknown
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_CALC_OPERATION_TYPE_H_
#define _LIB_RUNTIME_CALC_OPERATION_TYPE_H_

// **************************************************************************** PCH
// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

/******************************************************************************//**
 * @class   OperatorType
 * @brief   unknown
 *********************************************************************************/
class OperatorType
{
public:
	/**
	 * @enum  Type
	 * @brief Тип операции в РДО
	 */
	enum Type
	{
		OT_ARITHM,
		OT_LOGIC
	};

	template <Type>
	static REF(ruint) getCalcCounter();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/calc/operation_type.inl"

#endif // _LIB_RUNTIME_CALC_OPERATION_TYPE_H_

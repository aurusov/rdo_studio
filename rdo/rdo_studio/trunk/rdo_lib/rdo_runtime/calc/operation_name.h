/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      operation_name.h
 * @authors   Урусов Андрей
 * @date      13.03.2011
 * @brief     unknown
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_CALC_OPERATION_NAME_H_
#define _LIB_RUNTIME_CALC_OPERATION_NAME_H_

// **************************************************************************** PCH
// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

/******************************************************************************//**
 * @class   OperatorName
 * @tparam  Тип операции
 * @brief   unknown
 *********************************************************************************/
template <typename opr_type>
class OperatorName
{
public:
	static tstring name(CREF(opr_type) pOperator);

private:
	/**
	 * @struct OprItem
	 */
	struct OprItem
	{
		opr_type m_pOperator;
		tstring  m_name;

		OprItem(CREF(opr_type) pOperator, CREF(tstring) name);

		rbool operator== (CREF(opr_type) pOperator) const;
	};
	typedef std::list<OprItem> NameList;

	static CREF(NameList) getList();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/calc/operation_name.inl"

#endif // _LIB_RUNTIME_CALC_OPERATION_NAME_H_

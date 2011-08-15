/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      operation_name.h
 * @author    Урусов Андрей
 * @date      13.03.2011
 * @brief     Название операции
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
 * @tparam  opr_type Тип операции
 * @brief   Имя операторов
 *********************************************************************************/
template <typename opr_type>
class OperatorName
{
public:
	static tstring name(CREF(opr_type) pOperator);

private:
	/**
	 * @struct OprItem
	 * @brief  Экземпляр оператора
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

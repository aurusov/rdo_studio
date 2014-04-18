/*!
  \copyright (c) RDO-Team, 2011
  \file      operation_name.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     Название операции
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_OPERATION_NAME_H_
#define _LIB_RUNTIME_CALC_OPERATION_NAME_H_

// ----------------------------------------------------------------------- INCLUDES
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   OperatorName
  \tparam  opr_type Тип операции
  \brief   Регистрирует имена операций
*/
template <typename opr_type>
class OperatorName
{
public:
	static std::string name(const opr_type& pOperator);

private:
	struct OprItem
	{
		opr_type m_pOperator;
		std::string m_name;

		OprItem(const opr_type& pOperator, const std::string& name);

		bool operator== (const opr_type& pOperator) const;
	};
	typedef std::list<OprItem> NameList;

	static const NameList& getList();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/operation/operation_name.inl"

#endif // _LIB_RUNTIME_CALC_OPERATION_NAME_H_

/*!
  \copyright (c) RDO-Team, 2011
  \file      operation_name.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     Название операции
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
#include <algorithm>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- OperatorName
// --------------------------------------------------------------------------------
template <typename opr_type>
inline std::string OperatorName<opr_type>::name(const opr_type& pOperator)
{
	const auto it = std::find(getList().begin(), getList().end(), pOperator);
	ASSERT(it != getList().end());
	return it->m_name;
}

template <typename opr_type>
inline OperatorName<opr_type>::OprItem::OprItem(const opr_type& pOperator, const std::string& name)
	: m_pOperator(pOperator)
	, m_name(name)
{}

template <typename opr_type>
inline bool OperatorName<opr_type>::OprItem::operator== (const opr_type& pOperator) const
{
	return m_pOperator == pOperator;
}

template <>
inline const OperatorName<RDOValue (RDOValue::*)(const RDOValue&) const>::NameList& OperatorName<RDOValue (RDOValue::*)(const RDOValue&) const>::getList()
{
	static NameList s_nameList;
	if (s_nameList.empty())
	{
		s_nameList.push_back(OprItem(&RDOValue::operator+,  "+"));
		s_nameList.push_back(OprItem(&RDOValue::operator-,  "-"));
		s_nameList.push_back(OprItem(&RDOValue::operator*,  "*"));
		s_nameList.push_back(OprItem(&RDOValue::operator/,  "/"));
		s_nameList.push_back(OprItem(&RDOValue::operator&&, "and"));
		s_nameList.push_back(OprItem(&RDOValue::operator||, "or"));
	}
	return s_nameList;
}

template <>
inline const OperatorName<bool (RDOValue::*)(const RDOValue&) const>::NameList& OperatorName<bool (RDOValue::*)(const RDOValue&) const>::getList()
{
	static NameList s_nameList;
	if (s_nameList.empty())
	{
		s_nameList.push_back(OprItem(&RDOValue::operator==, "=="));
		s_nameList.push_back(OprItem(&RDOValue::operator!=, "<>"));
		s_nameList.push_back(OprItem(&RDOValue::operator<,  "<"));
		s_nameList.push_back(OprItem(&RDOValue::operator>,  ">"));
		s_nameList.push_back(OprItem(&RDOValue::operator<=, "<="));
		s_nameList.push_back(OprItem(&RDOValue::operator>=, ">="));
	}
	return s_nameList;
}

template <>
inline const OperatorName<bool (RDOValue::*)() const>::NameList& OperatorName<bool (RDOValue::*)() const>::getList()
{
	static NameList s_nameList;
	if (s_nameList.empty())
	{
		s_nameList.push_back(OprItem(&RDOValue::operator!, "!"));
	}
	return s_nameList;
}

template <>
inline const OperatorName<RDOValue (RDOValue::*)() const>::NameList& OperatorName<RDOValue (RDOValue::*)() const>::getList()
{
	static NameList s_nameList;
	if (s_nameList.empty())
	{
		s_nameList.push_back(OprItem(&RDOValue::operator-, "-"));
	}
	return s_nameList;
}

template <>
inline const OperatorName<int (RDOValue::*)() const>::NameList& OperatorName<int (RDOValue::*)() const>::getList()
{
	static NameList s_nameList;
	if (s_nameList.empty())
	{
		s_nameList.push_back(OprItem(&RDOValue::getInt, "getInt"));
	}
	return s_nameList;
}

CLOSE_RDO_RUNTIME_NAMESPACE

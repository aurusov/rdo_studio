/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      operation_name.inl
 * @authors   Урусов Андрей
 * @date      13.03.2011
 * @brief     unknown
 * @indent    4T
 *********************************************************************************/

// **************************************************************************** PCH
// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** OperatorName
// ********************************************************************************
template <typename opr_type>
inline tstring OperatorName<opr_type>::name(CREF(opr_type) pOperator)
{
	BOOST_AUTO(it, std::find(getList().begin(), getList().end(), pOperator));
	ASSERT(it != getList().end());
	return it->m_name;
}

template <typename opr_type>
inline OperatorName<opr_type>::OprItem::OprItem(CREF(opr_type) pOperator, CREF(tstring) name)
	: m_pOperator(pOperator)
	, m_name     (name     )
{}

template <typename opr_type>
inline rbool OperatorName<opr_type>::OprItem::operator== (CREF(opr_type) pOperator) const
{
	return m_pOperator == pOperator;
}

template <>
inline static CREF(OperatorName<RDOValue (RDOValue::*)(CREF(RDOValue)) const>::NameList) OperatorName<RDOValue (RDOValue::*)(CREF(RDOValue)) const>::getList()
{
	static NameList s_nameList;
	if (s_nameList.empty())
	{
		s_nameList.push_back(OprItem(&RDOValue::operator+,  _T("+"  )));
		s_nameList.push_back(OprItem(&RDOValue::operator-,  _T("-"  )));
		s_nameList.push_back(OprItem(&RDOValue::operator*,  _T("*"  )));
		s_nameList.push_back(OprItem(&RDOValue::operator/,  _T("/"  )));
		s_nameList.push_back(OprItem(&RDOValue::operator&&, _T("and")));
		s_nameList.push_back(OprItem(&RDOValue::operator||, _T("or" )));
	}
	return s_nameList;
}

template <>
inline static CREF(OperatorName<rbool (RDOValue::*)(CREF(RDOValue)) const>::NameList) OperatorName<rbool (RDOValue::*)(CREF(RDOValue)) const>::getList()
{
	static NameList s_nameList;
	if (s_nameList.empty())
	{
		s_nameList.push_back(OprItem(&RDOValue::operator==, _T("==")));
		s_nameList.push_back(OprItem(&RDOValue::operator!=, _T("<>")));
		s_nameList.push_back(OprItem(&RDOValue::operator<,  _T("<" )));
		s_nameList.push_back(OprItem(&RDOValue::operator>,  _T(">" )));
		s_nameList.push_back(OprItem(&RDOValue::operator<=, _T("<=")));
		s_nameList.push_back(OprItem(&RDOValue::operator>=, _T(">=")));
	}
	return s_nameList;
}

template <>
inline static CREF(OperatorName<rbool (RDOValue::*)() const>::NameList) OperatorName<rbool (RDOValue::*)() const>::getList()
{
	static NameList s_nameList;
	if (s_nameList.empty())
	{
		s_nameList.push_back(OprItem(&RDOValue::operator!, _T("!")));
	}
	return s_nameList;
}

template <>
inline static CREF(OperatorName<RDOValue (RDOValue::*)() const>::NameList) OperatorName<RDOValue (RDOValue::*)() const>::getList()
{
	static NameList s_nameList;
	if (s_nameList.empty())
	{
		s_nameList.push_back(OprItem(&RDOValue::operator-, _T("-")));
	}
	return s_nameList;
}

template <>
inline static CREF(OperatorName<rsint (RDOValue::*)() const>::NameList) OperatorName<rsint (RDOValue::*)() const>::getList()
{
	static NameList s_nameList;
	if (s_nameList.empty())
	{
		s_nameList.push_back(OprItem(&RDOValue::getInt, _T("getInt")));
	}
	return s_nameList;
}

CLOSE_RDO_RUNTIME_NAMESPACE

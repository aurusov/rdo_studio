/*
 * copyright: (c) RDO-Team, 2011
 * filename : operation_name.inl
 * author   : Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- OperatorName
// ----------------------------------------------------------------------------
template <typename ret_type>
inline tstring OperatorName<ret_type>::name(CREF(opr_type) pOperator)
{
	BOOST_AUTO(it, std::find(getList().begin(), getList().end(), pOperator));
	ASSERT(it != getList().end());
	return it->m_name;
}

template <typename ret_type>
inline OperatorName<ret_type>::OprItem::OprItem(CREF(typename OperatorName<ret_type>::opr_type) pOperator, CREF(tstring) name)
	: m_pOperator(pOperator)
	, m_name     (name     )
{}

template <typename ret_type>
inline rbool OperatorName<ret_type>::OprItem::operator== (CREF(typename OperatorName<ret_type>::opr_type) pOperator) const
{
	return m_pOperator == pOperator;
}

template <>
inline static CREF(OperatorName<RDOValue>::NameList) OperatorName<RDOValue>::getList()
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
inline static CREF(OperatorName<rbool>::NameList) OperatorName<rbool>::getList()
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

CLOSE_RDO_RUNTIME_NAMESPACE

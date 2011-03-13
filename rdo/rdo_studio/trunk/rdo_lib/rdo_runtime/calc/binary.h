/*
 * copyright: (c) RDO-Team, 2011
 * filename : binary.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALC_BINARY_H_
#define _RDOCALC_BINARY_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- OperatorName
// ----------------------------------------------------------------------------
template <typename ret_type>
class OperatorName
{
public:
	typedef ret_type (RDOValue::*opr_type)(CREF(RDOValue) rdovalue) const;

	static tstring name(CREF(opr_type) pOperator)
	{
		BOOST_AUTO(it, std::find(getList().begin(), getList().end(), pOperator));
		ASSERT(it != getList().end());
		return it->m_name;
	}

private:
	struct OprItem
	{
		opr_type m_pOperator;
		tstring  m_name;

		OprItem(CREF(opr_type) pOperator, CREF(tstring) name)
			: m_pOperator(pOperator)
			, m_name     (name     )
		{}
		rbool operator== (CREF(opr_type) pOperator) const
		{
			return m_pOperator == pOperator;
		}
	};
	typedef std::list<OprItem> NameList;
	static CREF(NameList) getList();
};

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

// ----------------------------------------------------------------------------
// ---------- RDOCalcBinary
// ----------------------------------------------------------------------------
template <typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue) rdovalue) const>
class RDOCalcBinary: public RDOCalc
{
friend class rdo::Factory<RDOCalcBinary<ret_type, pOperator> >;
public:
	static RDOSrcInfo getStaticSrcInfo(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	LPRDOCalc      getLeft        () const;
	LPRDOCalcConst getRightAsConst() const;
	void           setRight       (CREF(LPRDOCalc) pRight);

protected:
	RDOCalcBinary(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	LPRDOCalc  m_pLeft;
	LPRDOCalc  m_pRight;

private:
	REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/calc/binary.inl"

#endif //! _RDOCALC_BINARY_H_

/*!
  \copyright (c) RDO-Team, 2011
  \file      range-inl.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      10.02.2010
  \brief
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_range.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

template<class T>
inline RDOTypeRange<T>::RDOTypeRange(const LPRDOTypeRangeRange& range)
	: T      (     )
	, m_range(range)
{
	ASSERT(m_range);
}

template<class T>
inline RDOTypeRange<T>::~RDOTypeRange()
{}

template<class T>
inline std::string RDOTypeRange<T>::name() const
{
	return rdo::format("%s %s", T::name().c_str(), m_range->src_text().c_str());
}

template<class T>
inline LPIType RDOTypeRange<T>::type_cast(const LPIType& from, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
	return parent_type::type_cast(from, from_src_info, to_src_info, src_info);
}

template<class T>
inline LPRDOValue RDOTypeRange<T>::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
	LPRDOValue pToValue = T::value_cast(pFrom, to_src_info, src_info);
	ASSERT(pToValue);
	m_range->checkValue(pToValue);
	return pToValue;
}

template<class T>
inline rdo::runtime::LPRDOCalc RDOTypeRange<T>::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPIType& pType) const
{
	return rdo::Factory<rdo::runtime::RDOCalcCheckRange>::create(range()->getMin()->value(), range()->getMax()->value(), T::calc_cast(pCalc, pType));
}

template<class T>
inline const LPRDOTypeRangeRange& RDOTypeRange<T>::range() const
{
	return m_range;
}

template<class T>
inline rdo::runtime::RDOValue RDOTypeRange<T>::get_default() const
{
	return m_range->getMin()->value();
}

CLOSE_RDO_PARSER_NAMESPACE

/*!
  \copyright (c) RDO-Team, 2011
  \file      range.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      10.02.2010
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser_error.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/runtime/calc/procedural/calc_range.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

inline RDOTypeRangeRange::RDOTypeRangeRange(CREF(LPRDOValue) pMinValue, CREF(LPRDOValue) pMaxValue, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info )
	, m_pMinValue     (pMinValue)
	, m_pMaxValue     (pMaxValue)
{
	ASSERT(m_pMinValue->defined());
	ASSERT(m_pMaxValue->defined());
	setSrcText(rdo::format(_T("[%s..%s]"), m_pMinValue->value().getAsString().c_str(), m_pMaxValue->value().getAsString().c_str()));
}

inline RDOTypeRangeRange::~RDOTypeRangeRange()
{}

inline void RDOTypeRangeRange::checkRange() const
{
	if (m_pMinValue->typeID() != m_pMaxValue->typeID())
	{
		parser::g_error().error(m_pMaxValue->src_info(), rdo::format(_T("Границы диапазона должны быть одного типа, найдено: [%s] .. [%s]"), m_pMinValue->typeInfo()->type()->name().c_str(), m_pMaxValue->typeInfo()->type()->name().c_str()));
	}
	if (m_pMinValue->value() > m_pMaxValue->value())
	{
		parser::g_error().error(m_pMaxValue->src_info(), _T("Левая граница диапазона должна быть меньше правой"));
	}
}

inline void RDOTypeRangeRange::checkValue(CREF(LPRDOValue) pValue) const
{
	ASSERT(pValue);

	if (pValue->value() < m_pMinValue->value() || pValue->value() > m_pMaxValue->value())
	{
		if (pValue->src_filetype() == src_filetype() && pValue->src_pos().m_last_line == src_pos().m_last_line)
		{
			parser::g_error().error(pValue->src_info(), rdo::format(_T("Значение выходит за допустимый диапазон [%s..%s]: %s"), m_pMaxValue->value().getAsString().c_str(), m_pMaxValue->value().getAsString().c_str(), pValue->value().getAsString().c_str()));
		}
		else
		{
			parser::g_error().push_only(pValue->src_info(), rdo::format(_T("Значение выходит за допустимый диапазон [%s..%s]: %s"), m_pMaxValue->value().getAsString().c_str(), m_pMaxValue->value().getAsString().c_str(), pValue->value().getAsString().c_str()));
			parser::g_error().push_only(src_info(),         rdo::format(_T("См. описание диапазона")));
			parser::g_error().push_done();
		}
	}
}

inline CREF(LPRDOValue) RDOTypeRangeRange::getMin() const
{
	return m_pMinValue;
}

inline CREF(LPRDOValue) RDOTypeRangeRange::getMax() const
{
	return m_pMaxValue;
}

// --------------------------------------------------------------------------------
// -------------------- RDOTypeRange
// --------------------------------------------------------------------------------
template<class T>
inline RDOTypeRange<T>::RDOTypeRange(CREF(LPRDOTypeRangeRange) range)
	: T      (     )
	, m_range(range)
{
	ASSERT(m_range);
}

template<class T>
inline RDOTypeRange<T>::~RDOTypeRange()
{}

template<class T>
inline tstring RDOTypeRange<T>::name() const
{
	return rdo::format(_T("%s %s"), T::name().c_str(), m_range->src_text().c_str());
}

template<class T>
inline LPRDOType RDOTypeRange<T>::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	return parent_type::type_cast(from, from_src_info, to_src_info, src_info);
}

template<class T>
inline LPRDOValue RDOTypeRange<T>::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	LPRDOValue pToValue = T::value_cast(pFrom, to_src_info, src_info);
	ASSERT(pToValue);
	m_range->checkValue(pToValue);
	return pToValue;
}

template<class T>
inline rdo::runtime::LPRDOCalc RDOTypeRange<T>::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return rdo::Factory<rdo::runtime::RDOCalcCheckRange>::create(range()->getMin()->value(), range()->getMax()->value(), T::calc_cast(pCalc, pType));
}

template<class T>
inline CREF(LPRDOTypeRangeRange) RDOTypeRange<T>::range() const
{
	return m_range;
}

template<class T>
inline rdo::runtime::RDOValue RDOTypeRange<T>::get_default() const
{
	return m_range->getMin()->value();
}

CLOSE_RDO_PARSER_NAMESPACE

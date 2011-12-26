/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_type_range.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      10.02.2010
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdoparser_error.h"
#include "converter/smr2rdox/rdoparser.h"
#include "simulator/runtime/calc/procedural/calc_range.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

inline RDOTypeRangeRange::RDOTypeRangeRange(CREF(RDOValue) min_value, CREF(RDOValue) max_value, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info )
	, m_min_value     (min_value)
	, m_max_value     (max_value)
{
	ASSERT(m_min_value.defined());
	ASSERT(m_max_value.defined());
	setSrcText(rdo::format(_T("[%s..%s]"), m_min_value->getAsString().c_str(), m_max_value->getAsString().c_str()));
}

inline RDOTypeRangeRange::~RDOTypeRangeRange()
{}

inline void RDOTypeRangeRange::checkRange() const
{
	if (m_min_value->typeID() != m_max_value->typeID())
	{
		rdoConverter::g_error().error(m_max_value.src_info(), rdo::format(_T("Границы диапазона должны быть одного типа, найдено: [%s] .. [%s]"), m_min_value.type()->name().c_str(), m_max_value.type()->name().c_str()));
	}
	if (m_min_value.value() > m_max_value.value())
	{
		rdoConverter::g_error().error(m_max_value.src_info(), _T("Левая граница диапазона должна быть меньше правой"));
	}
}

inline void RDOTypeRangeRange::checkValue(CREF(RDOValue) value) const
{
	if (value.value() < m_min_value.value() || value.value() > m_max_value.value())
	{
		if (value.src_filetype() == src_filetype() && value.src_pos().m_last_line == src_pos().m_last_line)
		{
			rdoConverter::g_error().error(value.src_info(), rdo::format(_T("Значение выходит за допустимый диапазон [%s..%s]: %s"), m_min_value->getAsString().c_str(), m_max_value->getAsString().c_str(), value->getAsString().c_str()));
		}
		else
		{
			rdoConverter::g_error().push_only(value.src_info(), rdo::format(_T("Значение выходит за допустимый диапазон [%s..%s]: %s"), m_min_value->getAsString().c_str(), m_max_value->getAsString().c_str(), value->getAsString().c_str()));
			rdoConverter::g_error().push_only(src_info(),       rdo::format(_T("См. описание диапазона")));
			rdoConverter::g_error().push_done();
		}
	}
}

inline CREF(RDOValue) RDOTypeRangeRange::getMin() const
{
	return m_min_value;
}

inline CREF(RDOValue) RDOTypeRangeRange::getMax() const
{
	return m_max_value;
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
inline RDOValue RDOTypeRange<T>::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue = T::value_cast(from, to_src_info, src_info);
	m_range->checkValue(toValue);
	return toValue;
}

template<class T>
inline rdoRuntime::LPRDOCalc RDOTypeRange<T>::calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return rdo::Factory<rdoRuntime::RDOCalcCheckRange>::create(range()->getMin().value(), range()->getMax().value(), T::calc_cast(pCalc, pType));
}

template<class T>
inline CREF(LPRDOTypeRangeRange) RDOTypeRange<T>::range() const
{
	return m_range;
}

template<>
inline RDOValue RDOTypeRange<RDOType__int>::get_default() const
{
	return RDOValue(m_range->getMin().value(), rdo::Factory<RDOType__int>::create(), RDOParserSrcInfo());
}

template<>
inline RDOValue RDOTypeRange<RDOType__real>::get_default() const
{
	return RDOValue(m_range->getMin().value(), rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo());
}

CLOSE_RDO_CONVERTER_NAMESPACE

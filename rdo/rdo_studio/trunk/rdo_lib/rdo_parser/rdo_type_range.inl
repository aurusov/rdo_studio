/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type_range.inl
 * author   : Урусов Андрей
 * date     : 10.02.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdoparser_error.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

inline RDOTypeRange::RDOTypeRange(CREF(RDOValue) min_value, CREF(RDOValue) max_value, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info )
	, m_min_value     (min_value)
	, m_max_value     (max_value)
{
	ASSERT(m_min_value.defined());
	ASSERT(m_max_value.defined());
	setSrcText(rdo::format(_T("[%s..%s]"), m_min_value->getAsString().c_str(), m_max_value->getAsString().c_str()));
}

inline RDOTypeRange::~RDOTypeRange()
{}

inline void RDOTypeRange::checkRange() const throw(...)
{
	if (m_min_value.value() > m_max_value.value())
	{
		rdoParse::g_error().error(m_max_value.src_info(), _T("Левая граница диапазона должна быть меньше правой"));
	}
}

inline void RDOTypeRange::checkValue(CREF(RDOValue) value) const throw(...)
{
	if (value.value() < m_min_value.value() || value.value() > m_max_value.value())
	{
		if (value.src_filetype() == src_filetype() && value.src_pos().m_last_line == src_pos().m_last_line)
		{
			rdoParse::g_error().error(value.src_info(), rdo::format(_T("Значение выходит за допустимый диапазон [%s..%s]: %s"), m_min_value->getAsString().c_str(), m_max_value->getAsString().c_str(), value->getAsString().c_str()));
		}
		else
		{
			rdoParse::g_error().push_only(value.src_info(), rdo::format(_T("Значение выходит за допустимый диапазон [%s..%s]: %s"), m_min_value->getAsString().c_str(), m_max_value->getAsString().c_str(), value->getAsString().c_str()));
			rdoParse::g_error().push_only(src_info(),       rdo::format(_T("См. описание диапазона")));
			rdoParse::g_error().push_done();
		}
	}
}

//inline CREF(RDOValue) RDOTypeRange::getMin() const
//{
//	return m_min_value;
//}
//
//inline CREF(RDOValue) RDOTypeRange::getMax() const
//{
//	return m_max_value;
//}

CLOSE_RDO_PARSER_NAMESPACE

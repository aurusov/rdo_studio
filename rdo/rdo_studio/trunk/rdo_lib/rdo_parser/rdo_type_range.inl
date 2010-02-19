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
{}

inline RDOTypeRange::~RDOTypeRange()
{}

inline void RDOTypeRange::check()
{
	if (isExist())
	{
		if (m_min_value.value() > m_max_value.value())
		{
			rdoParse::g_error().error(m_max_value.src_info(), _T("Левая граница диапазона должна быть меньше правой"));
		}
		setSrcText(rdo::format(_T("[%s..%s]"), m_min_value->getAsString().c_str(), m_max_value->getAsString().c_str()));
	}
}

inline rbool RDOTypeRange::isExist() const
{
	return m_min_value.defined() && m_max_value.defined();
}

inline CREF(RDOValue) RDOTypeRange::getMin() const
{
	return m_min_value;
}

inline CREF(RDOValue) RDOTypeRange::getMax() const
{
	return m_max_value;
}

CLOSE_RDO_PARSER_NAMESPACE

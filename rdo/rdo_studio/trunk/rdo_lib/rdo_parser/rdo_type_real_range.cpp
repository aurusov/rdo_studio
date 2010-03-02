/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type_real_range.cpp
 * author   : Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_type_real_range.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOTypeRealRange
// ----------------------------------------------------------------------------
RDOTypeRealRange::RDOTypeRealRange(CREF(LPRDOTypeRange) range)
	: RDOType__real(     )
	, m_range      (range)
{
	ASSERT(m_range);
}

RDOTypeRealRange::~RDOTypeRealRange()
{}

inline tstring RDOTypeRealRange::name() const
{
	return rdo::format(_T("%s %s"), parent_type::name().c_str(), m_range->src_text().c_str());
}

RDOValue RDOTypeRealRange::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue = parent_type::value_cast(from, to_src_info, src_info);
	m_range->checkValue(toValue);
	return toValue;
}

CLOSE_RDO_PARSER_NAMESPACE

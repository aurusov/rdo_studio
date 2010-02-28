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
	: RDOType(&rdoRuntime::g_real)
	, m_range(range              )
{
	ASSERT(m_range);
}

RDOTypeRealRange::~RDOTypeRealRange()
{}

inline tstring RDOTypeRealRange::name() const
{
	return rdo::format(_T("%s %s"), g_real.name().c_str(), m_range->src_text().c_str());
}

CPTR(RDOType) RDOTypeRealRange::type_cast(CREF(RDOType) from) const
{
	return g_real.type_cast(from);
}

rdoRuntime::RDOValue RDOTypeRealRange::value_cast(CREF(rdoRuntime::RDOValue) from) const
{
	return g_real.value_cast(from);
}

void RDOTypeRealRange::writeModelStructure(REF(std::ostream) stream) const
{
	g_real.writeModelStructure(stream);
}

CLOSE_RDO_PARSER_NAMESPACE

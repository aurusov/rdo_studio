/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type_int_range.cpp
 * author   : Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_type_int_range.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOTypeIntRange
// ----------------------------------------------------------------------------
RDOTypeIntRange::RDOTypeIntRange(CREF(LPRDOTypeRange) range)
	: RDOType(&rdoRuntime::g_int)
	, m_range(range             )
{
	ASSERT(m_range);
}

RDOTypeIntRange::~RDOTypeIntRange()
{}

inline tstring RDOTypeIntRange::name() const
{
	return rdo::format(_T("%s %s"), g_int.name().c_str(), m_range->src_text().c_str());
}

CPTR(RDOType) RDOTypeIntRange::type_cast(CREF(RDOType) from) const
{
	return g_int.type_cast(from);
}

rdoRuntime::RDOValue RDOTypeIntRange::value_cast(CREF(rdoRuntime::RDOValue) from) const
{
	return g_int.value_cast(from);
}

void RDOTypeIntRange::writeModelStructure(REF(std::ostream) stream) const
{
	g_int.writeModelStructure(stream);
}

CLOSE_RDO_PARSER_NAMESPACE

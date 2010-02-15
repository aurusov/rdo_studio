/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type_param.cpp
 * author   : Урусов Андрей
 * date     : 12.02.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_type_param.h"
#include "rdo_lib/rdo_parser/rdo_type_range.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOTypeParam
// ----------------------------------------------------------------------------
RDOTypeParam::RDOTypeParam(CREF(rdoRuntime::RDOType) type, CREF(RDOParserSrcInfo) src_info)
	: RDOType         (rdoRuntime::g_unknow)
	, RDOParserSrcInfo(src_info            )
{}

RDOTypeParam::~RDOTypeParam()
{}

void RDOTypeParam::writeModelStructure(REF(std::ostream) stream) const
{
	//todo недоделано
	NEVER_REACH_HERE;
}

// ----------------------------------------------------------------------------
// ---------- RDOTypeParamInt
// ----------------------------------------------------------------------------
RDOTypeParamInt::RDOTypeParamInt(PTR(RDOTypeRange) range, CREF(RDOParserSrcInfo) src_info)
	: RDOTypeParam(rdoRuntime::g_int, src_info)
{
	m_range = range;
}

CLOSE_RDO_PARSER_NAMESPACE

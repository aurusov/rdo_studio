/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdortp_param.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdortp_param.h"
#include "rdo_lib/rdo_parser/rdortp.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDORTPParam
// ----------------------------------------------------------------------------
RDORTPParam::RDORTPParam(CPTR(RDORTPResType) const resType, CPTR(RDOTypeParam) const parType, CREF(RDOParserSrcInfo) src_info)
	: RDOParserObject (resType )
	, RDOParserSrcInfo(src_info)
	, m_resType       (resType )
	, m_parType       (parType )
{}

RDORTPParam::~RDORTPParam()
{}

void RDORTPParam::writeModelStructure(REF(std::ostream) stream) const
{
	stream << name() << _T(" ");
//todo back getType()->writeModelStructure(stream);
}

CLOSE_RDO_PARSER_NAMESPACE

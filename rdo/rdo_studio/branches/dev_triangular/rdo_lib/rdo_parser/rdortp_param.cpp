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
#include "rdo_lib/rdo_runtime/rdo_resource.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDORTPParam
// ----------------------------------------------------------------------------
RDORTPParam::RDORTPParam(CREF(LPTypeInfo) pType, CREF(RDOValue) default, CREF(RDOParserSrcInfo) src_info)
	: RDOParam(src_info, pType, default)
{}

RDORTPParam::~RDORTPParam()
{}

CREF(tstring) RDORTPParam::name() const
{
	return RDOParam::name();
}

void RDORTPParam::writeModelStructure(REF(std::ostream) stream) const
{
	stream << name() << _T(" ");
	getTypeInfo()->type()->writeModelStructure(stream);
}

CLOSE_RDO_PARSER_NAMESPACE

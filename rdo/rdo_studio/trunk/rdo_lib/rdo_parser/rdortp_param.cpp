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
RDORTPParam::RDORTPParam(CPTR(RDORTPResType) const resType, CREF(LPRDOTypeParam) pParamType, CREF(RDOValue) defaultValue, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info    )
	, m_resType       (resType     )
	, m_paramType     (pParamType  )
	, m_defaultValue  (defaultValue)
{
	ASSERT(m_paramType);
}

RDORTPParam::~RDORTPParam()
{}

CREF(tstring) RDORTPParam::name() const
{
	return src_info().src_text();
}

void RDORTPParam::writeModelStructure(REF(std::ostream) stream) const
{
	stream << name() << _T(" ");
	getParamType()->writeModelStructure(stream);
}

void RDORTPParam::checkDefault() const
{
	//! TODO: недоделано
	NEVER_REACH_HERE;
}

CLOSE_RDO_PARSER_NAMESPACE

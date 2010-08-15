/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdortp_param.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_converter/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/rdortp_param.h"
#include "rdo_lib/rdo_converter/rdortp.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDORTPParam
// ----------------------------------------------------------------------------
RDORTPParam::RDORTPParam(CREF(LPRDORTPResType) pResourceType, CREF(LPRDOTypeParam) pParamType, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info     )
	, m_pResourceType (pResourceType)
	, m_paramType     (pParamType   )
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

CLOSE_RDO_CONVERTER_NAMESPACE

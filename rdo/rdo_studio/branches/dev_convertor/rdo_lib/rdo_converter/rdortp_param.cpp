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
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDORTPParam
// ----------------------------------------------------------------------------
RDORTPParam::RDORTPParam(CREF(LPRDOTypeParam) pParamType, CREF(RDOValue) default, CREF(RDOParserSrcInfo) src_info)
	: RDOParam(src_info, pParamType, default)
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
	getType()->writeModelStructure(stream);
}

CLOSE_RDO_CONVERTER_NAMESPACE

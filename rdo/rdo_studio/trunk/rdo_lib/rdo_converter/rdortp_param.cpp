/*
  \copyright (c) RDO-Team, 2011
  \file      rdortp_param.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей
  \date      
  \brief     
  \indent    4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_converter/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/rdortp_param.h"
#include "rdo_lib/rdo_runtime/rdo_resource.h"
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

/*
 * copyright: (c) RDO-Team, 2011
 * filename : type_param_suchas.cpp
 * author   : Урусов Андрей
 * date     : 09.01.2011
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/type/type_param_suchas.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOTypeParamSuchAs
// ----------------------------------------------------------------------------
RDOTypeParamSuchAs::RDOTypeParamSuchAs(CREF(LPRDOParam) pParam)
	: RDOTypeParam(pParam->getType()->type(), pParam->getType()->src_info())
	, m_pParam(pParam)
{}

RDOTypeParamSuchAs::~RDOTypeParamSuchAs()
{}

CREF(LPRDOParam) RDOTypeParamSuchAs::getParam() const
{
	return m_pParam;
}

CLOSE_RDO_PARSER_NAMESPACE

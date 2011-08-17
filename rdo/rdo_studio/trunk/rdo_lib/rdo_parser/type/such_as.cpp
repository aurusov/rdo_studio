/*
  \copyright (c) RDO-Team, 2011
  \file      such_as.cpp
  \author    Урусов Андрей
  \date      09.01.2011
  \brief     
  \indent    4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/type/such_as.h"
#include "rdo_lib/rdo_runtime/rdo_resource.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOTypeParamSuchAs
// ----------------------------------------------------------------------------
RDOTypeParamSuchAs::RDOTypeParamSuchAs(CREF(LPRDOParam) pParam)
	: TypeInfo(pParam->getTypeInfo())
	, m_pParam(pParam               )
{}

RDOTypeParamSuchAs::~RDOTypeParamSuchAs()
{}

CREF(LPRDOParam) RDOTypeParamSuchAs::getParam() const
{
	return m_pParam;
}

CLOSE_RDO_PARSER_NAMESPACE

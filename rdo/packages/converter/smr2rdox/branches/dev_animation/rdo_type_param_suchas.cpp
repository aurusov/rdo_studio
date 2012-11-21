/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_type_param_suchas.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.01.2011
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_type_param_suchas.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOTypeParamSuchAs
// --------------------------------------------------------------------------------
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

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

/*!
  \copyright (c) RDO-Team, 2011
  \file      param.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.01.2011
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/param.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

RDOParam::RDOParam(CREF(tstring) name, CREF(LPRDOTypeParam) pType, CREF(LPRDOValue) pDefault)
	: RDOParserSrcInfo(name        )
	, m_pType         (pType       )
	, m_pDefault      (pDefault    )
{
	checkDefault();
}

RDOParam::RDOParam(CREF(RDOParserSrcInfo) src_info, CREF(LPRDOTypeParam) pType, CREF(LPRDOValue) pDefault)
	: RDOParserSrcInfo(src_info)
	, m_pType         (pType   )
	, m_pDefault      (pDefault)
{
	checkDefault();
}

RDOParam::~RDOParam()
{}

void RDOParam::checkDefault()
{
	if (m_pDefault && m_pDefault->defined())
	{
		m_pType->type()->type_cast(m_pDefault->type(), m_pDefault->src_info(), this->src_info(), m_pDefault->src_info());
		m_pDefault = m_pType->type()->value_cast(m_pDefault, this->src_info(), m_pDefault->src_info());
	}
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

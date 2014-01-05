/*!
  \copyright (c) RDO-Team, 2011
  \file      param.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.01.2011
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/param.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

const std::string RDOParam::CONTEXT_PARAM_PARAM_ID = "param_id";

RDOParam::RDOParam(CREF(tstring) name, CREF(LPTypeInfo) pType, CREF(LPRDOValue) pDefault)
	: RDOParserSrcInfo(name    )
	, m_pType         (pType   )
	, m_pDefault      (pDefault)
{
	checkDefault();
}

RDOParam::RDOParam(CREF(RDOParserSrcInfo) srcInfo, CREF(LPTypeInfo) pType, CREF(LPRDOValue) pDefault)
	: RDOParserSrcInfo(srcInfo )
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
		m_pType->type()->type_cast(m_pDefault->typeInfo()->type(), m_pDefault->src_info(), this->src_info(), m_pDefault->src_info());
		m_pDefault = m_pType->type()->value_cast(m_pDefault, this->src_info(), m_pDefault->src_info());
	}
}

CLOSE_RDO_PARSER_NAMESPACE

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

OPEN_RDO_CONVERTER_NAMESPACE

RDOParam::RDOParam(CREF(tstring) name, CREF(LPRDOTypeParam) pType, RDOValue defaultValue)
	: RDOParserSrcInfo(name        )
	, m_pType         (pType       )
	, m_default       (defaultValue)
{
	checkDefault();
}

RDOParam::RDOParam(CREF(RDOParserSrcInfo) src_info, CREF(LPRDOTypeParam) pType, RDOValue defaultValue)
	: RDOParserSrcInfo(src_info    )
	, m_pType         (pType       )
	, m_default       (defaultValue)
{
	checkDefault();
}

RDOParam::~RDOParam()
{}

void RDOParam::checkDefault()
{
	if (m_default.defined())
	{
		m_pType->type()->type_cast(m_default.type(), m_default.src_info(), this->src_info(), m_default.src_info());
		m_default = m_pType->type()->value_cast(m_default, this->src_info(), m_default.src_info());
	}
}

CLOSE_RDO_CONVERTER_NAMESPACE

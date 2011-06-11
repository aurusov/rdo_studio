/*
 * copyright: (c) RDO-Team, 2011
 * filename : param.cpp
 * author   : Урусов Андрей
 * date     : 09.01.2011
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_converter/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/param.h"
#include "rdo_lib/rdo_runtime/rdo_resource.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

RDOParam::RDOParam(CREF(tstring) name, CREF(LPRDOTypeParam) pType, RDOValue default)
	: RDOParserSrcInfo(name   )
	, m_pType         (pType  )
	, m_default       (default)
{
	checkDefault();
}

RDOParam::RDOParam(CREF(RDOParserSrcInfo) src_info, CREF(LPRDOTypeParam) pType, RDOValue default)
	: RDOParserSrcInfo(src_info)
	, m_pType         (pType   )
	, m_default       (default )
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

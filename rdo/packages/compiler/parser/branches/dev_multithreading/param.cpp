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
#include "simulator/compiler/parser/param.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

RDOParam::RDOParam(CREF(tstring) name, CREF(LPTypeInfo) pType, CREF(LPRDOValue) pDefault)
	: RDOParserSrcInfo(name        )
	, m_pType         (pType       )
	, m_pDefault      (pDefault    )
{
	checkDefault();
}

RDOParam::RDOParam(CREF(RDOParserSrcInfo) src_info, CREF(LPTypeInfo) pType, CREF(LPRDOValue) pDefault)
	: RDOParserSrcInfo(src_info    )
	, m_pType         (pType       )
	, m_pDefault      (pDefault    )
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

Context::FindResult RDOParam::onSwitchContext(CREF(LPExpression) pSwitchExpression, CREF(LPRDOValue) pValue) const
{
	ASSERT(pSwitchExpression);
	ASSERT(pValue           );

	LPRDORTPResType pResType = pSwitchExpression->typeInfo()->type().object_dynamic_cast<RDORTPResType>();
	if (pResType)
	{
		LPIContextSwitch pContextSwitch = pResType.interface_dynamic_cast<IContextSwitch>();
		ASSERT(pContextSwitch);
		return pContextSwitch->onSwitchContext(pSwitchExpression, pValue);
	}

	return Context::FindResult();
}

CLOSE_RDO_PARSER_NAMESPACE

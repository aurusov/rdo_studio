/*
 * copyright: (c) RDO-Team, 2011
 * file     : context.cpp
 * author   : Урусов Андрей
 * date     : 06.06.2010
 * brief    : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/context/context.h"
#include "rdo_lib/rdo_parser/context/context_find_i.h"
#include "rdo_lib/rdo_parser/context/context_create_expression_i.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

Context::Context()
{}

Context::~Context()
{}

void Context::setContextStack(CREF(LPContextStack) pContextStack)
{
	ASSERT(pContextStack   );
	ASSERT(!m_pContextStack);
	m_pContextStack = pContextStack;
}

LPContext Context::find(CREF(RDOValue) value) const
{
	LPContext pThis(const_cast<PTR(Context)>(this));
	LPIContextFind pThisContextFind = pThis.interface_dynamic_cast<IContextFind>();
	if (pThisContextFind)
	{
		LPContext pThisResult = pThisContextFind->onFindContext(value);
		if (pThisResult)
		{
			return pThisResult;
		}
	}
	LPContext pPrev = m_pContextStack->prev(pThis);
	return pPrev ? pPrev->find(value) : LPContext();
}

LPExpression Context::create(CREF(RDOValue) value)
{
	LPContext pThis(this);
	LPIContextCreateExpression pThisContextCreateExpression = pThis.interface_dynamic_cast<IContextCreateExpression>();
	if (pThisContextCreateExpression)
	{
		LPExpression pExpression = pThisContextCreateExpression->onCreateExpression(value);
		if (pExpression)
		{
			return pExpression;
		}
	}
	return NULL;
}

CLOSE_RDO_PARSER_NAMESPACE

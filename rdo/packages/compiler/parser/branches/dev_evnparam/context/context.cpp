/*!
  \copyright (c) RDO-Team, 2011
  \file      context.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.06.2010
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/context/context_find_i.h"
#include "simulator/compiler/parser/context/context_switch_i.h"
#include "simulator/compiler/parser/context/context_create_expression_i.h"
// --------------------------------------------------------------------------------

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

LPContext Context::find(CREF(LPRDOValue) pValue) const
{
	ASSERT(pValue);

	LPContext pThis(const_cast<PTR(Context)>(this));
	LPIContextFind pThisContextFind = pThis.interface_dynamic_cast<IContextFind>();
	if (pThisContextFind)
	{
		LPContext pThisResult = pThisContextFind->onFindContext(pValue);
		if (pThisResult)
		{
			return pThisResult;
		}
	}
	LPContext pPrev = m_pContextStack->prev(pThis);
	return pPrev ? pPrev->find(pValue) : LPContext();
}

LPContext Context::swch(CREF(LPRDOValue) pValue) const
{
	ASSERT(pValue);

	LPContext pThis(const_cast<PTR(Context)>(this));
	LPIContextSwitch pThisContextSwitch = pThis.interface_dynamic_cast<IContextSwitch>();
	ASSERT(pThisContextSwitch);
	LPContext pThisResult = pThisContextSwitch->onSwitchContext(pValue);
	ASSERT(pThisResult);
	return pThisResult;
}

LPExpression Context::create(CREF(LPRDOValue) pValue)
{
	ASSERT(pValue);

	LPContext pThis(this);
	LPIContextCreateExpression pThisContextCreateExpression = pThis.interface_dynamic_cast<IContextCreateExpression>();
	if (pThisContextCreateExpression)
	{
		LPExpression pExpression = pThisContextCreateExpression->onCreateExpression(pValue);
		if (pExpression)
		{
			return pExpression;
		}
	}
	return LPExpression(NULL);
}

CLOSE_RDO_PARSER_NAMESPACE

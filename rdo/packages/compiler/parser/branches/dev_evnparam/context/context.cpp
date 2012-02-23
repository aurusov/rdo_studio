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
#include "simulator/compiler/parser/context/context_switch_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- Context::FindResult
// --------------------------------------------------------------------------------
Context::FindResult::FindResult()
{}

Context::FindResult::FindResult(CREF(FindResult) result)
	: m_pContext     (result.m_pContext     )
	, m_pExpression  (result.m_pExpression  )
	, m_pFindByValue (result.m_pFindByValue )
	, m_pValueContext(result.m_pValueContext)
{}

Context::FindResult::FindResult(CREF(LPContext) pContext, CREF(LPExpression) pExpression, CREF(LPRDOValue) pFindByParam, LPContext pValueContext)
	: m_pContext     (pContext     )
	, m_pExpression  (pExpression  )
	, m_pFindByValue (pFindByParam )
	, m_pValueContext(pValueContext)
{}

// --------------------------------------------------------------------------------
// -------------------- Context
// --------------------------------------------------------------------------------
Context::Context()
{}

Context::~Context()
{}

void Context::init()
{}

void Context::deinit()
{
	m_findResult = FindResult();
}

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
		const_cast<PTR(Context)>(this)->m_findResult = pThisContextFind->onFindContext(pValue);
		if (m_findResult.m_pContext)
		{
			return m_findResult.m_pContext;
		}
	}
	LPContext pPrev = m_pContextStack->prev(pThis);
	return pPrev ? pPrev->find(pValue) : LPContext();
}

LPContext Context::swch(CREF(LPRDOValue) pValue) const
{
	ASSERT(pValue);

	LPIContextSwitch pContextSwitch = m_findResult.m_pValueContext.interface_dynamic_cast<IContextSwitch>();
	ASSERT(pContextSwitch);
	FindResult result = pContextSwitch->onSwitchContext(m_findResult.m_pExpression, pValue);
	ASSERT(result.m_pContext);
	result.m_pContext->m_findResult = result;
	ASSERT(result.m_pContext);
	const_cast<PTR(Context)>(this)->deinit();
	return result.m_pContext;
}

LPExpression Context::create(CREF(LPRDOValue) pValue)
{
	ASSERT(pValue);
	ASSERT(m_findResult.m_pFindByValue == pValue);
	ASSERT(m_findResult.m_pExpression);

	LPExpression pExpression = m_findResult.m_pExpression;
	deinit();
	return pExpression;
}

CLOSE_RDO_PARSER_NAMESPACE

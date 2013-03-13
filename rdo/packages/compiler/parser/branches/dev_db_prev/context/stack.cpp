/*!
  \copyright (c) RDO-Team, 2011
  \file      stack.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.06.2010
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/context/stack.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

ContextStack::ContextStack()
{}

ContextStack::~ContextStack()
{
	while (!m_container.empty())
		pop();
}

void ContextStack::push(LPContext pContext)
{
	m_container.push_back(pContext);
	pContext->setContextStack(this);
}

void ContextStack::pop()
{
	ASSERT(!m_container.empty());
	m_container.pop_back();
}

LPContext ContextStack::top() const
{
	ASSERT(!m_container.empty());
	return m_container.back();
}

LPContext ContextStack::global() const
{
	ASSERT(!m_container.empty());
	return m_container.front();
}

LPContext ContextStack::prev(CREF(LPContext) pContext) const
{
	Container::const_iterator it = std::find(m_container.begin(), m_container.end(), pContext);
	if (it == m_container.end() || it == m_container.begin())
	{
		return LPContext();
	}
	--it;
	return it != m_container.end() ? *it : LPContext();
}

CLOSE_RDO_PARSER_NAMESPACE

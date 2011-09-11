/*!
  \copyright (c) RDO-Team, 2011
  \file      stack.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.06.2010
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/context/stack.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

ContextStack::ContextStack()
{
}

ContextStack::~ContextStack()
{
	while (!m_container.empty())
		pop();
}

void ContextStack::push(CREF(LPContext) pContext)
{
	m_container.push_back(pContext);
}

void ContextStack::pop()
{
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

CLOSE_RDO_CONVERTER_NAMESPACE

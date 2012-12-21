/*!
  \copyright (c) RDO-Team, 2012
  \file      statement.cpp
  \authors   Поподьянец Евгений (kurt.gigacore@gmail.com)
  \date      18.04.2012
  \brief     Контекст выражений
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/context/statement.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ContextBreakable
// --------------------------------------------------------------------------------
ContextBreakable::ContextBreakable()
{}

ContextBreakable::~ContextBreakable()
{}

// --------------------------------------------------------------------------------
// -------------------- ContextReturnable
// --------------------------------------------------------------------------------
ContextReturnable::ContextReturnable()
	: m_returnFlag(false)
{}

ContextReturnable::~ContextReturnable()
{}

bool ContextReturnable::returnFlag()
{
	if ((m_returnFlag == false) && (!m_contextReturnableList.empty()))
	{
		m_returnFlag = checkChildFlags();
	}
	return m_returnFlag;
}

void ContextReturnable::setReturnFlag()
{
	m_returnFlag = true;
}

void ContextReturnable::addContext(REF(LPContextReturnable) pContext)
{
	ASSERT(pContext);

	m_contextReturnableList.push_back(pContext);
	ASSERT(!m_contextReturnableList.empty());
}

bool ContextReturnable::checkChildFlags()
{
	BOOST_FOREACH(const LPContextReturnable& pContext, m_contextReturnableList)
	{
		if (!pContext->returnFlag())
		{
			return false;
		}
	}
	return true;
}

CLOSE_RDO_PARSER_NAMESPACE

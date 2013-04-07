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
#include "simulator/compiler/parser/rdoparser.h"
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

rbool ContextReturnable::getReturnFlag() const
{
	if (m_returnFlag)
		return true;

	return !m_contextReturnableList.empty()
		? getChildFlags()
		: false;
}

void ContextReturnable::setReturnFlag()
{
	m_returnFlag = true;
}

void ContextReturnable::addChildContext()
{
	LPContextReturnable pContext = rdo::Factory<ContextReturnable>::create();
	ASSERT(pContext);

	m_contextReturnableList.push_back(pContext);
	RDOParser::s_parser()->contextStack()->push(pContext);
}

rbool ContextReturnable::getChildFlags() const
{
	BOOST_FOREACH(const LPContextReturnable& pContext, m_contextReturnableList)
	{
		if (!pContext->getReturnFlag())
			return false;
	}
	return true;
}

CLOSE_RDO_PARSER_NAMESPACE

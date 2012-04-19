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
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/context/statement.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE
// --------------------------------------------------------------------------------
// -------------------- ContextStatementBase
// --------------------------------------------------------------------------------
ContextStatementBase::ContextStatementBase()
{}

// --------------------------------------------------------------------------------
// -------------------- ContextBreakable
// --------------------------------------------------------------------------------
ContextBreakable::ContextBreakable()
{}

// --------------------------------------------------------------------------------
// -------------------- ContextReturnable
// --------------------------------------------------------------------------------
ContextReturnable::ContextReturnable()
	:m_returnFlag(false)
{}

bool ContextReturnable::returnFlag()
{
	compileFlags();
	return m_returnFlag;
}

void ContextReturnable::addContext(REF(LPContextReturnable) pContext)
{
	ASSERT(pContext);

	pContext->compileFlags();

	m_contextReturnableList.push_back(pContext);
	ASSERT(!m_contextReturnableList.empty());
}

void ContextReturnable::compileFlags()
{
	if(!m_contextReturnableList.empty())
	{
		bool compiledFlag;
		STL_FOR_ALL(m_contextReturnableList, contextIt)
		{
			LPContextReturnable pContext = *contextIt;
			compiledFlag = (compiledFlag)&&(pContext->returnFlag());
		}
		m_returnFlag = m_returnFlag&&compiledFlag;
	}
}

CLOSE_RDO_PARSER_NAMESPACE
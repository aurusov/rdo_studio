/*!
  \copyright (c) RDO-Team, 2011
  \file      memory.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      16.10.2010
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/context/memory.h"
#include "simulator/runtime/calc/locvar.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ContextMemory
// --------------------------------------------------------------------------------
ContextMemory::ContextMemory()
{
	m_pLocalVariableListStack = rdo::Factory<LocalVariableListStack>::create();
	ASSERT(m_pLocalVariableListStack);
}

LPLocalVariableListStack ContextMemory::getLocalMemory()
{
	return m_pLocalVariableListStack;
}

LPContext ContextMemory::onFindContext(CREF(LPRDOValue) pValue) const
{
	ASSERT(pValue);

	LPLocalVariable pLocalVariable = m_pLocalVariableListStack->findLocalVariable(pValue->value().getIdentificator());
	if (pLocalVariable)
	{
		return const_cast<PTR(ContextMemory)>(this);
	}

	return LPContext(NULL);
}

LPExpression ContextMemory::onCreateExpression(CREF(LPRDOValue) pValue)
{
	ASSERT(pValue);

	LPLocalVariable pLocalVariable = m_pLocalVariableListStack->findLocalVariable(pValue->value().getIdentificator());
	ASSERT(pLocalVariable);

	LPExpression pExpression = rdo::Factory<Expression>::create(
		pLocalVariable->getExpression()->typeInfo(),
		rdo::Factory<rdoRuntime::RDOCalcGetLocalVariable>::create(pLocalVariable->getValue()->value().getIdentificator()),
		pValue->src_info()
	);
	ASSERT(pExpression);
	return pExpression;
}

CLOSE_RDO_PARSER_NAMESPACE

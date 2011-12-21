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
#include "simulator/runtime/calc/procedural/calc_locvar.h"
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

IContextFind::Result ContextMemory::onFindContext(CREF(LPRDOValue) pValue) const
{
	ASSERT(pValue);

	LPLocalVariable pLocalVariable = m_pLocalVariableListStack->findLocalVariable(pValue->value().getIdentificator());
	if (pLocalVariable)
	{
		LPExpression pExpression = rdo::Factory<Expression>::create(
			pLocalVariable->getExpression()->typeInfo(),
			rdo::Factory<rdoRuntime::RDOCalcGetLocalVariable>::create(pLocalVariable->getValue()->value().getIdentificator()),
			pValue->src_info()
		);
		ASSERT(pExpression);
		return IContextFind::Result(const_cast<PTR(ContextMemory)>(this), pExpression, pValue);
	}

	return IContextFind::Result();
}

CLOSE_RDO_PARSER_NAMESPACE

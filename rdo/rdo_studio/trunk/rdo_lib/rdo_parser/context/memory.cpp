/*
 * copyright: (c) RDO-Team, 2011
 * filename : memory.cpp
 * author   : Урусов Андрей
 * date     : 16.10.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/context/memory.h"
#include "rdo_lib/rdo_runtime/calc/locvar.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- ContextMemory
// ----------------------------------------------------------------------------
ContextMemory::ContextMemory()
{
	m_pLocalVariableListStack = rdo::Factory<LocalVariableListStack>::create();
	ASSERT(m_pLocalVariableListStack);
}

LPLocalVariableListStack ContextMemory::getLocalMemory()
{
	return m_pLocalVariableListStack;
}

LPContext ContextMemory::onFindContext(CREF(RDOValue) value) const
{
	LPLocalVariable pLocalVariable = m_pLocalVariableListStack->findLocalVariable(value->getIdentificator());
	if (pLocalVariable)
	{
		return const_cast<PTR(ContextMemory)>(this);
	}

	return NULL;
}

LPExpression ContextMemory::onCreateExpression(CREF(RDOValue) value)
{
	LPLocalVariable pLocalVariable = m_pLocalVariableListStack->findLocalVariable(value->getIdentificator());
	ASSERT(pLocalVariable);

	LPExpression pExpression = rdo::Factory<Expression>::create(
		pLocalVariable->getExpression()->typeInfo(),
		rdo::Factory<rdoRuntime::RDOCalcGetLocalVariable>::create(pLocalVariable->getValue()->getIdentificator()),
		value.src_info()
	);
	ASSERT(pExpression);
	return pExpression;
}

CLOSE_RDO_PARSER_NAMESPACE

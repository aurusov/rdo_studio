/*!
  \copyright (c) RDO-Team, 2012
  \file      local_variable_context.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      16.12.2012
  \brief     Контекст инициализации списка локальных переменных: integer i, j;
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/src/function/local_variable/local_variable_context.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

ContextLocalVariable::ContextLocalVariable(const OnPushLocalVariable& onPushLocalVariable)
	: m_onPushLocalVariable(onPushLocalVariable)
{
	m_pCalcLocalVariableList = rdo::Factory<rdo::runtime::RDOCalcLocalVariableList>::create();
	ASSERT(m_pCalcLocalVariableList);
}

ContextLocalVariable::~ContextLocalVariable()
{}

void ContextLocalVariable::pushLocalVariable(const LPLocalVariable& pLocalVariable)
{
	rdo::runtime::LPRDOCalcCreateLocalVariable pCalcCreateLocalVariable =
		rdo::Factory<rdo::runtime::RDOCalcCreateLocalVariable>::create(
			pLocalVariable->getName(),
			pLocalVariable->getExpression()->calc()
		);
	ASSERT(pCalcCreateLocalVariable);
	m_pCalcLocalVariableList->addCalcLocalVariable(pCalcCreateLocalVariable);

	m_onPushLocalVariable(pLocalVariable);
}

CREF(rdo::runtime::LPRDOCalcLocalVariableList) ContextLocalVariable::calc() const
{
	return m_pCalcLocalVariableList;
}

CLOSE_RDO_PARSER_NAMESPACE

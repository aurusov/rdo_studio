/*!
  \copyright (c) RDO-Team, 2011
  \file      local_variable.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      16.10.2010
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "rdo_lib/rdo_parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_lib/rdo_parser/local_variable.h"
#include "rdo_lib/rdo_parser/rdoparser_error.h"
#include "rdo_lib/rdo_runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- LocalVariable
// --------------------------------------------------------------------------------
LocalVariable::LocalVariable(CREF(RDOValue) value, CREF(LPExpression) pExpression, CREF(LPTypeInfo) pType)
	: m_value      (value      )
	, m_pExpression(pExpression)
	, m_pType      (pType      )
{}

CREF(RDOValue) LocalVariable::getValue() const
{
	return m_value;
}

CREF(LPExpression) LocalVariable::getExpression() const
{
	return m_pExpression;
}

CREF(LPTypeInfo) LocalVariable::getTypeInfo() const
{
	return m_pType;
}

// --------------------------------------------------------------------------------
// -------------------- LocalVariableList
// --------------------------------------------------------------------------------
LocalVariableList::LocalVariableList()
{}

void LocalVariableList::append(CREF(LPLocalVariable) pVariable)
{
	ASSERT(pVariable);

	STL_FOR_ALL_CONST(m_variableList, it)
	{
		if ((*it)->getValue()->getIdentificator() == pVariable->getValue()->getIdentificator())
		{
			rdoParse::g_error().push_only(pVariable->getValue().src_info(), rdo::format(_T("Переменная %s уже объявлена"), pVariable->getValue()->getIdentificator().c_str()));
			rdoParse::g_error().push_only((*it)->getValue().src_info(),     _T("См. первое описание"));
			rdoParse::g_error().push_done();
		}
	}

	m_variableList.push_back(pVariable);
}

LPLocalVariable LocalVariableList::findLocalVariable(CREF(tstring) paramName) const
{
	STL_FOR_ALL_CONST(m_variableList, LocalVariable_it)
	{
		if((*LocalVariable_it)->getValue()->getIdentificator() == paramName)
			return (*LocalVariable_it);
	}
	return NULL;
}

// --------------------------------------------------------------------------------
// -------------------- LocalVariableListStack
// --------------------------------------------------------------------------------
LocalVariableListStack::LocalVariableListStack()
{}

void LocalVariableListStack::push(CREF(LPLocalVariableList) pVariableList)
{
	m_pVariableListStack.push_back(pVariableList);
}

void LocalVariableListStack::pop()
{
	ASSERT(!m_pVariableListStack.empty());

	m_pVariableListStack.pop_back();
}

void LocalVariableListStack::append(CREF(LPLocalVariable) pVariable)
{
	ASSERT(!m_pVariableListStack.empty());

	m_pVariableListStack.back()->append(pVariable);
}

LPLocalVariable LocalVariableListStack::findLocalVariable(CREF(tstring) paramName) const
{
	ASSERT(!m_pVariableListStack.empty());

	VariableListStack::const_iterator stack_it = m_pVariableListStack.begin();
	while(stack_it != m_pVariableListStack.end())
	{
		if((*stack_it)->findLocalVariable(paramName)) return (*stack_it)->findLocalVariable(paramName);
		++stack_it;
	}
	return NULL;
}
CLOSE_RDO_PARSER_NAMESPACE

/*!
  \copyright (c) RDO-Team, 2011
  \file      local_variable.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      16.10.2010
  \brief     
  \indent    4T
*/

#ifndef _LOCAL_VARIABLE_H_
#define _LOCAL_VARIABLE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/expression.h"
#include "simulator/compiler/parser/type/info.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- LocalVariable
// --------------------------------------------------------------------------------
OBJECT(LocalVariable)
{
DECLARE_FACTORY(LocalVariable);
public:
	CREF(LPRDOValue)             getValue        () const;
	CREF(LPExpression)           getExpression   () const;
	CREF(LPTypeInfo)             getTypeInfo     () const;
	CREF(rdoRuntime::RDOValue)   getDefaultValue () const;

private:
	LocalVariable(CREF(LPRDOValue) pValue, CREF(LPExpression) pExpression, CREF(LPTypeInfo) pType);

	LPRDOValue           m_pValue;
	LPExpression         m_pExpression;
	LPTypeInfo           m_pType;
	rdoRuntime::RDOValue m_pDefaultValue;
};

// --------------------------------------------------------------------------------
// -------------------- LocalVariableList
// --------------------------------------------------------------------------------
OBJECT(LocalVariableList)
{
DECLARE_FACTORY(LocalVariableList);
public:
	typedef std::list<LPLocalVariable> VariableList;

	void            append           (CREF(LPLocalVariable) pVariable);
	LPLocalVariable findLocalVariable(CREF(tstring)         name     ) const;

private:
	LocalVariableList();

	VariableList m_variableList;
};

// --------------------------------------------------------------------------------
// -------------------- LocalVariableListStack
// --------------------------------------------------------------------------------
OBJECT(LocalVariableListStack)
{
	DECLARE_FACTORY(LocalVariableListStack);
public:
	typedef std::list<LPLocalVariableList> VariableListStack;

	void push(CREF(LPLocalVariableList) pVariableList);
	void pop ();

	void            append           (CREF(LPLocalVariable) pVariable);
	LPLocalVariable findLocalVariable(CREF(tstring)         name     ) const;

private:
	LocalVariableListStack();

	VariableListStack m_pVariableListStack;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _LOCAL_VARIABLE_H_

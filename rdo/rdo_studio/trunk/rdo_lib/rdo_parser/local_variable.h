/*
  \copyright (c) RDO-Team, 2011
  \file      local_variable.h
  \author    Урусов Андрей
  \date      16.10.2010
  \brief     
  \indent    4T
 */

#ifndef _LOCAL_VARIABLE_H_
#define _LOCAL_VARIABLE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_parser/expression.h"
#include "rdo_lib/rdo_parser/type/info.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- LocalVariable
// ----------------------------------------------------------------------------
OBJECT(LocalVariable)
{
DECLARE_FACTORY(LocalVariable);
public:
	CREF(RDOValue)     getValue     () const;
	CREF(LPExpression) getExpression() const;
	CREF(LPTypeInfo)   getTypeInfo  () const;

private:
	LocalVariable(CREF(RDOValue) value, CREF(LPExpression) pExpression, CREF(LPTypeInfo) pType);

	RDOValue      m_value;
	LPExpression  m_pExpression;
	LPTypeInfo    m_pType;
};

// ----------------------------------------------------------------------------
// ---------- LocalVariableList
// ----------------------------------------------------------------------------
OBJECT(LocalVariableList)
{
DECLARE_FACTORY(LocalVariableList);
public:
	typedef std::list<LPLocalVariable> VariableList;

	void            append           (CREF(LPLocalVariable) pVariable);
	LPLocalVariable findLocalVariable(CREF(tstring)         paramName) const;

private:
	LocalVariableList();

	VariableList m_variableList;
};

// ----------------------------------------------------------------------------
// ---------- LocalVariableListStack
// ----------------------------------------------------------------------------
OBJECT(LocalVariableListStack)
{
	DECLARE_FACTORY(LocalVariableListStack);
public:
	typedef std::list<LPLocalVariableList> VariableListStack;

	void push(CREF(LPLocalVariableList) pVariableList);
	void pop ();

	void            append           (CREF(LPLocalVariable) pVariable);
	LPLocalVariable findLocalVariable(CREF(tstring)         paramName) const;

private:
	LocalVariableListStack();

	VariableListStack m_pVariableListStack;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _LOCAL_VARIABLE_H_

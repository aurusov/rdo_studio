/*
 * copyright: (c) RDO-Team, 2010
 * filename : local_variable.h
 * author   : Урусов Андрей
 * date     : 16.10.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _LOCAL_VARIABLE_H_
#define _LOCAL_VARIABLE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_parser/rdofun.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- LocalVariable
// ----------------------------------------------------------------------------
OBJECT(LocalVariable)
{
DECLARE_FACTORY(LocalVariable);
public:
	CREF(RDOValue) getValue() const;

private:
	LocalVariable(CREF(RDOValue) value, CREF(LPRDOFUNArithm) pArithm);

	RDOValue        m_value;
	LPRDOFUNArithm  m_pArithm;
};

// ----------------------------------------------------------------------------
// ---------- LocalVariableList
// ----------------------------------------------------------------------------
OBJECT(LocalVariableList)
{
DECLARE_FACTORY(LocalVariableList);
public:
	void append(CREF(LPLocalVariable) pVariable);

private:
	LocalVariableList();

	typedef std::list<LPLocalVariable> VariableList;
	VariableList m_variableList;
};

// ----------------------------------------------------------------------------
// ---------- LocalVariableType
// ----------------------------------------------------------------------------
OBJECT(LocalVariableType)
{
	DECLARE_FACTORY(LocalVariableType);
public:
	void           rememberType(CREF(LPRDOTypeParam) pType);
	LPRDOTypeParam getType();

private:
	LocalVariableType();

	LPRDOTypeParam m_variableType;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _LOCAL_VARIABLE_H_

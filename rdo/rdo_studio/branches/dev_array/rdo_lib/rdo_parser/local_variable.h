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
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_parser/rdo_type_param.h"
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
	LocalVariable(CREF(RDOValue) value, CREF(rdoRuntime::LPRDOCalc) pCalc);

	RDOValue              m_value;
	rdoRuntime::LPRDOCalc m_pCalc;
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

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _LOCAL_VARIABLE_H_

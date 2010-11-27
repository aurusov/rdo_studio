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
	CREF(RDOValue) getValue( ) const;
	LPRDOFUNArithm getArithm() const;
	LPRDOTypeParam getParam( ) const;

private:
	LocalVariable(CREF(RDOValue) value, CREF(LPRDOFUNArithm) pArithm, CREF(LPRDOTypeParam) pParam);

	RDOValue       m_value  ;
	LPRDOFUNArithm m_pArithm;
	LPRDOTypeParam m_pParam ;
};

// ----------------------------------------------------------------------------
// ---------- LocalVariableList
// ----------------------------------------------------------------------------
OBJECT(LocalVariableList)
{
DECLARE_FACTORY(LocalVariableList);
public:
	typedef std::list<LPLocalVariable> VariableList;

	void          append(CREF(LPLocalVariable) pVariable);
	LPLocalVariable findLocalVariable(CREF(tstring) paramName) const;

private:
	LocalVariableList();

	VariableList m_variableList;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _LOCAL_VARIABLE_H_

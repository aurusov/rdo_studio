/*!
  \copyright (c) RDO-Team, 2011
  \file      variable_container.h
  \author    Чирков Михаил
  \date      02.12.2010
  \brief     
  \indent    4T
*/

#ifndef _VARIABLE_CONTAINER_H_
#define _VARIABLE_CONTAINER_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/compiler/parser/local_variable.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- VariableContainer
// --------------------------------------------------------------------------------
OBJECT(VariableContainer)
{
DECLARE_FACTORY(VariableContainer);
public:
	CREF(rdoRuntime::LPRDOCalc) getCalc        () const;
	CREF(LPLocalVariable)       getLocalVariable() const;

private:
	VariableContainer(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPLocalVariable) pLocalVariable);

	rdoRuntime::LPRDOCalc m_pCalc;
	LPLocalVariable       m_pLocalVariable;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _VARIABLE_CONTAINER_H_

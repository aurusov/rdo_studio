/*
 * copyright: (c) RDO-Team, 2010
 * filename : variable_container.h
 * author   : Чирков Михаил
 * date     : 02.12.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _VARIABLE_CONTAINER_H_
#define _VARIABLE_CONTAINER_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_parser/local_variable.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- VariableContainer
// ----------------------------------------------------------------------------
OBJECT(VariableContainer)
{
DECLARE_FACTORY(VariableContainer);
public:
	CREF(rdoRuntime::LPRDOCalc) getCalc        () const;
	CREF(LPLocalVariable)       getLocalVarible() const;

private:
	VariableContainer(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPLocalVariable) pLocalVarible);

	rdoRuntime::LPRDOCalc m_pCalc;
	LPLocalVariable       m_pLocalVarible;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _VARIABLE_CONTAINER_H_

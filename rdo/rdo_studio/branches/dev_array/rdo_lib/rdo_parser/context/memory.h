/*
 * copyright: (c) RDO-Team, 2010
 * filename : memory.h
 * author   : Урусов Андрей
 * date     : 16.10.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _CONEXT_MEMORY_H_
#define _CONEXT_MEMORY_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/local_variable.h"
#include "rdo_lib/rdo_parser/context/context.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- IContextMemory
// ----------------------------------------------------------------------------
S_INTERFACE(IContextMemory)
{
	virtual LPLocalVariableList getLocalMemory() = 0;
};
#define DECLARE_IContextMemory            \
public:                                   \
	LPLocalVariableList getLocalMemory();

// ----------------------------------------------------------------------------
// ---------- ContextMemory
// ----------------------------------------------------------------------------
CLASS(ContextMemory):
	    INSTANCE_OF      (Context       )
	AND IMPLEMENTATION_OF(IContextMemory)
{
DECLARE_FACTORY(ContextMemory);
protected:
	ContextMemory();

private:
	LPLocalVariableList m_pLocalVariableList;

	DECLARE_IContextMemory;
};
DECLARE_POINTER(ContextMemory);

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _CONEXT_MEMORY_H_

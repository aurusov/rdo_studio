/*!
  \copyright (c) RDO-Team, 2011
  \file      memory.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      16.10.2010
  \brief     
  \indent    4T
*/

#ifndef _CONEXT_MEMORY_H_
#define _CONEXT_MEMORY_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/src/function/local_variable/local_variable.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/context/context_find_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- IContextMemory
// --------------------------------------------------------------------------------
S_INTERFACE(IContextMemory)
{
	virtual LPLocalVariableListStack getLocalMemory() = 0;
};
#define DECLARE_IContextMemory               \
public:                                      \
	LPLocalVariableListStack getLocalMemory();

// --------------------------------------------------------------------------------
// -------------------- ContextMemory
// --------------------------------------------------------------------------------
CLASS(ContextMemory):
	    INSTANCE_OF      (Context       )
	AND IMPLEMENTATION_OF(IContextMemory)
	AND IMPLEMENTATION_OF(IContextFind  )
{
DECLARE_FACTORY(ContextMemory);
public:
	static void push();
	static void pop ();

protected:
	ContextMemory();

private:
	LPLocalVariableListStack m_pLocalVariableListStack;

	DECLARE_IContextMemory;
	DECLARE_IContextFind;
};
DECLARE_POINTER(ContextMemory);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _CONEXT_MEMORY_H_

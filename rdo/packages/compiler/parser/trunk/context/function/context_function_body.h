/*!
  \copyright (c) RDO-Team, 2012
  \file      context_function_body.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      08.12.2012
  \brief     Контекст тела функции
  \indent    4T
*/

#ifndef _SIMULATOR_COMPILER_PARSER_CONTEXT_FUNCTION_BODY_H_
#define _SIMULATOR_COMPILER_PARSER_CONTEXT_FUNCTION_BODY_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/interface_ptr.h"
#include "simulator/compiler/parser/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

OBJECT_INTERFACE(IContextFunctionBodyManager)
{
DECLARE_FACTORY(IContextFunctionBodyManager)
public:
	virtual void pushFunctionBodyContext() = 0;
	virtual void popFunctionBodyContext () = 0;

protected:
	IContextFunctionBodyManager()
	{}
	virtual ~IContextFunctionBodyManager()
	{}
};

#define DECLARE_IContextFunctionBodyManager \
	void pushFunctionBodyContext();         \
	void popFunctionBodyContext ();

CLOSE_RDO_PARSER_NAMESPACE

#endif // _SIMULATOR_COMPILER_PARSER_CONTEXT_FUNCTION_BODY_H_

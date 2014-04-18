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
#include <boost/function.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/interface_ptr/interface_ptr.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/compiler/parser/namespace.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/context/statement.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- IContextFunctionBodyManager
// --------------------------------------------------------------------------------
PREDECLARE_OBJECT_INTERFACE(IContextFunctionBodyManager)
struct IContextFunctionBodyManager: public rdo::RefCounter<IContextFunctionBodyManager>
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

// --------------------------------------------------------------------------------
// -------------------- ContextFunctionBody
// --------------------------------------------------------------------------------
class ContextFunctionBody: public ContextReturnable
{
DECLARE_FACTORY(ContextFunctionBody)
public:
	void setBody(const rdo::runtime::LPRDOCalc& pBody);

	void pushContext();
	void popContext ();

private:
	typedef  boost::function<void (const rdo::runtime::LPRDOCalc&)>  OnBody;

	ContextFunctionBody(const OnBody& onBody);
	virtual ~ContextFunctionBody();

	OnBody m_onBody;
};
DECLARE_POINTER(ContextFunctionBody);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _SIMULATOR_COMPILER_PARSER_CONTEXT_FUNCTION_BODY_H_

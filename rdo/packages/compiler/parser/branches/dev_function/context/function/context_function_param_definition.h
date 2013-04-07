/*!
  \copyright (c) RDO-Team, 2012
  \file      context_param_definition.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      08.12.2012
  \brief     Контекст описания параметров функции
  \indent    4T
*/

#ifndef _SIMULATOR_COMPILER_PARSER_CONTEXT_FUNCTION_PARAM_DEFINITION_H_
#define _SIMULATOR_COMPILER_PARSER_CONTEXT_FUNCTION_PARAM_DEFINITION_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/function.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/param.h"
#include "simulator/compiler/parser/context/context.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- IContextParamDefinitionManager
// --------------------------------------------------------------------------------
OBJECT_INTERFACE(IContextParamDefinitionManager)
{
DECLARE_FACTORY(IContextParamDefinitionManager)
public:
	virtual void pushParamDefinitionContext() = 0;
	virtual void popParamDefinitionContext () = 0;

protected:
	IContextParamDefinitionManager()
	{}
	virtual ~IContextParamDefinitionManager()
	{}
};

#define DECLARE_IContextParamDefinitionManager \
	void pushParamDefinitionContext();         \
	void popParamDefinitionContext ();

// --------------------------------------------------------------------------------
// -------------------- ContextParamDefinition
// --------------------------------------------------------------------------------
class ContextParamDefinition: public Context
{
DECLARE_FACTORY(ContextParamDefinition)
public:
	void pushParam(const LPRDOParam& pParam);

private:
	typedef  boost::function<void (const LPRDOParam&)>  OnPushParam;

	ContextParamDefinition(const OnPushParam& onPushParam);
	virtual ~ContextParamDefinition();

	OnPushParam m_onPushParam;
};
DECLARE_POINTER(ContextParamDefinition);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _SIMULATOR_COMPILER_PARSER_CONTEXT_FUNCTION_PARAM_DEFINITION_H_

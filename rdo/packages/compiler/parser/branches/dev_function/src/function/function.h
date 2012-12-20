/*!
  \copyright (c) RDO-Team, 2012
  \file      function.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.12.2012
  \brief     РДО-функция на уровне парсера
  \indent    4T
*/

#ifndef _SIMULATOR_COMPILER_PARSER_FUNCTION_H_
#define _SIMULATOR_COMPILER_PARSER_FUNCTION_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/optional.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/compiler/parser/context/memory.h"
#include "simulator/compiler/parser/context/function/context_function_param_definition.h"
#include "simulator/compiler/parser/context/function/context_function_body.h"
#include "simulator/compiler/parser/type/function_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

class Function
	: public IContextParamDefinitionManager
	, public IContextFunctionBodyManager
	, public Context
	, public IContextFind
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(Function);
public:
	typedef boost::optional<ruint>  ParamID;
	typedef std::vector<LPRDOParam> ParamList;

	LPRDOParam findParam  (CREF(tstring) paramName) const;
	ParamID    findParamID(CREF(tstring) paramName) const;

	CREF(ParamList) getParams() const;
	void setCall(CREF(rdo::runtime::LPRDOCalc) pCalc);

	LPExpression expression() const;

protected:
	Function(CREF(LPTypeInfo) pReturnType, CREF(RDOParserSrcInfo) srcInfo);
	virtual ~Function();

	void pushContext();
	void popContext ();

	DECLARE_IContextFind;

private:
	LPFunctionType           m_pFunctionType;
	LPTypeInfo               m_pReturnType;
	ParamList                m_paramList;
	LPContextMemory          m_pContextMemory;
	rdo::runtime::LPRDOCalc  m_pCallpCalc;

	void onPushParam(CREF(LPRDOParam) pParam);

	ParamList::const_iterator find(CREF(tstring) paramName) const;
	LPFunctionType generateType() const;

	DECLARE_IContextParamDefinitionManager;
	DECLARE_IContextFunctionBodyManager;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _SIMULATOR_COMPILER_PARSER_FUNCTION_H_

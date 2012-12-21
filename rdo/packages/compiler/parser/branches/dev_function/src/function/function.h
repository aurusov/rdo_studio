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

	void pushContext();
	void popContext ();

	LPRDOParam findParam  (CREF(tstring) paramName) const;
	ParamID    findParamID(CREF(tstring) paramName) const;

	CREF(ParamList) getParams() const;

	LPExpression expression() const;

protected:
	Function(CREF(LPTypeInfo) pReturnType, CREF(RDOParserSrcInfo) srcInfo);
	virtual ~Function();

	DECLARE_IContextFind;

private:
	LPFunctionType           m_pFunctionType;
	LPTypeInfo               m_pReturnType;
	ParamList                m_paramList;
	LPContextFunctionBody    m_pContextFunctionBody;
	rdo::runtime::LPRDOCalc  m_pBody;

	void onPushParam(CREF(LPRDOParam) pParam);
	void setBody    (CREF(rdo::runtime::LPRDOCalc) pBody);

	ParamList::const_iterator find(CREF(tstring) paramName) const;
	LPFunctionType generateType() const;

	DECLARE_IContextParamDefinitionManager;
	DECLARE_IContextFunctionBodyManager;
};
DECLARE_POINTER(Function);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _SIMULATOR_COMPILER_PARSER_FUNCTION_H_

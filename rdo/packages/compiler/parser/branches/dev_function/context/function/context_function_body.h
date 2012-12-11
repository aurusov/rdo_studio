/*!
  \copyright (c) RDO-Team, 2012
  \file      context_param_definition.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      08.12.2012
  \brief     Контекст описания параметров функции
  \indent    4T
*/

#ifndef _SIMULATOR_COMPILER_PARSER_CONTEXT_PARAM_DEFINITION_H_
#define _SIMULATOR_COMPILER_PARSER_CONTEXT_PARAM_DEFINITION_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/optional.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/param.h"
#include "simulator/compiler/parser/context/context.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

class ContextParamDefinition: public Context
{
DECLARE_FACTORY(ContextParamDefinition)
public:
	typedef boost::optional<ruint> ParamID;

	void       pushParamBegin();
	void       pushParam     (CREF(LPRDOParam) pParam);
	void       pushParamEnd  ();

	LPRDOParam findParam  (CREF(tstring) paramName) const;
	ParamID    findParamID(CREF(tstring) paramName) const;

private:
	typedef std::vector<LPRDOParam> ParamList;

	ParamList m_paramList;
};
DECLARE_POINTER(ContextParamDefinition);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _SIMULATOR_COMPILER_PARSER_CONTEXT_PARAM_DEFINITION_H_

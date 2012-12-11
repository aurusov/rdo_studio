/*!
  \copyright (c) RDO-Team, 2012
  \file      context_param_definition.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      08.12.2012
  \brief     Контекст описания параметров функции
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/context/context_param_definition.h"
#include "simulator/compiler/parser/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

void ContextParamDefinition::pushParamBegin()
{
	RDOParser::s_parser()->contextStack()->push(this);
}

void ContextParamDefinition::pushParamEnd()
{
	ASSERT(RDOParser::s_parser()->context()->cast<ContextParamDefinition>() == LPContextParamDefinition(this));
	RDOParser::s_parser()->contextStack()->pop();
}

void ContextParamDefinition::pushParam(CREF(LPRDOParam) pParam)
{
	ASSERT(pParam);
	LPRDOParam pParamPrev = findParam(pParam->name());
	if (pParamPrev)
	{
		RDOParser::s_parser()->error().push_only(pParam->src_info(), rdo::format(_T("Параметр уже определён: %s"), pParam->name().c_str()));
		RDOParser::s_parser()->error().push_only(pParamPrev->src_info(), _T("См. первое определение"));
		RDOParser::s_parser()->error().push_done();
	}
	m_paramList.push_back(pParam); 
}

LPRDOParam ContextParamDefinition::findParam(CREF(tstring) paramName) const
{
	ParamList::const_iterator it = std::find_if(m_paramList.begin(), m_paramList.end(), compareName<RDOParam>(paramName));

	return it != m_paramList.end()
		? *it
		: LPRDOParam(NULL);
}

ContextParamDefinition::ParamID ContextParamDefinition::findParamID(CREF(tstring) paramName) const
{
	ParamList::const_iterator it = std::find_if(m_paramList.begin(), m_paramList.end(), compareName<RDOParam>(paramName));

	return it != m_paramList.end()
		? it - m_paramList.begin()
		: ParamID();
}

CLOSE_RDO_PARSER_NAMESPACE

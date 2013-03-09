/*!
  \copyright (c) RDO-Team, 2012
  \file      function.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.12.2012
  \brief     РДО-функция на уровне парсера
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/range/algorithm/find_if.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/src/function/function.h"
#include "simulator/compiler/parser/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

Function::Function(CREF(LPTypeInfo) pReturnType, CREF(RDOParserSrcInfo) srcInfo)
	: RDOParserSrcInfo(srcInfo)
	, m_pReturnType(pReturnType)
{
	ASSERT(m_pReturnType);
}

Function::~Function()
{}

void Function::setCall(CREF(rdo::runtime::LPRDOCalc) pCalc)
{
	ASSERT(!m_pCallpCalc);
	ASSERT(pCalc);
	m_pCallpCalc = pCalc;
}

LPExpression Function::expression() const
{
	ASSERT(m_pCallpCalc);
	LPExpression pExpression = rdo::Factory<Expression>::create(
		rdo::Factory<TypeInfo>::create(m_pFunctionType, m_pFunctionType->src_info()),
		m_pCallpCalc,
		src_info()
	);
	ASSERT(pExpression);
	return pExpression;
}

void Function::pushContext()
{
	RDOParser::s_parser()->contextStack()->push(this);
}

void Function::popContext()
{
	RDOParser::s_parser()->contextStack()->pop();
}

void Function::pushParamDefinitionContext()
{
	LPContextParamDefinition pContextParamDefinition = rdo::Factory<ContextParamDefinition>::create(
		boost::bind(&Function::onPushParam, this, _1)
	);
	ASSERT(pContextParamDefinition);
	RDOParser::s_parser()->contextStack()->push(pContextParamDefinition);
}

void Function::popParamDefinitionContext()
{
	ASSERT(RDOParser::s_parser()->context().object_dynamic_cast<ContextParamDefinition>());
	RDOParser::s_parser()->contextStack()->pop();

	m_pFunctionType = generateType();
	ASSERT(m_pFunctionType);
}

void Function::onPushParam(CREF(LPRDOParam) pParam)
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

LPRDOParam Function::findParam(CREF(tstring) paramName) const
{
	ParamList::const_iterator it = find(paramName);

	return it != m_paramList.end()
		? *it
		: LPRDOParam(NULL);
}

Function::ParamID Function::findParamID(CREF(tstring) paramName) const
{
	ParamList::const_iterator it = find(paramName);

	return it != m_paramList.end()
		? it - m_paramList.begin()
		: ParamID();
}

Function::ParamList::const_iterator Function::find(CREF(tstring) paramName) const
{
	return boost::range::find_if(m_paramList, compareName<RDOParam>(paramName));
}

LPFunctionType Function::generateType() const
{
	ASSERT(m_pReturnType);

	FunctionParamType::ParamList paramTypeList;
	BOOST_FOREACH(const LPRDOParam& pParam, m_paramList)
	{
		paramTypeList.push_back(pParam->getTypeInfo());
	}
	LPFunctionParamType pParamType = rdo::Factory<FunctionParamType>::create(paramTypeList, src_info());
	ASSERT(pParamType);

	return rdo::Factory<FunctionType>::create(m_pReturnType, pParamType, src_info());
}

void Function::pushFunctionBodyContext()
{
	ASSERT(!m_pContextMemory);
	m_pContextMemory = rdo::Factory<ContextMemory>::create();
	ASSERT(m_pContextMemory);
	RDOParser::s_parser()->contextStack()->push(m_pContextMemory);

	ContextMemory::push();
}

void Function::popFunctionBodyContext()
{
	ContextMemory::pop();
	RDOParser::s_parser()->contextStack()->pop();
}

Context::FindResult Function::onFindContext(CREF(LPRDOValue) pValue) const
{
	ASSERT(pValue);

	LPRDOParam pParam = findParam(pValue->value().getIdentificator());
	if (pParam)
	{
		rdo::runtime::RDOType::TypeID typeID = pParam->getTypeInfo()->type()->typeID();
		if (typeID == rdo::runtime::RDOType::t_identificator || typeID == rdo::runtime::RDOType::t_unknow)
		{
			RDOParser::s_parser()->error().push_only(
				pValue->src_info(),
				rdo::format(_T("Тип параметра '%s' определён неверно"), pValue->src_info().src_text().c_str())
			);
			RDOParser::s_parser()->error().push_only(pParam->getTypeInfo()->src_info(), _T("См. описание типа"));
			RDOParser::s_parser()->error().push_done();
		}
		ParamID paramID = findParamID(pValue->value().getIdentificator());
		ASSERT(paramID.is_initialized());
		LPExpression pExpression = rdo::Factory<Expression>::create(
			pParam->getTypeInfo(),
			rdo::Factory<rdo::runtime::RDOCalcFuncParam>::create(*paramID, pParam->src_info()),
			pValue->src_info()
		);
		ASSERT(pExpression);
		return Context::FindResult(const_cast<PTR(Function)>(this), pExpression, pValue, pParam);
	}

	return Context::FindResult();
}

CLOSE_RDO_PARSER_NAMESPACE

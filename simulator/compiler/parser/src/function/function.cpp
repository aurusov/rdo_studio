// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
#include <boost/range/algorithm/find_if.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_statement.h"
#include "simulator/runtime/calc/procedural/calc_braces.h"
#include "simulator/compiler/parser/src/function/function.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/context/function/context_function_body.h"
#include "simulator/compiler/parser/context/statement.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

Function::Function(const LPTypeInfo& pReturnType, const RDOParserSrcInfo& srcInfo)
	: RDOParserSrcInfo(srcInfo)
	, m_pReturnType(pReturnType)
{
	ASSERT(m_pReturnType);
}

Function::~Function()
{}

LPExpression Function::expression() const
{
	ASSERT(m_pBody);
	LPExpression pExpression = rdo::Factory<Expression>::create(
		rdo::Factory<TypeInfo>::create(m_pFunctionType, m_pFunctionType->src_info()),
		m_pBody,
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
	RDOParser::s_parser()->contextStack()->pop<Function>();
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
	RDOParser::s_parser()->contextStack()->pop<ContextParamDefinition>();

	m_pFunctionType = generateType();
	ASSERT(m_pFunctionType);
}

void Function::onPushParam(const LPRDOParam& pParam)
{
	ASSERT(pParam);
	LPRDOParam pParamPrev = findParam(pParam->name());
	if (pParamPrev)
	{
		RDOParser::s_parser()->error().push_only(pParam->src_info(), rdo::format("Параметр уже определён: %s", pParam->name().c_str()));
		RDOParser::s_parser()->error().push_only(pParamPrev->src_info(), "См. первое определение");
		RDOParser::s_parser()->error().push_done();
	}
	m_paramList.push_back(pParam);
}

LPRDOParam Function::findParam(const std::string& paramName) const
{
	ParamList::const_iterator it = find(paramName);

	return it != m_paramList.end()
		? *it
		: LPRDOParam(NULL);
}

Function::ParamID Function::findParamID(const std::string& paramName) const
{
	ParamList::const_iterator it = find(paramName);

	return it != m_paramList.end()
		? it - m_paramList.begin()
		: ParamID();
}

Function::ParamList::const_iterator Function::find(const std::string& paramName) const
{
	return boost::range::find_if(m_paramList, compareName<RDOParam>(paramName));
}

const Function::ParamList& Function::getParams() const
{
	return m_paramList;
}

LPFunctionType Function::generateType() const
{
	ASSERT(m_pReturnType);

	FunctionParamType::ParamList paramTypeList;
	for (const LPRDOParam& pParam: m_paramList)
	{
		paramTypeList.push_back(pParam->getTypeInfo());
	}

	if (paramTypeList.empty())
	{
		paramTypeList.push_back(
			rdo::Factory<TypeInfo>::delegate<RDOType__void>(src_info())
		);
	}

	LPFunctionParamType pParamType = rdo::Factory<FunctionParamType>::create(paramTypeList, src_info());
	ASSERT(pParamType);

	return rdo::Factory<FunctionType>::create(m_pReturnType, pParamType, src_info());
}

void Function::pushFunctionBodyContext()
{
	ASSERT(!m_pContextFunctionBody);
	m_pContextFunctionBody = rdo::Factory<ContextFunctionBody>::create(
		boost::bind(&Function::setBody, this, _1)
	);
	ASSERT(m_pContextFunctionBody);
	RDOParser::s_parser()->contextStack()->push(m_pContextFunctionBody);
	m_pContextFunctionBody->pushContext();
}

void Function::popFunctionBodyContext()
{
	ASSERT(m_pContextFunctionBody);

	if (m_pReturnType->typeID() != rdo::runtime::RDOType::t_void)
	{
		if (!m_pContextFunctionBody->getReturnFlag())
		{
			RDOParser::s_parser()->error().warning(
				src_info(),
				rdo::format("Возможно, не все ветки функции '%s' могут вернуть значение.", src_text().c_str())
			);
		}
	}

	m_pContextFunctionBody->popContext();
	RDOParser::s_parser()->contextStack()->pop<ContextFunctionBody>();
	m_pContextFunctionBody = NULL;
}

void Function::setBody(const rdo::runtime::LPRDOCalc& pBody)
{
	ASSERT(!m_pBody);
	ASSERT(pBody);

	rdo::runtime::LPRDOCalcBaseStatementList pCalcStatementList =
		rdo::Factory<rdo::runtime::RDOCalcBaseStatementList>::create();
	ASSERT(pCalcStatementList);

	rdo::runtime::LPRDOCalcOpenBrace pCalcOpenBrace = rdo::Factory<rdo::runtime::RDOCalcOpenBrace>::create();
	ASSERT(pCalcOpenBrace);

	rdo::runtime::LPRDOCalcCloseBrace pCalcCloseBrace = rdo::Factory<rdo::runtime::RDOCalcCloseBrace>::create();
	ASSERT(pCalcCloseBrace);

	pCalcStatementList->addCalcStatement(pCalcOpenBrace);
	pCalcStatementList->addCalcStatement(pBody);
	pCalcStatementList->addCalcStatement(pCalcCloseBrace);

	if (m_pReturnType->typeID() != rdo::runtime::RDOType::t_void && !m_pContextFunctionBody->getReturnFlag())
	{
		rdo::runtime::LPRDOCalc pCalcDefault = m_pDefaultValue;
		if (!pCalcDefault)
		{
			//! Присвоить автоматическое значение по умолчанию, если оно не задано в явном виде
			pCalcDefault = rdo::Factory<rdo::runtime::RDOCalcConst>::create(m_pReturnType->itype()->get_default());
			ASSERT(pCalcDefault);
			pCalcDefault->setSrcInfo(m_pReturnType->src_info());
		}
		rdo::runtime::LPRDOCalc pCalcReturn = rdo::Factory<rdo::runtime::RDOCalcFunReturn>::create(pCalcDefault);
		ASSERT(pCalcReturn);

		rdo::runtime::LPRDOCalcBaseStatementList pBodyStatementList =
			pBody.object_dynamic_cast<rdo::runtime::RDOCalcBaseStatementList>();
		ASSERT(pBodyStatementList);
		pBodyStatementList->addCalcStatement(pCalcReturn);
	}

	rdo::runtime::LPRDOCalcReturnCatch pCalcReturnCatch =
		rdo::Factory<rdo::runtime::RDOCalcReturnCatch>::create();
	ASSERT(pCalcReturnCatch);
	pCalcReturnCatch->setTryCalc(pCalcStatementList);

	m_pBody = pCalcReturnCatch;
}

void Function::setDefaultCalc(const rdo::runtime::LPRDOCalc& pDefaultValue)
{
	ASSERT(pDefaultValue);
	ASSERT(!m_pDefaultValue);
	m_pDefaultValue = pDefaultValue;
}

namespace
{

LPExpression contextParameter(const LPRDOParam& param, std::size_t paramID, const RDOParserSrcInfo& srcInfo)
{
	return rdo::Factory<Expression>::create(
		param->getTypeInfo(),
		rdo::Factory<rdo::runtime::RDOCalcFuncParam>::create(paramID, param->src_info()),
		srcInfo
	);
}

}

Context::LPFindResult Function::onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const
{
	if (method == Context::METHOD_GET || method == Context::METHOD_SET || method == Context::METHOD_OPERATOR_DOT)
	{
		const std::string identifier = params.identifier();

		LPRDOParam pParam = findParam(identifier);
		if (pParam)
		{
			rdo::runtime::RDOType::TypeID typeID = pParam->getTypeInfo()->typeID();
			if (typeID == rdo::runtime::RDOType::t_identificator || typeID == rdo::runtime::RDOType::t_unknow)
			{
				RDOParser::s_parser()->error().push_only(
					srcInfo,
					rdo::format("Тип параметра '%s' определён неверно", identifier.c_str())
				);
				RDOParser::s_parser()->error().push_only(pParam->getTypeInfo()->src_info(), "См. описание типа");
				RDOParser::s_parser()->error().push_done();
			}
			ParamID paramID = findParamID(identifier);
			ASSERT(paramID.is_initialized());

			if (method == Context::METHOD_GET)
			{
				return rdo::Factory<FindResult>::create(CreateExpression(boost::bind(&contextParameter, pParam, *paramID, srcInfo)));
			}
			else if (method == Context::METHOD_OPERATOR_DOT)
			{
				LPRDORTPResType resourceType = pParam->getTypeInfo()->itype().object_dynamic_cast<RDORTPResType>();
				if (resourceType)
				{
					Context::Params params_;
					params_[RDORSSResource::GET_RESOURCE] = contextParameter(pParam, *paramID, srcInfo);
					params_[Context::Params::IDENTIFIER] = identifier;
					return rdo::Factory<FindResult>::create(SwitchContext(resourceType, params_));
				}
				else
				{
					LPFunction pThis(const_cast<Function*>(this));
					return rdo::Factory<FindResult>::create(SwitchContext(pThis, params));
				}
			}
			else
			{
				ASSERT(method == Context::METHOD_SET);
				RDOParser::s_parser()->error().error(srcInfo, rdo::format("Функция не может изменить свой параметр: %s", identifier.c_str()));
			}
		}
	}

	return rdo::Factory<FindResult>::create();
}

CLOSE_RDO_PARSER_NAMESPACE

/*!
  \copyright (c) RDO-Team, 2011
  \file      param.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.01.2011
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/param.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/type/range.h"
#include "simulator/runtime/rdo_resource.h"
#include "simulator/runtime/calc/resource/calc_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

const std::string RDOParam::CONTEXT_PARAM_PARAM_ID = "param_id";

RDOParam::RDOParam(CREF(tstring) name, CREF(LPTypeInfo) pType, CREF(LPRDOValue) pDefault)
	: RDOParserSrcInfo(name    )
	, m_pType         (pType   )
	, m_pDefault      (pDefault)
{
	checkDefault();
}

RDOParam::RDOParam(CREF(RDOParserSrcInfo) srcInfo, CREF(LPTypeInfo) pType, CREF(LPRDOValue) pDefault)
	: RDOParserSrcInfo(srcInfo )
	, m_pType         (pType   )
	, m_pDefault      (pDefault)
{
	checkDefault();
}

RDOParam::~RDOParam()
{}

void RDOParam::checkDefault()
{
	if (m_pDefault && m_pDefault->defined())
	{
		m_pType->type()->type_cast(m_pDefault->typeInfo()->type(), m_pDefault->src_info(), this->src_info(), m_pDefault->src_info());
		m_pDefault = m_pType->type()->value_cast(m_pDefault, this->src_info(), m_pDefault->src_info());
	}
}

namespace
{

LPExpression contextGetParam(const rdo::runtime::LPRDOCalc& resource, ruint paramID, const LPTypeInfo& paramType, const RDOParserSrcInfo& srcInfo)
{
	return rdo::Factory<Expression>::create(
		paramType,
		rdo::Factory<rdo::runtime::RDOCalcGetResourceParam>::create(resource, paramID),
		srcInfo
	);
}

template <rdo::runtime::SetOperationType::Type setOperationType>
LPExpression contextSetParam(const rdo::runtime::LPRDOCalc& getResource, const RDOParam* param, const ruint paramID, const rdo::runtime::LPRDOCalc& rightValue, const RDOParserSrcInfo& srcInfo)
{	
	rdo::runtime::LPRDOCalc setParamCalc = rdo::Factory<rdo::runtime::RDOSetResourceParam<setOperationType> >::create(getResource, paramID, rightValue);
	
	LPRDOTypeIntRange pTypeIntRange = param->getTypeInfo()->type().object_dynamic_cast<RDOTypeIntRange>();
	if (pTypeIntRange)
	{
		setParamCalc = rdo::Factory<rdo::runtime::RDOCalcCheckRange>::create(pTypeIntRange->range()->getMin()->value(), pTypeIntRange->range()->getMax()->value(), setParamCalc);
	}

	LPRDOTypeRealRange pTypeRealRange = param->getTypeInfo()->type().object_dynamic_cast<RDOTypeRealRange>();
	if (pTypeRealRange)
	{
		setParamCalc = rdo::Factory<rdo::runtime::RDOCalcCheckRange>::create(pTypeRealRange->range()->getMin()->value(), pTypeRealRange->range()->getMax()->value(), setParamCalc);
	}
		
		return rdo::Factory<Expression>::create(
		param->getTypeInfo(),
		setParamCalc,
		srcInfo
	);

}

}

Context::FindResult RDOParam::onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const
{
	if (method == Context::METHOD_GET)
	{
		LPExpression resource = params.get<LPExpression>(RDORSSResource::CONTEXT_PARAM_RESOURCE_EXPRESSION);
		const ruint paramID = params.get<ruint>(RDOParam::CONTEXT_PARAM_PARAM_ID);
		return FindResult(CreateExpression(boost::bind(&contextGetParam, resource->calc(), paramID, getTypeInfo(), srcInfo)));
	}

	if (method == Context::METHOD_SET)
	{
		const rdo::runtime::LPRDOCalc rightValue = params.exists(Expression::CONTEXT_PARAM_SET_EXPRESSION)
				? params.get<LPExpression>(Expression::CONTEXT_PARAM_SET_EXPRESSION)->calc()
				: params.get<LPRDOFUNArithm>(RDOFUNArithm::CONTEXT_PARAM_SET_ARITHM)->createCalc(getTypeInfo());
		const ruint paramID = params.get<ruint>(RDOParam::CONTEXT_PARAM_PARAM_ID);
		const rdo::runtime::LPRDOCalc getResource = params.get<rdo::runtime::LPRDOCalc>("getResource()");

		using namespace rdo::runtime;
		switch (params.get<SetOperationType::Type>(Expression::CONTEXT_PARAM_SET_OPERATION_TYPE))
			{
			case SetOperationType::NOCHANGE   : return FindResult(CreateExpression(boost::bind(&contextSetParam<SetOperationType::NOCHANGE>, getResource, this, paramID, rightValue, srcInfo)));
			case SetOperationType::SET        : return FindResult(CreateExpression(boost::bind(&contextSetParam<SetOperationType::SET>, getResource, this, paramID, rightValue, srcInfo)));
			case SetOperationType::ADDITION   : return FindResult(CreateExpression(boost::bind(&contextSetParam<SetOperationType::ADDITION>, getResource, this, paramID, rightValue, srcInfo)));
			case SetOperationType::SUBTRACTION: return FindResult(CreateExpression(boost::bind(&contextSetParam<SetOperationType::SUBTRACTION>, getResource, this, paramID, rightValue, srcInfo)));
			case SetOperationType::MULTIPLY   : return FindResult(CreateExpression(boost::bind(&contextSetParam<SetOperationType::MULTIPLY>, getResource, this, paramID, rightValue, srcInfo)));
			case SetOperationType::DIVIDE     : return FindResult(CreateExpression(boost::bind(&contextSetParam<SetOperationType::DIVIDE>, getResource, this, paramID, rightValue, srcInfo)));
			case SetOperationType::INCREMENT  : return FindResult(CreateExpression(boost::bind(&contextSetParam<SetOperationType::INCREMENT>, getResource, this, paramID, rightValue, srcInfo)));
			case SetOperationType::DECRIMENT  : return FindResult(CreateExpression(boost::bind(&contextSetParam<SetOperationType::DECRIMENT>, getResource, this, paramID, rightValue, srcInfo)));
			default: return FindResult();
		}
	}

	return FindResult();
}

CLOSE_RDO_PARSER_NAMESPACE

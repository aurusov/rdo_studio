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
#include "simulator/runtime/calc/procedural/calc_const.h"
#include "simulator/runtime/calc/operation/calc_arithm.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

const std::string RDOParam::CONTEXT_PARAM_PARAM_ID = "param_id";

RDOParam::RDOParam(const std::string& name, const LPTypeInfo& pType, const LPRDOValue& pDefault)
	: RDOParserSrcInfo(name    )
	, m_pType         (pType   )
	, m_pDefault      (pDefault)
	, m_defined       (false)
{
	checkDefault();
}

RDOParam::RDOParam(const RDOParserSrcInfo& srcInfo, const LPTypeInfo& pType, const LPRDOValue& pDefault)
	: RDOParserSrcInfo(srcInfo )
	, m_pType         (pType   )
	, m_pDefault      (pDefault)
	, m_defined       (false)
{
	checkDefault();
}

RDOParam::~RDOParam()
{}

void RDOParam::checkDefault()
{
	if (m_pDefault && m_pDefault->defined())
	{
		m_pType->itype()->type_cast(m_pDefault->typeInfo()->itype(), m_pDefault->src_info(), this->src_info(), m_pDefault->src_info());
		m_pDefault = m_pType->itype()->value_cast(m_pDefault, this->src_info(), m_pDefault->src_info());
	}
}

namespace
{

LPExpression contextGetParam(const rdo::runtime::LPRDOCalc& resource, std::size_t paramID, const LPTypeInfo& paramType, const RDOParserSrcInfo& srcInfo)
{
	return rdo::Factory<Expression>::create(
		paramType,
		rdo::Factory<rdo::runtime::RDOCalcGetResourceParam>::create(resource, paramID),
		srcInfo
	);
}

template <rdo::runtime::SetOperationType::Type setOperationType>
LPExpression contextSetParam(const rdo::runtime::LPRDOCalc& getResource, const LPTypeInfo& pTypeInfo, const std::size_t paramID, const rdo::runtime::LPRDOCalc& rightValue, const RDOParserSrcInfo& srcInfo)
{
	rdo::runtime::LPRDOCalc setParamCalc = rdo::Factory<rdo::runtime::RDOSetResourceParam<setOperationType> >::create(getResource, paramID, rightValue);

	LPRDOTypeIntRange pTypeIntRange = pTypeInfo->itype().object_dynamic_cast<RDOTypeIntRange>();
	if (pTypeIntRange)
	{
		setParamCalc = rdo::Factory<rdo::runtime::RDOCalcCheckRange>::create(pTypeIntRange->range()->getMin()->value(), pTypeIntRange->range()->getMax()->value(), setParamCalc);
	}

	LPRDOTypeRealRange pTypeRealRange = pTypeInfo->itype().object_dynamic_cast<RDOTypeRealRange>();
	if (pTypeRealRange)
	{
		setParamCalc = rdo::Factory<rdo::runtime::RDOCalcCheckRange>::create(pTypeRealRange->range()->getMin()->value(), pTypeRealRange->range()->getMax()->value(), setParamCalc);
	}

	return rdo::Factory<Expression>::create(
		pTypeInfo,
		setParamCalc,
		srcInfo
	);

}

}

Context::LPFindResult RDOParam::onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const
{
	if (method == Context::METHOD_GET)
	{
			LPExpression resource = params.get<LPExpression>(RDORSSResource::GET_RESOURCE);
			const std::size_t paramID = params.get<std::size_t>(RDOParam::CONTEXT_PARAM_PARAM_ID);
			return rdo::Factory<FindResult>::create(CreateExpression(boost::bind(&contextGetParam, resource->calc(), paramID, getTypeInfo(), srcInfo)));
	}

	if (method == Context::METHOD_SET)
	{
		using namespace rdo::runtime;
		const LPExpression resource = params.get<LPExpression>(RDORSSResource::GET_RESOURCE);
		const std::size_t paramID = params.get<std::size_t>(RDOParam::CONTEXT_PARAM_PARAM_ID);
		const LPRDOCalc rightValue = params.exists(Expression::CONTEXT_PARAM_SET_EXPRESSION)
				? params.get<LPExpression>(Expression::CONTEXT_PARAM_SET_EXPRESSION)->calc()
				: params.get<LPRDOFUNArithm>(RDOFUNArithm::CONTEXT_PARAM_SET_ARITHM)->createCalc(getTypeInfo());

		const LPRDOCalc paramValue = rdo::Factory<FindResult>::create(CreateExpression
			(boost::bind(&contextGetParam, resource->calc(), paramID, getTypeInfo(), srcInfo))
			)->getCreateExpression()()->calc();

		LPRDOCalc operationResult;

		if (params.get<SetOperationType::Type>(Expression::CONTEXT_PARAM_SET_OPERATION_TYPE) == SetOperationType::SET)
		{
			LPRDOParam pThis(const_cast<RDOParam*>(this));
			pThis->setDefined(true);
		}

		switch (params.get<SetOperationType::Type>(Expression::CONTEXT_PARAM_SET_OPERATION_TYPE))
		{
		case SetOperationType::NOCHANGE   :
			return rdo::Factory<FindResult>::create(CreateExpression(boost::bind(&contextSetParam<SetOperationType::NOCHANGE>, resource->calc(), getTypeInfo(), paramID, rightValue, srcInfo)));
		case SetOperationType::SET        :
			operationResult = rightValue;
			break;
		case SetOperationType::ADDITION   :
			operationResult =  rdo::Factory<RDOCalcPlus>::create(paramValue, rightValue);
			break;
		case SetOperationType::SUBTRACTION:
			operationResult =  rdo::Factory<RDOCalcMinus>::create(paramValue, rightValue);
			break;
		case SetOperationType::MULTIPLY   :
			operationResult =  rdo::Factory<RDOCalcMult>::create(paramValue, rightValue);
			break;
		case SetOperationType::DIVIDE     :
			operationResult =  rdo::Factory<RDOCalcDiv>::create(paramValue, rightValue);
			break;
		case SetOperationType::INCREMENT  :
			operationResult =  rdo::Factory<RDOCalcPlus>::create(paramValue, rdo::Factory<RDOCalcConst>::create(1));
			break;
		case SetOperationType::DECRIMENT  :
			operationResult =  rdo::Factory<RDOCalcMinus>::create(paramValue, rdo::Factory<RDOCalcConst>::create(1));
			break;
		}
		ASSERT(operationResult);
		return rdo::Factory<FindResult>::create(CreateExpression(boost::bind(&contextSetParam<SetOperationType::SET>, resource->calc(), getTypeInfo(), paramID, operationResult, srcInfo)));
	}
	return rdo::Factory<FindResult>::create();
}

CLOSE_RDO_PARSER_NAMESPACE

/*!
  \copyright (c) RDO-Team, 2011
  \file      rdortp_param.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_resource.h"
#include "simulator/runtime/calc/resource/calc_resource.h"
#include "simulator/compiler/parser/rdortp_param.h"
#include "simulator/compiler/parser/rdorss.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDORTPParam
// --------------------------------------------------------------------------------
RDORTPParam::RDORTPParam(CREF(LPTypeInfo) pType, CREF(LPRDOValue) pDefault, CREF(RDOParserSrcInfo) src_info)
	: RDOParam(src_info, pType, pDefault)
{}

RDORTPParam::~RDORTPParam()
{}

CREF(tstring) RDORTPParam::name() const
{
	return RDOParam::name();
}

void RDORTPParam::writeModelStructure(REF(rdo::ostream) stream) const
{
	stream << name() << " ";
	getTypeInfo()->type()->writeModelStructure(stream);
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

}

Context::FindResult RDORTPParam::onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const
{
	if (method == Context::METHOD_GET)
	{
		LPExpression resource = params.get<LPExpression>(RDORSSResource::CONTEXT_PARAM_RESOURCE_EXPRESSION);
		const ruint paramID = params.get<ruint>(RDOParam::CONTEXT_PARAM_PARAM_ID);
		return FindResult(CreateExpression(boost::bind(&contextGetParam, resource->calc(), paramID, getTypeInfo(), srcInfo)));
	}

	return FindResult();
}

CLOSE_RDO_PARSER_NAMESPACE

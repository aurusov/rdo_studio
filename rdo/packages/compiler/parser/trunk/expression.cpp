/*!
  \copyright (c) RDO-Team, 2011
  \file      expression.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      07.03.2011
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/expression.h"
#include "simulator/runtime/rdo_resource.h"
#include "simulator/runtime/calc/procedural/calc_const.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- Expression
// --------------------------------------------------------------------------------
const std::string Expression::CONTEXT_PARAM_SET_EXPRESSION = "set_expression";

Expression::Expression(CREF(LPTypeInfo) pType, CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(RDOParserSrcInfo) src_info)
	: m_pType(pType)
	, m_pCalc(pCalc)
{
	ASSERT(m_pType);
	
	if (m_pCalc)
	{
		rdo::runtime::LPRDOCalcConst pConstCalc = m_pCalc.object_dynamic_cast<rdo::runtime::RDOCalcConst>();
		if (pConstCalc)
		{
			m_pValue = rdo::Factory<RDOValue>::create(pConstCalc->getValue(), src_info, m_pType);
		}
	}
	setSrcInfo(src_info);
}

Expression::Expression(CREF(LPRDOValue) pValue)
	: m_pType (pValue->typeInfo())
	, m_pValue(pValue            )
{
	ASSERT(pValue);
	ASSERT(pValue->constant());

	m_pCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(pValue->value());
	ASSERT(m_pCalc);

	setSrcInfo(pValue->src_info());
}

Expression::Expression(CREF(LPExpression) pExpression)
	: m_pType (pExpression->m_pType )
	, m_pValue(pExpression->m_pValue)
	, m_pCalc (pExpression->m_pCalc )
{}

Expression::~Expression()
{}

CREF(LPTypeInfo) Expression::typeInfo() const
{
	return m_pType;
}

CREF(rdo::runtime::LPRDOCalc) Expression::calc() const
{
	return m_pCalc;
}

void Expression::setSrcInfo(CREF(RDOParserSrcInfo) src_info)
{
	RDOParserSrcInfo::setSrcInfo(src_info);
	if (m_pCalc)
	{
		m_pCalc->setSrcInfo(src_info);
	}
}

LPRDOValue Expression::constant() const
{
	if (m_pValue)
	{
		return m_pValue;
	}
	return LPRDOValue(NULL);
}

// --------------------------------------------------------------------------------
// -------------------- ExpressionEmpty
// --------------------------------------------------------------------------------
ExpressionEmpty::ExpressionEmpty()
	: Expression(
		rdo::Factory<TypeInfo>::delegate<RDOType__void>(RDOParserSrcInfo()),
		rdo::runtime::LPRDOCalc(),
		RDOParserSrcInfo()
	)
{}

ExpressionEmpty::~ExpressionEmpty()
{}

CLOSE_RDO_PARSER_NAMESPACE

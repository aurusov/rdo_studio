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
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- Expression
// --------------------------------------------------------------------------------
Expression::Expression(CREF(LPTypeInfo) pType, CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(rdoRuntime::RDOSrcInfo) src_info)
	: m_pType(pType)
	, m_pCalc(pCalc)
{
	ASSERT(m_pType);
	ASSERT(m_pCalc);
	setSrcInfo(src_info);
}

Expression::Expression(CREF(LPExpression) pExpression)
	: m_pType(pExpression->m_pType)
	, m_pCalc(pExpression->m_pCalc)
{}

Expression::~Expression()
{}

CREF(LPTypeInfo) Expression::typeInfo() const
{
	return m_pType;
}

CREF(rdoRuntime::LPRDOCalc) Expression::calc() const
{
	return m_pCalc;
}

CREF(rdoRuntime::RDOSrcInfo) Expression::src_info() const
{
	ASSERT(m_pCalc);
	return m_pCalc->src_info();
}

void Expression::setSrcInfo(CREF(rdoRuntime::RDOSrcInfo) src_info)
{
	ASSERT(m_pCalc);
	m_pCalc->setSrcInfo(src_info);
}

RDOValue Expression::constant() const
{
	ASSERT(m_pCalc);
	ASSERT(m_pType);

	rdoRuntime::LPRDOCalcConst pCalc = m_pCalc.object_dynamic_cast<rdoRuntime::RDOCalcConst>();
	if (pCalc)
	{
		return RDOValue(pCalc->getValue(), src_info(), m_pType);
	}

	return RDOValue();
}
// --------------------------------------------------------------------------------
// -------------------- ExpressionStatement
// --------------------------------------------------------------------------------
ExpressionStatement::ExpressionStatement(CREF(LPTypeInfo) pType, CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(rdoRuntime::RDOSrcInfo) src_info)
	:Expression  (pType, pCalc, src_info),
	 m_returnFlag(true                  )
{}

ExpressionStatement::ExpressionStatement(CREF(LPExpression) pExpression)
	:Expression  (pExpression),
	 m_returnFlag(true       )
{}

ExpressionStatement::~ExpressionStatement()
{}

rbool ExpressionStatement::getReturn()
{
	return m_returnFlag;
}

CLOSE_RDO_PARSER_NAMESPACE

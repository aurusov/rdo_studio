/*!
  \copyright (c) RDO-Team, 2012
  \file      rdofun.cpp
  \authors   Поподьянец Евгений(kurt.gigacore@gmail.com)
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.09.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/compiler/parser/rdopatpreparse.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

rdo::runtime::LPRDOCalcEventPlan RDOEvent::prepareEvent(CREF(LPExpression) pExpression)
{
	ASSERT(pExpression);
	LPExpressionList pExpressionList = pExpression.object_dynamic_cast<ExpressionList>();
	rdo::runtime::LPRDOCalc pCalcTime;
	if(pExpressionList)
	{
		Expression::RDOExpressionList::const_iterator ExpressionIt = pExpressionList->expressionList().begin();
		if (ExpressionIt == pExpressionList->expressionList().end())
		{
			RDOParser::s_parser()->error().error( pExpressionList->src_info(), rdo::format(_T("Не указано время планирования события: %s"), name().c_str()));
		}
		LPExpression pExpressionTime = *ExpressionIt;
		ASSERT(pExpressionTime);
		++ExpressionIt;

		LPExpressionList pParamList = rdo::Factory<ExpressionList>::create();
		ASSERT(pParamList);

		while (ExpressionIt != pExpressionList->expressionList().end())
		{
			pParamList->addItem(*ExpressionIt);
			++ExpressionIt;
		}
		setParamList(pParamList);

		pCalcTime = pExpressionTime->calc();
		pCalcTime->setSrcInfo(pExpressionTime->src_info());
		ASSERT(pCalcTime);
	}
	else
	{
		pCalcTime = pExpression->calc();
		pCalcTime->setSrcInfo(pExpression->src_info());
		ASSERT(pCalcTime);
	}

	rdo::runtime::LPRDOCalcEventPlan pCalc = rdo::Factory<rdo::runtime::RDOCalcEventPlan>::create(pCalcTime);
	ASSERT(pCalc);

	attachCalc(pCalc);

	return pCalc;
}

CLOSE_RDO_PARSER_NAMESPACE

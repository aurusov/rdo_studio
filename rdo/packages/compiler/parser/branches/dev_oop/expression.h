/*!
  \copyright (c) RDO-Team, 2011
  \file      expression.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      07.03.2011
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_EXPRESSION_H_
#define _RDOPARSER_EXPRESSION_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/info.h"
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE
// --------------------------------------------------------------------------------
// -------------------- Expression
// --------------------------------------------------------------------------------
OBJECT(Expression) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(Expression);
friend class ExpressionGenerator;
public:
	typedef std::vector<LPExpression> RDOExpressionList;

	CREF(LPTypeInfo)               typeInfo  () const;
	CREF(rdo::runtime::LPRDOCalc)  calc      () const;
	virtual void                   setSrcInfo(CREF(RDOParserSrcInfo) src_info);

	LPRDOValue                     constant  () const;
	Expression();
	virtual ~Expression();


private:
	Expression(CREF(LPTypeInfo) pType, CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(RDOParserSrcInfo) src_info);
	Expression(CREF(LPRDOValue) pValue);
	Expression(CREF(LPExpression) pExpression);
	
	LPTypeInfo               m_pType;
	LPRDOValue               m_pValue; //! < Используется или m_pValue или m_pCalc
	rdo::runtime::LPRDOCalc  m_pCalc;  //! < Более красивое решение: завести парсеровский калк
};

// --------------------------------------------------------------------------------
// -------------------- ExpressionGenerator
// --------------------------------------------------------------------------------
CLASS(ExpressionGenerator): INSTANCE_OF(Expression)
{
DECLARE_FACTORY(ExpressionGenerator);
public:
static LPExpression generateByConst (CREF(LPRDOValue) pValue);

private:
	ExpressionGenerator(CREF(LPExpression) pExpression);
	virtual ~ExpressionGenerator();
};

// --------------------------------------------------------------------------------
// -------------------- ExpressionList
// --------------------------------------------------------------------------------
//! Список выражений
CLASS(ExpressionList): INSTANCE_OF(Expression)
{
DECLARE_FACTORY(ExpressionList);
public:
	
	CREF(RDOExpressionList) expressionList() const { return m_expressionList; }
	void                    addItem       (CREF(LPExpression) pExpression);

private:
	ExpressionList();
	virtual ~ExpressionList();

	RDOExpressionList m_expressionList;
};
DECLARE_POINTER(ExpressionList);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_EXPRESSION_H_

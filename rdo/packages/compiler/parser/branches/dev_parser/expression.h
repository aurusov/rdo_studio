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
class ExpressionStatement;
OBJECT(Expression) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(Expression);
friend class ExpressionStatement;
public:
	static const std::string CONTEXT_PARAM_SET_EXPRESSION;

	CREF(LPTypeInfo)               typeInfo  () const;
	CREF(rdo::runtime::LPRDOCalc)  calc      () const;
	virtual void                   setSrcInfo(CREF(RDOParserSrcInfo) src_info);

	LPRDOValue                     constant  () const;

protected:
	Expression(CREF(LPTypeInfo) pType, CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(RDOParserSrcInfo) src_info);
	Expression(CREF(LPRDOValue) pValue);
	Expression(CREF(LPExpression) pExpression);
	virtual ~Expression();

private:
	LPTypeInfo               m_pType;
	LPRDOValue               m_pValue; //! < Используется или m_pValue или m_pCalc
	rdo::runtime::LPRDOCalc  m_pCalc;  //! < Более красивое решение: завести парсеровский калк
};

// --------------------------------------------------------------------------------
// -------------------- ExpressionEmpty
// --------------------------------------------------------------------------------
CLASS(ExpressionEmpty): INSTANCE_OF(Expression)
{
DECLARE_FACTORY(ExpressionEmpty);
private:
	ExpressionEmpty();
	virtual ~ExpressionEmpty();
};
DECLARE_POINTER(ExpressionEmpty);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_EXPRESSION_H_

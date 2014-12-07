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
PREDECLARE_POINTER(Expression);
class Expression
	: public rdo::counter_reference
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(Expression);
friend class ExpressionStatement;
public:
	static const std::string CONTEXT_PARAM_SET_EXPRESSION;
	static const std::string CONTEXT_PARAM_SET_OPERATION_TYPE;

	const LPTypeInfo&               typeInfo  () const;
	const rdo::runtime::LPRDOCalc&  calc      () const;
	virtual void                    setSrcInfo(const RDOParserSrcInfo& src_info);

	LPRDOValue                     constant  () const;

protected:
	Expression(const LPTypeInfo& pType, const rdo::runtime::LPRDOCalc& pCalc, const RDOParserSrcInfo& src_info);
	Expression(const LPRDOValue& pValue);
	Expression(const LPExpression& pExpression);
	virtual ~Expression();

private:
	LPTypeInfo               m_pType;
	LPRDOValue               m_pValue; // Используется или m_pValue или m_pCalc
	rdo::runtime::LPRDOCalc  m_pCalc;  // Более красивое решение: завести парсеровский калк
};

// --------------------------------------------------------------------------------
// -------------------- ExpressionEmpty
// --------------------------------------------------------------------------------
class ExpressionEmpty: public Expression
{
DECLARE_FACTORY(ExpressionEmpty);
private:
	ExpressionEmpty();
	virtual ~ExpressionEmpty();
};
DECLARE_POINTER(ExpressionEmpty);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_EXPRESSION_H_

/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_statement.h
  \authors   Чирков Михаил
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      16.04.2011
  \brief     Инструкции
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_PROCEDURAL_STATEMENT_H_
#define _LIB_RUNTIME_CALC_PROCEDURAL_STATEMENT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Пустая операция
//! @todo заменить на CalcNop ?
CALC(RDOCalcNoChange)
{
	DECLARE_FACTORY(RDOCalcNoChange)
private:
	RDOCalcNoChange();
	DECLARE_ICalc;
};

//! Условный оператор if () then {}
CALC(RDOCalcIf)
{
	DECLARE_FACTORY(RDOCalcIf)
private:
	RDOCalcIf(CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pStatement);

	LPRDOCalc m_pCondition;
	LPRDOCalc m_pStatement;

	DECLARE_ICalc;
};

//! Условный оператор if () then {} else {}
CALC(RDOCalcIfElse)
{
	DECLARE_FACTORY(RDOCalcIfElse)
private:
	RDOCalcIfElse(CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pIfStatement, CREF(LPRDOCalc) pElseStatement);

	LPRDOCalc m_pCondition;
	LPRDOCalc m_pIfStatement;
	LPRDOCalc m_pElseStatement;

	DECLARE_ICalc;
};

//! Оператор цикла for
CALC(RDOCalcFor)
{
	DECLARE_FACTORY(RDOCalcFor)
public:
	void addCalcStatement(CREF(LPRDOCalc) pStatement);

private:
	RDOCalcFor(CREF(LPRDOCalc) pDeclaration, CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pExpression);

	LPRDOCalc m_pDeclaration;
	LPRDOCalc m_pCondition;
	LPRDOCalc m_pExpression;
	LPRDOCalc m_pStatement;

	DECLARE_ICalc;
};

//! Оператор возврата return
CALC(RDOCalcFunReturn)
{
	DECLARE_FACTORY(RDOCalcFunReturn)
private:
	RDOCalcFunReturn(CREF(LPRDOCalc) pReturn);

	LPRDOCalc m_pReturn;
	DECLARE_ICalc;
};

//! Оператор возврата break
CALC(RDOCalcFunBreak)
{
	DECLARE_FACTORY(RDOCalcFunBreak)
private:
	RDOCalcFunBreak();

	DECLARE_ICalc;
};

//! Список операторов
CALC(RDOCalcStatementList)
{
	DECLARE_FACTORY(RDOCalcStatementList)
public:
	void        addCalcStatement(CREF(LPRDOCalc) pStatement);
	RDOCalcList statementList();

private:
	RDOCalcStatementList();

	RDOCalcList m_calcStatementList;

	DECLARE_ICalc;
};

//! Ловушка для break
CALC(RDOCalcBreakCatch)
{
	DECLARE_FACTORY(RDOCalcBreakCatch)
public:
	void addStatementList(CREF(LPRDOCalc) pStatementList);

private:
	RDOCalcBreakCatch();
	
	LPRDOCalc m_pStatementList;

	DECLARE_ICalc;
};

//! Ловушка для return
CALC(RDOCalcReturnCatch)
{
	DECLARE_FACTORY(RDOCalcReturnCatch)
public:
	void addStatementList(CREF(LPRDOCalc) pStatementList);

private:
	RDOCalcReturnCatch();
	
	LPRDOCalc m_pStatementList;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_PROCEDURAL_STATEMENT_H_

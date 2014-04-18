/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_statement.h
  \authors   Чирков Михаил
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \authors   Поподьянец Евгений (kurt.gigacore@gmail.com)
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
PREDECLARE_POINTER(RDOCalcNoChange);
class RDOCalcNoChange: public RDOCalc
{
DECLARE_FACTORY(RDOCalcNoChange)
private:
	RDOCalcNoChange();
	DECLARE_ICalc;
};

//! Условный оператор if () then {}
PREDECLARE_POINTER(RDOCalcIf);
class RDOCalcIf: public RDOCalc
{
DECLARE_FACTORY(RDOCalcIf)
public:
	void setThenStatement(const LPRDOCalc& pStatement);
	void setElseStatement(const LPRDOCalc& pStatement);

	bool hasElse() const;

private:
	typedef  std::pair<LPRDOCalc, LPRDOCalc>  Statements;

	RDOCalcIf(const LPRDOCalc& pCondition);

	LPRDOCalc   m_pCondition;
	Statements  m_statements;

	DECLARE_ICalc;
};

//! Оператор цикла for
PREDECLARE_POINTER(RDOCalcFor);
class RDOCalcFor: public RDOCalc
{
DECLARE_FACTORY(RDOCalcFor)
public:
	void setStatement(const LPRDOCalc& pStatement);

private:
	RDOCalcFor(const LPRDOCalc& pDeclaration, const LPRDOCalc& pCondition, const LPRDOCalc& pExpression);

	LPRDOCalc m_pDeclaration;
	LPRDOCalc m_pCondition;
	LPRDOCalc m_pExpression;
	LPRDOCalc m_pStatement;

	DECLARE_ICalc;
};

//! Оператор возврата return
PREDECLARE_POINTER(RDOCalcFunReturn);
class RDOCalcFunReturn: public RDOCalc
{
DECLARE_FACTORY(RDOCalcFunReturn)
private:
	RDOCalcFunReturn(const LPRDOCalc& pReturn);

	LPRDOCalc m_pReturn;
	DECLARE_ICalc;
};

//! Оператор возврата break
PREDECLARE_POINTER(RDOCalcFunBreak);
class RDOCalcFunBreak: public RDOCalc
{
DECLARE_FACTORY(RDOCalcFunBreak)
private:
	RDOCalcFunBreak();

	DECLARE_ICalc;
};

//! Простой список операторов
PREDECLARE_POINTER(RDOCalcBaseStatementList);
class RDOCalcBaseStatementList: public RDOCalc
{
DECLARE_FACTORY(RDOCalcBaseStatementList)
public:
	void        addCalcStatement(const LPRDOCalc& pStatement);
	RDOCalcList statementList();

protected:
	RDOCalcBaseStatementList();

	RDOCalcList m_calcStatementList;

	DECLARE_ICalc;
};

//! Останавливаемый список операторов
PREDECLARE_POINTER(RDOCalcStatementList);
class RDOCalcStatementList: public RDOCalcBaseStatementList
{
DECLARE_FACTORY(RDOCalcStatementList)
private:
	RDOCalcStatementList();

	DECLARE_ICalc;
};

//! Ловушка для break
PREDECLARE_POINTER(RDOCalcBreakCatch);
class RDOCalcBreakCatch: public RDOCalc
{
DECLARE_FACTORY(RDOCalcBreakCatch)
public:
	void addStatementList(const LPRDOCalc& pStatementList);

private:
	RDOCalcBreakCatch();
	
	LPRDOCalc m_pStatementList;

	DECLARE_ICalc;
};

//! Ловушка для return
PREDECLARE_POINTER(RDOCalcReturnCatch);
class RDOCalcReturnCatch: public RDOCalc
{
DECLARE_FACTORY(RDOCalcReturnCatch)
public:
	void setTryCalc(const LPRDOCalc& pTryCalc);

private:
	RDOCalcReturnCatch();

	LPRDOCalc m_pTryCalc;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_PROCEDURAL_STATEMENT_H_

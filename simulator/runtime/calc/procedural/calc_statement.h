#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// TODO заменить на CalcNop ?
PREDECLARE_POINTER(RDOCalcNoChange);
class RDOCalcNoChange: public RDOCalc
{
DECLARE_FACTORY(RDOCalcNoChange)
private:
	RDOCalcNoChange();
	DECLARE_ICalc;
};

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

PREDECLARE_POINTER(RDOCalcFunReturn);
class RDOCalcFunReturn: public RDOCalc
{
DECLARE_FACTORY(RDOCalcFunReturn)
private:
	RDOCalcFunReturn(const LPRDOCalc& pReturn);

	LPRDOCalc m_pReturn;
	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcFunBreak);
class RDOCalcFunBreak: public RDOCalc
{
DECLARE_FACTORY(RDOCalcFunBreak)
private:
	RDOCalcFunBreak();

	DECLARE_ICalc;
};

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

PREDECLARE_POINTER(RDOCalcStatementList);
class RDOCalcStatementList: public RDOCalcBaseStatementList
{
DECLARE_FACTORY(RDOCalcStatementList)
private:
	RDOCalcStatementList();

	DECLARE_ICalc;
};

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

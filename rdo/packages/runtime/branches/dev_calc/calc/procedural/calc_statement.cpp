/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_statement.cpp
  \authors   Чирков Михаил
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      16.04.2011
  \brief     Инструкции
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_statement.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcNoChange
// --------------------------------------------------------------------------------
RDOCalcNoChange::RDOCalcNoChange()
{}

void RDOCalcNoChange::doCalc(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcIf
// --------------------------------------------------------------------------------
RDOCalcIf::RDOCalcIf(CREF(LPRDOCalc) pCondition)
	: m_pCondition(pCondition)
{
	ASSERT(m_pCondition);
}

void RDOCalcIf::setThenStatement(CREF(LPRDOCalc) pStatement)
{
	ASSERT(pStatement);
	m_statements.first = pStatement;
}

void RDOCalcIf::setElseStatement(CREF(LPRDOCalc) pStatement)
{
	ASSERT(pStatement);
	m_statements.second = pStatement;
}

rbool RDOCalcIf::hasElse() const
{
	return m_statements.second;
}

void RDOCalcIf::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pCondition->calcValue(pRuntime);

	if (pRuntime->stack().pop().getAsBool())
	{
		m_statements.first->calcValue(pRuntime);
	}
	else if (hasElse())
	{
		m_statements.second->calcValue(pRuntime);
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFor
// --------------------------------------------------------------------------------
RDOCalcFor::RDOCalcFor(CREF(LPRDOCalc) pDeclaration, CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pExpression)
	: m_pDeclaration(pDeclaration)
	, m_pCondition  (pCondition  )
	, m_pExpression (pExpression )
{
	ASSERT(m_pDeclaration);
	ASSERT(m_pCondition  );
	ASSERT(m_pExpression );
}

void RDOCalcFor::setStatement(CREF(LPRDOCalc) pStatement)
{
	ASSERT(pStatement);
	m_pStatement = pStatement;
}

void RDOCalcFor::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pDeclaration->calcValue(pRuntime);
	for (;;)
	{
		m_pCondition->calcValue(pRuntime);
		if (!pRuntime->stack().pop().getAsBool())
			break;

		m_pStatement->calcValue(pRuntime);
		if (pRuntime->getFunBreakFlag() != RDORuntime::FBF_NONE)
		{
			return;
		}

		m_pExpression->calcValue(pRuntime);
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFunReturn
// --------------------------------------------------------------------------------
RDOCalcFunReturn::RDOCalcFunReturn(CREF(LPRDOCalc) pReturn)
	: m_pReturn(pReturn)
{}

void RDOCalcFunReturn::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ASSERT(m_pReturn);

	m_pReturn->calcValue(pRuntime);
	pRuntime->setFunBreakFlag(RDORuntime::FBF_RETURN);
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFunBreak
// --------------------------------------------------------------------------------
RDOCalcFunBreak::RDOCalcFunBreak()
{}

void RDOCalcFunBreak::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->setFunBreakFlag(RDORuntime::FBF_BREAK);
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcBaseStatementList
// --------------------------------------------------------------------------------
RDOCalcBaseStatementList::RDOCalcBaseStatementList()
{}

void RDOCalcBaseStatementList::addCalcStatement(CREF(LPRDOCalc) pStatement)
{
	ASSERT(pStatement);
	m_calcStatementList.push_back(pStatement);
}

RDOCalc::RDOCalcList RDOCalcBaseStatementList::statementList()
{
	return m_calcStatementList;
}

void RDOCalcBaseStatementList::doCalc(CREF(LPRDORuntime) pRuntime)
{
	STL_FOR_ALL(m_calcStatementList, calcIt)
	{
		(*calcIt)->calcValue(pRuntime);
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcStatementList
// --------------------------------------------------------------------------------
RDOCalcStatementList::RDOCalcStatementList()
{}

void RDOCalcStatementList::doCalc(CREF(LPRDORuntime) pRuntime)
{
	if (pRuntime->getFunBreakFlag() != RDORuntime::FBF_NONE)
		return;

	STL_FOR_ALL(m_calcStatementList, calcIt)
	{
		(*calcIt)->calcValue(pRuntime);

		if (pRuntime->getFunBreakFlag() != RDORuntime::FBF_NONE)
		{
			return;
		}
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcBreakCatch
// --------------------------------------------------------------------------------
RDOCalcBreakCatch::RDOCalcBreakCatch()
{}

void RDOCalcBreakCatch::addStatementList(CREF(LPRDOCalc) pStatementList)
{
	ASSERT(pStatementList);
	m_pStatementList = pStatementList;
}

void RDOCalcBreakCatch::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ASSERT(m_pStatementList);

	m_pStatementList->calcValue(pRuntime);

	if (pRuntime->getFunBreakFlag() == RDORuntime::FBF_BREAK)
	{
		pRuntime->setFunBreakFlag(RDORuntime::FBF_NONE);
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcReturnCatch
// --------------------------------------------------------------------------------
RDOCalcReturnCatch::RDOCalcReturnCatch()
{}

void RDOCalcReturnCatch::setTryCalc(CREF(LPRDOCalc) pTryCalc)
{
	ASSERT(pTryCalc);
	m_pTryCalc = pTryCalc;
}

void RDOCalcReturnCatch::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ASSERT(m_pTryCalc);

	m_pTryCalc->calcValue(pRuntime);

	if (pRuntime->getFunBreakFlag() == RDORuntime::FBF_RETURN)
	{
		pRuntime->setFunBreakFlag(RDORuntime::FBF_NONE);
	}
}

CLOSE_RDO_RUNTIME_NAMESPACE

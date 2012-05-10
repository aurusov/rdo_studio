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
{
	m_value = RDOValue(0);
}

RDOValue RDOCalcNoChange::doCalc(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcIf
// --------------------------------------------------------------------------------
RDOCalcIf::RDOCalcIf(CREF(LPRDOCalc) pCondition)
	: m_pCondition(pCondition)
{
	ASSERT(m_pCondition);
	m_value = RDOValue(false);
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

RDOValue RDOCalcIf::doCalc(CREF(LPRDORuntime) pRuntime)
{
	return m_pCondition->calcValue(pRuntime).getAsBool()
		? m_statements.first->calcValue(pRuntime)
		: hasElse()
			? m_statements.second->calcValue(pRuntime)
			: m_value;
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

RDOValue RDOCalcFor::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = RDOValue(0);

	m_pDeclaration->calcValue(pRuntime);
	while (m_pCondition->calcValue(pRuntime).getAsBool())
	{
		m_value = m_pStatement->calcValue(pRuntime);
		if (pRuntime->getFunBreakFlag() != RDORuntime::FBF_NONE)
		{
			break;
		}
		m_pExpression->calcValue(pRuntime);
	}

	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFunReturn
// --------------------------------------------------------------------------------
RDOCalcFunReturn::RDOCalcFunReturn(CREF(LPRDOCalc) pReturn)
	: m_pReturn(pReturn)
{}

RDOValue RDOCalcFunReturn::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ASSERT(m_pReturn);

	m_value = m_pReturn->calcValue(pRuntime);
	pRuntime->setFunBreakFlag(RDORuntime::FBF_RETURN);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFunBreak
// --------------------------------------------------------------------------------
RDOCalcFunBreak::RDOCalcFunBreak()
{}

RDOValue RDOCalcFunBreak::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->setFunBreakFlag(RDORuntime::FBF_BREAK);
	return m_value;
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

RDOValue RDOCalcBaseStatementList::doCalc(CREF(LPRDORuntime) pRuntime)
{
	STL_FOR_ALL(m_calcStatementList, calcIt)
	{
		RDOValue tempValue = (*calcIt)->calcValue(pRuntime);
		if (tempValue.typeID() != RDOType::t_unknow)
		{
			m_value = tempValue;
		}
	}
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcStatementList
// --------------------------------------------------------------------------------
RDOCalcStatementList::RDOCalcStatementList()
{}

RDOValue RDOCalcStatementList::doCalc(CREF(LPRDORuntime) pRuntime)
{
	STL_FOR_ALL(m_calcStatementList, calcIt)
	{
		m_value = (*calcIt)->calcValue(pRuntime);

		if (pRuntime->getFunBreakFlag() != RDORuntime::FBF_NONE)
		{
			break;
		}
	}
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcBreakCatch
// --------------------------------------------------------------------------------
RDOCalcBreakCatch::RDOCalcBreakCatch()
{
	m_value = RDOValue(0);
}

void RDOCalcBreakCatch::addStatementList(CREF(LPRDOCalc) pStatementList)
{
	ASSERT(pStatementList);
	m_pStatementList = pStatementList;
}

RDOValue RDOCalcBreakCatch::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ASSERT(m_pStatementList);

	m_pStatementList->calcValue(pRuntime);

	if (pRuntime->getFunBreakFlag() == RDORuntime::FBF_BREAK)
	{
		pRuntime->setFunBreakFlag(RDORuntime::FBF_NONE);
	}

	return m_value;
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

RDOValue RDOCalcReturnCatch::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ASSERT(m_pTryCalc);

	m_value = m_pTryCalc->calcValue(pRuntime);

	if (pRuntime->getFunBreakFlag() == RDORuntime::FBF_RETURN)
	{
		pRuntime->setFunBreakFlag(RDORuntime::FBF_NONE);
	}

	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

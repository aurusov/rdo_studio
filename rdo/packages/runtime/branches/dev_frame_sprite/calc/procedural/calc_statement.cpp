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
#include "simulator/runtime/pch/stdpch.h"
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

RDOValue RDOCalcNoChange::doCalc(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return RDOValue();
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

RDOValue RDOCalcIf::doCalc(CREF(LPRDORuntime) pRuntime)
{
	return m_pCondition->calcValue(pRuntime).getAsBool()
		? m_statements.first->calcValue(pRuntime)
		: hasElse()
			? m_statements.second->calcValue(pRuntime)
			: RDOValue(false);
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
	RDOValue value = RDOValue(0);

	m_pDeclaration->calcValue(pRuntime);
	while (m_pCondition->calcValue(pRuntime).getAsBool())
	{
		value = m_pStatement->calcValue(pRuntime);
		if (pRuntime->getFunBreakFlag() != RDORuntime::FBF_NONE)
		{
			break;
		}
		m_pExpression->calcValue(pRuntime);
	}

	return value;
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

	RDOValue value = m_pReturn->calcValue(pRuntime);
	pRuntime->setFunBreakFlag(RDORuntime::FBF_RETURN);
	return value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFunBreak
// --------------------------------------------------------------------------------
RDOCalcFunBreak::RDOCalcFunBreak()
{}

RDOValue RDOCalcFunBreak::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->setFunBreakFlag(RDORuntime::FBF_BREAK);
	return RDOValue();
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
	RDOValue value;
	STL_FOR_ALL(m_calcStatementList, calcIt)
	{
		RDOValue tempValue = (*calcIt)->calcValue(pRuntime);
		if (tempValue.typeID() != RDOType::t_unknow)
		{
			value = tempValue;
		}
	}
	return value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcStatementList
// --------------------------------------------------------------------------------
RDOCalcStatementList::RDOCalcStatementList()
{}

RDOValue RDOCalcStatementList::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue value;
	STL_FOR_ALL(m_calcStatementList, calcIt)
	{
		value = (*calcIt)->calcValue(pRuntime);

		if (pRuntime->getFunBreakFlag() != RDORuntime::FBF_NONE)
		{
			break;
		}
	}
	return value;
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

RDOValue RDOCalcBreakCatch::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ASSERT(m_pStatementList);

	m_pStatementList->calcValue(pRuntime);

	if (pRuntime->getFunBreakFlag() == RDORuntime::FBF_BREAK)
	{
		pRuntime->setFunBreakFlag(RDORuntime::FBF_NONE);
	}

	return RDOValue();
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

	RDOValue value = m_pTryCalc->calcValue(pRuntime);

	if (pRuntime->getFunBreakFlag() == RDORuntime::FBF_RETURN)
	{
		pRuntime->setFunBreakFlag(RDORuntime::FBF_NONE);
	}

	return value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

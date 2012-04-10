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

REF(RDOValue) RDOCalcNoChange::doCalc(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	m_value = RDOValue(0);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcIf
// --------------------------------------------------------------------------------
RDOCalcIf::RDOCalcIf(CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pStatement)
	: m_pCondition(pCondition)
	, m_pStatement(pStatement)
{
	ASSERT(m_pCondition);
	ASSERT(m_pStatement);
}

REF(RDOValue) RDOCalcIf::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = RDOValue(false);
	return (m_pCondition->calcValue(pRuntime).getAsBool()) ? m_pStatement->calcValue(pRuntime) : (m_value);
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcIfElse
// --------------------------------------------------------------------------------
RDOCalcIfElse::RDOCalcIfElse(CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pIfStatement, CREF(LPRDOCalc) pElseStatement)
	: m_pCondition    (pCondition    )
	, m_pIfStatement  (pIfStatement  )
	, m_pElseStatement(pElseStatement)
{
	ASSERT(m_pCondition    );
	ASSERT(m_pIfStatement  );
	ASSERT(m_pElseStatement);
}

REF(RDOValue) RDOCalcIfElse::doCalc(CREF(LPRDORuntime) pRuntime)
{
	return (m_pCondition->calcValue(pRuntime).getAsBool()) ? m_pIfStatement->calcValue(pRuntime) : m_pElseStatement->calcValue(pRuntime);
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFor
// --------------------------------------------------------------------------------
RDOCalcFor::RDOCalcFor(CREF(LPRDOCalc) pDeclaration, CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pExpression, CREF(LPRDOCalc) pStatement)
	: m_pDeclaration(pDeclaration)
	, m_pCondition  (pCondition  )
	, m_pExpression (pExpression )
	, m_pStatement  (pStatement  )
{
	ASSERT(m_pDeclaration);
	ASSERT(m_pCondition  );
	ASSERT(m_pExpression );
	ASSERT(m_pStatement  );
}

RDOCalcFor::RDOCalcFor(CREF(LPRDOCalc) pDeclaration, CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pExpression)
	: m_pDeclaration(pDeclaration)
	, m_pCondition  (pCondition  )
	, m_pExpression (pExpression )
{
	ASSERT(m_pDeclaration);
	ASSERT(m_pCondition  );
	ASSERT(m_pExpression );
}

void RDOCalcFor::addCalcStatement(CREF(LPRDOCalc) pStatement)
{
	ASSERT(pStatement);
	m_pStatement = pStatement;
}

REF(RDOValue) RDOCalcFor::doCalc(CREF(LPRDORuntime) pRuntime)
{
	if(pRuntime->getFunBreakFlag() == RDORuntime::FBF_NONE)
	{
		m_value = m_pDeclaration->calcValue(pRuntime);
		while (m_pCondition->calcValue(pRuntime).getAsBool())
		{
			m_value = m_pStatement->calcValue(pRuntime);
			if(pRuntime->getFunBreakFlag() != RDORuntime::FBF_NONE)
			{
				return m_value;
			}
			m_pExpression->calcValue(pRuntime);
		}
	}
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFunReturn
// --------------------------------------------------------------------------------
RDOCalcFunReturn::RDOCalcFunReturn(CREF(LPRDOCalc) pReturn)
	: m_pReturn(pReturn)
{}

REF(RDOValue) RDOCalcFunReturn::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = m_pReturn->calcValue(pRuntime);
	pRuntime->setFunBreakFlag(RDORuntime::FBF_RETURN);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFunBreak
// --------------------------------------------------------------------------------
RDOCalcFunBreak::RDOCalcFunBreak()
{}

REF(RDOValue) RDOCalcFunBreak::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->setFunBreakFlag(RDORuntime::FBF_BREAK);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcStatementList
// --------------------------------------------------------------------------------
RDOCalcStatementList::RDOCalcStatementList()
{}

void RDOCalcStatementList::addCalcStatement(CREF(LPRDOCalc) pStatement)
{
	ASSERT(pStatement);
	m_calcStatementList.push_back(pStatement);
}

REF(RDOValue) RDOCalcStatementList::doCalc(CREF(LPRDORuntime) pRuntime)
{
	STL_FOR_ALL(m_calcStatementList, calcIt)
	{
		if(pRuntime->getFunBreakFlag() == RDORuntime::FBF_NONE);
		{
			LPRDOCalc pCalc = *calcIt;
			ASSERT(pCalc);
			m_value = pCalc->calcValue(pRuntime);
			if (pRuntime->getFunBreakFlag() != RDORuntime::FBF_NONE)
			{
				return m_value;
			}
		}
	}
	return m_value;
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

REF(RDOValue) RDOCalcBreakCatch::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = m_pStatementList->calcValue(pRuntime);
	if (pRuntime->getFunBreakFlag() == RDORuntime::FBF_BREAK)
	{
		pRuntime->setFunBreakFlag(RDORuntime::FBF_NONE);
		return m_value;
	}
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcReturnCatch
// --------------------------------------------------------------------------------
RDOCalcReturnCatch::RDOCalcReturnCatch()
{}

void RDOCalcReturnCatch::addStatementList(CREF(LPRDOCalc) pStatementList)
{
	ASSERT(pStatementList);
	m_pStatementList = pStatementList;
}

REF(RDOValue) RDOCalcReturnCatch::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = m_pStatementList->calcValue(pRuntime);
	if (pRuntime->getFunBreakFlag() == RDORuntime::FBF_RETURN)
	{
		pRuntime->setFunBreakFlag(RDORuntime::FBF_NONE);
		return m_value;
	}
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

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

REF(RDOValue) RDOCalcIf::doCalc(CREF(LPRDORuntime) pRuntime)
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

REF(RDOValue) RDOCalcFor::doCalc(CREF(LPRDORuntime) pRuntime)
{
	if (pRuntime->getFunBreakFlag() == RDORuntime::FBF_NONE)
	{
		m_value = m_pDeclaration->calcValue(pRuntime);
		while (m_pCondition->calcValue(pRuntime).getAsBool())
		{
			m_value = m_pStatement->calcValue(pRuntime);
			if (pRuntime->getFunBreakFlag() != RDORuntime::FBF_NONE)
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

REF(RDOValue) RDOCalcFunBreak::doCalc(CREF(LPRDORuntime) pRuntime)
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

REF(RDOValue) RDOCalcBaseStatementList::doCalc(CREF(LPRDORuntime) pRuntime)
{
	STL_FOR_ALL(m_calcStatementList, calcIt)
	{
		LPRDOCalc pCalc = *calcIt;
		ASSERT(pCalc);

		RDOValue tempValue = pCalc->calcValue(pRuntime);
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

REF(RDOValue) RDOCalcStatementList::doCalc(CREF(LPRDORuntime) pRuntime)
{
	if (pRuntime->getFunBreakFlag() != RDORuntime::FBF_NONE)
		return m_value;

	STL_FOR_ALL(m_calcStatementList, calcIt)
	{
		LPRDOCalc pCalc = *calcIt;
		ASSERT(pCalc);
		m_value = pCalc->calcValue(pRuntime);

		if (pRuntime->getFunBreakFlag() != RDORuntime::FBF_NONE)
		{
			return m_value;
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
	ASSERT(m_pStatementList);

	m_value = m_pStatementList->calcValue(pRuntime);
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

REF(RDOValue) RDOCalcReturnCatch::doCalc(CREF(LPRDORuntime) pRuntime)
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

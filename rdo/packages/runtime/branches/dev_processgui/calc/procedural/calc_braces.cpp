/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_braces.cpp
  \author    Чирков Михаил
  \date      16.04.2011
  \brief     Список инструкций в стиле C++
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_braces.h"
#include "simulator/runtime/calc/procedural/calc_statement.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcOpenBrace
// --------------------------------------------------------------------------------
RDOCalcOpenBrace::RDOCalcOpenBrace()
{}

REF(RDOValue) RDOCalcOpenBrace::doCalc(CREF(LPRDORuntime) pRuntime)
{
	LPRDOMemory pLocalMemory = rdo::Factory<RDOMemory>::create();
	pRuntime->getMemoryStack()->push(pLocalMemory);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcCloseBrace
// --------------------------------------------------------------------------------
RDOCalcCloseBrace::RDOCalcCloseBrace()
{}

REF(RDOValue) RDOCalcCloseBrace::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->getMemoryStack()->pop();
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcBodyBrace
// --------------------------------------------------------------------------------
RDOCalcBodyBrace::RDOCalcBodyBrace()
{}

void RDOCalcBodyBrace::addCalc(CREF(LPRDOCalc) pCalc)
{
	ASSERT(pCalc);
	m_calcList.push_back(pCalc);
	m_value = RDOValue(m_calcList.size());
}

REF(RDOValue) RDOCalcBodyBrace::doCalc(CREF(LPRDORuntime) pRuntime)
{
	STL_FOR_ALL(m_calcList, calc_it)
	{
		(*calc_it)->calcValue(pRuntime);
	}

	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFunBodyBrace
// --------------------------------------------------------------------------------
RDOCalcFunBodyBrace::RDOCalcFunBodyBrace()
{}

void RDOCalcFunBodyBrace::addFunCalc(CREF(LPRDOCalc) pCalc)
{
	ASSERT(pCalc);
	m_calcFunList.push_back(pCalc);
}

void RDOCalcFunBodyBrace::addRetCalc(CREF(LPRDOCalc) pCalc)
{
	ASSERT(pCalc);
	LPRDOCalc pCalcReturn = rdo::Factory<RDOCalcFunReturn>::create(pCalc);
	m_calcFunList.insert(--m_calcFunList.end(), pCalcReturn);
}

REF(RDOValue) RDOCalcFunBodyBrace::doCalc(CREF(LPRDORuntime) pRuntime)
{
	if (pRuntime->getFunBreakFlag() == RDORuntime::FBF_CONTINUE)
	{
		STL_FOR_ALL(m_calcFunList, calcIt)
		{
			LPRDOCalc pCalc = *calcIt;
			ASSERT(pCalc);
			m_value = pCalc->calcValue(pRuntime);
			if (pRuntime->getFunBreakFlag() == RDORuntime::FBF_BREAK)
			{
				m_calcFunList.back()->calcValue(pRuntime);
				pRuntime->setFunBreakFlag(RDORuntime::FBF_CONTINUE);
				m_value = RDOValue(false); //! break вообще ничего не возвращает
				return m_value;
			}
			if (pRuntime->getFunBreakFlag() == RDORuntime::FBF_RETURN)
			{
				m_calcFunList.back()->calcValue(pRuntime);
				return m_value;
			}
		}
	}
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFunEnd
// --------------------------------------------------------------------------------
RDOCalcFunEnd::RDOCalcFunEnd()
{}

REF(RDOValue) RDOCalcFunEnd::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->getMemoryStack()->pop();
	pRuntime->setFunBreakFlag(RDORuntime::FBF_CONTINUE);
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

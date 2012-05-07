/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_select.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      16.05.2007
  \brief     Калки команды Select
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/resource/calc_select.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelect
// --------------------------------------------------------------------------------
RDOFunCalcSelect::RDOFunCalcSelect(int nResType, CREF(LPRDOCalc) pCondition)
	: RDOFunCalcGroup(nResType, pCondition)
{}

void RDOFunCalcSelect::prepare(CREF(LPRDORuntime) pRuntime)
{
	res_list.clear();
	RDORuntime::ResCIterator end = pRuntime->res_end();
	for (RDORuntime::ResCIterator it = pRuntime->res_begin(); it != end; it++)
	{
		if (*it == LPRDOResource(NULL))
			continue;

		if (!(*it)->checkType(m_nResType))
			continue;

		pRuntime->pushGroupFunc(*it);

		m_pCondition->calcValue(pRuntime);
		if (pRuntime->stack().pop().getAsBool())
		{
			res_list.push_back(*it);
		}

		pRuntime->popGroupFunc();
	}
}

void RDOFunCalcSelect::doCalc(CREF(LPRDORuntime) pRuntime)
{
	prepare(pRuntime);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelectBase
// --------------------------------------------------------------------------------
RDOFunCalcSelectBase::RDOFunCalcSelectBase(CREF(LPRDOFunCalcSelect) pSelect, CREF(LPRDOCalc) pCondition)
	: m_pSelect   (pSelect   )
	, m_pCondition(pCondition)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelectExist
// --------------------------------------------------------------------------------
void RDOFunCalcSelectExist::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pSelect->prepare(pRuntime);
	rbool res = false;
	std::list<LPRDOResource>::iterator it  = m_pSelect->res_list.begin();
	std::list<LPRDOResource>::iterator end = m_pSelect->res_list.end();
	while (it != end && !res)
	{
		pRuntime->pushGroupFunc(*it);

		m_pCondition->calcValue(pRuntime);
		if (pRuntime->stack().pop().getAsBool())
		{
			res = true;
		}

		pRuntime->popGroupFunc();
		++it;
	}
	pRuntime->stack().push(RDOValue(res));
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelectNotExist
// --------------------------------------------------------------------------------
void RDOFunCalcSelectNotExist::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pSelect->prepare(pRuntime);
	rbool res = true;
	std::list<LPRDOResource>::iterator it  = m_pSelect->res_list.begin();
	std::list<LPRDOResource>::iterator end = m_pSelect->res_list.end();
	while (it != end && res)
	{
		pRuntime->pushGroupFunc(*it);

		m_pCondition->calcValue(pRuntime);
		if (pRuntime->stack().pop().getAsBool())
		{
			res = false;
		}

		pRuntime->popGroupFunc();
		++it;
	}
	pRuntime->stack().push(RDOValue(res));
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelectForAll
// --------------------------------------------------------------------------------
void RDOFunCalcSelectForAll::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pSelect->prepare(pRuntime);
	if (m_pSelect->res_list.empty())
	{
		pRuntime->stack().push(RDOValue(false));
		return;
	}
	rbool res = true;
	std::list<LPRDOResource>::iterator it  = m_pSelect->res_list.begin();
	std::list<LPRDOResource>::iterator end = m_pSelect->res_list.end();
	while (it != end && res)
	{
		pRuntime->pushGroupFunc(*it);

		m_pCondition->calcValue(pRuntime);
		if (!pRuntime->stack().pop().getAsBool())
		{
			res = false;
		}

		pRuntime->popGroupFunc();
		++it;
	}
	pRuntime->stack().push(RDOValue(res));
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelectNotForAll
// --------------------------------------------------------------------------------
void RDOFunCalcSelectNotForAll::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pSelect->prepare(pRuntime);
	rbool res = false;
	std::list<LPRDOResource>::iterator it  = m_pSelect->res_list.begin();
	std::list<LPRDOResource>::iterator end = m_pSelect->res_list.end();
	while (it != end && !res)
	{
		pRuntime->pushGroupFunc(*it);

		m_pCondition->calcValue(pRuntime);
		if (!pRuntime->stack().pop().getAsBool())
		{
			res = true;
		}

		pRuntime->popGroupFunc();
		++it;
	}
	pRuntime->stack().push(RDOValue(res));
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelectSize
// --------------------------------------------------------------------------------
void RDOFunCalcSelectSize::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pSelect->prepare(pRuntime);
	pRuntime->stack().push(RDOValue(m_pSelect->res_list.size()));
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelectEmpty
// --------------------------------------------------------------------------------
void RDOFunCalcSelectEmpty::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pSelect->prepare(pRuntime);
	pRuntime->stack().push(m_pSelect->res_list.empty());
}

CLOSE_RDO_RUNTIME_NAMESPACE

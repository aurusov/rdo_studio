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
{
	m_value = 1;
}

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
		if (m_pCondition->calcValue(pRuntime).getAsBool())
			res_list.push_back(*it);
		pRuntime->popGroupFunc();
	}
}

REF(RDOValue) RDOFunCalcSelect::doCalc(CREF(LPRDORuntime) pRuntime)
{
	prepare(pRuntime);
	return m_value;
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
REF(RDOValue) RDOFunCalcSelectExist::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pSelect->prepare(pRuntime);
	rbool res = false;
	std::list<LPRDOResource>::iterator it  = m_pSelect->res_list.begin();
	std::list<LPRDOResource>::iterator end = m_pSelect->res_list.end();
	while (it != end && !res)
	{
		pRuntime->pushGroupFunc(*it);
		if (m_pCondition->calcValue(pRuntime).getAsBool())
			res = true;
		pRuntime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelectNotExist
// --------------------------------------------------------------------------------
REF(RDOValue) RDOFunCalcSelectNotExist::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pSelect->prepare(pRuntime);
	rbool res = true;
	std::list<LPRDOResource>::iterator it  = m_pSelect->res_list.begin();
	std::list<LPRDOResource>::iterator end = m_pSelect->res_list.end();
	while (it != end && res)
	{
		pRuntime->pushGroupFunc(*it);
		if (m_pCondition->calcValue(pRuntime).getAsBool())
			res = false;
		pRuntime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelectForAll
// --------------------------------------------------------------------------------
REF(RDOValue) RDOFunCalcSelectForAll::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pSelect->prepare(pRuntime);
	if (m_pSelect->res_list.empty())
	{
		m_value = false;
		return m_value;
	}
	rbool res = true;
	std::list<LPRDOResource>::iterator it  = m_pSelect->res_list.begin();
	std::list<LPRDOResource>::iterator end = m_pSelect->res_list.end();
	while (it != end && res)
	{
		pRuntime->pushGroupFunc(*it);
		if (!m_pCondition->calcValue(pRuntime).getAsBool())
			res = false;
		pRuntime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelectNotForAll
// --------------------------------------------------------------------------------
REF(RDOValue) RDOFunCalcSelectNotForAll::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pSelect->prepare(pRuntime);
	rbool res = false;
	std::list<LPRDOResource>::iterator it  = m_pSelect->res_list.begin();
	std::list<LPRDOResource>::iterator end = m_pSelect->res_list.end();
	while (it != end && !res)
	{
		pRuntime->pushGroupFunc(*it);
		if (!m_pCondition->calcValue(pRuntime).getAsBool())
			res = true;
		pRuntime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelectSize
// --------------------------------------------------------------------------------
REF(RDOValue) RDOFunCalcSelectSize::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pSelect->prepare(pRuntime);
	m_value = m_pSelect->res_list.size();
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelectEmpty
// --------------------------------------------------------------------------------
REF(RDOValue) RDOFunCalcSelectEmpty::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pSelect->prepare(pRuntime);
	m_value = m_pSelect->res_list.empty();
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_group.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      16.05.2007
  \brief     Калки для группы ресурсов одного типа
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/resource/calc_group.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcGroup
// --------------------------------------------------------------------------------
RDOFunCalcGroup::RDOFunCalcGroup(int nResType, CREF(LPRDOCalc) pCondition)
	: m_nResType  (nResType  )
	, m_pCondition(pCondition)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcExist
// --------------------------------------------------------------------------------
void RDOFunCalcExist::doCalc(CREF(LPRDORuntime) pRuntime)
{
	rbool res = false;
	RDORuntime::ResCIterator end = pRuntime->res_end();
	for (RDORuntime::ResCIterator it = pRuntime->res_begin(); it != end && !res; ++it)
	{
		if (*it == LPRDOResource(NULL))
			continue;

		if (!(*it)->checkType(m_nResType))
			continue;

		pRuntime->pushGroupFunc(*it);

		m_pCondition->calcValue(pRuntime);
		if (pRuntime->stack().pop().getAsBool())
		{
			res = true;
		}

		pRuntime->popGroupFunc();
	}
	pRuntime->stack().push(RDOValue(res));
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcNotExist
// --------------------------------------------------------------------------------
void RDOFunCalcNotExist::doCalc(CREF(LPRDORuntime) pRuntime)
{
	rbool res = true;
	RDORuntime::ResCIterator end = pRuntime->res_end();
	for (RDORuntime::ResCIterator it = pRuntime->res_begin(); it != end && res; it++)
	{
		if (*it == LPRDOResource(NULL))
			continue;

		if (!(*it)->checkType(m_nResType))
			continue;

		pRuntime->pushGroupFunc(*it);

		m_pCondition->calcValue(pRuntime);
		if (pRuntime->stack().pop().getAsBool())
		{
			res = false;
		}

		pRuntime->popGroupFunc();
	}
	pRuntime->stack().push(RDOValue(res));
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcForAll
// --------------------------------------------------------------------------------
void RDOFunCalcForAll::doCalc(CREF(LPRDORuntime) pRuntime)
{
	rbool first_found = false;
	rbool res = true;
	RDORuntime::ResCIterator end = pRuntime->res_end();
	for (RDORuntime::ResCIterator it = pRuntime->res_begin(); it != end && res; it++)
	{
		if (*it == LPRDOResource(NULL))
			continue;

		if (!(*it)->checkType(m_nResType))
			continue;

		pRuntime->pushGroupFunc(*it);

		m_pCondition->calcValue(pRuntime);
		if (!pRuntime->stack().pop().getAsBool())
		{
			res = false;
		}
		else if (!first_found)
		{
			first_found = true;
		}

		pRuntime->popGroupFunc();
	}
	pRuntime->stack().push(first_found
		? RDOValue(res)
		: RDOValue(false)
	);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcNotForAll
// --------------------------------------------------------------------------------
void RDOFunCalcNotForAll::doCalc(CREF(LPRDORuntime) pRuntime)
{
	rbool res = false;
	RDORuntime::ResCIterator end = pRuntime->res_end();
	for (RDORuntime::ResCIterator it = pRuntime->res_begin(); it != end && !res; it++)
	{
		if (*it == LPRDOResource(NULL))
			continue;

		if (!(*it)->checkType(m_nResType))
			continue;

		pRuntime->pushGroupFunc(*it);

		m_pCondition->calcValue(pRuntime);
		if (!pRuntime->stack().pop().getAsBool())
		{
			res = true;
		}

		pRuntime->popGroupFunc();
	}
	pRuntime->stack().push(RDOValue(res));
}

CLOSE_RDO_RUNTIME_NAMESPACE

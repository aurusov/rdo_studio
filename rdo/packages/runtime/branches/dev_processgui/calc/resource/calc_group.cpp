/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_group.cpp
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      16.05.2007
  \brief     ����� ��� ������ �������� ������ ����
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
REF(RDOValue) RDOFunCalcExist::doCalc(CREF(LPRDORuntime) pRuntime)
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
		if (m_pCondition->calcValue(pRuntime).getAsBool())
			res = true;
		pRuntime->popGroupFunc();
	}
	m_value = res;
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcNotExist
// --------------------------------------------------------------------------------
REF(RDOValue) RDOFunCalcNotExist::doCalc(CREF(LPRDORuntime) pRuntime)
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
		if (m_pCondition->calcValue(pRuntime).getAsBool())
			res = false;
		pRuntime->popGroupFunc();
	}
	m_value = res;
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcForAll
// --------------------------------------------------------------------------------
REF(RDOValue) RDOFunCalcForAll::doCalc(CREF(LPRDORuntime) pRuntime)
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
		if (!m_pCondition->calcValue(pRuntime).getAsBool())
		{
			res = false;
		}
		else if (!first_found)
		{
			first_found = true;
		}
		pRuntime->popGroupFunc();
	}
	m_value = first_found ? res : false;
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcNotForAll
// --------------------------------------------------------------------------------
REF(RDOValue) RDOFunCalcNotForAll::doCalc(CREF(LPRDORuntime) pRuntime)
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
		if (!m_pCondition->calcValue(pRuntime).getAsBool())
			res = true;
		pRuntime->popGroupFunc();
	}
	m_value = res;
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

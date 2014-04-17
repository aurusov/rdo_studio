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
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/resource/calc_select.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_res_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelect
// --------------------------------------------------------------------------------
RDOFunCalcSelect::RDOFunCalcSelect(const LPIResourceType& pResType, int nResType, const LPRDOCalc& pCondition)
	: RDOFunCalcGroup(nResType, pCondition)
{
	m_pResType = pResType;
}

void RDOFunCalcSelect::prepare(const LPRDORuntime& pRuntime)
{
	res_list.clear();
	RDORuntime::ResCIterator end = pRuntime->getResType(m_nResType)->res_end();
	for (RDORuntime::ResCIterator it = pRuntime->getResType(m_nResType)->res_begin(); it != end; it++)
	{
		if (*it == LPRDOResource(NULL))
			continue;

		pRuntime->pushGroupFunc(*it);
		if (m_pCondition->calcValue(pRuntime).getAsBool())
			res_list.push_back(*it);
		pRuntime->popGroupFunc();
	}
}

RDOValue RDOFunCalcSelect::doCalc(const LPRDORuntime& pRuntime)
{
	prepare(pRuntime);
	return RDOValue();
}

const LPIResourceType& RDOFunCalcSelect::getResType()
{
	return m_pResType;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelectBase
// --------------------------------------------------------------------------------
RDOFunCalcSelectBase::RDOFunCalcSelectBase(const LPRDOFunCalcSelect& pSelect, const LPRDOCalc& pCondition)
	: m_pSelect   (pSelect   )
	, m_pCondition(pCondition)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelectExist
// --------------------------------------------------------------------------------
RDOValue RDOFunCalcSelectExist::doCalc(const LPRDORuntime& pRuntime)
{
	m_pSelect->prepare(pRuntime);
	bool res = false;
	std::list<LPRDOResource>::iterator it  = m_pSelect->res_list.begin();
	std::list<LPRDOResource>::iterator end = m_pSelect->res_list.end();
	while (it != end && !res)
	{
		pRuntime->pushGroupFunc(*it);
		if (m_pCondition->calcValue(pRuntime).getAsBool())
			res = true;
		pRuntime->popGroupFunc();
		++it;
	}
	return RDOValue(res);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelectNotExist
// --------------------------------------------------------------------------------
RDOValue RDOFunCalcSelectNotExist::doCalc(const LPRDORuntime& pRuntime)
{
	m_pSelect->prepare(pRuntime);
	bool res = true;
	std::list<LPRDOResource>::iterator it  = m_pSelect->res_list.begin();
	std::list<LPRDOResource>::iterator end = m_pSelect->res_list.end();
	while (it != end && res)
	{
		pRuntime->pushGroupFunc(*it);
		if (m_pCondition->calcValue(pRuntime).getAsBool())
			res = false;
		pRuntime->popGroupFunc();
		++it;
	}
	return RDOValue(res);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelectForAll
// --------------------------------------------------------------------------------
RDOValue RDOFunCalcSelectForAll::doCalc(const LPRDORuntime& pRuntime)
{
	m_pSelect->prepare(pRuntime);
	if (m_pSelect->res_list.empty())
	{
		return RDOValue(false);
	}
	bool res = true;
	std::list<LPRDOResource>::iterator it  = m_pSelect->res_list.begin();
	std::list<LPRDOResource>::iterator end = m_pSelect->res_list.end();
	while (it != end && res)
	{
		pRuntime->pushGroupFunc(*it);
		if (!m_pCondition->calcValue(pRuntime).getAsBool())
			res = false;
		pRuntime->popGroupFunc();
		++it;
	}
	return RDOValue(res);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelectNotForAll
// --------------------------------------------------------------------------------
RDOValue RDOFunCalcSelectNotForAll::doCalc(const LPRDORuntime& pRuntime)
{
	m_pSelect->prepare(pRuntime);
	bool res = false;
	std::list<LPRDOResource>::iterator it  = m_pSelect->res_list.begin();
	std::list<LPRDOResource>::iterator end = m_pSelect->res_list.end();
	while (it != end && !res)
	{
		pRuntime->pushGroupFunc(*it);
		if (!m_pCondition->calcValue(pRuntime).getAsBool())
			res = true;
		pRuntime->popGroupFunc();
		++it;
	}
	return RDOValue(res);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelectSize
// --------------------------------------------------------------------------------
RDOValue RDOFunCalcSelectSize::doCalc(const LPRDORuntime& pRuntime)
{
	m_pSelect->prepare(pRuntime);
	return RDOValue(static_cast<std::size_t>(m_pSelect->res_list.size()));
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelectEmpty
// --------------------------------------------------------------------------------
RDOValue RDOFunCalcSelectEmpty::doCalc(const LPRDORuntime& pRuntime)
{
	m_pSelect->prepare(pRuntime);
	return m_pSelect->res_list.empty();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelectArray
// --------------------------------------------------------------------------------
RDOValue RDOFunCalcSelectArray::doCalc(const LPRDORuntime& pRuntime)
{
	m_pSelect->prepare(pRuntime);
	std::list<LPRDOResource>::iterator it  = m_pSelect->res_list.begin();
	std::list<LPRDOResource>::iterator end = m_pSelect->res_list.end  ();
	rdo::runtime::LPRDOArrayType  pType  = rdo::Factory<rdo::runtime::RDOArrayType >::create(m_pSelect->getResType());
	ASSERT(pType);
	rdo::runtime::LPRDOArrayValue pValue = rdo::Factory<rdo::runtime::RDOArrayValue>::create(pType);
	ASSERT(pValue);
	while (it != end)
	{
		pValue->push_back(rdo::runtime::RDOValue(m_pSelect->getResType(), *(it++)));
	}
	return RDOValue(pType, pValue);
}

CLOSE_RDO_RUNTIME_NAMESPACE

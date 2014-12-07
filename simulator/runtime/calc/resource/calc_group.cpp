// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/resource/calc_group.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_res_type_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcGroup
// --------------------------------------------------------------------------------
RDOFunCalcGroup::RDOFunCalcGroup(int nResType, const LPRDOCalc& pCondition)
	: m_nResType  (nResType  )
	, m_pCondition(pCondition)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcExist
// --------------------------------------------------------------------------------
RDOValue RDOFunCalcExist::doCalc(const LPRDORuntime& pRuntime)
{
	bool res = false;
	RDORuntime::ResCIterator end = pRuntime->getResType(m_nResType)->res_end();
	for (RDORuntime::ResCIterator it = pRuntime->getResType(m_nResType)->res_begin(); it != end && !res; ++it)
	{
		if (*it == LPRDOResource(NULL))
			continue;

		pRuntime->pushGroupFunc(*it);
		if (m_pCondition->calcValue(pRuntime).getAsBool())
			res = true;
		pRuntime->popGroupFunc();
	}
	return RDOValue(res);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcNotExist
// --------------------------------------------------------------------------------
RDOValue RDOFunCalcNotExist::doCalc(const LPRDORuntime& pRuntime)
{
	bool res = true;
	RDORuntime::ResCIterator end = pRuntime->getResType(m_nResType)->res_end();
	for (RDORuntime::ResCIterator it = pRuntime->getResType(m_nResType)->res_begin(); it != end && res; it++)
	{
		if (*it == LPRDOResource(NULL))
			continue;

		pRuntime->pushGroupFunc(*it);
		if (m_pCondition->calcValue(pRuntime).getAsBool())
			res = false;
		pRuntime->popGroupFunc();
	}
	return RDOValue(res);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcForAll
// --------------------------------------------------------------------------------
RDOValue RDOFunCalcForAll::doCalc(const LPRDORuntime& pRuntime)
{
	bool first_found = false;
	bool res = true;
	RDORuntime::ResCIterator end = pRuntime->getResType(m_nResType)->res_end();
	for (RDORuntime::ResCIterator it = pRuntime->getResType(m_nResType)->res_begin(); it != end && res; it++)
	{
		if (*it == LPRDOResource(NULL))
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
	return RDOValue(first_found ? res : false);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcNotForAll
// --------------------------------------------------------------------------------
RDOValue RDOFunCalcNotForAll::doCalc(const LPRDORuntime& pRuntime)
{
	bool res = false;
	RDORuntime::ResCIterator end = pRuntime->getResType(m_nResType)->res_end();
	for (RDORuntime::ResCIterator it = pRuntime->getResType(m_nResType)->res_begin(); it != end && !res; it++)
	{
		if (*it == LPRDOResource(NULL))
			continue;

		pRuntime->pushGroupFunc(*it);
		if (!m_pCondition->calcValue(pRuntime).getAsBool())
			res = true;
		pRuntime->popGroupFunc();
	}
	return RDOValue(res);
}

CLOSE_RDO_RUNTIME_NAMESPACE

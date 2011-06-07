/*
 * copyright: (c) RDO-Team, 2011
 * filename : choice_from.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
#include <limits>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/choice_from.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdo_activity.h"
#include "rdo_lib/rdo_runtime/rdoprocess.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- Выбор ресурсов
// ----------------------------------------------------------------------------
REF(RDOValue) RDOSelectResourceNonExistCalc::doCalc(PTR(RDORuntime) runtime)
{
	runtime->getCurrentActivity()->setRelRes(rel_res_id, -1);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateNumberedResource
// ----------------------------------------------------------------------------
RDOCalcCreateNumberedResource::RDOCalcCreateNumberedResource(int _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _paramsCalcs, int _number, rbool _isPermanent)
	: m_pType    (_type       )
	, traceFlag  (_traceFlag  )
	, number     (_number     )
	, isPermanent(_isPermanent)
{}

REF(RDOValue) RDOCalcCreateNumberedResource::doCalc(PTR(RDORuntime) runtime)
{
	NEVER_REACH_HERE;
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateProcessResource
// ----------------------------------------------------------------------------
RDOCalcCreateProcessResource::RDOCalcCreateProcessResource(int _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _paramsCalcs, int _number, rbool _isPermanent)
	: m_pType    (_type       )
	, traceFlag  (_traceFlag  )
	, number     (_number     )
	, isPermanent(_isPermanent)
{}

REF(RDOValue) RDOCalcCreateProcessResource::doCalc(PTR(RDORuntime) runtime)
{
	NEVER_REACH_HERE;
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateResource
// ----------------------------------------------------------------------------
RDOCalcCreateResource::RDOCalcCreateResource(LPRDOResourceType _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _paramsCalcs, int _number, rbool _isPermanent)
	: m_pType    (_type       )
	, traceFlag  (_traceFlag  )
	, number     (_number     )
	, isPermanent(_isPermanent)
{
	paramsCalcs.insert(paramsCalcs.begin(), _paramsCalcs.begin(), _paramsCalcs.end());
}

REF(RDOValue) RDOCalcCreateResource::doCalc(PTR(RDORuntime) runtime)
{
	LPRDOResource res = m_pType->createRes(runtime, 0 /**@TODO вместо 0 вызвать функцию, дающую ID*/, true);
	if (!isPermanent)
	{
		res->makeTemporary(true);
	}
	res->appendParams(paramsCalcs.begin(), paramsCalcs.end());
	return m_value; // just to return something
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateEmptyResource
// ----------------------------------------------------------------------------
RDOCalcCreateEmptyResource::RDOCalcCreateEmptyResource(int _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _params_default, int _rel_res_id)
	: type      (_type      )
	, traceFlag (_traceFlag )
	, rel_res_id(_rel_res_id)
{
	params_default.insert(params_default.begin(), _params_default.begin(), _params_default.end());
}

REF(RDOValue) RDOCalcCreateEmptyResource::doCalc(PTR(RDORuntime) runtime)
{
	NEVER_REACH_HERE;
//	PTR(RDOResource) res = runtime->createNewResource(type, traceFlag);
	//runtime->getCurrentActivity()->setRelRes(rel_res_id, res->getTraceID());
	//res->appendParams(params_default.begin(), params_default.end());
	return m_value; // just to return something
}

// ----------------------------------------------------------------------------
// ---------- Выбор ресурсов
// ----------------------------------------------------------------------------
RDOSelectResourceCalc::RDOSelectResourceCalc(int _rel_res_id, CREF(LPRDOCalc) _choice_calc, CREF(LPRDOCalc) _order_calc, Type _order_type)
	: rel_res_id (_rel_res_id )
	, choice_calc(_choice_calc)
	, order_calc (_order_calc )
	, order_type (_order_type )
{}

REF(RDOValue) RDOSelectResourceDirectCalc::doCalc(PTR(RDORuntime) runtime)
{
	runtime->getCurrentActivity()->setRelRes(rel_res_id, res_id);
	if (choice_calc && !choice_calc->calcValue(runtime).getAsBool())
	{
		runtime->getCurrentActivity()->setRelRes(rel_res_id, -1);
		m_value = 0;
		return m_value;
	}
	m_value = 1;
	return m_value;
}

REF(RDOValue) RDOSelectResourceByTypeCalc::doCalc(PTR(RDORuntime) runtime)
{
	RDOValue maxVal   = -DBL_MAX;
	RDOValue minVal   = DBL_MAX;
	int res_minmax_id = -1;
	RDORuntime::ResCIterator end = runtime->res_end();
	for (RDORuntime::ResCIterator it = runtime->res_begin(); it != end; it++)
	{
		if (*it && (*it)->checkType(resType))
		{
			int res_id = (*it)->getTraceID();

			switch (order_type)
			{
				case order_empty:
				case order_first:
				{
					runtime->getCurrentActivity()->setRelRes(rel_res_id, res_id);
					if (choice_calc && !choice_calc->calcValue(runtime).getAsBool())
					{
						runtime->getCurrentActivity()->setRelRes(rel_res_id, -1);
						continue;
					}
					m_value = 1;
					return m_value;
				}
				case order_with_min:
				{
					runtime->getCurrentActivity()->setRelRes(rel_res_id, res_id);
					if (choice_calc && !choice_calc->calcValue(runtime).getAsBool())
					{
						runtime->getCurrentActivity()->setRelRes(rel_res_id, -1);
						continue;
					}
					RDOValue tmp = order_calc->calcValue(runtime);
					if (tmp < minVal)
					{
						minVal        = tmp;
						res_minmax_id = res_id;
					}
					break;
				}
				case order_with_max:
				{
					runtime->getCurrentActivity()->setRelRes(rel_res_id, res_id);
					if (choice_calc && !choice_calc->calcValue(runtime).getAsBool())
					{
						runtime->getCurrentActivity()->setRelRes(rel_res_id, -1);
						continue;
					}
					RDOValue tmp = order_calc->calcValue(runtime);
					if (tmp > maxVal)
					{
						maxVal        = tmp;
						res_minmax_id = res_id;
					}
					break;
				}
			}
		}
	}

	if (res_minmax_id != -1)
	{
		runtime->getCurrentActivity()->setRelRes(rel_res_id, res_minmax_id);
		m_value = 1;
		return m_value;
	}
	m_value = 0;
	return m_value;
}

void RDOSelectResourceCommonCalc::getBest(REF(std::vector< std::vector<int> >) allNumbs, ruint level, REF(std::vector<int>) res, REF(RDOValue) bestVal, PTR(RDORuntime) runtime, REF(rbool) hasBest) const
{
	if (level >= allNumbs.size())
	{
		for (ruint i = 0; i < resSelectors.size(); i++)
		{
			if (!resSelectors.at(i)->callChoice(runtime))
			{
				return; // state not valid
			}
		}
		RDOValue newVal = const_cast<PTR(RDOSelectResourceCommonCalc)>(this)->choice_calc->calcValue(runtime);
		if (!hasBest || (useCommonWithMax && (newVal > bestVal)) ||
		   (!useCommonWithMax && (newVal < bestVal))) // found better value
		{
			for (ruint i = 0; i < resSelectors.size(); i++)
			{
				res.at(i) = runtime->getCurrentActivity()->getResByRelRes(i);
			}
			bestVal = newVal;
			hasBest = true;
		}
		return;
	}
	REF(std::vector<int>) ourLevel = allNumbs.at(level);
	for (ruint i = 0; i < ourLevel.size(); i++)
	{
		runtime->getCurrentActivity()->setRelRes(level, ourLevel.at(i));
		getBest(allNumbs, level+1, res, bestVal, runtime, hasBest);
	}
}

rbool RDOSelectResourceCommonCalc::getFirst(REF(std::vector< std::vector<int> >) allNumbs, ruint level, PTR(RDORuntime) runtime) const
{
	if (level >= allNumbs.size())
	{
		for (ruint i = 0; i < resSelectors.size(); i++)
		{
			if (!resSelectors.at(i)->callChoice(runtime))
			{
				return false;
			}
		}
		return true;
	}
	REF(std::vector<int>) ourLevel = allNumbs.at(level);
	for (ruint i = 0; i < ourLevel.size(); i++)
	{
		runtime->getCurrentActivity()->setRelRes(level, ourLevel.at(i));
		if (getFirst(allNumbs, level+1, runtime)) return true;
	}
	return false;
}

//rbool RDOSelectResourceCommonCalc::getFirst(REF(std::vector< std::vector<int> >) allNumbs, int level,PTR(RDORuntime) runtime) const
//{
//	if (level <= 0) {
//		for (int i = 0; i < resSelectors.size(); i++) {
//			if (!resSelectors.at(i)->callChoice(runtime)) {
//				return false;
//			}
//		}
//		return true;
//	} else {
//		level--;
//		REF(std::vector<int>) ourLevel = allNumbs.at(level);
//		for (int i = 0; i < ourLevel.size(); i++) {
//			runtime->setRelRes(level, ourLevel.at(i));
//			if (getFirst(allNumbs, level, runtime)) return true;
//		}
//	}
//	return false;
//}

REF(RDOValue) RDOSelectResourceCommonCalc::doCalc(PTR(RDORuntime) runtime)
{
	std::vector< std::vector<int> > allNumbs;
	std::vector<int> res;
	for (ruint i = 0; i < resSelectors.size(); i++)
	{
		allNumbs.push_back(resSelectors.at(i)->getPossibleNumbers(runtime));
		res.push_back(runtime->getCurrentActivity()->getResByRelRes(i));
	}
	if (!choice_calc)
	{
		// first
//		if (getFirst(allNumbs, allNumbs.size(), runtime)) {
//			return true;
//		}
		if (getFirst(allNumbs, 0, runtime))
		{
			m_value = 1;
			return m_value;
		}
	}
	else
	{
		// with_min / with_max
		RDOValue bestVal = 0;
		rbool found = false;
		getBest(allNumbs, 0, res, bestVal, runtime, found);
		if (found)
		{
			for (ruint i = 0; i < res.size(); i++)
			{
				runtime->getCurrentActivity()->setRelRes(i, res.at(i));
			}
			m_value = 1;
			return m_value;
		}
	}
	m_value = 0;
	return m_value;
}

std::vector<int> RDOSelectResourceDirectCommonCalc::getPossibleNumbers(PTR(RDORuntime) runtime) const
{
	std::vector<int> res;	
	res.push_back(res_id);
	return res;
}

std::vector<int> RDOSelectResourceByTypeCommonCalc::getPossibleNumbers(PTR(RDORuntime) runtime) const
{
	std::vector<int> res;
	RDORuntime::ResCIterator end = runtime->res_end();
	for (RDORuntime::ResCIterator it = runtime->res_begin(); it != end; it++)
	{
		if (*it == NULL)
			continue;

		if (!(*it)->checkType(resType))
			continue;

		res.push_back((*it)->getTraceID());
	}
	return res;
}

rbool RDOSelectResourceDirectCommonCalc::callChoice(PTR(RDORuntime) runtime) const
{
	return (choice_calc && !const_cast<PTR(RDOSelectResourceDirectCommonCalc)>(this)->choice_calc->calcValue(runtime).getAsBool()) ? false : true;
}

rbool RDOSelectResourceByTypeCommonCalc::callChoice(PTR(RDORuntime) runtime) const
{
	return (choice_calc && !const_cast<PTR(RDOSelectResourceByTypeCommonCalc)>(this)->choice_calc->calcValue(runtime).getAsBool()) ? false : true;
}

IRDOSelectResourceCommon::IRDOSelectResourceCommon()
{}

IRDOSelectResourceCommon::~IRDOSelectResourceCommon()
{}

RDOSelectResourceDirectCommonCalc::~RDOSelectResourceDirectCommonCalc()
{}

RDOSelectResourceByTypeCommonCalc::~RDOSelectResourceByTypeCommonCalc()
{}

CLOSE_RDO_RUNTIME_NAMESPACE

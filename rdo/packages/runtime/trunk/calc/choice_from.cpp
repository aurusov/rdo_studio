/*!
  \copyright (c) RDO-Team, 2011
  \file      choice_from.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     RDOCalc для подбора релевантных ресурсов и создания ресурсов
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <limits>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/choice_from.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_activity.h"
#include "simulator/runtime/rdoprocess.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceNonExistCalc
// --------------------------------------------------------------------------------
REF(RDOValue) RDOSelectResourceNonExistCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->getCurrentActivity()->setRelRes(m_relResID, ~0);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcCreateNumberedResource
// --------------------------------------------------------------------------------
RDOCalcCreateNumberedResource::RDOCalcCreateNumberedResource(int typeID, rbool traceFlag, CREF(std::vector<RDOValue>) paramsCalcs, int number, rbool isPermanent)
	: m_typeID     (typeID     )
	, m_traceFlag  (traceFlag  )
	, m_number     (number     )
	, m_isPermanent(isPermanent)
{}

REF(RDOValue) RDOCalcCreateNumberedResource::doCalc(CREF(LPRDORuntime) pRuntime)
{
	NEVER_REACH_HERE;
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcCreateProcessResource
// --------------------------------------------------------------------------------
RDOCalcCreateProcessResource::RDOCalcCreateProcessResource(int typeID, rbool traceFlag, CREF(std::vector<RDOValue>) paramsCalcs, int number, rbool isPermanent)
	: m_typeID     (typeID     )
	, m_traceFlag  (traceFlag  )
	, m_number     (number     )
	, m_isPermanent(isPermanent)
{}

REF(RDOValue) RDOCalcCreateProcessResource::doCalc(CREF(LPRDORuntime) pRuntime)
{
	NEVER_REACH_HERE;
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcCreateResource
// --------------------------------------------------------------------------------
RDOCalcCreateResource::RDOCalcCreateResource(CREF(LPIResourceType) pType, CREF(std::vector<RDOValue>) rParamsCalcs, rbool traceFlag, rbool permanentFlag, ruint relResID)
	: m_pResType     (pType        )
	, m_traceFlag    (traceFlag    )
	, m_permanentFlag(permanentFlag)
	, m_relResID     (relResID     )
{
	m_paramsCalcs.insert(m_paramsCalcs.begin(), rParamsCalcs.begin(), rParamsCalcs.end());
	/// @todo переделать на ASSERT
	if (m_permanentFlag && m_relResID > 0) NEVER_REACH_HERE; //попытка создавать постоянные ресурсы динамически
}

REF(RDOValue) RDOCalcCreateResource::doCalc(CREF(LPRDORuntime) pRuntime)
{
	LPRDOResource pResource = m_pResType->createRes(pRuntime, pRuntime->getResourceId(), m_paramsCalcs, m_traceFlag, m_permanentFlag);
	if (m_relResID)
	{
		pRuntime->getCurrentActivity()->setRelRes(m_relResID, pResource->getTraceID());
	}
	return m_value; // just to return something
}

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceCalc
// --------------------------------------------------------------------------------
RDOSelectResourceCalc::RDOSelectResourceCalc(int relResID, CREF(LPRDOCalc) pCalcChoiceFrom, CREF(LPRDOCalc) pCalcOrder, Type orderType)
	: m_relResID       (relResID       )
	, m_pCalcChoiceFrom(pCalcChoiceFrom)
	, m_pCalcOrder     (pCalcOrder     )
	, m_orderType      (orderType      )
{}

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceDirectCalc
// --------------------------------------------------------------------------------
REF(RDOValue) RDOSelectResourceDirectCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->getCurrentActivity()->setRelRes(m_relResID, m_resID);
	if (m_pCalcChoiceFrom && !m_pCalcChoiceFrom->calcValue(pRuntime).getAsBool())
	{
		pRuntime->getCurrentActivity()->setRelRes(m_relResID, -1);
		m_value = 0;
		return m_value;
	}
	m_value = 1;
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceByTypeCalc
// --------------------------------------------------------------------------------
REF(RDOValue) RDOSelectResourceByTypeCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue maxVal   = -DBL_MAX;
	RDOValue minVal   = DBL_MAX;
	int res_minmax_id = -1;
	RDORuntime::ResCIterator end = pRuntime->res_end();
	for (RDORuntime::ResCIterator it = pRuntime->res_begin(); it != end; it++)
	{
		if (*it && (*it)->checkType(m_resTypeID))
		{
			int res_id = (*it)->getTraceID();

			switch (m_orderType)
			{
			case order_empty:
			case order_first:
				{
					pRuntime->getCurrentActivity()->setRelRes(m_relResID, res_id);
					if (m_pCalcChoiceFrom && !m_pCalcChoiceFrom->calcValue(pRuntime).getAsBool())
					{
						pRuntime->getCurrentActivity()->setRelRes(m_relResID, -1);
						continue;
					}
					m_value = 1;
					return m_value;
				}
			case order_with_min:
				{
					pRuntime->getCurrentActivity()->setRelRes(m_relResID, res_id);
					if (m_pCalcChoiceFrom && !m_pCalcChoiceFrom->calcValue(pRuntime).getAsBool())
					{
						pRuntime->getCurrentActivity()->setRelRes(m_relResID, -1);
						continue;
					}
					RDOValue tmp = m_pCalcOrder->calcValue(pRuntime);
					if (tmp < minVal)
					{
						minVal        = tmp;
						res_minmax_id = res_id;
					}
					break;
				}
			case order_with_max:
				{
					pRuntime->getCurrentActivity()->setRelRes(m_relResID, res_id);
					if (m_pCalcChoiceFrom && !m_pCalcChoiceFrom->calcValue(pRuntime).getAsBool())
					{
						pRuntime->getCurrentActivity()->setRelRes(m_relResID, -1);
						continue;
					}
					RDOValue tmp = m_pCalcOrder->calcValue(pRuntime);
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
		pRuntime->getCurrentActivity()->setRelRes(m_relResID, res_minmax_id);
		m_value = 1;
		return m_value;
	}
	m_value = 0;
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceCommonCalc
// --------------------------------------------------------------------------------
void RDOSelectResourceCommonCalc::getBest(REF(std::vector< std::vector<int> >) allNumbs, ruint level, REF(std::vector<int>) res, REF(RDOValue) bestVal, CREF(LPRDORuntime) pRuntime, REF(rbool) hasBest) const
{
	if (level >= allNumbs.size())
	{
		for (ruint i = 0; i < m_resSelectorList.size(); i++)
		{
			if (!m_resSelectorList.at(i)->callChoice(pRuntime))
			{
				return; // state not valid
			}
		}
		RDOValue newVal = const_cast<PTR(RDOSelectResourceCommonCalc)>(this)->m_pCalcChoiceFrom->calcValue(pRuntime);
		if (!hasBest || (m_useCommonWithMax && (newVal > bestVal)) ||
		   (!m_useCommonWithMax && (newVal < bestVal))) // found better value
		{
			for (ruint i = 0; i < m_resSelectorList.size(); i++)
			{
				res.at(i) = pRuntime->getCurrentActivity()->getResByRelRes(i);
			}
			bestVal = newVal;
			hasBest = true;
		}
		return;
	}
	REF(std::vector<int>) ourLevel = allNumbs.at(level);
	for (ruint i = 0; i < ourLevel.size(); i++)
	{
		pRuntime->getCurrentActivity()->setRelRes(level, ourLevel.at(i));
		getBest(allNumbs, level+1, res, bestVal, pRuntime, hasBest);
	}
}

rbool RDOSelectResourceCommonCalc::getFirst(REF(std::vector< std::vector<int> >) allNumbs, ruint level, CREF(LPRDORuntime) pRuntime) const
{
	if (level >= allNumbs.size())
	{
		for (ruint i = 0; i < m_resSelectorList.size(); i++)
		{
			if (!m_resSelectorList.at(i)->callChoice(pRuntime))
			{
				return false;
			}
		}
		return true;
	}
	REF(std::vector<int>) ourLevel = allNumbs.at(level);
	for (ruint i = 0; i < ourLevel.size(); i++)
	{
		pRuntime->getCurrentActivity()->setRelRes(level, ourLevel.at(i));
		if (getFirst(allNumbs, level+1, pRuntime)) return true;
	}
	return false;
}

//rbool RDOSelectResourceCommonCalc::getFirst(REF(std::vector< std::vector<int> >) allNumbs, int level,CREF(LPRDORuntime) pRuntime) const
//{
//	if (level <= 0) {
//		for (int i = 0; i < m_resSelectorList.size(); i++) {
//			if (!m_resSelectorList.at(i)->callChoice(pRuntime)) {
//				return false;
//			}
//		}
//		return true;
//	} else {
//		level--;
//		REF(std::vector<int>) ourLevel = allNumbs.at(level);
//		for (int i = 0; i < ourLevel.size(); i++) {
//			pRuntime->setRelRes(level, ourLevel.at(i));
//			if (getFirst(allNumbs, level, pRuntime)) return true;
//		}
//	}
//	return false;
//}

REF(RDOValue) RDOSelectResourceCommonCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	std::vector< std::vector<int> > allNumbs;
	std::vector<int> res;
	for (ruint i = 0; i < m_resSelectorList.size(); i++)
	{
		allNumbs.push_back(m_resSelectorList.at(i)->getPossibleNumbers(pRuntime));
		res.push_back(pRuntime->getCurrentActivity()->getResByRelRes(i));
	}
	if (!m_pCalcChoiceFrom)
	{
		// first
//		if (getFirst(allNumbs, allNumbs.size(), pRuntime)) {
//			return true;
//		}
		if (getFirst(allNumbs, 0, pRuntime))
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
		getBest(allNumbs, 0, res, bestVal, pRuntime, found);
		if (found)
		{
			for (ruint i = 0; i < res.size(); i++)
			{
				pRuntime->getCurrentActivity()->setRelRes(i, res.at(i));
			}
			m_value = 1;
			return m_value;
		}
	}
	m_value = 0;
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceDirectCommonCalc
// --------------------------------------------------------------------------------
std::vector<int> RDOSelectResourceDirectCommonCalc::getPossibleNumbers(CREF(LPRDORuntime) pRuntime) const
{
	std::vector<int> res;	
	res.push_back(m_resID);
	return res;
}

rbool RDOSelectResourceDirectCommonCalc::callChoice(CREF(LPRDORuntime) pRuntime) const
{
	return (m_pCalcChoiceFrom && !const_cast<PTR(RDOSelectResourceDirectCommonCalc)>(this)->m_pCalcChoiceFrom->calcValue(pRuntime).getAsBool()) ? false : true;
}

RDOSelectResourceDirectCommonCalc::~RDOSelectResourceDirectCommonCalc()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceByTypeCommonCalc
// --------------------------------------------------------------------------------
std::vector<int> RDOSelectResourceByTypeCommonCalc::getPossibleNumbers(CREF(LPRDORuntime) pRuntime) const
{
	std::vector<int> res;
	RDORuntime::ResCIterator end = pRuntime->res_end();
	for (RDORuntime::ResCIterator it = pRuntime->res_begin(); it != end; it++)
	{
		if (*it == NULL)
			continue;

		if (!(*it)->checkType(m_resTypeID))
			continue;

		res.push_back((*it)->getTraceID());
	}
	return res;
}

rbool RDOSelectResourceByTypeCommonCalc::callChoice(CREF(LPRDORuntime) pRuntime) const
{
	const rbool tmp = !const_cast<PTR(RDOSelectResourceByTypeCommonCalc)>(this)->m_pCalcChoiceFrom->calcValue(pRuntime).getAsBool();
	return (m_pCalcChoiceFrom && tmp) ? false : true;
}

RDOSelectResourceByTypeCommonCalc::~RDOSelectResourceByTypeCommonCalc()
{}

// --------------------------------------------------------------------------------
// -------------------- IRDOSelectResourceCommon
// --------------------------------------------------------------------------------
IRDOSelectResourceCommon::IRDOSelectResourceCommon()
{}

IRDOSelectResourceCommon::~IRDOSelectResourceCommon()
{}

CLOSE_RDO_RUNTIME_NAMESPACE

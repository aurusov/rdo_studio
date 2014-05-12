/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_dptsearch_activity.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      08.08.2009
  \brief     Активности точки принятия решения DPTSearch
  \indent    4T
*/

#ifndef _LIB_RUNTIME_DPTSEARCH_ACTIVITY_H_
#define _LIB_RUNTIME_DPTSEARCH_ACTIVITY_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_rule_i.h"
#include "simulator/runtime/rdo_dptsearch_activity_i.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdotrace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class     RDODPTSearchActivity
  \brief     Активность точки принятия решения DPTSearch
*/
class RDODPTSearchActivity: public IDPTSearchActivity, public RDOTraceableObject
{
DECLARE_FACTORY(RDODPTSearchActivity);
private:
	RDODPTSearchActivity(const LPIRule& rule, ValueTime valueTime, const LPRDOCalc& pCost);

	LPIRule                        m_rule;
	LPRDOCalc                      m_pCost;
	IDPTSearchActivity::ValueTime  m_valueTime;

	DECLARE_IDPTSearchActivity;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_DPTSEARCH_ACTIVITY_H_

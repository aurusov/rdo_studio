/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_dptsearch_activity.h
 * author   : Урусов Андрей
 * date     : 08.08.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_DPTSEARCH_ACTIVITY_H_
#define _RDO_DPTSEARCH_ACTIVITY_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_rule_interface.h"
#include "rdo_runtime_interface_registrator.h"
#include "rdo_dptsearch_activity_interface.h"
#include "rdocalc.h"
#include "rdotrace.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

class RDODPTSearchActivity: public IDPTSearchActivity, public RDOTraceableObject
{
DEFINE_FACTORY(RDODPTSearchActivity);
QUERY_INTERFACE_BEGIN
QUERY_INTERFACE_PARENT(RDOTraceableObject)
QUERY_INTERFACE(IDPTSearchActivity)
QUERY_INTERFACE_END

private:
	RDODPTSearchActivity(CREF(LPIRule) rule, ValueTime valueTime, PTR(RDOCalc) cost);

	LPIRule                        m_rule;
	PTR(RDOCalc)                   m_cost;
	IDPTSearchActivity::ValueTime  m_valueTime;

	DECLARE_IDPTSearchActivity;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDO_DPTSEARCH_ACTIVITY_H_

/*
 * copyright: (c) RDO-Team, 2010
 * filename : calc_event_plan.h
 * author   : Лущан Дмитрий
 * date     : 26.05.2010
 * bref     : RDOCalc для планирования и остановки событий
 * indent   : 4T
 */

#ifndef _CALC_EVENT_PLAN_H_
#define _CALC_EVENT_PLAN_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcEvent
// ----------------------------------------------------------------------------
CALC(RDOCalcEvent)
{
public:
	RDOCalcEvent();

	void setEvent(CREF(LPIBaseOperation) event);

protected:
	LPIBaseOperation m_event;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcEventPlan
// ----------------------------------------------------------------------------
CALC_SUB(RDOCalcEventPlan, RDOCalcEvent)
{
public:
	RDOCalcEventPlan(CREF(LPRDOCalc) pTimeCalc);

private:
	LPRDOCalc m_pTimeCalc;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcEventStop
// ----------------------------------------------------------------------------
CALC_SUB(RDOCalcEventStop, RDOCalcEvent)
{
public:
	RDOCalcEventStop();

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _CALC_EVENT_PLAN_H_

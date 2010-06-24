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
class RDOCalcEvent: public RDOCalc
{
public:
	RDOCalcEvent(PTR(RDORuntimeParent) parent);

	void setEvent(CREF(LPIBaseOperation) event);

protected:
	LPIBaseOperation m_event;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcEventPlan
// ----------------------------------------------------------------------------
class RDOCalcEventPlan: public RDOCalcEvent
{
public:
	RDOCalcEventPlan(PTR(RDORuntimeParent) parent, PTR(RDOCalc) timeCalc);

private:
	PTR(RDOCalc) m_timeCalc;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcEventStop
// ----------------------------------------------------------------------------
class RDOCalcEventStop: public RDOCalcEvent
{
public:
	RDOCalcEventStop(PTR(RDORuntimeParent) parent);

private:
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _CALC_EVENT_PLAN_H_

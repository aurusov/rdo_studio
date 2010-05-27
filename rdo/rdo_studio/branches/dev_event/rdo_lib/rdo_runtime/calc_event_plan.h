/*
 * copyright: (c) RDO-Team, 2010
 * filename : calc_event_plan.h
 * author   : Ћущан ƒмитрий
 * date     : 26.05.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALCEVENTPLAN_H_
#define _RDOCALCEVENTPLAN_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcEventPlan
// ----------------------------------------------------------------------------
class RDOCalcEventPlan: public RDOCalc
{
public:
	RDOCalcEventPlan(PTR(RDORuntimeParent) parent, PTR(IBaseOperation) _event, PTR(RDOCalc) calc)
		: RDOCalc    (parent)
		, m_event    (_event)
		, m_timerCalc(calc  )
	{}

private:
	PTR(IBaseOperation) m_event;
	PTR(RDOCalc)        m_timerCalc;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _RDOCALCEVENTPLAN_H_

#ifndef _LIB_RUNTIME_CALC_EVENT_H_
#define _LIB_RUNTIME_CALC_EVENT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_event_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOCalcEvent);
class RDOCalcEvent: public RDOCalc
{
DECLARE_FACTORY(RDOCalcEvent)

protected:
	RDOCalcEvent(const LPIEvent& event);

	LPIEvent m_pEvent;
};

PREDECLARE_POINTER(RDOCalcEventPlan);
class RDOCalcEventPlan: public RDOCalcEvent
{
DECLARE_FACTORY(RDOCalcEventPlan)
private:
	RDOCalcEventPlan(const LPIEvent& event, const LPRDOCalc& pTimeCalc, const std::vector<runtime::LPRDOCalc>& params);

	LPRDOCalc              m_pTimeCalc;
	std::vector<LPRDOCalc> m_params;

	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcEventStop);
class RDOCalcEventStop: public RDOCalcEvent
{
DECLARE_FACTORY(RDOCalcEventStop)
private:
	RDOCalcEventStop(const LPIEvent& event);

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_EVENT_H_

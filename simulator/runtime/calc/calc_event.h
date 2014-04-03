/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_event.h
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      26.05.2010
  \brief     RDOCalc для планирования и остановки событий
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_EVENT_H_
#define _LIB_RUNTIME_CALC_EVENT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_event_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Базовая калка для управления событиями
CALC(RDOCalcEvent)
{
DECLARE_FACTORY(RDOCalcEvent)

protected:
	RDOCalcEvent(const LPIEvent& event);

	LPIEvent m_pEvent;
};

//! Планирование события
CALC_SUB(RDOCalcEventPlan, RDOCalcEvent)
{
DECLARE_FACTORY(RDOCalcEventPlan)
private:
	RDOCalcEventPlan(const LPIEvent& event, const LPRDOCalc& pTimeCalc, const std::vector<runtime::LPRDOCalc>& params);

	LPRDOCalc              m_pTimeCalc;
	std::vector<LPRDOCalc> m_params;

	DECLARE_ICalc;
};

//! Остановка события
CALC_SUB(RDOCalcEventStop, RDOCalcEvent)
{
DECLARE_FACTORY(RDOCalcEventStop)
private:
	RDOCalcEventStop(const LPIEvent& event);

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_EVENT_H_

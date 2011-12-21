/*!
  \copyright (c) RDO-Team, 2011
  \file      event_plan.h
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      26.05.2010
  \brief     RDOCalc для планирования и остановки событий
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_EVENT_PLAN_H_
#define _LIB_RUNTIME_CALC_EVENT_PLAN_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDOCalcEvent
  \brief   Базовая калка для управления событиями
*/
CALC(RDOCalcEvent)
{
DECLARE_FACTORY(RDOCalcEvent)
public:
	void setEvent(CREF(LPIBaseOperation) pEvent);

protected:
	RDOCalcEvent();

	LPIBaseOperation m_pEvent;
};

/*!
  \class   RDOCalcEventPlan
  \brief   Планирование событий
*/
CALC_SUB(RDOCalcEventPlan, RDOCalcEvent)
{
DECLARE_FACTORY(RDOCalcEventPlan)
private:
	RDOCalcEventPlan(CREF(LPRDOCalc) pTimeCalc);

	LPRDOCalc m_pTimeCalc;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcEventStop
  \brief   Остановка событий
*/
CALC_SUB(RDOCalcEventStop, RDOCalcEvent)
{
DECLARE_FACTORY(RDOCalcEventStop)
private:
	RDOCalcEventStop();

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_EVENT_PLAN_H_

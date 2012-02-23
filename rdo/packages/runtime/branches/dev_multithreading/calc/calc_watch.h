/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_watch.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      04.03.2011
  \brief     Собираемые показатели
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_WATCH_H_
#define _LIB_RUNTIME_CALC_WATCH_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/result/result_group_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Начало сбора статистики по группе показателей
CALC(RDOCalcWatchGroupStart)
{
DECLARE_FACTORY(RDOCalcWatchGroupStart)
protected:
	RDOCalcWatchGroupStart(CREF(LPIResultGroup) pResultGroup);
	virtual ~RDOCalcWatchGroupStart();

	LPIResultGroup m_pResultGroup;

	DECLARE_ICalc;
};

//! Окончание сбора статистики по группе показателей
CALC(RDOCalcWatchGroupStop)
{
DECLARE_FACTORY(RDOCalcWatchGroupStop)
protected:
	RDOCalcWatchGroupStop(CREF(LPIResultGroup) pResultGroup);
	virtual ~RDOCalcWatchGroupStop();

	LPIResultGroup m_pResultGroup;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_WATCH_H_

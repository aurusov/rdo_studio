/*!
  \copyright (c) RDO-Team, 2011
  \file      watch.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      04.03.2011
  \brief     Собираемые показатели
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_WATCH_H_
#define _LIB_RUNTIME_CALC_WATCH_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdopokaz_group_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDOCalcWatchGroupStart
  \brief   RDOCalc 
*/
CALC(RDOCalcWatchGroupStart)
{
DECLARE_FACTORY(RDOCalcWatchGroupStart)
protected:
	RDOCalcWatchGroupStart(CREF(LPIPokazGroup) pResultGroup);
	virtual ~RDOCalcWatchGroupStart();

	LPIPokazGroup m_pResultGroup;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcWatchGroupStop
  \brief   Начало сбора статистики по группе показателей
*/
CALC(RDOCalcWatchGroupStop)
{
DECLARE_FACTORY(RDOCalcWatchGroupStop)
protected:
	RDOCalcWatchGroupStop(CREF(LPIPokazGroup) pResultGroup);
	virtual ~RDOCalcWatchGroupStop();

	LPIPokazGroup m_pResultGroup;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_WATCH_H_

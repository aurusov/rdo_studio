/*
 * @copyright (c) RDO-Team, 2011
 * @file      watch.h
 * @authors   Урусов Андрей
 * @date      04.03.2011
 * @brief     
 * @indent    4T
 *********************************************************************************/

#ifndef _RDOCALC_WATCH_H_
#define _RDOCALC_WATCH_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdopokaz_group_i.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOCalcWatchGroupStart
// ********************************************************************************
CALC(RDOCalcWatchGroupStart)
{
DECLARE_FACTORY(RDOCalcWatchGroupStart)
protected:
	RDOCalcWatchGroupStart(CREF(LPIPokazGroup) pResultGroup);
	virtual ~RDOCalcWatchGroupStart();

	LPIPokazGroup m_pResultGroup;

	DECALRE_ICalc;
};

// ********************************************************************************
// ******************** RDOCalcWatchGroupStop
// ********************************************************************************
CALC(RDOCalcWatchGroupStop)
{
DECLARE_FACTORY(RDOCalcWatchGroupStop)
protected:
	RDOCalcWatchGroupStop(CREF(LPIPokazGroup) pResultGroup);
	virtual ~RDOCalcWatchGroupStop();

	LPIPokazGroup m_pResultGroup;

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _RDOCALC_WATCH_H_

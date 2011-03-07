/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdocalc_result.h
 * author   : Урусов Андрей
 * date     : 04.03.2011
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALC_RESULT_H_
#define _RDOCALC_RESULT_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdopokaz_group_i.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcResultGroupStart
// ----------------------------------------------------------------------------
CALC(RDOCalcResultGroupStart)
{
DECLARE_FACTORY(RDOCalcResultGroupStart)
protected:
	RDOCalcResultGroupStart(CREF(LPIPokazGroup) pResultGroup);
	virtual ~RDOCalcResultGroupStart();

	LPIPokazGroup m_pResultGroup;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcResultGroupStop
// ----------------------------------------------------------------------------
CALC(RDOCalcResultGroupStop)
{
DECLARE_FACTORY(RDOCalcResultGroupStop)
protected:
	RDOCalcResultGroupStop(CREF(LPIPokazGroup) pResultGroup);
	virtual ~RDOCalcResultGroupStop();

	LPIPokazGroup m_pResultGroup;

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOCALC_RESULT_H_

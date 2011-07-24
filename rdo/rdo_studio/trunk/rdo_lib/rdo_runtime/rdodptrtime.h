/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdodptrtime.h
 * @authors   Урусов Андрей
 * @date      unknown
 * @brief     unknown
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_DPTRTIME_
#define _LIB_RUNTIME_DPTRTIME_

// *********************************************************************** INCLUDES
#include <sstream>
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/searchtrace.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

/******************************************************************************//**
 * @class   RDODPTSearchRuntime
 * @brief   потомок RDODPTSearch
 *********************************************************************************/
class RDODPTSearchRuntime: public RDODPTSearchTrace
{
DEFINE_IFACTORY(RDODPTSearchRuntime);

private:
	RDODPTSearchRuntime(CREF(LPRDORuntime) pRuntime, LPIBaseOperationContainer parent, CREF(LPRDOCalc) _pCondition, CREF(LPRDOCalc) _pTermCondition, CREF(LPRDOCalc) _pEvaluateBy, rbool _compTops, RDODPTSearchTrace::DPT_TraceFlag _traceFlag);

	LPRDOCalc pCondition;
	LPRDOCalc pTermCondition;
	LPRDOCalc pEvaluateBy;
	rbool     compTops;

	virtual rbool  onCheckCondition(CREF(LPRDORuntime) pRuntime);
	virtual rbool  TermCondition   (CREF(LPRDORuntime) pRuntime);
	virtual double EvaluateBy      (CREF(LPRDORuntime) pRuntime);
	virtual rbool  NeedCompareTops ();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/rdodptrtime.inl"

#endif // _LIB_RUNTIME_DPTRTIME_

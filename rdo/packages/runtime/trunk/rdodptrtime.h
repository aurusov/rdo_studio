/*!
  \copyright (c) RDO-Team, 2011
  \file      rdodptrtime.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      16.05.2007
  \brief     Runtime трассировка для DPTSearch
  \indent    4T
*/

#ifndef _LIB_RUNTIME_DPTRTIME_H_
#define _LIB_RUNTIME_DPTRTIME_H_

// ----------------------------------------------------------------------- INCLUDES
#include <sstream>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/searchtrace.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDODPTSearchRuntime
  \brief   потомок RDODPTSearch
*/
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

#include "simulator/runtime/rdodptrtime.inl"

#endif // _LIB_RUNTIME_DPTRTIME_H_

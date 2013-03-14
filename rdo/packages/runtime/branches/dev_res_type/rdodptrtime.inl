/*!
  \copyright (c) RDO-Team, 2011
  \file      rdodptrtime.inl
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      22.07.2011
  \brief     Runtime трассировка для DPTSearch
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdomacros.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearchRuntime
// --------------------------------------------------------------------------------
inline RDODPTSearchRuntime::RDODPTSearchRuntime(CREF(LPRDORuntime) pRuntime, LPIBaseOperationContainer parent, CREF(LPRDOCalc) _pCondition, CREF(LPRDOCalc) _pTermCondition, CREF(LPRDOCalc) _pEvaluateBy, rbool _compTops, RDODPTSearchTrace::DPT_TraceFlag _traceFlag)
	: RDODPTSearchTrace(pRuntime, parent)
	, pCondition       (_pCondition     )
	, pTermCondition   (_pTermCondition )
	, pEvaluateBy      (_pEvaluateBy    )
	, compTops         (_compTops       )
{
	setTraceID(pRuntime->getFreeDPTId());
	traceFlag = _traceFlag;
}

inline rbool  RDODPTSearchRuntime::onCheckCondition(CREF(LPRDORuntime) pRuntime)
{
	return pCondition->calcValue(pRuntime).getAsBool();
}

inline rbool RDODPTSearchRuntime::TermCondition(CREF(LPRDORuntime) pRuntime)
{
	return pTermCondition->calcValue(pRuntime).getAsBool();
}

inline double RDODPTSearchRuntime::EvaluateBy(CREF(LPRDORuntime) pRuntime)
{
	return pEvaluateBy->calcValue(pRuntime).getDouble();
}

inline rbool RDODPTSearchRuntime::NeedCompareTops()
{
	return compTops;
}

CLOSE_RDO_RUNTIME_NAMESPACE

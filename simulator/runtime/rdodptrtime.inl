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
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearchRuntime
// --------------------------------------------------------------------------------
inline RDODPTSearchRuntime::RDODPTSearchRuntime(const LPRDORuntime& pRuntime, LPIBaseOperationContainer parent, const LPRDOCalc& _pCondition, const LPRDOCalc& _pTermCondition, const LPRDOCalc& _pEvaluateBy, bool _compTops, RDODPTSearchTrace::DPT_TraceFlag _traceFlag)
	: RDODPTSearchTrace(pRuntime, parent)
	, pCondition       (_pCondition     )
	, pTermCondition   (_pTermCondition )
	, pEvaluateBy      (_pEvaluateBy    )
	, compTops         (_compTops       )
{
	setTraceID(pRuntime->getFreeDPTId());
	traceFlag = _traceFlag;
}

inline bool RDODPTSearchRuntime::onCheckCondition(const LPRDORuntime& pRuntime)
{
	return pCondition->calcValue(pRuntime).getAsBool();
}

inline bool RDODPTSearchRuntime::TermCondition(const LPRDORuntime& pRuntime)
{
	return pTermCondition->calcValue(pRuntime).getAsBool();
}

inline double RDODPTSearchRuntime::EvaluateBy(const LPRDORuntime& pRuntime)
{
	return pEvaluateBy->calcValue(pRuntime).getDouble();
}

inline bool RDODPTSearchRuntime::NeedCompareTops()
{
	return compTops;
}

CLOSE_RDO_RUNTIME_NAMESPACE

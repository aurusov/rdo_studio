#ifndef RDODTPRTIME_H
#define RDODTPRTIME_H

#include <sstream>
#include "rdo_lib/rdo_runtime/searchtrace.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDODPTSearchRuntime - DPT (потомок RDODPTSearch)
// ----------------------------------------------------------------------------
class RDODPTSearchRuntime: public RDODPTSearchTrace
{
DEFINE_IFACTORY(RDODPTSearchRuntime);

private:
	RDODPTSearchRuntime( CREF(LPRDORuntime) pRuntime, LPIBaseOperationContainer parent, CREF(LPRDOCalc) _pCondition, CREF(LPRDOCalc) _pTermCondition, CREF(LPRDOCalc) _pEvaluateBy, rbool _compTops, RDODPTSearchTrace::DPT_TraceFlag _traceFlag ):
		RDODPTSearchTrace( pRuntime, parent ),
		pCondition( _pCondition ),
		pTermCondition( _pTermCondition ),
		pEvaluateBy( _pEvaluateBy ),
		compTops( _compTops )
	{
		setTraceID( pRuntime->getFreeDPTId() );
		traceFlag = _traceFlag;
	}

	LPRDOCalc pCondition;
	LPRDOCalc pTermCondition;
	LPRDOCalc pEvaluateBy;
	rbool     compTops;

	virtual rbool  onCheckCondition(CREF(LPRDORuntime) pRuntime) { return pCondition    ->calcValue(pRuntime).getAsBool(); }
	virtual rbool  TermCondition   (CREF(LPRDORuntime) pRuntime) { return pTermCondition->calcValue(pRuntime).getAsBool(); }
	virtual double EvaluateBy      (CREF(LPRDORuntime) pRuntime) { return pEvaluateBy   ->calcValue(pRuntime).getDouble(); }
	virtual rbool  NeedCompareTops ()                            { return compTops; }
};

} // namespace rdoRuntime

#endif // RDODTPRTIME_H

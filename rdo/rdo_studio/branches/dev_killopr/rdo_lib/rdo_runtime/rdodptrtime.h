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
DEFINE_FACTORY(RDODPTSearchRuntime);

private:
	RDODPTSearchRuntime( RDORuntime* runtime, LPIBaseOperationContainer parent, RDOCalc* _condition, RDOCalc* _termCondition, RDOCalc* _evaluateBy, bool _compTops, RDODPTSearchTrace::DPT_TraceFlag _traceFlag ):
		RDODPTSearchTrace( runtime, parent ),
		condition( _condition ),
		termCondition( _termCondition ),
		evaluateBy( _evaluateBy ),
		compTops( _compTops )
	{
		setTraceID( runtime->getFreeDPTId() );
		traceFlag = _traceFlag;
	}

	RDOCalc* condition;
	RDOCalc* termCondition;
	RDOCalc* evaluateBy;
	bool compTops;

	virtual bool   onCheckCondition( RDOSimulator* sim ) { return condition->calcValue( static_cast<RDORuntime*>(sim) ).getAsBool();     }
	virtual bool   TermCondition( RDOSimulator* sim )    { return termCondition->calcValue( static_cast<RDORuntime*>(sim) ).getAsBool(); }
	virtual double EvaluateBy( RDOSimulator* sim )       { return evaluateBy->calcValue( static_cast<RDORuntime*>(sim) ).getDouble();    }
	virtual bool   NeedCompareTops()                     { return compTops; }
};

} // namespace rdoRuntime

#endif // RDODTPRTIME_H

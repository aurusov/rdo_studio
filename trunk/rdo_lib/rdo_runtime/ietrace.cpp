#include "pch.h"
#include "ietrace.h"
#include "simtrace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime {

RDOIETrace::RDOIETrace( RDORuntimeParent* _runtime, RDOSimulatorTrace* _sim ): //qq
	RDOIE( _runtime ),
	RDOTraceableObject( _sim ),
	RDOPatternTrace()
{
}

void RDOIETrace::onAfterIrregularEvent( RDOSimulator* sim )
{ 
	RDOSimulatorTrace* simTr = (RDOSimulatorTrace*)sim;
	simTr->getTracer()->writeIrregularEvent( this, simTr );
}

} // namespace rdoRuntime

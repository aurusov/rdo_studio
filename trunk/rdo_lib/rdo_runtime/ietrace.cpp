#include "pch.h"
#include "ietrace.h"
#include "simtrace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime {

RDOIETrace::RDOIETrace( RDORuntimeParent* runtime, bool trace ):
	RDOIE( runtime ),
	RDOPatternTrace( trace )
{
}

void RDOIETrace::onAfterIrregularEvent( RDOSimulator* sim )
{ 
	RDOSimulatorTrace* simTr = (RDOSimulatorTrace*)sim;
	simTr->getTracer()->writeIrregularEvent( this, simTr );
}

} // namespace rdoRuntime

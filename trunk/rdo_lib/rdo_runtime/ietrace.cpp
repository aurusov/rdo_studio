#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdotrace.h"
#include "ietrace.h"

void RDOIETrace::onAfterIrregularEvent( RDOSimulator* sim )
{ 
	RDOSimulatorTrace* simTr = (RDOSimulatorTrace*)sim;
	simTr->getTracer()->writeIrregularEvent( this, simTr );
}

RDOIETrace::RDOIETrace( RDOSimulatorTrace* _sim ): RDOTraceableObject( _sim ), RDOPatternTrace()
{
}

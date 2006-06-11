#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdotrace.h"
#include "ietrace.h"

void RDOIETrace::onAfterIrregularEvent(RDOSimulator *sim)
{ 
   RDOSimulatorTrace *simTr = (RDOSimulatorTrace *)sim;
   onAfter(simTr);
   simTr->getTracer()->writeIrregularEvent(this, simTr);
   simTr->clearJustCreatedFlags();
}

RDOIETrace::RDOIETrace(RDOSimulatorTrace *i_sim):
   RDOTraceableObject(i_sim), RDOPattern(i_sim)
{}

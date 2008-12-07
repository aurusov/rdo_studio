#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "RdoTrace.h"

void RDOOperationTrace::onBeforeOperationBegin(RDOSimulator *sim)
{
   RDOSimulatorTrace *simTr = (RDOSimulatorTrace *)sim;
   simTr->getTracer()->writeBeforeOperationBegin(this, simTr); 
}

void RDOOperationTrace::onAfterOperationBegin(RDOSimulator *sim)
{
   RDOSimulatorTrace *simTr = (RDOSimulatorTrace *)sim;
   onAfter(simTr);
   simTr->getTracer()->writeAfterOperationBegin(this, simTr);
   simTr->clearJustCreatedFlags();
}

void RDOOperationTrace::onBeforeOperationEnd(RDOSimulator *sim)
{
   RDOSimulatorTrace *simTr = (RDOSimulatorTrace *)sim;
   simTr->getTracer()->writeBeforeOperationEnd(this, simTr); 
}

void RDOOperationTrace::onAfterOperationEnd(RDOSimulator *sim)
{
   RDOSimulatorTrace *simTr = (RDOSimulatorTrace *)sim;
   simTr->getTracer()->writeAfterOperationEnd(this, simTr); 
   simTr->clearJustCreatedFlags();
   ((RDOSimulatorTrace *)sim)->freeOperationId(operId);
}

RDOOperationTrace::RDOOperationTrace(RDOSimulatorTrace *i_sim):
   RDOTraceableObject(i_sim), RDOPattern(i_sim)
{
//	operId = RDOTraceableObject::sim->getFreeOperationId();
}

RDOOperationTrace::~RDOOperationTrace()
{
//   RDOTraceableObject::sim->freeOperationId(operId);
}

 
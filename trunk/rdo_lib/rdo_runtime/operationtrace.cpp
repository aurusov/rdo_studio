#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdotrace.h"
#include "ietrace.h"

void RDOOperationTrace::onAfterOperationBegin( RDOSimulator* sim )
{
	RDOSimulatorTrace* simTr = static_cast<RDOSimulatorTrace*>(sim);
	simTr->getTracer()->writeAfterOperationBegin( this, simTr );
}

void RDOOperationTrace::onAfterOperationEnd( RDOSimulator* sim )
{
	RDOSimulatorTrace* simTr = static_cast<RDOSimulatorTrace*>(sim);
	simTr->getTracer()->writeAfterOperationEnd(this, simTr); 
	((RDOSimulatorTrace *)sim)->freeOperationId(operId);
}

RDOOperationTrace::RDOOperationTrace( RDOSimulatorTrace* i_sim ):
	RDOTraceableObject( i_sim ),
	RDOPatternTrace()
{
	id = i_sim->activityCounter++;
//	operId = RDOTraceableObject::sim->getFreeOperationId();
}

RDOOperationTrace::~RDOOperationTrace()
{
//	RDOTraceableObject::sim->freeOperationId(operId);
}

 
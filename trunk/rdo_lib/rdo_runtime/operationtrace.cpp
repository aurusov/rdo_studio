#include "pch.h"
#include "operationtrace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime
{

RDOOperationTrace::RDOOperationTrace( RDORuntimeParent* _runtime, RDOSimulatorTrace* i_sim ): //qq
	RDOOperation( _runtime ),
	RDOTraceableObject( i_sim ),
	RDOPatternTrace()
{
	id = i_sim->activityCounter++;
//	operId = RDOTraceableObject::sim->getFreeOperationId();
}

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

RDOOperationTrace::~RDOOperationTrace()
{
//	RDOTraceableObject::sim->freeOperationId(operId);
}

} // namespace rdoRuntime

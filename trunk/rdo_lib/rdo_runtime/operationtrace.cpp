#include "pch.h"
#include "operationtrace.h"
#include "rdo_runtime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime
{

RDOOperationTrace::RDOOperationTrace( RDORuntime* runtime, bool trace ):
	RDOOperation( runtime ),
	RDOPatternTrace( trace )
{
	setTraceID( runtime->activityCounter++ );
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
	((RDOSimulatorTrace *)sim)->freeOperationId(m_operId);
}

RDOOperationTrace::~RDOOperationTrace()
{
//	RDOTraceableObject::sim->freeOperationId(operId);
}

} // namespace rdoRuntime

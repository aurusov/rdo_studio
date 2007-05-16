#include "pch.h"
#include "ruletrace.h"
#include "simtrace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime {

RDORuleTrace::RDORuleTrace( RDOSimulatorTrace* i_sim ):
	RDORule( i_sim ), //qq
	RDOTraceableObject( i_sim ),
	RDOPatternTrace(),
	trace( false )
{
	id = i_sim->activityCounter++;
}

void RDORuleTrace::onBeforeRule(RDOSimulator *sim)
{
}

void RDORuleTrace::onAfterRule( RDOSimulator* sim, bool inSearch )
{
	if ( !inSearch ) {
		RDOTraceableObject::sim->getTracer()->writeRule( this, RDOTraceableObject::sim );
	}
}

} // namespace rdoRuntime

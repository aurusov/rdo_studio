#include "pch.h"
#include "ruletrace.h"
#include "rdo_runtime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime {

RDORuleTrace::RDORuleTrace( RDORuntime* sim, bool trace ):
	RDORule( sim ),
	RDOPatternTrace( trace ),
	RDORuntimeContainer( sim )
{
	setTraceID( getRuntime()->activityCounter++ );
}

void RDORuleTrace::onBeforeRule(RDOSimulator *sim)
{
}

void RDORuleTrace::onAfterRule( RDOSimulator* sim, bool inSearch )
{
	if ( !inSearch ) {
		getRuntime()->getTracer()->writeRule( this, getRuntime() );
	}
}

} // namespace rdoRuntime

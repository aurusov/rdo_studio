#ifndef RULETRACE_H
#define RULETRACE_H

#include "rdo.h"
#include "rdotrace.h"

namespace rdoRuntime {

class RDORuleTrace: public RDORule, public RDOTraceableObject, protected RDOPatternTrace
{
friend class RDOTrace;

private:
   virtual const std::string& tracePatternId() const = 0;

protected:
	RDORuleTrace( RDOSimulatorTrace* sim );
	virtual void onBeforeRule( RDOSimulator* sim );
	virtual void onAfterRule( RDOSimulator* sim, bool inSearch = false );

public:
	bool trace;  
};

} // namespace rdoRuntime

#endif // RULETRACE_H

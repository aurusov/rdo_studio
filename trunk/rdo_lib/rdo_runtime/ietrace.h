#ifndef RDO_IETRACE
#define RDO_IETRACE

#include "rdo.h"
#include "rdotrace.h"

namespace rdoRuntime {

class RDOIETrace: public RDOIE, public RDOTraceableObject, protected RDOPatternTrace
{
friend RDOTrace;
friend RDOSimulatorTrace;

private:
	virtual const std::string& tracePatternId() const = 0;

protected:
	RDOIETrace( RDORuntimeParent* _runtime, RDOSimulatorTrace* sim ); //qq
	virtual void onAfterIrregularEvent( RDOSimulator* sim );
};

} // namespace rdoRuntime

#endif // RDO_IETRACE

#ifndef RDO_IETRACE
#define RDO_IETRACE

#include "rdo.h"
#include "rdotrace.h"

namespace rdoRuntime {

class RDOIETrace: public RDOIE, public RDOPatternTrace
{
friend RDOTrace;
friend RDOSimulatorTrace;

private:
	virtual const std::string& tracePatternId() const = 0;

protected:
	RDOIETrace( RDORuntimeParent* runtime, bool trace );
	virtual void onAfterIrregularEvent( RDOSimulator* sim );
};

} // namespace rdoRuntime

#endif // RDO_IETRACE

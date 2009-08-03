#ifndef RDO_IE_H
#define RDO_IE_H

#include "rdo.h"
#include "rdotrace.h"
#include "rdo_pattern.h"
#include "rdo_activity.h"
#include "rdo_ie_interface.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOIrregEvent
// ----------------------------------------------------------------------------
class RDOIrregEvent: public IBaseOperation, public IIrregEvent, public RDOActivityPattern<RDOPatternIrregEvent>
{
typedef RDOActivityPattern<RDOPatternIrregEvent> pattern_type;
RDO_IOBJECT(RDOIrregEvent);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(pattern_type)
	QUERY_INTERFACE(IBaseOperation)
	QUERY_INTERFACE(IIrregEvent)
QUERY_INTERFACE_END

friend class RDOTrace;

public:
	RDOIrregEvent( RDORuntime* runtime, RDOPatternIrregEvent* pattern, bool trace, const std::string& name );

private:
	double  m_time;

	void convertEvent( RDOSimulator* sim );

	double getNextTimeInterval( RDOSimulator* sim );
	virtual void onBeforeIrregularEvent( RDOSimulator* sim );
	virtual void onAfterIrregularEvent( RDOSimulator* sim );

	DECLARE_IBaseOperation;
	DECLARE_IIrregEvent;
};

} // namespace rdoRuntime

#endif // RDO_IE_H

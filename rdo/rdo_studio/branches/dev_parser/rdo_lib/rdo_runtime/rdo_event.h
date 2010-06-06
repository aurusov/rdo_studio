#ifndef RDO_EVENT_H
#define RDO_EVENT_H

#include "rdo_lib/rdo_runtime/rdo.h"
#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/rdo_pattern.h"
#include "rdo_lib/rdo_runtime/rdo_activity.h"
#include "rdo_lib/rdo_runtime/rdo_event_interface.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOEvent
// ----------------------------------------------------------------------------
class RDOEvent: public IBaseOperation, public IEvent, public RDOActivityPattern<RDOPatternEvent>
{
typedef RDOActivityPattern<RDOPatternEvent> pattern_type;
DEFINE_IFACTORY(RDOEvent);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(pattern_type)
	QUERY_INTERFACE(IBaseOperation)
	QUERY_INTERFACE(IEvent)
QUERY_INTERFACE_END

friend class RDOTrace;

private:
	RDOEvent( RDORuntime* runtime, RDOPatternEvent* pattern, bool trace, const std::string& name );

	double  m_time;

	void convertEvent( RDOSimulator* sim );

	virtual void onBeforeEvent( RDOSimulator* sim );
	virtual void onAfterEvent( RDOSimulator* sim );

	DECLARE_IBaseOperation;
	DECLARE_IEvent;
};

} // namespace rdoRuntime

#endif // RDO_EVENT_H

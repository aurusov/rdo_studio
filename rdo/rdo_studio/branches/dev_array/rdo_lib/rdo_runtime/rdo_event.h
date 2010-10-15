/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_event.h
 * author   : Урусов Андрей, Лущан Дмитрий
 * date     : 18.08.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_EVENT_H_
#define _RDO_EVENT_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo.h"
#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/rdo_pattern.h"
#include "rdo_lib/rdo_runtime/rdo_activity.h"
#include "rdo_lib/rdo_runtime/rdo_event_interface.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

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

	void convertEvent( RDOSimulator* sim );

	virtual void onBeforeEvent( RDOSimulator* sim );
	virtual void onAfterEvent( RDOSimulator* sim );

	DECLARE_IBaseOperation;
	DECLARE_IEvent;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDO_EVENT_H_

#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/rdo_pattern.h"
#include "simulator/runtime/rdo_activity.h"
#include "simulator/runtime/rdo_event_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOEvent: public IBaseOperation, public IEvent, public RDOActivityPattern<RDOPatternEvent>
{
typedef RDOActivityPattern<RDOPatternEvent> pattern_type;
DECLARE_FACTORY(RDOEvent);
friend class RDOTrace;

private:
	RDOEvent(const LPRDORuntime& pRuntime, const LPRDOPatternEvent& pPattern, bool trace, const std::string& name);

	void convertEvent(const LPRDORuntime& pRuntime);

	        void onBeforeEvent(const LPRDORuntime& pRuntime, const std::vector<RDOValue>& params);
	        void onAfterEvent (const LPRDORuntime& pRuntime);
	virtual void onMakePlaned (const LPRDORuntime& pRuntime, const std::vector<RDOValue>& params);

	DECLARE_IBaseOperation;
};

CLOSE_RDO_RUNTIME_NAMESPACE

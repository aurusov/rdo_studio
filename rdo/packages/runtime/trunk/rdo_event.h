/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_event.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      18.08.2010
  \brief     События
  \indent    4T
*/

#ifndef _LIB_RUNTIME_EVENT_H_
#define _LIB_RUNTIME_EVENT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/rdo_pattern.h"
#include "simulator/runtime/rdo_activity.h"
#include "simulator/runtime/rdo_event_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class     RDOEvent
  \brief     Событие
*/
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
	RDOEvent(CREF(LPRDORuntime) pRuntime, CREF(LPRDOPatternEvent) pPattern, rbool trace, CREF(tstring) name);

	void convertEvent(CREF(LPRDORuntime) pRuntime);

	virtual void onBeforeEvent(CREF(LPRDORuntime) pRuntime);
	virtual void onAfterEvent (CREF(LPRDORuntime) pRuntime);

	DECLARE_IBaseOperation;
	DECLARE_IEvent;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_EVENT_H_

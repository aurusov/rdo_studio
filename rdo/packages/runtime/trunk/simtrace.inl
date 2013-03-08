/*!
  \copyright (c) RDO-Team, 2011
  \file      simtrace.inl
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      06.08.2011
  \brief     Трассировка симулятора
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/rdo_simulator.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOSimulatorTrace
// --------------------------------------------------------------------------------
inline RDOSimulatorTrace::RDOSimulatorTrace()
	: RDOSimulator     (             )
	, m_tracer         (NULL         )
	, maxOperationId   (1            )
	, traceStartTime   (UNDEFINE_TIME)
	, traceEndTime     (UNDEFINE_TIME)
	, maxResourcesId   (0            )
	, m_ieCounter      (1            )
	, m_eventCounter   (1            )
	, m_activityCounter(1            )
	, m_dptCounter     (1            )
	, memory_current   (0            )
	, memory_max       (0            )
{}

inline PTR(RDOTrace) RDOSimulatorTrace::getTracer() const
{
	return m_tracer;
}

inline rbool RDOSimulatorTrace::canTrace() const
{
	return getTracer()->canTrace();
}

inline double RDOSimulatorTrace::getTraceStartTime() const
{
	return traceStartTime;
}

inline void RDOSimulatorTrace::setTraceStartTime(double value)
{
	traceStartTime = value;
}

inline double RDOSimulatorTrace::getTraceEndTime() const
{
	return traceEndTime;
}

inline void RDOSimulatorTrace::setTraceEndTime(double value)
{
	traceEndTime = value;
}

inline void RDOSimulatorTrace::onNewTimeNow()
{
	if (timeForTrace())
	{
		getTracer()->startWriting();
	}
	else
	{
		getTracer()->stopWriting();
	}
}

inline void RDOSimulatorTrace::memory_insert(ruint mem)
{
	memory_current += mem;
	if (memory_current > memory_max) memory_max = memory_current;
}

inline void RDOSimulatorTrace::memory_remove(ruint mem)
{
	memory_current -= mem;
}

inline ruint RDOSimulatorTrace::memory_get() const
{
	return memory_max;
}

inline int RDOSimulatorTrace::getFreeEventId()
{
	return m_eventCounter++;
}

inline int RDOSimulatorTrace::getFreeActivityId()
{
	return m_activityCounter++;
}

inline int RDOSimulatorTrace::getFreeDPTId()
{
	return m_dptCounter++;
}

inline rbool RDOSimulatorTrace::timeForTrace() const
{
	if (getTraceStartTime() != UNDEFINE_TIME && getTraceStartTime() > getCurrentTime())
		return false;
	if (getTraceEndTime()   != UNDEFINE_TIME && getTraceEndTime()   < getCurrentTime())
		return false;
	return true;
}

CLOSE_RDO_RUNTIME_NAMESPACE

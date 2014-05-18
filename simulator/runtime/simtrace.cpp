/*!
  \copyright (c) RDO-Team, 2011
  \file      simtrace.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.06.2006
  \brief     Трассировка симулятора
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
#include <set>

#include "utils/src/common/platform.h"
// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/debug/rdodebug.h"
#include "simulator/runtime/simtrace.h"
#include "simulator/runtime/searchtrace.h"
#include "simulator/runtime/rdo_rule.h"
#include "simulator/runtime/rdo_operation.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning(disable : 4786)
#endif // COMPILER_VISUAL_STUDIO

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOSimulatorTrace
// --------------------------------------------------------------------------------
RDOSimulatorTrace::RDOSimulatorTrace()
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

RDOSimulatorTrace::~RDOSimulatorTrace()
{
	if (m_tracer)
	{
		delete m_tracer;
		m_tracer = NULL;
	}
}

RDOTrace* RDOSimulatorTrace::getTracer() const
{
	return m_tracer;
}

bool RDOSimulatorTrace::canTrace() const
{
	return getTracer()->canTrace();
}

double RDOSimulatorTrace::getTraceStartTime() const
{
	return traceStartTime;
}

void RDOSimulatorTrace::setTraceStartTime(double value)
{
	traceStartTime = value;
}

double RDOSimulatorTrace::getTraceEndTime() const
{
	return traceEndTime;
}

void RDOSimulatorTrace::setTraceEndTime(double value)
{
	traceEndTime = value;
}

void RDOSimulatorTrace::onNewTimeNow()
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

void RDOSimulatorTrace::memory_insert(std::size_t mem)
{
	memory_current += mem;
	if (memory_current > memory_max) memory_max = memory_current;
}

void RDOSimulatorTrace::memory_remove(std::size_t mem)
{
	memory_current -= mem;
}

std::size_t RDOSimulatorTrace::memory_get() const
{
	return memory_max;
}

int RDOSimulatorTrace::getFreeEventId()
{
	return m_eventCounter++;
}

int RDOSimulatorTrace::getFreeActivityId()
{
	return m_activityCounter++;
}

int RDOSimulatorTrace::getFreeDPTId()
{
	return m_dptCounter++;
}

bool RDOSimulatorTrace::timeForTrace() const
{
	if (getTraceStartTime() != UNDEFINE_TIME && getTraceStartTime() > getCurrentTime())
		return false;
	if (getTraceEndTime()   != UNDEFINE_TIME && getTraceEndTime()   < getCurrentTime())
		return false;
	return true;
}

void RDOSimulatorTrace::copyFrom(const LPRDOSimulatorTrace& pOther)
{
	ASSERT(pOther);

	registeredResourcesId    = pOther->registeredResourcesId;
	maxResourcesId   = pOther->maxResourcesId;
}

void RDOSimulatorTrace::rdoInit()
{
	maxResourcesId = 0;
	maxOperationId = 1;
	registeredResourcesId.clear();
//	ASSERT(m_tracer != NULL);
	RDOSimulator::rdoInit();
}

void RDOSimulatorTrace::registerResourceId(std::size_t id)
{
	ASSERT(registeredResourcesId.find(id) == registeredResourcesId.end());
	registeredResourcesId.insert(id);
	maxResourcesId++;
}

std::size_t RDOSimulatorTrace::getResourceId()
{
#ifdef _DEBUG
		for (const auto& id: registeredResourcesId)
		{
			TRACE1("getRegisteredResourcesId: %d\n", id);
		}
#endif

	while (registeredResourcesId.find(maxResourcesId) != registeredResourcesId.end())
		maxResourcesId++;
	return maxResourcesId;
}

void RDOSimulatorTrace::eraseFreeResourceId(std::size_t id)
{
	MAPII::iterator it = resourcesIdsRefs.find(id);
	if (it != resourcesIdsRefs.end())
	{
		if(--(*it).second >= 1) return;
		resourcesIdsRefs.erase(it);
	}
	registeredResourcesId.erase(id);
}

void RDOSimulatorTrace::incrementResourceIdReference(int id)
{
	MAPII::iterator it = resourcesIdsRefs.find(id);
	if (it == resourcesIdsRefs.end())
	{
		resourcesIdsRefs.insert(MAPII::value_type(id, 2));
	}
	else
	{
		(*it).second++;
	}
}

int RDOSimulatorTrace::getFreeOperationId()
{
	if (freeOperationsIds.empty())
	{
		return maxOperationId++;
	}
	else
	{
		int id = freeOperationsIds.front();
		freeOperationsIds.pop_front();
		return id;
	}
}

void RDOSimulatorTrace::freeOperationId(int id)
{
	freeOperationsIds.push_front(id);
}

void RDOSimulatorTrace::onResourceErase(const LPRDOResource& pResource)
{
	eraseFreeResourceId(pResource->getTraceID());
}

void RDOSimulatorTrace::preProcess()
{
	RDOSimulator::preProcess();
	getTracer()->startWriting();
	LPRDORuntime pRuntime = static_cast<RDORuntime*>(this);
	getTracer()->writeTraceBegin(pRuntime);
	getTracer()->writePermanentResources(pRuntime, getResourcesBeforeSim());
	getTracer()->writeModelBegin(pRuntime);
	getTracer()->startWriting();
	onCheckResult();
	onAfterCheckResult();
}

void RDOSimulatorTrace::postProcess()
{
	LPRDORuntime pRuntime = static_cast<RDORuntime*>(this);
	getTracer()->writeTraceEnd(pRuntime);
//	getTracer()->stopWriting();
}

CLOSE_RDO_RUNTIME_NAMESPACE

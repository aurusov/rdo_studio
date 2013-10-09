/*!
  \copyright (c) RDO-Team, 2011
  \file      simtrace.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.06.2006
  \brief     Трассировка симулятора
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdodebug.h"
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
RDOSimulatorTrace::~RDOSimulatorTrace()
{
	if (m_tracer)
	{
		delete m_tracer;
		m_tracer = NULL;
	}
}

void RDOSimulatorTrace::copyFrom(CREF(LPRDOSimulatorTrace) pOther)
{
	ASSERT(pOther);

	freeResourcesIds = pOther->freeResourcesIds;
	maxResourcesId   = pOther->maxResourcesId;
	m_activityCounter = pOther->m_activityCounter;
	m_dptCounter = pOther->m_dptCounter;
	memory_current = pOther->memory_current;
	memory_max = pOther->memory_max;

	parent_type::copyFrom(pOther.object_parent_cast<parent_type>());
}

void RDOSimulatorTrace::rdoInit()
{
	maxResourcesId = 0;
	maxOperationId = 1;
//	ASSERT(m_tracer != NULL);
	RDOSimulator::rdoInit();
}

ruint RDOSimulatorTrace::getResourceId()
{
	if (freeResourcesIds.empty())
	{
		return maxResourcesId++;
	}
	else
	{
#ifdef _DEBUG
		STL_FOR_ALL(freeResourcesIds, it)
		{
			TRACE1("getFreeResourceId: %d\n", *it);
		}
#endif
		ruint id = freeResourcesIds.back();
		freeResourcesIds.pop_back();
		return id;
	}
}

void RDOSimulatorTrace::eraseFreeResourceId(ruint id)
{
	MAPII::iterator it = resourcesIdsRefs.find(id);
	if (it != resourcesIdsRefs.end())
	{
		if(--(*it).second >= 1) return;
		resourcesIdsRefs.erase(it);
	}
	freeResourcesIds.push_back(id);
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

void RDOSimulatorTrace::onResourceErase(CREF(LPRDOResource) pResource)
{
	eraseFreeResourceId(pResource->getTraceID());
}

void RDOSimulatorTrace::preProcess()
{
	RDOSimulator::preProcess();
	getTracer()->startWriting();
	LPRDORuntime pRuntime = static_cast<PTR(RDORuntime)>(this);
	getTracer()->writeTraceBegin(pRuntime);
	getTracer()->writePermanentResources(pRuntime, getResourcesBeforeSim());
	getTracer()->writeModelBegin(pRuntime);
	getTracer()->startWriting();
	onCheckResult();
	onAfterCheckResult();
}

void RDOSimulatorTrace::postProcess()
{
	LPRDORuntime pRuntime = static_cast<PTR(RDORuntime)>(this);
	getTracer()->writeTraceEnd(pRuntime);
//	getTracer()->stopWriting();
}

CLOSE_RDO_RUNTIME_NAMESPACE

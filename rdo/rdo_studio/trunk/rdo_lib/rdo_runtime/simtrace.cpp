#include "rdo_lib/rdo_runtime/pch.h"
#include "rdo_common/rdodebug.h"
#include "rdo_lib/rdo_runtime/simtrace.h"
#include "rdo_lib/rdo_runtime/searchtrace.h"
#include "rdo_lib/rdo_runtime/rdo_rule.h"
#include "rdo_lib/rdo_runtime/rdo_operation.h"
#include "rdo_lib/rdo_runtime/rdo_resource.h"

#pragma warning(disable : 4786)  

namespace rdoRuntime
{

RDOSimulatorTrace::~RDOSimulatorTrace()
{
	if ( m_tracer ) {
		delete m_tracer;
		m_tracer = NULL;
	}
}

void RDOSimulatorTrace::copyFrom(CREF(LPRDOSimulatorTrace) pOther)
{
	ASSERT(pOther);

	freeResourcesIds = pOther->freeResourcesIds;
	maxResourcesId   = pOther->maxResourcesId;
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
	MAPII::iterator it = resourcesIdsRefs.find( id );
	if ( it != resourcesIdsRefs.end() ) {
		if( --(*it).second >= 1 ) return;
		resourcesIdsRefs.erase( it );
	}
	freeResourcesIds.push_back( id ); 
}

void RDOSimulatorTrace::incrementResourceIdReference( int id )
{
	MAPII::iterator it = resourcesIdsRefs.find( id );
	if ( it == resourcesIdsRefs.end() ) {
		resourcesIdsRefs.insert( MAPII::value_type(id, 2) );
	} else {
		(*it).second++;
	}
}

int RDOSimulatorTrace::getFreeOperationId() 
{
	if ( freeOperationsIds.empty() ) {
		return maxOperationId++;
	} else {
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
	onCheckPokaz();
	onAfterCheckPokaz();
}

void RDOSimulatorTrace::postProcess()
{
	LPRDORuntime pRuntime = static_cast<PTR(RDORuntime)>(this);
	getTracer()->writeTraceEnd(pRuntime);
//	getTracer()->stopWriting();
}

} // namespace rdoRuntime

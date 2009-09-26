#include "pch.h"
#include "simtrace.h"
#include "searchtrace.h"
#include "rdo_ie.h"
#include "rdo_rule.h"
#include "rdo_operation.h"
#include "rdo_resource.h"
#include <rdodebug.h>

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

void RDOSimulatorTrace::operator= (const RDOSimulatorTrace& other)
{
	freeResourcesIds = other.freeResourcesIds;
	maxResourcesId   = other.maxResourcesId;
}

void RDOSimulatorTrace::rdoInit()
{
	maxResourcesId = 0;
	maxOperationId = 1;
//	ASSERT(m_tracer != NULL);
	RDOSimulator::rdoInit();
}

int RDOSimulatorTrace::getFreeResourceId( int _id )
{
	if ( freeResourcesIds.empty() ) {
		if ( _id == -1 ) {
			// Для новых ресурсов в процессе работы
			maxResourcesId++;
			return maxResourcesId - 1;
		} else {
			// id может быть больше maxResourcesId только при,
			// инициализации RSS, в котором ресурсы могут инититься
			// не по порядку в RSS, а по RTP
			if ( _id >= maxResourcesId ) {
				maxResourcesId = _id + 1;
			}
			return _id;
		}
	} else {
#ifdef _DEBUG
		std::list< int >::const_iterator it = freeResourcesIds.begin();
		while ( it != freeResourcesIds.end() ) {
			TRACE1("getFreeResourceId: %d\n", *it);
			it++;
		}
#endif
//		freeResourcesIds.sort();
//#ifdef _DEBUG
//		it = freeResourcesIds.begin();
//		while ( it != freeResourcesIds.end() ) {
//			TRACE( "%d\n", *it );
//			it++;
//		}
//#endif
		int id = freeResourcesIds.back();
		freeResourcesIds.pop_back();
		return id;
	}
}

void RDOSimulatorTrace::eraseFreeResourceId( int id ) 
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

void RDOSimulatorTrace::onResourceErase( RDOResource* res )
{
	eraseFreeResourceId( res->getTraceID() );
}

void RDOSimulatorTrace::preProcess()
{
	RDOSimulator::preProcess();
	getTracer()->startWriting();
	getTracer()->writeTraceBegin(this);
	getTracer()->writePermanentResources( this, getResourcesBeforeSim() );
	getTracer()->writeModelBegin(this);
	getTracer()->startWriting();
	onCheckPokaz();
	onAfterCheckPokaz();
}

void RDOSimulatorTrace::postProcess()
{
	getTracer()->writeTraceEnd( this );
//	getTracer()->stopWriting();
}

} // namespace rdoRuntime

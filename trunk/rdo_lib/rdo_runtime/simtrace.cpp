#include "pch.h"
#include "simtrace.h"
#include "searchtrace.h"
#include "operationtrace.h"
#include "ietrace.h"
#include "ruletrace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime
{

void RDOSimulatorTrace::addTemplateDecisionPoint( RDODecisionPointTrace* dp )
{
	dp->id = dptCounter++;
	RDOSimulator::addTemplateBaseOperation( dp );
}

void RDOSimulatorTrace::addTemplateOperation( RDOOperationTrace* op )
{
	RDOSimulator::addTemplateBaseOperation( op );
}

void RDOSimulatorTrace::addTemplateIrregularEvent( RDOIETrace* ev )
{
	ev->id = ieCounter++;
	RDOSimulator::addTemplateBaseOperation( ev );
}

void RDOSimulatorTrace::addTemplateRule( RDORuleTrace* rule )
{
	RDOSimulator::addTemplateBaseOperation( rule );
}

void RDOSimulatorTrace::addTemplateBaseOperation( RDOBaseOperation* bop )
{
	RDORuleTrace* rule = dynamic_cast<RDORuleTrace*>(bop);
	if ( rule ) {
		addTemplateRule( rule );
	} else {
		RDOIETrace* ie = dynamic_cast<RDOIETrace*>(bop);
		if ( ie ) {
			addTemplateIrregularEvent( ie );
		} else {
			RDOOperationTrace* op = dynamic_cast<RDOOperationTrace*>(bop);
			if ( op ) {
				addTemplateOperation( op );
			} else {
				RDODecisionPointTrace* dp = dynamic_cast<RDODecisionPointTrace*>(bop);
				if ( dp ) {
					addTemplateDecisionPoint( dp );
				} else {
					RDOSimulator::addTemplateBaseOperation( bop );
				}
			}
		}
	}
}

void RDOSimulatorTrace::rdoInit()
{
	maxResourcesId = 0;
	maxOperationId = 1;
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
			TRACE( "getFreeResourceId: %d\n", *it );
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

void RDOSimulatorTrace::onResourceErase( RDOResourceTrace* res )
{
	eraseFreeResourceId( res->id );
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

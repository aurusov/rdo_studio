#include "pch.h"
#include "rdoprocess.h"
#include "rdocalc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOPROCProcess
// ----------------------------------------------------------------------------
RDOPROCProcess::RDOPROCProcess( const std::string& _name, RDOSimulator* sim ):
	RDOBaseOperation( static_cast<RDORuntime*>(sim) ),
	name( _name ),
	parent( NULL )
{
	static_cast<RDORuntime*>(sim)->addRuntimeProcess( this );
}

void RDOPROCProcess::insertChild( RDOPROCProcess* value )
{
	if ( value ) {
		child.push_back( value );
		value->parent = this;
	}
}

void RDOPROCProcess::init( RDOSimulator* sim )
{
	std::list< RDOPROCBlock* >::iterator it = blocks.begin();
	while ( it != blocks.end() ) {
		(*it)->init( sim );
		it++;
	}
}

RDOBaseOperation::BOResult RDOPROCProcess::checkOperation( RDOSimulator* sim )
{
	if ( std::find_if( blocks.begin(), blocks.end(), CheckOperations( sim ) ) != blocks.end() ) {
		return RDOBaseOperation::BOR_can_run;
	}
	return RDOBaseOperation::BOR_cant_run;
}

void RDOPROCProcess::next( RDOPROCTransact* transact )
{
	std::list< RDOPROCBlock* >::iterator it = std::find( blocks.begin(), blocks.end(), transact->block );
	if ( it != blocks.end() ) {
		RDOPROCBlock* block = *it;
		std::list< RDOPROCTransact* >::iterator it_res = std::find( block->transacts.begin(), block->transacts.end(), transact );
		if ( it_res != block->transacts.end() ) {
			block->transacts.erase( it_res );
		}
		it++;
		if ( it != blocks.end() ) {
			transact->block = *it;
			(*it)->transacts.push_back( transact );
		} else {
			// run-time error
		}
	} else {
		// run-time error
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCTransact
// ----------------------------------------------------------------------------
int RDOPROCTransact::typeID = -1;

RDOPROCTransact::RDOPROCTransact( RDOSimulator* sim, RDOPROCBlock* _block ):
	RDOResource( static_cast<RDORuntime*>(sim), -1, true ),
	block( _block )
{
	static_cast<RDORuntime*>(sim)->insertNewResource( this );
	type        = typeID;
	trace       = true;
	temporary   = true;
	state       = RDOResourceTrace::CS_Create;
	params.push_back( sim->getCurrentTime() );
	params.push_back( 0 );
}

void RDOPROCTransact::next()
{
	block->process->next( this );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlock
// ----------------------------------------------------------------------------
RDOPROCBlock::RDOPROCBlock( RDOPROCProcess* _process ):
	RDOBaseOperation( _process ),
	process( _process )
{
	process->blocks.push_back( this );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCGenerate
// ----------------------------------------------------------------------------
void RDOPROCGenerate::init( RDOSimulator* sim )
{
	calcNextTimeInterval( sim );
}

RDOBaseOperation::BOResult RDOPROCGenerate::checkOperation( RDOSimulator* sim )
{
	if ( sim->getCurrentTime() >= timeNext ) {
		calcNextTimeInterval( sim );
		RDOPROCTransact* transact = new RDOPROCTransact( sim, this );
		RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
		if ( !tracer->isNull() ) {
			tracer->getOStream() << transact->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
		}
		TRACE( "%7.1f GENERATE\n", sim->getCurrentTime() );
		transact->next();
		return RDOBaseOperation::BOR_can_run;
	}
	return RDOBaseOperation::BOR_cant_run;
}

void RDOPROCGenerate::calcNextTimeInterval( RDOSimulator* sim )
{
	sim->addTimePoint( timeNext = timeCalc->calcValueBase( static_cast<RDORuntime*>(sim) ).getDouble() + sim->getCurrentTime(), process, this );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
RDOBaseOperation::BOResult RDOPROCSeize::checkOperation( RDOSimulator* sim )
{
	if ( !transacts.empty() ) {
		RDOResource* rss = static_cast<RDORuntime*>(sim)->getResourceByID( rss_id );

		RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
		if ( !tracer->isNull() ) {
			tracer->getOStream() << rss->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
		}

		// Свободен
		if ( rss->params[0] == 0.0 ) {
			rss->params[0] = 1.0;
			transacts.front()->next();
			TRACE( "%7.1f SEIZE\n", sim->getCurrentTime() );
			return RDOBaseOperation::BOR_can_run;
		} else {
			TRACE( "%7.1f SEIZE CANNOT\n", sim->getCurrentTime() );
		}
	}
	return RDOBaseOperation::BOR_cant_run;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
RDOBaseOperation::BOResult RDOPROCRelease::checkOperation( RDOSimulator* sim )
{
	if ( !transacts.empty() ) {
		RDOResource* rss = static_cast<RDORuntime*>(sim)->getResourceByID( rss_id );

		RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
		if ( !tracer->isNull() ) {
			tracer->getOStream() << rss->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
		}

		// Занят
		if ( rss->params[0] == 1.0 ) {
			rss->params[0] = 0.0;
			transacts.front()->next();
			TRACE( "%7.1f RELEASE\n", sim->getCurrentTime() );
			return RDOBaseOperation::BOR_can_run;
		} else {
			TRACE( "%7.1f RELEASE CANNOT\n", sim->getCurrentTime() );
		}
	}
	return RDOBaseOperation::BOR_cant_run;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCAdvance
// ----------------------------------------------------------------------------
RDOBaseOperation::BOResult RDOPROCAdvance::checkOperation( RDOSimulator* sim )
{
	if ( !transacts.empty() ) {
		double timeLeave = delayCalc->calcValueBase( static_cast<RDORuntime*>(sim) ).getDouble() + sim->getCurrentTime();
		leave_list.push_back( LeaveTr(transacts.front(), timeLeave) );
		transacts.erase( transacts.begin() );
		sim->addTimePoint( timeLeave, process, this );
		TRACE( "%7.1f ADVANCE BEGIN\n", sim->getCurrentTime() );
//		RDOTrace* tracer = sim->getTracer();
//		if ( !tracer->isNull() ) {
//			tracer->getOStream() << res->traceResourceState('\0', sim) << tracer->getEOL();
//		}
		return RDOBaseOperation::BOR_can_run;
	} else if ( !leave_list.empty() ) {
		double tnow = sim->getCurrentTime();
		std::list< LeaveTr >::iterator it = leave_list.begin();
		while ( it != leave_list.end() ) {
			if ( tnow >= it->timeLeave ) {
//				RDOTrace* tracer = sim->getTracer();
//				if ( !tracer->isNull() ) {
//					tracer->getOStream() << res->traceResourceState('\0', sim) << tracer->getEOL();
//				}
				it->transact->next();
				TRACE( "%7.1f ADVANCE END\n", it->timeLeave );
				leave_list.erase( it );
				return RDOBaseOperation::BOR_cant_run;
			}
			it++;
		}
	}
	return RDOBaseOperation::BOR_cant_run;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
RDOBaseOperation::BOResult RDOPROCTerminate::checkOperation( RDOSimulator* sim )
{
	if ( !transacts.empty() ) {
		RDOPROCTransact* transact = transacts.front();
		transact->setState( RDOResourceTrace::CS_Erase );
		RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
		if ( !tracer->isNull() ) {
			tracer->getOStream() << transact->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
		}
		static_cast<RDORuntime*>(sim)->onEraseRes( transact->getTraceID(), NULL );
		transacts.erase( transacts.begin() );
		TRACE( "%7.1f TERMINATE\n", sim->getCurrentTime() );
		return RDOBaseOperation::BOR_can_run;
	}
	return RDOBaseOperation::BOR_cant_run;
}

} // rdoRuntime

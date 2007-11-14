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
// ---------- RDOPROCResource
// ----------------------------------------------------------------------------
RDOPROCResource::RDOPROCResource( RDORuntime* _runtime, int _number, bool _trace ):
	RDOResource( _runtime, _number, _trace )
{
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
		TRACE( "%7.1f GENERATE\n", sim->getCurrentTime() );
		calcNextTimeInterval( sim );
		RDOPROCTransact* transact = new RDOPROCTransact( sim, this );
		RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
		if ( !tracer->isNull() ) {
			tracer->getOStream() << transact->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
		}
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
// ---------- RDOPROCBlockForSeize
// ----------------------------------------------------------------------------
RDOPROCBlockForSeize::RDOPROCBlockForSeize( RDOPROCProcess* _process, int _rss_id ):
	RDOPROCBlock( _process ),
	rss( NULL ),
	rss_id( _rss_id )
{
}

void RDOPROCBlockForSeize::init( RDOSimulator* sim )
{
	// todo: если потребуется стоить деревья, вершинами которых будут полные снимки БД,
	// как при DPT search, то инициализацию атрибутов надо будет делать в checkOperation
	rss = static_cast<RDORuntime*>(sim)->getResourceByID( rss_id );
	enum_free = RDOValue( rss->params[0].getEnum(), RDOPROCBlockForSeize::getStateEnumFree() );
	enum_buzy = RDOValue( rss->params[0].getEnum(), RDOPROCBlockForSeize::getStateEnumBuzy() );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
RDOBaseOperation::BOResult RDOPROCSeize::checkOperation( RDOSimulator* sim )
{
	if ( !transacts.empty() ) {
		RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
		if ( !tracer->isNull() ) {
			tracer->getOStream() << rss->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
		}
		// Свободен
		if ( rss->params[0] == enum_free ) {
			TRACE( "%7.1f SEIZE\n", sim->getCurrentTime() );
			rss->params[0] = enum_buzy;
			transacts.front()->next();
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
		RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
		if ( !tracer->isNull() ) {
			tracer->getOStream() << rss->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
		}
		// Занят
		if ( rss->params[0] == enum_buzy ) {
			TRACE( "%7.1f RELEASE\n", sim->getCurrentTime() );
			rss->params[0] = enum_free;
			transacts.front()->next();
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
		TRACE( "%7.1f ADVANCE BEGIN\n", sim->getCurrentTime() );
		double timeLeave = delayCalc->calcValueBase( static_cast<RDORuntime*>(sim) ).getDouble() + sim->getCurrentTime();
		leave_list.push_back( LeaveTr(transacts.front(), timeLeave) );
		transacts.erase( transacts.begin() );
		sim->addTimePoint( timeLeave, process, this );
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
				TRACE( "%7.1f ADVANCE END\n", it->timeLeave );
				it->transact->next();
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
		TRACE( "%7.1f TERMINATE\n", sim->getCurrentTime() );
		RDOPROCTransact* transact = transacts.front();
		transact->setState( RDOResourceTrace::CS_Erase );
		RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
		if ( !tracer->isNull() ) {
			tracer->getOStream() << transact->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
		}
		static_cast<RDORuntime*>(sim)->onEraseRes( transact->getTraceID(), NULL );
		transacts.erase( transacts.begin() );
		return RDOBaseOperation::BOR_can_run;
	}
	return RDOBaseOperation::BOR_cant_run;
}

} // rdoRuntime

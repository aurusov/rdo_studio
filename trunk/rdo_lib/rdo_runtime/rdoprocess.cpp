#include "pch.h"
#include "rdoprocess.h"
#include <rdoruntime.h>
#include <rdodpt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime {

class RDOPROCCheckOperations
{
private:
	RDORuntime* sim;

public:
   RDOPROCCheckOperations( RDORuntime* _sim ): sim( _sim ) {
   }
   bool operator()( RDOPROCBlock* op ) {
	   return op->checkOperation( sim );
   }
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCProcess
// ----------------------------------------------------------------------------
RDOPROCProcess::RDOPROCProcess( const std::string& _name, RDORuntime* sim ):
	RDOBaseOperation(),
	name( _name ),
	parent( NULL )
{
	sim->addRuntimeProcess( this );
}

void RDOPROCProcess::insertChild( RDOPROCProcess* value )
{
	if ( value ) {
		child.push_back( value );
		value->parent = this;
	}
}

bool RDOPROCProcess::checkOperation( RDOSimulator* sim )
{
	if( std::find_if( blocks.begin(), blocks.end(), RDOPROCCheckOperations(static_cast<RDORuntime*>(sim)) ) != blocks.end() ) {
		return true;
	}
	return false;
}

void RDOPROCProcess::preProcess( RDOSimulator* sim )
{
	std::list< RDOPROCBlock* >::iterator it = blocks.begin();
	while ( it != blocks.end() ) {
		RDOPROCGenerate* generate = dynamic_cast<RDOPROCGenerate*>(*it);
		if ( generate ) {
			generate->calcNextTimeInterval( static_cast<RDORuntime*>(sim) );
		}
		it++;
	}
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
RDOPROCTransact::RDOPROCTransact( RDORuntime* sim, RDOPROCBlock* _block ):
	RDOResource( sim ),
	block( _block )
{
	sim->insertNewResource( this );
	type        = rdoParse::RDOPROCTransact::makeRTP()->getNumber();
	trace       = true;
	tempotary   = true;
	justCreated = true;
	params.push_back( sim->getTimeNow() );
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
	process( _process )
{
	process->blocks.push_back( this );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCGenerate
// ----------------------------------------------------------------------------
bool RDOPROCGenerate::checkOperation( RDORuntime* sim )
{
	if ( sim->getTimeNow() >= timeNext ) {
		calcNextTimeInterval( sim );
		RDOPROCTransact* res = new RDOPROCTransact( sim, this );

		RDOTrace* tracer = sim->getTracer();
		if ( !tracer->isNull() ) {
			tracer->getOStream() << res->traceResourceState('\0', sim) << tracer->getEOL();
		}

		res->next();
		return true;
	}
	return false;
}

void RDOPROCGenerate::calcNextTimeInterval( RDORuntime* sim )
{
	sim->addTimePoint( timeNext = timeCalc->calcValueBase( sim ) + sim->getTimeNow() );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
bool RDOPROCTerminate::checkOperation( RDORuntime* sim )
{
	if ( !transacts.empty() ) {
		RDOPROCTransact* res = transacts.front();
		sim->eraseRes( res->number, NULL );
		transacts.erase( transacts.begin() );
		return true;
	}
	return false;
}

} // rdoRuntime

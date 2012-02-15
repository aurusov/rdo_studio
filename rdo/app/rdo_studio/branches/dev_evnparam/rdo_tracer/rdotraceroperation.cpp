#include "app/rdo_studio_mfc/pch/stdpch.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotraceroperation.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracervalues.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOTracerOperationBase
// --------------------------------------------------------------------------------
RDOTracerOperationBase::RDOTracerOperationBase( RDOTracerPattern* const pat )
	: RDOTracerSerie( RDOST_OPERATION ),
	pattern( pat )
{
}

RDOTracerOperationBase::~RDOTracerOperationBase()
{
}

void RDOTracerOperationBase::incOperationsCount( RDOTracerTimeNow* const time, const int eventIndex )
{
	RDOTracerValue* newval = new RDOTracerValue( time, eventIndex );
	RDOTracerValue* prevval;
	getLastValue( prevval );
	newval->value = prevval ? prevval->value + 1 : 1;
	addValue( newval );
}

void RDOTracerOperationBase::getCaptions( std::vector<std::string> &captions, const int val_count ) const
{
	RDOTracerSerie::getCaptionsInt( captions, val_count );
}

void RDOTracerOperationBase::monitorTime( RDOTracerTimeNow* const time, const int eventIndex )
{
	UNUSED(time      );
	UNUSED(eventIndex);
}

// --------------------------------------------------------------------------------
// -------------------- RDOTracerOperation
// --------------------------------------------------------------------------------
RDOTracerOperation::RDOTracerOperation( RDOTracerPattern* const pat ) :
	RDOTracerOperationBase( pat )
{
}

RDOTracerOperation::~RDOTracerOperation()
{
}

void RDOTracerOperation::start( RDOTracerTimeNow* const time, const int eventIndex )
{
	RDOTracerOperationBase::incOperationsCount( time, eventIndex );
}

void RDOTracerOperation::accomplish( RDOTracerTimeNow* const time, const int eventIndex )
{
	RDOTracerValue* lastval;
	getLastValue( lastval );
	if ( lastval ) {
		RDOTracerValue* newval = new RDOTracerValue( time, eventIndex );
		newval->value = lastval->value - 1;
		addValue( newval );
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOTracerIrregularEvent
// --------------------------------------------------------------------------------
RDOTracerEvent::RDOTracerEvent( RDOTracerPattern* const pat ) :
	RDOTracerOperationBase( pat )
{
}

RDOTracerEvent::~RDOTracerEvent()
{
}

void RDOTracerEvent::occurs( RDOTracerTimeNow* const time, const int eventIndex )
{
	RDOTracerOperationBase::incOperationsCount( time, eventIndex );
}

void RDOTracerEvent::monitorTime( RDOTracerTimeNow* const time, const int eventIndex )
{
	RDOTracerValue* prevval;
	getLastValue( prevval );
	RDOTracerValue* newval = NULL;
	if ( prevval && prevval->value != 0 ) {
		if ( *prevval->getModelTime() != *time ) {
			newval = new RDOTracerValue( prevval->getModelTime(), prevval->getModelTime()->eventCount );
			newval->value = 0;
		}
	}
	if ( !prevval ) {
		newval = new RDOTracerValue( time, eventIndex );
		newval->value = 0;
	}
	if( newval ) {
		addValue( newval );
	}
}
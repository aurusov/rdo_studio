#include "stdafx.h"
#include "rdotraceroperation.h"
#include "rdotracervalues.h"

// ----------------------------------------------------------------------------
// ---------- RDOTracerOperation
// ----------------------------------------------------------------------------
RDOTracerOperation::RDOTracerOperation( RDOTracerPattern* const pat )
	: RDOTracerSerie( RDOST_OPERATION ),
	pattern( pat )
{
}

RDOTracerOperation::~RDOTracerOperation()
{
}

void RDOTracerOperation::start( RDOTracerTimeNow* const time, const int eventIndex )
{
	RDOTracerValue* newval = new RDOTracerValue( time, eventIndex );
	RDOTracerValue* prevval = getLastValue();
	newval->value = prevval ? prevval->value + 1 : 1;
	addValue( newval );
}

void RDOTracerOperation::accomplish( RDOTracerTimeNow* const time, const int eventIndex )
{
	RDOTracerValue* newval = new RDOTracerValue( time, eventIndex );
	newval->value = getLastValue()->value - 1;
	addValue( newval );
}

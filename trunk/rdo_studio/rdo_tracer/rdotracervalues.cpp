#include "stdafx.h"
#include "rdotracervalues.h"

// ----------------------------------------------------------------------------
// ---------- RDOTracerValue
// ----------------------------------------------------------------------------
RDOTracerValue::RDOTracerValue( RDOTracerTimeNow* const timenow, const int _eventIndex )
	: modeltime( timenow ),
	eventIndex( _eventIndex )
{
}

RDOTracerValue::~RDOTracerValue()
{
}

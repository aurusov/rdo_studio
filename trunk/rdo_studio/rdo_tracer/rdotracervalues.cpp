#include "stdafx.h"
#include "rdotracervalues.h"

// ----------------------------------------------------------------------------
// ---------- RDOTracerTimeNow
// ----------------------------------------------------------------------------
RDOTracerTimeNow::RDOTracerTimeNow( const double _time ) : time( _time )
{
	eventCount = 0;
}

RDOTracerTimeNow& RDOTracerTimeNow::operator =( const RDOTracerTimeNow& timenow )
{
	time         = timenow.time;
	eventCount   = timenow.eventCount;

	return *this;
}

bool RDOTracerTimeNow::operator ==( const RDOTracerTimeNow& timenow ) const
{
	return time         == timenow.time &&
	       eventCount   == timenow.eventCount;
}

bool RDOTracerTimeNow::operator !=( const RDOTracerTimeNow& timenow ) const
{
	return !(*this == timenow);
}

bool RDOTracerTimeNow::compareTimes( const RDOTracerTimeNow* timenow )
{
	bool res = false;
	if( timenow ) {
		res = ( time >= timenow->time );
	}
	return res;
}

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

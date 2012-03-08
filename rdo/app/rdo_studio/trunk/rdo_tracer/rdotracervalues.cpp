#include "app/rdo_studio_mfc/pch/stdpch.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracervalues.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOTracerTimeNow
// --------------------------------------------------------------------------------
RDOTracerTimeNow::RDOTracerTimeNow( const double _time ) : time( _time )
{
	eventCount = 0;
}

RDOTracerTimeNow::RDOTracerTimeNow( const double _time, const int _event_count )
{
	time = _time;
	eventCount = _event_count;
}

RDOTracerTimeNow& RDOTracerTimeNow::operator =( const RDOTracerTimeNow& timenow )
{
	time         = timenow.time;
	eventCount   = timenow.eventCount;

	return *this;
}

rbool RDOTracerTimeNow::operator ==( const RDOTracerTimeNow& timenow ) const
{
	return time         == timenow.time &&
	       eventCount   == timenow.eventCount;
}

rbool RDOTracerTimeNow::operator !=( const RDOTracerTimeNow& timenow ) const
{
	return !(*this == timenow);
}

rbool RDOTracerTimeNow::compareTimes( const RDOTracerTimeNow* timenow )
{
	rbool res = false;
	if( timenow ) {
		res = ( time >= timenow->time );
	}
	return res;
}

// --------------------------------------------------------------------------------
// -------------------- RDOTracerValue
// --------------------------------------------------------------------------------
RDOTracerValue::RDOTracerValue( RDOTracerTimeNow* const timenow, const int _eventIndex, const double _value )
	: modeltime( timenow ),
	eventIndex( _eventIndex ),
	value( _value )
{
}

RDOTracerValue::~RDOTracerValue()
{
}

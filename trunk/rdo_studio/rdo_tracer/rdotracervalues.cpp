#include "stdafx.h"
#include "rdotracervalues.h"

// ----------------------------------------------------------------------------
// ---------- RDOTracerTimeNow
// ----------------------------------------------------------------------------
bool RDOTracerTimeNow::checkTimeMoreOrEqual( RDOTracerTimeNow* timenow )
{
	bool res = false;
	if( timenow ) {
		res = time >= timenow->time;
	}
	return res;
}

bool RDOTracerTimeNow::operator >=( const RDOTracerTimeNow* timenow ) const
{
	bool res = false;
	if( timenow ) {
		res = time >= timenow->time;
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

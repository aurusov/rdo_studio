/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracervalues.cpp
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/rdo_tracer/rdotracervalues.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- TracerTimeNow
// --------------------------------------------------------------------------------
TracerTimeNow::TracerTimeNow(const double _time)
	: time(_time)
{
	eventCount = 0;
}

TracerTimeNow::TracerTimeNow(const double _time, const int _event_count)
{
	time = _time;
	eventCount = _event_count;
}

TracerTimeNow& TracerTimeNow::operator =(const TracerTimeNow& timenow)
{
	time = timenow.time;
	eventCount = timenow.eventCount;

	return *this;
}

rbool TracerTimeNow::operator ==(const TracerTimeNow& timenow) const
{
	return time == timenow.time && eventCount == timenow.eventCount;
}

rbool TracerTimeNow::operator !=(const TracerTimeNow& timenow) const
{
	return !(*this == timenow);
}

rbool TracerTimeNow::compareTimes(const TracerTimeNow* timenow)
{
	rbool res = false;
	if (timenow)
	{
		res = (time >= timenow->time);
	}
	return res;
}

// --------------------------------------------------------------------------------
// -------------------- TracerValue
// --------------------------------------------------------------------------------
TracerValue::TracerValue(TracerTimeNow* const timenow, const int _eventIndex, const double _value)
	: modeltime(timenow),
	  eventIndex(_eventIndex),
	  value(_value)
{}

TracerValue::~TracerValue()
{}

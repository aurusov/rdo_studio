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
TracerTimeNow::TracerTimeNow(const double time)
	: time(time)
{
	eventCount = 0;
}

TracerTimeNow::TracerTimeNow(const double time, const int eventCount)
	: time(time)
	, eventCount(eventCount)
{}

TracerTimeNow& TracerTimeNow::operator= (const TracerTimeNow& timeNow)
{
	time       = timeNow.time;
	eventCount = timeNow.eventCount;

	return *this;
}

rbool TracerTimeNow::operator== (const TracerTimeNow& timeNow) const
{
	return time == timeNow.time && eventCount == timeNow.eventCount;
}

rbool TracerTimeNow::operator!= (const TracerTimeNow& timeNow) const
{
	return !(*this == timeNow);
}

rbool TracerTimeNow::compareTimes(const TracerTimeNow* pTimeNow)
{
	return pTimeNow
		? time >= pTimeNow->time
		: false;
}

// --------------------------------------------------------------------------------
// -------------------- TracerValue
// --------------------------------------------------------------------------------
TracerValue::TracerValue(TracerTimeNow* const pTimeNow, const int eventID, const double value)
	: m_value     (value   )
	, m_pModelTime(pTimeNow)
	, m_eventID   (eventID )
{}

TracerValue::~TracerValue()
{}

double TracerValue::getValue() const
{
	return m_value;
}

TracerTimeNow* const TracerValue::getModelTime() const
{
	return m_pModelTime;
}

int TracerValue::getEventID() const
{
	return m_eventID;
}

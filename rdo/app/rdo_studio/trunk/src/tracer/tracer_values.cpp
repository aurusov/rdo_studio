/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_values.cpp
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/tracer_values.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdo::gui::tracer;

// --------------------------------------------------------------------------------
// -------------------- Time
// --------------------------------------------------------------------------------
Time::Time(const double time)
	: time(time)
{
	eventCount = 0;
}

Time::Time(const double time, const int eventCount)
	: time(time)
	, eventCount(eventCount)
{}

Time& Time::operator= (const Time& timeNow)
{
	time       = timeNow.time;
	eventCount = timeNow.eventCount;

	return *this;
}

rbool Time::operator== (const Time& timeNow) const
{
	return time == timeNow.time && eventCount == timeNow.eventCount;
}

rbool Time::operator!= (const Time& timeNow) const
{
	return !(*this == timeNow);
}

rbool Time::compareTimes(const Time* pTimeNow)
{
	return pTimeNow
		? time >= pTimeNow->time
		: false;
}

// --------------------------------------------------------------------------------
// -------------------- Value
// --------------------------------------------------------------------------------
Value::Value(Time* const pTimeNow, const int eventID, const double value)
	: m_value     (value   )
	, m_pModelTime(pTimeNow)
	, m_eventID   (eventID )
{}

Value::~Value()
{}

double Value::getValue() const
{
	return m_value;
}

Time* const Value::getModelTime() const
{
	return m_pModelTime;
}

int Value::getEventID() const
{
	return m_eventID;
}

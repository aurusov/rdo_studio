/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_values.h
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_VALUES_H_
#define _RDO_STUDIO_TRACER_VALUES_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

// --------------------------------------------------------------------------------
// -------------------- Time
// --------------------------------------------------------------------------------
class Time
{
public:
	Time(const double time = 0);
	Time(const double time, const int eventCount);

	double time;
	int eventCount;

	Time& operator =(const Time& timeNow);
	rbool operator ==(const Time& timeNow) const;
	rbool operator !=(const Time& timeNow) const;

	rbool compareTimes(const Time* pTimeNow);
};

// --------------------------------------------------------------------------------
// -------------------- Value
// --------------------------------------------------------------------------------
class Value
{
public:
	Value(Time* const pTimeNow, const int eventID, const double value);
	~Value();

	double      getValue    () const;
	Time* const getModelTime() const;
	int         getEventID  () const;

private:
	double  m_value;
	Time*   m_pModelTime;
	int     m_eventID;
};

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_VALUES_H_

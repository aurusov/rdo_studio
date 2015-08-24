#pragma once

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
    bool  operator ==(const Time& timeNow) const;
    bool  operator !=(const Time& timeNow) const;

    bool  compareTimes(const Time* pTimeNow);
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

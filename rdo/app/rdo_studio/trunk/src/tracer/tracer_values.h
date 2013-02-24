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
// -------------------- TracerTimeNow
// --------------------------------------------------------------------------------
class TracerTimeNow
{
public:
	TracerTimeNow(const double time = 0);
	TracerTimeNow(const double time, const int eventCount);

	double time;
	int eventCount;

	TracerTimeNow& operator =(const TracerTimeNow& timeNow);
	rbool operator ==(const TracerTimeNow& timeNow) const;
	rbool operator !=(const TracerTimeNow& timeNow) const;

	rbool compareTimes(const TracerTimeNow* pTimeNow);
};

// --------------------------------------------------------------------------------
// -------------------- TracerValue
// --------------------------------------------------------------------------------
class TracerValue
{
public:
	TracerValue(TracerTimeNow* const pTimeNow, const int eventID, const double value);
	~TracerValue();

	double               getValue    () const;
	TracerTimeNow* const getModelTime() const;
	int                  getEventID  () const;

private:
	double          m_value;
	TracerTimeNow*  m_pModelTime;
	int             m_eventID;
};

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_VALUES_H_

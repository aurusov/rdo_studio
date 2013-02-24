/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracervalues.h
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_RDOTRACERVALUES_H_
#define _RDO_STUDIO_TRACER_RDOTRACERVALUES_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

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
friend class TracerSerie;
friend class RDOStudioChartDoc;
friend class RDOStudioChartDocInsertTime;
friend class RDOStudioChartDocInsertTime;
friend class TracerSerieFindValue;

public:
	double value;
	TracerValue(TracerTimeNow* const timenow, const int _eventIndex, const double _value = 0);
	~TracerValue();

	TracerTimeNow* const getModelTime() const
	{
		return modeltime;
	}

protected:
	TracerTimeNow* modeltime;
	int eventIndex;
};

#endif // _RDO_STUDIO_TRACER_RDOTRACERVALUES_H_

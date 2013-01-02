/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracervalues.h
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_TRACER_RDOTRACERVALUES_H_
#define _RDO_STUDIO_MFC_TRACER_RDOTRACERVALUES_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- TracerTimeNow
// --------------------------------------------------------------------------------
class TracerTimeNow
{
public:
	TracerTimeNow( const double _time = 0 );
	TracerTimeNow( const double _time, const int _event_count );
	~TracerTimeNow() {};

	double time;
	int eventCount;
	
	TracerTimeNow& operator =( const TracerTimeNow& timenow );
	rbool operator ==( const TracerTimeNow& timenow ) const;
	rbool operator !=( const TracerTimeNow& timenow ) const;

	rbool compareTimes( const TracerTimeNow* timenow );
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

protected:
	TracerTimeNow* modeltime;
	int eventIndex;
public:
	double value;
	TracerValue( TracerTimeNow* const timenow, const int _eventIndex, const double _value = 0 );
	~TracerValue();
	TracerTimeNow* const getModelTime() const { return modeltime; };
};

#endif // _RDO_STUDIO_MFC_TRACER_RDOTRACERVALUES_H_

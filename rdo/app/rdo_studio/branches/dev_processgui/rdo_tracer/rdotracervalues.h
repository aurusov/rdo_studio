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
// -------------------- RDOTracerTimeNow
// --------------------------------------------------------------------------------
class RDOTracerTimeNow
{
public:
	RDOTracerTimeNow( const double _time = 0 );
	RDOTracerTimeNow( const double _time, const int _event_count );
	~RDOTracerTimeNow() {};

	double time;
	int eventCount;
	
	RDOTracerTimeNow& operator =( const RDOTracerTimeNow& timenow );
	rbool operator ==( const RDOTracerTimeNow& timenow ) const;
	rbool operator !=( const RDOTracerTimeNow& timenow ) const;

	rbool compareTimes( const RDOTracerTimeNow* timenow );
};

// --------------------------------------------------------------------------------
// -------------------- RDOTracerValue
// --------------------------------------------------------------------------------
class RDOTracerValue
{
friend class RDOTracerSerie;
friend class RDOStudioChartDoc;
friend class RDOStudioChartDocInsertTime;
friend class RDOStudioChartDocInsertTime;
friend class RDOTracerSerieFindValue;

protected:
	RDOTracerTimeNow* modeltime;
	int eventIndex;
public:
	double value;
	RDOTracerValue( RDOTracerTimeNow* const timenow, const int _eventIndex, const double _value = 0 );
	~RDOTracerValue();
	RDOTracerTimeNow* const getModelTime() const { return modeltime; };
};

#endif // _RDO_STUDIO_MFC_TRACER_RDOTRACERVALUES_H_

#ifndef RDOTRACERVALUES_H
#define RDOTRACERVALUES_H
#pragma once

// ----------------------------------------------------------------------------
// ---------- RDOTracerTimeNow
// ----------------------------------------------------------------------------
class RDOTracerTimeNow
{
public:
	RDOTracerTimeNow( const double _time = 0 );
	~RDOTracerTimeNow() {};

	double time;
	int eventCount;
	int overallCount;
	
	RDOTracerTimeNow& operator =( const RDOTracerTimeNow& timenow );
	bool operator ==( const RDOTracerTimeNow& timenow ) const;
	bool operator !=( const RDOTracerTimeNow& timenow ) const;

	bool compareTimes( const RDOTracerTimeNow* timenow );
};

// ----------------------------------------------------------------------------
// ---------- RDOTracerValue
// ----------------------------------------------------------------------------
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
	RDOTracerValue( RDOTracerTimeNow* const timenow, const int _eventIndex );
	~RDOTracerValue();
};

#endif // RDOTRACERVALUES_H
#ifndef RDOTRACERVALUES_H
#define RDOTRACERVALUES_H
#pragma once

// ----------------------------------------------------------------------------
// ---------- RDOTracerTimeNow
// ----------------------------------------------------------------------------
class RDOTracerTimeNow
{
public:
	RDOTracerTimeNow() {};
	~RDOTracerTimeNow() {};

	double time;
	int eventCount;
	int overallCount;

	bool operator >=( const RDOTracerTimeNow* timenow ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOTracerValue
// ----------------------------------------------------------------------------
class RDOTracerValue
{
friend class RDOTracerSerie;
friend class RDOStudioChartDoc;
friend class RDOStudioChartDocInsertTime;

protected:
	RDOTracerTimeNow* modeltime;
	int eventIndex;
public:
	double value;
	RDOTracerValue( RDOTracerTimeNow* const timenow, const int _eventIndex );
	~RDOTracerValue();
};

#endif // RDOTRACERVALUES_H
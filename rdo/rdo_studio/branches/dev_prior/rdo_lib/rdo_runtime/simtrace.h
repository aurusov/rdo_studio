#ifndef RDO_SIMTRACE_H
#define RDO_SIMTRACE_H

#include "rdotrace.h"
#include "rdo_simulator.h"

namespace rdoParse {
class RDODPTSome;
}

namespace rdoRuntime {

class RDODPTSearchTrace;
class RDODPTSearchTrace;
class RDOIrregEvent;
class RDORule;
class RDOOperation;

class RDOSimulatorTrace: public RDOSimulator
{
public:
	enum { UNDEFINE_TIME = -1 };

	virtual void rdoInit();

	RDOTrace* getTracer() const            { return m_tracer;                }
	bool      canTrace() const             { return getTracer()->canTrace(); }

	double getTraceStartTime() const       { return traceStartTime;          }
	void setTraceStartTime( double value ) { traceStartTime = value;         }

	double getTraceEndTime() const         { return traceEndTime;            }
	void setTraceEndTime( double value )   { traceEndTime = value;           }

	virtual void onNewTimeNow() {
		if ( timeForTrace() ) {
			getTracer()->startWriting();
		} else {
			getTracer()->stopWriting();
		}
	}

	void memory_insert( unsigned int mem ) {
		memory_current += mem;
		if ( memory_current > memory_max ) memory_max = memory_current;
	}
	void memory_remove( unsigned int mem ) {
		memory_current -= mem;
	}
	unsigned int memory_get() const {
		return memory_max;
	}

	int getFreeResourceId( int id = -1 );
	void incrementResourceIdReference(int id);

	void freeOperationId(int id);
	int getFreeOperationId(); 
	void onResourceErase( RDOResource* res );

	int getFreeIrregEventId()
	{
		return m_ieCounter++;
	}
	int getFreeActivityId()
	{
		return m_activityCounter++;
	}

	int getFreeDPTId()
	{
		return m_dptCounter++;
	}

protected:
	RDOSimulatorTrace():
		RDOSimulator(),
		m_tracer( NULL ),
		traceStartTime( UNDEFINE_TIME ),
		traceEndTime( UNDEFINE_TIME ),
		m_ieCounter( 1 ),
		m_activityCounter( 1 ),
		m_dptCounter( 1 ),
		maxResourcesId( 0 ),
		maxOperationId( 1 ),
		memory_current( 0 ),
		memory_max( 0 )
	{
	}
	virtual ~RDOSimulatorTrace();

	virtual void operator= (const RDOSimulatorTrace& other);

	RDOTrace* m_tracer;

	int maxOperationId;

	virtual void preProcess();
	virtual void postProcess();
	void checkRSSDefinedResources();

	virtual std::list< RDOResource* > getResourcesBeforeSim() const = 0;

private:
	double traceStartTime;
	double traceEndTime;

	int maxResourcesId;

	std::list< int > freeResourcesIds;
	typedef std::map<int, int> MAPII;
	MAPII resourcesIdsRefs;
	std::list< int > freeOperationsIds;

	void eraseFreeResourceId( int id );

	int m_ieCounter;
	int m_activityCounter;
	int m_dptCounter;

	void addTemplateDecisionPoint ( RDODPTSearchTrace *dp   );
	void addTemplateIrregularEvent( RDOIrregEvent     *ev   );
	void addTemplateRule          ( RDORule           *rule );
	void addTemplateOperation     ( RDOOperation      *op   );

	ruint memory_current;
	ruint memory_max;

	bool timeForTrace() const
	{
		if ( getTraceStartTime() != UNDEFINE_TIME && getTraceStartTime() > getCurrentTime() ) return false;
		if ( getTraceEndTime()   != UNDEFINE_TIME && getTraceEndTime()   < getCurrentTime() ) return false;
		return true;
	}
};

} // namespace rdoRuntime

#endif // RDO_SIMTRACE_H

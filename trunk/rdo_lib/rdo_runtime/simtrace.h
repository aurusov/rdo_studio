#ifndef RDO_SIMTRACE_H
#define RDO_SIMTRACE_H

#include "rdo_simulator.h"
#include "rdotrace.h"

namespace rdoParse {
class RDODPTSome;
}

namespace rdoRuntime {

class RDOBaseOperation;

class RDOSimulatorTrace: public RDOSimulator
{
friend class RDODPTSearchTrace;
friend class TreeRootTrace;
friend class TreeNodeTrace;
friend class RDOTrace;
friend class RDOResource;
friend class RDOIrregEvent;
friend class RDORule;
friend class RDOOperation;
friend class RDOActivityTrace;
friend class rdoParse::RDODPTSome;

public:
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

protected:
	RDOSimulatorTrace():
		RDOSimulator(),
		m_tracer( NULL ),
		traceStartTime( -1 ),
		traceEndTime( -1 ),
		dptCounter( 1 ),
		activityCounter( 1 ),
		ieCounter( 1 ),
		maxResourcesId( 0 ),
		maxOperationId( 1 ),
		memory_current( 0 ),
		memory_max( 0 )
	{
	}
	~RDOSimulatorTrace();

	RDOTrace* m_tracer;

	int maxOperationId;
	void addTemplateBaseOperation( RDOBaseOperation* op );

	void incrementResourceIdReference(int id);

	virtual void preProcess();
	virtual void postProcess();
	void checkRSSDefinedResources();

	void onResourceErase( RDOResource* res );
	virtual std::list< RDOResource* > getResourcesBeforeSim() const = 0;

private:
	double traceStartTime;
	double traceEndTime;

	int maxResourcesId;

	std::list< int > freeResourcesIds;
	typedef std::map<int, int> MAPII;
	MAPII resourcesIdsRefs;
	std::list< int > freeOperationsIds;

	int getFreeResourceId( int id = -1 );
	void eraseFreeResourceId( int id );
	int getFreeOperationId(); 
	void freeOperationId(int id);

	int ieCounter;
	int activityCounter;
	int dptCounter;

	void addTemplateDecisionPoint ( RDODPTSearchTrace *dp   );
	void addTemplateIrregularEvent( RDOIrregEvent     *ev   );
	void addTemplateRule          ( RDORule           *rule );
	void addTemplateOperation     ( RDOOperation      *op   );

	unsigned int memory_current;
	unsigned int memory_max;

	bool timeForTrace() const {
		if ( getTraceStartTime() != -1 && getTraceStartTime() > getCurrentTime() ) return false;
		if ( getTraceEndTime() != -1 && getTraceEndTime() < getCurrentTime() ) return false;
		return true;
	}
};

} // namespace rdoRuntime

#endif // RDO_SIMTRACE_H

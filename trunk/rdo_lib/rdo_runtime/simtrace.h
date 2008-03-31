#ifndef SIMTRACE_H
#define SIMTRACE_H

#include "rdotrace.h"

namespace rdoParse {
class RDODPTSome;
}

namespace rdoRuntime {

class RDOActivityRuntime;
class RDOBaseOperation;

class RDOSimulatorTrace: public RDOSimulator
{
friend class RDODecisionPointTrace;
friend class RDOOperationTrace;
friend class TreeRootTrace;
friend class TreeNodeTrace;
friend class RDOTrace;
friend class RDOResourceTrace;
friend class RDOIETrace;
friend class RDORuleTrace;
friend class RDOActivityTrace;
friend class RDOActivityRuntime;
friend class rdoParse::RDODPTSome;

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

	void addTemplateDecisionPoint(RDODecisionPointTrace *dp);
	void addTemplateOperation(RDOOperationTrace *op);
	void addTemplateIrregularEvent(RDOIETrace *ev);
	void addTemplateRule(RDORuleTrace *rule);

	unsigned int memory_current;
	unsigned int memory_max;

protected:
	RDOSimulatorTrace():
		RDOSimulator(),
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
	int maxOperationId;
	void addTemplateBaseOperation( RDOBaseOperation* op );

	void incrementResourceIdReference(int id);

	virtual void preProcess();
	virtual void postProcess();
	void checkRSSDefinedResources();

	void onResourceErase( RDOResourceTrace* res );
	virtual std::list< RDOResourceTrace* > getResourcesBeforeSim() const = 0;

public:
	virtual RDOTrace* getTracer() = 0;
	virtual void rdoInit();

	double getTraceStartTime() const       { return traceStartTime;  }
	void setTraceStartTime( double value ) { traceStartTime = value; }

	double getTraceEndTime() const         { return traceEndTime;    }
	void setTraceEndTime( double value )   { traceEndTime = value;   }

	bool canTrace() const {
		if ( getTraceStartTime() != -1 && getTraceStartTime() > getCurrentTime() ) return false;
		if ( getTraceEndTime() != -1 && getTraceEndTime() < getCurrentTime() ) return false;
		return true;
	}
	virtual void onNewTimeNow() {
		RDOTrace* tracer = getTracer();
		if ( canTrace() ) {
			tracer->startWriting();
		} else {
			tracer->stopWriting();
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
};

} // namespace rdoRuntime

#endif // SIMTRACE_H

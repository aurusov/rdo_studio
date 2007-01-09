#ifndef SIMTRACE_H
#define SIMTRACE_H

#include "rdotrace.h"

namespace rdoRuntime {
class RDOActivityRuntime;
}

namespace rdoParse {
class RDODPTSome;
}

class RDOSimulatorTrace: public RDOSimulator
{
friend RDODecisionPointTrace;
friend RDOOperationTrace;
friend TreeRootTrace;
friend TreeNodeTrace;
friend RDOTrace;
friend RDOResourceTrace;
friend RDOIETrace;
friend RDORuleTrace;
friend class RDOActivityTrace;
friend class rdoRuntime::RDOActivityRuntime;
friend class rdoParse::RDODPTSome;

private:
	int maxResourcesId;

	std::list<int> freeResourcesIds;
	typedef std::map<int, int> MAPII;
	MAPII resourcesIdsRefs;
	std::list< int > freeOperationsIds;
	std::list< RDOResourceTrace* > allResourcesInSim;
	std::list< RDOResourceTrace* > perm;

	int getFreeResourceId();
	void freeResourceId(int id);
	int getFreeOperationId(); 
	void freeOperationId(int id);

	RDOSimulator *createCopy();

	int ieCounter;
	int activityCounter;
	int dptCounter;

//	virtual void onAfterCheckPokaz();

	void addTemplateDecisionPoint(RDODecisionPointTrace *dp);
	void addTemplateOperation(RDOOperationTrace *op);
	void addTemplateIrregularEvent(RDOIETrace *ev);
	void addTemplateRule(RDORuleTrace *rule);

	unsigned int memory_current;
	unsigned int memory_max;

protected:
	RDOSimulatorTrace():
		RDOSimulator(),
		dptCounter( 1 ),
		activityCounter( 1 ),
		ieCounter( 1 ),
		memory_current( 0 ),
		memory_max( 0 )
	{
	}
	int maxOperationId;
	void addTemplateBaseOperation( RDOBaseOperation* op );

	void incrementResourceIdReference(int id);

	virtual void preProcess();
	virtual void postProcess();
	void checkPermanentResources();
	void checkRSSDefinedResources();

	void onResourceErase(RDOResourceTrace *res);
	virtual std::list< RDOResourceTrace* > getPermanentResources() = 0;

public:
	virtual RDOTrace* getTracer() = 0;
	void rdoDestroy();
	void rdoInit();

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

#endif // SIMTRACE_H

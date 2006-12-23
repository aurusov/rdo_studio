#ifndef SIMTRACE_H
#define SIMTRACE_H

#include "rdotrace.h"

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
friend CheckRelevantResource;
friend RDORuleTrace;
friend class RDOActivityTrace;
friend class rdoParse::RDODPTSome;

private:
	int maxResourcesId;

	std::list<int> freeResourcesIds;
	typedef std::map<int, int> MAPII;
	MAPII resourcesIdsRefs;
	std::list<int> freeOperationsIds;
	std::list<RDOResourceTrace *> allResourcesInSim;
	std::vector<RDOResourceTrace *> perm;

	int getFreeResourceId();
	void freeResourceId(int id);
	int getFreeOperationId(); 
	void freeOperationId(int id);
	void clearJustCreatedFlags();

	RDOSimulator *createCopy();

	int ieCounter;
	int activityCounter;
	int dptCounter;

	void onAfterCheckPokaz();

	void addTemplateDecisionPoint(RDODecisionPointTrace *dp);
	void addTemplateOperation(RDOOperationTrace *op);
	void addTemplateIrregularEvent(RDOIETrace *ev);
	void addTemplateRule(RDORuleTrace *rule);

protected:
	RDOSimulatorTrace(): dptCounter(1), activityCounter(1), ieCounter(1) {}
	int maxOperationId;
	void addTemplateBaseOperation(RDOBaseOperation *op);

	void incrementResourceIdReference(int id);

	virtual void preProcess();
	virtual void postProcess();
	void checkPermanentResources();
	void checkRSSDefinedResources();

	void onResourceErase(RDOResourceTrace *res);
	virtual std::vector<RDOResourceTrace *> getPermanentResources() = 0;

public:
	virtual RDOTrace* getTracer() = 0;
	void rdoDestroy();
	void rdoInit();
};

#endif // SIMTRACE_H

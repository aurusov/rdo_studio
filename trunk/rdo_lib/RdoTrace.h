#ifndef RDO_TRACE
#define RDO_TRACE

#include "Rdo.h"

class TreeNodeTrace;
class RDODecisionPointTrace;
class RDOSimulatorTrace;
class RDORuleTrace;
class RDOIETrace;
class RDOResourceTrace;
class RDOOperationTrace;
class RDOTraceableObject;
class TreeRootTrace;
class RDOPokazTrace;

class RDOEndL
{
public:
	virtual void onEndl() {}
};

inline std::ostream &operator << (std::ostream &stream, RDOEndL& rdoEndL)
{
//	stream << std::endl;
	rdoEndL.onEndl();
	return stream;
}

class RDOTrace
{
friend RDOSimulatorTrace;
friend RDOResourceTrace;
   std::string fileName;
   std::ofstream out;
	bool canWriteToStream;
   virtual std::ostream &getOStream() { return out; }
	RDOEndL rdoEndL;

protected:
   virtual RDOEndL& getEOL() { return rdoEndL; }
	bool isNullTracer;

public:
   RDOTrace(): isNullTracer(true) {}
   RDOTrace(std::string fn): fileName(fn), out(fileName.begin(), std::ios_base::out), isNullTracer(false) {}
	virtual ~RDOTrace() {}

	void startWriting() { canWriteToStream = true; }
	void stopWriting() { canWriteToStream = false; }
	bool canWrite() { return canWriteToStream; }

// Search in tree
   virtual void writeSearchBegin(double currentTime, std::string decisionPointId);
   virtual void writeSearchDecisionHeader();
   virtual void writeSearchDecision(RDOSimulator *sim, TreeNode *node);
   virtual void writeString(std::string);
   virtual void writeSearchOpenNode(int nodeCount, int parentCount,
      double pathCost, double restCost);
   virtual void writeSearchNodeInfo(char sign, TreeNodeTrace *node);
   virtual void writeSearchResult(char letter, RDOSimulatorTrace *simTr, TreeRoot *treeRoot);

   virtual void writeIrregularEvent(RDOIETrace *ie, RDOSimulatorTrace *sim);
   virtual void writeRule(RDORuleTrace *rule, RDOSimulatorTrace *sim);
   virtual void writeBeforeOperationBegin(RDOOperationTrace *op, RDOSimulatorTrace *sim);
   virtual void writeAfterOperationBegin(RDOOperationTrace *op, RDOSimulatorTrace *sim);
   virtual void writeBeforeOperationEnd(RDOOperationTrace *op, RDOSimulatorTrace *sim);
   virtual void writeAfterOperationEnd(RDOOperationTrace *op, RDOSimulatorTrace *sim);

   virtual void writeTraceBegin(RDOSimulatorTrace *sim);
   virtual void writeModelBegin(RDOSimulatorTrace *sim);
   virtual void writePermanentResources(RDOSimulatorTrace *sim, std::vector<RDOResourceTrace *> perm);

   virtual std::string traceResourcesListNumbers(RDOSimulatorTrace *sim, std::vector<RDOResourceTrace *> resArray);
   virtual std::string traceResourcesList(char prefix, RDOSimulatorTrace *sim, std::vector<RDOResourceTrace *> resArray);

   virtual void writePokaz(RDOSimulatorTrace *sim, RDOPokazTrace *pok);
};

class RDOTraceableObject
{
friend RDOSimulatorTrace;
friend RDOTrace;
protected:
   int id;
   RDOSimulatorTrace *sim;
   virtual std::string traceId() const { return toString(id); }
   RDOTraceableObject(RDOSimulatorTrace *i_sim): sim(i_sim) {trace = false;}
public:
   bool trace;
};

class CheckRelevantResource
{
   RDOSimulatorTrace *sim;
public:
   CheckRelevantResource(RDOSimulatorTrace *i_sim): sim(i_sim) {}
   void operator ()(RDOResourceTrace *res);
};

class RDOPattern
{
friend RDOSimulatorTrace;

   std::list<RDOResourceTrace *> temp;

protected:
   std::vector<RDOResourceTrace *> relevantResources;
   virtual std::vector<RDOResourceTrace *> getRelevantResources(RDOSimulator *sim) = 0;
   RDOSimulatorTrace *sim;
   RDOPattern(RDOSimulatorTrace *i_sim): sim(i_sim) {}
   void onAfter(RDOSimulator *sim);
};


// Base class for all resources
class RDOResourceTrace: public RDOTraceableObject
{
friend RDOTrace;
friend RDOSimulatorTrace;
friend CheckRelevantResource;
   
   virtual std::string traceResourceState(char prefix, RDOSimulatorTrace *sim);
   bool justCreated;
   std::string typeId;
   std::string traceTypeId() { return typeId.empty()?(typeId=getTypeId()):typeId; }
   bool tempotary;
protected:
   virtual std::string getTypeId() = 0;
   virtual std::string traceParametersValue() = 0;
   RDOResourceTrace(RDOSimulatorTrace *sim);
   RDOResourceTrace(const RDOResourceTrace &orig);
   virtual ~RDOResourceTrace();
public:
	void makeTemporary(bool temp) { tempotary = temp; }
   static void clearJCFlag(RDOResourceTrace *res) { res->justCreated = false; }
};

class RDOPokazTrace: public RDOPokaz, public RDOTraceableObject
{
protected:
	bool wasChanged;
public:
   RDOPokazTrace(RDOSimulatorTrace *i_sim): RDOTraceableObject(i_sim), wasChanged(true) {}
	bool tracePokaz();
   virtual std::string traceValue() = 0;
};



#include "SimTrace.h"
#include "SearchTrace.h"
#include "RuleTrace.h"
#include "OperationTrace.h"
#include "IETrace.h"

#endif //RDO_TRACE

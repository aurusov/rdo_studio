class RDORuleTrace: public RDORule, RDOTraceableObject, RDOPattern
{
friend RDOTrace;
   virtual std::vector<RDOResourceTrace *> getRelevantResources(RDOSimulator *sim) = 0;
   virtual std::string tracePatternId() = 0;

protected:
   virtual void onBeforeRule(RDOSimulator *sim);
   virtual void onAfterRule(RDOSimulator *sim, bool inSearch = false);
   RDORuleTrace(RDOSimulatorTrace *sim);

public:
   bool trace;  
};


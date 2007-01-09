#ifndef RULE_TRACE_H
#define RULE_TRACE_H

class RDORuleTrace: public RDORule, RDOTraceableObject, protected RDOPatternTrace
{
friend RDOTrace;

private:
   virtual const std::string& tracePatternId() const = 0;

protected:
	RDORuleTrace( RDOSimulatorTrace* sim );
	virtual void onBeforeRule( RDOSimulator* sim );
	virtual void onAfterRule( RDOSimulator* sim, bool inSearch = false );

public:
	bool trace;  
};

#endif // RULE_TRACE_H

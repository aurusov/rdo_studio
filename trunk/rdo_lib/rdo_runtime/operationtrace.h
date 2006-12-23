class RDOOperationTrace: public RDOOperation, public RDOTraceableObject, RDOPattern
{
friend RDOTrace;
friend RDOSimulatorTrace;
private:
	virtual std::string traceOperId() { return toString(operId); }
	virtual std::string tracePatternId() = 0;
	int operId;

	RDOOperation *clone(RDOSimulator *sim) 
	{
		RDOOperationTrace *newOper = clone2(sim);
		newOper->id = id;
		newOper->operId = ((RDOSimulatorTrace *)sim)->getFreeOperationId();
		return newOper;
	};

protected:
	virtual void onBeforeOperationBegin(RDOSimulator *sim);
	virtual void onAfterOperationBegin(RDOSimulator *sim);
	virtual void onBeforeOperationEnd(RDOSimulator *sim);
	virtual void onAfterOperationEnd(RDOSimulator *sim);

	RDOOperationTrace(RDOSimulatorTrace *sim);
	virtual ~RDOOperationTrace();
	virtual RDOOperationTrace *clone2(RDOSimulator *sim) = 0; 
};


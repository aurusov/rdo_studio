#ifndef OPERATION_TRACE_H
#define OPERATION_TRACE_H

class RDOOperationTrace: public RDOOperation, public RDOTraceableObject, protected RDOPatternTrace
{
friend RDOTrace;
friend RDOSimulatorTrace;

private:
	virtual std::string traceOperId() { return toString(operId); }
	virtual const std::string& tracePatternId() const = 0;
	int operId;

	RDOOperation* clone( RDOSimulator* sim ) {
		RDOOperationTrace* newOper = clone2( sim );
		newOper->id     = id;
		newOper->operId = static_cast<RDOSimulatorTrace*>(sim)->getFreeOperationId();
		return newOper;
	};

protected:
	RDOOperationTrace( RDOSimulatorTrace* sim );
	virtual ~RDOOperationTrace();

	virtual void onAfterOperationBegin( RDOSimulator* sim );
	virtual void onAfterOperationEnd( RDOSimulator* sim );

	virtual RDOOperationTrace* clone2( RDOSimulator* sim ) = 0; 
};

#endif // OPERATION_TRACE_H

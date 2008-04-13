#ifndef OPERATION_TRACE_H
#define OPERATION_TRACE_H

#include "rdo.h"
#include "rdotrace.h"
#include "simtrace.h"

namespace rdoRuntime
{

class RDOOperationTrace: public RDOOperation, public RDOPatternTrace
{
friend RDOTrace;
friend RDOSimulatorTrace;

private:
	virtual std::string traceOperId() { return toString(m_operId); }
	virtual const std::string& tracePatternId() const = 0;
	int m_operId;

	RDOOperation* clone( RDOSimulator* sim ) {
		RDOOperationTrace* newOper = clone2( sim );
		newOper->setTraceID( getTraceID() );
		newOper->m_operId = static_cast<RDOSimulatorTrace*>(sim)->getFreeOperationId();
		return newOper;
	};

protected:
	RDOOperationTrace( RDORuntime* runtime, bool trace );
	virtual ~RDOOperationTrace();

	virtual void onAfterOperationBegin( RDOSimulator* sim );
	virtual void onAfterOperationEnd( RDOSimulator* sim );

	virtual RDOOperationTrace* clone2( RDOSimulator* sim ) = 0; 
};

} // namespace rdoRuntime

#endif // OPERATION_TRACE_H

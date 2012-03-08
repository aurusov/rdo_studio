#ifndef RDOTRACER_H
#define RDOTRACER_H

#include "app/rdo_studio_mfc/rdo_tracer/rdotracerbase.h"
#include "simulator/service/rdosimwin.h"
#include "kernel/rdothread.h"

// --------------------------------------------------------------------------------
// -------------------- RDOTracer
// --------------------------------------------------------------------------------
namespace rdoTracer {

class RDOTracer: public RDOTracerBase
{
protected:
	void proc( RDOThread::RDOMessageInfo& msg );

public:
	RDOTracer();
	virtual ~RDOTracer();

	void setRuntimeMode( const rdoRuntime::RunTimeMode value );
};

}; // namespace rdoTracer

// --------------------------------------------------------------------------------
extern rdoTracer::RDOTracer* tracer;

#endif // RDOTRACER_H

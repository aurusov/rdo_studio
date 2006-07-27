#ifndef RDOTRACER_H
#define RDOTRACER_H
#pragma once

#include "rdotracerbase.h"
#include <rdosimwin.h>
#include <rdothread.h>
// ----------------------------------------------------------------------------
// ---------- RDOTracer
// ----------------------------------------------------------------------------
namespace rdoTracer {

class RDOTracer: public RDOTracerBase
{
protected:
	void proc( RDOThread::RDOMessageInfo& msg );

public:
	RDOTracer();
	virtual ~RDOTracer();

	void setShowMode( const rdoSimulator::ShowMode value );
};

}; // namespace rdoTracer

// ----------------------------------------------------------------------------
extern rdoTracer::RDOTracer* tracer;

#endif // RDOTRACER_H

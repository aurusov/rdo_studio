#ifndef RDOTRACER_H
#define RDOTRACER_H
#pragma once

#include "rdotracerbase.h"

// ----------------------------------------------------------------------------
// ---------- RDOTracer
// ----------------------------------------------------------------------------
namespace rdoTracer {

class RDOTracer : public RDOTracerBase
{
private:

	static void newModelNotify();
	static void openModelNotify();
	static void closeModelNotify();
	static void beforeModelStartNotify();
	static void traceStringNotify( std::string trace_string );
public:
	RDOTracer();
	virtual ~RDOTracer();
};

}; // namespace rdoTracer

// ----------------------------------------------------------------------------
extern rdoTracer::RDOTracer* tracer;

#endif // RDOTRACER_H

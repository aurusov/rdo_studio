#ifndef RDOTRACER_H
#define RDOTRACER_H
#pragma once

#include "rdotracerbase.h"
#include <rdosimwin.h>
// ----------------------------------------------------------------------------
// ---------- RDOTracer
// ----------------------------------------------------------------------------
namespace rdoTracer {

class RDOTracer : public RDOTracerBase
{
private:
	static void closeModelNotify();
	static void beforeModelStartNotify();
	static void modelStoppedNotify();
	static void traceStringNotify( const std::string& trace_string );
public:
	RDOTracer();
	virtual ~RDOTracer();

	void initNotify();

	void setShowMode( const rdosim::ShowMode value );
};

}; // namespace rdoTracer

// ----------------------------------------------------------------------------
extern rdoTracer::RDOTracer* tracer;

#endif // RDOTRACER_H

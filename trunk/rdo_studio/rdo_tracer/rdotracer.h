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

class RDOTracer : public RDOTracerBase
{
friend class RDOThreadStudioGUI;
protected:
	void procGUI( RDOThread::RDOMessageInfo& msg );

public:
	RDOTracer();
	virtual ~RDOTracer();

	void setShowMode( const rdosim::ShowMode value );
};

}; // namespace rdoTracer

// ----------------------------------------------------------------------------
extern rdoTracer::RDOTracer* tracer;

#endif // RDOTRACER_H

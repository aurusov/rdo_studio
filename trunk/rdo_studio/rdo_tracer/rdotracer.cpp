#include "stdafx.h"
#include "rdotracer.h"
#include <rdokernel.h>
#include <rdorepository.h>
#include <rdosimwin.h>

#include "../rdostudiomainfrm.h"
#include "../rdostudioapp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;
using namespace rdoRepository;
using namespace RDOSimulatorNS;
using namespace rdoTracer;

RDOTracer* tracer = NULL;

// ----------------------------------------------------------------------------
// ---------- RDOTracer
// ----------------------------------------------------------------------------

RDOTracer::RDOTracer() : RDOTracerBase()
{
	tracer = this;

	kernel.setNotifyReflect( RDOKernel::newModel, newModelNotify );
	kernel.setNotifyReflect( RDOKernel::openModel, openModelNotify );
	kernel.setNotifyReflect( RDOKernel::closeModel, closeModelNotify );

	kernel.setNotifyReflect( RDOKernel::beforeModelStart, beforeModelStartNotify );

	kernel.setNotifyReflect( RDOKernel::traceString, traceStringNotify );
}

RDOTracer::~RDOTracer()
{
}

void RDOTracer::newModelNotify()
{
	tracer->clear();
}

void RDOTracer::openModelNotify()
{
	tracer->clear();
}

void RDOTracer::closeModelNotify()
{
	tracer->clear();
}

void RDOTracer::beforeModelStartNotify()
{
	tracer->clear();
	tracer->setModelName( kernel.getRepository()->getName() );
	tracer->getModelStructure( kernel.getSimulator()->getModelStructure() );
}

void RDOTracer::traceStringNotify( string trace_string )
{
	tracer->getTraceString( trace_string );
}

void RDOTracer::setShowMode( const RDOSimulatorNS::ShowMode value )
{
	if ( value == SM_NoShow ) {
		clearCharts();
	}
}
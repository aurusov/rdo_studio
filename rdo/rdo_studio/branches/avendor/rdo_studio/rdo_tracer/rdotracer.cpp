#include "stdafx.h"
#include "rdotracer.h"
#include "../rdostudiomodel.h"
#include <rdokernel.h>
#include <rdorepository.h>
#include <rdosimwin.h>

#include "../rdostudiomainfrm.h"
#include "../rdostudioapp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using namespace std;
using namespace rdoRepository;
using namespace RDOSimulatorNS;
using namespace rdoTracer;

RDOTracer* tracer = NULL;

static bool clear_after_stop = false;

// ----------------------------------------------------------------------------
// ---------- RDOTracer
// ----------------------------------------------------------------------------

RDOTracer::RDOTracer() :
	RDOTracerBase()
{
	clear_after_stop = false;
	
	tracer = this;

	kernel.setNotifyReflect( RDOKernel::closeModel, closeModelNotify );

	kernel.setNotifyReflect( RDOKernel::beforeModelStart, beforeModelStartNotify );
	kernel.setNotifyReflect( RDOKernel::endExecuteModel, modelStoppedNotify );
	kernel.setNotifyReflect( RDOKernel::modelStopped, modelStoppedNotify );
	kernel.setNotifyReflect( RDOKernel::executeError, modelStoppedNotify );

	kernel.setNotifyReflect( RDOKernel::traceString, traceStringNotify );
}

RDOTracer::~RDOTracer()
{
	tracer = NULL;
}

void RDOTracer::closeModelNotify()
{
	clear_after_stop = model->isRunning();
	if ( !clear_after_stop )
		tracer->clear();
}

void RDOTracer::beforeModelStartNotify()
{
	tracer->clear();
	tracer->setShowMode( kernel.getSimulator()->getShowMode() );
	tracer->setModelName( kernel.getRepository()->getName() );
	try {
		kernel.debug( format( IDS_TRACER_GETTING_MODEL_STRUCTURE ).c_str() );
		tracer->getModelStructure( kernel.getSimulator()->getModelStructure() );
		kernel.debug( format( IDS_MODEL_RESOURCE_LOADING_NAME_OK ).c_str() );
	} catch ( ... ) {
		kernel.debug( format( IDS_MODEL_RESOURCE_LOADING_NAME_FAILED ).c_str() );
	}
}

void RDOTracer::modelStoppedNotify()
{
	if ( clear_after_stop ) {
		tracer->clear();
		clear_after_stop = false;
	}
	tracer->setDrawTrace( true );
}

void RDOTracer::traceStringNotify( string trace_string )
{
	tracer->getTraceString( trace_string );
}

void RDOTracer::setShowMode( const RDOSimulatorNS::ShowMode value )
{
	if ( value == SM_NoShow ) {
		tracer->setDrawTrace( false );
	} else {
		tracer->setDrawTrace( true );
	}
}
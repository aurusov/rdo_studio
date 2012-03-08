#include "app/rdo_studio_mfc/pch/stdpch.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracer.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/thread.h"
#include "kernel/rdokernel.h"
#include "repository/rdorepository.h"
#include "simulator/service/rdosimwin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using namespace rdoRepository;
using namespace rdoSimulator;
using namespace rdoTracer;

RDOTracer* tracer = NULL;

static rbool clear_after_stop = false;

// --------------------------------------------------------------------------------
// -------------------- RDOTracer
// --------------------------------------------------------------------------------
RDOTracer::RDOTracer(): RDOTracerBase( "RDOStudioTracerGUI", static_cast<RDOKernelGUI*>(studioApp.m_pStudioGUI) )
{
	clear_after_stop = false;

	tracer = this;

	notifies.push_back( RT_REPOSITORY_MODEL_CLOSE );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_OK );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_BY_USER );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR );
	notifies.push_back( RT_RUNTIME_MODEL_START_BEFORE );
	notifies.push_back( RT_RUNTIME_MODEL_START_AFTER );
	notifies.push_back( RT_RUNTIME_TRACE_STRING );

	after_constructor();
}

RDOTracer::~RDOTracer()
{
	tracer = NULL;
}

void RDOTracer::proc( RDOThread::RDOMessageInfo& msg )
{
	switch ( msg.message ) {
		case RDOThread::RT_REPOSITORY_MODEL_CLOSE: {
			clear_after_stop = model->isRunning();
			if ( !clear_after_stop ) {
				clear();
			}
			break;
		}
		case RDOThread::RT_RUNTIME_MODEL_START_BEFORE: {
			clear();
			try {
				setModelName( model->getName() );
				studioApp.m_pMainFrame->output.appendStringToDebug( rdo::format( IDS_TRACER_GETTING_MODEL_STRUCTURE ) );
				rdo::textstream model_structure;
				sendMessage( kernel->simulator(), RT_SIMULATOR_GET_MODEL_STRUCTURE, &model_structure );
				getModelStructure( model_structure );
				studioApp.m_pMainFrame->output.appendStringToDebug( rdo::format( IDS_MODEL_RESOURCE_LOADING_NAME_OK ) );
			} catch ( ... ) {
				studioApp.m_pMainFrame->output.appendStringToDebug( rdo::format( IDS_MODEL_RESOURCE_LOADING_NAME_FAILED ) );
			}
			break;
		}
		case RDOThread::RT_RUNTIME_MODEL_START_AFTER: {
			rdoRuntime::RunTimeMode runtimeMode;
			sendMessage( kernel->runtime(), RT_RUNTIME_GET_MODE, &runtimeMode );
			setRuntimeMode( runtimeMode );
			break;
		}
		case RDOThread::RT_SIMULATOR_MODEL_STOP_OK           :
		case RDOThread::RT_SIMULATOR_MODEL_STOP_BY_USER      :
		case RDOThread::RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR: {
			if ( clear_after_stop ) {
				clear();
				clear_after_stop = false;
			}
			setDrawTrace( true );
			break;
		}
		case RDOThread::RT_RUNTIME_TRACE_STRING: {
			msg.lock();
			plugins->traceProc( *static_cast<tstring*>(msg.param) );
			getTraceString( *static_cast<tstring*>(msg.param) );
			msg.unlock();
			break;
		}
	}
}

void RDOTracer::setRuntimeMode( const rdoRuntime::RunTimeMode value )
{
	if ( value == rdoRuntime::RTM_MaxSpeed ) {
		setDrawTrace( false );
	} else {
		setDrawTrace( true );
	}
}

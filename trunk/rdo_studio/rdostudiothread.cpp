#include "stdafx.h"
#include "rdostudiothread.h"
#include "rdostudiomodel.h"
#include "rdo_tracer/rdotracer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef RDO_MT
// --------------------------------------------------------------------
// ---------- RDOThreadStudio
// --------------------------------------------------------------------
RDOThreadStudio::RDOThreadStudio(): RDOThread( "RDOThreadStudio" )
{
	after_constructor();
}

void RDOThreadStudio::proc( RDOMessageInfo& msg )
{
	switch ( msg.message ) {
		case RT_STUDIO_MODEL_NEW    :
		case RT_STUDIO_MODEL_OPEN   :
		case RT_STUDIO_MODEL_SAVE   :
		case RT_STUDIO_MODEL_SAVE_AS:
		case RT_STUDIO_MODEL_CLOSE  :
		case RT_STUDIO_MODEL_BUILD  :
		case RT_STUDIO_MODEL_RUN    :
		case RT_STUDIO_MODEL_STOP   :
		{
			broadcastMessage( msg.message, msg.param );
			break;
		}
	}
}
#endif

// --------------------------------------------------------------------
// ---------- RDOThreadStudioGUI
// --------------------------------------------------------------------
#ifdef RDO_MT
RDOThreadStudioGUI::RDOThreadStudioGUI(): RDOThread( "RDOThreadStudioGUI", NULL )
#else
RDOThreadStudioGUI::RDOThreadStudioGUI(): RDOThread( "RDOThreadStudioGUI" )
#endif
{
	// Для модели
	notifies.push_back( RT_REPOSITORY_MODEL_NEW );
	notifies.push_back( RT_REPOSITORY_MODEL_OPEN );
	notifies.push_back( RT_REPOSITORY_MODEL_CLOSE );
	notifies.push_back( RT_REPOSITORY_MODEL_CLOSE_CAN_CLOSE );
	notifies.push_back( RT_REPOSITORY_MODEL_CLOSE_ERROR );
	notifies.push_back( RT_REPOSITORY_MODEL_SAVE );
	notifies.push_back( RT_SIMULATOR_PARSE_STRING );
	notifies.push_back( RT_SIMULATOR_PARSE_OK );
	notifies.push_back( RT_SIMULATOR_PARSE_ERROR );
	notifies.push_back( RT_SIMULATOR_MODEL_START_BEFORE );
	notifies.push_back( RT_SIMULATOR_MODEL_START_AFTER );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_OK );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_BY_USER );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR );
	notifies.push_back( RT_SIMULATOR_FRAME_SHOW );
	notifies.push_back( RT_DEBUG_STRING );

	// Для трассировщика
	notifies.push_back( RT_REPOSITORY_MODEL_CLOSE );
	notifies.push_back( RT_SIMULATOR_MODEL_START_BEFORE );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_OK );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_BY_USER );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR );
	notifies.push_back( RT_SIMULATOR_TRACE_STRING );

	after_constructor();
	sendMessage( kernel, RT_THREAD_CONNECTION, this );
}

void RDOThreadStudioGUI::proc( RDOMessageInfo& msg )
{
	model->procGUI( msg );
	tracer->procGUI( msg );
}

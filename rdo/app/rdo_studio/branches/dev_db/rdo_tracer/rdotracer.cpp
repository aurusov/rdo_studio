/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracer.cpp
  \author    Захаров Павел
  \date      01.04.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdokernel.h"
#include "repository/rdorepository.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_studio/rdo_tracer/rdotracer.h"
#include "app/rdo_studio/src/model/model.h"
#include "app/rdo_studio/src/main_windows_base.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/thread.h"
#include "app/rdo_studio/src/chart/document.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using namespace rdo::repository;
using namespace rdo::service::simulation;
using namespace rdo::gui::tracer;

Tracer* g_pTracer = NULL;

static rbool clear_after_stop = false;

// --------------------------------------------------------------------------------
// -------------------- Tracer
// --------------------------------------------------------------------------------
Tracer::Tracer(): TracerBase( "RDOStudioTracerGUI", static_cast<RDOKernelGUI*>(studioApp.m_pStudioGUI) )
{
	clear_after_stop = false;

	g_pTracer = this;

	notifies.push_back( RT_REPOSITORY_MODEL_CLOSE );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_OK );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_BY_USER );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR );
	notifies.push_back( RT_RUNTIME_MODEL_START_BEFORE );
	notifies.push_back( RT_RUNTIME_MODEL_START_AFTER );
	notifies.push_back( RT_RUNTIME_TRACE_STRING );

	after_constructor();
}

Tracer::~Tracer()
{
	g_pTracer = NULL;
}

void Tracer::proc( RDOThread::RDOMessageInfo& msg )
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
			RDOStudioChartDoc::resetTitleIndex();
			try {
				setModelName( model->getName() );
				studioApp.getIMainWnd()->getDockDebug().appendString( rdo::format( IDS_TRACER_GETTING_MODEL_STRUCTURE ) );
				rdo::textstream model_structure;
				sendMessage( kernel->simulator(), RT_SIMULATOR_GET_MODEL_STRUCTURE, &model_structure );
				getModelStructure( model_structure );
				studioApp.getIMainWnd()->getDockDebug().appendString("ok\n");
			} catch ( ... ) {
				studioApp.getIMainWnd()->getDockDebug().appendString("failed\n");
			}
			break;
		}
		case RDOThread::RT_RUNTIME_MODEL_START_AFTER: {
			rdo::runtime::RunTimeMode runtimeMode;
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
			getTraceString( *static_cast<tstring*>(msg.param) );
			msg.unlock();
			break;
		}
	}
}

void Tracer::setRuntimeMode( const rdo::runtime::RunTimeMode value )
{
	if ( value == rdo::runtime::RTM_MaxSpeed ) {
		setDrawTrace( false );
	} else {
		setDrawTrace( true );
	}
}

/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer.cpp
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
#include "app/rdo_studio/src/model/model.h"
#include "app/rdo_studio/src/main_window_base.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/thread.h"
#include "app/rdo_studio/src/tracer/tracer.h"
#include "app/rdo_studio/src/tracer/chart/chart_doc.h"
// --------------------------------------------------------------------------------

using namespace rdo::repository;
using namespace rdo::service::simulation;
using namespace rdo::gui::tracer;

Tracer* g_pTracer = NULL;

static bool s_clearAfterStop = false;

// --------------------------------------------------------------------------------
// -------------------- Tracer
// --------------------------------------------------------------------------------
Tracer::Tracer()
	: TracerBase("RDOStudioTracerGUI", static_cast<RDOKernelGUI*>(g_pApp->m_pStudioGUI))
{
	s_clearAfterStop = false;

	g_pTracer = this;

	notifies.push_back(RT_REPOSITORY_MODEL_CLOSE);
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_OK);
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_BY_USER);
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR);
	notifies.push_back(RT_RUNTIME_MODEL_START_BEFORE);
	notifies.push_back(RT_RUNTIME_MODEL_START_AFTER);
	notifies.push_back(RT_RUNTIME_TRACE_STRING);

	after_constructor();
}

Tracer::~Tracer()
{
	g_pTracer = NULL;
}

void Tracer::proc(RDOThread::RDOMessageInfo& msg)
{
	switch (msg.message)
	{
	case RDOThread::RT_REPOSITORY_MODEL_CLOSE:
		s_clearAfterStop = g_pModel->isRunning();
		if (!s_clearAfterStop)
		{
			clear();
		}
		break;

	case RDOThread::RT_RUNTIME_MODEL_START_BEFORE:
		clear();
		ChartDoc::resetTitleIndex();
		try
		{
			setModelName(g_pModel->getName());
			g_pApp->getIMainWnd()->getDockDebug().appendString("Получение структуры модели... ");
			rdo::textstream model_structure;
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_STRUCTURE, &model_structure);
			getModelStructure(model_structure);
			g_pApp->getIMainWnd()->getDockDebug().appendString("ok\n");
		}
		catch (...)
		{
			g_pApp->getIMainWnd()->getDockDebug().appendString("failed\n");
		}
		break;

	case RDOThread::RT_RUNTIME_MODEL_START_AFTER:
	{
		rdo::runtime::RunTimeMode runtimeMode;
		sendMessage(kernel->runtime(), RT_RUNTIME_GET_MODE, &runtimeMode);
		setRuntimeMode(runtimeMode);
		break;
	}

	case RDOThread::RT_SIMULATOR_MODEL_STOP_OK:
	case RDOThread::RT_SIMULATOR_MODEL_STOP_BY_USER:
	case RDOThread::RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR:
		if (s_clearAfterStop)
		{
			clear();
			s_clearAfterStop = false;
		}
		setDrawTrace(true);
		break;

	case RDOThread::RT_RUNTIME_TRACE_STRING:
		msg.lock();
		getTraceString(*static_cast<tstring*>(msg.param));
		msg.unlock();
		break;

	default:
		break;
	}
}

void Tracer::setRuntimeMode(const rdo::runtime::RunTimeMode value)
{
	if (value == rdo::runtime::RTM_MaxSpeed)
	{
		setDrawTrace(false);
	}
	else
	{
		setDrawTrace(true);
	}
}

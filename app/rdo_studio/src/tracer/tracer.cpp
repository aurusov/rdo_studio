// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/tracer_pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdokernel.h"
#include "repository/rdorepository.h"
#include "simulator/service/src/simulator.h"
#include "app/rdo_studio/src/model/model.h"
#include "app/rdo_studio/src/main_window_base.h"
#include "app/rdo_studio/src/application.h"
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

    notifies.push_back(Message::REPOSITORY_MODEL_CLOSE);
    notifies.push_back(Message::SIMULATOR_MODEL_STOP_OK);
    notifies.push_back(Message::SIMULATOR_MODEL_STOP_BY_USER);
    notifies.push_back(Message::SIMULATOR_MODEL_STOP_RUNTIME_ERROR);
    notifies.push_back(Message::RUNTIME_MODEL_START_BEFORE);
    notifies.push_back(Message::RUNTIME_MODEL_START_AFTER);
    notifies.push_back(Message::RUNTIME_TRACE_STRING);

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
    case RDOThread::Message::REPOSITORY_MODEL_CLOSE:
        s_clearAfterStop = g_pModel->isRunning();
        if (!s_clearAfterStop)
        {
            clear();
        }
        break;

    case RDOThread::Message::RUNTIME_MODEL_START_BEFORE:
        clear();
        ChartDoc::resetTitleIndex();
        try
        {
            setModelName(g_pModel->getName());
            g_pApp->getIMainWnd()->getDockDebug().appendString("Получение структуры модели... ");
            std::stringstream model_structure;
            sendMessage(kernel->simulator(), Message::SIMULATOR_GET_MODEL_STRUCTURE, &model_structure);
            getModelStructure(model_structure);
            g_pApp->getIMainWnd()->getDockDebug().appendString("ok\n");
        }
        catch (...)
        {
            g_pApp->getIMainWnd()->getDockDebug().appendString("failed\n");
        }
        break;

    case RDOThread::Message::RUNTIME_MODEL_START_AFTER:
    {
        rdo::runtime::RunTimeMode runtimeMode;
        sendMessage(kernel->runtime(), Message::RUNTIME_GET_MODE, &runtimeMode);
        setRuntimeMode(runtimeMode);
        break;
    }

    case RDOThread::Message::SIMULATOR_MODEL_STOP_OK:
    case RDOThread::Message::SIMULATOR_MODEL_STOP_BY_USER:
    case RDOThread::Message::SIMULATOR_MODEL_STOP_RUNTIME_ERROR:
        if (s_clearAfterStop)
        {
            clear();
            s_clearAfterStop = false;
        }
        setDrawTrace(true);
        break;

    case RDOThread::Message::RUNTIME_TRACE_STRING:
        msg.lock();
        getTraceString(*static_cast<std::string*>(msg.param));
        msg.unlock();
        break;

    default:
        break;
    }
}

void Tracer::setRuntimeMode(const rdo::runtime::RunTimeMode value)
{
    if (value == rdo::runtime::RunTimeMode::MAX_SPEED)
    {
        setDrawTrace(false);
    }
    else
    {
        setDrawTrace(true);
    }
}

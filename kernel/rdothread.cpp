// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <fstream>
#include <algorithm>

#ifdef OST_WINDOWS
    #include <windows.h>
#else
    #include <pthread.h>
#endif // OST_WINDOWS
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
#include "kernel/rdokernel.h"
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

#ifdef TR_TRACE
std::fstream file("C:\\rdo_kernel_st.log", std::ios_base::in | std::ios_base::out | std::ios_base::trunc);

void RDOThread::trace(const std::string& str)
{
    file << str << std::endl;
    file.flush();
}
#endif // TR_TRACE

// --------------------------------------------------------------------------------
// -------------------- RDOThread
// --------------------------------------------------------------------------------
RDOThread::RDOThread(const std::string& _thread_name)
    : thread_name(_thread_name)
{
#ifdef TR_TRACE
    trace(thread_name + "::" + thread_name);
#endif
    // Чтобы треда не получала ниодного сообщения. RT_THREAD_CLOSE обрабатывается автоматически
    notifies.push_back(RT_THREAD_CLOSE);
}

RDOThread::~RDOThread()
{
#ifdef TR_TRACE
    trace(rdo::format("%s (%d)::~%s", thread_name.c_str(), thread_id, thread_name.c_str()));
//    trace(thread_name + "::~" + thread_name);
#endif
}

RDOThread* RDOThread::getKernel()
{
    return kernel;
}

void RDOThread::after_constructor()
{
    notifies.push_back(RT_THREAD_REGISTERED);
    if (getKernel() && getKernel() != this)
        sendMessage(getKernel(), RT_THREAD_CONNECTION);
    start();
}

void RDOThread::broadcastMessage(RDOTreadMessage message, void* pParam, bool /*lock*/)
{
    RDOMessageInfo msg(this, message, pParam);
    std::list< RDOThread* >::iterator it = kernel->threads.begin();
    while (it != kernel->threads.end())
    {
        RDOThread* thread = *it;
        if (thread != this && (thread->notifies.empty() || std::find(thread->notifies.begin(), thread->notifies.end(), message) != thread->notifies.end()))
            thread->processMessages(msg);
        ++it;
    }
}

void RDOThread::idle()
{}

void RDOThread::start()
{
#ifdef TR_TRACE
    trace( thread_name + " start" );
#endif
}

void RDOThread::stop()
{
#ifdef TR_TRACE
    trace( thread_name + " stop" );
#endif
    broadcastMessage( RT_THREAD_STOP_AFTER );
}

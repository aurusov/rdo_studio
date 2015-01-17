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
    // Чтобы треда не получала ниодного сообщения. Message::THREAD_CLOSE обрабатывается автоматически
    notifies.push_back(Message::THREAD_CLOSE);
}

RDOThread::~RDOThread()
{
#ifdef TR_TRACE
    trace(rdo::format("%s (%d)::~%s", thread_name.c_str(), thread_id, thread_name.c_str()));
//    trace(thread_name + "::~" + thread_name);
#endif
}

std::string RDOThread::messageToString(Message message)
{
    switch (message)
    {
    case Message::THREAD_CLOSE                      : return "THREAD_CLOSE";
    case Message::THREAD_CONNECTION                 : return "THREAD_CONNECTION";
    case Message::THREAD_DISCONNECTION              : return "THREAD_DISCONNECTION";
    case Message::THREAD_REGISTERED                 : return "THREAD_REGISTERED";
    case Message::THREAD_UNREGISTERED               : return "THREAD_UNREGISTERED";
    case Message::THREAD_STOP_AFTER                 : return "THREAD_STOP_AFTER";
    case Message::STUDIO_MODEL_NEW                  : return "STUDIO_MODEL_NEW";
    case Message::STUDIO_MODEL_OPEN                 : return "STUDIO_MODEL_OPEN";
    case Message::STUDIO_MODEL_SAVE                 : return "STUDIO_MODEL_SAVE";
    case Message::STUDIO_MODEL_CLOSE                : return "STUDIO_MODEL_CLOSE";
    case Message::STUDIO_MODEL_BUILD                : return "STUDIO_MODEL_BUILD";
    case Message::STUDIO_MODEL_RUN                  : return "STUDIO_MODEL_RUN";
    case Message::STUDIO_MODEL_STOP                 : return "STUDIO_MODEL_STOP";
    case Message::STUDIO_MODEL_GET_TEXT             : return "STUDIO_MODEL_GET_TEXT";
    case Message::REPOSITORY_MODEL_NEW              : return "REPOSITORY_MODEL_NEW";
    case Message::REPOSITORY_MODEL_OPEN             : return "REPOSITORY_MODEL_OPEN";
    case Message::REPOSITORY_MODEL_OPEN_ERROR       : return "REPOSITORY_MODEL_OPEN_ERROR";
    case Message::REPOSITORY_MODEL_SAVE             : return "REPOSITORY_MODEL_SAVE";
    case Message::REPOSITORY_MODEL_OPEN_GET_NAME    : return "REPOSITORY_MODEL_OPEN_GET_NAME";
    case Message::REPOSITORY_MODEL_CLOSE            : return "REPOSITORY_MODEL_CLOSE";
    case Message::REPOSITORY_MODEL_CLOSE_CAN_CLOSE  : return "REPOSITORY_MODEL_CLOSE_CAN_CLOSE";
    case Message::REPOSITORY_MODEL_CLOSE_ERROR      : return "REPOSITORY_MODEL_CLOSE_ERROR";
    case Message::REPOSITORY_MODEL_GET_FILEINFO     : return "REPOSITORY_MODEL_GET_FILEINFO";
    case Message::REPOSITORY_LOAD                   : return "REPOSITORY_LOAD";
    case Message::REPOSITORY_SAVE                   : return "REPOSITORY_SAVE";
    case Message::REPOSITORY_LOAD_BINARY            : return "REPOSITORY_LOAD_BINARY";
    case Message::REPOSITORY_CREATE_FILE            : return "REPOSITORY_CREATE_FILE";
    case Message::SIMULATOR_PARSE_OK                : return "SIMULATOR_PARSE_OK";
    case Message::SIMULATOR_PARSE_ERROR             : return "SIMULATOR_PARSE_ERROR";
    case Message::SIMULATOR_PARSE_ERROR_SMR         : return "SIMULATOR_PARSE_ERROR_SMR";
    case Message::SIMULATOR_PARSE_ERROR_SMR_EMPTY   : return "SIMULATOR_PARSE_ERROR_SMR_EMPTY";
    case Message::SIMULATOR_PARSE_STRING            : return "SIMULATOR_PARSE_STRING";
    case Message::SIMULATOR_MODEL_STOP_OK           : return "SIMULATOR_MODEL_STOP_OK";
    case Message::SIMULATOR_MODEL_STOP_BY_USER      : return "SIMULATOR_MODEL_STOP_BY_USER";
    case Message::SIMULATOR_MODEL_STOP_RUNTIME_ERROR: return "SIMULATOR_MODEL_STOP_RUNTIME_ERROR";
    case Message::SIMULATOR_MODEL_STOP_RUNTIME_DELAY: return "SIMULATOR_MODEL_STOP_RUNTIME_DELAY";
    case Message::SIMULATOR_GET_MODEL_STRUCTURE     : return "SIMULATOR_GET_MODEL_STRUCTURE";
    case Message::SIMULATOR_GET_MODEL_RESULTS       : return "SIMULATOR_GET_MODEL_RESULTS";
    case Message::SIMULATOR_GET_MODEL_RESULTS_INFO  : return "SIMULATOR_GET_MODEL_RESULTS_INFO";
    case Message::SIMULATOR_GET_MODEL_EXITCODE      : return "SIMULATOR_GET_MODEL_EXITCODE";
    case Message::SIMULATOR_GET_LIST                : return "SIMULATOR_GET_LIST";
    case Message::SIMULATOR_GET_ERRORS              : return "SIMULATOR_GET_ERRORS";
    case Message::RUNTIME_MODEL_START_BEFORE        : return "RUNTIME_MODEL_START_BEFORE";
    case Message::RUNTIME_MODEL_START_AFTER         : return "RUNTIME_MODEL_START_AFTER";
    case Message::RUNTIME_MODEL_STOP_BEFORE         : return "RUNTIME_MODEL_STOP_BEFORE";
    case Message::RUNTIME_MODEL_STOP_AFTER          : return "RUNTIME_MODEL_STOP_AFTER";
    case Message::RUNTIME_TRACE_STRING              : return "RUNTIME_TRACE_STRING";
    case Message::RUNTIME_GET_MODE                  : return "RUNTIME_GET_MODE";
    case Message::RUNTIME_SET_MODE                  : return "RUNTIME_SET_MODE";
    case Message::RUNTIME_GET_SPEED                 : return "RUNTIME_GET_SPEED";
    case Message::RUNTIME_SET_SPEED                 : return "RUNTIME_SET_SPEED";
    case Message::RUNTIME_GET_SHOWRATE              : return "RUNTIME_GET_SHOWRATE";
    case Message::RUNTIME_SET_SHOWRATE              : return "RUNTIME_SET_SHOWRATE";
    case Message::RUNTIME_GET_TIMENOW               : return "RUNTIME_GET_TIMENOW";
    case Message::RUNTIME_GET_FRAME                 : return "RUNTIME_GET_FRAME";
    case Message::RUNTIME_GET_LAST_BREAKPOINT       : return "RUNTIME_GET_LAST_BREAKPOINT";
    case Message::RUNTIME_KEY_DOWN                  : return "RUNTIME_KEY_DOWN";
    case Message::RUNTIME_KEY_UP                    : return "RUNTIME_KEY_UP";
    case Message::RUNTIME_FRAME_AREA_DOWN           : return "RUNTIME_FRAME_AREA_DOWN";
    case Message::CONVERTOR_NONE                    : return "CONVERTOR_NONE";
    case Message::CONVERTOR_OK                      : return "CONVERTOR_OK";
    case Message::CONVERTOR_ERROR                   : return "CONVERTOR_ERROR";
    case Message::CODECOMP_GET_DATA                 : return "CODECOMP_GET_DATA";
    case Message::DEBUG_STRING                      : return "DEBUG_STRING";
    case Message::RESULT_STRING                     : return "RESULT_STRING";
    case Message::CORBA_PARSER_GET_RTP              : return "CORBA_PARSER_GET_RTP";
    case Message::CORBA_PARSER_GET_RSS              : return "CORBA_PARSER_GET_RSS";
    case Message::CORBA_PARSER_GET_RTP_COUNT        : return "CORBA_PARSER_GET_RTP_COUNT";
    case Message::CORBA_PARSER_GET_RTP_PAR_COUNT    : return "CORBA_PARSER_GET_RTP_PAR_COUNT";
    default                                         : return "UNKNOWN";
    }
}

RDOThread* RDOThread::getKernel()
{
    return kernel;
}

void RDOThread::after_constructor()
{
    notifies.push_back(Message::THREAD_REGISTERED);
    if (getKernel() && getKernel() != this)
        sendMessage(getKernel(), Message::THREAD_CONNECTION);
    start();
}

void RDOThread::broadcastMessage(Message message, void* pParam, bool /*lock*/)
{
    RDOMessageInfo msg(this, message, pParam);
    std::list<RDOThread*>::const_iterator it = kernel->threads.begin();
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
    broadcastMessage(Message::THREAD_STOP_AFTER);
}

#pragma once

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <list>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>

#ifdef COMPILER_VISUAL_STUDIO
    #pragma warning( disable : 4786 )
#endif // COMPILER_VISUAL_STUDIO
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// #define RDOSIM_COMPATIBLE
//
// Задается через параметры проекта
// Используется для компиляции версии, более совместимой, со старым РДО
// Изменят (делает как было в старом РДО):
//   1. дисциплину одномоментных событий на LIFO
//   2. трассировку удаляемых и создаваемых ресурсов
//   3. трассировку события выводит после измененного этим события ресурса
//   4. выводит в трассировку вещественные значения с двумя знаками после экспоненты, а не тремя

// --------------------------------------------------------------------------------
// -------------------- RDOThread
// --------------------------------------------------------------------------------
class RDOThread: private boost::noncopyable
{
friend class RDOKernel;
public:
    enum class Message
    {
        THREAD_CLOSE = 1,
        THREAD_CONNECTION,
        THREAD_DISCONNECTION,
        THREAD_REGISTERED,                  // param = thread:RDOThread*
        THREAD_UNREGISTERED,                // param = thread:RDOThread*
        THREAD_STOP_AFTER,
        STUDIO_MODEL_NEW,                   // param = rdo::repository::RDOThreadRepository::NewModel*
        STUDIO_MODEL_OPEN,                  // param = rdo::repository::RDOThreadRepository::OpenFile*
        STUDIO_MODEL_SAVE,
        STUDIO_MODEL_CLOSE,
        STUDIO_MODEL_BUILD,
        STUDIO_MODEL_RUN,
        STUDIO_MODEL_STOP,
        STUDIO_MODEL_GET_TEXT,              // param = rdo::repository::RDOThreadRepository::FileData*
        REPOSITORY_MODEL_NEW,               // param = rdo::repository::RDOThreadRepository::NewModel*
        REPOSITORY_MODEL_OPEN,
        REPOSITORY_MODEL_OPEN_ERROR,        // param = model_name:boost::filesystem::path*
        REPOSITORY_MODEL_SAVE,
        REPOSITORY_MODEL_OPEN_GET_NAME,     // param = rdo::repository::RDOThreadRepository::OpenFile*
        REPOSITORY_MODEL_CLOSE,
        REPOSITORY_MODEL_CLOSE_CAN_CLOSE,   // param = result:bool*, работает как И
        REPOSITORY_MODEL_CLOSE_ERROR,
        REPOSITORY_MODEL_GET_FILEINFO,      // param = rdo::repository::RDOThreadRepository::FileInfo*
        REPOSITORY_LOAD,                    // param = rdo::repository::RDOThreadRepository::FileData*
        REPOSITORY_SAVE,                    // param = rdo::repository::RDOThreadRepository::FileData*
        REPOSITORY_LOAD_BINARY,             // param = rdo::repository::RDOThreadRepository::BinaryFile*
        REPOSITORY_CREATE_FILE,             // param = rdo::repository::RDOThreadRepository::CreateFile*
        SIMULATOR_PARSE_OK,
        SIMULATOR_PARSE_ERROR,
        SIMULATOR_PARSE_ERROR_SMR,
        SIMULATOR_PARSE_ERROR_SMR_EMPTY,
        SIMULATOR_PARSE_STRING,             // param = std::string*
        SIMULATOR_MODEL_STOP_OK,
        SIMULATOR_MODEL_STOP_BY_USER,
        SIMULATOR_MODEL_STOP_RUNTIME_ERROR,
        SIMULATOR_MODEL_STOP_RUNTIME_DELAY,
        SIMULATOR_GET_MODEL_STRUCTURE,      // param = result:std::stringstream*
        SIMULATOR_GET_MODEL_RESULTS,        // param = result:std::stringstream*
        SIMULATOR_GET_MODEL_RESULTS_INFO,   // param = result:std::stringstream*
        SIMULATOR_GET_MODEL_EXITCODE,       // param = result:rdo::simulation::report::ExitCode*
        SIMULATOR_GET_LIST,                 // param = rdo::service::simulation::RDOThreadSimulator::GetList*
        SIMULATOR_GET_ERRORS,               // param = result:std::vector< RDOSyntaxError >*
        RUNTIME_MODEL_START_BEFORE,
        RUNTIME_MODEL_START_AFTER,
        RUNTIME_MODEL_STOP_BEFORE,
        RUNTIME_MODEL_STOP_AFTER,
        RUNTIME_TRACE_STRING,               // param = std::string*
        RUNTIME_GET_MODE,                   // param = result:rdo::runtime::RunTimeMode*
        RUNTIME_SET_MODE,                   // param = mode:rdo::runtime::RunTimeMode*
        RUNTIME_GET_SPEED,                  // param = result:double[0..1]%*
        RUNTIME_SET_SPEED,                  // param = speed:double[0..1]%*
        RUNTIME_GET_SHOWRATE,               // param = result:double[0..+max_double]*
        RUNTIME_SET_SHOWRATE,               // param = show_rate:double[0..+max_double]*
        RUNTIME_GET_TIMENOW,                // param = result:double*
        RUNTIME_GET_FRAME,                  // param = { result:rdo::animation::Frame*, frame_number:int }*
        RUNTIME_GET_LAST_BREAKPOINT,        // param = std::string*
        RUNTIME_KEY_DOWN,                   // param = scanCode:uint*
        RUNTIME_KEY_UP,                     // param = scanCode:uint*
        RUNTIME_FRAME_AREA_DOWN,            // param = area_name:std::string*
        CONVERTOR_NONE,
        CONVERTOR_OK,
        CONVERTOR_ERROR,
        CODECOMP_GET_DATA,                  // param = rdo::service::simulation::RDOThreadCodeComp::GetCodeComp*
        DEBUG_STRING,                       // param = std::string*
        RESULT_STRING,                      // param = std::string*
        CORBA_PARSER_GET_RTP,
        CORBA_PARSER_GET_RSS,
        CORBA_PARSER_GET_RTP_COUNT,
        CORBA_PARSER_GET_RTP_PAR_COUNT,
    };

    static std::string messageToString(Message message);

    class RDOMessageInfo {
    friend class RDOThread;
    friend class RDOKernel;
    public:
        RDOThread* from;
        Message message;
        void* param;
        RDOMessageInfo(RDOThread* _from, Message _message, void* _param)
            : from   (_from   )
            , message(_message)
            , param  (_param  )
        {}
        RDOMessageInfo(const RDOMessageInfo& copy)
        : from   (copy.from   )
            , message(copy.message)
            , param  (copy.param  )
        {}
        void lock()   {}
        void unlock() {}
    };

    std::string getName() const { return thread_name; }

    // SEND: отправка сообщений с ожиданием выполнения
    void sendMessage(RDOThread* to, Message message, void* pParam = NULL)
    {
        RDOMessageInfo messageInfo(this, message, pParam);
        to->processMessages(messageInfo);
    }

    // Рассылка уведомлений всем тредам с учетом их notifies
    // Важно: должна вызываться только для this (в собственной треде)
    void broadcastMessage(Message message, void* pParam = NULL, bool lock = false);

protected:
    const std::string thread_name;

    typedef std::vector<Message> NotifieList;

    NotifieList notifies; // Список сообщений, на которые реагирует треда
                          // Если он пуст, то обрабатываются все сообщения.
                          // Message::THREAD_CLOSE обрабатывается в RDOThread::processMessages()
                          // Если сообщение посылается не из kernel'а, а напрямую, то
                          // то оно не будет проходить через этот фильтр, а сразу попадет в очередь.

    RDOThread* getKernel();

    // Создавать можно только через потомков
    RDOThread(const std::string& _thread_name);
    // Удаляет объкты функция треды. kernel удаляется через статический метод
    virtual ~RDOThread();

    // Надо обязательно вызвать из конструктора объекта, чтобы настроить правильно this для виртуальных функций,
    void after_constructor();

    virtual void proc(RDOMessageInfo& msg) = 0;
    virtual void idle();
    virtual void start();
    virtual void stop();
    virtual void destroy();

    void processMessages(RDOMessageInfo& msg);
};

// --------------------------------------------------------------------------------
// -------------------- RDOThreadMT
// --------------------------------------------------------------------------------
// Основной базовый класс для треды, которая работает в своей ветке
//
class RDOThreadMT: public RDOThread
{
protected:
    RDOThreadMT(const std::string& _thread_name)
        : RDOThread(_thread_name)
    {}
};

// --------------------------------------------------------------------------------
// -------------------- RDOThreadGUI
// --------------------------------------------------------------------------------
// Базовый класс для треды, которая получает сообщения от kernel_gui, а не от kernel.
// Используется как логическая вложенная треда в настоящую треду kernel_gui
//
class RDOThreadGUI: public RDOThread
{
protected:
    RDOThreadGUI(const std::string& _thread_name, RDOThread*)
        : RDOThread(_thread_name)
    {}
};

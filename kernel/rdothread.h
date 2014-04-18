#ifndef RDO_THREAD_H
#define RDO_THREAD_H

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
// Используется для компиляции (одно/много)-тредовой версии РДО
// Если определена в дефайнах проекта
#if defined(RDO_ST)
	#undef RDO_MT
// Если определена в дефайнах проекта
#elif defined(RDO_MT)
	#undef RDO_ST
// Если НЕ определена в дефайнах проекта
#else
	#define RDO_MT
	#undef RDO_MT // Скомпилить однотредувую версию РДО. Если закомментировать, то получится многотредовая

	// RDO_ST автоматически выставляется для однотредовой версии РДО
	#ifndef RDO_MT
		#define RDO_ST
	#endif
#endif

// Используется для трассировки сообщений в файл C:\rdo_kernel_mt.log
// Может быть использовано и в однотредовой версии
// Если НЕ определена в дефайнах проекта
#ifndef TR_TRACE
	#define TR_TRACE
	#undef TR_TRACE // Закомментировать для вывода трассировка
#endif

#ifdef RDO_MT
#include <afxmt.h>
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOThread
// --------------------------------------------------------------------------------
#ifdef RDO_MT
typedef std::size_t (*RDOThreadFun)(void* pParam);
#endif

class RDOThread: public boost::noncopyable
{
#ifdef RDO_MT
friend class RDOKernelGUI;
friend class RDOThreadGUI;
#else
friend class RDOKernel;
#endif
public:
	enum RDOTreadMessage {
		RT_THREAD_CLOSE = 1,
		RT_THREAD_CONNECTION,
		RT_THREAD_DISCONNECTION,
		RT_THREAD_REGISTERED,                  // param = thread:RDOThread*
		RT_THREAD_UNREGISTERED,                // param = thread:RDOThread*
		RT_THREAD_STOP_AFTER,
		RT_STUDIO_MODEL_NEW,                   // param = rdo::repository::RDOThreadRepository::NewModel*
		RT_STUDIO_MODEL_OPEN,                  // param = rdo::repository::RDOThreadRepository::OpenFile*
		RT_STUDIO_MODEL_SAVE,
		RT_STUDIO_MODEL_CLOSE,
		RT_STUDIO_MODEL_BUILD,
		RT_STUDIO_MODEL_RUN,
		RT_STUDIO_MODEL_STOP,
		RT_STUDIO_MODEL_GET_TEXT,              // param = rdo::repository::RDOThreadRepository::FileData*
		RT_REPOSITORY_MODEL_NEW,               // param = rdo::repository::RDOThreadRepository::NewModel*
		RT_REPOSITORY_MODEL_OPEN,
		RT_REPOSITORY_MODEL_OPEN_ERROR,        // param = model_name:boost::filesystem::path*
		RT_REPOSITORY_MODEL_SAVE,
		RT_REPOSITORY_MODEL_OPEN_GET_NAME,     // param = rdo::repository::RDOThreadRepository::OpenFile*
		RT_REPOSITORY_MODEL_CLOSE,
		RT_REPOSITORY_MODEL_CLOSE_CAN_CLOSE,   // param = result:bool*, работает как И
		RT_REPOSITORY_MODEL_CLOSE_ERROR,
		RT_REPOSITORY_MODEL_GET_FILEINFO,      // param = rdo::repository::RDOThreadRepository::FileInfo*
		RT_REPOSITORY_LOAD,                    // param = rdo::repository::RDOThreadRepository::FileData*
		RT_REPOSITORY_SAVE,                    // param = rdo::repository::RDOThreadRepository::FileData*
		RT_REPOSITORY_LOAD_BINARY,             // param = rdo::repository::RDOThreadRepository::BinaryFile*
		RT_REPOSITORY_CREATE_FILE,             // param = rdo::repository::RDOThreadRepository::CreateFile*
		RT_SIMULATOR_PARSE_OK,
		RT_SIMULATOR_PARSE_ERROR,
		RT_SIMULATOR_PARSE_ERROR_SMR,
		RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY,
		RT_SIMULATOR_PARSE_STRING,             // param = std::string*
		RT_SIMULATOR_MODEL_STOP_OK,
		RT_SIMULATOR_MODEL_STOP_BY_USER,
		RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR,
		RT_SIMULATOR_MODEL_STOP_RUNTIME_DELAY,
		RT_SIMULATOR_GET_MODEL_STRUCTURE,      // param = result:std::stringstream*
		RT_SIMULATOR_GET_MODEL_RESULTS,        // param = result:std::stringstream*
		RT_SIMULATOR_GET_MODEL_RESULTS_INFO,   // param = result:std::stringstream*
		RT_SIMULATOR_GET_MODEL_EXITCODE,       // param = result:rdo::simulation::report::RDOExitCode*
		RT_SIMULATOR_GET_LIST,                 // param = rdo::service::simulation::RDOThreadSimulator::GetList*
		RT_SIMULATOR_GET_ERRORS,               // param = result:std::vector< RDOSyntaxError >*
		RT_RUNTIME_MODEL_START_BEFORE,
		RT_RUNTIME_MODEL_START_AFTER,
		RT_RUNTIME_MODEL_STOP_BEFORE,
		RT_RUNTIME_MODEL_STOP_AFTER,
		RT_RUNTIME_TRACE_STRING,               // param = std::string*
		RT_RUNTIME_GET_MODE,                   // param = result:rdo::runtime::RunTimeMode*
		RT_RUNTIME_SET_MODE,                   // param = mode:rdo::runtime::RunTimeMode*
		RT_RUNTIME_GET_SPEED,                  // param = result:double[0..1]%*
		RT_RUNTIME_SET_SPEED,                  // param = speed:double[0..1]%*
		RT_RUNTIME_GET_SHOWRATE,               // param = result:double[0..+max_double]*
		RT_RUNTIME_SET_SHOWRATE,               // param = show_rate:double[0..+max_double]*
		RT_RUNTIME_GET_TIMENOW,                // param = result:double*
		RT_RUNTIME_GET_FRAME,                  // param = { result:rdo::animation::Frame*, frame_number:int }*
		RT_RUNTIME_GET_LAST_BREAKPOINT,        // param = std::string*
		RT_RUNTIME_KEY_DOWN,                   // param = scanCode:uint*
		RT_RUNTIME_KEY_UP,                     // param = scanCode:uint*
		RT_RUNTIME_FRAME_AREA_DOWN,            // param = area_name:std::string*
		RT_CONVERTOR_NONE,
		RT_CONVERTOR_OK,
		RT_CONVERTOR_ERROR,
		RT_CODECOMP_GET_DATA,                  // param = rdo::service::simulation::RDOThreadCodeComp::GetCodeComp*
		RT_DEBUG_STRING,                       // param = std::string*
		RT_RESULT_STRING,                      // param = std::string*
		RT_CORBA_PARSER_GET_RTP,
		RT_CORBA_PARSER_GET_RSS,
		RT_CORBA_PARSER_GET_RTP_COUNT,
		RT_CORBA_PARSER_GET_RTP_PAR_COUNT,
		RT_PROCGUI_BLOCK_CREATE,
		RT_PROCGUI_BLOCK_PROCESS,
		RT_PROCGUI_BLOCK_TERMINATE
	};
	std::string messageToString(RDOTreadMessage message) {
		switch ( message ) {
			case RT_THREAD_CLOSE                      : return "RT_THREAD_CLOSE";
			case RT_THREAD_CONNECTION                 : return "RT_THREAD_CONNECTION";
			case RT_THREAD_DISCONNECTION              : return "RT_THREAD_DISCONNECTION";
			case RT_THREAD_REGISTERED                 : return "RT_THREAD_REGISTERED";
			case RT_THREAD_UNREGISTERED               : return "RT_THREAD_UNREGISTERED";
			case RT_THREAD_STOP_AFTER                 : return "RT_THREAD_STOP_AFTER";
			case RT_STUDIO_MODEL_NEW                  : return "RT_STUDIO_MODEL_NEW";
			case RT_STUDIO_MODEL_OPEN                 : return "RT_STUDIO_MODEL_OPEN";
			case RT_STUDIO_MODEL_SAVE                 : return "RT_STUDIO_MODEL_SAVE";
			case RT_STUDIO_MODEL_CLOSE                : return "RT_STUDIO_MODEL_CLOSE";
			case RT_STUDIO_MODEL_BUILD                : return "RT_STUDIO_MODEL_BUILD";
			case RT_STUDIO_MODEL_RUN                  : return "RT_STUDIO_MODEL_RUN";
			case RT_STUDIO_MODEL_STOP                 : return "RT_STUDIO_MODEL_STOP";
			case RT_STUDIO_MODEL_GET_TEXT             : return "RT_STUDIO_MODEL_GET_TEXT";
			case RT_REPOSITORY_MODEL_NEW              : return "RT_REPOSITORY_MODEL_NEW";
			case RT_REPOSITORY_MODEL_OPEN             : return "RT_REPOSITORY_MODEL_OPEN";
			case RT_REPOSITORY_MODEL_OPEN_ERROR       : return "RT_REPOSITORY_MODEL_OPEN_ERROR";
			case RT_REPOSITORY_MODEL_SAVE             : return "RT_REPOSITORY_MODEL_SAVE";
			case RT_REPOSITORY_MODEL_OPEN_GET_NAME    : return "RT_REPOSITORY_MODEL_OPEN_GET_NAME";
			case RT_REPOSITORY_MODEL_CLOSE            : return "RT_REPOSITORY_MODEL_CLOSE";
			case RT_REPOSITORY_MODEL_CLOSE_CAN_CLOSE  : return "RT_REPOSITORY_MODEL_CLOSE_CAN_CLOSE";
			case RT_REPOSITORY_MODEL_CLOSE_ERROR      : return "RT_REPOSITORY_MODEL_CLOSE_ERROR";
			case RT_REPOSITORY_MODEL_GET_FILEINFO     : return "RT_REPOSITORY_MODEL_GET_FILEINFO";
			case RT_REPOSITORY_LOAD                   : return "RT_REPOSITORY_LOAD";
			case RT_REPOSITORY_SAVE                   : return "RT_REPOSITORY_SAVE";
			case RT_REPOSITORY_LOAD_BINARY            : return "RT_REPOSITORY_LOAD_BINARY";
			case RT_REPOSITORY_CREATE_FILE            : return "RT_REPOSITORY_CREATE_FILE";
			case RT_SIMULATOR_PARSE_OK                : return "RT_SIMULATOR_PARSE_OK";
			case RT_SIMULATOR_PARSE_ERROR             : return "RT_SIMULATOR_PARSE_ERROR";
			case RT_SIMULATOR_PARSE_ERROR_SMR         : return "RT_SIMULATOR_PARSE_ERROR_SMR";
			case RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY   : return "RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY";
			case RT_SIMULATOR_PARSE_STRING            : return "RT_SIMULATOR_PARSE_STRING";
			case RT_SIMULATOR_MODEL_STOP_OK           : return "RT_SIMULATOR_MODEL_STOP_OK";
			case RT_SIMULATOR_MODEL_STOP_BY_USER      : return "RT_SIMULATOR_MODEL_STOP_BY_USER";
			case RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR: return "RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR";
			case RT_SIMULATOR_MODEL_STOP_RUNTIME_DELAY: return "RT_SIMULATOR_MODEL_STOP_RUNTIME_DELAY";
			case RT_SIMULATOR_GET_MODEL_STRUCTURE     : return "RT_SIMULATOR_GET_MODEL_STRUCTURE";
			case RT_SIMULATOR_GET_MODEL_RESULTS       : return "RT_SIMULATOR_GET_MODEL_RESULTS";
			case RT_SIMULATOR_GET_MODEL_RESULTS_INFO  : return "RT_SIMULATOR_GET_MODEL_RESULTS_INFO";
			case RT_SIMULATOR_GET_MODEL_EXITCODE      : return "RT_SIMULATOR_GET_MODEL_EXITCODE";
			case RT_SIMULATOR_GET_LIST                : return "RT_SIMULATOR_GET_LIST";
			case RT_SIMULATOR_GET_ERRORS              : return "RT_SIMULATOR_GET_ERRORS";
			case RT_RUNTIME_MODEL_START_BEFORE        : return "RT_RUNTIME_MODEL_START_BEFORE";
			case RT_RUNTIME_MODEL_START_AFTER         : return "RT_RUNTIME_MODEL_START_AFTER";
			case RT_RUNTIME_MODEL_STOP_BEFORE         : return "RT_RUNTIME_MODEL_STOP_BEFORE";
			case RT_RUNTIME_MODEL_STOP_AFTER          : return "RT_RUNTIME_MODEL_STOP_AFTER";
			case RT_RUNTIME_TRACE_STRING              : return "RT_RUNTIME_TRACE_STRING";
			case RT_RUNTIME_GET_MODE                  : return "RT_RUNTIME_GET_MODE";
			case RT_RUNTIME_SET_MODE                  : return "RT_RUNTIME_SET_MODE";
			case RT_RUNTIME_GET_SPEED                 : return "RT_RUNTIME_GET_SPEED";
			case RT_RUNTIME_SET_SPEED                 : return "RT_RUNTIME_SET_SPEED";
			case RT_RUNTIME_GET_SHOWRATE              : return "RT_RUNTIME_GET_SHOWRATE";
			case RT_RUNTIME_SET_SHOWRATE              : return "RT_RUNTIME_SET_SHOWRATE";
			case RT_RUNTIME_GET_TIMENOW               : return "RT_RUNTIME_GET_TIMENOW";
			case RT_RUNTIME_GET_FRAME                 : return "RT_RUNTIME_GET_FRAME";
			case RT_RUNTIME_GET_LAST_BREAKPOINT       : return "RT_RUNTIME_GET_LAST_BREAKPOINT";
			case RT_RUNTIME_KEY_DOWN                  : return "RT_RUNTIME_KEY_DOWN";
			case RT_RUNTIME_KEY_UP                    : return "RT_RUNTIME_KEY_UP";
			case RT_RUNTIME_FRAME_AREA_DOWN           : return "RT_RUNTIME_FRAME_AREA_DOWN";
			case RT_CONVERTOR_NONE                    : return "RT_CONVERTOR_NONE";
			case RT_CONVERTOR_OK                      : return "RT_CONVERTOR_OK";
			case RT_CONVERTOR_ERROR                   : return "RT_CONVERTOR_ERROR";
			case RT_CODECOMP_GET_DATA                 : return "RT_CODECOMP_GET_DATA";
			case RT_DEBUG_STRING                      : return "RT_DEBUG_STRING";
			case RT_RESULT_STRING                     : return "RT_RESULT_STRING";
			case RT_CORBA_PARSER_GET_RTP              : return "RT_CORBA_PARSER_GET_RTP";
			case RT_CORBA_PARSER_GET_RSS              : return "RT_CORBA_PARSER_GET_RSS";
			case RT_CORBA_PARSER_GET_RTP_COUNT        : return "RT_CORBA_PARSER_GET_RTP_COUNT";
			case RT_CORBA_PARSER_GET_RTP_PAR_COUNT    : return "RT_CORBA_PARSER_GET_RTP_PAR_COUNT";
			case RT_PROCGUI_BLOCK_CREATE              : return "RT_PROCGUI_BLOCK_CREATE";
			case RT_PROCGUI_BLOCK_PROCESS             : return "RT_PROCGUI_BLOCK_PROCESS";
			case RT_PROCGUI_BLOCK_TERMINATE           : return "RT_PROCGUI_BLOCK_TERMINATE";
			default                                   : return "RT_UNKNOWN";
		}
	}

	class RDOMessageInfo {
	friend class RDOThread;
	friend class RDOKernel;
#ifdef RDO_MT
	private:
		enum Type { post = 0, send, manual } type;
		CEvent* send_event;
		CMutex* param_lock;
#endif
	public:
		RDOThread* from;
		RDOTreadMessage message;
		void* param;
#ifdef RDO_MT
		RDOMessageInfo(RDOThread* _from, RDOTreadMessage _message, void* _param, Type _type )
			: from      (_from   )
			, message   (_message)
			, param     (_param  )
			, type      (_type   )
			, send_event(NULL    )
			, param_lock(NULL    )
		{}
		RDOMessageInfo(const RDOMessageInfo& copy)
			: type      (copy.type      )
			, send_event(copy.send_event)
			, from      (copy.from      )
			, message   (copy.message   )
			, param     (copy.param     )
			, param_lock(copy.param_lock)
		{}
		void lock()   { if ( param_lock ) param_lock->Lock();   }
		void unlock() { if ( param_lock ) param_lock->Unlock(); }
#else
		RDOMessageInfo(RDOThread* _from, RDOTreadMessage _message, void* _param)
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
#endif
	};

	std::string getName() const { return thread_name; }
	std::size_t getID() const { return thread_id; }
#ifdef RDO_MT
	bool isGUI() const { return thread_fun ? false : true; }
	CEvent* getDestroyEvent() const { return thread_destroy; }
#endif

/*
	class RDOTreadMethod {
	public:
		RDOThread* thread;
		std::string name;
		std::size_t index;
	};

	// POST: отправка сообщений без ожидания выполнения
	void postMessage( RDOTreadMessage message, void* pParam = NULL ) {
		postMessageFrom( NULL, message, pParam );
	}
	void postMessageFrom( RDOThread* from, RDOTreadMessage message, void* pParam = NULL ) {
		messages_mutex.Lock();
		messages.push_back( RDOMessageInfo( from, message, pParam, RDOThread::RDOMessageInfo::post ) );
		messages_mutex.Unlock();
	}
*/
	// SEND: отправка сообщений с ожиданием выполнения
	void sendMessage(RDOThread* to, RDOTreadMessage message, void* pParam = NULL)
	{
#ifdef RDO_MT
		RDOMessageInfo msg(this, message, pParam, RDOThread::RDOMessageInfo::send);
		CEvent event;
		msg.send_event = &event;

		to->messages_mutex.Lock();
		if (!to->was_close)
			to->messages.push_back(msg);

		to->messages_mutex.Unlock();

		while (::WaitForSingleObject(event.m_hObject, 0) == WAIT_TIMEOUT)
			processMessages();
#else
		RDOMessageInfo messageInfo(this, message, pParam);
		to->processMessages(messageInfo);
#endif
	}

#ifdef RDO_MT
	// MANUAL: отправка сообщений с 'ручным' ожиданием выполнения для this
	CEvent* manualMessageFrom(RDOTreadMessage message, void* pParam = NULL);
#endif

	// Рассылка уведомлений всем тредам с учетом их notifies
	// Важно: должна вызываться только для this (в собственной треде)
	void broadcastMessage(RDOTreadMessage message, void* pParam = NULL, bool lock = false);

#ifdef TR_TRACE
	static void trace(const std::string& str);
#endif

protected:
#ifdef RDO_MT
	// Есть два ограничения на использование тред в RDO_MT (с thread-safety всё в порядке, imho):
	// 1. Каждая треда имеет доступ к стеку сообщений (messages) любой другой треды, чтобы положить туда новое сообщение.
	// 2. Каждая треда имеет доступ к списку уведомлений (notifies), чтобы понять, а надо ли посылать сообщение треде.
	// Второе еще можно сделать через дублирование: map< key = thread*, value = notifies > в каджой треде,
	// а вот как добавить сообщение - не совсем понрятно.
	static std::size_t threadFun(void* pParam);
	const RDOThreadFun thread_fun;
	CWinThread* thread_win;
	CEvent proc_create; // Вызывается из процедуры треды, конструктор должен его дождаться
	CEvent thread_create; // Вызывается из конструктора объекта, процедура должна его дождаться
	CEvent* thread_destroy; // Вызывается из деструктора объекта
	bool broadcast_waiting; // Без мутекса, т.к. меняется только в одной треде
	bool was_start; // Без мутекса, т.к. меняется только в одной треде
	bool was_close;
#endif
	const std::string thread_name;
	int thread_id;
	int idle_cnt;

	typedef std::vector<RDOTreadMessage> NotifieList;

	NotifieList notifies; // Список сообщений, на которые реагирует треда
	                      // Если он пуст, то обрабатываются все сообщения.
	                      // RT_THREAD_CLOSE обрабатывается в RDOThread::processMessages()
	                      // Если сообщение посылается не из kernel'а, а напрямую, то
	                      // то оно не будет проходить через этот фильтр, а сразу попадет в очередь.

#ifdef RDO_MT
	CMutex notifies_mutex;

	// Очередь сообщений
	typedef std::list<RDOMessageInfo> MessageList;
	MessageList messages;
	CMutex messages_mutex;

	class BroadcastData {
	public:
		typedef std::vector<CEvent*> EventList;
		EventList events;
		std::size_t cnt;
		HANDLE* handles;

		BroadcastData()
			: cnt    (0   )
			, handles(NULL)
		{}
		BroadcastData(std::size_t _cnt)
			: cnt(_cnt)
		{
			for (std::size_t i = 0; i < cnt; i++ )
				events.push_back(new CEvent());
			handles = new HANDLE[cnt];
		}
		BroadcastData(const BroadcastData& copy)
			: cnt    (copy.cnt    )
			, handles(copy.handles)
		{
			events.assign(copy.events.begin(), copy.events.end());
		}
		~BroadcastData()
		{}

		void clear()
		{
			for (std::size_t i = 0; i < cnt; i++)
				delete events[i];
			delete[] handles;
		}
		void resize()
		{
			if (cnt)
			{
				events.resize(cnt * 2);
				HANDLE* handles_backup = handles;
				handles = new HANDLE[cnt * 2];
				for (std::size_t i = 0; i < cnt; i++)
				{
					events.push_back(new CEvent());
					handles[i] = handles_backup[i];
					handles[cnt+i] = new HANDLE;
				}
				delete[] handles_backup;
				cnt *= 2;
			}
			else
			{
				cnt = 10;
				for (std::size_t i = 0; i < cnt; i++)
					events.push_back(new CEvent());
				handles = new HANDLE[cnt];
			}
		}
	};
	typedef std::vector<BroadcastData> BroadcastDataList;
	BroadcastDataList broadcast_data;
	std::size_t broadcast_cnt;

#endif

#ifdef RDO_MT
	virtual RDOThread* getKernel();
#else
	RDOThread* getKernel();
#endif

	// Создавать можно только через потомков
#ifdef RDO_MT
	RDOThread(const std::string& _thread_name, RDOThreadFun _thread_fun = NULL);
#else
	RDOThread(const std::string& _thread_name);
#endif
	// Удаляет объкты функция треды. kernel удаляется через статический метод
	virtual ~RDOThread();                                                                          

	// Надо обязательно вызвать из конструктора объекта, чтобы настроить правильно this для виртуальных функций,
	void after_constructor();

	virtual void  proc (RDOMessageInfo& msg) = 0;
	virtual void  idle ();
	virtual void  start();
	virtual void  stop ();
#ifdef RDO_MT
	virtual bool processMessages();
#else
	void          processMessages(RDOMessageInfo& msg)
	{
#ifdef TR_TRACE
		RDOThread::trace("---------------- " + messageToString(msg.message) + ": " + (msg.from ? msg.from->thread_name : "NULL") + " -> " + thread_name);
#endif
		proc(msg);
		if (msg.message == RT_THREAD_CLOSE)
		{
			if (this != getKernel())
				sendMessage(getKernel(), RT_THREAD_DISCONNECTION);
			stop();
			destroy();
			return;
		}
	}
#endif

	virtual void destroy()
	{
		delete this;
	}
};

// --------------------------------------------------------------------------------
// -------------------- RDOThreadMT
// --------------------------------------------------------------------------------
// Основной базовый класс для треды, которая работает в своей ветке
//
class RDOThreadMT: public RDOThread
{
protected:
#ifdef RDO_MT
	RDOThreadMT(const std::string& _thread_name, RDOThreadFun _thread_fun = RDOThread::threadFun)
		: RDOThread(_thread_name, _thread_fun)
	{}
#else
	RDOThreadMT(const std::string& _thread_name)
		: RDOThread(_thread_name)
	{}
#endif
};

// --------------------------------------------------------------------------------
// -------------------- RDOThreadGUI
// --------------------------------------------------------------------------------
// Базовый класс для треды, которая получает сообщения от kernel_gui, а не от kernel.
// Используется как логическая вложенная треда в настоящую треду kernel_gui
//
#ifdef RDO_MT
class RDOThreadGUI: public RDOThread
{
friend class RDOKernelGUI;
private:
	RDOThread* kernel_gui;

protected:
	RDOThreadGUI(const std::string& _thread_name, RDOThread* _kernel_gui)
		: RDOThread (_thread_name)
		, kernel_gui(_kernel_gui )
	{}
	virtual RDOThread* getKernel();
	virtual bool processMessages();
};
#else
class RDOThreadGUI: public RDOThread
{
protected:
	RDOThreadGUI(const std::string& _thread_name, RDOThread*)
		: RDOThread(_thread_name)
	{}
};
#endif

#endif // RDO_THREAD_H

#ifndef RDO_THREAD_H
#define RDO_THREAD_H

#if _MSC_VER > 1000
#pragma once
#endif

#pragma warning( disable : 4786 )

#include <string>
#include <list>
#include <vector>
#include <afxmt.h>

// --------------------------------------------------------------------
// ---------- RDOThread
// --------------------------------------------------------------------
typedef unsigned int (*RDOThreadFun)( void* param );

#define TR_TRACE
#undef TR_TRACE

class RDOThread
{
friend class RDOKernel;

public:
	enum RDOTreadMessage {
		RT_THREAD_CLOSE = 1,
		RT_THREAD_CONNECTION,
		RT_THREAD_DISCONNECTION,
		RT_THREAD_REGISTERED,
		RT_THREAD_UNREGISTERED,
		RT_THREAD_STOP_AFTER,
		RT_STUDIO_MODEL_NEW,
		RT_STUDIO_MODEL_OPEN,
		RT_STUDIO_MODEL_SAVE,
		RT_STUDIO_MODEL_SAVE_AS,
		RT_STUDIO_MODEL_CLOSE,
		RT_STUDIO_MODEL_BUILD,
		RT_STUDIO_MODEL_RUN,
		RT_STUDIO_MODEL_STOP,
		RT_REPOSITORY_MODEL_NEW,
		RT_REPOSITORY_MODEL_OPEN,
		RT_REPOSITORY_MODEL_SAVE,
		RT_REPOSITORY_MODEL_CLOSE,
		RT_REPOSITORY_MODEL_CLOSE_CAN_CLOSE, // Работает как И
		RT_REPOSITORY_MODEL_CLOSE_ERROR,
		RT_REPOSITORY_LOAD,
		RT_REPOSITORY_SAVE,
		RT_SIMULATOR_PARSE_OK,
		RT_SIMULATOR_PARSE_ERROR,
		RT_SIMULATOR_PARSE_ERROR_SMR,
		RT_SIMULATOR_PARSE_STRING,
		RT_SIMULATOR_MODEL_START_BEFORE,
		RT_SIMULATOR_MODEL_START_AFTER,
		RT_SIMULATOR_MODEL_STOP_OK,
		RT_SIMULATOR_MODEL_STOP_BY_USER,
		RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR,
		RT_SIMULATOR_TRACE_STRING,
		RT_SIMULATOR_FRAME_SHOW,
		RT_SIMULATOR_GET_ERRORS,
		RT_DEBUG_STRING,
		RT_TEST1,
		RT_TEST2,
		RT_TEST3
	};
	std::string messageToString( RDOTreadMessage message ) {
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
			case RT_STUDIO_MODEL_SAVE_AS              : return "RT_STUDIO_MODEL_SAVE_AS";
			case RT_STUDIO_MODEL_CLOSE                : return "RT_STUDIO_MODEL_CLOSE";
			case RT_STUDIO_MODEL_BUILD                : return "RT_STUDIO_MODEL_BUILD";
			case RT_STUDIO_MODEL_RUN                  : return "RT_STUDIO_MODEL_RUN";
			case RT_STUDIO_MODEL_STOP                 : return "RT_STUDIO_MODEL_STOP";
			case RT_REPOSITORY_MODEL_NEW              : return "RT_REPOSITORY_MODEL_NEW";
			case RT_REPOSITORY_MODEL_OPEN             : return "RT_REPOSITORY_MODEL_OPEN";
			case RT_REPOSITORY_MODEL_SAVE             : return "RT_REPOSITORY_MODEL_SAVE";
			case RT_REPOSITORY_MODEL_CLOSE            : return "RT_REPOSITORY_MODEL_CLOSE";
			case RT_REPOSITORY_MODEL_CLOSE_CAN_CLOSE  : return "RT_REPOSITORY_MODEL_CLOSE_CAN_CLOSE";
			case RT_REPOSITORY_MODEL_CLOSE_ERROR      : return "RT_REPOSITORY_MODEL_CLOSE_ERROR";
			case RT_REPOSITORY_LOAD                   : return "RT_REPOSITORY_LOAD";
			case RT_REPOSITORY_SAVE                   : return "RT_REPOSITORY_SAVE";
			case RT_SIMULATOR_PARSE_OK                : return "RT_SIMULATOR_PARSE_OK";
			case RT_SIMULATOR_PARSE_ERROR             : return "RT_SIMULATOR_PARSE_ERROR";
			case RT_SIMULATOR_PARSE_ERROR_SMR         : return "RT_SIMULATOR_PARSE_ERROR_SMR";
			case RT_SIMULATOR_PARSE_STRING            : return "RT_SIMULATOR_PARSE_STRING";
			case RT_SIMULATOR_MODEL_START_BEFORE      : return "RT_SIMULATOR_MODEL_START_BEFORE";
			case RT_SIMULATOR_MODEL_START_AFTER       : return "RT_SIMULATOR_MODEL_START_AFTER";
			case RT_SIMULATOR_MODEL_STOP_OK           : return "RT_SIMULATOR_MODEL_STOP_OK";
			case RT_SIMULATOR_MODEL_STOP_BY_USER      : return "RT_SIMULATOR_MODEL_STOP_BY_USER";
			case RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR: return "RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR";
			case RT_SIMULATOR_TRACE_STRING            : return "RT_SIMULATOR_TRACE_STRING";
			case RT_SIMULATOR_FRAME_SHOW              : return "RT_SIMULATOR_FRAME_SHOW";
			case RT_SIMULATOR_GET_ERRORS              : return "RT_SIMULATOR_GET_ERRORS";
			case RT_DEBUG_STRING                      : return "RT_DEBUG_STRING";
			case RT_TEST1                             : return "RT_TEST1";
			case RT_TEST2                             : return "RT_TEST2";
			case RT_TEST3                             : return "RT_TEST3";
			default                                   : return "RT_UNKNOWN";
		}
	}

	class RDOMessageInfo {
	friend class RDOThread;
	friend class RDOKernel;
	private:
		enum Type { post = 0, send, manual } type;
		CEvent* send_event;
		CMutex* param_lock;
	public:
		RDOThread*      from;
		RDOTreadMessage message;
		void*           param;
		RDOMessageInfo( RDOThread* _from, RDOTreadMessage _message, void* _param, Type _type ): from( _from ), message( _message ), param( _param ), type( _type ), send_event( NULL ), param_lock( NULL ) {}
		RDOMessageInfo( const RDOMessageInfo& copy ): type( copy.type ), send_event( copy.send_event ), from( copy.from ), message( copy.message ), param( copy.param ), param_lock( copy.param_lock ) {}
		void lock()   { if ( param_lock ) param_lock->Lock();   }
		void unlock() { if ( param_lock ) param_lock->Unlock(); }
	};

/*
	class RDOTreadMethod {
	public:
		RDOThread*   thread;
		std::string  name;
		unsigned int index;
	};

	// POST: отправка сообщений без ожидания выполнения
	void postMessage( RDOTreadMessage message, void* param = NULL ) {
		postMessageFrom( NULL, message, param );
	}
	void postMessageFrom( RDOThread* from, RDOTreadMessage message, void* param = NULL ) {
		messages_mutex.Lock();
		messages.push_back( RDOMessageInfo( from, message, param, RDOThread::RDOMessageInfo::post ) );
		messages_mutex.Unlock();
	}
*/
	// SEND: отправка сообщений с ожиданием выполнения
	void sendMessage( RDOThread* to, RDOTreadMessage message, void* param = NULL );

	// MANUAL: отправка сообщений с 'ручным' ожиданием выполнения для this
	CEvent* manualMessageFrom( RDOTreadMessage message, void* param = NULL );

	// Рассылка уведомлений всем тредам с учетом их notifies
	// Важно: должна вызываться только для this (в собственной треде)
	void broadcastMessage( RDOTreadMessage message, void* param = NULL );

protected:
	static unsigned int threadFun( void* param );
	const RDOThreadFun thread_fun;
	const std::string  thread_name;
	CWinThread*        thread_win;
	CEvent             proc_create;    // Вызывается из процедуры треды, конструктор должен его дождаться
	CEvent             thread_create;  // Вызывается из конструктора объекта, процедура должна его дождаться
	CEvent*            thread_destroy; // Вызывается из деструктора объекта
	bool               was_start;         // Без мутекса, т.к. меняется только в одной треде
	bool               was_close;
	bool               broadcast_waiting; // Без мутекса, т.к. меняется только в одной треде

	std::vector< RDOTreadMessage > notifies; // Список сообщений, на которые реагирует треда
	                                         // Если он пуст, то обрабатываются все сообщения.
											 // RT_THREAD_CLOSE обрабатывается всегда в RDOThread::proc()
	                                         // Если сообщение посылается не из kernel'а, а напрямую, то
											 // то оно не будет проходить через этот фильтр, а сразу попадет в очередь.
	CMutex                         notifies_mutex;

	// Очередь сообщений
	std::list< RDOMessageInfo > messages;
	CMutex                      messages_mutex;

	// Есть два ограничения на использование тред (с thread-safety всё в порядке, imho):
	// 1. Каждая треда имеет доступ к стеку сообщений (messages) любой другой треды, чтобы положить туда новое сообщение.
	// 2. Каждая треда имеет доступ к списку уведомлений (notifies), чтобы понять, а надо ли посылать сообщение треде.
	// Второе еще можно сделать через дублирование: map< key = thread*, value = notifies > в каджой треде,
	// а вот как добавить сообщение - не совсем понрятно.

	// Создавать можно только через потомков
	RDOThread( const std::string& _thread_name, RDOThreadFun _thread_fun = RDOThread::threadFun );
	// Удаляет объкты функция треды. kernel удаляется через статический метод
	virtual ~RDOThread();                                                                          

	// Надо обязательно вызвать из конструктора объекта, чтобы настроить правильно this для виртуальных функций,
	void after_constructor();

	virtual void proc( RDOMessageInfo& msg ) = 0;
	virtual void idle();
	virtual void start();
	virtual void stop();
	virtual bool processMessages();

#ifdef TR_TRACE
	static void trace( const std::string& str );
#endif
};

#endif // RDO_THREAD_H

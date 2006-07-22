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
// Используется для компиляции (одно/много)-тредовой версии РДО
// Если определена в дефайнах проекта
#if defined(RDO_SP)
	#undef RDO_MT
// Если определена в дефайнах проекта
#elif defined(RDO_MT)
	#undef RDO_SP
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

// --------------------------------------------------------------------
// ---------- RDOThread
// --------------------------------------------------------------------
#ifdef RDO_MT
typedef unsigned int (*RDOThreadFun)( void* param );
#endif

class RDOThread
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
#ifdef RDO_MT
	private:
		enum Type { post = 0, send, manual } type;
		CEvent* send_event;
		CMutex* param_lock;
#endif
	public:
		RDOThread*      from;
		RDOTreadMessage message;
		void*           param;
#ifdef RDO_MT
		RDOMessageInfo( RDOThread* _from, RDOTreadMessage _message, void* _param, Type _type ): from( _from ), message( _message ), param( _param ), type( _type ), send_event( NULL ), param_lock( NULL ) {}
		RDOMessageInfo( const RDOMessageInfo& copy ): type( copy.type ), send_event( copy.send_event ), from( copy.from ), message( copy.message ), param( copy.param ), param_lock( copy.param_lock ) {}
		void lock()   { if ( param_lock ) param_lock->Lock();   }
		void unlock() { if ( param_lock ) param_lock->Unlock(); }
#else
		RDOMessageInfo( RDOThread* _from, RDOTreadMessage _message, void* _param ): from( _from ), message( _message ), param( _param ) {}
		RDOMessageInfo( const RDOMessageInfo& copy ): from( copy.from ), message( copy.message ), param( copy.param ) {}
		void lock()   {}
		void unlock() {}
#endif
	};

	std::string getName() const         { return thread_name;               }
	int         getID() const           { return thread_id;                 }
#ifdef RDO_MT
	bool        isGUI() const           { return thread_fun ? false : true; }
	CEvent*     getDestroyEvent() const { return thread_destroy;            }
#endif

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
	void sendMessage( RDOThread* to, RDOTreadMessage message, void* param = NULL ) {
#ifdef RDO_MT
		RDOMessageInfo msg( this, message, param, RDOThread::RDOMessageInfo::send );
		CEvent event;
		msg.send_event = &event;

		to->messages_mutex.Lock();
		to->messages.push_back( msg );
		to->messages_mutex.Unlock();

		while ( ::WaitForSingleObject( event.m_hObject, 0 ) == WAIT_TIMEOUT ) {
			processMessages();
		}
#else
		to->processMessages( RDOMessageInfo( this, message, param ) );
#endif
	}

#ifdef RDO_MT
	// MANUAL: отправка сообщений с 'ручным' ожиданием выполнения для this
	CEvent* manualMessageFrom( RDOTreadMessage message, void* param = NULL );
#endif

	// Рассылка уведомлений всем тредам с учетом их notifies
	// Важно: должна вызываться только для this (в собственной треде)
	void broadcastMessage( RDOTreadMessage message, void* param = NULL, bool lock = false );

protected:
#ifdef RDO_MT
	// Есть два ограничения на использование тред в RDO_MT (с thread-safety всё в порядке, imho):
	// 1. Каждая треда имеет доступ к стеку сообщений (messages) любой другой треды, чтобы положить туда новое сообщение.
	// 2. Каждая треда имеет доступ к списку уведомлений (notifies), чтобы понять, а надо ли посылать сообщение треде.
	// Второе еще можно сделать через дублирование: map< key = thread*, value = notifies > в каджой треде,
	// а вот как добавить сообщение - не совсем понрятно.
	static unsigned int threadFun( void* param );
	const RDOThreadFun thread_fun;
	CWinThread*        thread_win;
	CEvent             proc_create;    // Вызывается из процедуры треды, конструктор должен его дождаться
	CEvent             thread_create;  // Вызывается из конструктора объекта, процедура должна его дождаться
	CEvent*            thread_destroy; // Вызывается из деструктора объекта
	bool               broadcast_waiting; // Без мутекса, т.к. меняется только в одной треде
	bool               was_start;         // Без мутекса, т.к. меняется только в одной треде
	bool               was_close;
#endif
	const std::string  thread_name;
	int                thread_id;
	int                idle_cnt;
	int                broadcast_cnt;

	std::vector< RDOTreadMessage > notifies; // Список сообщений, на которые реагирует треда
	                                         // Если он пуст, то обрабатываются все сообщения.
											 // RT_THREAD_CLOSE обрабатывается в RDOThread::processMessages()
	                                         // Если сообщение посылается не из kernel'а, а напрямую, то
											 // то оно не будет проходить через этот фильтр, а сразу попадет в очередь.
#ifdef RDO_MT
	CMutex                         notifies_mutex;

	// Очередь сообщений
	std::list< RDOMessageInfo > messages;
	CMutex                      messages_mutex;

	class BroadCastData {
	public:
		int cnt;
		std::vector< CEvent* > events;
		HANDLE*                handles;
		BroadCastData(): cnt( 0 ), handles( NULL ) {
		}
		BroadCastData( int _cnt ): cnt( _cnt ) {
			for ( int i = 0; i < cnt; i++ ) {
				events.push_back( new CEvent() );
			}
			handles = new HANDLE[cnt];
		}
		BroadCastData( const BroadCastData& copy ): cnt( copy.cnt ), handles( copy.handles ) {
			events.assign( copy.events.begin(), copy.events.end() );
		}
		~BroadCastData() {
		}
		void clear() {
			for ( int i = 0; i < cnt; i++ ) {
				delete events[i];
			}
			delete[] handles;
		}
		void resize() {
			if ( cnt ) {
				events.resize( cnt * 2 );
				HANDLE* handles_backup = handles;
				handles = new HANDLE[ cnt * 2 ];
				for ( int i = 0; i < cnt; i++ ) {
					events.push_back( new CEvent() );
					handles[i] = handles_backup[i];
					handles[cnt+i] = new HANDLE;
				}
				delete[] handles_backup;
				cnt *= 2;
			} else {
				cnt = 10;
				for ( int i = 0; i < cnt; i++ ) {
					events.push_back( new CEvent() );
				}
				handles = new HANDLE[cnt];
			}
		}
	};
	std::vector< BroadCastData > broadcast_data;
#endif

#ifdef RDO_MT
	virtual RDOThread* getKernel();
#else
	RDOThread* getKernel();
#endif

	// Создавать можно только через потомков
#ifdef RDO_MT
	RDOThread( const std::string& _thread_name, RDOThreadFun _thread_fun = NULL );
#else
	RDOThread( const std::string& _thread_name );
#endif
	// Удаляет объкты функция треды. kernel удаляется через статический метод
	virtual ~RDOThread();                                                                          

	// Надо обязательно вызвать из конструктора объекта, чтобы настроить правильно this для виртуальных функций,
	void after_constructor();
//	// Надо обязательно вызвать из деструктора объекта, чтобы правильно остановить треду в режиме RDO_ST
//	void before_destructor();

	virtual void proc( RDOMessageInfo& msg ) = 0;
	virtual void idle();
	virtual void start();
	virtual void stop();
#ifdef RDO_MT
	virtual bool processMessages();
#else
	void processMessages( RDOMessageInfo& msg ) {
#ifdef TR_TRACE
		RDOThread::trace( "---------------- " + messageToString(msg.message) + ": " + (msg.from ? msg.from->thread_name : "NULL") + " -> " + thread_name );
#endif
		proc( msg );
		if ( msg.message == RT_THREAD_CLOSE ) {
			if ( this != getKernel() ) sendMessage( getKernel(), RT_THREAD_DISCONNECTION );
			stop();
			delete this;
			return;
		}
	}
#endif

#ifdef TR_TRACE
	static void trace( const std::string& str );
#endif
};

// --------------------------------------------------------------------
// ---------- RDOThreadMT
// --------------------------------------------------------------------
class RDOThreadMT: public RDOThread
{
protected:
#ifdef RDO_MT
	RDOThreadMT( const std::string& _thread_name, RDOThreadFun _thread_fun = RDOThread::threadFun ): RDOThread( _thread_name, _thread_fun ) {}
#else
	RDOThreadMT( const std::string& _thread_name ): RDOThread( _thread_name ) {}
#endif
};

// --------------------------------------------------------------------
// ---------- RDOThreadGUI
// --------------------------------------------------------------------
#ifdef RDO_MT
class RDOThreadGUI: public RDOThread
{
private:
	RDOThread* kernel_gui;

protected:
	RDOThreadGUI( const std::string& _thread_name, RDOThread* _kernel_gui ): RDOThread( _thread_name ), kernel_gui( _kernel_gui ) {}
	virtual RDOThread* getKernel();
	virtual bool processMessages();
};
#else
class RDOThreadGUI: public RDOThread
{
protected:
	RDOThreadGUI( const std::string& _thread_name, RDOThread* ): RDOThread( _thread_name ) {}
};
#endif

#endif // RDO_THREAD_H

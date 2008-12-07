#include "rdothread.h"
#include "rdokernel.h"
#include <rdocommon.h>
#include <afxwin.h>
#include <fstream>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef TR_TRACE
#ifdef RDO_MT
std::fstream file( "C:\\rdo_kernel_mt.log", std::ios_base::in | std::ios_base::out | std::ios_base::trunc );
#else
std::fstream file( "C:\\rdo_kernel_st.log", std::ios_base::in | std::ios_base::out | std::ios_base::trunc );
#endif

void RDOThread::trace( const std::string& str )
{
	file << str << std::endl;
	file.flush();
}
#endif // TR_TRACE

// --------------------------------------------------------------------
// ---------- RDOThread
// --------------------------------------------------------------------
#ifdef RDO_MT
RDOThread::RDOThread( const std::string& _thread_name, RDOThreadFun _thread_fun ):
	thread_name( _thread_name ),
	thread_id( 0 ),
	broadcast_cnt( -1 ),
	idle_cnt( 0 ),
	thread_fun( _thread_fun ),
	thread_win( NULL ),
	thread_destroy( new CEvent() ), // thread_destroy удаляется после деструктора объекта
	broadcast_waiting( false ),
	was_start( false ),
	was_close( false )
#else
RDOThread::RDOThread( const std::string& _thread_name ):
	thread_name( _thread_name ),
	thread_id( ::GetCurrentThreadId() ),
	idle_cnt( 0 )
#endif
{
#ifdef TR_TRACE
	trace( thread_name + "::" + thread_name );
#endif
	// Чтобы треда не получала ниодного сообщения. RT_THREAD_CLOSE обрабатывается автоматически
	notifies.push_back( RT_THREAD_CLOSE );
#ifdef RDO_MT
	for ( int i = 0; i < 10; i++ ) {
		broadcast_data.push_back( BroadcastData(10) );
	}
	if ( !isGUI() ) {
		// THREAD_PRIORITY_NORMAL
		// THREAD_PRIORITY_HIGHEST
		// THREAD_PRIORITY_TIME_CRITICAL
		thread_win = AfxBeginThread( thread_fun, this, THREAD_PRIORITY_NORMAL );
	}
#endif
}

RDOThread::~RDOThread()
{
#ifdef TR_TRACE
	trace( rdo::format("%s (%d)::~%s", thread_name.c_str(), thread_id, thread_name.c_str()) );
//	trace( thread_name + "::~" + thread_name );
#endif

#ifdef RDO_MT
	std::vector< BroadcastData >::iterator it = broadcast_data.begin();
	while ( it != broadcast_data.end() ) {
		it->clear();
		it++;
	}
	if ( !isGUI() ) {
		if ( thread_destroy ) {
			thread_destroy->SetEvent();
		}
	} else {
		if ( thread_destroy ) {
			delete thread_destroy;
			thread_destroy = NULL;
		}
	}
#endif
}

RDOThread* RDOThread::getKernel()
{
	return kernel;
}

void RDOThread::after_constructor()
{
	notifies.push_back( RT_THREAD_REGISTERED );
#ifdef RDO_MT
	if ( !isGUI() ) {
		proc_create.Lock();
		thread_create.SetEvent();
	} else {
		if ( getKernel() && getKernel() != this ) sendMessage( getKernel(), RT_THREAD_CONNECTION );
		start();
	}
#else
	if ( getKernel() && getKernel() != this ) sendMessage( getKernel(), RT_THREAD_CONNECTION );
	start();
#endif
}

#ifdef RDO_MT
unsigned int RDOThread::threadFun( void* param )
{
	RDOThread* thread = static_cast<RDOThread*>(param);
	thread->thread_id = ::GetCurrentThreadId();
	thread->proc_create.SetEvent();
	thread->thread_create.Lock();
	if ( kernel && kernel != thread ) thread->sendMessage( kernel, RT_THREAD_CONNECTION );
	thread->start();
	thread->was_start = true;

	while ( thread->processMessages() ) {};

	thread->stop();

	delete thread;

	return 0;
}
#endif

#ifdef RDO_MT
bool RDOThread::processMessages()
{
	messages_mutex.Lock();
	if ( was_close ) {
		messages_mutex.Unlock();
		return false;
	}
	if ( !messages.empty() ) {
		RDOMessageInfo msg = messages.front();
		messages.erase( messages.begin() );
		messages_mutex.Unlock();
#ifdef TR_TRACE
		RDOThread::trace( (msg.type == RDOThread::RDOMessageInfo::post ? "-------(post)--- " : "---------------- ") + messageToString(msg.message) + ": " + (msg.from ? msg.from->thread_name : "NULL") + " -> " + thread_name );
#endif
		proc( msg );
		if ( msg.message == RT_THREAD_CLOSE ) {
			messages_mutex.Lock();
			was_close = true;
			messages_mutex.Unlock();
			if ( this != getKernel() ) sendMessage( getKernel(), RT_THREAD_DISCONNECTION );
			// Отпускаем вызывающие треды
			messages_mutex.Lock();
			if ( msg.type == RDOThread::RDOMessageInfo::send || msg.type == RDOThread::RDOMessageInfo::manual ) {
				msg.send_event->SetEvent();
			}
			std::list< RDOMessageInfo >::iterator it = messages.begin();
			while ( it != messages.end() )
			{
				if ( it->type == RDOThread::RDOMessageInfo::send || it->type == RDOThread::RDOMessageInfo::manual )
				{
					it->send_event->SetEvent();
				}
				it++;
			}
			messages.clear();
			messages_mutex.Unlock();
			return false;
		}
		if ( msg.type == RDOThread::RDOMessageInfo::send || msg.type == RDOThread::RDOMessageInfo::manual ) {
			msg.send_event->SetEvent();
		}
	} else {
		messages_mutex.Unlock();
		idle();
	}
	return true;
}
#endif
/*
void RDOThread::sendMessage( RDOThread* to, RDOTreadMessage message, void* param )
{
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
*/
#ifdef RDO_MT
CEvent* RDOThread::manualMessageFrom( RDOTreadMessage message, void* param )
{
	RDOMessageInfo msg( this, message, param, RDOThread::RDOMessageInfo::manual );
	msg.send_event = new CEvent();

	messages_mutex.Lock();
	messages.push_back( msg );
	messages_mutex.Unlock();

	return msg.send_event;
}
#endif

void RDOThread::broadcastMessage( RDOTreadMessage message, void* param, bool lock )
{
#ifdef RDO_MT
	if ( param ) lock = true;
	broadcast_cnt++;
	if ( broadcast_data.size() < broadcast_cnt + 1 ) {
		broadcast_data.push_back( BroadcastData(10) );
	}
	kernel->threads_mutex.Lock();
	int cnt = 0;
	CMutex* param_lock = NULL;
	std::list< RDOThread* >::iterator it = kernel->threads.begin();
	while ( it != kernel->threads.end() ) {
		RDOThread* thread = *it;
		thread->notifies_mutex.Lock();
		if ( thread != this && (thread->notifies.empty() || std::find( thread->notifies.begin(), thread->notifies.end(), message ) != thread->notifies.end()) ) {
			if ( broadcast_data[broadcast_cnt].cnt < cnt + 1 ) broadcast_data[broadcast_cnt].resize();
			RDOMessageInfo msg( this, message, param, RDOThread::RDOMessageInfo::send );
			if ( lock ) {
				if ( !param_lock ) param_lock = new CMutex();
				msg.param_lock = param_lock;
			}
			msg.send_event = broadcast_data[broadcast_cnt].events[cnt];
			cnt++;
			thread->messages_mutex.Lock();
			thread->messages.push_back( msg );
			thread->messages_mutex.Unlock();
		}
		thread->notifies_mutex.Unlock();
		it++;
	}
	kernel->threads_mutex.Unlock();
	if ( cnt ) {
		for ( int i = 0; i < cnt; i++ ) {
			broadcast_data[broadcast_cnt].handles[i] = broadcast_data[broadcast_cnt].events[i]->m_hObject;
		}
		broadcast_waiting = true;
		while ( ::WaitForMultipleObjects( cnt, broadcast_data[broadcast_cnt].handles, true, 0 ) == WAIT_TIMEOUT ) {
			processMessages();
		}
		broadcast_waiting = false;
	}
	broadcast_cnt--;
	if ( lock && param_lock ) delete param_lock;
/*
//	no cash, no lock
	kernel->threads_mutex.Lock();
	std::vector< CEvent* > events;
	std::list< RDOThread* >::iterator it = kernel->threads.begin();
	while ( it != kernel->threads.end() ) {
		RDOThread* thread = *it;
		thread->notifies_mutex.Lock();
		if ( thread != this && (thread->notifies.empty() || std::find( thread->notifies.begin(), thread->notifies.end(), message ) != thread->notifies.end()) ) {
			RDOMessageInfo msg( this, message, param, RDOThread::RDOMessageInfo::send );
			msg.send_event = new CEvent();
			events.push_back( msg.send_event );
			thread->messages_mutex.Lock();
			thread->messages.push_back( msg );
			thread->messages_mutex.Unlock();
		}
		thread->notifies_mutex.Unlock();
		it++;
	}
	kernel->threads_mutex.Unlock();
	if ( !events.empty() ) {
		int cnt = events.size();
		HANDLE* threads_handles = new HANDLE[cnt];
		int i;
		for ( i = 0; i < cnt; i++ ) {
			threads_handles[i] = events[i]->m_hObject;
		}
		broadcast_waiting = true;
		while ( ::WaitForMultipleObjects( cnt, threads_handles, true, 0 ) == WAIT_TIMEOUT ) {
			processMessages();
		}
		broadcast_waiting = false;
		for ( i = 0; i < cnt; i++ ) {
			delete events[i];
		}
		delete[] threads_handles;
	}
*/
#else
	RDOMessageInfo msg( this, message, param );
	std::list< RDOThread* >::iterator it = kernel->threads.begin();
	while ( it != kernel->threads.end() ) {
		RDOThread* thread = *it;
		if ( thread != this && (thread->notifies.empty() || std::find( thread->notifies.begin(), thread->notifies.end(), message ) != thread->notifies.end()) ) {
			thread->processMessages( msg );
		}
		it++;
	}
#endif
}

void RDOThread::idle()
{
#ifdef RDO_MT
	::Sleep( 0 );
//	if ( idle_cnt++ > 1000 ) {
//		idle_cnt = 0;
//		::Sleep( 1 );
//	}
#endif
}

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

#ifdef RDO_MT
// --------------------------------------------------------------------
// ---------- RDOThreadGUI
// --------------------------------------------------------------------
RDOThread* RDOThreadGUI::getKernel()
{
	return kernel_gui;
}

bool RDOThreadGUI::processMessages()
{
	return kernel_gui->processMessages();
}
#endif

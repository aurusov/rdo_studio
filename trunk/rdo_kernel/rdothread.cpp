#include "rdothread.h"
#include <rdokernel.h>
#include <afxwin.h>
#include <fstream>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef TR_TRACE
std::fstream file( "C:\\rdo_kernel_mt.log", std::ios_base::in | std::ios_base::out | std::ios_base::trunc );

void RDOThread::trace( const std::string& str )
{
	file << str << std::endl;
	file.flush();
}
#endif

// --------------------------------------------------------------------
// ---------- RDOThread
// --------------------------------------------------------------------
#ifdef RDO_MT
RDOThread::RDOThread( const std::string& _thread_name, RDOThreadFun _thread_fun ):
	thread_name( _thread_name ),
	thread_fun( _thread_fun ),
	thread_win( NULL ),
	thread_destroy( new CEvent() ), // thread_destroy удал€етс€ после деструктора объекта
	broadcast_waiting( false ),
	was_start( false ),
	was_close( false )
#else
RDOThread::RDOThread( const std::string& _thread_name ):
	thread_name( _thread_name )
#endif
{
#ifdef TR_TRACE
	trace( thread_name + "::" + thread_name );
#endif
	// „тобы треда не получала ниодного сообщени€. RT_THREAD_CLOSE обрабатываетс€ автоматически
	notifies.push_back( RT_THREAD_CLOSE );
#ifdef RDO_MT
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
	trace( thread_name + "::~" + thread_name );
#endif

#ifdef RDO_MT
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

void RDOThread::after_constructor()
{
	notifies.push_back( RT_THREAD_REGISTERED );
#ifdef RDO_MT
	if ( !isGUI() ) {
		proc_create.Lock();
		thread_create.SetEvent();
	}
#else
	if ( kernel && kernel != this ) sendMessage( kernel, RT_THREAD_CONNECTION, this );
	start();
#endif
}
/*
void RDOThread::before_destructor()
{
#ifdef RDO_ST
	stop();
#endif
}
*/
#ifdef RDO_MT
unsigned int RDOThread::threadFun( void* param )
{
	RDOThread* thread = static_cast<RDOThread*>(param);
	thread->proc_create.SetEvent();
	thread->thread_create.Lock();
	if ( kernel && kernel != thread ) thread->sendMessage( kernel, RT_THREAD_CONNECTION, thread );
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
//		if ( msg.message == RT_THREAD_REGISTERED ) {
//			RDOThread::trace( "R: " + static_cast<RDOThread*>(msg.param)->thread_name );
//		}
//		if ( msg.message == RT_THREAD_REGISTERED && msg.param == this ) {
//			RDOThread::trace( thread_name + ": registered this" );
//		}
		proc( msg );
		if ( msg.type == RDOThread::RDOMessageInfo::send || msg.type == RDOThread::RDOMessageInfo::manual ) {
			msg.send_event->SetEvent();
		}
		if ( msg.message == RT_THREAD_CLOSE ) {
			messages_mutex.Lock();
			was_close = true;
			messages_mutex.Unlock();
			if ( this != kernel ) sendMessage( kernel, RT_THREAD_DISCONNECTION, this );
			return false;
		}
	} else {
		messages_mutex.Unlock();
		idle();
	}
	return true;
}
#else
void RDOThread::processMessages( RDOMessageInfo& msg )
{
#ifdef TR_TRACE
		RDOThread::trace( "---------------- " + messageToString(msg.message) + ": " + (msg.from ? msg.from->thread_name : "NULL") + " -> " + thread_name );
#endif
	proc( msg );
	if ( msg.message == RT_THREAD_CLOSE ) {
		if ( this != kernel ) sendMessage( kernel, RT_THREAD_DISCONNECTION, this );
		stop();
		delete this;
		return;
	}
}
#endif

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

void RDOThread::broadcastMessage( RDOTreadMessage message, void* param )
{
#ifdef RDO_MT
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

#include "rdokernel.h"
#include <rdostudiothread.h>
#include <rdorepositoryfile.h>
#include <rdosimwin.h>

#include <algorithm>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RDOKernel* kernel = NULL;

// --------------------------------------------------------------------
// ---------- RDOKernel
// --------------------------------------------------------------------
RDOKernel::RDOKernel():
	RDOThread( "RDOKernel" ),
	thread_studio( NULL ),
	thread_simulator( NULL ),
	thread_repository( NULL )
{
	notifies.push_back( RT_THREAD_CONNECTION );
	notifies.push_back( RT_THREAD_DISCONNECTION );
	after_constructor();
	kernel = this;
}

RDOKernel::~RDOKernel()
{
	kernel = NULL;
}

void RDOKernel::init()
{
	new RDOKernel();
}

void RDOKernel::close()
{
	CEvent* thread_destroy = kernel->thread_destroy;
	kernel->sendMessage( kernel, RDOThread::RT_THREAD_CLOSE );
	thread_destroy->Lock();
	delete thread_destroy;
}

void RDOKernel::start()
{
	RDOThread::start();
#ifdef TR_TRACE
	trace( "kernel ready !!!" );
#endif
}

void RDOKernel::proc( RDOMessageInfo& msg )
{
	switch ( msg.message ) {
		// Закрыть все треды
		case RT_THREAD_CLOSE: {
#ifdef TR_TRACE
			trace( thread_name + " stop begin" );
#endif
			threads_mutex.Lock();
			std::list< RDOThread* >::iterator it = threads.begin();
			while ( it != threads.end() ) {
				RDOThread* thread = *it;
				// Если thread_fun == NULL, то треда для GUI
				if ( thread->thread_fun ) {
					CEvent* thread_destroy = thread->thread_destroy;
					threads_mutex.Unlock();
					sendMessage( thread, RDOThread::RT_THREAD_CLOSE );
					thread_destroy->Lock();
					threads_mutex.Lock();
					delete thread_destroy;
					it = threads.begin();
				} else {
					it++;
				}
			}
			threads_mutex.Unlock();
#ifdef TR_TRACE
			trace ( thread_name + " stop end" );
#endif
			break;
		}
		case RT_THREAD_CONNECTION: {
			if ( static_cast<RDOThread*>(msg.param) != this ) {
				registration( static_cast<RDOThread*>(msg.param) );
			}
			break;
		}
		case RT_THREAD_DISCONNECTION: {
			if ( static_cast<RDOThread*>(msg.param) != this ) {
				unregistered( static_cast<RDOThread*>(msg.param) );
			}
			break;
		}
		default: break;
	}
}
/*
RDOThread* RDOKernel::find( const std::string& thread_name ) const
{
	threads_mutex.Unlock();
	std::list< RDOThread* >::const_iterator it = threads.begin();
	while ( it != threads.end() ) {
		if ( (*it)->thread_name.compare( thread_name ) == 0 ) {
			RDOThread* thread = *it;
			threads_mutex.Unlock();
			return thread;
		}
		it++;
	}
	threads_mutex.Unlock();
	return NULL;
}

void RDOKernel::sendMessageToThreadByName( const std::string& thread_name, RDOTreadMessage message, void* param )
{
	threads_mutex.Lock();
	RDOThread* thread = find( thread_name );
	if ( thread ) {
		thread->sendMessageFrom( this, message, param );
	}
	threads_mutex.Unlock();
}

void RDOKernel::sendMessageToThreadByNameFrom( RDOThread* from, const std::string& thread_name, RDOTreadMessage message, void* param )
{
	threads_mutex.Lock();
	RDOThread* thread = find( thread_name );
	if ( thread ) {
		thread->sendMessageFrom( from, message, param );
	}
	threads_mutex.Unlock();
}
*/
void RDOKernel::registration( RDOThread* thread )
{
	threads_mutex.Lock();
	if ( thread && std::find( threads.begin(), threads.end(), thread ) == threads.end() ) {
		threads.push_back( thread );
	}
	if ( !thread_studio     && thread->thread_name.compare( "RDOThreadStudio" )     == 0 ) thread_studio     = static_cast<RDOThreadStudio*>(thread);
	if ( !thread_simulator  && thread->thread_name.compare( "RDOThreadSimulator" )  == 0 ) thread_simulator  = static_cast<rdosim::RDOThreadSimulator*>(thread);
	if ( !thread_repository && thread->thread_name.compare( "RDOThreadRepository" ) == 0 ) thread_repository = static_cast<rdoRepository::RDOThreadRepository*>(thread);
	threads_mutex.Unlock();
#ifdef TR_TRACE
	trace( "Kernel INFO: " + thread->thread_name + " REGISTERED" );
#endif
	broadcastMessage( RT_THREAD_REGISTERED, thread );
}

void RDOKernel::unregistered( RDOThread* thread )
{
	threads_mutex.Lock();
	if ( thread && std::find( threads.begin(), threads.end(), thread ) != threads.end() ) {
		threads.remove( thread );
	}
	if ( thread_studio     && thread->thread_name.compare( "RDOThreadStudio" )     == 0 ) thread_studio     = NULL;
	if ( thread_simulator  && thread->thread_name.compare( "RDOThreadSimulator" )  == 0 ) thread_simulator  = NULL;
	if ( thread_repository && thread->thread_name.compare( "RDOThreadRepository" ) == 0 ) thread_repository = NULL;
	threads_mutex.Unlock();
#ifdef TR_TRACE
	trace( "Kernel INFO: " + thread->thread_name + " UNREGISTERED" );
#endif
	broadcastMessage( RT_THREAD_UNREGISTERED, thread );
}

/*
void RDOKernel::method_registration( RDOTreadMethod& msg )
{
	methods_mutex.Lock();
	msg.index = 0;
	bool insert = true;
	std::list< RDOTreadMethod >::iterator it = methods.begin();
	while ( it != methods.end() ) {
		if ( it->thread == msg.thread && it->name == msg.name ) {
			msg.index = it->index;
			insert = false;
			break;
		} else if ( it->index >= msg.index ) {
			msg.index = it->index + 1;
		}
		it++;
	}
	if ( insert ) methods.push_back( msg );
	methods_mutex.Unlock();
#ifdef TR_TRACE
	trace( rdo::format("Kernel INFO: methods: %s = %d", msg.name.c_str(), msg.index) );
#endif
}
*/

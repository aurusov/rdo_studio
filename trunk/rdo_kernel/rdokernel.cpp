#include "rdokernel.h"
#include <rdostudiothread.h>
#include <rdorepository.h>
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
#ifdef RDO_MT
	CEvent* thread_destroy = kernel->thread_destroy;
#endif

	kernel->sendMessage( kernel, RDOThread::RT_THREAD_CLOSE );

#ifdef RDO_MT
	thread_destroy->Lock();
	delete thread_destroy;
#endif
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
#ifdef RDO_MT
			threads_mutex.Lock();
#endif
			std::list< RDOThread* >::iterator it = threads.begin();
			while ( it != threads.end() ) {
				RDOThread* thread = *it;
#ifdef RDO_MT
				if ( !thread->isGUI() ) {
					CEvent* thread_destroy = thread->getDestroyEvent();
					threads_mutex.Unlock();
					sendMessage( thread, RDOThread::RT_THREAD_CLOSE );
					thread_destroy->Lock();
					threads_mutex.Lock();
					delete thread_destroy;
					it = threads.begin();
				} else {
					it++;
				}
#else
				sendMessage( thread, RDOThread::RT_THREAD_CLOSE );
				it = threads.begin();
#endif
			}
#ifdef RDO_MT
			threads_mutex.Unlock();
#endif
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

#ifdef RDO_ST
void RDOKernel::idle()
{
	std::list< RDOThread* >::iterator it = threads.begin();
	while ( it != threads.end() ) {
		// it_next используется из-за того, что в RDOThreadRunTime->idle() м.б. удален RDOThreadRunTime и убран из threads
		std::list< RDOThread* >::iterator it_next = it;
		it_next++;
		(*it)->idle();
		it = it_next;
	}
}
#endif
// -autorun -autoexit "C:\rdo\rdo_cdrom_1\RAO-cd-rom-1\bin\RAO-explorer\Data\Russian\Models\Barber\Source\Barber.smr"

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
#ifdef RDO_MT
	threads_mutex.Lock();
#endif
	if ( thread && std::find( threads.begin(), threads.end(), thread ) == threads.end() ) {
		threads.push_back( thread );
	}
#ifdef RDO_MT
	if ( !thread_studio     && thread->getName() == "RDOThreadStudio"     ) thread_studio     = static_cast<RDOThreadStudio*>(thread);
#else
	if ( !thread_studio     && thread->getName() == "RDOThreadStudioGUI"  ) thread_studio     = static_cast<RDOThreadStudioGUI*>(thread);
#endif
	if ( !thread_simulator  && thread->getName() == "RDOThreadSimulator"  ) thread_simulator  = static_cast<rdosim::RDOThreadSimulator*>(thread);
	if ( !thread_repository && thread->getName() == "RDOThreadRepository" ) thread_repository = static_cast<rdoRepository::RDOThreadRepository*>(thread);
#ifdef RDO_MT
	threads_mutex.Unlock();
#endif

#ifdef TR_TRACE
	trace( "Kernel INFO: " + thread->getName() + " REGISTERED" );
#endif
	broadcastMessage( RT_THREAD_REGISTERED, thread );
}

void RDOKernel::unregistered( RDOThread* thread )
{
#ifdef RDO_MT
	threads_mutex.Lock();
#endif
	if ( thread && std::find( threads.begin(), threads.end(), thread ) != threads.end() ) {
		threads.remove( thread );
	}
#ifdef RDO_MT
	if ( thread_studio     && thread->getName() == "RDOThreadStudio"     ) thread_studio     = NULL;
#else
	if ( thread_studio     && thread->getName() == "RDOThreadStudioGUI"  ) thread_studio     = NULL;
#endif
	if ( thread_simulator  && thread->getName() == "RDOThreadSimulator"  ) thread_simulator  = NULL;
	if ( thread_repository && thread->getName() == "RDOThreadRepository" ) thread_repository = NULL;
#ifdef RDO_MT
	threads_mutex.Unlock();
#endif

#ifdef TR_TRACE
	trace( "Kernel INFO: " + thread->getName() + " UNREGISTERED" );
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

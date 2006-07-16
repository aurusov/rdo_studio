#ifndef RDO_KERNEL_H
#define RDO_KERNEL_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdothread.h"

// --------------------------------------------------------------------
// ---------- RDOKernel
// --------------------------------------------------------------------
#ifdef RDO_MT
class RDOThreadStudio;
#else
class RDOThreadStudioGUI;
#endif

namespace rdosim {
class RDOThreadSimulator;
}

namespace rdoRepository {
class RDOThreadRepository;
}

class RDOKernel: public RDOThread
{
friend class RDOThread;
#ifdef RDO_ST
friend class RDOStudioApp;
#endif
protected:
	RDOKernel();           // —оздание и удаление через статические методы
	virtual ~RDOKernel();

	virtual void proc( RDOMessageInfo& msg );
	virtual void start();
	std::list< RDOThread* >     threads;
	mutable CMutex              threads_mutex;
//	std::list< RDOTreadMethod > methods;
//	CMutex                      methods_mutex;
#ifdef RDO_MT
	RDOThreadStudio*                    thread_studio;
#else
	RDOThreadStudioGUI*                 thread_studio;
#endif
	rdosim::RDOThreadSimulator*         thread_simulator;
	rdoRepository::RDOThreadRepository* thread_repository;

	void registration( RDOThread* thread );
	void unregistered( RDOThread* thread );

#ifdef RDO_ST
	virtual void idle();
#endif

public:
//	RDOThread* find( const std::string& thread_name ) const; // thread-safety
//	void sendMessageToThreadByName( const std::string& thread_name, RDOTreadMessage message, void* param = NULL );                      // thread-safety
//	void sendMessageToThreadByNameFrom( RDOThread* from, const std::string& thread_name, RDOTreadMessage message, void* param = NULL ); // thread-safety

	static void init();
	static void close();

//	void method_registration( RDOTreadMethod& msg ); // thread-safety

#ifdef RDO_MT
	RDOThreadStudio*                    studio() const     { return thread_studio;     }
#else
	RDOThreadStudioGUI*                 studio() const     { return thread_studio;     }
#endif
	rdosim::RDOThreadSimulator*         simulator() const  { return thread_simulator;  }
	rdoRepository::RDOThreadRepository* repository() const { return thread_repository; }
};

// ----------------------------------------------------------------------------
extern RDOKernel* kernel;

#endif // RDO_KERNEL_H

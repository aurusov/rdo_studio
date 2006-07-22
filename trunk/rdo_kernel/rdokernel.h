#ifndef RDO_KERNEL_H
#define RDO_KERNEL_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdothread.h"

// --------------------------------------------------------------------
// ---------- RDOKernel
// --------------------------------------------------------------------
namespace rdosim {
class RDOThreadSimulator;
}

namespace rdoRepository {
class RDOThreadRepository;
}

class RDOKernel: public RDOThreadMT
{
friend void RDOThread::broadcastMessage( RDOTreadMessage message, void* param, bool lock );
#ifdef RDO_ST
friend class RDOStudioApp;
#endif
protected:
	RDOKernel();           // Создание и удаление через статические методы
	virtual ~RDOKernel();

	virtual void proc( RDOMessageInfo& msg );
	virtual void start();

	std::list< RDOThread* >     threads;
#ifdef RDO_MT
	mutable CMutex              threads_mutex;
#endif
//	std::list< RDOTreadMethod > methods;
//	CMutex                      methods_mutex;
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

	rdosim::RDOThreadSimulator*         simulator() const  { return thread_simulator;  }
	rdoRepository::RDOThreadRepository* repository() const { return thread_repository; }
};

#ifdef RDO_MT
// --------------------------------------------------------------------
// ---------- RDOKernelGUI
// --------------------------------------------------------------------
class RDOKernelGUI: public RDOThread
{
//#ifdef RDO_MT
	friend virtual bool RDOThreadGUI::processMessages();
//#endif
protected:
	RDOKernelGUI( const std::string& _thread_name );           // Создание и удаление через потомков
	virtual ~RDOKernelGUI();

	virtual void proc( RDOMessageInfo& msg );
	virtual void idle();

	std::list< RDOThread* > threads;

	void registration( RDOThread* thread );
	void unregistered( RDOThread* thread );
};
#else
typedef RDOKernel RDOKernelGUI;
#endif

// ----------------------------------------------------------------------------
extern RDOKernel* kernel;

#endif // RDO_KERNEL_H

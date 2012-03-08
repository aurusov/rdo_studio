#ifndef RDO_KERNEL_H
#define RDO_KERNEL_H

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOKernel
// --------------------------------------------------------------------------------

#ifdef CORBA_ENABLE

namespace rdoCorba {
class RDOThreadCorba;
}

#endif

namespace rdoSimulator {
class RDOThreadSimulator;
class RDOThreadCodeComp;
}

namespace rdoRuntime {
class RDOThreadRunTime;
}

namespace rdoRepository {
class RDOThreadRepository;
}

class RDOKernel: public RDOThreadMT
{
friend void RDOThread::broadcastMessage( RDOTreadMessage message, void* param, rbool lock );
friend class RDOStudioApp;

protected:
	RDOKernel();           // Создание и удаление через статические методы
	virtual ~RDOKernel();

	virtual void proc( RDOMessageInfo& msg );
	virtual void start();

	typedef std::list<PTR(RDOThread)> RDOThreadList;
	RDOThreadList               threads;
#ifdef RDO_MT
	mutable CMutex              threads_mutex;
#endif
//	std::list< RDOTreadMethod > methods;
//	CMutex                      methods_mutex;
//	void method_registration( RDOTreadMethod& msg ); // thread-safety

	RDOThread*                          thread_studio;
	rdoRuntime::RDOThreadRunTime*       thread_runtime;
	rdoSimulator::RDOThreadSimulator*   thread_simulator;
	rdoSimulator::RDOThreadCodeComp*    thread_codecomp;
	rdoRepository::RDOThreadRepository* thread_repository;
	
	
#ifdef CORBA_ENABLE
	rdoCorba::RDOThreadCorba*           thread_corba;
#endif
	

	void registration( RDOThread* thread );
	void unregistered( RDOThread* thread );

public:
	static void init();
	static void close();

#ifdef RDO_ST
	virtual void idle();
#endif

	RDOThread*                          studio() const     { return thread_studio;     }
	rdoRuntime::RDOThreadRunTime*       runtime() const    { return thread_runtime;    }
	rdoSimulator::RDOThreadSimulator*   simulator() const  { return thread_simulator;  }
	rdoSimulator::RDOThreadCodeComp*    codecomp() const   { return thread_codecomp;   }
	rdoRepository::RDOThreadRepository* repository() const { return thread_repository; }

#ifdef CORBA_ENABLE
	rdoCorba::RDOThreadCorba*           corba() const      { return thread_corba;      }
#endif
};

#ifdef RDO_MT
// --------------------------------------------------------------------------------
// -------------------- RDOKernelGUI
// --------------------------------------------------------------------------------
// Является фиктивной тредой без своей процедуры.
// Раздает сообщения прикрепленным к ней тредам (RDOThreadGUI).
// Можно представить её в виде корня дерева, к которому прикреплены RDOThreadGUI.
// Обработчик сообщений RDOKernelGUI::processMessages() вызывается из цикла обработки
// сообщений самого win32-gui приложения, точнее из OnIdle() приложения, т.е. процедуру
// треды выполняет запущенный екзешник (логически). При этом, именно от имени RDOKernelGUI,
// приложение посылает сообщения остальным настоящим тредам.
//
class RDOKernelGUI: public RDOThread
{
friend virtual rbool RDOThreadGUI::processMessages();

protected:
	RDOKernelGUI( CREF(tstring) _thread_name ); // Создание и удаление через потомков
	virtual ~RDOKernelGUI();

	virtual void proc( RDOMessageInfo& msg );
	virtual void idle();

	std::list< RDOThread* > threads;

	void registration( RDOThread* thread );
	void unregistered( RDOThread* thread );
	void update_notifies();
};
#else
typedef RDOKernel RDOKernelGUI;
#endif

// --------------------------------------------------------------------------------
extern RDOKernel* kernel;

#endif // RDO_KERNEL_H

#ifndef RDO_KERNEL_H
#define RDO_KERNEL_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdothread.h"

// --------------------------------------------------------------------
// ---------- RDOKernel
// --------------------------------------------------------------------
namespace rdoCorba {
class RDOThreadCorba;
}

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
friend void RDOThread::broadcastMessage( RDOTreadMessage message, void* param, bool lock );
friend class RDOStudioApp;

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
//	void method_registration( RDOTreadMethod& msg ); // thread-safety

	RDOThread*                          thread_studio;
	rdoRuntime::RDOThreadRunTime*       thread_runtime;
	rdoSimulator::RDOThreadSimulator*   thread_simulator;
	rdoSimulator::RDOThreadCodeComp*    thread_codecomp;
	rdoRepository::RDOThreadRepository* thread_repository;
	rdoCorba::RDOThreadCorba*           thread_corba;

	void registration( RDOThread* thread );
	void unregistered( RDOThread* thread );

#ifdef RDO_ST
	virtual void idle();
#endif

public:
	static void init();
	static void close();

	RDOThread*                          studio() const     { return thread_studio;     }
	rdoRuntime::RDOThreadRunTime*       runtime() const    { return thread_runtime;    }
	rdoSimulator::RDOThreadSimulator*   simulator() const  { return thread_simulator;  }
	rdoSimulator::RDOThreadCodeComp*    codecomp() const   { return thread_codecomp;   }
	rdoRepository::RDOThreadRepository* repository() const { return thread_repository; }
	rdoCorba::RDOThreadCorba*           corba() const      { return thread_corba;      }
};

#ifdef RDO_MT
// --------------------------------------------------------------------
// ---------- RDOKernelGUI
// --------------------------------------------------------------------
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
friend virtual bool RDOThreadGUI::processMessages();

protected:
	RDOKernelGUI( const std::string& _thread_name ); // Создание и удаление через потомков
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

// ----------------------------------------------------------------------------
extern RDOKernel* kernel;

#endif // RDO_KERNEL_H

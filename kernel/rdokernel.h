#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOKernel
// --------------------------------------------------------------------------------

#ifdef CORBA_ENABLE

namespace rdoCorba {
class RDOThreadCorba;
}

#endif

namespace rdo {
namespace service {
namespace simulation {

class RDOThreadSimulator;
class RDOThreadCodeComp;

}}} // namespace rdo::service::simulation

OPEN_RDO_RUNTIME_NAMESPACE
class RDOThreadRunTime;
CLOSE_RDO_RUNTIME_NAMESPACE

namespace rdo { namespace repository {
class RDOThreadRepository;
}} // namespace rdo::repository

class RDOKernel: public RDOThreadMT
{
friend void RDOThread::broadcastMessage( RDOTreadMessage message, void* param, bool lock );
friend class Application;

protected:
    RDOKernel();           // Создание и удаление через статические методы
    virtual ~RDOKernel();

    virtual void proc( RDOMessageInfo& msg );
    virtual void start();

    typedef std::list<RDOThread*> RDOThreadList;
    RDOThreadList threads;

    RDOThread*                                     thread_studio;
    rdo::runtime::RDOThreadRunTime*                thread_runtime;
    rdo::service::simulation::RDOThreadSimulator*  thread_simulator;
    rdo::service::simulation::RDOThreadCodeComp*   thread_codecomp;
    rdo::repository::RDOThreadRepository*          thread_repository;


#ifdef CORBA_ENABLE
    rdoCorba::RDOThreadCorba*           thread_corba;
#endif


    void registration( RDOThread* thread );
    void unregistered( RDOThread* thread );

public:
    static void init();
    static void close();

    virtual void idle();

    RDOThread*                                     studio    () const { return thread_studio;     }
    rdo::runtime::RDOThreadRunTime*                runtime   () const { return thread_runtime;    }
    rdo::service::simulation::RDOThreadSimulator*  simulator () const { return thread_simulator;  }
    rdo::service::simulation::RDOThreadCodeComp*   codecomp  () const { return thread_codecomp;   }
    rdo::repository::RDOThreadRepository*          repository() const { return thread_repository; }

#ifdef CORBA_ENABLE
    rdoCorba::RDOThreadCorba*                      corba     () const { return thread_corba;      }
#endif
};

typedef RDOKernel RDOKernelGUI;

// --------------------------------------------------------------------------------
extern RDOKernel* kernel;

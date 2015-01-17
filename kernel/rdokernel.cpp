// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <algorithm>
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdokernel.h"
#include "repository/rdorepository.h"
#include "simulator/service/src/simulator.h"
// --------------------------------------------------------------------------------

RDOKernel* kernel = NULL;

// --------------------------------------------------------------------------------
// -------------------- RDOKernel
// --------------------------------------------------------------------------------
RDOKernel::RDOKernel():
    RDOThreadMT( "RDOKernel" ),
    thread_studio( NULL ),
    thread_runtime( NULL ),
    thread_simulator( NULL ),
    thread_codecomp( NULL ),
#ifdef CORBA_ENABLE
    thread_corba( NULL ),
#endif
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
#ifdef COMPILER_VISUAL_STUDIO
    _set_output_format(_TWO_DIGIT_EXPONENT);
#endif

    new RDOKernel();
}

void RDOKernel::close()
{
    kernel->sendMessage( kernel, RDOThread::RT_THREAD_CLOSE );
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
            std::list< RDOThread* >::iterator it = threads.begin();
            while ( it != threads.end() ) {
                RDOThread* thread = *it;
                sendMessage( thread, RDOThread::RT_THREAD_CLOSE );
                it = threads.begin();
            }
#ifdef TR_TRACE
            trace ( thread_name + " stop end" );
#endif
            break;
        }
        case RT_THREAD_CONNECTION: {
            if ( msg.from != this ) {
                registration( msg.from );
            }
            break;
        }
        case RT_THREAD_DISCONNECTION: {
            if ( msg.from != this ) {
                unregistered( msg.from );
            }
            break;
        }
        default: break;
    }
}

void RDOKernel::idle()
{
    std::list< RDOThread* >::iterator it = threads.begin();
    while ( it != threads.end() ) {
        // it_next используется из-за того, что в RDOThreadRunTime->idle() м.б. удален RDOThreadRunTime и убран из threads
        std::list< RDOThread* >::iterator it_next = it;
        ++it_next;
        (*it)->idle();
        it = it_next;
    }
}

// -autorun -autoexit "C:\rdo\rdo_cdrom_1\RAO-cd-rom-1\bin\RAO-explorer\Data\Russian\Models\Barber\Source\Barber.rdox"
// -autorun -autoexit "C:\rdo\rdo_cdrom_1\RAO-cd-rom-1\bin\RAO-explorer\Data\Russian\Models\Heidel\Source\Heidel.rdox"

void RDOKernel::registration( RDOThread* thread )
{
    if (thread && std::find( threads.begin(), threads.end(), thread ) == threads.end())
        threads.push_back( thread );

    if (!thread_runtime && thread->getName() == "RDOThreadRunTime")
        thread_runtime = static_cast<rdo::runtime::RDOThreadRunTime*>(thread);
    if (!thread_simulator && thread->getName() == "RDOThreadSimulator")
        thread_simulator = static_cast<rdo::service::simulation::RDOThreadSimulator*>(thread);
    if (!thread_codecomp && thread->getName() == "RDOThreadCodeComp")
        thread_codecomp = static_cast<rdo::service::simulation::RDOThreadCodeComp*>(thread);
    if (!thread_repository && thread->getName() == "RDOThreadRepository")
        thread_repository = static_cast<rdo::repository::RDOThreadRepository*>(thread);

#ifdef CORBA_ENABLE
    if ( !thread_corba      && thread->getName() == "RDOThreadCorba"      ) thread_corba      = static_cast<rdoCorba::RDOThreadCorba*>(thread);
#endif

#ifdef TR_TRACE
    trace( getName() + " INFO: " + thread->getName() + " REGISTERED" );
#endif
    broadcastMessage( RT_THREAD_REGISTERED, thread );
}

void RDOKernel::unregistered( RDOThread* thread )
{
    if (thread && std::find( threads.begin(), threads.end(), thread ) != threads.end())
        threads.remove( thread );
    else
        return;

    if (thread_runtime && thread->getName() == "RDOThreadRunTime")
        thread_runtime = NULL;
    if (thread_simulator && thread->getName() == "RDOThreadSimulator")
        thread_simulator = NULL;
    if (thread_codecomp && thread->getName() == "RDOThreadCodeComp")
        thread_codecomp = NULL;
    if (thread_repository && thread->getName() == "RDOThreadRepository")
        thread_repository = NULL;

#ifdef CORBA_ENABLE
    if ( thread_corba      && thread->getName() == "RDOThreadCorba"      ) thread_corba      = NULL;
#endif

#ifdef TR_TRACE
    trace( getName() + " INFO: " + thread->getName() + " UNREGISTERED" );
#endif
    broadcastMessage( RT_THREAD_UNREGISTERED, thread );
}

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/application_pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/thread.h"
#include "app/rdo_studio/src/model/model.h"
#include "app/rdo_studio/src/tracer/tracer.h"
// --------------------------------------------------------------------------------

#ifdef RDO_MT
// --------------------------------------------------------------------------------
// -------------------- ThreadStudio
// --------------------------------------------------------------------------------
ThreadStudio::ThreadStudio(): RDOThread( "ThreadStudio", RDOThread::threadFun )
{
    after_constructor();
}

void ThreadStudio::proc( RDOMessageInfo& msg )
{
    switch ( msg.message ) {
        case RT_STUDIO_MODEL_NEW  :
        case RT_STUDIO_MODEL_OPEN :
        case RT_STUDIO_MODEL_SAVE :
        case RT_STUDIO_MODEL_CLOSE:
        case RT_STUDIO_MODEL_BUILD:
        case RT_STUDIO_MODEL_RUN  :
        case RT_STUDIO_MODEL_STOP :
        {
            broadcastMessage( msg.message, msg.param, true );
            break;
        }
    }
}

// --------------------------------------------------------------------------------
// -------------------- ThreadStudioGUI
// --------------------------------------------------------------------------------
ThreadStudioGUI::ThreadStudioGUI(): RDOKernelGUI( "ThreadStudioGUI" )
{
    after_constructor();
}
#endif

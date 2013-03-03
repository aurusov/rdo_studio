/*!
  \copyright (c) RDO-Team, 2006-2012
  \file      app/rdo_studio/src/thread.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      15.07.2006
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
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

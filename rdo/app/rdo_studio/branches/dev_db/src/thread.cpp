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
// -------------------- RDOThreadStudio
// --------------------------------------------------------------------------------
RDOThreadStudio::RDOThreadStudio(): RDOThread( "RDOThreadStudio", RDOThread::threadFun )
{
	after_constructor();
}

void RDOThreadStudio::proc( RDOMessageInfo& msg )
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
// -------------------- RDOThreadStudioGUI
// --------------------------------------------------------------------------------
RDOThreadStudioGUI::RDOThreadStudioGUI(): RDOKernelGUI( "RDOThreadStudioGUI" )
{
	after_constructor();
}
#endif

/*
// --------------------------------------------------------------------------------
// -------------------- RDOThreadStudio1
// --------------------------------------------------------------------------------
RDOThreadStudio1::RDOThreadStudio1(): RDOThreadMT( "RDOThreadStudio1" )
{
	notifies.push_back( RT_RUNTIME_TRACE_STRING );
	after_constructor();
}

void RDOThreadStudio1::proc( RDOMessageInfo& msg )
{
	switch ( msg.message ) {
		case RT_RUNTIME_TRACE_STRING: {
//			::Sleep( 1000 );
			break;
		}
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOThreadStudio2
// --------------------------------------------------------------------------------
RDOThreadStudio2::RDOThreadStudio2(): RDOThreadMT( "RDOThreadStudio2" )
{
	notifies.push_back( RT_RUNTIME_TRACE_STRING );
	after_constructor();
}

void RDOThreadStudio2::proc( RDOMessageInfo& msg )
{
	switch ( msg.message ) {
		case RT_RUNTIME_TRACE_STRING: {
//			::Sleep( 1000 );
			break;
		}
	}
}
*/

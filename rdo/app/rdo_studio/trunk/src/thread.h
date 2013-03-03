/*!
  \copyright (c) RDO-Team, 2006-2012
  \file      app/rdo_studio/src/thread.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      15.07.2006
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_THREAD_H_
#define _RDO_STUDIO_THREAD_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
#include "kernel/rdokernel.h"
// --------------------------------------------------------------------------------

#ifdef RDO_MT
// --------------------------------------------------------------------------------
// -------------------- ThreadStudio
// --------------------------------------------------------------------------------
class ThreadStudio: public RDOThread
{
friend class Application;
protected:
	ThreadStudio();
	virtual ~ThreadStudio() {}; // Чтобы нельзя было удалить через delete
	virtual void proc( RDOMessageInfo& msg );
};

// --------------------------------------------------------------------------------
// -------------------- ThreadStudioGUI
// --------------------------------------------------------------------------------
class ThreadStudioGUI: public RDOKernelGUI
{
friend class Application;
protected:
	ThreadStudioGUI();
	virtual ~ThreadStudioGUI() {}; // Чтобы нельзя было удалить через delete
};
#endif

#endif // _RDO_STUDIO_THREAD_H_

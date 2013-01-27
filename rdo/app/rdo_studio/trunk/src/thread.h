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
// -------------------- RDOThreadStudio
// --------------------------------------------------------------------------------
class RDOThreadStudio: public RDOThread
{
friend class RDOStudioApp;
protected:
	RDOThreadStudio();
	virtual ~RDOThreadStudio() {}; // Чтобы нельзя было удалить через delete
	virtual void proc( RDOMessageInfo& msg );
};

// --------------------------------------------------------------------------------
// -------------------- RDOThreadStudioGUI
// --------------------------------------------------------------------------------
class RDOThreadStudioGUI: public RDOKernelGUI
{
friend class RDOStudioApp;
protected:
	RDOThreadStudioGUI();
	virtual ~RDOThreadStudioGUI() {}; // Чтобы нельзя было удалить через delete
};
#endif

/*
// --------------------------------------------------------------------------------
// -------------------- RDOThreadStudio1
// --------------------------------------------------------------------------------
class RDOThreadStudio1: public RDOThreadMT
{
friend class RDOStudioApp;
protected:
	RDOThreadStudio1();
	virtual ~RDOThreadStudio1() {}; // Чтобы нельзя было удалить через delete
	virtual void proc( RDOMessageInfo& msg );
};

// --------------------------------------------------------------------------------
// -------------------- RDOThreadStudio2
// --------------------------------------------------------------------------------
class RDOThreadStudio2: public RDOThreadMT
{
friend class RDOStudioApp;
protected:
	RDOThreadStudio2();
	virtual ~RDOThreadStudio2() {}; // Чтобы нельзя было удалить через delete
	virtual void proc( RDOMessageInfo& msg );
};
*/

#endif // _RDO_STUDIO_THREAD_H_

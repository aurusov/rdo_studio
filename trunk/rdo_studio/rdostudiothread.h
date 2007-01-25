#ifndef RDO_STUDIO_THREAD_H
#define RDO_STUDIO_THREAD_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <rdothread.h>
#include <rdokernel.h>

#ifdef RDO_MT
// --------------------------------------------------------------------
// ---------- RDOThreadStudio
// --------------------------------------------------------------------
class RDOThreadStudio: public RDOThread
{
friend class RDOStudioApp;
protected:
	RDOThreadStudio();
	virtual ~RDOThreadStudio() {}; // Чтобы нельзя было удалить через delete
	virtual void proc( RDOMessageInfo& msg );
};

// --------------------------------------------------------------------
// ---------- RDOThreadStudioGUI
// --------------------------------------------------------------------
class RDOThreadStudioGUI: public RDOKernelGUI
{
friend class RDOStudioApp;
protected:
	RDOThreadStudioGUI();
	virtual ~RDOThreadStudioGUI() {}; // Чтобы нельзя было удалить через delete
};
#endif

/*
// --------------------------------------------------------------------
// ---------- RDOThreadStudio1
// --------------------------------------------------------------------
class RDOThreadStudio1: public RDOThreadMT
{
friend class RDOStudioApp;
protected:
	RDOThreadStudio1();
	virtual ~RDOThreadStudio1() {}; // Чтобы нельзя было удалить через delete
	virtual void proc( RDOMessageInfo& msg );
};

// --------------------------------------------------------------------
// ---------- RDOThreadStudio2
// --------------------------------------------------------------------
class RDOThreadStudio2: public RDOThreadMT
{
friend class RDOStudioApp;
protected:
	RDOThreadStudio2();
	virtual ~RDOThreadStudio2() {}; // Чтобы нельзя было удалить через delete
	virtual void proc( RDOMessageInfo& msg );
};
*/

#endif // RDO_STUDIO_THREAD_H

/*
 * @copyright (c) RDO-Team, 2011
 * @file      thread_proxy_i.h
 * @authors   Урусов Андрей
 * @date      05.03.2011
 * @brief     
 * @indent    4T
 *********************************************************************************/

#ifndef _THREAD_PROXY_I_H_
#define _THREAD_PROXY_I_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/smart_ptr/interface_ptr.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
// ********************************************************************************

// ********************************************************************************
// ******************** IThreadProxy
// ********************************************************************************
OBJECT_INTERFACE(IThreadProxy)
{
DECLARE_FACTORY(IThreadProxy)
public:
	enum ThreadID
	{
		TID_REPOSITORY
	};
	virtual void sendMessage(ThreadID threadID, ruint messageID, PTR(void) pParam) = 0;

protected:
	IThreadProxy()
	{}
	virtual ~IThreadProxy()
	{}
};

#endif // _THREAD_PROXY_I_H_

/*!
  \copyright (c) RDO-Team, 2011
  \file      thread_proxy_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      05.03.2011
  \brief     Интерфейс IThreadProxy
  \indent    4T
*/

#ifndef _LIB_RUNTIME_THREAD_PROXY_I_H_
#define _LIB_RUNTIME_THREAD_PROXY_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/interface_ptr.h"
#include "utils/smart_ptr/factory.h"
// --------------------------------------------------------------------------------

/*!
  \interface IThreadProxy
  \brief     Интерфейс IThreadProxy
*/
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

#endif // _LIB_RUNTIME_THREAD_PROXY_I_H_

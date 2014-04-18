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
#include "utils/src/smart_ptr/interface_ptr/interface_ptr.h"
#include "utils/src/smart_ptr/factory/factory.h"
// --------------------------------------------------------------------------------

/*!
  \interface IThreadProxy
  \brief     Интерфейс IThreadProxy
*/
PREDECLARE_OBJECT_INTERFACE(IThreadProxy)
struct IThreadProxy: public rdo::RefCounter<IThreadProxy>
{
DECLARE_FACTORY(IThreadProxy)
public:
	enum ThreadID
	{
		TID_REPOSITORY
	};
	virtual void sendMessage(ThreadID threadID, std::size_t messageID, void* pParam) = 0;

protected:
	IThreadProxy()
	{}
	virtual ~IThreadProxy()
	{}
};

#endif // _LIB_RUNTIME_THREAD_PROXY_I_H_

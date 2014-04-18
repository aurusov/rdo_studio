/*!
  \copyright (c) RDO-Team, 2012
  \file      notify.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      08.01.2012
  \brief     
  \indent    4T
*/

#ifndef _LIB_RUNTIME_NOTIFY_H_
#define _LIB_RUNTIME_NOTIFY_H_

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- INCLUDES
#include <map>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/notify_i.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Передача сообщений между объектами
class Notify
{
public:
	//! Типы сообщений
	enum Type
	{
		RO_BEFOREDELETE = 0
	};

	void connect(INotify* pTo, std::size_t message);
	void disconnect(INotify* pTo);
	void disconnect(INotify* pTo, std::size_t message);
	void fireMessage(std::size_t message, void* pParam) const;

private:
	typedef std::multimap<std::size_t, INotify*> Connected;
	Connected m_connected;

	void disconnect(Connected::iterator it, INotify* pTo);
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_NOTIFY_H_

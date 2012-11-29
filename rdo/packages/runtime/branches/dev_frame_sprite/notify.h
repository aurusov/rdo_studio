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

	void connect    (PTR(INotify) pTo, ruint message);
	void disconnect (PTR(INotify) pTo               );
	void disconnect (PTR(INotify) pTo, ruint message);
	void fireMessage(ruint message, PTR(void) pParam) const;

private:
	typedef std::multimap<ruint, PTR(INotify)> Connected;
	Connected m_connected;

	void disconnect(Connected::iterator it, PTR(INotify) pTo);
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_NOTIFY_H_

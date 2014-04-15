/*!
  \copyright (c) RDO-Team, 2012
  \file      notify.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      08.01.2012
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/notify.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- Notify
// --------------------------------------------------------------------------------
void Notify::connect(INotify* pTo, std::size_t message)
{
	Connected::iterator it = m_connected.find(message);
	while (it != m_connected.end())
	{
		if (it->second == pTo)
			break;
		++it;
	}
	if (it == m_connected.end())
	{
		m_connected.insert(Connected::value_type(message, pTo));
	}
}

void Notify::disconnect(Connected::iterator it, INotify* pTo)
{
	while (it != m_connected.end())
	{
		if (it->second == pTo)
		{
			m_connected.erase(it++);
			if (it == m_connected.end())
			{
				break;
			}
		}
		++it;
	}
}

void Notify::disconnect(INotify* pTo)
{
	disconnect(m_connected.begin(), pTo);
}

void Notify::disconnect(INotify* pTo, std::size_t message)
{
	disconnect(m_connected.find(message), pTo);
}

void Notify::fireMessage(std::size_t message, void* pParam) const
{
	Connected::const_iterator it = m_connected.find(message);
	while (it != m_connected.end())
	{
		it->second->notify(message, pParam);
		++it;
	}
}

CLOSE_RDO_RUNTIME_NAMESPACE

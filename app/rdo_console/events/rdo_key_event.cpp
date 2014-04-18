/*!
  \copyright (c) RDO-Team, 2012
  \file      rdo_key_event.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.11.2012
  \brief     Консольная версия RDO
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/events/rdo_key_event.h"
// --------------------------------------------------------------------------------

namespace rdo {

key_event::key_event(const std::string& name, double time, states state, int key_code)
	: event(name, time, event::key)
	, m_state(state)
	, m_key_code(key_code)
{}

key_event::~key_event()
{}

void key_event::setState(states state)
{
	m_state = state;
}

void key_event::setKeyCode(int key_code)
{
	m_key_code = key_code;
}

key_event::states key_event::getState() const
{
	return m_state;
}

int key_event::getKeyCode() const
{
	return m_key_code;
}

} // namespace rdo

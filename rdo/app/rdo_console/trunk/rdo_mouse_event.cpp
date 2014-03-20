/*!
  \copyright (c) RDO-Team, 2012
  \file      app/rdo_console/rdo_mouse_event.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.11.2012
  \brief     Консольная версия RDO
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/rdo_mouse_event.h"
// --------------------------------------------------------------------------------

namespace rdo {

mouse_event::mouse_event(CREF(tstring) name, double time, buttons button, int x, int y)
	: event(name, time, event::mouse)
	, m_button(button)
	, m_x(x)
	, m_y(y)
{}

mouse_event::~mouse_event()
{}

void mouse_event::setButton(buttons button)
{
	m_button = button;
}

void mouse_event::setX(int x)
{
	m_x = x;
}

void mouse_event::sety(int y)
{
	m_y = y;
}

mouse_event::buttons mouse_event::getButton() const
{
	return m_button;
}

int mouse_event::getX() const
{
	return m_x;
}

int mouse_event::getY() const
{
	return m_y;
}

} // namespace rdo

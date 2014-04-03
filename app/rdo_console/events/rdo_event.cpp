/*!
  \copyright (c) RDO-Team, 2012
  \file      rdo_event.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.11.2012
  \brief     Консольная версия RDO
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/events/rdo_event.h"
// --------------------------------------------------------------------------------

namespace rdo {

event::event(CREF(tstring) name, etime time, types type) :
	m_name(name), m_time(time), m_type(type)
{
}

event::~event()
{
}

void event::setName(CREF(tstring) name)
{
	m_name = name;
}

void event::setTime(double time)
{
	m_time =  time;
}

tstring event::getName() const
{
	return m_name;
}

event::etime event::getTime() const
{
	return m_time;
}

event::types event::getType() const
{
	return m_type;
}

} // namespace rdo

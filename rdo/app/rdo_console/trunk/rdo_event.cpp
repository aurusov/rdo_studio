/*!
  \copyright (c) RDO-Team, 2012
  \file      app/rdo_console/rdo_event.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.11.2012
  \brief     Консольная версия RDO
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/rdo_event.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

event::event(CREF(tstring) name, double time, types type) :
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

double event::getTime() const
{
	return m_time;
}

event::types event::getType() const
{
	return m_type;
}

bool operator< (const event& left, const event& right)
{
	return left.m_time < right.m_time;
}

CLOSE_RDO_NAMESPACE
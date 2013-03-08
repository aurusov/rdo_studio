/*!
  \copyright (c) RDO-Team, 2012
  \file      app/rdo_console/rdo_event.h
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.11.2012
  \brief     Консольная версия RDO
  \indent    4T
*/

#ifndef _RDO_EVENT_H_
#define _RDO_EVENT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
// --------------------------------------------------------------------------------

#include <time.h>

OPEN_RDO_NAMESPACE

/// base event class
class event
{
public:
	enum types {
		none,
		key,
		mouse
	};

	typedef double etime;

public:
	event(CREF(tstring) name, etime time, types type = none);
	virtual ~event();

	void setName(CREF(tstring) name);
	void setTime(etime time);

	tstring getName() const;
	etime   getTime() const;
	types   getType() const;

private:
	tstring m_name;
	etime   m_time;
	types   m_type;
};

CLOSE_RDO_NAMESPACE

#endif // _RDO_EVENT_H_

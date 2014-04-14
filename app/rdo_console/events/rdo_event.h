/*!
  \copyright (c) RDO-Team, 2012
  \file      rdo_event.h
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.11.2012
  \brief     Консольная версия RDO
  \indent    4T
*/

#ifndef _RDO_EVENT_H_
#define _RDO_EVENT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

#include <time.h>

namespace rdo {

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
	event(CREF(std::string) name, etime time, types type = none);
	virtual ~event();

	void setName(CREF(std::string) name);
	void setTime(etime time);

	std::string getName() const;
	etime getTime() const;
	types getType() const;

private:
	std::string m_name;
	etime m_time;
	types m_type;
};

} // namespace rdo

#endif // _RDO_EVENT_H_

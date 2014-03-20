/*!
  \copyright (c) RDO-Team, 2012
  \file      app/rdo_console/rdo_key_event.h
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.11.2012
  \brief     Консольная версия RDO
  \indent    4T
*/

#ifndef _RDO_KEY_EVENT_H_
#define _RDO_KEY_EVENT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/rdo_event.h"
// --------------------------------------------------------------------------------

namespace rdo {

/// base event class
class key_event: public event
{
public:
	enum states
	{
		press = 0,
		release
	};

	key_event(CREF(tstring) name, double time, states state, int key_code);
	virtual ~key_event();

	void setState(states state);
	void setKeyCode(int key_code);

	states getState() const;
	int  getKeyCode() const;

private:
	states m_state;
	int m_key_code;
};

} // namespace rdo

#endif // _RDO_KEY_EVENT_H_

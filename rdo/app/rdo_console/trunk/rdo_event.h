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
#include <list>

#include <boost/any.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
#include "utils/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

struct event
{
// types
	enum types {
		any = 0,
		keyboard,
		mouse
	};

	static const tstring any_text;
	static const tstring keyboard_text;
	static const tstring mouse_text;

 // methods
	static types text_type2type(const tstring& text_type);
	static tstring type2text_type(const types& type);

	friend bool operator< (const event& left, const event& right);

// data
	tstring    name;
	types      type;
	double     time;
	boost::any value;
};

typedef std::list<rdo::event> event_list;

CLOSE_RDO_NAMESPACE

#endif // _RDO_EVENT_H_
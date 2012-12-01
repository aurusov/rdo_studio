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
#include "app/rdo_console/rdo_event.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

const tstring event::any_text      = _T("any");
const tstring event::keyboard_text = _T("keyboard");
const tstring event::mouse_text    = _T("mouse");

event::types event::text_type2type(const tstring& text_type)
{
	if(text_type == keyboard_text) {
	return keyboard;
	}
	else if(text_type == mouse_text) {
	return mouse;
	}
	return any;
}

tstring event::type2text_type(const event::types& type)
{
	switch(type) {
	case keyboard:
	return keyboard_text;

	case mouse:
	return mouse_text;

	default:
	return any_text;
	}
}

bool operator< (const event& left, const event& right)
{
	return left.time < right.time;
}

CLOSE_RDO_NAMESPACE
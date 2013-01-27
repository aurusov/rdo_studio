/*!
  \copyright (c) RDO-Team, 2012
  \file      help_context_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.10.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_HELP_CONTEXT_I_H_
#define _RDO_STUDIO_HELP_CONTEXT_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

struct IHelpContext
{
	virtual void onHelpContext() = 0;
};

#define DECLARE_IHelpContext \
	void onHelpContext();

#endif // _RDO_STUDIO_HELP_CONTEXT_I_H_

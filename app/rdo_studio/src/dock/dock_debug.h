/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_debug.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      03.10.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_DOCK_DEBUG_H_
#define _RDO_STUDIO_DOCK_DEBUG_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_focusable.h"
#include "app/rdo_studio/src/editor/debug_edit.h"
// --------------------------------------------------------------------------------

class DockDebug: public DockFocusable
{
public:
	typedef rdo::gui::editor::Debug context_type;

	DockDebug(QWidget* pParent);
	virtual ~DockDebug();

	void appendString(const QString& str);
	void clear();

	context_type& getContext();
};

#endif // _RDO_STUDIO_DOCK_DEBUG_H_

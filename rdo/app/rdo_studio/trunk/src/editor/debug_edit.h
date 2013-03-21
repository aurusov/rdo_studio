/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      debug_edit.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_EDITOR_DEBUG_EDIT_H_
#define _RDO_STUDIO_EDITOR_DEBUG_EDIT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/edit.h"
#include "app/rdo_studio/src/editor/edit_popup_menu.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

class Debug
	: public Edit
	, public PopupMenu
{
public:
	Debug(QWidget* pParent);
	virtual ~Debug();

	void appendLine(CREF(QString) str);

private:
	typedef  Edit  super;

	virtual void mousePressEvent(QMouseEvent* pEvent);
	virtual void onHelpContext();
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDITOR_DEBUG_EDIT_H_

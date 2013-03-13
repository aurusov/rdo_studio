/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      find_edit.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_EDITOR_FIND_EDIT_H_
#define _RDO_STUDIO_EDITOR_FIND_EDIT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/log_edit.h"
#include "app/rdo_studio/src/editor/find_edit_style.h"
#include "app/rdo_studio/src/editor/edit_popup_menu.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

class Find
	: public Log
	, public PopupMenu
{
public:
	Find(PTR(QWidget) pParent);
	virtual ~Find();

	void setEditorStyle(FindStyle* pStyle);

	void setKeyword(CREF(QString) keyword, const rbool matchCase = false) const;

private:
	typedef  Log  super;

	virtual void mousePressEvent(QMouseEvent* pEvent);
	virtual void onHelpContext();
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDITOR_FIND_EDIT_H_

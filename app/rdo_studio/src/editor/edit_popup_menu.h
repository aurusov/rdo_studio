#ifndef _RDO_STUDIO_EDITOR_EDIT_POPUP_MENU_H_
#define _RDO_STUDIO_EDITOR_EDIT_POPUP_MENU_H_

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QWidget>
#include <QMenu>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

class PopupMenu
{
protected:
	PopupMenu(QWidget* pParent);
	virtual ~PopupMenu();

	QMenu* m_pPopupMenu;
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDITOR_EDIT_POPUP_MENU_H_

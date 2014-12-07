#pragma once

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

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
	Find(QWidget* pParent);
	virtual ~Find();

	void setEditorStyle(FindStyle* pStyle);

	void setKeyword(const QString& keyword, const bool matchCase = false) const;

private:
	typedef  Log  super;

	virtual void contextMenuEvent(QContextMenuEvent* pEvent);
	virtual void onHelpContext();
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDITOR_FIND_EDIT_H_

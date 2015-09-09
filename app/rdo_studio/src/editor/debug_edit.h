#pragma once

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

    void appendLine(const QString& str);

private:
    typedef  Edit  super;

    virtual void contextMenuEvent(QContextMenuEvent* pEvent);
    virtual void onHelpContext();
};

}}} // namespace rdo::gui::editor

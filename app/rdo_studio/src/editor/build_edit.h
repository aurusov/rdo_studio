#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "app/rdo_studio/src/editor/log_edit.h"
#include "app/rdo_studio/src/editor/edit_popup_menu.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

class Build
    : public Log
    , public PopupMenu
{
public:
    Build(QWidget* pParent);
    virtual ~Build();

    virtual void showFirstError();

protected:
    virtual void updateEdit(Model* pEdit, const LogEditLineInfo* pLineInfo);

private:
    typedef  Log  super;
    typedef  rdo::simulation::report::FileMessage SyntaxMessage;

    virtual void contextMenuEvent(QContextMenuEvent* pEvent);
    virtual void onHelpContext   ();
};

}}} // namespace rdo::gui::editor

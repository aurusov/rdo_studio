#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/parser_edit.h"
#include "app/rdo_studio/src/editor/results_edit_style.h"
#include "app/rdo_studio/src/editor/edit_popup_menu.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

class Results
    : public Parser
    , public PopupMenu
{
public:
    Results(QWidget* pParent);
    virtual ~Results();

    void setEditorStyle(ResultsStyle* pStyle);

private:
    typedef  Parser  super;

    virtual void contextMenuEvent(QContextMenuEvent* pEvent);
    virtual void onHelpContext();
};

}}} // namespace rdo::gui::editor

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/editor_pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/results_edit.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_window.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::editor;

Results::Results(QWidget* pParent)
    : super(pParent)
    , PopupMenu(pParent)
{
    kw0 = "TRUE FALSE";
    kw1 = "";
    kw2 = "";
    kw3 = "";

    setReadOnly( true );
}

Results::~Results()
{}

void Results::setEditorStyle(ResultsStyle* pStyle)
{
    super::setEditorStyle(pStyle);
}

void Results::onHelpContext()
{
    std::string keyword = getCurrentOrSelectedWord();
    std::string s = getAllKW();

    if (s.find_first_of(keyword) == std::string::npos || keyword.empty())
    {
        keyword = "pmv";
    }

    QByteArray ba;
    ba.append("activateKeyword ");
    ba.append(keyword.c_str());
    ba.append("\n");
    g_pApp->callQtAssistant(ba);
}

void Results::contextMenuEvent(QContextMenuEvent *pEvent)
{
    m_pPopupMenu->exec(pEvent->globalPos());
}

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/editor_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QProcess>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/report/src/build_edit_line_info.h"
#include "app/rdo_studio/src/editor/build_edit.h"
#include "app/rdo_studio/src/editor/model_edit.h"
#include "app/rdo_studio/src/main_window.h"
#include "app/rdo_studio/src/application.h"
// --------------------------------------------------------------------------------

using namespace rdo::simulation::report;
using namespace rdo::gui::editor;

Build::Build(QWidget* pParent)
    : super(pParent)
    , PopupMenu(pParent)
{}

Build::~Build()
{}

void Build::showFirstError()
{
    setCurrentLine(getCurrentLine() + 1);
    LogEditLineInfoList lines;
    getLines(lines);
    std::list<LogEditLineInfo*>::iterator it = lines.begin();
    TRACE3("ln = %d, w = %d, msg = %s\n", (*it)->getLineNumber(), static_cast<BuildEditLineInfo*>(*it)->getType() == SyntaxMessage::Type::MESSAGE_WARNING ? 1 : 0, (*it)->getText().c_str());
    int i;
    for (i = 0; i < getCurrentLine(); i++)
    {
        if (it != lines.end())
        {
            ++it;
            TRACE3("ln = %d, w = %d, msg = %s\n", (*it)->getLineNumber(), static_cast<BuildEditLineInfo*>(*it)->getType() == SyntaxMessage::Type::MESSAGE_WARNING ? 1 : 0, (*it)->getText().c_str());
        }
        else
        {
            setCurrentLine(0);
            break;
        }
    }
    it = lines.begin();
    TRACE3("ln = %d, w = %d, msg = %s\n", (*it)->getLineNumber(), static_cast<BuildEditLineInfo*>(*it)->getType() == SyntaxMessage::Type::MESSAGE_WARNING ? 1 : 0, (*it)->getText().c_str());
    for (i = 0; i < getCurrentLine(); i++)
    {
        ++it;
        TRACE3("ln = %d, w = %d, msg = %s\n", (*it)->getLineNumber(), static_cast<BuildEditLineInfo*>(*it)->getType() == SyntaxMessage::Type::MESSAGE_WARNING ? 1 : 0, (*it)->getText().c_str());
    }
    while (it != lines.end() && ((*it)->getLineNumber() == std::size_t(~0) || static_cast<BuildEditLineInfo*>(*it)->getType() == SyntaxMessage::Type::MESSAGE_WARNING))
    {
        ++it;
        if (it != lines.end())
        {
            TRACE3("ln = %d, w = %d, msg = %s\n", (*it)->getLineNumber(), static_cast<BuildEditLineInfo*>(*it)->getType() == SyntaxMessage::Type::MESSAGE_WARNING ? 1 : 0, (*it)->getText().c_str());
        }
        setCurrentLine(getCurrentLine() + 1);
    }
    if (it == lines.end())
    {
        it = lines.begin();
        TRACE3("ln = %d, w = %d, msg = %s\n", (*it)->getLineNumber(), static_cast<BuildEditLineInfo*>(*it)->getType() == SyntaxMessage::Type::MESSAGE_WARNING ? 1 : 0, (*it)->getText().c_str());
        setCurrentLine(0);
        while (it != lines.end() && ((*it)->getLineNumber() == std::size_t(~0) || static_cast<BuildEditLineInfo*>(*it)->getType() == SyntaxMessage::Type::MESSAGE_WARNING))
        {
            ++it;
            if (it != lines.end())
            {
                TRACE3("ln = %d, w = %d, msg = %s\n", (*it)->getLineNumber(), static_cast<BuildEditLineInfo*>(*it)->getType() == SyntaxMessage::Type::MESSAGE_WARNING ? 1 : 0, (*it)->getText().c_str());
            }
            setCurrentLine(getCurrentLine() + 1);
        }
    }
    if (it != lines.end())
    {
        TRACE3("ln = %d, w = %d, msg = %s\n", (*it)->getLineNumber(), static_cast<BuildEditLineInfo*>(*it)->getType() == SyntaxMessage::Type::MESSAGE_WARNING ? 1 : 0, (*it)->getText().c_str());
    }
    if (it != lines.end() && (*it)->getLineNumber() != std::size_t(~0) && static_cast<BuildEditLineInfo*>(*it)->getType() == SyntaxMessage::Type::MESSAGE_ERROR)
    {
        setSelectLine(getCurrentLine(), *it, true);
    }
}

void Build::updateEdit(Model* pEdit, const LogEditLineInfo* pLineInfo)
{
    super::updateEdit(pEdit, pLineInfo);
    pEdit->setErrorLine(pLineInfo->getLineNumber());
}

void Build::onHelpContext()
{
    QByteArray ba;
    ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_run.htm#output_build\n");
    g_pApp->callQtAssistant(ba);
}

void Build::contextMenuEvent(QContextMenuEvent* pEvent)
{
    m_pPopupMenu->exec(pEvent->globalPos());
}

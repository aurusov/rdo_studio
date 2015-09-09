// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/editor_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/algorithm/string.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/report/src/error_code.h"
#include "app/rdo_studio/src/editor/log_edit.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/model/model.h"
#include "app/rdo_studio/src/model/model_tab_ctrl.h"
#include "thirdparty/scintilla/include/Scintilla.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::editor;

Log::Log(QWidget* pParent)
    : Edit(pParent)
    , m_currentLine(-1)
{
    setCurrentLine(-1);
    m_sciMarkerLine = getNewMarker();

    sendEditor(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT);

    setReadOnly(true);

    QObject::connect(this, SIGNAL(doubleClick(int, int)), this, SLOT(catchDoubleClick(int, int)));
    QObject::connect(this, SIGNAL(modified(int, int, int, int, const QByteArray&, int, int, int)), this, SLOT(catchModified()));
}

void Log::catchDoubleClick(int position, int line)
{
    m_currentLine = line;

    LogEditLineInfoList::iterator it = m_lines.begin();
    for (int i = 0; i < line; i++)
    {
        if (it != m_lines.end())
        {
            ++it;
        }
    }
    if (it != m_lines.end() && (*it)->getLineNumber() != std::size_t(~0))
    {
        setSelectLine(line, *it);
    }

    sendEditor(SCI_SETSELECTIONSTART, position);
    sendEditor(SCI_SETSELECTIONEND  , position);
}

void Log::catchModified()
{
    if (hasSelectLine())
    {
        clearSelectLine();
    }
}

Log::~Log()
{
    clearLines();
}

void Log::setEditorStyle(LogStyle* pStyle)
{
    Edit::setEditorStyle(pStyle);
    if (!m_pStyle)
        return;

    // ----------
    // Selected Line
    defineMarker(m_sciMarkerLine, SC_MARK_BACKGROUND, QColor(0xFF, 0xFF, 0xFF), static_cast<LogStyle*>(m_pStyle)->selectLineBgColor);
}

void Log::gotoPrev()
{
    m_currentLine--;
    if (m_currentLine < 0)
    {
        m_currentLine = m_lines.size() - 1;
    }
    if (m_currentLine < 0)
        return;

    LogEditLineInfoList::iterator it = m_lines.begin();
    int i;
    for (i = 0; i < m_currentLine; i++)
    {
        ++it;
    }
    while (it != m_lines.begin() && (*it)->getLineNumber() == std::size_t(~0))
    {
        --it;
        --m_currentLine;
    }
    if (it == m_lines.begin() && (*it)->getLineNumber() == std::size_t(~0))
    {
        it = m_lines.end();
        m_currentLine = m_lines.size();
        while (it == m_lines.end() || (it != m_lines.begin() && (*it)->getLineNumber() == std::size_t(~0)))
        {
            --it;
            --m_currentLine;
        }
    }
    if (it != m_lines.end() && (*it)->getLineNumber() != std::size_t(~0))
    {
        setSelectLine(m_currentLine, *it, true);
    }
}

void Log::getLines(LogEditLineInfoList& lines) const
{
    lines = m_lines;
}

int Log::getCurrentLine() const
{
    return m_currentLine;
}

int Log::getSciMarkerLine() const
{
    return m_sciMarkerLine;
}

void Log::setCurrentLine(int currentLine)
{
    m_currentLine = currentLine;
}

void Log::setSciMarkerLine(int sciMarkerLine)
{
    m_sciMarkerLine = sciMarkerLine;
}

void Log::gotoNext()
{
    m_currentLine++;
    LogEditLineInfoList::iterator it = m_lines.begin();
    int i;
    for (i = 0; i < m_currentLine; i++)
    {
        if (it != m_lines.end())
        {
            ++it;
        }
        else
        {
            m_currentLine = 0;
            break;
        }
    }
    it = m_lines.begin();
    for (i = 0; i < m_currentLine; i++)
    {
        ++it;
    }
    while (it != m_lines.end() && (*it)->getLineNumber() == std::size_t(~0))
    {
        ++it;
        ++m_currentLine;
    }
    if (it == m_lines.end())
    {
        it = m_lines.begin();
        m_currentLine = 0;
        while (it != m_lines.end() && (*it)->getLineNumber() == std::size_t(~0))
        {
            ++it;
            ++m_currentLine;
        }
    }
    if (it != m_lines.end() && (*it)->getLineNumber() != std::size_t(~0))
    {
        setSelectLine(m_currentLine, *it, true);
    }
}

void Log::clearAll()
{
    Edit::clearAll();
    clearLines();
}

void Log::appendLine(LogEditLineInfo* pLine)
{
    m_lines.push_back(pLine);
    bool readOnly = isReadOnly();
    if (readOnly)
    {
        setReadOnly(false);
    }
    std::string str = boost::algorithm::trim_right_copy(pLine->getMessage());
    str += "\r\n";
    setCurrentPos(getLength());
    appendText(QString::fromStdString(str));
    pLine->setPosInLog(getLength());
    scrollToLine2(getLineCount());
    setCurrentPos(pLine->getPosInLog());
    if (readOnly)
    {
        setReadOnly(true);
    }
    onUpdateEditGUI();
}

void Log::setSelectLine(int line, const LogEditLineInfo* pLineInfo, bool useScroll)
{
    if (pLineInfo->getLineNumber() != std::size_t(~0))
    {
        if (sendEditor(SCI_MARKERNEXT, 0, 1 << m_sciMarkerLine) != line)
        {
            clearSelectLine();
            sendEditor(SCI_MARKERADD, line, m_sciMarkerLine);
            if (useScroll)
            {
                setCurrentPos(pLineInfo->getPosInLog());
                scrollToCarret();
            }
        }
        model::TabCtrl* pTab = g_pModel->getTab();
        if (pTab)
        {
            if (pTab->getCurrentRDOItem() != pLineInfo->getFileType())
            {
                Model* pEdit = pTab->getCurrentEdit();
                if (!pEdit || (pEdit && pEdit->getLog() == this))
                {
                    pTab->setCurrentRDOItem(pLineInfo->getFileType());
                }
            }
            Model* pEdit = pTab->getCurrentEdit();
            if (pEdit && pEdit->getLog() == this)
            {
                updateEdit(pEdit, pLineInfo);
            }
        }
    }
}

void Log::updateEdit(Model* pEdit, const LogEditLineInfo* pLineInfo)
{
    pEdit->scrollToLine(pLineInfo->getLineNumber());
    int pos = pEdit->getPositionFromLine(pLineInfo->getLineNumber()) + pLineInfo->getPosInLine();
    pEdit->setCurrentPos(pos);
    pEdit->horzScrollToCurrentPos();
    pEdit->setFocus();
}

void Log::clearSelectLine()
{
    int nextLine = sendEditor(SCI_MARKERNEXT, 0, 1 << m_sciMarkerLine);
    if (nextLine >= 0)
    {
        sendEditor(SCI_MARKERDELETE, nextLine, m_sciMarkerLine);
        update();
    }
}

bool Log::hasSelectLine() const
{
    int nextLine = sendEditor(SCI_MARKERNEXT, 0, 1 << m_sciMarkerLine);
    return nextLine >= 0;
}

void Log::clearLines()
{
    for (LogEditLineInfo* pInfo: m_lines)
    {
        delete pInfo;
    }
    m_lines.clear();
    m_currentLine = 0;
}

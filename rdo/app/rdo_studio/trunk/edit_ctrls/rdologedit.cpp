/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdologedit.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/report/error_code.h"
#include "app/rdo_studio/edit_ctrls/rdologedit.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/model/model.h"
#include "app/rdo_studio/rdo_edit/rdoeditortabctrl.h"
#include "thirdparty/scintilla/include/Scintilla.h"
// --------------------------------------------------------------------------------

using namespace rdoEditCtrl;

// --------------------------------------------------------------------------------
// -------------------- LogEdit
// ---------------------------------------------------------------------------
LogEdit::LogEdit(PTR(QWidget) pParent)
	: RDOBaseEdit  (pParent)
	, m_currentLine(-1)
{
	setCurrentLine(-1);
	m_sciMarkerLine = getNewMarker();

	sendEditor(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT);

	setReadOnly(true);

	QObject::connect(this, SIGNAL(doubleClick(int, int)), this, SLOT(catchDoubleClick(int, int)));
	QObject::connect(this, SIGNAL(modified(int, int, int, int, const QByteArray&, int, int, int)), this, SLOT(catchModified()));
}

void LogEdit::catchDoubleClick(int position, int line)
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
	if (it != m_lines.end() && (*it)->getLineNumber() != -1)
	{
		setSelectLine(line, *it);
	}

	sendEditor( SCI_SETSELECTIONSTART, position);
	sendEditor( SCI_SETSELECTIONEND  , position);
}

void LogEdit::catchModified()
{
	if (hasSelectLine())
	{
		clearSelectLine();
	}
}

LogEdit::~LogEdit()
{
	clearLines();
}

void LogEdit::setEditorStyle(PTR(LogEditStyle) pStyle)
{
	RDOBaseEdit::setEditorStyle(pStyle);
	if (!m_pStyle)
		return;

	// ----------
	// Selected Line
	defineMarker(m_sciMarkerLine, SC_MARK_BACKGROUND, QColor(0xFF, 0xFF, 0xFF), static_cast<PTR(LogEditTheme)>(m_pStyle->theme)->selectLineBgColor);
}

void LogEdit::gotoPrev()
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
	while (it != m_lines.begin() && (*it)->getLineNumber() == -1)
	{
		--it;
		--m_currentLine;
	}
	if (it == m_lines.begin() && (*it)->getLineNumber() == -1)
	{
		it = m_lines.end();
		m_currentLine = m_lines.size();
		while (it == m_lines.end() || (it != m_lines.begin() && (*it)->getLineNumber() == -1))
		{
			--it;
			--m_currentLine;
		}
	}
	if (it != m_lines.end() && (*it)->getLineNumber() != -1)
	{
		setSelectLine(m_currentLine, *it, true);
	}
}

void LogEdit::getLines(REF(LogEditLineInfoList) lines) const
{
	lines = m_lines;
}

rsint LogEdit::getCurrentLine() const
{
	return m_currentLine;
}

rsint LogEdit::getSciMarkerLine() const
{
	return m_sciMarkerLine;
}

void LogEdit::setCurrentLine(rsint currentLine)
{
	m_currentLine = currentLine;
}

void LogEdit::setSciMarkerLine(rsint sciMarkerLine)
{
	m_sciMarkerLine = sciMarkerLine;
}

void LogEdit::gotoNext()
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
	while (it != m_lines.end() && (*it)->getLineNumber() == -1)
	{
		++it;
		++m_currentLine;
	}
	if (it == m_lines.end())
	{
		it = m_lines.begin();
		m_currentLine = 0;
		while (it != m_lines.end() && (*it)->getLineNumber() == -1)
		{
			++it;
			++m_currentLine;
		}
	}
	if (it != m_lines.end() && (*it)->getLineNumber() != -1)
	{
		setSelectLine(m_currentLine, *it, true);
	}
}

void LogEdit::clearAll()
{
	RDOBaseEdit::clearAll();
	clearLines();
}

void LogEdit::appendLine(PTR(LogEditLineInfo) pLine)
{
	m_lines.push_back(pLine);
	rbool readOnly = isReadOnly();
	if (readOnly)
	{
		setReadOnly(false);
	}
	tstring str = boost::algorithm::trim_right_copy(pLine->getMessage());
	str += "\r\n";
	setCurrentPos(getLength());
	appendText(QString::fromLocal8Bit(str.c_str()));
	pLine->setPosInLog(getLength());
	scrollToLine2(getLineCount());
	setCurrentPos(pLine->getPosInLog());
	if (readOnly)
	{
		setReadOnly(true);
	}
	onUpdateEditGUI();
}

void LogEdit::setSelectLine(int line, CPTR(LogEditLineInfo) pLineInfo, rbool useScroll)
{
	if (pLineInfo->getLineNumber() != -1)
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
		PTR(rdoEditor::RDOEditorTabCtrl) pTab = g_pModel->getTab();
		if (pTab)
		{
			if (pTab->getCurrentRDOItem() != pLineInfo->getFileType())
			{
				PTR(rdoEditor::RDOEditorEdit) pEdit = pTab->getCurrentEdit();
				if (!pEdit || (pEdit && pEdit->getLog() == this))
				{
					pTab->setCurrentRDOItem(pLineInfo->getFileType());
				}
			}
			PTR(rdoEditor::RDOEditorEdit) pEdit = pTab->getCurrentEdit();
			if (pEdit && pEdit->getLog() == this)
			{
				updateEdit(pEdit, pLineInfo);
			}
		}
	}
}

void LogEdit::updateEdit(PTR(rdoEditor::RDOEditorEdit) pEdit, CPTR(LogEditLineInfo) pLineInfo)
{
	pEdit->scrollToLine(pLineInfo->getLineNumber());
	int pos = pEdit->getPositionFromLine(pLineInfo->getLineNumber()) + pLineInfo->getPosInLine();
	pEdit->setCurrentPos(pos);
	pEdit->horzScrollToCurrentPos();
	pEdit->setFocus();
}

void LogEdit::clearSelectLine()
{
	int nextLine = sendEditor(SCI_MARKERNEXT, 0, 1 << m_sciMarkerLine);
	if (nextLine >= 0)
	{
		sendEditor(SCI_MARKERDELETE, nextLine, m_sciMarkerLine);
		update();
	}
}

rbool LogEdit::hasSelectLine() const
{
	int nextLine = sendEditor(SCI_MARKERNEXT, 0, 1 << m_sciMarkerLine);
	return nextLine >= 0;
}

void LogEdit::clearLines()
{
	BOOST_FOREACH(LogEditLineInfo* pInfo, m_lines)
	{
		delete pInfo;
	}
	m_lines.clear();
	m_currentLine = 0;
}

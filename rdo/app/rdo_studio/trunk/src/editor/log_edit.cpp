/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      log_edit.cpp
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
#include "app/rdo_studio/src/editor/log_edit.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/model/model.h"
#include "app/rdo_studio/src/model/model_tab_ctrl.h"
#include "thirdparty/scintilla/include/Scintilla.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::editor;

Log::Log(PTR(QWidget) pParent)
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
	if (it != m_lines.end() && (*it)->getLineNumber() != -1)
	{
		setSelectLine(line, *it);
	}

	sendEditor( SCI_SETSELECTIONSTART, position);
	sendEditor( SCI_SETSELECTIONEND  , position);
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

void Log::setEditorStyle(PTR(LogEditStyle) pStyle)
{
	Edit::setEditorStyle(pStyle);
	if (!m_pStyle)
		return;

	// ----------
	// Selected Line
	defineMarker(m_sciMarkerLine, SC_MARK_BACKGROUND, QColor(0xFF, 0xFF, 0xFF), static_cast<PTR(LogEditTheme)>(m_pStyle->theme)->selectLineBgColor);
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

void Log::getLines(REF(LogEditLineInfoList) lines) const
{
	lines = m_lines;
}

rsint Log::getCurrentLine() const
{
	return m_currentLine;
}

rsint Log::getSciMarkerLine() const
{
	return m_sciMarkerLine;
}

void Log::setCurrentLine(rsint currentLine)
{
	m_currentLine = currentLine;
}

void Log::setSciMarkerLine(rsint sciMarkerLine)
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

void Log::clearAll()
{
	Edit::clearAll();
	clearLines();
}

void Log::appendLine(PTR(LogEditLineInfo) pLine)
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

void Log::setSelectLine(int line, CPTR(LogEditLineInfo) pLineInfo, rbool useScroll)
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
		PTR(ModelTabCtrl) pTab = g_pModel->getTab();
		if (pTab)
		{
			if (pTab->getCurrentRDOItem() != pLineInfo->getFileType())
			{
				PTR(Model) pEdit = pTab->getCurrentEdit();
				if (!pEdit || (pEdit && pEdit->getLog() == this))
				{
					pTab->setCurrentRDOItem(pLineInfo->getFileType());
				}
			}
			PTR(Model) pEdit = pTab->getCurrentEdit();
			if (pEdit && pEdit->getLog() == this)
			{
				updateEdit(pEdit, pLineInfo);
			}
		}
	}
}

void Log::updateEdit(PTR(Model) pEdit, CPTR(LogEditLineInfo) pLineInfo)
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

rbool Log::hasSelectLine() const
{
	int nextLine = sendEditor(SCI_MARKERNEXT, 0, 1 << m_sciMarkerLine);
	return nextLine >= 0;
}

void Log::clearLines()
{
	BOOST_FOREACH(LogEditLineInfo* pInfo, m_lines)
	{
		delete pInfo;
	}
	m_lines.clear();
	m_currentLine = 0;
}

/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdologedit.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/report//error_code.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdologedit.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditortabctrl.h"
#include "app/rdo_studio_mfc/resource.h"
#include "thirdparty/sci/include/Scintilla.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#	define new DEBUG_NEW
#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditCtrl;

// --------------------------------------------------------------------------------
// -------------------- RDOLogEdit
// ---------------------------------------------------------------------------

//! @todo qt
//BEGIN_MESSAGE_MAP( RDOLogEdit, RDOBaseEdit )
//	ON_COMMAND(ID_BUILDFINDLOG_GOTO_NEXT, OnGotoNext)
//	ON_COMMAND(ID_BUILDFINDLOG_GOTO_PREV, OnGotoPrev)
//END_MESSAGE_MAP()

RDOLogEdit::RDOLogEdit(PTR(QWidget) pParent)
	: RDOBaseEdit  (pParent)
	, m_currentLine(-1)
{
	setCurrentLine(-1);
	m_sciMarkerLine = getNewMarker();

	sendEditor(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT);

	setReadOnly(true);

	sendEditorString(SCI_SETPROPERTY, reinterpret_cast<unsigned long>("withoutselectbyclick"), "1");

	QObject::connect(this, SIGNAL(doubleClick(int, int)), this, SLOT(catchDoubleClick(int, int)));
	QObject::connect(this, SIGNAL(modified()), this, SLOT(catchModified()));
}

void RDOLogEdit::catchDoubleClick(int position, int line)
{
	UNUSED(position);
	UNUSED(line    );

	setSelectLine();
}

void RDOLogEdit::catchModified()
{
	if (hasSelectLine())
	{
		clearSelectLine();
	}
}

RDOLogEdit::~RDOLogEdit()
{
	clearLines();
}

void RDOLogEdit::setEditorStyle(PTR(RDOLogEditStyle) pStyle)
{
	RDOBaseEdit::setEditorStyle(pStyle);
	if (!style) 
	{
		return;
	}

	// ----------
	// Selected Line
	defineMarker(m_sciMarkerLine, SC_MARK_BACKGROUND, RGB(0xFF, 0xFF, 0xFF), static_cast<PTR(RDOLogEditTheme)>(style->theme)->selectLineBgColor);
}

void RDOLogEdit::gotoPrev()
{
	m_currentLine--;
	if (m_currentLine < 0)
	{
		m_currentLine = m_lines.size() - 1;
	}
	if (m_currentLine < 0)
		return;

	RDOLogEditLineInfoList::iterator it = m_lines.begin();
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

void RDOLogEdit::getLines(REF(RDOLogEditLineInfoList) lines) const
{
	lines = m_lines;
}

rsint RDOLogEdit::getCurrentLine() const
{
	return m_currentLine;
}

rsint RDOLogEdit::getSciMarkerLine() const
{
	return m_sciMarkerLine;
}

void RDOLogEdit::setCurrentLine(rsint currentLine)
{
	m_currentLine = currentLine;
}

void RDOLogEdit::setSciMarkerLine(rsint sciMarkerLine)
{
	m_sciMarkerLine = sciMarkerLine;
}

void RDOLogEdit::setSelectLine()
{
	QPoint point = QCursor::pos();
	QWidget::mapFromGlobal(point);
	int pos  = sendEditor(SCI_POSITIONFROMPOINT, point.x(), point.y());
	int line = getLineFromPosition(pos);
	setCurrentPos(pos);
	m_currentLine = line;

	RDOLogEditLineInfoList::iterator it = m_lines.begin();
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
}

void RDOLogEdit::gotoNext()
{
	m_currentLine++;
	RDOLogEditLineInfoList::iterator it = m_lines.begin();
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

void RDOLogEdit::clearAll()
{
	RDOBaseEdit::clearAll();
	clearLines();
}

void RDOLogEdit::appendLine(PTR(RDOLogEditLineInfo) pLine)
{
	m_lines.push_back(pLine);
	rbool readOnly = isReadOnly();
	if (readOnly)
	{
		setReadOnly(false);
	}
	tstring str = pLine->getMessage();
	rdo::trimRight(str);
	str += "\r\n";
	setCurrentPos(getLength());
	appendText(str);
	pLine->setPosInLog(getLength());
	scrollToLine2(getLineCount());
	setCurrentPos(pLine->getPosInLog());
	if (readOnly)
	{
		setReadOnly(true);
	}
	updateEditGUI();
}

void RDOLogEdit::OnGotoNext()
{
	gotoNext();
}

void RDOLogEdit::OnGotoPrev()
{
	gotoPrev();
}

void RDOLogEdit::setSelectLine(int line, CPTR(RDOLogEditLineInfo) pLineInfo, rbool useScroll)
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
		PTR(rdoEditor::RDOEditorTabCtrl) pTab = model->getTab();
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

void RDOLogEdit::updateEdit(PTR(rdoEditor::RDOEditorEdit) pEdit, CPTR(RDOLogEditLineInfo) pLineInfo)
{
	pEdit->scrollToLine(pLineInfo->getLineNumber());
	int pos = pEdit->getPositionFromLine(pLineInfo->getLineNumber()) + pLineInfo->getPosInLine();
	pEdit->setCurrentPos(pos);
	pEdit->horzScrollToCurrentPos();
	pEdit->setFocus();
}

void RDOLogEdit::clearSelectLine()
{
	int nextLine = sendEditor(SCI_MARKERNEXT, 0, 1 << m_sciMarkerLine);
	if (nextLine >= 0)
	{
		sendEditor(SCI_MARKERDELETE, nextLine, m_sciMarkerLine);
		update();
	}
}

rbool RDOLogEdit::hasSelectLine() const
{
	int nextLine = sendEditor(SCI_MARKERNEXT, 0, 1 << m_sciMarkerLine);
	return nextLine >= 0;
}

void RDOLogEdit::clearLines()
{
	RDOLogEditLineInfoList::iterator it = m_lines.begin();
	while (it != m_lines.end())
	{
		delete *it;
		++it;
	}
	m_lines.clear();
	m_currentLine = 0;
}

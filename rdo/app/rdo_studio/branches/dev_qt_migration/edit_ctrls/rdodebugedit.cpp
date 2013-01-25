/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdodebugedit.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtCore/qprocess.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/edit_ctrls/rdodebugedit.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/resource.h"
#include "thirdparty/sci/include/SciLexer.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditCtrl;

// --------------------------------------------------------------------------------
// -------------------- RDODebugEdit
// ---------------------------------------------------------------------------
RDODebugEdit::RDODebugEdit(PTR(QWidget) pParent)
	: RDOBaseEdit(pParent)
{
	setReadOnly(true);

	Ui::MainWindow* pMainWindow = studioApp.getMainWndUI();
	ASSERT(pMainWindow);
	m_pPopupMenu = new QMenu(this);
	m_pPopupMenu->addAction(pMainWindow->actEditCopy);
	m_pPopupMenu->addAction(pMainWindow->actEditSelectAll);
	m_pPopupMenu->addSeparator();
	m_pPopupMenu->addAction(pMainWindow->actSearchFind);
	m_pPopupMenu->addAction(pMainWindow->actSearchFindNext);
	m_pPopupMenu->addAction(pMainWindow->actSearchFindPrevious);
	m_pPopupMenu->addSeparator();
	m_pPopupMenu->addAction(pMainWindow->actSearchBookmarksToggle);
	m_pPopupMenu->addAction(pMainWindow->actSearchBookmarkNext);
	m_pPopupMenu->addAction(pMainWindow->actSearchBookmarkPrev);
	m_pPopupMenu->addAction(pMainWindow->actSearchBookmarksClearAll);
}

RDODebugEdit::~RDODebugEdit()
{}

void RDODebugEdit::appendLine(CREF(tstring) str)
{
	rbool readOnly = isReadOnly();
	setReadOnly(false);
	rbool scroll = isLineVisible( getLineCount() - 1 );
	setCurrentPos(getLength());
	appendText(str);
	if (scroll)
	{
		int line = getLineCount();
		int line_to_scroll = line > 0 ? line - 1 : 0;
		scrollToLine (line_to_scroll);
		setCurrentPos(getLength());
	}
	setReadOnly(readOnly);
	onUpdateEditGUI();
}

void RDODebugEdit::onHelpContext()
{
	QByteArray ba;
	ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_run.htm#output_debug\n");
	studioApp.callQtAssistant(ba);
}

void RDODebugEdit::mousePressEvent(QMouseEvent* pEvent)
{
	if (pEvent->button() == Qt::LeftButton)
	{
		RDOBaseEdit::mousePressEvent(pEvent);
	}
	else if (pEvent->button() == Qt::RightButton)
	{
		m_pPopupMenu->exec(pEvent->globalPos());
	}
}
/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdodebugedit.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtCore/qprocess.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/edit_ctrls/rdodebugedit.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_frm.h"
#include "thirdparty/scintilla/include/SciLexer.h"
// --------------------------------------------------------------------------------

using namespace rdoEditCtrl;

// --------------------------------------------------------------------------------
// -------------------- RDODebugEdit
// ---------------------------------------------------------------------------
RDODebugEdit::RDODebugEdit(PTR(QWidget) pParent)
	: super(pParent)
	, EditWithReadOnlyPopupMenu(pParent)
{
	setReadOnly(true);
}

RDODebugEdit::~RDODebugEdit()
{}

void RDODebugEdit::appendLine(CREF(QString) str)
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
	g_pApp->callQtAssistant(ba);
}

void RDODebugEdit::mousePressEvent(QMouseEvent* pEvent)
{
	if (pEvent->button() == Qt::LeftButton)
	{
		super::mousePressEvent(pEvent);
	}
	else if (pEvent->button() == Qt::RightButton)
	{
		m_pPopupMenu->exec(pEvent->globalPos());
	}
}

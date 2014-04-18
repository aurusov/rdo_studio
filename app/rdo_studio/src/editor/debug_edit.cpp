/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      debug_edit.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/editor_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QProcess>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/debug_edit.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_window.h"
#include "thirdparty/scintilla/include/SciLexer.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::editor;

Debug::Debug(QWidget* pParent)
	: super(pParent)
	, PopupMenu(pParent)
{
	setReadOnly(true);
}

Debug::~Debug()
{}

void Debug::appendLine(const QString& str)
{
	bool readOnly = isReadOnly();
	setReadOnly(false);
	bool scroll = isLineVisible( getLineCount() - 1 );
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

void Debug::onHelpContext()
{
	QByteArray ba;
	ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_run.htm#output_debug\n");
	g_pApp->callQtAssistant(ba);
}

void Debug::contextMenuEvent(QContextMenuEvent* pEvent)
{
	m_pPopupMenu->exec(pEvent->globalPos());
}
/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdobuildedit.cpp
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
#include "simulator/report/build_edit_line_info.h"
#include "app/rdo_studio/edit_ctrls/rdobuildedit.h"
#include "app/rdo_studio/rdo_edit/rdoeditoredit.h"
#include "app/rdo_studio/src/main_frm.h"
#include "app/rdo_studio/src/application.h"
// --------------------------------------------------------------------------------

using namespace rdo::simulation::report;
using namespace rdoEditCtrl;

// --------------------------------------------------------------------------------
// -------------------- RDOBuildEdit
// ---------------------------------------------------------------------------
RDOBuildEdit::RDOBuildEdit(PTR(QWidget) pParent)
	: super(pParent)
	, EditWithReadOnlyPopupMenu(pParent)
{}

RDOBuildEdit::~RDOBuildEdit()
{}

void RDOBuildEdit::showFirstError()
{
	setCurrentLine(getCurrentLine() + 1);
	LogEditLineInfoList lines;
	getLines(lines);
	std::list<LogEditLineInfo*>::iterator it = lines.begin();
	TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<BuildEditLineInfo*>(*it)->getType() == RDOSyntaxMessage::MT_WARNING ? 1 : 0, (*it)->getText().c_str());
	int i;
	for (i = 0; i < getCurrentLine(); i++)
	{
		if (it != lines.end())
		{
			++it;
			TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<BuildEditLineInfo*>(*it)->getType() == RDOSyntaxMessage::MT_WARNING ? 1 : 0, (*it)->getText().c_str());
		}
		else
		{
			setCurrentLine(0);
			break;
		}
	}
	it = lines.begin();
	TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<BuildEditLineInfo*>(*it)->getType() == RDOSyntaxMessage::MT_WARNING ? 1 : 0, (*it)->getText().c_str());
	for (i = 0; i < getCurrentLine(); i++)
	{
		++it;
		TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<BuildEditLineInfo*>(*it)->getType() == RDOSyntaxMessage::MT_WARNING ? 1 : 0, (*it)->getText().c_str());
	}
	while (it != lines.end() && ((*it)->getLineNumber() == -1 || static_cast<BuildEditLineInfo*>(*it)->getType() == RDOSyntaxMessage::MT_WARNING))
	{
		++it;
		if (it != lines.end())
		{
			TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<BuildEditLineInfo*>(*it)->getType() == RDOSyntaxMessage::MT_WARNING ? 1 : 0, (*it)->getText().c_str());
		}
		setCurrentLine(getCurrentLine() + 1);
	}
	if (it == lines.end())
	{
		it = lines.begin();
		TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<BuildEditLineInfo*>(*it)->getType() == RDOSyntaxMessage::MT_WARNING ? 1 : 0, (*it)->getText().c_str());
		setCurrentLine(0);
		while (it != lines.end() && ((*it)->getLineNumber() == -1 || static_cast<BuildEditLineInfo*>(*it)->getType() == RDOSyntaxMessage::MT_WARNING))
		{
			++it;
			if (it != lines.end())
			{
				TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<BuildEditLineInfo*>(*it)->getType() == RDOSyntaxMessage::MT_WARNING ? 1 : 0, (*it)->getText().c_str());
			}
			setCurrentLine(getCurrentLine() + 1);
		}
	}
	if (it != lines.end())
	{
		TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<BuildEditLineInfo*>(*it)->getType() == RDOSyntaxMessage::MT_WARNING ? 1 : 0, (*it)->getText().c_str());
	}
	if (it != lines.end() && (*it)->getLineNumber() != -1 && static_cast<BuildEditLineInfo*>(*it)->getType() == RDOSyntaxMessage::MT_ERROR)
	{
		setSelectLine(getCurrentLine(), *it, true);
	}
}

void RDOBuildEdit::updateEdit(rdoEditor::RDOEditorEdit* pEdit, const LogEditLineInfo* pLineInfo)
{
	super::updateEdit(pEdit, pLineInfo);
	pEdit->setErrorLine(pLineInfo->getLineNumber());
}

void RDOBuildEdit::onHelpContext()
{
	QByteArray ba;
	ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_run.htm#output_build\n");
	studioApp.callQtAssistant(ba);
}

void RDOBuildEdit::mousePressEvent(QMouseEvent*  pEvent)
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

/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdobuildedit.cpp
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
#include "simulator/report/rdo_build_edit_line_info.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdobuildedit.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditoredit.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/htmlhelp.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#	define new DEBUG_NEW
#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

using namespace rdo::simulation::report;

namespace rdoEditCtrl {

// --------------------------------------------------------------------------------
// -------------------- RDOBuildEdit
// ---------------------------------------------------------------------------

// ON_UPDATE_COMMAND_UI сделано

BEGIN_MESSAGE_MAP( RDOBuildEdit, RDOLogEdit )
	ON_COMMAND(ID_HELP_KEYWORD, OnHelpKeyword)
	ON_UPDATE_COMMAND_UI( ID_COORD_STATUSBAR , OnUpdateCoordStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODIFY_STATUSBAR, OnUpdateModifyStatusBar )
END_MESSAGE_MAP()

RDOBuildEdit::RDOBuildEdit()
	: RDOLogEdit()
{}

RDOBuildEdit::~RDOBuildEdit()
{}

void RDOBuildEdit::showFirstError()
{
	setCurrentLine(getCurrentLine() + 1);
	RDOLogEditLineInfoList lines;
	getLines(lines);
	std::list< RDOLogEditLineInfo* >::iterator it = lines.begin();
	TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<RDOBuildEditLineInfo*>(*it)->getMessageType() == RDOSyntaxMessage::MT_WARNING ? 1 : 0, (*it)->getText().c_str() );
	int i;
	for ( i = 0; i < getCurrentLine(); i++ ) {
		if ( it != lines.end() ) {
			it++;
			TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<RDOBuildEditLineInfo*>(*it)->getMessageType() == RDOSyntaxMessage::MT_WARNING ? 1 : 0, (*it)->getText().c_str() );
		}
		else
		{
			setCurrentLine(0);
			break;
		}
	}
	it = lines.begin();
	TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<RDOBuildEditLineInfo*>(*it)->getMessageType() == RDOSyntaxMessage::MT_WARNING ? 1 : 0, (*it)->getText().c_str() );
	for ( i = 0; i < getCurrentLine(); i++ ) {
		it++;
		TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<RDOBuildEditLineInfo*>(*it)->getMessageType() == RDOSyntaxMessage::MT_WARNING ? 1 : 0, (*it)->getText().c_str() );
	}
	while ( it != lines.end() && ((*it)->getLineNumber() == -1 || static_cast<RDOBuildEditLineInfo*>(*it)->getMessageType() == RDOSyntaxMessage::MT_WARNING) ) {
		it++;
		if ( it != lines.end() ) {
			TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<RDOBuildEditLineInfo*>(*it)->getMessageType() == RDOSyntaxMessage::MT_WARNING ? 1 : 0, (*it)->getText().c_str() );
		}
		setCurrentLine(getCurrentLine() + 1);
	}
	if ( it == lines.end() ) {
		it = lines.begin();
		TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<RDOBuildEditLineInfo*>(*it)->getMessageType() == RDOSyntaxMessage::MT_WARNING ? 1 : 0, (*it)->getText().c_str() );
		setCurrentLine(0);
		while ( it != lines.end() && ((*it)->getLineNumber() == -1 || static_cast<RDOBuildEditLineInfo*>(*it)->getMessageType() == RDOSyntaxMessage::MT_WARNING) ) {
			it++;
			if ( it != lines.end() ) {
				TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<RDOBuildEditLineInfo*>(*it)->getMessageType() == RDOSyntaxMessage::MT_WARNING ? 1 : 0, (*it)->getText().c_str() );
			}
			setCurrentLine(getCurrentLine() + 1);
		}
	}
	if ( it != lines.end() ) {
		TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<RDOBuildEditLineInfo*>(*it)->getMessageType() == RDOSyntaxMessage::MT_WARNING ? 1 : 0, (*it)->getText().c_str() );
	}
	if ( it != lines.end() && (*it)->getLineNumber() != -1 && static_cast<RDOBuildEditLineInfo*>(*it)->getMessageType() == RDOSyntaxMessage::MT_ERROR ) {
		setSelectLine( getCurrentLine(), *it, true );
	}
}

void RDOBuildEdit::updateEdit( rdoEditor::RDOEditorEdit* edit, const RDOLogEditLineInfo* lineInfo )
{
	RDOLogEdit::updateEdit( edit, lineInfo );
	edit->setErrorLine( lineInfo->getLineNumber() );
}

void RDOBuildEdit::OnUpdateCoordStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( rdo::format( "%d: %d", getCurrentColumnNumber() + 1, getCurrentLineNumber() + 1 ).c_str() );
}

void RDOBuildEdit::OnUpdateModifyStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( rdo::format( ID_STATUSBAR_READONLY ).c_str() );
}

void RDOBuildEdit::OnHelpKeyword()
{
	QByteArray ba;
	ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_run.htm#output_build\n");
	studioApp.callQtAssistant(ba);
}

}; // namespace rdoEditCtrl
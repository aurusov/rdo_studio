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

namespace rdoEditCtrl {

// --------------------------------------------------------------------------------
// -------------------- RDOBuildEditLineInfo
// --------------------------------------------------------------------------------
RDOBuildEditLineInfo::RDOBuildEditLineInfo(CREF(RDOSyntaxMessage) message)
	: RDOLogEditLineInfo(message)
{}

RDOBuildEditLineInfo::RDOBuildEditLineInfo(CREF(tstring) message)
	: RDOLogEditLineInfo(message)
{}

RDOBuildEditLineInfo::~RDOBuildEditLineInfo()
{}

tstring RDOBuildEditLineInfo::getMessage() const
{
	tstring file;
	switch ( getFileType() ) {
		case rdoModelObjects::RTP : file = "RTP" ; break;
		case rdoModelObjects::RSS : file = "RSS" ; break;
		case rdoModelObjects::EVN : file = "EVN" ; break;
		case rdoModelObjects::PAT : file = "PAT" ; break;
		case rdoModelObjects::DPT : file = "DPT" ; break;
		case rdoModelObjects::PRC : file = "PRC" ; break;
		case rdoModelObjects::PRCX: file = "PRCX"; break;
		case rdoModelObjects::FRM : file = "FRM" ; break;
		case rdoModelObjects::FUN : file = "FUN" ; break;
		case rdoModelObjects::SMR : file = "SMR" ; break;
		case rdoModelObjects::PMD : file = "PMD" ; break;
		default: file = "";
	}
	if ( isSimpleTextMessage() )
	{
		return getText();
	}
	else
	{
		tstring s_error = rdo::format(getMessageType() == RDOSyntaxMessage::MESSAGE_WARNING ? IDS_WARNING : IDS_ERROR);
		return rdo::format( "%s (%d): %s: %s", file.c_str(), getLineNumber() + 1, s_error.c_str(), getText().c_str() );
	}
}

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
	TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<RDOBuildEditLineInfo*>(*it)->getMessageType() == RDOSyntaxMessage::MESSAGE_WARNING ? 1 : 0, (*it)->getText().c_str() );
	int i;
	for ( i = 0; i < getCurrentLine(); i++ ) {
		if ( it != lines.end() ) {
			it++;
			TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<RDOBuildEditLineInfo*>(*it)->getMessageType() == RDOSyntaxMessage::MESSAGE_WARNING ? 1 : 0, (*it)->getText().c_str() );
		}
		else
		{
			setCurrentLine(0);
			break;
		}
	}
	it = lines.begin();
	TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<RDOBuildEditLineInfo*>(*it)->getMessageType() == RDOSyntaxMessage::MESSAGE_WARNING ? 1 : 0, (*it)->getText().c_str() );
	for ( i = 0; i < getCurrentLine(); i++ ) {
		it++;
		TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<RDOBuildEditLineInfo*>(*it)->getMessageType() == RDOSyntaxMessage::MESSAGE_WARNING ? 1 : 0, (*it)->getText().c_str() );
	}
	while ( it != lines.end() && ((*it)->getLineNumber() == -1 || static_cast<RDOBuildEditLineInfo*>(*it)->getMessageType() == RDOSyntaxMessage::MESSAGE_WARNING) ) {
		it++;
		if ( it != lines.end() ) {
			TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<RDOBuildEditLineInfo*>(*it)->getMessageType() == RDOSyntaxMessage::MESSAGE_WARNING ? 1 : 0, (*it)->getText().c_str() );
		}
		setCurrentLine(getCurrentLine() + 1);
	}
	if ( it == lines.end() ) {
		it = lines.begin();
		TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<RDOBuildEditLineInfo*>(*it)->getMessageType() == RDOSyntaxMessage::MESSAGE_WARNING ? 1 : 0, (*it)->getText().c_str() );
		setCurrentLine(0);
		while ( it != lines.end() && ((*it)->getLineNumber() == -1 || static_cast<RDOBuildEditLineInfo*>(*it)->getMessageType() == RDOSyntaxMessage::MESSAGE_WARNING) ) {
			it++;
			if ( it != lines.end() ) {
				TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<RDOBuildEditLineInfo*>(*it)->getMessageType() == RDOSyntaxMessage::MESSAGE_WARNING ? 1 : 0, (*it)->getText().c_str() );
			}
			setCurrentLine(getCurrentLine() + 1);
		}
	}
	if ( it != lines.end() ) {
		TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->getLineNumber(), static_cast<RDOBuildEditLineInfo*>(*it)->getMessageType() == RDOSyntaxMessage::MESSAGE_WARNING ? 1 : 0, (*it)->getText().c_str() );
	}
	if ( it != lines.end() && (*it)->getLineNumber() != -1 && static_cast<RDOBuildEditLineInfo*>(*it)->getMessageType() == RDOSyntaxMessage::MESSAGE_ERROR ) {
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
	QProcess* assistant = studioApp.chkQtAssistantWindow();
	if ( assistant->state() != assistant->Running ) return;
	QByteArray ba;
	ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_run.htm#output_build\n");
	assistant->write(ba);
}

}; // namespace rdoEditCtrl
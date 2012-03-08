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
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/edit_ctrls/rdobuildedit.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditoredit.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/htmlhelp.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditCtrl;

// --------------------------------------------------------------------------------
// -------------------- RDOBuildEditLineInfo
// --------------------------------------------------------------------------------
RDOBuildEditLineInfo::RDOBuildEditLineInfo( rdoSimulator::RDOSyntaxError::ErrorCode _error_code, CREF(tstring) _message, const rdoModelObjects::RDOFileType _fileType, const int _lineNumber, const int _posInLine, bool _warning ):
	RDOLogEditLineInfo( _message, _fileType, _lineNumber, _posInLine ),
	error_code( _error_code ),
	warning( _warning )
{
}

RDOBuildEditLineInfo::RDOBuildEditLineInfo( CREF(tstring) _message ):
	RDOLogEditLineInfo( _message ),
	error_code( rdoSimulator::RDOSyntaxError::UNKNOWN ),
	warning( false )
{
}

RDOBuildEditLineInfo::~RDOBuildEditLineInfo()
{
}

tstring RDOBuildEditLineInfo::getMessage() const
{
	tstring file;
	switch ( fileType ) {
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
	if ( lineNumber < 0 || file.empty() ) {
		return message;
	} else {
		tstring s_error = warning ? rdo::format( IDS_WARNING, error_code ) : rdo::format( IDS_ERROR, error_code );
		return rdo::format( "%s (%d): %s: %s", file.c_str(), lineNumber + 1, s_error.c_str(), message.c_str() );
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

RDOBuildEdit::RDOBuildEdit(): RDOLogEdit()
{
}

RDOBuildEdit::~RDOBuildEdit()
{
}

void RDOBuildEdit::showFirstError()
{
	current_line++;
	std::list< RDOLogEditLineInfo* >::iterator it = lines.begin();
	TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->lineNumber, static_cast<RDOBuildEditLineInfo*>(*it)->warning ? 1 : 0, (*it)->message.c_str() );
	int i;
	for ( i = 0; i < current_line; i++ ) {
		if ( it != lines.end() ) {
			it++;
			TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->lineNumber, static_cast<RDOBuildEditLineInfo*>(*it)->warning ? 1 : 0, (*it)->message.c_str() );
		} else {
			current_line = 0;
			break;
		}
	}
	it = lines.begin();
	TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->lineNumber, static_cast<RDOBuildEditLineInfo*>(*it)->warning ? 1 : 0, (*it)->message.c_str() );
	for ( i = 0; i < current_line; i++ ) {
		it++;
		TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->lineNumber, static_cast<RDOBuildEditLineInfo*>(*it)->warning ? 1 : 0, (*it)->message.c_str() );
	}
	while ( it != lines.end() && ((*it)->lineNumber == -1 || static_cast<RDOBuildEditLineInfo*>(*it)->warning) ) {
		it++;
		if ( it != lines.end() ) {
			TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->lineNumber, static_cast<RDOBuildEditLineInfo*>(*it)->warning ? 1 : 0, (*it)->message.c_str() );
		}
		current_line++;
	}
	if ( it == lines.end() ) {
		it = lines.begin();
		TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->lineNumber, static_cast<RDOBuildEditLineInfo*>(*it)->warning ? 1 : 0, (*it)->message.c_str() );
		current_line = 0;
		while ( it != lines.end() && ((*it)->lineNumber == -1 || static_cast<RDOBuildEditLineInfo*>(*it)->warning) ) {
			it++;
			if ( it != lines.end() ) {
				TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->lineNumber, static_cast<RDOBuildEditLineInfo*>(*it)->warning ? 1 : 0, (*it)->message.c_str() );
			}
			current_line++;
		}
	}
	if ( it != lines.end() ) {
		TRACE3(_T("ln = %d, w = %d, msg = %s\n"), (*it)->lineNumber, static_cast<RDOBuildEditLineInfo*>(*it)->warning ? 1 : 0, (*it)->message.c_str() );
	}
	if ( it != lines.end() && (*it)->lineNumber != -1 && !static_cast<RDOBuildEditLineInfo*>(*it)->warning ) {
		setSelectLine( current_line, *it, true );
	}
}

void RDOBuildEdit::updateEdit( rdoEditor::RDOEditorEdit* edit, const RDOLogEditLineInfo* lineInfo )
{
	RDOLogEdit::updateEdit( edit, lineInfo );
	edit->setErrorLine( lineInfo->lineNumber );
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
	tstring filename = studioApp.getFullHelpFileName();
	if ( filename.empty() ) return;
	filename += "::/html/work_run.htm#output_build";
	::HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_DISPLAY_TOPIC, NULL );
}

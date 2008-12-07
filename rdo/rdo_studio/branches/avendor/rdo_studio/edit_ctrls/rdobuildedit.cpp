#include "stdafx.h"
#include "rdobuildedit.h"
#include "../rdo_edit/rdoeditoredit.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditCtrl;

// ----------------------------------------------------------------------------
// ---------- RDOBuildEditLineInfo
// ----------------------------------------------------------------------------
RDOBuildEditLineInfo::RDOBuildEditLineInfo( const string& _message, const rdoModelObjects::RDOFileType _fileType, const int _lineNumber, bool _error ):
	RDOLogEditLineInfo( _message, _fileType, _lineNumber ),
	error( _error )
{
}

RDOBuildEditLineInfo::~RDOBuildEditLineInfo()
{
}

string RDOBuildEditLineInfo::getMessage() const
{
	string file;
	switch ( fileType ) {
		case rdoModelObjects::PAT: file = "PAT"; break;
		case rdoModelObjects::RTP: file = "RTP"; break;
		case rdoModelObjects::RSS: file = "RSS"; break;
		case rdoModelObjects::OPR: file = "OPR"; break;
		case rdoModelObjects::FRM: file = "FRM"; break;
		case rdoModelObjects::FUN: file = "FUN"; break;
		case rdoModelObjects::DPT: file = "DPT"; break;
		case rdoModelObjects::SMR: file = "SMR"; break;
		case rdoModelObjects::PMD: file = "PMD"; break;
		default: file = "";
	}
	if ( lineNumber < 0 || file.empty() ) {
		return message;
	} else {
		string s = error ? "error" : "warning";
		return format( "%s (%d): %s: %s", file.c_str(), lineNumber + 1, s.c_str(), message.c_str() );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOBuildEdit
// ---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOBuildEdit, RDOLogEdit )
	//{{AFX_MSG_MAP(RDOBuildEdit)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI( ID_COORD_STATUSBAR , OnUpdateCoordStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODIFY_STATUSBAR, OnUpdateModifyStatusBar )
END_MESSAGE_MAP()

RDOBuildEdit::RDOBuildEdit(): RDOLogEdit()
{
}

RDOBuildEdit::~RDOBuildEdit()
{
}

void RDOBuildEdit::updateEdit( rdoEditor::RDOEditorEdit* edit, const RDOLogEditLineInfo* lineInfo )
{
	RDOLogEdit::updateEdit( edit, lineInfo );
	edit->setErrorLine( lineInfo->lineNumber );
}

void RDOBuildEdit::OnUpdateCoordStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( format( "%d: %d", getCurrentColumnNumber() + 1, getCurrentLineNumber() + 1 ).c_str() );
}

void RDOBuildEdit::OnUpdateModifyStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( format( ID_STATUSBAR_READONLY ).c_str() );
}

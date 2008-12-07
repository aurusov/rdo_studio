#include "stdafx.h"
#include "rdodebugedit.h"
#include "sci/SciLexer.h"
#include "../rdostudioapp.h"
#include "../resource.h"
#include "../htmlhelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditCtrl;

// ----------------------------------------------------------------------------
// ---------- RDODebugEdit
// ---------------------------------------------------------------------------

// ON_UPDATE_COMMAND_UI сделано

BEGIN_MESSAGE_MAP( RDODebugEdit, RDOBaseEdit )
	//{{AFX_MSG_MAP(RDODebugEdit)
	ON_WM_CREATE()
	ON_COMMAND(ID_HELP_KEYWORD, OnHelpKeyword)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI( ID_COORD_STATUSBAR , OnUpdateCoordStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODIFY_STATUSBAR, OnUpdateModifyStatusBar )
END_MESSAGE_MAP()

RDODebugEdit::RDODebugEdit(): RDOBaseEdit()
{
}

RDODebugEdit::~RDODebugEdit()
{
}

int RDODebugEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( RDOBaseEdit::OnCreate(lpCreateStruct) == -1 ) return -1;

	setReadOnly( true );

	return 0;
}

void RDODebugEdit::appendLine( const std::string& str )
{
	bool readOnly = isReadOnly();
	setReadOnly( false );
	bool scroll = isLineVisible( getLineCount() - 1 );
	setCurrentPos( getLength() );
	appendText( str );
	if ( scroll ) {
		int line = getLineCount();
		int line_to_scroll = line > 0 ? line - 1 : 0;
		scrollToLine( line_to_scroll );
		setCurrentPos( getLength() );
	}
	setReadOnly( readOnly );
	updateEditGUI();
}

void RDODebugEdit::OnUpdateCoordStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( rdo::format( "%d: %d", getCurrentColumnNumber() + 1, getCurrentLineNumber() + 1 ).c_str() );
}

void RDODebugEdit::OnUpdateModifyStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( rdo::format( ID_STATUSBAR_READONLY ).c_str() );
}

void RDODebugEdit::OnHelpKeyword()
{
	std::string filename = studioApp.getFullHelpFileName();
	if ( filename.empty() ) return;
	filename += "::/html/work_run.htm#output_debug";
	::HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_DISPLAY_TOPIC, NULL );
}

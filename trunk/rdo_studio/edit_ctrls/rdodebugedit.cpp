#include "stdafx.h"
#include "rdodebugedit.h"
#include "sci/SciLexer.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
using namespace rdoEditCtrl;

// ----------------------------------------------------------------------------
// ---------- RDODebugEdit
// ---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDODebugEdit, RDOBaseEdit )
	//{{AFX_MSG_MAP(RDODebugEdit)
	ON_WM_CREATE()
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

void RDODebugEdit::appendLine( const string& str )
{
	bool readOnly = isReadOnly();
	setReadOnly( false );
	bool scroll = isLineVisible( getLineCount() - 1 );
	appendText( str );
	if ( scroll ) {
		int line = getLineCount();
		int line_to_scroll = line > 0 ? line - 1 : 0;
		scrollToLine( line_to_scroll );
		setCurrentPos( getLength() );
	}
	setReadOnly( readOnly );
}

void RDODebugEdit::OnUpdateCoordStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( format( "%d: %d", getCurrentColumnNumber() + 1, getCurrentLineNumber() + 1 ).c_str() );
}

void RDODebugEdit::OnUpdateModifyStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( format( ID_STATUSBAR_READONLY ).c_str() );
}

#include "stdafx.h"
#include "rdodebugedit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ----------------------------------------------------------------------------
// ---------- RDODebugEdit
// ---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDODebugEdit, RDOBaseEdit )
	//{{AFX_MSG_MAP(RDODebugEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDODebugEdit::RDODebugEdit(): RDOBaseEdit()
{
}

RDODebugEdit::~RDODebugEdit()
{
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

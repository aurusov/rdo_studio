#include "stdafx.h"
#include "rdodebugedit.h"
#include "sci/SciLexer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
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
END_MESSAGE_MAP()

RDODebugEdit::RDODebugEdit(): RDOBaseEdit()
{
}

RDODebugEdit::~RDODebugEdit()
{
}

int RDODebugEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( RDOBaseEdit ::OnCreate(lpCreateStruct) == -1 ) return -1;

	sendEditor( SCI_SETLEXER, SCLEX_TEXT );
	int lexLanguage = sendEditor( SCI_GETLEXER );

	setReadOnly( true );

	return 0;
}

void RDODebugEdit::setEditorStyle( RDOBaseEditStyle* _style )
{
	RDOBaseEdit::setEditorStyle( _style );
	if ( !style ) return;

	// ----------
	// Colors
	sendEditor( SCI_STYLESETFORE, SCE_TEXT_DEFAULT, style->theme->defaultColor );
	sendEditor( SCI_STYLESETBACK, SCE_TEXT_DEFAULT, style->theme->backgroundColor );

	// ----------
	// Styles
	sendEditor( SCI_STYLESETBOLD     , SCE_TEXT_DEFAULT, style->theme->defaultStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_TEXT_DEFAULT, style->theme->defaultStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_TEXT_DEFAULT, style->theme->defaultStyle & RDOFS_UNDERLINE );
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

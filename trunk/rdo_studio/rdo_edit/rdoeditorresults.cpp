#include "stdafx.h"
#include "rdoeditorresults.h"
#include "../rdostudioapp.h"
#include "../resource.h"
#include "../htmlhelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
using namespace rdoEditor;

// ----------------------------------------------------------------------------
// ---------- RDOEditorResults
// ---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOEditorResults, RDOEditorBaseEdit )
	//{{AFX_MSG_MAP(RDOEditorResults)
	ON_WM_CREATE()
	ON_COMMAND(ID_HELP_KEYWORD, OnHelpKeyword)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI( ID_COORD_STATUSBAR , OnUpdateCoordStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODIFY_STATUSBAR, OnUpdateModifyStatusBar )
END_MESSAGE_MAP()

RDOEditorResults::RDOEditorResults(): RDOEditorBaseEdit()
{
}

RDOEditorResults::~RDOEditorResults()
{
}

int RDOEditorResults::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( RDOEditorBaseEdit ::OnCreate(lpCreateStruct) == -1 ) return -1;

	setReadOnly( true );

	return 0;
}

void RDOEditorResults::setEditorStyle( RDOEditorResultsStyle* _style )
{
	RDOEditorBaseEdit::setEditorStyle( _style );
	if ( !style ) return;
}

void RDOEditorResults::OnHelpKeyword()
{
	string filename = studioApp.getFullHelpFileName( "RAO-studio.chm" );
	if ( filename.empty() ) return;

	string keyword = getCurrentOrSelectedWord();

	string s = kw0;
	s += " ";
	s += kw1;
	s += " ";
	s += kw2;

	if ( s.find_first_of( keyword ) == string::npos || keyword.empty() ) {
		keyword = "pmv";
	}

	HH_AKLINK link;
	::ZeroMemory( &link, sizeof( HH_AKLINK ) );
	link.cbStruct     = sizeof( HH_AKLINK );
	link.fIndexOnFail = TRUE;
	link.pszKeywords  = keyword.c_str();

	::HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_KEYWORD_LOOKUP, (DWORD)&link );
}

void RDOEditorResults::OnUpdateCoordStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( format( "%d: %d", getCurrentColumnNumber() + 1, getCurrentLineNumber() + 1 ).c_str() );
}

void RDOEditorResults::OnUpdateModifyStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( format( ID_STATUSBAR_READONLY ).c_str() );
}

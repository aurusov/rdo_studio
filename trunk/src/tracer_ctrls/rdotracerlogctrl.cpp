#include "stdafx.h"
#include "rdotracerlogctrl.h"
//#include "../../rdostudioapp.h"
#include "../rdotracerapp.h"
//#include "../../rdostudiomainfrm.h"
#include "../rdotracermainfrm.h"
#include "../../resource.h"
#include "../../htmlhelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoTracerLog;
using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOTracerLogCtrl
// ----------------------------------------------------------------------------
static const UINT FIND_REPLASE_MSG = ::RegisterWindowMessage( FINDMSGSTRING );

BEGIN_MESSAGE_MAP( RDOTracerLogCtrl, RDOLogCtrl )
	//{{AFX_MSG_MAP(RDOTracerLogCtrl)
	ON_WM_CREATE()
	ON_COMMAND( ID_SEARCH_FIND, OnFind )
	ON_COMMAND( ID_SEARCH_FIND_NEXT    , OnFindNext )
	ON_COMMAND( ID_SEARCH_FIND_PREVIOUS, OnFindPrev )
	ON_COMMAND( ID_EDIT_COPY, OnCopy )
	ON_REGISTERED_MESSAGE( FIND_REPLASE_MSG, OnFindReplaceMsg )
	ON_UPDATE_COMMAND_UI( ID_EDIT_COPY           , OnCanCopy )
	ON_UPDATE_COMMAND_UI( ID_SEARCH_FIND_NEXT    , OnUpdateFindNextPrev )
	ON_UPDATE_COMMAND_UI( ID_SEARCH_FIND         , OnUpdateFind )
	ON_UPDATE_COMMAND_UI( ID_SEARCH_FIND_PREVIOUS, OnUpdateFindNextPrev )
	ON_WM_INITMENUPOPUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_HELP_KEYWORD, OnHelpKeyword)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI( ID_COORD_STATUSBAR , OnUpdateCoordStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODIFY_STATUSBAR, OnUpdateModifyStatusBar )
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE( RDOTracerLogCtrl, RDOLogCtrl )

RDOTracerLogCtrl::RDOTracerLogCtrl() :
	//RDOLogCtrl( &studioApp.mainFrame->style_trace ),
	RDOLogCtrl( &rdoTracerApp.mainFrame->style_trace ),
	addingSubitems( false ),
	bShowMenu( true )
{
}

RDOTracerLogCtrl::~RDOTracerLogCtrl()
{
}

int RDOTracerLogCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( RDOLogCtrl::OnCreate(lpCreateStruct) == -1 ) return -1;

	popupMenu.CreatePopupMenu();

	CMenu* mainMenu = AfxGetMainWnd()->GetMenu();
	
	//BOOL maximized;
	//studioApp.mainFrame->MDIGetActive( &maximized );
	//rdoTracerApp.mainFrame->MDIGetActive( &maximized );
	//int delta = maximized ? 1 : 0;
	int delta = 0;

	/*appendMenu( mainMenu->GetSubMenu( 1 + delta ), 4, &popupMenu );
	popupMenu.AppendMenu( MF_SEPARATOR );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 0, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 1, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 2, &popupMenu );*/

	appendMenu( mainMenu->GetSubMenu( 1 + delta ), 0, &popupMenu );
	popupMenu.AppendMenu( MF_SEPARATOR );
	appendMenu( mainMenu->GetSubMenu( 1 + delta ), 2, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 1 + delta ), 3, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 1 + delta ), 4, &popupMenu );

	return 0;
}


bool RDOTracerLogCtrl::getItemColors( const int index, RDOLogColorPair* &colors ) const
{
	const_cast<CMutex&>(mutex).Lock();

	bool res = true;
	RDOColorMap::const_iterator it = subitemColors.find( index );
	if( it != subitemColors.end() ) {
		colors = (*it).second;
	} else
		res = RDOLogCtrl::getItemColors( "", colors );

	const_cast<CMutex&>(mutex).Unlock();

	return res;
}

void RDOTracerLogCtrl::showFindError( string& findStr )
{
	MessageBox( format( ID_MSG_CANTFIND, findStr.c_str() ).c_str(), NULL, MB_OK | MB_ICONWARNING );
	SetFocus();
}

void RDOTracerLogCtrl::clear()
{
	mutex.Lock();

	RDOLogCtrl::clear();
	subitemColors.clear();

	mutex.Unlock();
}

void RDOTracerLogCtrl::addStringToLog( const string logStr )
{
	mutex.Lock();
	
	if ( !logStr.empty() ) {
		int posstart = logStr.find_first_not_of( ' ' );
		int posend = logStr.find_first_of( ' ', posstart );
		string key = logStr.substr( posstart, posend - posstart );
		trim( key );

		RDOLogColorPair* colors = NULL;

		if ( logStyle->getItemColors( key, colors ) )
			addingSubitems = false;
		else if ( addingSubitems ) {
			subitemColors.insert( RDOColorMap::value_type( stringsCount, itemColor ) );
		}
		
		if ( key == "SD" ) {
			addingSubitems = true;
			logStyle->getItemColors( key, itemColor );
		}
	}
	
	RDOLogCtrl::addStringToLog( logStr );

	mutex.Unlock();
}

void RDOTracerLogCtrl::setStyle( RDOTracerLogStyle* style, const bool needRedraw )
{
	logStyle = style;
	RDOLogCtrl::setStyle( style, needRedraw );
}

void RDOTracerLogCtrl::OnCopy()
{
	copy();
}

void RDOTracerLogCtrl::OnFind()
{
	firstFoundLine = -1;
	CFindReplaceDialog* pDlg = new CFindReplaceDialog();
	DWORD flag = (bSearchDown ? FR_DOWN : 0) | (bMatchCase ? FR_MATCHCASE : 0) | (bMatchWholeWord ? FR_WHOLEWORD : 0);
	string str;
	getSelected( str );
	pDlg->Create( true, str.c_str(), NULL, flag, this );
}

void RDOTracerLogCtrl::OnFindNext()
{
	firstFoundLine = -1;
	findNext();
	if ( !bHaveFound ) {
		showFindError( findStr );
	}
}

void RDOTracerLogCtrl::OnFindPrev()
{
	firstFoundLine = -1;
	findPrevious();
	if ( !bHaveFound ) {
		showFindError( findStr );
	}
}

LRESULT RDOTracerLogCtrl::OnFindReplaceMsg( WPARAM wParam, LPARAM lParam )
{
	CFindReplaceDialog* pDialog = CFindReplaceDialog::GetNotifier( lParam );

	findStr = pDialog->GetFindString();

	if ( pDialog->IsTerminating() ) {
		firstFoundLine = -1;
		SetFocus();
		return 0;
	} else {
		bool newSearchDown     = pDialog->SearchDown() ? true : false;
		bool newMatchCase      = pDialog->MatchCase() ? true : false;
		bool newMatchWholeWord = pDialog->MatchWholeWord() ? true : false;
		if ( newSearchDown != bSearchDown || newMatchCase != bMatchCase || newMatchWholeWord != bMatchWholeWord ) {
			firstFoundLine = -1;
		}
		bSearchDown     = newSearchDown;
		bMatchCase      = newMatchCase;
		bMatchWholeWord = newMatchWholeWord;

		if ( pDialog->FindNext() ) {
			findNext();
			if ( !bHaveFound ) {
				showFindError( findStr );
			}
		}
	}
	return 0;
}

void RDOTracerLogCtrl::OnInitMenuPopup( CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu )
{
	CWnd::OnInitMenuPopup( pPopupMenu, nIndex, bSysMenu );
	CFrameWnd* pwndFrame = (CFrameWnd*)AfxGetMainWnd();
	if( pwndFrame ) pwndFrame->SendMessage( WM_INITMENUPOPUP, WPARAM(pPopupMenu->m_hMenu), MAKELPARAM(nIndex, bSysMenu) );
}

void RDOTracerLogCtrl::OnContextMenu( CWnd* pWnd, CPoint pos )
{
	if ( !bShowMenu ) return;
	CWnd::OnContextMenu( pWnd, pos );
	if ( popupMenu.m_hMenu ) popupMenu.TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, this );
}

void RDOTracerLogCtrl::OnCanCopy( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( canCopy() );
}

void RDOTracerLogCtrl::OnUpdateFindNextPrev( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( !findStr.empty() );
}

void RDOTracerLogCtrl::OnUpdateFind( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( stringsCount );
}

void RDOTracerLogCtrl::OnHelpKeyword()
{
	string filename = rdoTracerApp.getFullHelpFileName( "RAO-language.chm" );
	if ( filename.empty() ) return;

	string line;

	getSelected( line );

	if ( !line.empty() ) {
		int posstart = line.find_first_not_of( ' ' );
		int posend = line.find_first_of( ' ', posstart );
		string keyword = line.substr( posstart, posend - posstart );
		trim( keyword );

		if ( !keyword.empty() ) {
			RDOLogColorPair* colors;
			if ( !logStyle->getItemColors( keyword, colors ) ) {
				getItemColors( selectedLine, colors );
				if ( *colors == static_cast<RDOTracerLogTheme*>(logStyle->theme)->sd )
					keyword = "SD";
				else
					keyword = "trc";
			}
		} else {
			keyword = "trc";
		}


		HH_AKLINK link;
		::ZeroMemory( &link, sizeof( HH_AKLINK ) );
		link.cbStruct     = sizeof( HH_AKLINK );
		link.fIndexOnFail = TRUE;
		link.pszKeywords  = keyword.c_str();

		::HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_KEYWORD_LOOKUP, (DWORD)&link );
	} else {
		filename += "::/html/rdo_res_trc.htm";
		HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_DISPLAY_TOPIC, NULL );
	}

}

void RDOTracerLogCtrl::OnUpdateCoordStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	string str = "";
	if ( selectedLine != -1 ) {
		int x = 1;
		int y = selectedLine + 1;
		str = format( "%d: %d", x, y );
	}
	pCmdUI->SetText( str.c_str() );
}

void RDOTracerLogCtrl::OnUpdateModifyStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( format( ID_STATUSBAR_READONLY ).c_str() );
}
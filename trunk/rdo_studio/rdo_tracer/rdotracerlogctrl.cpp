#include "stdafx.h"
#include "rdotracerlogctrl.h"
#include "../rdostudioapp.h"
#include "../rdostudiomainfrm.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ----------------------------------------------------------------------------
// ---------- RDOTracerLogCtrl
// ----------------------------------------------------------------------------
static const UINT FIND_REPLASE_MSG = ::RegisterWindowMessage( FINDMSGSTRING );

BEGIN_MESSAGE_MAP( RDOTracerLogCtrl, RDOLogCtrl )
	//{{AFX_MSG_MAP(RDOTracerLogCtrl)
	ON_COMMAND( ID_SEARCH_FIND, OnFind )
	ON_COMMAND( ID_SEARCH_FINDNEXT       , OnFindNext )
	ON_COMMAND( ID_SEARCH_FINDPREVIOUS   , OnFindPrev )
	ON_REGISTERED_MESSAGE( FIND_REPLASE_MSG, OnFindReplaceMsg )
	ON_COMMAND( ID_EDIT_COPY, OnCopy )
	ON_UPDATE_COMMAND_UI( ID_EDIT_COPY         , OnCanCopy )
	ON_UPDATE_COMMAND_UI( ID_SEARCH_FIND         , OnUpdateFind )
	ON_UPDATE_COMMAND_UI( ID_SEARCH_FINDNEXT    , OnUpdateFindNextPrev )
	ON_UPDATE_COMMAND_UI( ID_SEARCH_FINDPREVIOUS, OnUpdateFindNextPrev )
	ON_WM_CREATE()
	ON_WM_INITMENUPOPUP()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE( RDOTracerLogCtrl, RDOLogCtrl )

RDOTracerLogCtrl::RDOTracerLogCtrl() :
	RDOLogCtrl( new RDOTracerLogStyle() ),
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
	
	BOOL maximized;
	studioApp.mainFrame->MDIGetActive( &maximized );
	int delta = maximized ? 1 : 0;

	appendMenu( mainMenu->GetSubMenu( 1 + delta ), 4, &popupMenu );
	popupMenu.AppendMenu( MF_SEPARATOR );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 0, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 1, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 2, &popupMenu );

	return 0;
}


bool RDOTracerLogCtrl::getItemColors( const int index, COLORREF& textColor, COLORREF& backColor ) const
{
	bool res = true;
	RDOColorMap::const_iterator it = subitemColors.find( index );
	if( it != subitemColors.end() ) {
		textColor = (*it).second.foregroundColor;
		backColor = (*it).second.backgroundColor;
	} else
		res = false;
	return res;
}

void RDOTracerLogCtrl::showFindError( CString& findStr )
{
	CString s;
	s.Format( ID_MSG_CANTFIND, findStr );
	MessageBox( s, NULL, MB_OK | MB_ICONWARNING );
	SetFocus();
}

void RDOTracerLogCtrl::clear()
{
	RDOLogCtrl::clear();
	subitemColors.clear();
}

void RDOTracerLogCtrl::addStringToLog( const CString logStr )
{
	CString key = logStr.SpanExcluding( " " );
	key.TrimLeft();
	key.TrimRight();

	COLORREF fore;
	COLORREF back;

	if ( logStyle->getItemColors( key, fore, back ) )
		addingSubitems = false;
	else if ( addingSubitems ) {
		subitemColors.insert( RDOColorMap::value_type( logStrings.GetCount(), itemColor ) );
	}
	
	if ( key == "SD" ) {
		addingSubitems = true;
		logStyle->getItemColors( key, fore, back );
		itemColor.backgroundColor = back;
		itemColor.foregroundColor = fore;
	}
	
	RDOLogCtrl::addStringToLog( logStr );
}

void RDOTracerLogCtrl::setStyle( const RDOTracerLogStyle& style, const bool needRedraw )
{
	*(RDOTracerLogStyle*)logStyle = style;
	RDOLogCtrl::setStyle( style, needRedraw );
}

void RDOTracerLogCtrl::startTrace()
{
	clear();
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
	pDlg->Create( true, getSelected(), NULL, flag, this );
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
	pCmdUI->Enable( !findStr.IsEmpty() );
}

void RDOTracerLogCtrl::OnUpdateFind( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( logStrings.GetCount() );
}

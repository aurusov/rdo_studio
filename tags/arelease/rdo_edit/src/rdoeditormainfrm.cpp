#include "stdafx.h"
#include "rdoeditormainfrm.h"
#include "rdoeditorapp.h"
#include "rdoeditoroptions.h"
#include "../resource.h"
#include "../Htmlhelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ----------------------------------------------------------------------------
// ---------- RDOEditorStatusBar
// ----------------------------------------------------------------------------
typedef void (*connectRdoAppFun)( HWND );
typedef void (*disconnectRdoAppFun)( HWND );
typedef bool (*isConnectedFun)( HWND );
typedef void (*setHookFun)( HWND, HHOOK );
typedef LRESULT (CALLBACK *callWndProcFun)( int, WPARAM, LPARAM );

BEGIN_MESSAGE_MAP( RDOEditorStatusBar, CStatusBar )
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

RDOEditorStatusBar::RDOEditorStatusBar(): CStatusBar()
{
}

RDOEditorStatusBar::~RDOEditorStatusBar()
{
}

void RDOEditorStatusBar::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	CStatusBar::OnLButtonDblClk( nFlags, point );
	CRect rect;
	GetItemRect( 3, rect );
	if ( rect.PtInRect( point ) ) {
		((RDOEditorMainFrame*)AfxGetMainWnd())->showRunError();
	}
}

void RDOEditorStatusBar::OnRButtonDown( UINT nFlags, CPoint point )
{
	CStatusBar::OnRButtonDown( nFlags, point );
	CRect rect;
	GetItemRect( 3, rect );
	if ( rect.PtInRect( point ) ) {
		((RDOEditorMainFrame*)AfxGetMainWnd())->gotoRunError();
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorMainFrame
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOEditorMainFrame, CFrameWnd )

	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_SETFOCUS()
	ON_WM_TIMER()

	ON_COMMAND( ID_FILE_NEW         , OnProjectNew )
	ON_COMMAND( ID_FILE_OPEN        , OnProjectLoad )
	ON_COMMAND_RANGE( ID_FILE_REOPEN_1, ID_FILE_REOPEN_10, OnProjectReopen )
	ON_COMMAND( ID_FILE_SAVE        , OnProjectSave )
	ON_COMMAND( ID_FILE_SAVE_AS     , OnProjectSaveAs )
	ON_COMMAND( ID_RDO_RUN          , OnProjectStart )
	ON_COMMAND( ID_RDO_STOP         , OnProjectStop )
	ON_COMMAND( ID_RDO_PATH         , OnProjectPath )
	ON_COMMAND( ID_RDO_RELOADPMVFILE, OnProjectLoadPMV )
	ON_COMMAND( ID_RDO_RELOADTRCFILE, OnProjectLoadTRC )
	ON_UPDATE_COMMAND_UI( ID_FILE_SAVE        , OnUpdateProjectSave )
	ON_UPDATE_COMMAND_UI( ID_RDO_RUN          , OnUpdateProjectStart )
	ON_UPDATE_COMMAND_UI( ID_RDO_STOP         , OnUpdateProjectStop )
	ON_UPDATE_COMMAND_UI( ID_RDO_RELOADPMVFILE, OnUpdateProjectLoadPMVTRC )
	ON_UPDATE_COMMAND_UI( ID_RDO_RELOADTRCFILE, OnUpdateProjectLoadPMVTRC )

	ON_COMMAND( ID_SEARCH_BOOKMARKTOGGLE  , bookmarkToggle )
	ON_COMMAND( ID_SEARCH_BOOKMARKNEXT    , bookmarkNext )
	ON_COMMAND( ID_SEARCH_BOOKMARKPREVIOUS, bookmarkPrev )
	ON_COMMAND( ID_SEARCH_BOOKMARKSCLEAR  , bookmarkClearAll )
	ON_UPDATE_COMMAND_UI( ID_SEARCH_BOOKMARKNEXT    , OnHasBookmarks )
	ON_UPDATE_COMMAND_UI( ID_SEARCH_BOOKMARKPREVIOUS, OnHasBookmarks )
	ON_UPDATE_COMMAND_UI( ID_SEARCH_BOOKMARKSCLEAR  , OnHasBookmarks )

	ON_COMMAND_EX( ID_VIEW_WHITESPACE      , OnSciCommandForAll )
	ON_COMMAND_EX( ID_VIEW_ENDOFLINE       , OnSciCommandForAll )
	ON_COMMAND_EX( ID_VIEW_LINENUMBERMARGIN, OnSciCommandForAll )
	ON_COMMAND_EX( ID_VIEW_MARGIN          , OnSciCommandForAll )
	ON_COMMAND_EX( ID_VIEW_FOLDMARGIN      , OnSciCommandForAll )
	ON_COMMAND_EX( ID_VIEW_ZOOMIN          , OnSciCommandForAll )
	ON_COMMAND_EX( ID_VIEW_ZOOMOUT         , OnSciCommandForAll )
	ON_COMMAND_EX( ID_VIEW_ZOOMRESET       , OnSciCommandForAll )
	ON_UPDATE_COMMAND_UI( ID_VIEW_WHITESPACE      , OnUpdateWhiteSpace )
	ON_UPDATE_COMMAND_UI( ID_VIEW_ENDOFLINE       , OnUpdateEndOfLine )
	ON_UPDATE_COMMAND_UI( ID_VIEW_LINENUMBERMARGIN, OnUpdateViewLineNumberMargin )
	ON_UPDATE_COMMAND_UI( ID_VIEW_MARGIN          , OnUpdateViewMargin )
	ON_UPDATE_COMMAND_UI( ID_VIEW_FOLDMARGIN      , OnUpdateViewFoldMargin )
	ON_UPDATE_COMMAND_UI( ID_VIEW_ZOOMIN          , OnUpdateZoomIn )
	ON_UPDATE_COMMAND_UI( ID_VIEW_ZOOMOUT         , OnUpdateZoomOut )
	ON_UPDATE_COMMAND_UI( ID_VIEW_ZOOMRESET       , OnUpdateZoomReset )

	ON_COMMAND( ID_VIEW_PROJECTTOOLBAR, OnProjectToolBar )
	ON_COMMAND( ID_VIEW_EDITTOOLBAR   , OnEditToolBar )
	ON_UPDATE_COMMAND_UI( ID_VIEW_PROJECTTOOLBAR, OnUpdateProjectToolBar )
	ON_UPDATE_COMMAND_UI( ID_VIEW_EDITTOOLBAR   , OnUpdateEditToolBar )

	ON_COMMAND( ID_VIEW_OPTIONS, OnOptions )

	ON_COMMAND( ID_HELP_CONTENTS, OnHelpContents )

	ON_UPDATE_COMMAND_UI( ID_COORDSTATUSBAR          , OnUpdateCoordStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODIFYSTATUSBAR         , OnUpdateModifyStatusBar )
	ON_UPDATE_COMMAND_UI( ID_INSERTOVERWRITESTATUSBAR, OnUpdateInsertOverwriteStatusBar )
	ON_UPDATE_COMMAND_UI( ID_INFOSTATUSBAR           , OnUpdateInfoStatusBar )

END_MESSAGE_MAP()

static UINT indicators[] = {
	ID_COORDSTATUSBAR,
	ID_MODIFYSTATUSBAR,
	ID_INSERTOVERWRITESTATUSBAR,
	ID_INFOSTATUSBAR
};

#define timerRDO_ID  1
#define timerBuf1_ID 2
#define timerBuf2_ID 3
#define timerBuf3_ID 4
#define timerBuf4_ID 5

RDOEditorMainFrame::RDOEditorMainFrame():
	CFrameWnd(),
	rdoMainHWnd( 0 ),
	runError( RDORE_None ),
	strError( "" ),
	debug( RDOD_None ),
	debugNext( RDOD_None ),
	debugX( -1 ),
	debugY( -1 ),
	debugTabItem( RDOEDIT_NONE ),
	hook( 0 ),
	timerRDO( 0 ),
	timerBuf1( 0 ),
	timerBuf2( 0 ),
	timerBuf3( 0 ),
	timerBuf4( 0 )
{
	m_bAutoMenuEnable = false;
}

RDOEditorMainFrame::~RDOEditorMainFrame()
{
}

BOOL RDOEditorMainFrame::PreCreateWindow( CREATESTRUCT& cs )
{
	if( !CFrameWnd::PreCreateWindow( cs ) ) return FALSE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass( 0, ::LoadCursor(NULL, IDC_ARROW) );
	return TRUE;
}

int RDOEditorMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CFrameWnd::OnCreate(lpCreateStruct) == -1 ) return -1;

	childView.Create( NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL );

	CString s;
	s.LoadString( ID_TOOLBAR_PROJECT );
	projectToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_SIZE_DYNAMIC );
	projectToolBar.LoadToolBar( IDR_PROJECTTOOLBAR );
	projectToolBar.GetToolBarCtrl().SetWindowText( s );

	projectToolBarImageList.Create( IDB_PROJECTTOOLBAR_D, 16, 0, 0xFF00FF );
	projectToolBar.GetToolBarCtrl().SetDisabledImageList( &projectToolBarImageList );

	s.LoadString( ID_TOOLBAR_EDIT );
	editToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_SIZE_DYNAMIC );
	editToolBar.LoadToolBar( IDR_EDITTOOLBAR );
	editToolBar.GetToolBarCtrl().SetWindowText( s );

	editToolBarImageList.Create( IDB_EDITTOOLBAR_D, 16, 0, 0xFF00FF );
	editToolBar.GetToolBarCtrl().SetDisabledImageList( &editToolBarImageList );

	statusBar.Create( this );
	statusBar.SetIndicators( indicators, 4 );
	statusBar.SetPaneInfo( 0, ID_COORDSTATUSBAR          , SBPS_NORMAL , 70 );
	statusBar.SetPaneInfo( 1, ID_MODIFYSTATUSBAR         , SBPS_NORMAL , 70 );
	statusBar.SetPaneInfo( 2, ID_INSERTOVERWRITESTATUSBAR, SBPS_NORMAL , 70 );
	statusBar.SetPaneInfo( 3, ID_INFOSTATUSBAR           , SBPS_STRETCH, 70 );

	projectToolBar.EnableDocking( CBRS_ALIGN_ANY );
	editToolBar.EnableDocking( CBRS_ALIGN_ANY );

	EnableDocking( CBRS_ALIGN_ANY );

	DockControlBar( &projectToolBar );
	dockControlBarBesideOf( editToolBar, projectToolBar );

	loadRdoPath();
	loadReopen();
	updateReopenSubMenu();

	rdoEditorApp.setCaption( "" );

	RDODebug d = canHook() ? RDOD_Hook : RDOD_Timer;
	debug = (RDODebug)rdoEditorApp.GetProfileInt( "debug", "debug", d );
	if ( d != RDOD_Hook && debug == RDOD_Hook ) {
		debug = d;
	}
	setDebug( debug );

	pi.dwProcessId = 0;
	pi.dwThreadId  = 0;
	pi.hProcess    = 0;
	pi.hThread     = 0;

	return 0;
}

void RDOEditorMainFrame::OnClose()
{
	if ( rdoEditorProject.closeProject() ) {
		stopTimer( timerRDO );
		stopTimer( timerBuf1 );
		stopTimer( timerBuf2 );
		stopTimer( timerBuf3 );
		stopTimer( timerBuf4 );
		OnProjectStop();
		CFrameWnd::OnClose();
	}
}

BOOL RDOEditorMainFrame::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo )
{
	if ( childView.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	return CFrameWnd::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}

int RDOEditorMainFrame::getIntegerAfterWord( const CString& str, const CString& word ) const
{
	int res = -1;
	int pos = str.Find( word );
	if ( pos != -1 ) {
		CString s = str;
		s.Delete( 0, pos + word.GetLength() );
		s.TrimLeft();
		int i;
		for ( i = 0; i < s.GetLength(); i++ ) {
			if ( s[i] == ' ' ) break;
		}
		s.Delete( i, s.GetLength() - i );
		s.TrimRight();
		res = strtol( s, NULL, 10 );
	}
	return res;
}

void RDOEditorMainFrame::showRunError()
{
	if ( runError != RDORE_None ) {
		CString s = "";
		switch ( runError ) {
			case RDORE_Syntax : s.LoadString( ID_RDOERROR_SYNTAX ); break;
			case RDORE_Runtime: s.LoadString( ID_RDOERROR_RUNTIME ); break;
			case RDORE_Load   : s.LoadString( ID_RDOERROR_LOAD ); break;
			case RDORE_License: s.LoadString( ID_RDOERROR_LICENSE ); break;
		}
		MessageBox( strError, s );
	}
}

void RDOEditorMainFrame::gotoRunError() const
{
	if ( debugX > 0 && debugY > 0 && debugTabItem != RDOEDIT_NONE ) {
		RDOEditorTabCtrl* tab = &((RDOEditorMainFrame*)AfxGetMainWnd())->childView.tab;
		tab->setCurrentRDOItem( debugTabItem );
		tab->getCurrentEdit()->setCurrentPos( debugY-1, debugX-1, true );
		tab->getCurrentEdit()->setErrorLine();
	}
}

LRESULT RDOEditorMainFrame::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
	if ( message == WM_USER + 144 ) {
		if ( wParam == WM_CREATE ) {
			if ( runError == RDORE_None ) {
				HWND h = (HWND)lParam;
				if ( pi.dwThreadId == ::GetWindowThreadProcessId( h, NULL ) ) {
					if ( !rdoMainHWnd ) {
						rdoMainHWnd = h;
						return 0;
					}
					HWND h1 = ::GetParent( h );
					if ( h1 && ::GetParent( h1 ) == rdoMainHWnd ) {
						CWnd* wnd = CWnd::FromHandle( h );
						CString text;
						wnd->GetWindowText( text );
						if ( text.GetLength() != 2 ) {

							CString s;

							// get error line
							if ( text.Find("Строка:") != -1 ) {
								s = "Строка:";
							} else {
								s = "Line:";
							}
							debugY = getIntegerAfterWord( text, s );

							// get error position
							if ( text.Find("Позиция:") != -1 ) {
								s = "Позиция:";
							} else {
								s = "Position:";
							}
							debugX = getIntegerAfterWord( text, s );

							// get error file ext
							if ( text.Find( "Файл:" ) != -1 ) {
								s = "Файл:";
							} else {
								s = "File:";
							}
							CString fileExt = "";
							int pos = text.Find( s );
							if ( pos != -1 ) {
								fileExt = text;
								fileExt.Delete( 0, pos + s.GetLength() );
								fileExt.TrimLeft();
								int i;
								for ( i = 0; i < fileExt.GetLength(); i++ ) {
									if ( fileExt[i] == ' ' ) break;
								}
								fileExt.Delete( i, fileExt.GetLength() - i );
								fileExt.TrimRight();
								i = fileExt.Find( "." );
								if ( i != -1 ) {
									fileExt.Delete( 0, i + 1 );
								} else {
									fileExt == "";
								}
							}

							if ( !fileExt.IsEmpty() && debugX > 0 && debugY > 0 && ( text.Find("Ошибка синтаксиса:") != -1 || text.Find("Syntax error:") != -1 )) {
								runError = RDORE_Syntax;
							} else if ( text.Find("Ошибка прогона:") != -1 || text.Find("Run-time error:") != -1 ) {
								runError = RDORE_Runtime;
							} else if ( debugX == 1 && debugY == 0 ) {
								runError = RDORE_Load;
							} else if ( text.Find("Истек срок лицензии") != -1 || text.Find("License period expired") != -1 ) {
								runError = RDORE_License;
							}
							if ( runError != RDORE_None ) {
								OnProjectStop();
								if ( runError == RDORE_Syntax ) {
									debugTabItem = RDOEDIT_NONE;
									if ( fileExt == "pat" ) {
										debugTabItem = RDOEDIT_PAT;
									} else if ( fileExt == "rtp" ) {
										debugTabItem = RDOEDIT_RTP;
									} else if ( fileExt == "rss" ) {
										debugTabItem = RDOEDIT_RSS;
									} else if ( fileExt == "rtp" ) {
										debugTabItem = RDOEDIT_RTP;
									} else if ( fileExt == "opr" ) {
										debugTabItem = RDOEDIT_OPR;
									} else if ( fileExt == "frm" ) {
										debugTabItem = RDOEDIT_FRM;
									} else if ( fileExt == "fun" ) {
										debugTabItem = RDOEDIT_FUN;
									} else if ( fileExt == "dpt" ) {
										debugTabItem = RDOEDIT_DPT;
									} else if ( fileExt == "smr" ) {
										debugTabItem = RDOEDIT_SMR;
									} else if ( fileExt == "pmd" ) {
										debugTabItem = RDOEDIT_PMD;
									}
									gotoRunError();
								}
								strError = text;
								showRunError();
								return 0;
							}
						}
					}
				}
			}
		} else if ( wParam == WM_NCDESTROY ) {
			HWND h = (HWND)lParam;
			if ( pi.dwThreadId == ::GetWindowThreadProcessId( h, NULL ) ) {
				if ( h == rdoMainHWnd ) {
					OnProjectStop();
				}
			}
		}
		return 0;
	}
	return CFrameWnd::WindowProc( message, wParam, lParam );
}

void RDOEditorMainFrame::dockControlBarBesideOf( CControlBar& bar, CControlBar& baseBar )
{
	RecalcLayout( TRUE );
	
	CRect rect;
	baseBar.GetWindowRect( rect );

	DWORD dw = baseBar.GetBarStyle();

	UINT n = 0;
	n = ( dw & CBRS_ALIGN_TOP          ) ? AFX_IDW_DOCKBAR_TOP    : n;
	n = ( dw & CBRS_ALIGN_BOTTOM && !n ) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = ( dw & CBRS_ALIGN_LEFT   && !n ) ? AFX_IDW_DOCKBAR_LEFT   : n;
	n = ( dw & CBRS_ALIGN_RIGHT  && !n ) ? AFX_IDW_DOCKBAR_RIGHT  : n;

	int dx = 0;
	int dy = 0;
	if ( n == AFX_IDW_DOCKBAR_TOP || n == AFX_IDW_DOCKBAR_BOTTOM ) dx = 1;
	if ( n == AFX_IDW_DOCKBAR_LEFT || n == AFX_IDW_DOCKBAR_RIGHT ) dy = 1;

	rect.OffsetRect( dx, dy );

	DockControlBar( &bar, n, rect );
}

void RDOEditorMainFrame::OnSetFocus( CWnd* pOldWnd )
{
	CFrameWnd::OnSetFocus( pOldWnd );
	childView.SetFocus();
}

void RDOEditorMainFrame::OnProjectLoad()
{
	insertReopenItem( rdoEditorProject.loadProject() );
};

void RDOEditorMainFrame::OnProjectReopen( UINT nID )
{
	int i = 0;
	switch( nID ) {
		case ID_FILE_REOPEN_1 : i = 0; break;
		case ID_FILE_REOPEN_2 : i = 1; break;
		case ID_FILE_REOPEN_3 : i = 2; break;
		case ID_FILE_REOPEN_4 : i = 3; break;
		case ID_FILE_REOPEN_5 : i = 4; break;
		case ID_FILE_REOPEN_6 : i = 5; break;
		case ID_FILE_REOPEN_7 : i = 6; break;
		case ID_FILE_REOPEN_8 : i = 7; break;
		case ID_FILE_REOPEN_9 : i = 8; break;
		case ID_FILE_REOPEN_10: i = 9; break;
	}
	CString s = reopenList.GetAt( reopenList.FindIndex( i ) );
	s = rdoEditorProject.loadProject( s );
	if ( s.IsEmpty() ) {
		reopenList.RemoveAt( reopenList.FindIndex( i ) );
		updateReopenSubMenu();
	} else {
		insertReopenItem( s );
	}
}

void RDOEditorMainFrame::insertReopenItem( const CString& item )
{
	if ( !item.IsEmpty() ) {
		POSITION pos = reopenList.Find( item );
		if ( !pos ) {
			reopenList.AddHead( item );
		} else {
			reopenList.RemoveAt( pos );
			reopenList.AddHead( item );
		}
		while ( reopenList.GetCount() > 10 ) {
			reopenList.RemoveTail();
		}
		updateReopenSubMenu();
	}
}

void RDOEditorMainFrame::updateReopenSubMenu() const
{
	CMenu* reopenMenu = AfxGetMainWnd()->GetMenu()->GetSubMenu( 0 )->GetSubMenu( 2 );

	while ( reopenMenu->GetMenuItemCount() ) {
		reopenMenu->DeleteMenu( 0, MF_BYPOSITION );
	}

	if ( !reopenList.IsEmpty() ) {
		AfxGetMainWnd()->GetMenu()->GetSubMenu( 0 )->EnableMenuItem( 2, MF_BYPOSITION | MF_ENABLED );
		for ( int i = 0; i < reopenList.GetCount(); i++ ) {
			if ( i == 4 ) reopenMenu->AppendMenu( MF_SEPARATOR );
			int id = ID_FILE_MRU_FILE1;
			switch( i ) {
				case 0: id = ID_FILE_REOPEN_1; break;
				case 1: id = ID_FILE_REOPEN_2; break;
				case 2: id = ID_FILE_REOPEN_3; break;
				case 3: id = ID_FILE_REOPEN_4; break;
				case 4: id = ID_FILE_REOPEN_5; break;
				case 5: id = ID_FILE_REOPEN_6; break;
				case 6: id = ID_FILE_REOPEN_7; break;
				case 7: id = ID_FILE_REOPEN_8; break;
				case 8: id = ID_FILE_REOPEN_9; break;
				case 9: id = ID_FILE_REOPEN_10; break;
			}
			POSITION pos = reopenList.FindIndex( i );
			CString s;
			s.Format( "%d. %s", i+1, reopenList.GetAt( pos ) );
			reopenMenu->AppendMenu( MF_STRING, id, s );
		}
	} else {
		AfxGetMainWnd()->GetMenu()->GetSubMenu( 0 )->EnableMenuItem( 2, MF_BYPOSITION | MF_DISABLED | MF_GRAYED );
	}

	saveReopen();
}

void RDOEditorMainFrame::loadReopen()
{
	reopenList.RemoveAll();
	for ( int i = 0; i < 10; i++ ) {
		CString sec;
		if ( i+1 < 10 ) {
			sec.Format( "0%d", i+1 );
		} else {
			sec.Format( "%d", i+1 );
		}
		TRY {
			CString s = rdoEditorApp.GetProfileString( "reopen", sec, "" );
			if ( !s.IsEmpty() ) {
				reopenList.AddTail( s );
			}
		} CATCH( CException, e ) {
		}
		END_CATCH
	}
}

void RDOEditorMainFrame::saveReopen() const
{
	for ( int i = 0; i < 10; i++ ) {
		CString sec;
		if ( i+1 < 10 ) {
			sec.Format( "0%d", i+1 );
		} else {
			sec.Format( "%d", i+1 );
		}
		CString s;
		if ( i < reopenList.GetCount() ) {
			s = reopenList.GetAt( reopenList.FindIndex( i ) );
		} else {
			s = "";
		}
		TRY {
			rdoEditorApp.WriteProfileString( "reopen", sec, s );
		} CATCH( CException, e ) {
		}
		END_CATCH
	}
}

CString RDOEditorMainFrame::loadRdoPath() const
{
	TRY {
		CString s = rdoEditorApp.GetProfileString( "RAO-simulator", "path", "" );

		CFileStatus status;
		if ( CFile::GetStatus( s, status ) ) {
			rdoEditorProject.rdoFileName = s;
		} else {
			rdoEditorProject.rdoFileName = "";
		}

	} CATCH( CException, e ) {
		rdoEditorProject.rdoFileName = "";
	}
	END_CATCH
	return rdoEditorProject.rdoFileName;
}

void RDOEditorMainFrame::saveRdoPath( const CString& path ) const
{
	TRY {
		rdoEditorApp.WriteProfileString( "RAO-simulator", "path", path );
	} CATCH( CException, e ) {
	}
	END_CATCH
}

void RDOEditorMainFrame::stopTimer( UINT& timer )
{
	if ( timer ) {
		KillTimer( timer );
		timer = 0;
	}
}

void RDOEditorMainFrame::restartBufTimer( const int bufIndex )
{
	bool canClear = rdoEditorProject.canClearBuffer;
	int  delay    = rdoEditorProject.clearBufferDelay * 1000;
	if ( delay < 0 ) delay = 0;
	switch ( bufIndex ) {
		case 1: stopTimer( timerBuf1 ); if ( canClear ) timerBuf1 = SetTimer( timerBuf1_ID, delay, NULL ); break;
		case 2: stopTimer( timerBuf2 ); if ( canClear ) timerBuf2 = SetTimer( timerBuf2_ID, delay, NULL ); break;
		case 3: stopTimer( timerBuf3 ); if ( canClear ) timerBuf3 = SetTimer( timerBuf3_ID, delay, NULL ); break;
		case 4: stopTimer( timerBuf4 ); if ( canClear ) timerBuf4 = SetTimer( timerBuf4_ID, delay, NULL ); break;
	}
}

void RDOEditorMainFrame::OnProjectStart()
{
	if ( rdoEditorProject.saveProject() && !rdoEditorProject.isRunning() ) {
		::SetCurrentDirectory( rdoEditorApp.extractFilePath( rdoEditorProject.modelFileName ) );
		rdoMainHWnd = 0;
		runError    = RDORE_None;
		strError    = "";
		STARTUPINFO si;
		memset( &si, 0, sizeof(STARTUPINFO) );
		si.cb          = sizeof(STARTUPINFO);
		si.wShowWindow = SW_SHOWMAXIMIZED;
	    si.dwFlags     = STARTF_USESHOWWINDOW;
		pi.dwProcessId = 0;
		pi.dwThreadId  = 0;
		pi.hProcess    = 0;
		pi.hThread     = 0;
		CString str = "\"" + rdoEditorProject.rdoFileName + "\" \"" + rdoEditorProject.getName() + ".smr\"";

		debug = debugNext;
		stopTimer( timerRDO );

		if ( ::CreateProcess( NULL, (char*)((LPCTSTR)str), NULL, NULL, false, CREATE_SUSPENDED | NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi ) ) {
			switch ( debug ) {
				case RDOD_None: {
					::ResumeThread( pi.hThread );
					break;
				}
				case RDOD_Hook: {
					rdoEditorProject.start();
					hook = 0;
					connectRdoAppFun connect = (connectRdoAppFun)::GetProcAddress( rdoEditorApp.dllHandle, "connectRdoApp" );
					if ( connect ) {
						connect( AfxGetMainWnd()->m_hWnd );

						callWndProcFun callback = (callWndProcFun)::GetProcAddress( rdoEditorApp.dllHandle, "callWndProc" );
						if ( callback ) {
							hook = ::SetWindowsHookEx( WH_CALLWNDPROC, callback, rdoEditorApp.dllHandle, 0 );

							setHookFun setHook = (setHookFun)::GetProcAddress( rdoEditorApp.dllHandle, "setHook" );
							if ( setHook ) {
								setHook( AfxGetMainWnd()->m_hWnd, hook );
							}
						} else {
							disconnectRdoAppFun disconnect = (disconnectRdoAppFun)::GetProcAddress( rdoEditorApp.dllHandle, "disconnectRdoApp" );
							if ( disconnect ) {
								disconnect( AfxGetMainWnd()->m_hWnd );
							}
						}
					}
					if ( !hook ) {
						rdoEditorProject.stop();
						CString s;
						s.LoadString( ID_MSG_RDOSTARTFULLDEBUG_ERROR );
						MessageBox( s, NULL, MB_OK | MB_ICONERROR );
					} else {
						::ResumeThread( pi.hThread );
					}
					break;
				}
				case RDOD_Timer: {
					rdoEditorProject.start();
					timerRDO = SetTimer( timerRDO_ID, 1000, NULL );
					::ResumeThread( pi.hThread );
					break;
				}
				case RDOD_WaitForSingleObject: {
					rdoEditorProject.start();
					::ResumeThread( pi.hThread );
					::WaitForSingleObject( pi.hProcess, INFINITE );
					rdoEditorProject.stop();
					break;
				}
			}
		} else {
			CString s;
			s.Format( ID_MSG_RDOSTART_ERROR, rdoEditorProject.rdoFileName );
			MessageBox( s, NULL, MB_OK | MB_ICONERROR );
		}
	}
}

void RDOEditorMainFrame::OnProjectStop()
{
	if ( hook ) {

		::UnhookWindowsHookEx( hook );

		disconnectRdoAppFun disconnect = (disconnectRdoAppFun)::GetProcAddress( rdoEditorApp.dllHandle, "disconnectRdoApp" );
		if ( disconnect ) {
			disconnect( AfxGetMainWnd()->m_hWnd );
		}

		hook = 0;

		setHookFun setHook = (setHookFun)::GetProcAddress( rdoEditorApp.dllHandle, "setHook" );
		if ( setHook ) {
			setHook( AfxGetMainWnd()->m_hWnd, hook );
		}

	}

	rdoEditorProject.stop();

	if ( pi.dwProcessId ) {
		::TerminateProcess( pi.hProcess, 0 );
		::CloseHandle( pi.hThread );
		::CloseHandle( pi.hProcess );
		pi.dwProcessId = 0;
		pi.dwThreadId  = 0;
		pi.hProcess    = 0;
		pi.hThread     = 0;
	}
}

void RDOEditorMainFrame::OnTimer( UINT nIDEvent )
{
	CFrameWnd::OnTimer( nIDEvent );
	if ( nIDEvent == timerRDO ) {
		DWORD status;
		if ( ::GetExitCodeProcess( pi.hProcess, &status ) ) {
			if ( status == STILL_ACTIVE ) {
			} else {
				rdoEditorProject.stop();
				stopTimer( timerRDO );
			}
		} else {
			rdoEditorProject.stop();
			stopTimer( timerRDO );
		}
	} else if ( nIDEvent == timerBuf1 ) {
		rdoEditorProject.resetBuf1 = true;
		stopTimer( timerBuf1 );
	} else if ( nIDEvent == timerBuf2 ) {
		rdoEditorProject.resetBuf2 = true;
		stopTimer( timerBuf2 );
	} else if ( nIDEvent == timerBuf3 ) {
		rdoEditorProject.resetBuf3 = true;
		stopTimer( timerBuf3 );
	} else if ( nIDEvent == timerBuf4 ) {
		rdoEditorProject.resetBuf4 = true;
		stopTimer( timerBuf4 );
	}
}

RDODebug RDOEditorMainFrame::getDebug() const
{
	return debugNext;
}

void RDOEditorMainFrame::setDebug( const RDODebug value )
{
	debugNext = value;
	rdoEditorApp.WriteProfileInt( "debug", "debug", debugNext );
}

bool RDOEditorMainFrame::canHook() const
{
	return rdoEditorApp.dllHandle != 0;
}

void RDOEditorMainFrame::OnProjectPath()
{
	CString s;
	s.LoadString( ID_RDO_FILETYPE );
	CFileDialog dlg( true, "exe", rdoEditorProject.rdoFileName, OFN_HIDEREADONLY, s );
	if ( dlg.DoModal() == IDOK ) {
		rdoEditorProject.rdoFileName = dlg.GetPathName();
		saveRdoPath( rdoEditorProject.rdoFileName );
	}
}

void RDOEditorMainFrame::bookmarkToggle()
{
	childView.tab.getCurrentEdit()->bookmarkToggle();
}

void RDOEditorMainFrame::bookmarkNext()
{
	RDOEditorEdit* edit_first = childView.tab.getCurrentEdit();
	if ( edit_first->bookmarkNext( false, true ) ) return;
	
	RDOEditorEdit* edit = edit_first;
	bool allItem = false;
	bool wasLoop = true;
	while ( !allItem && edit && wasLoop ) {
		edit = (RDOEditorEdit*)childView.tab.getItemNext( edit );
		if ( edit == edit_first ) {
			allItem = true;
		} else {
			edit->bookmarkNext( false, false, &wasLoop );
		}
	}
	if ( !allItem ) {
		childView.tab.setCurrentItem( childView.tab.findItem( edit ) );
	} else {
		edit->bookmarkNext();
	}
}

void RDOEditorMainFrame::bookmarkPrev()
{
	RDOEditorEdit* edit_first = childView.tab.getCurrentEdit();
	if ( edit_first->bookmarkPrev( false, true ) ) return;
	
	RDOEditorEdit* edit = edit_first;
	bool allItem = false;
	bool wasLoop = true;
	while ( !allItem && edit && wasLoop ) {
		edit = (RDOEditorEdit*)childView.tab.getItemNext( edit, false, true );
		if ( edit == edit_first ) {
			allItem = true;
		} else {
			edit->bookmarkPrev( false, false, &wasLoop );
		}
	}
	if ( !allItem ) {
		childView.tab.setCurrentItem( childView.tab.findItem( edit ) );
	} else {
		edit->bookmarkPrev();
	}
}

void RDOEditorMainFrame::bookmarkClearAll()
{
	for ( int i = 0; i < childView.tab.getItemCount(); i++ ) {
		childView.tab.getItemEdit( i )->bookmarkClearAll();
	}
}

void RDOEditorMainFrame::OnHasBookmarks( CCmdUI* pCmdUI )
{
	bool flag = false;
	for ( int i = 0; i < childView.tab.getItemCount(); i++ ) {
		flag = childView.tab.getItemEdit( i )->hasBookmarks();
		if ( flag ) break;
	}
	pCmdUI->Enable( flag );
}

void RDOEditorMainFrame::OnSciCommandForAll( UINT nID )
{
	RDOEditorEdit* edit = childView.tab.getCurrentEdit();
	int itemCount = childView.tab.getItemCount();

	switch ( nID ) {
		case ID_VIEW_WHITESPACE: {
			bool flag = !edit->isViewWhiteSpace();
			for ( int i = 0; i < itemCount; i++ ) {
				childView.tab.getItemEdit( i )->setViewWhiteSpace( flag );
			}
			break;
		}
		case ID_VIEW_ENDOFLINE : {
			bool flag = !edit->isViewEndOfLine();
			for ( int i = 0; i < itemCount; i++ ) {
				childView.tab.getItemEdit( i )->setEndOfLine( flag );
			}
			break;
		}
		case ID_VIEW_LINENUMBERMARGIN: {
			bool flag = !edit->isViewLineNumberMargin();
			for ( int i = 0; i < itemCount; i++ ) {
				childView.tab.getItemEdit( i )->setViewLineNumberMargin( flag );
			}
			break;
		}
		case ID_VIEW_MARGIN: {
			bool flag = !edit->isViewMargin();
			for ( int i = 0; i < itemCount; i++ ) {
				childView.tab.getItemEdit( i )->setViewMargin( flag );
			}
			break;
		}
		case ID_VIEW_FOLDMARGIN: {
			bool flag = !edit->isViewFoldMargin();
			for ( int i = 0; i < itemCount; i++ ) {
				childView.tab.getItemEdit( i )->setViewFoldMargin( flag );
			}
			break;
		}
		case ID_VIEW_ZOOMIN: {
			for ( int i = 0; i < itemCount; i++ ) {
				childView.tab.getItemEdit( i )->zoomIn();
			}
			break;
		}
		case ID_VIEW_ZOOMOUT: {
			for ( int i = 0; i < itemCount; i++ ) {
				childView.tab.getItemEdit( i )->zoomOut();
			}
			break;
		}
		case ID_VIEW_ZOOMRESET: {
			for ( int i = 0; i < itemCount; i++ ) {
				childView.tab.getItemEdit( i )->resetZoom();
			}
			break;
		}
	}
}

void RDOEditorMainFrame::OnUpdateZoomIn( CCmdUI *pCmdUI )
{
	pCmdUI->Enable( childView.tab.getCurrentEdit()->getZoom() < 20 );
}

void RDOEditorMainFrame::OnUpdateZoomOut( CCmdUI *pCmdUI )
{
	pCmdUI->Enable( childView.tab.getCurrentEdit()->getZoom() > -10 );
}

void RDOEditorMainFrame::OnUpdateZoomReset( CCmdUI *pCmdUI )
{
	pCmdUI->Enable( childView.tab.getCurrentEdit()->getZoom() );
}

void RDOEditorMainFrame::OnOptions()
{
	RDOEditorOptions dlg( childView.tab.getCurrentEdit()->getEditorStyle() );
	dlg.DoModal();
}

void RDOEditorMainFrame::OnHelpContents()
{
	CString filename = rdoEditorApp.getFullHelpFileName();
	if ( filename.IsEmpty() ) return;

	HtmlHelp( ::GetDesktopWindow(), filename, HH_DISPLAY_TOPIC, NULL );
}

void RDOEditorMainFrame::OnUpdateCoordStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	RDOEditorEdit* edit = childView.tab.getCurrentEdit();
	int x = edit->getCurrentColumnNumber() + 1;
	int y = edit->getCurrentLineNumber() + 1;
	CString str;
	str.Format( "%d: %d", x, y );
	pCmdUI->SetText( str );
}

void RDOEditorMainFrame::OnUpdateModifyStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();

	RDOEditorEdit* edit = childView.tab.getCurrentEdit();

	CString s = "";

	if ( edit->isReadOnly() ) {
		s.LoadString( ID_STATUSBAR_READONLY );
	} else if ( edit->isModify() ) {
		s.LoadString( ID_STATUSBAR_MODIFIED );
	}

	pCmdUI->SetText( s );
}

void RDOEditorMainFrame::OnUpdateInsertOverwriteStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();

	if ( childView.tab.getCurrentEdit()->isOverwrite() ) {

		CString s;
		s.LoadString( ID_STATUSBAR_OVERWRITE );
		pCmdUI->SetText( s );

	} else {
		pCmdUI->SetText( "" );
	}
}

void RDOEditorMainFrame::OnUpdateInfoStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	CString s = "";
	switch ( runError ) {
		case RDORE_Syntax : s.LoadString( ID_RDOERROR_SYNTAX ); break;
		case RDORE_Runtime: s.LoadString( ID_RDOERROR_RUNTIME ); break;
		case RDORE_Load   : s.LoadString( ID_RDOERROR_LOAD ); break;
		case RDORE_License: s.LoadString( ID_RDOERROR_LICENSE ); break;
	}
	pCmdUI->SetText( s );
}

#ifdef _DEBUG
void RDOEditorMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void RDOEditorMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif

#include "stdafx.h"
#include "rdopluginmfcmainframe.h"
#include "rdopluginmfc.h"
#include "resource.h"

#include <limits>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOPluginMFCMainFrame
// ----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(RDOPluginMFCMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(RDOPluginMFCMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(RDOPluginMFCMainFrame)
	ON_COMMAND(ID_PLUGIN_CLOSE, OnPluginClose)
	ON_COMMAND(ID_MODEL_BUILD, OnModelBuild)
	ON_COMMAND(ID_MODEL_RUN, OnModelRun)
	ON_COMMAND(ID_MODEL_STOP, OnModelStop)
	ON_UPDATE_COMMAND_UI(ID_MODEL_BUILD, OnUpdateModelBuild)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUN, OnUpdateModelRun)
	ON_UPDATE_COMMAND_UI(ID_MODEL_STOP, OnUpdateModelStop)
	ON_WM_CREATE()
	ON_COMMAND(ID_MODEL_RUN_NOSHOW, OnModelRunNoshow)
	ON_COMMAND(ID_MODEL_RUN_ANIMATION, OnModelRunAnimation)
	ON_COMMAND(ID_MODEL_RUN_MONITOR, OnModelRunMonitor)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUN_NOSHOW, OnUpdateModelRunNoshow)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUN_ANIMATION, OnUpdateModelRunAnimation)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUN_MONITOR, OnUpdateModelRunMonitor)
	ON_COMMAND(ID_MODEL_SHOWRATE_INC, OnModelShowRateInc)
	ON_COMMAND(ID_MODEL_SHOWRATE_INCFOUR, OnModelShowRateIncFour)
	ON_COMMAND(ID_MODEL_SHOWRATE_DECFOUR, OnModelShowRateDecFour)
	ON_COMMAND(ID_MODEL_SHOWRATE_DEC, OnModelShowRateDec)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_INC, OnUpdateModelShowRateInc)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_INCFOUR, OnUpdateModelShowRateIncFour)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_DECFOUR, OnUpdateModelShowRateDecFour)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_DEC, OnUpdateModelShowRateDec)
	ON_COMMAND(ID_MODEL_FRAME_NEXT, OnModelFrameNext)
	ON_COMMAND(ID_MODEL_FRAME_PREV, OnModelFramePrev)
	ON_UPDATE_COMMAND_UI(ID_MODEL_FRAME_NEXT, OnUpdateModelFrameNext)
	ON_UPDATE_COMMAND_UI(ID_MODEL_FRAME_PREV, OnUpdateModelFramePrev)
	ON_COMMAND(ID_MODEL_NEW, OnModelNew)
	ON_COMMAND(ID_MODEL_OPEN, OnModelOpen)
	ON_COMMAND(ID_MODEL_SAVE, OnModelSave)
	ON_COMMAND(ID_MODEL_CLOSE, OnModelClose)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SAVE, OnUpdateModelSave)
	ON_UPDATE_COMMAND_UI(ID_MODEL_CLOSE, OnUpdateModelClose)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOPluginMFCMainFrame::RDOPluginMFCMainFrame():
	CFrameWnd(),
	closed( false )
{
}

RDOPluginMFCMainFrame::~RDOPluginMFCMainFrame()
{
}

BOOL RDOPluginMFCMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) ) return FALSE;
	cs.dwExStyle |= WS_EX_TOPMOST;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass( 0, pluginMFCApp.LoadCursor( IDC_ARROW ), NULL, pluginMFCApp.LoadIcon( IDR_MAINFRAME ) );
	return TRUE;
}

int RDOPluginMFCMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if ( CFrameWnd::OnCreate(lpCreateStruct) == -1 ) return -1;

	SetWindowPos( &CWnd::wndTopMost, 0, 0, 300, 200, SWP_NOMOVE );

	edit.CreateEx( WS_EX_CLIENTEDGE, _T("EDIT"), NULL, WS_HSCROLL | WS_VSCROLL | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN, CRect( 0, 0, 100, 100 ), this, 0 );
	edit.ModifyStyle( WS_BORDER, 0 );
	edit.LimitText();
	edit.SetFont( CFont::FromHandle( (HFONT)::GetStockObject( DEFAULT_GUI_FONT ) ) );
	edit.SetReadOnly();

	return 0;
}

BOOL RDOPluginMFCMainFrame::DestroyWindow()
{
	TRACE( "destroy\n" );
	return CFrameWnd::DestroyWindow();
}

BOOL RDOPluginMFCMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if ( edit.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	return CFrameWnd::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}

void RDOPluginMFCMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize( nType, cx, cy );
	if ( edit.GetSafeHwnd() ) {
		CRect r;
		GetClientRect( r );
		edit.MoveWindow( r );
	}
}

void RDOPluginMFCMainFrame::insertLine( const char* line )
{
	pluginMFCApp.studio.lock( AfxGetInstanceHandle() );
	pluginMFCApp.closeMutex.Lock();
	if ( !closed ) {
		TRACE( "line1\n" );
		int length = edit.GetWindowTextLength();
		edit.SetSel( length, length );
		CString str;
		str.Format( "%d. %s\r\n", edit.GetLineCount(), line );
		edit.ReplaceSel( str );
		TRACE( "line2\n" );
	} else {
		TRACE( "noline\n" );
	}
	pluginMFCApp.closeMutex.Unlock();
	pluginMFCApp.studio.unlock( AfxGetInstanceHandle() );
}

void RDOPluginMFCMainFrame::OnPluginClose()
{
	OnClose();
}

void RDOPluginMFCMainFrame::OnModelBuild()
{
	pluginMFCApp.studio.model.build();
}

void RDOPluginMFCMainFrame::OnModelRun()
{
	pluginMFCApp.studio.model.run();
}

void RDOPluginMFCMainFrame::OnModelStop()
{
	pluginMFCApp.studio.model.stop();
}

void RDOPluginMFCMainFrame::OnUpdateModelBuild( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( pluginMFCApp.studio.model.hasModel() && !pluginMFCApp.studio.model.isRunning() );
}

void RDOPluginMFCMainFrame::OnUpdateModelRun( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( pluginMFCApp.studio.model.hasModel() && !pluginMFCApp.studio.model.isRunning() );
}

void RDOPluginMFCMainFrame::OnUpdateModelStop( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( pluginMFCApp.studio.model.isRunning() );
}

void RDOPluginMFCMainFrame::OnModelRunNoshow() 
{
	pluginMFCApp.studio.model.setShowMode( rdoPlugin::NoShow );
}

void RDOPluginMFCMainFrame::OnModelRunAnimation() 
{
	pluginMFCApp.studio.model.setShowMode( rdoPlugin::Animation );
}

void RDOPluginMFCMainFrame::OnModelRunMonitor() 
{
	pluginMFCApp.studio.model.setShowMode( rdoPlugin::Monitor );
}

void RDOPluginMFCMainFrame::OnUpdateModelRunNoshow(CCmdUI* pCmdUI) 
{
	bool flag = pluginMFCApp.studio.model.isRunning() && pluginMFCApp.studio.frame.isDescribed();
	pCmdUI->Enable( flag );
	pCmdUI->SetCheck( flag ? pluginMFCApp.studio.model.getShowMode() == rdoPlugin::NoShow : 0 );
}

void RDOPluginMFCMainFrame::OnUpdateModelRunAnimation(CCmdUI* pCmdUI) 
{
	bool flag = pluginMFCApp.studio.model.isRunning() && pluginMFCApp.studio.frame.isDescribed();
	pCmdUI->Enable( flag );
	pCmdUI->SetCheck( flag ? pluginMFCApp.studio.model.getShowMode() == rdoPlugin::Animation : 0 );
}

void RDOPluginMFCMainFrame::OnUpdateModelRunMonitor(CCmdUI* pCmdUI) 
{
	bool flag = pluginMFCApp.studio.model.isRunning() && pluginMFCApp.studio.frame.isDescribed();
	pCmdUI->Enable( flag );
	pCmdUI->SetCheck( flag ? pluginMFCApp.studio.model.getShowMode() == rdoPlugin::Monitor : 0 );
}

void RDOPluginMFCMainFrame::OnModelShowRateInc() 
{
	double showRate = pluginMFCApp.studio.frame.getShowRate();
	showRate *= 1.5;
	if ( showRate <= DBL_MAX ) {
		pluginMFCApp.studio.frame.setShowRate( showRate );
	}
}

void RDOPluginMFCMainFrame::OnModelShowRateIncFour() 
{
	double showRate = pluginMFCApp.studio.frame.getShowRate();
	showRate *= 4;
	if ( showRate <= DBL_MAX ) {
		pluginMFCApp.studio.frame.setShowRate( showRate );
	}
}

void RDOPluginMFCMainFrame::OnModelShowRateDecFour() 
{
	double showRate = pluginMFCApp.studio.frame.getShowRate();
	showRate /= 4;
	if ( showRate >= DBL_MIN ) {
		pluginMFCApp.studio.frame.setShowRate( showRate );
	}
}

void RDOPluginMFCMainFrame::OnModelShowRateDec() 
{
	double showRate = pluginMFCApp.studio.frame.getShowRate();
	showRate /= 1.5;
	if ( showRate >= DBL_MIN ) {
		pluginMFCApp.studio.frame.setShowRate( showRate );
	}
}

void RDOPluginMFCMainFrame::OnUpdateModelShowRateInc(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( pluginMFCApp.studio.model.isRunning() && pluginMFCApp.studio.model.getShowMode() != rdoPlugin::NoShow && pluginMFCApp.studio.frame.getShowRate() * 1.5 <= DBL_MAX );
}

void RDOPluginMFCMainFrame::OnUpdateModelShowRateIncFour(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( pluginMFCApp.studio.model.isRunning() && pluginMFCApp.studio.model.getShowMode() != rdoPlugin::NoShow && pluginMFCApp.studio.frame.getShowRate() * 4 <= DBL_MAX );
}

void RDOPluginMFCMainFrame::OnUpdateModelShowRateDecFour(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( pluginMFCApp.studio.model.isRunning() && pluginMFCApp.studio.model.getShowMode() != rdoPlugin::NoShow && pluginMFCApp.studio.frame.getShowRate() / 4 >= DBL_MIN );
}

void RDOPluginMFCMainFrame::OnUpdateModelShowRateDec(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( pluginMFCApp.studio.model.isRunning() && pluginMFCApp.studio.model.getShowMode() != rdoPlugin::NoShow && pluginMFCApp.studio.frame.getShowRate() / 1.5 >= DBL_MIN );
}

void RDOPluginMFCMainFrame::OnModelFrameNext() 
{
	pluginMFCApp.studio.frame.showNext();
}

void RDOPluginMFCMainFrame::OnModelFramePrev() 
{
	pluginMFCApp.studio.frame.showPrev();
}

void RDOPluginMFCMainFrame::OnUpdateModelFrameNext(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( pluginMFCApp.studio.frame.canShowNext() );
}

void RDOPluginMFCMainFrame::OnUpdateModelFramePrev(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( pluginMFCApp.studio.frame.canShowPrev() );
}

void RDOPluginMFCMainFrame::OnModelNew()
{
	pluginMFCApp.studio.model.newModel();
}

void RDOPluginMFCMainFrame::OnModelOpen()
{
	pluginMFCApp.studio.model.openModel( NULL );
}

void RDOPluginMFCMainFrame::OnModelSave()
{
	pluginMFCApp.studio.model.saveModel();
}

void RDOPluginMFCMainFrame::OnModelClose()
{
	pluginMFCApp.studio.model.closeModel();
}

void RDOPluginMFCMainFrame::OnUpdateModelSave(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( pluginMFCApp.studio.model.isModify() );
}

void RDOPluginMFCMainFrame::OnUpdateModelClose(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( pluginMFCApp.studio.model.hasModel() );
}

void RDOPluginMFCMainFrame::OnClose() 
{
	CFrameWnd::OnClose();
}

LRESULT RDOPluginMFCMainFrame::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
	if ( message == WM_CLOSE ) {
		TRACE( "WM_CLOSE\n" );
//		pluginMFCApp.closeMutex.Lock();
		TRACE( "close = true\n" );
		closed = true;
//		pluginMFCApp.closeMutex.Unlock();
	} else if ( message == WM_SYSCOMMAND && wParam == SC_CLOSE ) {
		TRACE( "SC_CLOSE\n" );
		closed = true;
		pluginMFCApp.studio.stopPlugin( AfxGetInstanceHandle() );
	}
	return CFrameWnd::WindowProc( message, wParam, lParam );
}

#include "stdafx.h"
#include "bkemulapp.h"
#include "bkemulmainfrm.h"
#include "bkemul.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------
// ---------- BKEmulApp
// --------------------------------------------------------------
BKEmulApp enulApp;

BEGIN_MESSAGE_MAP(BKEmulApp, CWinApp)
	//{{AFX_MSG_MAP(BKEmulApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BKEmulApp::BKEmulApp()
{
}

BOOL BKEmulApp::InitInstance()
{
#ifdef _AFXDLL
	Enable3dControls();
#else
	Enable3dControlsStatic();
#endif

	free( (void*)m_pszProfileName );
	m_pszProfileName = _tcsdup( _T("") );
	free( (void*)m_pszRegistryKey );
	m_pszRegistryKey = _tcsdup( _T("BKEmulator") );

	mainFrame = new BKMainFrame;
	m_pMainWnd = mainFrame;

	mainFrame->LoadFrame( IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL );
	mainFrame->SetIcon( LoadIcon( MAKEINTRESOURCE(IDR_MAINFRAME) ), TRUE );
	mainFrame->SetIcon( (HICON)::LoadImage( NULL, MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, ::GetSystemMetrics( SM_CXSMICON ), ::GetSystemMetrics( SM_CYSMICON ), LR_DEFAULTCOLOR ), FALSE );
	mainFrame->ShowWindow(SW_SHOW);
	mainFrame->UpdateWindow();

	emul.powerON();

	return TRUE;
}

int BKEmulApp::ExitInstance()
{
	emul.powerOFF();
	return CWinApp::ExitInstance();
}

BOOL BKEmulApp::OnIdle(LONG lCount)
{
	if ( emul.isPowerON() ) {
		emul.nextIteration();
	}
	return CWinApp::OnIdle(lCount);
}

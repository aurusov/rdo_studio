#include "stdafx.h"
#include "rdoprocess_app.h"
#include "rdoprocess_mainfrm.h"
#include "rdoprocess_childfrm.h"
#include "ctrl/rdoprocess_pagectrl.h"
#include "resource.h"
#include "method/process2rdo/rdo_process_project_RDO_proc_MJ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPApp
// ----------------------------------------------------------------------------
RPApp rpapp;

BEGIN_MESSAGE_MAP(RPApp, CWinApp)
	//{{AFX_MSG_MAP(RPApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RPApp::RPApp():
	CWinApp(),
	mainFrame( NULL ),
	_msg( NULL ),
	_project( NULL )
{
	log.open( "log.txt" );
}

BOOL RPApp::InitInstance()
{
	log << "RPApp::InitInstance().." << std::endl;

	_msg = new rp::msg();

	_project = new RPProject_RDO_Proc_MJ();
	project()->setName( "project" );

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	cursors[ IDC_FLOW_SELECT ]        = AfxGetApp()->LoadCursor(IDC_FLOW_SELECT);
	cursors[ IDC_FLOW_MOVE ]          = AfxGetApp()->LoadCursor(IDC_FLOW_MOVE);
	cursors[ IDC_FLOW_CONNECTOR ]     = AfxGetApp()->LoadCursor(IDC_FLOW_CONNECTOR);
	cursors[ IDC_FLOW_ROTATE ]        = AfxGetApp()->LoadCursor(IDC_FLOW_ROTATE);
	cursors[ IDC_FLOW_ROTATE_CENTER ] = AfxGetApp()->LoadCursor(IDC_FLOW_ROTATE_CENTER);
	cursors[ IDC_FLOW_ROTATE_TL ]     = AfxGetApp()->LoadCursor(IDC_FLOW_ROTATE_TL);
//	cursors[ IDC_FLOW_ROTATE_TR ]     = AfxGetApp()->LoadCursor(IDC_FLOW_ROTATE_TL);
//	cursors[ IDC_FLOW_ROTATE_BL ]     = AfxGetApp()->LoadCursor(IDC_FLOW_ROTATE_TL);
//	cursors[ IDC_FLOW_ROTATE_BR ]     = AfxGetApp()->LoadCursor(IDC_FLOW_ROTATE_TL);
	cursors[ IDC_FLOW_SCALE_LR ]      = AfxGetApp()->LoadCursor(IDC_FLOW_SCALE_LR);
	cursors[ IDC_FLOW_SCALE_TB ]      = AfxGetApp()->LoadCursor(IDC_FLOW_SCALE_TB);
	cursors[ IDC_FLOW_SCALE_TLBR ]    = AfxGetApp()->LoadCursor(IDC_FLOW_SCALE_TLBR);
	cursors[ IDC_FLOW_SCALE_TRBL ]    = AfxGetApp()->LoadCursor(IDC_FLOW_SCALE_TRBL);
	cursors[ IDC_FLOW_DOCK_IN ]       = AfxGetApp()->LoadCursor(IDC_FLOW_DOCK_IN);
	cursors[ IDC_FLOW_DOCK_NOT ]      = AfxGetApp()->LoadCursor(IDC_FLOW_DOCK_NOT);

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

	mainFrame  = new RPMainFrame;
	m_pMainWnd = mainFrame;

	// create main MDI frame window
	if ( !mainFrame->LoadFrame(IDR_MAINFRAME) ) return FALSE;

	// try to load shared MDI menus and accelerator table
	//TODO: add additional member variables and load calls for
	//	additional menu types your application may need. 

	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu  = ::LoadMenu( hInst, MAKEINTRESOURCE(IDR_RDO_PRTYPE) );
	m_hMDIAccel = ::LoadAccelerators( hInst, MAKEINTRESOURCE(IDR_RDO_PRTYPE) );

	// The main window has been initialized, so show and update it.
	mainFrame->ShowWindow( m_nCmdShow );
	mainFrame->UpdateWindow();

	log << "RPApp::InitInstance().. ok" << std::endl;
	return TRUE;
}

int RPApp::ExitInstance() 
{
	if ( _project ) {
		delete _project;
		_project = NULL;
	}
	if ( _msg ) {
		delete _msg;
		_msg = NULL;
	}

	if (m_hMDIMenu != NULL)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);

	return CWinApp::ExitInstance();
}

BOOL RPApp::PreTranslateMessage( MSG* pMsg )
{
	if ( pMsg->message == RP_PAGECTRL_DELETEITEM ) {
		mainFrame->projectBar.removePage( reinterpret_cast<RPPageCtrlItem*>(pMsg->wParam) );
	}
	return CWinApp::PreTranslateMessage( pMsg );
}

void RPApp::OnFileNew() 
{
	RPMainFrame* pFrame = STATIC_DOWNCAST(RPMainFrame, m_pMainWnd);

	// create a new MDI child window
	pFrame->CreateNewChild(
		RUNTIME_CLASS(RPChildFrame), IDR_RDO_PRTYPE, m_hMDIMenu, m_hMDIAccel);
}

void RPApp::OnAppAbout()
{
	RPAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// ----------------------------------------------------------------------------
// ---------- RPAboutDlg
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RPAboutDlg, CDialog )
	//{{AFX_MSG_MAP(RPAboutDlg)
	ON_BN_CLICKED(IDC_ABOUT_EMAIL, OnAboutEmail)
	ON_BN_CLICKED(IDC_ABOUT_WEB, OnAboutWeb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RPAboutDlg::RPAboutDlg():
	CDialog( IDD )
{
	//{{AFX_DATA_INIT(RPAboutDlg)
	m_caption = _T("");
	//}}AFX_DATA_INIT
	TCHAR szExeName[ MAX_PATH + 1 ];
	if ( ::GetModuleFileName( NULL, szExeName, MAX_PATH ) ) {
		DWORD dwHnd;
		DWORD size = ::GetFileVersionInfoSize( szExeName, &dwHnd );
		if ( size ) {
			void* pBuffer = malloc( size );
			if ( pBuffer != NULL ) {
				if ( ::GetFileVersionInfo( szExeName, dwHnd, size, pBuffer ) ) {
					DWORD* pTranslation;
					UINT   length;
					if ( ::VerQueryValue( pBuffer, _T("\\VarFileInfo\\Translation"), (void**)&pTranslation, &length ) ) {
						DWORD translation = *pTranslation;
						char key[2000];
						wsprintf( key, _T("\\StringFileInfo\\%04x%04x\\ProductName"), LOWORD( translation ), HIWORD( translation ) );
						char* productName;
						if ( ::VerQueryValue( pBuffer, key, (void**)&productName, &length ) ) {
							VS_FIXEDFILEINFO* fixedInfo;
							if ( ::VerQueryValue( pBuffer, _T("\\"), (void**)&fixedInfo, &length ) ) {
								CString s;
								s.Format( "%s     version %u.%u (build %u)", productName, HIWORD( fixedInfo->dwProductVersionMS ), LOWORD( fixedInfo->dwProductVersionMS ), LOWORD( fixedInfo->dwProductVersionLS ) );
								m_caption = s;
							}
						}
					}
				}
				free( pBuffer );
			}
		}
	}
}

RPAboutDlg::~RPAboutDlg()
{
}

void RPAboutDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RPAboutDlg)
	DDX_Control(pDX, IDC_ABOUT_WEB, m_web);
	DDX_Control(pDX, IDC_ABOUT_EMAIL, m_email);
	DDX_Text(pDX, IDC_ABOUT_CAPTION, m_caption);
	//}}AFX_DATA_MAP
}

void RPAboutDlg::OnAboutEmail() 
{
	CString s;
	m_email.GetWindowText( s );
	::ShellExecute( m_hWnd, "open", "mailto:" + s, 0, 0, SW_SHOWNORMAL );
}

void RPAboutDlg::OnAboutWeb() 
{
	CString s;
	m_web.GetWindowText( s );
	::ShellExecute( m_hWnd, "open", s, 0, 0, SW_SHOWNORMAL );
}

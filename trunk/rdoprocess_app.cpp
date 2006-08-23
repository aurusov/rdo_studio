#include "stdafx.h"
#include "rdoprocess_app.h"
#include "rdoprocess_project.h"
#include "rdoprocess_childfrm.h"
#include "ctrl/rdoprocess_pagectrl.h"
#include <rdoprocess_factory.h>

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
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RPApp::RPApp():
	CWinApp(),
	mainFrame( NULL )
{
	log.open( "log.txt" );

	new RPProjectMFC();

	log << "new factory.." << std::endl;
	new RPObjectFactory();
	rpMethod::factory->registerDefaultObject();
	log << "new factory..ok" << std::endl;
}

BOOL RPApp::InitInstance()
{
	log << "RPApp::InitInstance().." << std::endl;
	
	if ( ::CoInitializeEx( NULL, COINIT_APARTMENTTHREADED ) != S_OK ) {
		::CoUninitialize();
		return false;
	}

	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	rpMethod::project->cursors[ RPProject::cursor_flow_select ]        = AfxGetApp()->LoadCursor(IDC_FLOW_SELECT);
	rpMethod::project->cursors[ RPProject::cursor_flow_move ]          = AfxGetApp()->LoadCursor(IDC_FLOW_MOVE);
	rpMethod::project->cursors[ RPProject::cursor_flow_connector ]     = AfxGetApp()->LoadCursor(IDC_FLOW_CONNECTOR);
	rpMethod::project->cursors[ RPProject::cursor_flow_rotate ]        = AfxGetApp()->LoadCursor(IDC_FLOW_ROTATE);
	rpMethod::project->cursors[ RPProject::cursor_flow_rotate_center ] = AfxGetApp()->LoadCursor(IDC_FLOW_ROTATE_CENTER);
	rpMethod::project->cursors[ RPProject::cursor_flow_rotate_tl ]     = AfxGetApp()->LoadCursor(IDC_FLOW_ROTATE_TL);
	rpMethod::project->cursors[ RPProject::cursor_flow_rotate_tr ]     = AfxGetApp()->LoadCursor(IDC_FLOW_ROTATE_TL);
	rpMethod::project->cursors[ RPProject::cursor_flow_rotate_bl ]     = AfxGetApp()->LoadCursor(IDC_FLOW_ROTATE_TL);
	rpMethod::project->cursors[ RPProject::cursor_flow_rotate_br ]     = AfxGetApp()->LoadCursor(IDC_FLOW_ROTATE_TL);
	rpMethod::project->cursors[ RPProject::cursor_flow_scale_lr ]      = AfxGetApp()->LoadCursor(IDC_FLOW_SCALE_LR);
	rpMethod::project->cursors[ RPProject::cursor_flow_scale_tb ]      = AfxGetApp()->LoadCursor(IDC_FLOW_SCALE_TB);
	rpMethod::project->cursors[ RPProject::cursor_flow_scale_tlbr ]    = AfxGetApp()->LoadCursor(IDC_FLOW_SCALE_TLBR);
	rpMethod::project->cursors[ RPProject::cursor_flow_scale_trbl ]    = AfxGetApp()->LoadCursor(IDC_FLOW_SCALE_TRBL);
	rpMethod::project->cursors[ RPProject::cursor_flow_dock_in ]       = AfxGetApp()->LoadCursor(IDC_FLOW_DOCK_IN);
	rpMethod::project->cursors[ RPProject::cursor_flow_dock_out ]      = AfxGetApp()->LoadCursor(IDC_FLOW_DOCK_IN);
	rpMethod::project->cursors[ RPProject::cursor_flow_dock_inout ]    = AfxGetApp()->LoadCursor(IDC_FLOW_DOCK_IN);
	rpMethod::project->cursors[ RPProject::cursor_flow_dock_fly ]      = AfxGetApp()->LoadCursor(IDC_FLOW_DOCK_IN);
	rpMethod::project->cursors[ RPProject::cursor_flow_dock_not ]      = AfxGetApp()->LoadCursor(IDC_FLOW_DOCK_NOT);

	mainFrame  = new RPMainFrame;
	m_pMainWnd = mainFrame;

	if ( !mainFrame->LoadFrame(IDR_MAINFRAME) ) return FALSE;
	mainFrame->SetIcon( LoadIcon(IDR_MAINFRAME), true );

	// try to load shared MDI menus and accelerator table
	//TODO: add additional member variables and load calls for
	//	additional menu types your application may need. 

	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu  = ::LoadMenu( hInst, MAKEINTRESOURCE(IDR_MAINFRAME) );
	m_hMDIAccel = ::LoadAccelerators( hInst, MAKEINTRESOURCE(IDR_MAINFRAME) );

	mainFrame->ShowWindow( m_nCmdShow );
	mainFrame->UpdateWindow();

	methods.init();
	mainFrame->projectBar.selectFirst();

	log << "RPApp::InitInstance().. ok" << std::endl;
	return TRUE;
}

int RPApp::ExitInstance() 
{
	log << "methods.close().." << std::endl;
	methods.close();
	log << "methods.close().. ok" << std::endl;

	log << "delete factory.. " << std::endl;
	if ( rpMethod::factory ) {
		delete rpMethod::factory;
		rpMethod::factory = NULL;
	}
	log << "delete factory.. ok" << std::endl;

	log << "delete project.." << std::endl;
	if ( rpMethod::project ) {
		delete rpMethod::project;
		rpMethod::project = NULL;
	}
	log << "delete project.. ok" << std::endl;

	if (m_hMDIMenu != NULL)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);

	::CoUninitialize();

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
	CWnd* child = mainFrame->CreateNewChild( RUNTIME_CLASS(RPChildFrame), IDR_RDO_PRTYPE, m_hMDIMenu, m_hMDIAccel );
	child->SetIcon( LoadIcon(IDR_RDO_PRTYPE), true );
}

void RPApp::OnFileOpen()
{
	static_cast<RPProjectMFC*>(rpMethod::project)->open();
}

void RPApp::OnFileSave()
{
	static_cast<RPProjectMFC*>(rpMethod::project)->save();
}

// ----------------------------------------------------------------------------
// ---------- RPAboutDlg
// ----------------------------------------------------------------------------
class RPAboutDlg: public CDialog
{
public:
	RPAboutDlg();
	virtual ~RPAboutDlg();

protected:
	//{{AFX_DATA(RPAboutDlg)
	enum { IDD = IDD_ABOUT };
	RDOLink	m_web;
	RDOLink	m_email;
	CString	m_caption;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(RPAboutDlg)
	protected:
	virtual void DoDataExchange( CDataExchange* pDX );
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RPAboutDlg)
	afx_msg void OnAboutEmail();
	afx_msg void OnAboutWeb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

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

// ----------------------------------------------------------------------------
// ---------- RPApp
// ----------------------------------------------------------------------------
void RPApp::OnAppAbout()
{
	RPAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

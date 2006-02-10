#include "stdafx.h"
#include "rdoprocess_app.h"
#include "rdoprocess_mainfrm.h"
#include "rdoprocess_childfrm.h"
#include "rdoprocess_object.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOPROCApp
// ----------------------------------------------------------------------------
RDOPROCApp rpapp;

BEGIN_MESSAGE_MAP(RDOPROCApp, CWinApp)
	//{{AFX_MSG_MAP(RDOPROCApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOPROCApp::RDOPROCApp()
{
	project.setName( "project" );
}

BOOL RDOPROCApp::InitInstance()
{
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


	cursors[ IDC_FLOW_SELECT ]    = AfxGetApp()->LoadCursor(IDC_FLOW_SELECT);
	cursors[ IDC_FLOW_CONNECTOR ] = AfxGetApp()->LoadCursor(IDC_FLOW_CONNECTOR);
	cursors[ IDC_FLOW_ROTATE ]    = AfxGetApp()->LoadCursor(IDC_FLOW_ROTATE);

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

	CMDIFrameWnd* pFrame = new RDOPROCMainFrame;
	m_pMainWnd = pFrame;

	// create main MDI frame window
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	// try to load shared MDI menus and accelerator table
	//TODO: add additional member variables and load calls for
	//	additional menu types your application may need. 

	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_RDO_PRTYPE));
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_RDO_PRTYPE));

	// The main window has been initialized, so show and update it.
	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();

	return TRUE;
}

int RDOPROCApp::ExitInstance() 
{
	if (m_hMDIMenu != NULL)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);

	return CWinApp::ExitInstance();
}

void RDOPROCApp::OnFileNew() 
{
	RDOPROCMainFrame* pFrame = STATIC_DOWNCAST(RDOPROCMainFrame, m_pMainWnd);

	// create a new MDI child window
	pFrame->CreateNewChild(
		RUNTIME_CLASS(RDOPROCChildFrame), IDR_RDO_PRTYPE, m_hMDIMenu, m_hMDIAccel);
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCApp
// ----------------------------------------------------------------------------
class RDOPROCAboutDlg: public CDialog
{
public:
	RDOPROCAboutDlg();

	//{{AFX_DATA(RDOPROCAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(RDOPROCAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOPROCAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(RDOPROCAboutDlg, CDialog)
	//{{AFX_MSG_MAP(RDOPROCAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOPROCAboutDlg::RDOPROCAboutDlg() : CDialog(RDOPROCAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(RDOPROCAboutDlg)
	//}}AFX_DATA_INIT
}

void RDOPROCAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

void RDOPROCApp::OnAppAbout()
{
	RDOPROCAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void RDOPROCApp::connect( RDOPROCObject* to, UINT message )
{
	connected.insert( Connected::value_type( message, to ) );
}

void RDOPROCApp::sendMessage( RDOPROCObject* from, UINT message, WPARAM wParam, LPARAM lParam )
{
	Connected::iterator it = connected.find( message );
	while ( it != connected.end() ) {
		it->second->notify( from, message, wParam, lParam );
		it++;
	}
}

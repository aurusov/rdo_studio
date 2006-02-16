#include "stdafx.h"
#include "rdoprocess_app.h"
#include "rdoprocess_mainfrm.h"
#include "rdoprocess_childfrm.h"
#include "resource.h"

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
	_msg( NULL ),
	_project( NULL )
{
	log.open( "log.txt" );
}

BOOL RPApp::InitInstance()
{
	log << "RPApp::InitInstance().." << std::endl;

	_msg = new rp::msg();

	_project = new RPProject();
	project().setName( "project" );

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
	cursors[ IDC_FLOW_MOVE ]        = AfxGetApp()->LoadCursor(IDC_FLOW_MOVE);
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

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

	CMDIFrameWnd* pFrame = new RPMainFrame;
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

void RPApp::OnFileNew() 
{
	RPMainFrame* pFrame = STATIC_DOWNCAST(RPMainFrame, m_pMainWnd);

	// create a new MDI child window
	pFrame->CreateNewChild(
		RUNTIME_CLASS(RPChildFrame), IDR_RDO_PRTYPE, m_hMDIMenu, m_hMDIAccel);
}

// ----------------------------------------------------------------------------
// ---------- RPApp
// ----------------------------------------------------------------------------
class RPAboutDlg: public CDialog
{
public:
	RPAboutDlg();

	//{{AFX_DATA(RPAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(RPAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RPAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(RPAboutDlg, CDialog)
	//{{AFX_MSG_MAP(RPAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RPAboutDlg::RPAboutDlg() : CDialog(RPAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(RPAboutDlg)
	//}}AFX_DATA_INIT
}

void RPAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

void RPApp::OnAppAbout()
{
	RPAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

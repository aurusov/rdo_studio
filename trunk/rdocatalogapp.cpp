#include "stdafx.h"
#include "rdocatalogapp.h"
#include "rdocatalogmainfrm.h"
#include "rdocatalogdoc.h"
#include "rdocatalogview.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOCatalogApp
// ----------------------------------------------------------------------------
RDOCatalogApp catalog;

BEGIN_MESSAGE_MAP(RDOCatalogApp, CWinApp)
	//{{AFX_MSG_MAP(RDOCatalogApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

RDOCatalogApp::RDOCatalogApp(): CWinApp()
{
}

RDOCatalogApp::~RDOCatalogApp()
{
}

BOOL RDOCatalogApp::InitInstance()
{
	AfxEnableControlContainer();

#ifdef _AFXDLL
	Enable3dControls();
#else
	Enable3dControlsStatic();
#endif

	free( (void*)m_pszProfileName );
	m_pszProfileName = _tcsdup( _T("") );
	free( (void*)m_pszRegistryKey );
	m_pszRegistryKey = _tcsdup( _T("RAO-catalog") );

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate( IDR_MAINFRAME, RUNTIME_CLASS(RDOCatalogDoc), RUNTIME_CLASS(RDOCatalogMainFrame), RUNTIME_CLASS(RDOCatalogView) );
	AddDocTemplate( pDocTemplate );

	CDocument* doc = OpenDocumentFile( "index.html" );
	if ( !doc ) return FALSE;

	m_pMainWnd->ShowWindow( SW_SHOW );
	m_pMainWnd->UpdateWindow();

//	POSITION pos = doc->GetFirstViewPosition();
//	static_cast<CHtmlView*>(doc->GetNextView( pos ))->SetToolBar( 1 );
//	m_pMainWnd->SetFocus();

	return TRUE;
}

void RDOCatalogApp::OnAppAbout()
{
	RDOAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// ----------------------------------------------------------------------------
// ---------- RDOAboutDlg
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOAboutDlg, CDialog )
	//{{AFX_MSG_MAP(RDOAboutDlg)
	ON_BN_CLICKED(IDC_ABOUT_EMAIL, OnAboutEmail)
	ON_BN_CLICKED(IDC_ABOUT_WEB, OnAboutWeb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOAboutDlg::RDOAboutDlg():
	CDialog( IDD )
{
	//{{AFX_DATA_INIT(RDOAboutDlg)
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

RDOAboutDlg::~RDOAboutDlg()
{
}

void RDOAboutDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RDOAboutDlg)
	DDX_Control(pDX, IDC_ABOUT_WEB, m_web);
	DDX_Control(pDX, IDC_ABOUT_EMAIL, m_email);
	DDX_Text(pDX, IDC_ABOUT_CAPTION, m_caption);
	//}}AFX_DATA_MAP
}

void RDOAboutDlg::OnAboutEmail() 
{
	CString s;
	m_email.GetWindowText( s );
	::ShellExecute( m_hWnd, "open", "mailto:" + s, 0, 0, SW_SHOWNORMAL );
}

void RDOAboutDlg::OnAboutWeb() 
{
	CString s;
	m_web.GetWindowText( s );
	::ShellExecute( m_hWnd, "open", s, 0, 0, SW_SHOWNORMAL );
}

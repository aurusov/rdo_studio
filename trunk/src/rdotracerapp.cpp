#include "stdafx.h"
#include "rdotracerapp.h"
#include "rdoabout.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOTracerApp
// ----------------------------------------------------------------------------
RDOTracerApp rdoTracerApp;

BEGIN_MESSAGE_MAP( RDOTracerApp, CWinApp )
	//{{AFX_MSG_MAP(RDOTracerApp)
	ON_COMMAND( ID_APP_ABOUT, OnAppAbout )
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOTracerApp::RDOTracerApp(): CWinApp()
{
}

RDOTracerApp::~RDOTracerApp()
{
}

BOOL RDOTracerApp::InitInstance()
{
	CWinApp::InitInstance();

	if ( ::OleInitialize( NULL ) != S_OK )
		return FALSE;

	free( (void*)m_pszProfileName );
	m_pszProfileName = _tcsdup( _T("") );
	free( (void*)m_pszRegistryKey );
	m_pszRegistryKey = _tcsdup( _T("RAO-tracer") );

	HFONT hf = (HFONT)::GetStockObject( DEFAULT_GUI_FONT );
	if ( hf ) {
		CFont* f = CFont::FromHandle( hf );
		if ( f ) {
			LOGFONT lf;
			f->GetLogFont( &lf );
			font.CreateFontIndirect( &lf );
		}
	}

	tracer = new rdoTracer::RDOTracer();

	mainFrame = new RDOTracerMainFrame();
	m_pMainWnd = mainFrame;
	mainFrame->LoadFrame( IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL );
	mainFrame->SetIcon( LoadIcon( MAKEINTRESOURCE(IDR_MAINFRAME) ), TRUE );
	//pFrame->SetIcon( LoadIcon( MAKEINTRESOURCE(IDR_MAINFRAME) ), FALSE );
	mainFrame->SetIcon( (HICON)::LoadImage( NULL, MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR ), FALSE );
	mainFrame->ShowWindow( SW_SHOW );
	mainFrame->UpdateWindow();

	return TRUE;
}

int RDOTracerApp::ExitInstance()
{
	::OleUninitialize();
	if( tracer ) {
		delete tracer;
	}
	return CWinApp::ExitInstance();
}

CFont& RDOTracerApp::getFont()
{
	return font;
}

void RDOTracerApp::appendMenu( CMenu* from, const int from_index, CMenu* to )
{
	CString s;
	from->GetMenuString( from_index, s, MF_BYPOSITION );

	CMenu* m_from = from->GetSubMenu( from_index );
	int item_count = 0;
	if ( m_from ) item_count = m_from->GetMenuItemCount();

	if ( item_count ) {

		CMenu* m_to = new CMenu();
		m_to->CreateMenu();
		to->AppendMenu( MF_STRING | MF_POPUP, (int)m_to->m_hMenu, s );

		for ( int i = 0; i < item_count; i++ ) {
			appendMenu( m_from, i, m_to );
		}

	} else {
		UINT itemID = from->GetMenuItemID( from_index );
		if ( itemID ) {
			to->AppendMenu( MF_STRING, itemID, s );
		} else {
			to->AppendMenu( MF_SEPARATOR );
		}
	}
}

void RDOTracerApp::eraseMenu( CMenu* from, const int from_index )
{
	CMenu* m_from;
	if ( from_index != -1 )	{
		m_from = from->GetSubMenu( from_index );
	} else {
		m_from = from;
	}
	int item_count = 0;
	if ( m_from ) item_count = m_from->GetMenuItemCount();

	if ( item_count ) {

		for ( int i = item_count-1; i >= 0; i-- ) {
			eraseMenu( m_from, i );
		}

		if ( from_index != -1 ) delete m_from;

	}

	if ( from_index != -1 ) from->DeleteMenu( from_index, MF_BYPOSITION );
}

void RDOTracerApp::OnAppAbout()
{
	RDOAbout aboutDlg;

	aboutDlg.hPixmap = LoadIcon( MAKEINTRESOURCE(IDR_MAINFRAME) );
	
	aboutDlg.bgPixmap  = RGB( 0x9D, 0xC0, 0xD2 );
	aboutDlg.fgCaption = RGB( 0xF2, 0xF4, 0xF4 );
	aboutDlg.bgCaption = RGB( 0x1C, 0x5F, 0x81 );
	aboutDlg.fgInfo    = RGB( 0x15, 0x47, 0x60 );
	aboutDlg.bgInfo    = RGB( 0xE7, 0xF8, 0xF8 );
	
	aboutDlg.DoModal();
}

string RDOTracerApp::extractFilePath( const string& fileName )
{
	string s;
	string::size_type pos = fileName.find_last_of( '\\' );
	if ( pos == string::npos ) {
		pos = fileName.find_last_of( '/' );
	}
	if ( pos != string::npos && pos < fileName.length() - 1 ) {
		s.assign( fileName.begin(), pos + 1 );
		static char szDelims[] = " \t\n\r";
		s.erase( 0, s.find_first_not_of( szDelims ) );
		s.erase( s.find_last_not_of( szDelims ) + 1, string::npos );
	} else {
		s = fileName;
	}
	pos = s.find_last_of( '\\' );
	if ( pos == string::npos ) {
		pos = s.find_last_of( '/' );
	}
	if ( pos != s.length() - 1 && s.length() ) {
		s += "/";
	}
	return s;
}

bool RDOTracerApp::isFileExists( const string& fileName )
{
	CFileFind finder;
	return finder.FindFile( fileName.c_str() ) ? true : false;
}

string RDOTracerApp::getFullFileName()
{
	string fileName = "";
	TCHAR szExeName[ MAX_PATH + 1 ];
	if ( ::GetModuleFileName( NULL, szExeName, MAX_PATH ) ) {
		fileName = szExeName;
	}
	return fileName;
}

string RDOTracerApp::getFullHelpFileName( string str )
{
	str.insert( 0, extractFilePath( getFullFileName() ) );
	
	if ( !isFileExists( str ) ) {
		::MessageBox( NULL, format( ID_MSG_NO_HELP_FILE, str.c_str() ).c_str(), NULL, MB_ICONEXCLAMATION | MB_OK );
		return "";
	}

	return str;
}
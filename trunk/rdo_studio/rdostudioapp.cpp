#include "stdafx.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "rdostudiochildfrm.h"
#include "rdostudiomodel.h"
#include "rdostudioeditdoc.h"
#include "rdostudioeditview.h"
#include "resource.h"
#include "./rdo_tracer/rdotracertrace.h"
#include "htmlhelp.h"

#include <rdokernel.h>
#include <rdorepository.h>
#include <rdoabout.h>

using namespace std;
using namespace rdoRepository;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOFileAssociationDlg
// ----------------------------------------------------------------------------
class RDOFileAssociationDlg: public CDialog
{
protected:
	virtual void DoDataExchange( CDataExchange* pDX );
	virtual void OnCancel();

public:
	RDOFileAssociationDlg( CWnd* pParentWnd = NULL );
	virtual ~RDOFileAssociationDlg();

	int checkInFuture;
};

RDOFileAssociationDlg::RDOFileAssociationDlg( CWnd* pParentWnd ):
	CDialog( IDD_FILEASSOCIATION, pParentWnd ),
	checkInFuture( true )
{
}

RDOFileAssociationDlg::~RDOFileAssociationDlg()
{
}

void RDOFileAssociationDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange( pDX );

	DDX_Check( pDX, IDC_FILEASSOCIATION_CHECK, checkInFuture );
}

void RDOFileAssociationDlg::OnCancel()
{
	CDialog::UpdateData( true );
	CDialog::OnCancel();
}

// ----------------------------------------------------------------------------
// ---------- RDOStudioApp
// ----------------------------------------------------------------------------
RDOStudioApp studioApp;

BEGIN_MESSAGE_MAP(RDOStudioApp, CWinApp)
	//{{AFX_MSG_MAP(RDOStudioApp)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_MODEL_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_MODEL_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_FILE_SAVE_ALL, OnFileSaveAll)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_ALL, OnUpdateFileSaveAll)
	ON_COMMAND(ID_FILE_MODEL_CLOSE, OnFileClose)
	ON_COMMAND(ID_FILE_MODEL_SAVE_AS, OnFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_MODEL_CLOSE, OnUpdateFileClose)
	ON_UPDATE_COMMAND_UI(ID_FILE_MODEL_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_MODEL_BUILD, OnModelBuild)
	ON_COMMAND(ID_MODEL_RUN, OnModelRun)
	ON_COMMAND(ID_MODEL_STOP, OnModelStop)
	ON_UPDATE_COMMAND_UI(ID_MODEL_BUILD, OnUpdateModelBuild)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUN, OnUpdateModelRun)
	ON_UPDATE_COMMAND_UI(ID_MODEL_STOP, OnUpdateModelStop)
	ON_COMMAND(ID_CHART_STARTTRACE, OnChartStarttrace)
	ON_UPDATE_COMMAND_UI(ID_CHART_STARTTRACE, OnUpdateChartStarttrace)
	ON_COMMAND(ID_CHART_STOPTRACE, OnChartStoptrace)
	ON_UPDATE_COMMAND_UI(ID_CHART_STOPTRACE, OnUpdateChartStoptrace)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE( ID_FILE_REOPEN_1, ID_FILE_REOPEN_10, OnProjectReopen )
	ON_THREAD_MESSAGE( RDO_ADDNEWFRAME_MSG, OnAddNewFrame )
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

RDOStudioApp::RDOStudioApp():
	CWinApp(),
	initInstance( false ),
	editDocTemplate( NULL )
{
}

BOOL RDOStudioApp::InitInstance()
{
	CWinApp::InitInstance();
	
	if ( ::OleInitialize( NULL ) != S_OK )
		return FALSE;

#ifdef _AFXDLL
	Enable3dControls();
#else
	Enable3dControlsStatic();
#endif

	free( (void*)m_pszProfileName );
	m_pszProfileName = _tcsdup( _T("") );
	free( (void*)m_pszRegistryKey );
	m_pszRegistryKey = _tcsdup( _T("RAO-studio") );

	editDocTemplate = new CMultiDocTemplate( IDR_EDITTYPE, RUNTIME_CLASS(RDOStudioEditDoc), RUNTIME_CLASS(RDOStudioChildFrame), RUNTIME_CLASS(RDOStudioEditView) );
	AddDocTemplate( editDocTemplate );

	AddDocTemplate( tracer.createDocTemplate() );

	mainFrame = new RDOStudioMainFrame;
	if ( !mainFrame->LoadFrame( IDR_MAINFRAME ) ) return FALSE;
	m_pMainWnd = mainFrame;

	loadReopen();
	updateReopenSubMenu();

	setupFileAssociation();

	mainFrame->ShowWindow(m_nCmdShow);
	mainFrame->UpdateWindow();

	string fileName( m_lpCmdLine );
	if ( !fileName.empty() ) {
		int pos = fileName.find_first_of( '"' );
		if ( pos == 0 ) {
			fileName.erase( 0, 1 );
		}
		pos = fileName.find_last_of( '"' );
		if ( pos == fileName.length() - 1 ) {
			fileName.erase( pos, 1 );
		}
		if ( model->openModel( fileName ) ) {
			insertReopenItem( kernel.getRepository()->getFullName() );
		} else {
			OnFileNew();
		}
	} else {
		OnFileNew();
	}

	initInstance = true;

	return TRUE;
}

int RDOStudioApp::ExitInstance()
{
	HtmlHelp( NULL, NULL, HH_CLOSE_ALL, 0 );

	return CWinApp::ExitInstance();
}

void RDOStudioApp::OnFileNew() 
{
	model->newModel( initInstance );
}

void RDOStudioApp::OnFileOpen() 
{
	if ( model->openModel() ) {
		insertReopenItem( kernel.getRepository()->getFullName() );
	}
}

void RDOStudioApp::OnFileClose() 
{
	model->closeModel();
}

void RDOStudioApp::OnFileSave() 
{
	model->saveModel();
}

void RDOStudioApp::OnFileSaveAs() 
{
	model->saveAsModel();
}

void RDOStudioApp::OnFileSaveAll() 
{
	model->saveModel();
}

void RDOStudioApp::OnUpdateFileClose(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( model->getModelDoc() ? true : false );
}

void RDOStudioApp::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	model->updateModify();

	POSITION pos = editDocTemplate->GetFirstDocPosition();
	while ( pos ) {
		RDOStudioEditDoc* doc = static_cast<RDOStudioEditDoc*>(editDocTemplate->GetNextDoc( pos ));
		if ( doc ) {
			doc->updateModify();
		}
	}
	bool flag = false;
	CMDIChildWnd* mdi = mainFrame->MDIGetActive();
	if ( mdi ) {
		CDocument* doc = mdi->GetActiveDocument();
		if ( doc ) {
			flag = doc->IsModified() ? true : false;
		}
	}
	pCmdUI->Enable( flag );
}

void RDOStudioApp::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( model->getModelDoc() ? true : false );
}

void RDOStudioApp::OnUpdateFileSaveAll(CCmdUI* pCmdUI) 
{
	bool flag = model->isModify();
	if ( !flag ) {
		POSITION pos = editDocTemplate->GetFirstDocPosition();
		while ( pos ) {
			RDOStudioEditDoc* doc = static_cast<RDOStudioEditDoc*>(editDocTemplate->GetNextDoc( pos ));
			if ( doc ) {
				doc->updateModify();
				flag = doc->IsModified() ? true : false;
				if ( flag ) break;
			}
		}
	}
	pCmdUI->Enable( flag );
}

void RDOStudioApp::OnProjectReopen( UINT nID )
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
	if ( model->openModel( reopenList[i] ) ) {
		insertReopenItem( kernel.getRepository()->getFullName() );
	} else {
		string item = reopenList[i];
		for ( vector< string >::iterator it = reopenList.begin(); it != reopenList.end(); it++ ) {
			if ( *it == item ) {
				reopenList.erase( it );
				break;
			}
		}
		updateReopenSubMenu();
	}
}

void RDOStudioApp::insertReopenItem( const string& item )
{
	if ( !item.empty() ) {

		for ( vector< string >::iterator it = reopenList.begin(); it != reopenList.end(); it++ ) {
			if ( *it == item ) {
				reopenList.erase( it );
				break;
			}
		}

		reopenList.insert( reopenList.begin(), item );

		while ( reopenList.size() > 10 ) {
			vector< string >::iterator it = reopenList.end();
			it--;
			reopenList.erase( it );
		}

		updateReopenSubMenu();
	}
}

void RDOStudioApp::updateReopenSubMenu() const
{
	BOOL maximized;
	mainFrame->MDIGetActive( &maximized );
	int delta = maximized ? 1 : 0;

	CMenu* reopenMenu = AfxGetMainWnd()->GetMenu()->GetSubMenu( delta )->GetSubMenu( 2 );

	while ( reopenMenu->GetMenuItemCount() ) {
		reopenMenu->DeleteMenu( 0, MF_BYPOSITION );
	}

	if ( !reopenList.empty() ) {
		AfxGetMainWnd()->GetMenu()->GetSubMenu( delta )->EnableMenuItem( 2, MF_BYPOSITION | MF_ENABLED );
		for ( vector< string >::size_type i = 0; i < reopenList.size(); i++ ) {
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
			reopenMenu->AppendMenu( MF_STRING, id, format( "%d. %s", i+1, reopenList[i].c_str() ).c_str() );
		}
	} else {
		AfxGetMainWnd()->GetMenu()->GetSubMenu( delta )->EnableMenuItem( 2, MF_BYPOSITION | MF_DISABLED | MF_GRAYED );
	}

	saveReopen();
}

void RDOStudioApp::loadReopen()
{
	reopenList.clear();
	for ( int i = 0; i < 10; i++ ) {
		string sec;
		if ( i+1 < 10 ) {
			sec = format( "0%d", i+1 );
		} else {
			sec = format( "%d", i+1 );
		}
		TRY {
			string s = AfxGetApp()->GetProfileString( "reopen", sec.c_str(), "" );
			if ( !s.empty() ) {
				reopenList.insert( reopenList.end(), s );
			}
		} CATCH( CException, e ) {
		}
		END_CATCH
	}
}

void RDOStudioApp::saveReopen() const
{
	for ( vector< string >::size_type i = 0; i < 10; i++ ) {
		string sec;
		if ( i+1 < 10 ) {
			sec = format( "0%d", i+1 );
		} else {
			sec = format( "%d", i+1 );
		}
		string s;
		if ( i < reopenList.size() ) {
			s = reopenList[i];
		} else {
			s = "";
		}
		TRY {
			AfxGetApp()->WriteProfileString( "reopen", sec.c_str(), s.c_str() );
		} CATCH( CException, e ) {
		}
		END_CATCH
	}
}

void RDOStudioApp::OnModelBuild() 
{
	model->buildModel();
}

void RDOStudioApp::OnModelRun() 
{
	model->runModel();
}

void RDOStudioApp::OnModelStop() 
{
	model->stopModel();
}

void RDOStudioApp::OnUpdateModelBuild(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( model && model->getModelDoc() && !model->isRunning() );
}

void RDOStudioApp::OnUpdateModelRun(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( model && model->getModelDoc() && !model->isRunning() );
}

void RDOStudioApp::OnUpdateModelStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( model && model->getModelDoc() && model->isRunning() );
}

void RDOStudioApp::OnChartStarttrace() 
{
	tracer.startTrace();
}

void RDOStudioApp::OnUpdateChartStarttrace(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( !tracer.isTracing() );
}

void RDOStudioApp::OnChartStoptrace() 
{
	tracer.stopTrace();
}

void RDOStudioApp::OnUpdateChartStoptrace(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( tracer.isTracing() );
}

string RDOStudioApp::getFullFileName()
{
	string fileName = "";
	TCHAR szExeName[ MAX_PATH + 1 ];
	if ( ::GetModuleFileName( NULL, szExeName, MAX_PATH ) ) {
		fileName = szExeName;
	}
	return fileName;
}

string RDOStudioApp::extractFilePath( const string& fileName )
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

bool RDOStudioApp::isFileExists( const string& fileName )
{
	CFileFind finder;
	return finder.FindFile( fileName.c_str() ) ? true : false;
}

string RDOStudioApp::getFullHelpFileName( string str )
{
	str.insert( 0, extractFilePath( getFullFileName() ) );
	
	if ( !isFileExists( str ) ) {
		::MessageBox( NULL, format( ID_MSG_NO_HELP_FILE, str.c_str() ).c_str(), NULL, MB_ICONEXCLAMATION | MB_OK );
		return "";
	}

	return str;
}

void RDOStudioApp::setupFileAssociation()
{
	if ( !GetProfileInt( "fileAssociation", "checkInFuture", true ) ) return;

	string strFileTypeId   = _T("RAO.FileInfo");
	string strFileTypeName = _T("RAO FileInfo");
	string strParam        = _T(" \"%1\"");
	string strPathName     = getFullFileName();
	string strRAOExt       = _T(".smr");

	bool win2000 = false;
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if ( ::GetVersionEx( &osvi ) ) {
		win2000 = osvi.dwMajorVersion >= 5;
	}

	HKEY hCurUserSoftClasses = 0;
	HKEY hKey;
	DWORD result;
	if ( win2000 ) {
		if ( ::RegOpenKeyEx( HKEY_CURRENT_USER, _T("Software\\Classes"), 0, KEY_ALL_ACCESS, &hCurUserSoftClasses ) != ERROR_SUCCESS ) {
			hCurUserSoftClasses = 0;
		}
	} else {
		hCurUserSoftClasses = HKEY_CLASSES_ROOT;
	}
	if ( hCurUserSoftClasses ) {
		if ( ::RegCreateKeyEx( hCurUserSoftClasses, strFileTypeId.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &result ) == ERROR_SUCCESS ) {
			bool mustBeRegistered = true;
			DWORD size;
			if ( ::RegQueryValueEx( hKey, _T(""), NULL, NULL, NULL, &size ) == ERROR_SUCCESS ) {
				if ( size > 0 ) {
					HKEY hKeyPath;
					if ( ::RegOpenKeyEx( hKey, _T("shell\\open\\command"), 0, KEY_READ, &hKeyPath ) == ERROR_SUCCESS ) {
						CString s;
						DWORD length = MAX_PATH + strParam.length() * sizeof(TCHAR) + 1;
						::RegQueryValueEx( hKeyPath, _T(""), NULL, NULL, (LPBYTE)s.GetBuffer( length ), &length );
						s.ReleaseBuffer();
						::RegCloseKey( hKeyPath );

						int pos = s.Find( strParam.c_str() );
						if ( pos != -1 ) {
							s.Delete( pos, strParam.length() );
							if ( s != strPathName.c_str() ) {
								RDOFileAssociationDlg dlg;
								mustBeRegistered = dlg.DoModal() == IDOK;
								WriteProfileInt( "fileAssociation", "checkInFuture", dlg.checkInFuture ? true : false );
							} else {
								mustBeRegistered = false;
							}
						}
					}
				}
			}
			if ( mustBeRegistered ) {
				HKEY hKey_tmp;
				if ( ::RegCreateKeyEx( hCurUserSoftClasses, strFileTypeId.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result ) == ERROR_SUCCESS ) {
					string s = strFileTypeName;
					::RegSetValueEx( hKey_tmp, _T(""), 0, REG_SZ, (LPBYTE)s.c_str(), s.length() );
					::RegCloseKey( hKey_tmp );
				}
				if ( ::RegCreateKeyEx( hCurUserSoftClasses, string(strFileTypeId + _T("\\DefaultIcon")).c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result ) == ERROR_SUCCESS ) {
					string s = strPathName + _T(",0");
					::RegSetValueEx( hKey_tmp, _T(""), 0, REG_SZ, (LPBYTE)s.c_str(), s.length() );
					::RegCloseKey( hKey_tmp );
				}
				if ( ::RegCreateKeyEx( hCurUserSoftClasses, string(strFileTypeId + _T("\\shell\\open\\command")).c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result ) == ERROR_SUCCESS ) {
					string s = strPathName + strParam;
					::RegSetValueEx( hKey_tmp, _T(""), 0, REG_SZ, (LPBYTE)s.c_str(), s.length() );
					::RegCloseKey( hKey_tmp );
				}
				if ( ::RegCreateKeyEx( hCurUserSoftClasses, strRAOExt.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result ) == ERROR_SUCCESS ) {
					string s = strFileTypeId;
					::RegSetValueEx( hKey_tmp, _T(""), 0, REG_SZ, (LPBYTE)s.c_str(), s.length() );
					::RegCloseKey( hKey_tmp );
				}
				if ( ::RegCreateKeyEx( hCurUserSoftClasses, strRAOExt.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result ) == ERROR_SUCCESS ) {
					string s = strFileTypeId;
					::RegSetValueEx( hKey_tmp, _T(""), 0, REG_SZ, (LPBYTE)s.c_str(), s.length() );
					::RegCloseKey( hKey_tmp );
				}
				if ( ::RegCreateKeyEx( hCurUserSoftClasses, string(strRAOExt + _T("\\ShellNew")).c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result ) == ERROR_SUCCESS ) {
					string s = "";
					::RegSetValueEx( hKey_tmp, _T("NullFile"), 0, REG_SZ, (LPBYTE)s.c_str(), s.length() );
					::RegCloseKey( hKey_tmp );
				}
			}
			::RegCloseKey( hKey );
		}
		if ( hCurUserSoftClasses != HKEY_CLASSES_ROOT ) {
			::RegCloseKey( hCurUserSoftClasses );
		}
	}
}

void RDOStudioApp::OnAppAbout()
{
	RDOAbout aboutDlg;
	aboutDlg.BMSTU.LoadString( ID_ABOUT_BMSTU );
	aboutDlg.tel.LoadString( ID_ABOUT_TEL );
	aboutDlg.buttonOkText.LoadString( ID_ABOUT_OKBUTTON );
	aboutDlg.hPixmap = LoadIcon( MAKEINTRESOURCE(IDR_MAINFRAME) );
	aboutDlg.changeColor( 1, 20, 4 );
	aboutDlg.DoModal();
}

void RDOStudioApp::OnAddNewFrame( WPARAM /*wParam*/, LPARAM /*lParam*/ )
{
	model->addNewFrame();
}

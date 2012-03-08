/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      application.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <gdiplus.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdofile.h"
#include "kernel/rdothread.h"
#include "repository/rdorepository.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/src/child_frm.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/src/edit/document.h"
#include "app/rdo_studio_mfc/src/edit/view.h"
#include "app/rdo_studio_mfc/src/plugins.h"
#include "app/rdo_studio_mfc/src/thread.h"
#include "app/rdo_studio_mfc/src/model/new.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracer.h"
#include "app/rdo_studio_mfc/htmlhelp.h"
#include "app/rdo_studio_mfc/plugins/common/rdoplugin.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_factory.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_method.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_project.h"
#include "thirdparty/win_registry/registry.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOFileAssociationDlg
// --------------------------------------------------------------------------------
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

// -----------------------------------------------------------------
// -------------------- RDOStudioCommandLineInfo
// -----------------------------------------------------------------
class RDOStudioCommandLineInfo: public CCommandLineInfo
{
protected:
	bool plugin_start_found;
	bool plugin_exit_found;
	virtual void ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast );

public:
	RDOStudioCommandLineInfo():
		CCommandLineInfo(),
		plugin_start_found( false ),
		plugin_exit_found( false )
	{
	}
};

void RDOStudioCommandLineInfo::ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast )
{
	CCommandLineInfo::ParseParam( lpszParam, bFlag, bLast );
	if ( bFlag ) {
		plugin_start_found = false;
		plugin_exit_found  = false;
		if ( CString( lpszParam ).CompareNoCase( "autorun" ) == 0 ) {
			studioApp.autoRun = true;
		} else if ( CString( lpszParam ).CompareNoCase( "autoexit" ) == 0 ) {
			studioApp.autoExitByModel = true;
		} else if ( CString( lpszParam ).CompareNoCase( "dont_close_if_error" ) == 0 ) {
			studioApp.dontCloseIfError = true;
		} else if ( CString( lpszParam ).CompareNoCase( "pluginstart" ) == 0 ) {
			plugin_start_found = true;
		} else if ( CString( lpszParam ).CompareNoCase( "pluginautoexit" ) == 0 ) {
			plugin_exit_found = true;
		}
	} else {
		if ( plugin_start_found ) {
			studioApp.plugin_start_name.push_back( lpszParam );
		} else if ( plugin_exit_found ) {
			studioApp.plugin_exit_name.push_back( lpszParam );
		} else if ( studioApp.openModelName.empty() ) {
			studioApp.openModelName = lpszParam;
		}
		plugin_start_found = false;
		plugin_exit_found  = false;
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioApp
// --------------------------------------------------------------------------------
RDOStudioApp studioApp;

// ON_UPDATE_COMMAND_UI сделано

BEGIN_MESSAGE_MAP(RDOStudioApp, CWinApp)
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
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_COMMAND_RANGE( ID_FILE_REOPEN_1, ID_FILE_REOPEN_10, OnProjectReopen )
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

RDOStudioApp::RDOStudioApp():
	CWinApp(),
	studioGUI( NULL ),
#ifdef RDO_MT
	studioMT( NULL ),
#endif
	editDocTemplate( NULL ),
	fileAssociationSetup( false ),
	fileAssociationCheckInFuture( false ),
	openLastProject( false ),
	lastProjectName( "" ),
	showCaptionFullName( false ),
	autoRun( false ),
	autoExitByModel( false ),
	dontCloseIfError( false ),
	exitCode( rdoSimulator::EC_OK ),
	openModelName( "" ),
	mainFrame( NULL )
{
	setlocale( LC_ALL, "rus" );
	setlocale( LC_NUMERIC, "eng" );

	log.open( "log.txt" );
#ifdef PROCGUI_ENABLE
	new RPProjectMFC();
	new RPObjectFactory();
	rpMethod::factory->registerDefaultObject();
#endif
}

BOOL RDOStudioApp::InitInstance()
{
	CWinApp::InitInstance();

	if ( ::OleInitialize( NULL ) != S_OK )
		return FALSE;

	free( (void*)m_pszProfileName );
	m_pszProfileName = _tcsdup( _T("") );
	free( (void*)m_pszRegistryKey );
	m_pszRegistryKey = _tcsdup( _T("RAO-studio") );

	fileAssociationSetup         = GetProfileInt( "fileAssociation", "setup", true ) ? true : false;
	fileAssociationCheckInFuture = GetProfileInt( "fileAssociation", "checkInFuture", true ) ? true : false;
	openLastProject              = GetProfileInt( "general", "openLastProject", true ) ? true : false;
	lastProjectName              = GetProfileString( "general", "lastProject", "" );
	showCaptionFullName          = GetProfileInt( "general", "showCaptionFullName", false ) ? true : false;

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	editDocTemplate = new CMultiDocTemplate( IDR_EDIT_TYPE, RUNTIME_CLASS(RDOStudioEditDoc), RUNTIME_CLASS(RDOStudioChildFrame), RUNTIME_CLASS(RDOStudioEditView) );
	AddDocTemplate( editDocTemplate );

	// Кто-то должен поднять кернел и треды
	RDOKernel::init();
#ifdef RDO_MT
	studioGUI = new RDOThreadStudioGUI();
#else
	studioGUI = kernel;
#endif
	new rdoSimulator::RDOThreadSimulator();
	new rdoSimulator::RDOThreadCodeComp();
	new rdoRepository::RDOThreadRepository();
	



//#define CORBA_ENABLE
#ifdef CORBA_ENABLE
	new rdoCorba::RDOThreadCorba();
#endif




#ifdef RDO_MT
	studioMT = new RDOThreadStudio();
#endif
//	new RDOThreadStudio1();
//	new RDOThreadStudio2();

	tracer = new rdoTracer::RDOTracer();
	AddDocTemplate( tracer->createDocTemplate() );
#ifdef PROCGUI_ENABLE
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
	rpMethod::project->cursors[ RPProject::cursor_flow_trash ]         = AfxGetApp()->LoadCursor(IDC_FLOW_TRASH);
#endif
	// Внутри создается объект модели
	mainFrame = new RDOStudioMainFrame;
	m_pMainWnd = mainFrame;
	if ( !mainFrame->LoadFrame( IDR_MAINFRAME ) ) return FALSE;

#ifdef RDO_MT
	kernel->thread_studio = studioGUI;
#else
	kernel->thread_studio = model;
#endif

	loadReopen();
	updateReopenSubMenu();

	if ( getFileAssociationCheckInFuture() ) {
		setupFileAssociation();
	}

	mainFrame->ShowWindow( m_nCmdShow );
	mainFrame->UpdateWindow();
#ifdef PROCGUI_ENABLE
	methods.init();
	mainFrame->workspace.pagectrl->selectFirst();
#endif
	RDOStudioCommandLineInfo cmdInfo;
	ParseCommandLine( cmdInfo );

	// Создаем менеджер плагинов после создания главного окна и парсера командной строки
	plugins = new RDOStudioPlugins;

	bool newModel  = true;
	bool autoModel = false;
	if ( !openModelName.empty() ) {
		if ( !rdo::extractFilePath( openModelName ).empty() ) {
			tstring longFileName;
			if ( shortToLongPath( openModelName, longFileName ) ) {
				openModelName = longFileName;
			}
			if ( model->openModel( openModelName ) ) {
				autoModel = true;
			}
		} else {
			openModelName = rdo::extractFilePath( RDOStudioApp::getFullFileName() ) + openModelName;
			if ( rdo::File::exist( openModelName ) && model->openModel( openModelName ) ) {
				autoRun          = true;
				autoExitByModel  = true;
				autoModel        = true;
				dontCloseIfError = true;
			} else {
				exitCode = rdoSimulator::EC_ModelNotFound;
				return false;
			}
		}
	} else {
		if ( getOpenLastProject() && !getLastProjectName().empty() && rdo::File::exist( getLastProjectName() ) ) {
			model->openModel( getLastProjectName() );
			newModel = false;
		}
	}
	if ( autoModel ) {
		newModel = false;
	} else {
		autoRun          = false;
		autoExitByModel  = false;
		dontCloseIfError = false;
	}
	if ( newModel ) {
//		OnFileNew();
	}

	if ( autoRun ) {
		OnModelRun();
	}

	return TRUE;
}

bool RDOStudioApp::shortToLongPath( CREF(tstring) shortPath, tstring& longPath )
{
	USES_CONVERSION;

	LPSHELLFOLDER psfDesktop = NULL;
	ULONG         chEaten = 0;
	LPITEMIDLIST  pidlShellItem = NULL;
	WCHAR         szLongPath[_MAX_PATH] = { 0 };

	// Get the Desktop's shell folder interface
	HRESULT hr = ::SHGetDesktopFolder( &psfDesktop );

	LPWSTR  lpwShortPath = A2W( shortPath.c_str() );

	// Request an ID list (relative to the desktop) for the short pathname 
	hr = psfDesktop->ParseDisplayName( NULL, NULL, lpwShortPath, &chEaten, &pidlShellItem, NULL );
	psfDesktop->Release(); // Release the desktop's IShellFolder

	if ( FAILED( hr ) ) {
		// If we couldn't get an ID list for short pathname, it must not exist.
		longPath.empty();
		return false;
	} else { 
		// We did get an ID list, convert it to a long pathname
		::SHGetPathFromIDListW( pidlShellItem, szLongPath );
		// Free the ID list allocated by ParseDisplayName
		LPMALLOC pMalloc = NULL;
		::SHGetMalloc( &pMalloc );
		pMalloc->Free( pidlShellItem );
		pMalloc->Release();
		longPath = W2A( szLongPath );
		return true;
	}
}

int RDOStudioApp::ExitInstance()
{
#ifdef PROCGUI_ENABLE
	methods.close();

	if ( rpMethod::factory ) {
		delete rpMethod::factory;
		rpMethod::factory = NULL;
	}

	if ( rpMethod::project ) {
		delete rpMethod::project;
		rpMethod::project = NULL;
	}
#endif
	if ( exitCode != rdoSimulator::EC_ModelNotFound ) {
		exitCode = model->getExitCode();
	}
#ifdef RDO_MT
	if ( studioGUI ) {
		studioGUI->sendMessage( studioGUI, RDOThread::RT_THREAD_CLOSE );
		delete static_cast<RDOThreadStudioGUI*>(studioGUI);
		studioGUI = NULL;
	}
#endif

	if ( plugins ) { delete plugins; plugins = NULL; }

	// Роняем кернел и закрываем все треды
	RDOKernel::close();

	::HtmlHelp( NULL, NULL, HH_CLOSE_ALL, 0 );

	Gdiplus::GdiplusShutdown(m_gdiplusToken);

	if ( autoExitByModel || !plugin_exit_name.empty() ) {
		CWinApp::ExitInstance();
		return exitCode;
	} else {
		return CWinApp::ExitInstance();
	}
}

void RDOStudioApp::OnFileNew() 
{
	RDOStudioModelNew dlg;
	if (dlg.DoModal() == IDOK)
	{
		model->newModel(dlg.getModelName(), dlg.getModelPath() + dlg.getModelName(), dlg.getModelTemplate());
	}
}

void RDOStudioApp::OnFileOpen() 
{
	model->openModel();
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

void RDOStudioApp::OnUpdateFileNew( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( model->canNew() );
}

void RDOStudioApp::OnUpdateFileOpen( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( model->canOpen() );
}

void RDOStudioApp::OnUpdateFileClose(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( model->canClose() );
}

void RDOStudioApp::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( model->canSave() );
}

void RDOStudioApp::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( model->hasModel() && !model->isRunning() );
}

void RDOStudioApp::OnUpdateFileSaveAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( model->canSave() );
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
	if ( !model->openModel( reopenList[i] ) && model->isPrevModelClosed() ) {
		tstring item = reopenList[i];
		for ( std::vector< tstring >::iterator it = reopenList.begin(); it != reopenList.end(); it++ ) {
			if ( *it == item ) {
				reopenList.erase( it );
				break;
			}
		}
		updateReopenSubMenu();
	}
}

void RDOStudioApp::insertReopenItem( CREF(tstring) item )
{
	if ( !item.empty() ) {

		for ( std::vector< tstring >::iterator it = reopenList.begin(); it != reopenList.end(); it++ ) {
			if ( *it == item ) {
				reopenList.erase( it );
				break;
			}
		}

		reopenList.insert( reopenList.begin(), item );

		while ( reopenList.size() > 10 ) {
			std::vector< tstring >::iterator it = reopenList.end();
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
		for ( std::vector< tstring >::size_type i = 0; i < reopenList.size(); i++ ) {
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
			reopenMenu->AppendMenu( MF_STRING, id, rdo::format( "%d. %s", i+1, reopenList[i].c_str() ).c_str() );
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
		tstring sec;
		if ( i+1 < 10 ) {
			sec = rdo::format( "0%d", i+1 );
		} else {
			sec = rdo::format( "%d", i+1 );
		}
		TRY {
			tstring s = AfxGetApp()->GetProfileString( "reopen", sec.c_str(), "" );
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
	for ( std::vector< tstring >::size_type i = 0; i < 10; i++ ) {
		tstring sec;
		if ( i+1 < 10 ) {
			sec = rdo::format( "0%d", i+1 );
		} else {
			sec = rdo::format( "%d", i+1 );
		}
		tstring s;
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
	pCmdUI->Enable( model->canBuild() );
}

void RDOStudioApp::OnUpdateModelRun(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( model->canRun() );
}

void RDOStudioApp::OnUpdateModelStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( model->isRunning() );
}

tstring RDOStudioApp::getFullFileName()
{
	tstring fileName = "";
	TCHAR szExeName[ MAX_PATH + 1 ];
	if ( ::GetModuleFileName( NULL, szExeName, MAX_PATH ) ) {
		fileName = szExeName;
	}
	return fileName;
}

tstring RDOStudioApp::getFullHelpFileName( tstring str )
{
	str.insert( 0, rdo::extractFilePath( RDOStudioApp::getFullFileName() ) );
	
	if ( !rdo::File::exist( str ) ) {
		::MessageBox( NULL, rdo::format( ID_MSG_NO_HELP_FILE, str.c_str() ).c_str(), NULL, MB_ICONEXCLAMATION | MB_OK );
		return "";
	}

	return str;
}

void RDOStudioApp::setFileAssociationSetup( const bool value )
{
	fileAssociationSetup = value;
	WriteProfileInt( "fileAssociation", "setup", fileAssociationSetup );
	if ( fileAssociationSetup ) {
		setupFileAssociation();
	}
}

void RDOStudioApp::setFileAssociationCheckInFuture( const bool value )
{
	if ( fileAssociationCheckInFuture != value ) {
		fileAssociationCheckInFuture = value;
		WriteProfileInt( "fileAssociation", "checkInFuture", fileAssociationCheckInFuture );
	}
}

void RDOStudioApp::setOpenLastProject( const bool value )
{
	if ( openLastProject != value ) {
		openLastProject = value;
		WriteProfileInt( "general", "openLastProject", openLastProject );
	}
}

void RDOStudioApp::setLastProjectName( CREF(tstring) projectName )
{
	insertReopenItem( projectName );
	if ( lastProjectName != projectName ) {
		lastProjectName = projectName;
		WriteProfileString( "general", "lastProject", getOpenLastProject() ? lastProjectName.c_str() : "" );
	}
}

void RDOStudioApp::setShowCaptionFullName( const bool value )
{
	if ( showCaptionFullName != value ) {
		showCaptionFullName = value;
		model->setName( model->getName() );
		WriteProfileInt( "general", "showCaptionFullName", showCaptionFullName );
	}
}

void RDOStudioApp::setupFileAssociation()
{
	tstring strFileTypeId    = _T("RAO.Project");
	tstring strFileTypeName  = _T("RAO Project");
	tstring strParam         = _T(" \"%1\"");
	tstring strPathName      = RDOStudioApp::getFullFileName();
	tstring strRAOExt        = _T(".rdox");

	tstring strFileTypeIdOld = _T("RAO.FileInfo");
	tstring strRAOExtOld     = _T(".smr");

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
								if ( !getFileAssociationSetup() ) {
									RDOFileAssociationDlg dlg;
									mustBeRegistered = dlg.DoModal() == IDOK;
									setFileAssociationCheckInFuture( dlg.checkInFuture ? true : false );
								}
							} else {
								mustBeRegistered = false;
							}
						}
					}
				}
			}
			if ( mustBeRegistered ) {
				DeleteRegistryKey(hCurUserSoftClasses, strFileTypeIdOld.c_str());
				DeleteRegistryKey(hCurUserSoftClasses, tstring(strFileTypeIdOld + _T("\\DefaultIcon")).c_str());
				DeleteRegistryKey(hCurUserSoftClasses, tstring(strFileTypeIdOld + _T("\\shell\\open\\command")).c_str());
				DeleteRegistryKey(hCurUserSoftClasses, tstring(strRAOExtOld + _T("\\ShellNew")).c_str());
				DeleteRegistryKey(hCurUserSoftClasses, strRAOExtOld.c_str());

				HKEY hKey_tmp;
				if ( ::RegCreateKeyEx( hCurUserSoftClasses, strFileTypeId.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result ) == ERROR_SUCCESS ) {
					tstring s = strFileTypeName;
					::RegSetValueEx( hKey_tmp, _T(""), 0, REG_SZ, (LPBYTE)s.c_str(), s.length() );
					::RegCloseKey( hKey_tmp );
				}
				if ( ::RegCreateKeyEx( hCurUserSoftClasses, tstring(strFileTypeId + _T("\\DefaultIcon")).c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result ) == ERROR_SUCCESS ) {
					tstring s = strPathName + _T(",0");
					::RegSetValueEx( hKey_tmp, _T(""), 0, REG_SZ, (LPBYTE)s.c_str(), s.length() );
					::RegCloseKey( hKey_tmp );
				}
				if ( ::RegCreateKeyEx( hCurUserSoftClasses, tstring(strFileTypeId + _T("\\shell\\open\\command")).c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result ) == ERROR_SUCCESS ) {
					tstring s = strPathName + strParam;
					::RegSetValueEx( hKey_tmp, _T(""), 0, REG_SZ, (LPBYTE)s.c_str(), s.length() );
					::RegCloseKey( hKey_tmp );
				}
				if ( ::RegCreateKeyEx( hCurUserSoftClasses, strRAOExt.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result ) == ERROR_SUCCESS ) {
					tstring s = strFileTypeId;
					::RegSetValueEx( hKey_tmp, _T(""), 0, REG_SZ, (LPBYTE)s.c_str(), s.length() );
					::RegCloseKey( hKey_tmp );
				}
				if ( ::RegCreateKeyEx( hCurUserSoftClasses, tstring(strRAOExt + _T("\\ShellNew")).c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result ) == ERROR_SUCCESS ) {
					tstring s = "";
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
	RDOAboutDlg dlg;
	dlg.DoModal();
}

void RDOStudioApp::autoCloseByModel()
{
	if ( autoExitByModel ) {
		if ( !dontCloseIfError || !model || (dontCloseIfError && (model->getExitCode() == rdoSimulator::EC_OK || model->getExitCode() == rdoSimulator::EC_NoMoreEvents) ) ) {
			mainFrame->SendMessage( WM_CLOSE );
		}
	}
}

void RDOStudioApp::autoCloseByPlugin( RDOStudioPlugin* plugin )
{
	if ( std::find( plugin_exit_name.begin(), plugin_exit_name.end(), plugin->getFileName() ) != plugin_exit_name.end() ) {
		mainFrame->SendMessage( WM_CLOSE );
	}
}

bool RDOStudioApp::isPluginAutoStart( RDOStudioPlugin* plugin ) const
{
	return std::find( plugin_start_name.begin(), plugin_start_name.end(), plugin->getFileName() ) != plugin_start_name.end();
}

BOOL RDOStudioApp::PreTranslateMessage( MSG* pMsg ) 
{
	if ( pMsg->message == WM_KEYDOWN ) {
		CMDIChildWnd* child = mainFrame->MDIGetActive();
		if ( child ) {
			CView* view = child->GetActiveView();
			if ( dynamic_cast<RDOStudioFrameView*>(view) && view == child->GetFocus() ) {
				view->SendMessage( pMsg->message, pMsg->wParam, pMsg->lParam );
				return true;
			}
		}
	}
	if ( pMsg->message == PLUGIN_MUSTEXIT_MESSAGE ) {
		plugins->stopPluginByStudio( reinterpret_cast<HMODULE>(pMsg->wParam) );
	}
	return CWinApp::PreTranslateMessage(pMsg);
}

BOOL RDOStudioApp::ProcessMessageFilter( int code, LPMSG lpMsg )
{
	return CWinApp::ProcessMessageFilter( code, lpMsg );
}

// --------------------------------------------------------------------------------
// -------------------- RDOAboutDlg
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOAboutDlg, CDialog )
	ON_BN_CLICKED(IDC_ABOUT_EMAIL, OnAboutEmail)
	ON_BN_CLICKED(IDC_ABOUT_WEB, OnAboutWeb)
END_MESSAGE_MAP()

RDOAboutDlg::RDOAboutDlg():
	CDialog( IDD )
{
	m_caption = _T("");
	m_developer = _T("");
	CString s;
	s.Format( IDS_DEVELOPERS );
	m_developer = s;

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
#ifdef RDO_MT
								char* thread_ver = "mt";
#else
								char* thread_ver = "st";
#endif

#ifdef RDOSIM_COMPATIBLE
								char* rdo_compatible = "-comp";
#else
								char* rdo_compatible = "";
#endif

								s.Format( "%s   %s%s-version %u.%u (build %u)", productName, thread_ver, rdo_compatible, HIWORD( fixedInfo->dwProductVersionMS ), LOWORD( fixedInfo->dwProductVersionMS ), LOWORD( fixedInfo->dwProductVersionLS ) );
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
	DDX_Control(pDX, IDC_ABOUT_WEB, m_web);
	DDX_Control(pDX, IDC_ABOUT_EMAIL, m_email);
	DDX_Text(pDX, IDC_ABOUT_CAPTION, m_caption);
	DDX_Text(pDX, IDC_DEVELOPERS, m_developer);
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

void RDOStudioApp::broadcastMessage( RDOThread::RDOTreadMessage message, void* param )
{
#ifdef RDO_MT
	CEvent* event = studioMT->manualMessageFrom( message, param );
	while ( ::WaitForSingleObject( event->m_hObject, 0 ) == WAIT_TIMEOUT ) {
		static_cast<RDOThreadStudioGUI*>(studioGUI)->processMessages();
		if ( mainFrame ) {
			mainFrame->UpdateWindow();
		} else {
			break;
		}
	}
	delete event;
#else
	studioGUI->broadcastMessage( message, param );
#endif
}

BOOL RDOStudioApp::OnIdle( LONG lCount )
{
#ifdef RDO_MT
	static_cast<RDOThreadStudioGUI*>(studioGUI)->processMessages();
	CWinApp::OnIdle( lCount );
	return true;
#else
	kernel->idle();
	if ( lCount > 10000 ) {
		return CWinApp::OnIdle( lCount );
	} else {
		CWinApp::OnIdle( lCount );
		return true;
	}
#endif
}

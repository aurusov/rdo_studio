#include "stdafx.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "rdostudiochildfrm.h"
#include "rdostudiomodel.h"
#include "rdostudioeditdoc.h"
#include "rdostudioeditview.h"
#include "resource.h"
#include "./rdo_tracer/rdotracertrace.h"

#include <rdokernel.h>
#include <rdorepository.h>

using namespace std;
using namespace rdoRepository;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

	// Change the registry key under which our settings are stored.
//	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

//	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	editDocTemplate = new CMultiDocTemplate( IDR_EDITTYPE, RUNTIME_CLASS(RDOStudioEditDoc), RUNTIME_CLASS(RDOStudioChildFrame), RUNTIME_CLASS(RDOStudioEditView) );
	AddDocTemplate( editDocTemplate );

	AddDocTemplate( tracer.createDocTemplate() );

	mainFrame = new RDOStudioMainFrame;
	if ( !mainFrame->LoadFrame( IDR_MAINFRAME ) ) return FALSE;
	m_pMainWnd = mainFrame;

	loadReopen();
	updateReopenSubMenu();

	// Enable drag/drop open
//	mainFrame->DragAcceptFiles();

	// Enable DDE Execute open
//	EnableShellOpen();
//	RegisterShellFileTypes( TRUE );

	mainFrame->ShowWindow(m_nCmdShow);
	mainFrame->UpdateWindow();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine( cmdInfo );

	// Dispatch commands specified on the command line
	if ( !ProcessShellCommand(cmdInfo) ) return FALSE;

	initInstance = true;

	return TRUE;
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

void RDOStudioApp::OnAddNewFrame( WPARAM /*wParam*/, LPARAM /*lParam*/ )
{
	model->addNewFrame();
}

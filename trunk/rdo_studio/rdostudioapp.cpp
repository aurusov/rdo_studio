#include "stdafx.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "rdostudiochildfrm.h"
#include "rdostudiomodel.h"
#include "rdostudiomodeldoc.h"
#include "rdostudiomodelview.h"
#include "rdostudioeditdoc.h"
#include "rdostudioeditview.h"
#include "resource.h"
#include "./rdo_tracer/rdotracertrace.h"

#include <rdokernel.h>
#include <rdorepository.h>

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
	ON_COMMAND(ID_FILE_RDO_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_RDO_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_FILE_SAVE_ALL, OnFileSaveAll)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_ALL, OnUpdateFileSaveAll)
	ON_COMMAND(ID_FILE_RDO_CLOSE, OnFileClose)
	ON_COMMAND(ID_FILE_RDO_SAVE_AS, OnFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_RDO_CLOSE, OnUpdateFileClose)
	ON_UPDATE_COMMAND_UI(ID_FILE_RDO_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_RDO_RUN, OnRdoRun)
	ON_COMMAND(ID_RDO_STOP, OnRdoStop)
	ON_UPDATE_COMMAND_UI(ID_RDO_RUN, OnUpdateRdoRun)
	ON_UPDATE_COMMAND_UI(ID_RDO_STOP, OnUpdateRdoStop)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE( ID_FILE_REOPEN_1, ID_FILE_REOPEN_10, OnProjectReopen )
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

RDOStudioApp::RDOStudioApp():
	CWinApp(),
	initInstance( false ),
	modelDocTemplate( NULL )
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

	modelDocTemplate = new CMultiDocTemplate( IDR_MODELTYPE, RUNTIME_CLASS(RDOStudioModelDoc), RUNTIME_CLASS(RDOStudioChildFrame), RUNTIME_CLASS(RDOStudioModelView) );
	AddDocTemplate( modelDocTemplate );

	editDocTemplate = new CMultiDocTemplate( IDR_EDITTYPE, RUNTIME_CLASS(RDOStudioEditDoc), RUNTIME_CLASS(RDOStudioChildFrame), RUNTIME_CLASS(RDOStudioEditView) );
	AddDocTemplate( editDocTemplate );

	AddDocTemplate( trace.createDocTemplate() );

	mainFrame = new RDOStudioMainFrame;
	if ( !mainFrame->LoadFrame( IDR_MAINFRAME ) ) return FALSE;
	m_pMainWnd = mainFrame;

	model = new RDOStudioModel;

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

int RDOStudioApp::ExitInstance() 
{
	::OleUninitialize();
	if ( model ) { delete model; model = NULL; }
	return CWinApp::ExitInstance();
}

void RDOStudioApp::appendMenu( CMenu* from, const int from_index, CMenu* to )
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

void RDOStudioApp::eraseMenu( CMenu* from, const int from_index )
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

RDOStudioModelDoc* RDOStudioApp::getModelDoc()
{
	POSITION pos = modelDocTemplate->GetFirstDocPosition();
	if ( pos ) {
		return (RDOStudioModelDoc*)modelDocTemplate->GetNextDoc( pos );
	}
	return NULL;
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
	pCmdUI->Enable( getModelDoc() ? true : false );
}

void RDOStudioApp::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	POSITION pos;
	pos = modelDocTemplate->GetFirstDocPosition();
	while ( pos ) {
		RDOStudioModelDoc* doc = (RDOStudioModelDoc*)modelDocTemplate->GetNextDoc( pos );
		if ( doc ) {
			doc->updateModify();
		}
	}
	pos = editDocTemplate->GetFirstDocPosition();
	while ( pos ) {
		RDOStudioEditDoc* doc = (RDOStudioEditDoc*)editDocTemplate->GetNextDoc( pos );
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
	pCmdUI->Enable( getModelDoc() ? true : false );
}

void RDOStudioApp::OnUpdateFileSaveAll(CCmdUI* pCmdUI) 
{
	bool flag = model->isModify();
	if ( !flag ) {
		POSITION pos;
		pos = modelDocTemplate->GetFirstDocPosition();
		while ( pos ) {
			RDOStudioModelDoc* doc = (RDOStudioModelDoc*)modelDocTemplate->GetNextDoc( pos );
			if ( doc ) {
				doc->updateModify();
				flag = doc->IsModified() ? true : false;
				if ( flag ) break;
			}
		}
		if ( !flag ) {
			pos = editDocTemplate->GetFirstDocPosition();
			while ( pos ) {
				RDOStudioEditDoc* doc = (RDOStudioEditDoc*)editDocTemplate->GetNextDoc( pos );
				if ( doc ) {
					doc->updateModify();
					flag = doc->IsModified() ? true : false;
					if ( flag ) break;
				}
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
		for ( int i = 0; i < reopenList.size(); i++ ) {
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
			CString s;
			s.Format( "%d. %s", i+1, reopenList[i].c_str() );
			reopenMenu->AppendMenu( MF_STRING, id, s );
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
		CString sec;
		if ( i+1 < 10 ) {
			sec.Format( "0%d", i+1 );
		} else {
			sec.Format( "%d", i+1 );
		}
		TRY {
			string s = AfxGetApp()->GetProfileString( "reopen", sec, "" );
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
	for ( int i = 0; i < 10; i++ ) {
		CString sec;
		if ( i+1 < 10 ) {
			sec.Format( "0%d", i+1 );
		} else {
			sec.Format( "%d", i+1 );
		}
		string s;
		if ( i < reopenList.size() ) {
			s = reopenList[i];
		} else {
			s = "";
		}
		TRY {
			AfxGetApp()->WriteProfileString( "reopen", sec, s.c_str() );
		} CATCH( CException, e ) {
		}
		END_CATCH
	}
}

void RDOStudioApp::OnRdoRun() 
{
	model->runModel();
}

void RDOStudioApp::OnRdoStop() 
{
	model->stopModel();
}

void RDOStudioApp::OnUpdateRdoRun(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( model && !model->isRunning() );
}

void RDOStudioApp::OnUpdateRdoStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( model && model->isRunning() );
}

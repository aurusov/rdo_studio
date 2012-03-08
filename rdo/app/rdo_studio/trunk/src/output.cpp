/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      output.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/output.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdobuildedit.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdodebugedit.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdofindedit.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditorresults.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditortabctrl.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracer.h"
#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdotracerlogctrl.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditor;
using namespace rdoEditCtrl;

// --------------------------------------------------------------------------------
// -------------------- RDOStudioOutput
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioOutput, RDOStudioDockWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

RDOStudioOutput::RDOStudioOutput()
{
}

RDOStudioOutput::~RDOStudioOutput()
{
	eraseMenu( &popupMenu );
}

int RDOStudioOutput::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (RDOStudioDockWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	tab.Create( NULL, NULL, 0, CRect(0, 0, 100, 100), this, 0 );
	tab.modifyTabStyle( 0, TCS_MULTILINE );
//	tab.modifyTabStyle( 0, TCS_BOTTOM | TCS_MULTILINE );

	popupMenu.CreatePopupMenu();

	CMenu* mainMenu = AfxGetMainWnd()->GetMenu();

	BOOL maximized;
	studioApp.mainFrame->MDIGetActive( &maximized );
	int delta = maximized ? 1 : 0;

	appendMenu( mainMenu->GetSubMenu( 1 + delta ), 4, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 1 + delta ), 8, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 1 + delta ), 10, &popupMenu );
	popupMenu.AppendMenu( MF_SEPARATOR );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 0, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 1, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 2, &popupMenu );
	popupMenu.AppendMenu( MF_SEPARATOR );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 7, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 8, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 9, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 10, &popupMenu );

	build   = new RDOBuildEdit;
	debug   = new RDODebugEdit;
	trace   = tracer->createLog();
	results = new RDOEditorResults;
	find    = new RDOFindEdit;

	build->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), 0 );
	debug->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), 0 );
	trace->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), 0 );
	results->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), 0 );
	find->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), 0 );

	build->setEditorStyle( &studioApp.mainFrame->style_build );
	build->setPopupMenu( &popupMenu );

	debug->setEditorStyle( &studioApp.mainFrame->style_debug );
	debug->setPopupMenu( &popupMenu );

	trace->setStyle( &studioApp.mainFrame->style_trace );

	results->setEditorStyle( &studioApp.mainFrame->style_results );
	results->setPopupMenu( &popupMenu );

	find->setEditorStyle( &studioApp.mainFrame->style_find );
	find->setPopupMenu( &popupMenu );

	tab.insertItem( build, rdo::format( IDS_TAB_BUILD ).c_str() );
	tab.insertItem( debug, rdo::format( IDS_TAB_DEBUG ).c_str() );
	tab.insertItem( trace, rdo::format( IDS_TAB_TRACE ).c_str() );
	tab.insertItem( results, rdo::format( IDS_TAB_RESULT ).c_str() );
	tab.insertItem( find, rdo::format( IDS_TAB_FIND ).c_str() );

	studioApp.mainFrame->registerCmdWnd( build, build->getSCIHWND() );
	studioApp.mainFrame->registerCmdWnd( debug, debug->getSCIHWND() );
	studioApp.mainFrame->registerCmdWnd( trace );
	studioApp.mainFrame->registerCmdWnd( results, results->getSCIHWND() );
	studioApp.mainFrame->registerCmdWnd( find, find->getSCIHWND() );

	return 0;
}

void RDOStudioOutput::showBuild()
{
	studioApp.mainFrame->showOutput();
	tab.setCurrentItem( 0 );
	if ( plugins->studioIsShow() ) {
		build->SetFocus();
		UpdateWindow();
	}
}

void RDOStudioOutput::showDebug()
{
	studioApp.mainFrame->showOutput();
	tab.setCurrentItem( 1 );
	if ( plugins->studioIsShow() ) {
		debug->SetFocus();
		UpdateWindow();
	}
}

void RDOStudioOutput::showTrace()
{
	studioApp.mainFrame->showOutput();
	tab.setCurrentItem( 2 );
	if ( plugins->studioIsShow() ) {
		trace->SetFocus();
		UpdateWindow();
	}
}

void RDOStudioOutput::showResults()
{
	studioApp.mainFrame->showOutput();
	tab.setCurrentItem( 3 );
	if ( plugins->studioIsShow() ) {
		results->SetFocus();
		UpdateWindow();
	}
}

void RDOStudioOutput::showFind()
{
	studioApp.mainFrame->showOutput();
	tab.setCurrentItem( 4 );
	if ( plugins->studioIsShow() ) {
		find->SetFocus();
		UpdateWindow();
	}
}

void RDOStudioOutput::clearBuild()
{
	if ( build ) build->clearAll();
}

void RDOStudioOutput::clearDebug()
{
	if ( debug ) debug->clearAll();
}

void RDOStudioOutput::clearResults()
{
	if ( results ) results->clearAll();
}

void RDOStudioOutput::clearFind()
{
	if ( find ) find->clearAll();
}

void RDOStudioOutput::appendStringToBuild( const std::string& str ) const
{
	RDOBuildEditLineInfo* line = new RDOBuildEditLineInfo( str );
	build->appendLine( line );
}

void RDOStudioOutput::appendStringToBuild( rdoSimulator::RDOSyntaxError::ErrorCode error_code, const std::string& str, const rdoModelObjects::RDOFileType fileType, const int lineNumber, const int posInLine, const bool warning ) const
{
	if ( !warning || (warning && static_cast<RDOBuildEditTheme*>(studioApp.mainFrame->style_build.theme)->warning) ) {
		RDOBuildEditLineInfo* line = new RDOBuildEditLineInfo( error_code, str, fileType, lineNumber, posInLine, warning );
		build->appendLine( line );
	}
}

void RDOStudioOutput::appendStringToDebug( const std::string& str ) const
{
	debug->appendLine( str );
}

void RDOStudioOutput::appendStringToResults( const std::string& str ) const
{
	int pos = results->getCurrentPos();
	results->setCurrentPos(results->getLength());
	results->setReadOnly  (false);
	results->appendText   (str  );
	results->setReadOnly  (true );
	results->setCurrentPos(pos  );
}

void RDOStudioOutput::appendStringToFind( const std::string& str, const rdoModelObjects::RDOFileType fileType, const int lineNumber, const int posInLine ) const
{
	RDOLogEditLineInfo* line = new RDOLogEditLineInfo( str, fileType, lineNumber, posInLine );
	find->appendLine( line );
}

void RDOStudioOutput::Tab::changeCurrentItem()
{
	studioApp.mainFrame->output.updateLogConnection();
}

void RDOStudioOutput::updateLogConnection() const
{
	int item = tab.getCurrentIndex();
	RDOLogEdit* log = NULL;
	if ( item == 0 ) {
		log = build;
	} else if ( item == 4 ) {
		log = find;
	}
	if ( log ) {
		rdoEditor::RDOEditorTabCtrl* editor_tab = model->getTab();
		if ( editor_tab ) {
			for ( int i = 0; i < editor_tab->getItemCount(); i++ ) {
				editor_tab->getItemEdit( i )->setLog( *log );
			}
		}
	}
}

void RDOStudioOutput::updateStyles() const
{
	build->setEditorStyle( &studioApp.mainFrame->style_build );
	debug->setEditorStyle( &studioApp.mainFrame->style_debug );
	trace->setStyle( &studioApp.mainFrame->style_trace );
	results->setEditorStyle( &studioApp.mainFrame->style_results );
	find->setEditorStyle( &studioApp.mainFrame->style_find );
}

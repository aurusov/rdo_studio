#include "stdafx.h"
#include "rdoprocess_project.h"
#include "rdoprocess_app.h"
#include "rdoprocess_childfrm.h"
#include "rdoprocess_docview.h"
#include "ctrl/rdoprocess_toolbar.h"
#include <rdoprocess_pixmap.h>
#include <rdoprocess_object_flowchart.h>
#include <rdoprocess_method.h>
#include <rdoprocess_xml.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPProjectMFC
// ----------------------------------------------------------------------------
RPProjectMFC::RPProjectMFC():
	RPProject()
{
}

RPProjectMFC::~RPProjectMFC()
{
	std::list< RPCtrlToolbarMFC* >::iterator it = toolbars.begin();
	while ( it != toolbars.end() ) {
		delete *it;
		it++;
	}
	toolbars.clear();
}

std::ofstream& RPProjectMFC::log() const
{
	return rpapp.log;
}

RPCtrlToolbar* RPProjectMFC::createToolBar( const rp::string& caption )
{
	RPCtrlToolbarMFC* toolbar = new RPCtrlToolbarMFC( rpapp.mainFrame );
	toolbars.push_back( toolbar );
	rpapp.mainFrame->insertToolBar( &toolbar->toolbar );
	toolbar->setCaption( caption );
	return toolbar;
}

RPPixmap* RPProjectMFC::createBitmap( char* xpm[] )
{
	return new RPPixmap( xpm );
}
/*
bool RPProjectMFC::lockResource( rpMethod::RPMethod* method )
{
	RPMethodPlugin* plugin = rpapp.getMethodManager().find( method );
	if ( plugin ) {
		AfxSetResourceHandle( plugin->getLib() );
		return true;
	}
	return false;
}

void RPProjectMFC::unlockResource()
{
	AfxSetResourceHandle( rpapp.m_hInstance );
}

HWND RPProjectMFC::getMainWnd()
{
	return AfxGetMainWnd()->m_hWnd;
}
*/
void RPProjectMFC::open()
{
	try {
		rp::RPXML xml_doc;
		rp::RPXMLNode* project_node = xml_doc.open( "c:\\sample.xml" );
		if ( project_node ) {
			load( project_node );
		}
	} catch ( rp::RPXMLException& ex ) {
		rpapp.mainFrame->MessageBox( ex.getError().c_str(), NULL, MB_ICONERROR );
	}
}

void RPProjectMFC::save()
{
	try {
		rp::RPXML xml_doc;
		rp::RPXMLNode* project_node = xml_doc.getDocument().makeChild( "project" );
		save_child( project_node );
		xml_doc.save( "c:\\sample.xml" );
	} catch ( rp::RPXMLException& ex ) {
		rpapp.mainFrame->MessageBox( ex.getError().c_str(), NULL, MB_ICONERROR );
	}
}

void RPProjectMFC::load( rp::RPXMLNode* node )
{
	rp::RPXMLNode* flowchart_node = NULL;
	while ( flowchart_node = node->nextChild(flowchart_node) ) {
		if ( flowchart_node->getName() == "flowchart" ) {
			RPObjectFlowChart* flowobj = static_cast<RPObjectFlowChart*>(rpMethod::factory->getNewObject( flowchart_node->getAttribute("class"), rpMethod::project ));
			flowobj->load( flowchart_node );
		}
	}
}

void RPProjectMFC::makeFlowChartWnd( RPObjectFlowChart* flowobj )
{
	BOOL maximized = false;
	rpapp.mainFrame->MDIGetActive( &maximized );

	RPDoc* doc = static_cast<RPDoc*>(rpapp.mainFrame->flowchartDocTemplate->OpenDocumentFile( NULL ));
	RPChildFrame* mdi = static_cast<RPChildFrame*>(doc->getView()->GetParent());
	mdi->SetIcon( flowobj->getMethod()->getPixmap()->getIcon(), true );
	if ( maximized ) {
		mdi->ShowWindow( SW_HIDE );
		mdi->MDIRestore();
		mdi->ShowWindow( SW_HIDE );
	}
	doc->getView()->makeFlowChartWnd( flowobj );
	flowobj->setCorrectName( flowobj->getClassInfo()->getLabel() );
	if ( maximized ) {
		mdi->MDIMaximize();
	}
/*
CDocument::SetTitle Ч установить заголовок дл€ документа

CChildFrame::OnUpdateFrameTitle Ч переопределить дл€ своих нужд Ч mdiчилды

CFrameWnd::OnUpdateFrameTitle Ч переопределить дл€ своих нужд Ч главное окно

AfxSetWindowText Ч нужно вызывать дл€ правильного(чилд или главное окно) окна, иначе ничего видно не будет.
CViewXXX €вл€етюс€ чилдома у CMDIChildWnd
*/
}

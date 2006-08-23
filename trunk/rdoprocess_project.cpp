#include "stdafx.h"
#include "rdoprocess_project.h"
#include "rdoprocess_app.h"
#include "rdoprocess_xml.h"
#include "rdoprocess_childfrm.h"
#include "ctrl/rdoprocess_bitmap.h"
#include "ctrl/rdoprocess_toolbar.h"
#include <rdoprocess_object_flowchart.h>

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

RPBitmap* RPProjectMFC::createBitmap( char* xpm[] )
{
	return new RPBitmapMFC( xpm );
}

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

void RPProjectMFC::open()
{
	try {
		RPXMLMFC xml_doc;
		xml_doc.open( "c:\\sample.xml" );
	} catch ( rp::RPXMLException& ex ) {
		rpapp.mainFrame->MessageBox( ex.getError().c_str(), NULL, MB_ICONERROR );
	}
}

void RPProjectMFC::save()
{
	try {
		RPXMLMFC xml_doc;
		rp::RPXMLNode* project_node = xml_doc.getDocument().makeChild( "project" );
		save_child( project_node );
		xml_doc.save( "c:\\sample.xml" );
	} catch ( rp::RPXMLException& ex ) {
		rpapp.mainFrame->MessageBox( ex.getError().c_str(), NULL, MB_ICONERROR );
	}
}

void RPProjectMFC::load( rp::RPXMLNode* node )
{
	RPXMLNodeMFC* flowchart_node = NULL;
	while ( flowchart_node = static_cast<RPXMLNodeMFC*>(node->nextChild(flowchart_node)) ) {
		if ( flowchart_node->getName() == "flowchart" ) {
			RPObjectFlowChart* flowobj = static_cast<RPObjectFlowChart*>(rpMethod::factory->getNewObject( flowchart_node->getAttribute("class"), rpMethod::project ));
			rpMethod::project->makeFlowChart( flowobj );
			flowobj->load( flowchart_node );
		}
	}
}

void RPProjectMFC::makeFlowChart( RPObjectFlowChart* flowobj )
{
	RPChildFrame* child = static_cast<RPChildFrame*>(rpapp.mainFrame->CreateNewChild( RUNTIME_CLASS(RPChildFrame), IDR_RDO_PRTYPE, rpapp.m_hMDIMenu, rpapp.m_hMDIAccel ));
	child->SetIcon( rpapp.LoadIcon(IDR_RDO_PRTYPE), true );
	child->makeFlowChart( flowobj );
}

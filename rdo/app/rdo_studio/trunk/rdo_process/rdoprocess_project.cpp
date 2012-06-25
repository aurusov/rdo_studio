/*!
  \copyright (c) RDO-Team, 2006-2012
  \file      rdoprocess_project.cpp
  \authors   ћанжай »горь (daynin@gmail.com)
  \authors   ”русов јндрей (rdo@rk9.bmstu.ru)
  \authors   Ўувалов јндрей (dronbas@gmail.com)
  \date      10.02.2006
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_project.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_childfrm.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_docview.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_toolbar.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_pixmap.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object_flowchart.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_method.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_xml.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RPProjectMFC
// --------------------------------------------------------------------------------
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
	return studioApp.log();
}

RPPixmap* RPProjectMFC::createBitmap( char* xpm[] )
{
	return new RPPixmap( xpm );
}
/*
rbool RPProjectMFC::lockResource( rpMethod::RPMethod* method )
{
	RPMethodPlugin* plugin = studioApp.getMethodManager().find( method );
	if ( plugin ) {
		AfxSetResourceHandle( plugin->getLib() );
		return true;
	}
	return false;
}

void RPProjectMFC::unlockResource()
{
	AfxSetResourceHandle( studioApp.m_hInstance );
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
		studioApp.m_pMainFrame->MessageBox( ex.getError().c_str(), NULL, MB_ICONERROR );
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
		studioApp.m_pMainFrame->MessageBox( ex.getError().c_str(), NULL, MB_ICONERROR );
	}
}

void RPProjectMFC::saveToXML(REF(pugi::xml_node) parentNode) const
{
	// »нициализируем первую ноду именем класса первого встретившегос€ потомка:
	pugi::xml_node node = parentNode.append_child(getClassName().c_str());
	// јтрибуты <RPProjectMFC>:
	node.append_attribute(_T("name")).set_value(getName().c_str());

	// —сылаемс€ на первого потомка RPObject (RPProcessFlowChart), использу€ контейнер "list":
	std::list<PTR(RPObject)> childList;
	getAllChild(childList);
	STL_FOR_ALL_CONST(childList, it)
	{
		(*it)->saveToXML(node);
	}
}

void RPProjectMFC::loadFromXML(CREF(pugi::xml_node) node)
{
	// ѕришел узел, следующий за <Model>, т.е. <RPProjectMFC>:
	if (strcmp(node.name(), getClassName().c_str()) == 0)
	{
		// ѕровер€ем, а стоим ли мы на узле RPProcessFlowChart ? (т.к. их может быть несколько, то запускаем цикл)
		// јтрибуты <RPProjectMFC> не считываем. ќни не актуальны.
		for (pugi::xml_node next_node = node.first_child(); next_node; next_node = next_node.next_sibling())
		{
			if (strcmp(next_node.name(), "RPProcessFlowChart") == 0)
			{
				// √енерируем FlowChart
				PTR(RPObjectFlowChart) pFlowChart = static_cast<PTR(RPObjectFlowChart)>(rpMethod::factory->getNewObject(next_node.name(), rpMethod::project));
				ASSERT(pFlowChart);
				pFlowChart->loadFromXML(next_node);
			}
		}
	}
}

void RPProjectMFC::load( rp::RPXMLNode* node )
{
	rp::RPXMLNode* flowchart_node = node->nextChild(NULL);
	while ( flowchart_node )
	{
		if ( flowchart_node->getName() == "flowchart" ) {
			RPObjectFlowChart* flowobj = static_cast<RPObjectFlowChart*>(rpMethod::factory->getNewObject( flowchart_node->getAttribute("class"), rpMethod::project ));
			flowobj->load( flowchart_node );
		}
		flowchart_node = node->nextChild(flowchart_node);
	}
}

void RPProjectMFC::makeFlowChartWnd( RPObjectFlowChart* flowobj )
{
	BOOL maximized = false;
	studioApp.m_pMainFrame->MDIGetActive( &maximized );
	PTR(RPDoc) doc = model->getFlowchartDoc();
	PTR(RPChildFrame) mdi = static_cast<PTR(RPChildFrame)>(doc->getView()->GetParent());
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

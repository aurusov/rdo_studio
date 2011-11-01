#include "app/rdo_studio_mfc/pch/stdpch.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_project.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_childfrm.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_docview.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_toolbar.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_pixmap.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object_flowchart.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_method.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_xml.h"

#include "app/rdo_studio_mfc/src/model/model.h"

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
	return studioApp.log;
}

RPPixmap* RPProjectMFC::createBitmap( char* xpm[] )
{
	return new RPPixmap( xpm );
}
/*
bool RPProjectMFC::lockResource( rpMethod::RPMethod* method )
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
		studioApp.mainFrame->MessageBox( ex.getError().c_str(), NULL, MB_ICONERROR );
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
		studioApp.mainFrame->MessageBox( ex.getError().c_str(), NULL, MB_ICONERROR );
	}
}


void RPProjectMFC::save_To_XML(pugi::xml_document &doc, std::list< RPObject* >& all_child)
{

#pragma region Fill the list <RPObject*> with getAllChild(...) method.
	//std::list <RPObject*> all_child;
	//getAllChild(all_child);
	//for(std::list <RPObject*>::const_iterator it = all_child.begin(); it != all_child.end(); ++it )
	//{
	//	(*it)->save_To_XML(doc);
	//}
#pragma endregion »спользование готовой функции. // COMMENT //

	// «аполн€ем список всеми созданными потомками RPObject в рабочей области "–ƒќ-ѕроцесс".
	// —начала список пополн€ют сами окна этой рабочей области.
	// »спользуем функцию "insert" именно в такой интерпретации, поскольку потомков одинакового  
	// типа может быть более одного.
	all_child.insert( all_child.end(), child.begin(), child.end() );
	for( std::list< RPObject* >::const_iterator it = all_child.begin(); it != all_child.end(); ++it )
	//{
		(*it)->save_To_XML(doc, all_child);        // - вызываем дл€ каждого помещенного на рабочую область
	                                               //   элемента его же виртуальную функцию сохранени€
	
		//pugi::xml_node node = doc.append_child((*it)->getClassName().c_str());
		//pugi::xml_node node = doc.insert_child_after((*it)->getClassName().c_str(), doc);

	//}

	// јвтоматически открываем файл при создании потока:
	// P.S. –ежим открыти€ файла (битовые маски) - 
	//         ios::out - открыть файл дл€ записи;
	//         ios::ate - начало вывода устанавливаетс€ в конец файла;
	std::ofstream OutFile("D:\\TESTXML.txt", std::ios::out | std::ios::ate);

	// ѕровер€ем открытый нами поток на наличие ошибок ввода-вывода:
	if (OutFile.good())
	{
		doc.save(OutFile, "\t", pugi::format_indent | pugi::format_no_declaration);
		OutFile.close();
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
	studioApp.mainFrame->MDIGetActive( &maximized );
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

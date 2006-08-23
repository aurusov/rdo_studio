// rdoprocess_shape_terminate_MJ.cpp: implementation of the RPShapeTerminate_MJ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "rdoprocess_shape_terminate_MJ.h"
#include "rdoprocess_shape_terminate_dlg1_MJ.h"
#include "rdoprocess_method_proc2rdo_MJ.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RPShapeTerminateMJ::RPShapeTerminateMJ( RPObject* _parent ):
	RPShape_MJ( _parent, _T("Terminate") )
{
	// èíèöèàëèçàöèÿ òèïà áëîêà 
	type ="block";
		
	pa_src.push_back( rp::point(-50, 0) );
	pa_src.push_back( rp::point(-25, -25) );
	pa_src.push_back( rp::point(50, -25) );
	pa_src.push_back( rp::point(50, 25) );
	pa_src.push_back( rp::point(-25, 25) );
	pa_src.push_back( rp::point(-50, 0) );

	docks.push_back( new RPConnectorDock( this, RPConnectorDock::in,  rp::point( -50, 0 ), 180 ) );
}

RPShapeTerminateMJ::~RPShapeTerminateMJ()
{
}

RPObject* RPShapeTerminateMJ::newObject( RPObject* parent )
{
	return new RPShapeTerminateMJ( parent );
}

void RPShapeTerminateMJ::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	if ( rpMethod::project->lockResource( proc2rdo ) ) {
		RPShapeTerminateDlg1_MJ dlg( CWnd::FromHandle(rpMethod::project->getMainWnd()), this );
		dlg.DoModal();
		rpMethod::project->unlockResource();
	}
}

void RPShapeTerminateMJ::generate()
{

	//çàïîëíÿåì ëèñò íàçâàíèÿìè ïàòòåğíîâ äëÿ *.opr
		CString name_value1("Áëîê_óäàëåíèÿ_");
		CString name_value2(getName().c_str());
		CString name_value(name_value1 + name_value2);
		proc2rdo->list_pattern_names.push_back(name_value);



	/*
	RDOfiles->pattern <<std::endl<<std::endl<<"èìÿ ñëåäóşùåãî áëîêà - "<<id_next
	<<std::endl<<"èìÿ - "<<getName().c_str();
*/

// ÃÅÍÅĞÀÖÈß ğåñóğñîâ ĞÄÎ ÔÀÉË *.res

	RPCreationRDOFilesMJ* RDOfiles = proc2rdo->RDOfiles;
	RDOfiles->resourse<<std::endl<<std::endl<<"{-------áëîê process ------" <<getName().c_str()<<"-------------------}" <<std::endl

	<<"Block_Del_"<<getName().c_str()<<" : Block_Del 0";






// ÃÅÍÅĞÀÖÈß ïàòòåğíîå ĞÄÎ ÔÀÉË *.pat


RDOfiles->pattern <<std::endl
<<std::endl<<"{-------áëîê óäàëåíèÿ------------"<<getName().c_str()<<"---------------------------------}"
<<std::endl
<<std::endl
<<std::endl<<"$Pattern Áëîê_óäàëåíèÿ_"<<getName().c_str()<<" : rule  {ñğàáàòûâàíèå çàêîíà}trace"
<<std::endl<<"	$Relevant_resources"
<<std::endl
<<std::endl<<"		_transact_X : Group_of_transacts_X Erase"
<<std::endl<<"		_block : Block_Del_"<<getName().c_str()<<" Keep"
<<std::endl<<"			{ïåğå÷èñëèòü âñå ãğóïïû òğàíçàêòîâ êîòîğûå ñîçäàşòñÿ - ÂĞÓ×ÅÓŞ ÏÎËÜÇÎÂÀÒÅËÅÌ}"
<<std::endl
<<std::endl
<<std::endl
<<std::endl<<"$Body"
<<std::endl 
<<std::endl<<"  _transact_X"
<<std::endl<<"		 Choice from _transact_X.Ñîñòîÿíèå_òğàíñïîğòèğîâêè = îæèäàåò and"
<<std::endl<<"		_transact_X.Ìåñòî_íàõîæäåíèÿ = "<<getName().c_str()
<<std::endl<<"			first"
<<std::endl	 
<<std::endl
<<std::endl<<"	_block"
<<std::endl<<"		  Choice from 1=1"
<<std::endl<<"			first"
<<std::endl<<"		Convert_rule	"
<<std::endl<<"		 Êîëëè÷åñòâî_óäàëåííûõ set _block.Êîëëè÷åñòâî_óäàëåííûõ + 1"
<<std::endl	 
<<std::endl<<"$End";

}

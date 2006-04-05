// rdoprocess_shape_terminate_MJ.cpp: implementation of the RPShapeTerminate_MJ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "rdo_process.h"
#include "rdoprocess_shape_terminate_MJ.h"
#include "rdoprocess_shape_terminate_dlg1_MJ.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RPShapeTerminateMJ::RPShapeTerminateMJ( RPObject* _parent ):
	RPShape( _parent, _T("Terminate") )
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

void RPShapeTerminateMJ::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
RPShapeTerminateDlg1_MJ dlg(NULL,this);
dlg.DoModal();

}


RPShapeTerminateMJ::~RPShapeTerminateMJ()
{

}

void RPShapeTerminateMJ::generate_MJ()
{
	/*
	rpapp.RDOfiles->pattern <<endl<<endl<<"èìÿ ñëåäóşùåãî áëîêà - "<<id_next
	<<endl<<"èìÿ - "<<getName().c_str();
*/

// ÃÅÍÅĞÀÖÈß ğåñóğñîâ ĞÄÎ ÔÀÉË *.res


	rpapp.RDOfiles->resourse<<endl<<"{-------áëîê process ------" <<getName().c_str()<<"-------------------}" <<endl

	<<endl<<"Block_Del_"<<getName().c_str()<<" : Block_Del 0";






// ÃÅÍÅĞÀÖÈß ïàòòåğíîå ĞÄÎ ÔÀÉË *.pat


rpapp.RDOfiles->pattern <<endl
<<endl<<"{-------áëîê óäàëåíèÿ------------"<<getName().c_str()<<"---------------------------------}"
<<endl
<<endl
<<endl<<"$Pattern Áëîê_óäàëåíèÿ_"<<getName().c_str()<<" : rule  {ñğàáàòûâàíèå çàêîíà}trace"
<<endl<<"	$Relevant_resources"
<<endl

<<endl<<"		_block : Block_Del_"<<getName().c_str()<<" Keep"
<<endl<<"			{ïåğå÷èñëèòü âñå ãğóïïû òğàíçàêòîâ êîòîğûå ñîçäàşòñÿ - ÂĞÓ×ÅÓŞ ÏÎËÜÇÎÂÀÒÅËÅÌ}"
<<endl<<"		_transact_X : Group_of_transacts_X Erase"
<<endl
<<endl
<<endl
<<endl<<"$Body"
<<endl 
<<endl<<"  _transact_X"
<<endl<<"		 Choice from _transact_X.Ñîñòîÿíèå_òğàíñïîğòèğîâêè = îæèäàåò and"
<<endl<<"		_transact_X.Ìåñòî_íàõîæäåíèÿ = "<<getName().c_str()
<<endl<<"			first"
<<endl	 
<<endl
<<endl<<"	_block"
<<endl<<"		  Choice from 1=1"
<<endl<<"		 Êîëëè÷åñòâî_óäàëåííûõ set _block.Êîëëè÷åñòâî_óäàëåííûõ+1"
<<endl	 
<<endl<<"$End";

}

// rdoprocess_shape_resource.cpp: implementation of the RPShapeResource_MJ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "rdo_process.h"
#include "rdoprocess_shape_resource.h"
#include "rdoprocess_shape_resource_DLG1_MJ.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RPShapeResource_MJ::RPShapeResource_MJ( RPObject* _parent ):
	RPShape( _parent, _T("Resource") )
{
	
	gamount=1;

	// инициализация типа блока 
	type ="resource";
		
	pa_src.push_back( rp::point(-50, -10) );
	pa_src.push_back( rp::point(-35, -25) );
	pa_src.push_back( rp::point(35, -25) );
	pa_src.push_back( rp::point(50, -10) );
	pa_src.push_back( rp::point(50, 10) );

	pa_src.push_back( rp::point(35, 25) );
	pa_src.push_back( rp::point(-35, 25) );
	pa_src.push_back( rp::point(-50, 10) );
	pa_src.push_back( rp::point(-50, -10) );

	pa_src.push_back( rp::point(50, -10) );
	pa_src.push_back( rp::point(50, 10) );
	pa_src.push_back( rp::point(-50, 10) );

docks.push_back( new RPConnectorDockResource( this, RPConnectorDock::out,  rp::point( 0, -25 ), 90 ) );
}

RPShapeResource_MJ::~RPShapeResource_MJ()
{

}


void RPShapeResource_MJ::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	RPShapeResourceDLG1_MJ dlg(NULL,this);
	dlg.DoModal();

}


void RPShapeResource_MJ::generate_MJ()
{
	rpapp.RDOfiles->pattern <<endl<<endl<<"имя следующего блока - "<<id_next
	<<endl<<"имя - "<<getName().c_str()
	<<endl<<"всего может обслуживать одновременно - "<<gamount;
}

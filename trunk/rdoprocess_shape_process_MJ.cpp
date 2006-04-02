// rdoprocess_shape_process_MJ.cpp: implementation of the RPShapeProcessMJ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <list>
//#include "rdo_process.h"
#include "rdoprocess_shape_process_MJ.h"
#include "rdoprocess_shape_process_dlg1_MJ.h"
#include "rdoprocess_app.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RPShapeProcessMJ::RPShapeProcessMJ( RPObject* _parent ):
	RPShape( _parent, _T("Process") )
{
	
	gname; // имя
	
	gtype = 0;
	base_gen = 1234567;
	
	//атрибуты законов
	gexp=0;
	gdisp=0;
	gmax=0;
	gmin=0;

	action=0; // тип действия по отношению к ресурсу
	prior=0;
	queue=0;
	parameter=888; // ПОКА НЕ ТРОГАЛ ЭТОТ ПАРАМЕТР
		
		// инициализация типа блока 
	type ="block";
		
	pa_src.push_back( rp::point(-50, -25) );
	pa_src.push_back( rp::point(50, -25) );
	pa_src.push_back( rp::point(50, 25) );
	pa_src.push_back( rp::point(-50, 25) );
	pa_src.push_back( rp::point(-50, -25) );

	docks.push_back( new RPConnectorDock( this, RPConnectorDock::in,  rp::point( -50, 0 ), 180 ) );
	docks.push_back( new RPConnectorDockOne( this, RPConnectorDock::out, rp::point(  50, 0 ), 0 ) );
	docks.push_back( new RPConnectorDock( this, RPConnectorDock::in,  rp::point(  0, 25), 270 ) );
}

RPShapeProcessMJ::~RPShapeProcessMJ()
{
}

void RPShapeProcessMJ::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	RPShapeProcessDlg1_MJ dlg(NULL,this);
	dlg.DoModal();
//	rpapp.project().list_name();
}

void RPShapeProcessMJ::list_name()
{
	RPShape::list_name();
	TRACE( "*** Process ***\n" );
}


void RPShapeProcessMJ::generate_MJ()
{
	RPShape::generate_MJ();

rpapp.RDOfiles->pattern <<endl<<endl<<"имя следующего блока - "<<id_next
<<endl<<"имя - "<<getName().c_str()

<<endl<<"закон прибытия - "<<gtype
<<endl<<"база генератора - "<<base_gen
<<endl<<"атрибуты закона - "<<"exp - "<<gexp<<" disp"<<gdisp<<" max - " <<gmax<<" min - "<<gmin
<<endl<<"действие"<<action
<<endl<<"закон"<<queue<<endl

<<"РЕСУРСЫ :";

std::list<CString>::iterator it = list_resource_procMJ.begin();
	while( it != list_resource_procMJ.end() ) 
	{
	rpapp.RDOfiles->pattern <<(*it)<<"     ";
	it++;
	}	
}
// rdoprocess_shape_process_MJ.cpp: implementation of the RPShapeProcessMJ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <list>
#include "rdoprocess_shape_process_MJ.h"
#include "rdoprocess_shape_process_dlg1_MJ.h"
#include "rdoprocess_method_proc2rdo_MJ.h"
#include "rdo_studio/rdostudioapp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
RPShapeProcessMJ::RPShapeProcessMJ( RPObject* _parent ):
	RPShape_MJ( _parent, _T("Process") )
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

	docks.push_back( new RPConnectorDock( this, RPConnectorDock::in, rp::point( -50, 0 ), 180, "transact" ) );
	docks.push_back( new RPConnectorDockOne( this, RPConnectorDock::out, rp::point(  50, 0 ), 0, "transact" ) );
	docks.push_back( new RPConnectorDock( this, RPConnectorDock::in,  rp::point(  0, 25), 270, "resource" ) );
}

RPShapeProcessMJ::~RPShapeProcessMJ()
{
}

RPObject* RPShapeProcessMJ::newObject( RPObject* parent )
{
	return new RPShapeProcessMJ( parent );
}

void RPShapeProcessMJ::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	RPShapeProcessDlg1_MJ dlg( AfxGetMainWnd(), this );
	dlg.DoModal();
}

void RPShapeProcessMJ::generate()
{
	std::vector <double>  params;
	params.push_back(static_cast<double>(gtype));
	params.push_back(static_cast<double>(base_gen));
	params.push_back(gexp);
	params.push_back(gdisp);
	params.push_back(static_cast<double>(action));
	studioApp.broadcastMessage(RDOThread::RT_PROCGUI_BLOCK_PROCESS,&params);
}
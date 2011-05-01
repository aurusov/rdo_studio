// rdoprocess_shape_terminate_MJ.cpp: implementation of the RPShapeTerminate_MJ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "rdoprocess_shape_terminate_MJ.h"
#include "rdoprocess_shape_terminate_dlg1_MJ.h"
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

RPShapeTerminateMJ::RPShapeTerminateMJ( RPObject* _parent ):
	RPShape_MJ( _parent, _T("Terminate") )
{
	m_time=100;
	m_term_counter=1;

	// инициализация типа блока 
	type ="block";
		
	pa_src.push_back( rp::point(-50, 0) );
	pa_src.push_back( rp::point(-25, -25) );
	pa_src.push_back( rp::point(50, -25) );
	pa_src.push_back( rp::point(50, 25) );
	pa_src.push_back( rp::point(-25, 25) );
	pa_src.push_back( rp::point(-50, 0) );

	docks.push_back( new RPConnectorDock( this, RPConnectorDock::in,  rp::point( -50, 0 ), 180, "transact" ) );
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
	RPShapeTerminateDlg1_MJ dlg( AfxGetMainWnd(), this );
	dlg.DoModal();
}

void RPShapeTerminateMJ::generate()
{
	std::vector <double>  params;
	params.push_back(static_cast<double>(m_term_state));
	params.push_back(m_time);
	params.push_back(m_term_counter);
	studioApp.broadcastMessage(RDOThread::RT_PROCGUI_BLOCK_TERMINATE,&params);
}

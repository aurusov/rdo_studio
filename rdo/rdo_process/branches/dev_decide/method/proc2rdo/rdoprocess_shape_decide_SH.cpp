// rdoprocess_shape_create_MJ.cpp: implementation of the RPShapeDecideSH class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "rdoprocess_shape_decide_SH.h"
//#include "rdoprocess_shape_create_dlg1_MJ.h"
#include "rdoprocess_method_proc2rdo_MJ.h"
#include <rdoprocess_shape.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RPShapeDecideSH::RPShapeDecideSH( RPObject* _parent ):
	RPShape_MJ( _parent, _T("_Decide") )
{
	pa_src.push_back( rp::point(55, 0) );
	pa_src.push_back( rp::point(0, 55) );
	pa_src.push_back( rp::point(-55, 0) );
	pa_src.push_back( rp::point(0, -55) );
	pa_src.push_back( rp::point(55, 0) );
	//pa_src.push_back( rp::point(-50, -25) );

	//docks.push_back( new RPConnectorDockOne( this, RPConnectorDock::out, rp::point(  50, 0 ), 0, "transact" ) );
}

RPShapeDecideSH::~RPShapeDecideSH()
{
}

rp::RPXMLNode* RPShapeDecideSH::save( rp::RPXMLNode* parent_node )
{
	rp::RPXMLNode* obj_node = RPShape_MJ::save( parent_node );
	return obj_node;
}

RPObject* RPShapeDecideSH::newObject( RPObject* parent )
{
	return new RPShapeDecideSH( parent );
}
/*
void RPShapeDecideSH::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	RPShapeCreateDlg1_MJ dlg( AfxGetMainWnd(), this );
	dlg.DoModal();
}

void RPShapeDecideSH::generate()
{
}
*/
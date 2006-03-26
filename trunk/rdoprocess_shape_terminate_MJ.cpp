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
	pa_src.push_back( rp::point(-25, 0) );
	pa_src.push_back( rp::point(-15, -25) );
	pa_src.push_back( rp::point(20, -25) );
	pa_src.push_back( rp::point(20, 25) );
	pa_src.push_back( rp::point(-15, 25) );
	pa_src.push_back( rp::point(-25, 0) );

	matrix_scale.data[0][0] = 2;
}

void RPShapeTerminateMJ::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
RPShapeTerminateDlg1_MJ dlg(NULL,this);
dlg.DoModal();

}


RPShapeTerminateMJ::~RPShapeTerminateMJ()
{

}

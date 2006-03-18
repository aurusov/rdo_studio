// rdoprocess_shape_create_MJ.cpp: implementation of the RPShapeCreateMJ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "rdo_process.h"
#include "rdoprocess_shape_create_MJ.h"
#include "rdoprocess_shape_create_dlg1_MJ.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RPShapeCreateMJ::RPShapeCreateMJ( RPObject* _parent ):
	RPShape( _parent, _T("Create") )
{
	pa_src.push_back( rp::point(-40, -25) );
	pa_src.push_back( rp::point(20, -25) );
	pa_src.push_back( rp::point(40, 0) );
	pa_src.push_back( rp::point(20, 25) );
	pa_src.push_back( rp::point(-40, 25) );
	pa_src.push_back( rp::point(-40, -25) );

	matrix_scale.data[0][0] = 2;
}


void RPShapeCreateMJ::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
RPShapeCreateDlg1_MJ dlg;
dlg.DoModal();

}




RPShapeCreateMJ::~RPShapeCreateMJ()
{

}

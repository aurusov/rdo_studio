// rdoprocess_shape_process_MJ.cpp: implementation of the RPShapeProcessMJ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "rdo_process.h"
#include "rdoprocess_shape_process_MJ.h"
#include "rdoprocess_shape_process_dlg1_MJ.h"

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
	pa_src.push_back( rp::point(-25, -25) );
	pa_src.push_back( rp::point(25, -25) );
	pa_src.push_back( rp::point(25, 25) );
	pa_src.push_back( rp::point(-25, 25) );
	pa_src.push_back( rp::point(-25, -25) );

	matrix_scale.data[0][0] = 2;
}

void RPShapeProcessMJ::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
RPShapeProcessDlg1_MJ dlg(NULL,this);
dlg.DoModal();

}	
	
RPShapeProcessMJ::~RPShapeProcessMJ()
{

}

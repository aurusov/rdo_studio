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
	pa_src.push_back( rp::point(-20, -25) );
	pa_src.push_back( rp::point(15, -25) );
	pa_src.push_back( rp::point(25, 0) );
	pa_src.push_back( rp::point(15, 25) );
	pa_src.push_back( rp::point(-20, 25) );
	pa_src.push_back( rp::point(-20, -25) );

	matrix_scale.data[0][0] = 2;

	// инициализация параметров для генерирования
	gname; // имя
	 gfirst=0; // время первого
	gamount=1000000; // кол-во создаваемых
	gtype=0; // закон прибытия
	base_gen=1234567890;
	//атрибуты законов
	gexp=0;
	gdisp=0;
	gmax=0;
	gmin=0;

	//второе окно
	inf=1000000; // бесконечноть
	gID=1; // ID группы
	gpar1=0;
	gpar2=0;
	gpar3=0;




}

RPShapeCreateMJ::~RPShapeCreateMJ()
{
}

void RPShapeCreateMJ::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	RPShapeCreateDlg1_MJ dlg( NULL, this );
	dlg.DoModal();
}

void RPShapeCreateMJ::list_name()
{
	RPShape::list_name();
	TRACE( "*** Create ***\n" );
}

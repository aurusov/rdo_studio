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
	pa_src.push_back( rp::point(-50, -25) );
	pa_src.push_back( rp::point(25, -25) );
	pa_src.push_back( rp::point(50, 0) );
	pa_src.push_back( rp::point(25, 25) );
	pa_src.push_back( rp::point(-50, 25) );
	pa_src.push_back( rp::point(-50, -25) );

	docks.push_back( new RPConnectorDock( this, RPConnectorDock::in,  rp::point( -50, 0 ), 180 ) );
	docks.push_back( new RPConnectorDockOne( this, RPConnectorDock::out, rp::point(  50, 0 ), 0 ) );

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

/*void RPShapeCreateMJ::list_name()
{
	RPShape::list_name();
	TRACE( "*** Create ***\n" );
}
*/


void RPShapeCreateMJ::generate_MJ()
{
rpapp.RDOfiles->pattern <<"имя следующего блока - "<<id_next
<<endl<<"имя - "<<getName().c_str()
<<endl<<"время первого - "<<gfirst
<<endl<< "кол-во создаваемых - "<<gamount
<<endl<<"закон прибытия - "<<gtype
<<endl<<"база генератора - "<<base_gen
<<endl<<"атрибуты закона - "<<"exp - "<<gexp<<" disp"<<gdisp<<" max - " <<gmax<<" min - "<<gmin
<<endl<<"второе окно! бесконечноть - "<<inf
<<endl<<"ID группы - "<<gID
<<endl<<"параметры - "<<"1-  "<<gpar1<< " 2 - "<<gpar2<<" 3 - "<<gpar3<<endl;
}
// rdoprocess_shape_create_MJ.cpp: implementation of the RPShapeCreateMJ class.
//
//////////////////////////////////////////////////////////////////////
#include "rdo_studio/rdo_process/proc2rdo/stdafx.h"
#include "rdoprocess_shape_create_MJ.h"
#include "rdoprocess_shape_create_dlg1_MJ.h"
#include "rdoprocess_method_proc2rdo_MJ.h"
#include "rdo_studio/rdo_process/rp_method/rdoprocess_shape.h"
#include "rdo_studio/rdostudioapp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RPShapeCreateMJ::RPShapeCreateMJ( RPObject* _parent ):
	RPShape_MJ( _parent, _T("Create") )
{
	pa_src.push_back( rp::point(-50, -25) );
	pa_src.push_back( rp::point(25, -25) );
	pa_src.push_back( rp::point(50, 0) );
	pa_src.push_back( rp::point(25, 25) );
	pa_src.push_back( rp::point(-50, 25) );
	pa_src.push_back( rp::point(-50, -25) );

	docks.push_back( new RPConnectorDockOne( this, RPConnectorDock::out, rp::point(  50, 0 ), 0, "transact" ) );

	// инициализация параметров для генерирования
	gname; // имя
	gfirst=0; // время первого
	gamount=100; // кол-во создаваемых
	gtype=0; // закон прибытия
	base_gen=1234567890;
	//атрибуты законов
	gexp=0;
	gdisp=0;

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

rp::RPXMLNode* RPShapeCreateMJ::save( rp::RPXMLNode* parent_node )
{
	rp::RPXMLNode* obj_node = RPShape_MJ::save( parent_node );
	return obj_node;
}

RPObject* RPShapeCreateMJ::newObject( RPObject* parent )
{
	return new RPShapeCreateMJ( parent );
}

void RPShapeCreateMJ::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	RPShapeCreateDlg1_MJ dlg( AfxGetMainWnd(), this );
	dlg.DoModal();
}

/*void RPShapeCreateMJ::list_name()
{
	RPShape_MJ::list_name();
	TRACE( "*** Create ***\n" );
}
*/


void RPShapeCreateMJ::generate()
{
	std::vector <double>  params;
	params.push_back(static_cast<double>(gtype));
	params.push_back(static_cast<double>(base_gen));
	params.push_back(gfirst);
	params.push_back(gamount);
	params.push_back(gexp);
	params.push_back(gdisp);

	studioApp.broadcastMessage(RDOThread::RT_PROCGUI_BLOCK_CREATE,&params);

	/*
	RDOfiles->function <<"имя следующего блока - "<<id_next
	<<std::endl<<"имя - "<<getName().c_str()
	<<std::endl<<"время первого - "<<gfirst
	<<std::endl<< "кол-во создаваемых - "<<gamount
	<<std::endl<<"закон прибытия - "<<gtype
	<<std::endl<<"база генератора - "<<base_gen
	<<std::endl<<"атрибуты закона - "<<"exp - "<<gexp<<" disp"<<gdisp<<" max - " <<gmax<<" min - "<<gmin
	<<std::endl<<"второе окно! бесконечноть - "<<inf
	<<std::endl<<"ID группы - "<<gID
	<<std::endl<<"параметры - "<<"1-  "<<gpar1<< " 2 - "<<gpar2<<" 3 - "<<gpar3;
	*/

	/*
	int cur = gtype;
	switch(cur) // определяем активные окна исходя из закона
	{
		case 0: // константа 
			break;	
		case 1: // нормальный
			break;
		case 2: // равномерный закон
			break;
		case 3: // экспоненциальный
			break;
	}*/


}
// rdoprocess_shape_resource.cpp: implementation of the RPShapeResource_MJ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "rdo_process.h"
#include "rdoprocess_shape_resource.h"
#include "rdoprocess_shape_resource_DLG1_MJ.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RPShapeResource_MJ::RPShapeResource_MJ( RPObject* _parent ):
	RPShape( _parent, _T("Resource") )
{
	
	gamount=1;

	// инициализация типа блока 
	type ="resource";
		
	pa_src.push_back( rp::point(-50, -10) );
	pa_src.push_back( rp::point(-35, -25) );
	pa_src.push_back( rp::point(35, -25) );
	pa_src.push_back( rp::point(50, -10) );
	pa_src.push_back( rp::point(50, 10) );

	pa_src.push_back( rp::point(35, 25) );
	pa_src.push_back( rp::point(-35, 25) );
	pa_src.push_back( rp::point(-50, 10) );
	pa_src.push_back( rp::point(-50, -10) );

	pa_src.push_back( rp::point(50, -10) );
	pa_src.push_back( rp::point(50, 10) );
	pa_src.push_back( rp::point(-50, 10) );

docks.push_back( new RPConnectorDockResource( this, RPConnectorDock::out,  rp::point( 0, -25 ), 90 ) );
}

RPShapeResource_MJ::~RPShapeResource_MJ()
{

}


void RPShapeResource_MJ::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	RPShapeResourceDLG1_MJ dlg(NULL,this);
	dlg.DoModal();

}


void RPShapeResource_MJ::generate_MJ()
{
	/*rpapp.RDOfiles->pattern <<endl<<endl<<"имя следующего блока - "<<id_next
	<<endl<<"имя - "<<getName().c_str()
	<<endl<<"всего может обслуживать одновременно - "<<gamount;
*/

	
// ГЕНЕРАЦИЯ ФУНКЦИЙ РДО ФАЙЛ *.fun
rpapp.RDOfiles->function<<endl<<"{-------ресурс ------" <<getName().c_str()<<"-------------------}" <<endl
	
	
	
	
<<endl<<"$Function  fun_resource_" <<getName().c_str()<<" : such_as Resource.Состояние {освобождение или занятие ресурса}"
<<endl<<"	$Type = algorithmic"
<<endl<<"	$Parameters"
<<endl<<"		 _amount        : integer {сколько транзактов уже обслуживается}"
<<endl<<"		 _state         : such_as Resource.Состояние {состяние ресурса}"
<<endl<<"		 _max           : integer      { макс колличество которое может обслуживаться"
<<endl<<"после которого рессурс переходит в состяоние занят}"
<<endl
<<endl<<"$Body"
<<endl<<"	Calculate_if _amount >= _max and _state = свободен"
<<endl<<"		fun_resource_" <<getName().c_str()<<" = занят"
<<endl<<"	{занимает если кол-во превысило максимум}"
<<endl 
<<endl<<"	Calculate_if _amount < _max and _state = занят"
<<endl<<"		fun_resource_" <<getName().c_str()<<" = свободен"
<<endl    
<<endl<<"	Calculate_if 1=1"
<<endl<<"		fun_resource_" <<getName().c_str()<<" = _state"
<<endl
<<endl<<"$End";



// ГЕНЕРАЦИЯ ресурсов РДО ФАЙЛ *.res
rpapp.RDOfiles->resourse<<endl<<endl<<"{-------ресурс ------" <<getName().c_str()<<"-------------------}" <<endl

<<"Resource_"<<getName().c_str()<<" : Resource свободен 0 1";








}

// rdoprocess_shape_resource.cpp: implementation of the RPShapeResource_MJ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "rdo_process.h"
#include "rdoprocess_shape_resource.h"
#include "rdoprocess_shape_resource_DLG1_MJ.h"
#include "rdo_process_project_RDO_proc_MJ.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RPShapeResource_MJ::RPShapeResource_MJ( RPObject* _parent ):
	RPShape_MJ( _parent, _T("Resource") )
{
	
	gamount=1;

	// ������������� ���� ����� 

	type ="resource";

	main_pen_default.DeleteObject();
	main_pen_default.CreatePen( PS_SOLID, main_pen_width, RGB( 0x00, 0x00, 0x00 ) );
	setPen( main_pen_default );
	bg_brush.DeleteObject();
	bg_brush.CreateSolidBrush( RGB(0xF0, 0xF0, 0xF0) );

	pa_src.push_back( rp::point(-50, -10) );
	pa_src.push_back( rp::point(-35, -25) );
	pa_src.push_back( rp::point(35, -25) );
	pa_src.push_back( rp::point(50, -10) );
	pa_src.push_back( rp::point(50, 10) );
	pa_src.push_back( rp::point(35, 25) );
	pa_src.push_back( rp::point(-35, 25) );
	pa_src.push_back( rp::point(-50, 10) );
	pa_src.push_back( rp::point(50, 10) );
	pa_src.push_back( rp::point(-50, 10) );
	pa_src.push_back( rp::point(-50, -10) );
	pa_src.push_back( rp::point(50, -10) );
	pa_src.push_back( rp::point(-50, -10) );

/*
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
*/
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

void RPShapeResource_MJ::generate()
{
	/*RDOfiles->pattern <<endl<<endl<<"��� ���������� ����� - "<<id_next
	<<endl<<"��� - "<<getName().c_str()
	<<endl<<"����� ����� ����������� ������������ - "<<gamount;
*/

	RPCreationRDOFilesMJ* RDOfiles = static_cast<RPProject_RDO_Proc_MJ*>(rpapp.project())->RDOfiles;

	// ��������� ���������� ��� ���� *.pmd
	RDOfiles->statistic

	<<endl<<"���������_�������_" <<getName().c_str()<<
	  ":  watch_state "<<"Resource_"<<getName().c_str()<<".��������� = �����";




	
// ��������� ������� ��� ���� *.fun
RDOfiles->function<<endl<<"{-------������ ------" <<getName().c_str()<<"-------------------}" <<endl
	
	
	
	
<<endl<<"$Function  fun_resource_" <<getName().c_str()<<" : such_as Resource.��������� {������������ ��� ������� �������}"
<<endl<<"	$Type = algorithmic"
<<endl<<"	$Parameters"
<<endl<<"		 _amount        : integer {������� ���������� ��� �������������}"
<<endl<<"		 _state         : such_as Resource.��������� {�������� �������}"
<<endl<<"		 _max           : integer      { ���� ����������� ������� ����� �������������"
<<endl<<"����� �������� ������� ��������� � ��������� �����}"
<<endl
<<endl<<"$Body"
<<endl<<"	Calculate_if _amount >= _max and _state = ��������"
<<endl<<"		fun_resource_" <<getName().c_str()<<" = �����"
<<endl<<"	{�������� ���� ���-�� ��������� ��������}"
<<endl 
<<endl<<"	Calculate_if _amount < _max and _state = �����"
<<endl<<"		fun_resource_" <<getName().c_str()<<" = ��������"
<<endl    
<<endl<<"	Calculate_if 1=1"
<<endl<<"		fun_resource_" <<getName().c_str()<<" = _state"
<<endl
<<endl<<"$End";



// ��������� �������� ��� ���� *.res
RDOfiles->resourse<<endl<<endl<<"{-------������ ------" <<getName().c_str()<<"-------------------}" <<endl

<<"Resource_"<<getName().c_str()<<" : Resource �������� 0 1";








}

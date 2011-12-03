// rdoprocess_shape_resource.cpp: implementation of the RPShapeResource_MJ class.
//
//////////////////////////////////////////////////////////////////////

#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_resource.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_resource_DLG1.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_method_proc2rdo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// --------------------------------------------------------------------------------
// -------------------- RPConnectorDockResourceOut
// --------------------------------------------------------------------------------
class RPConnectorDockResourceOut: public RPConnectorDock
{
public:
	RPConnectorDockResourceOut( RPShape* _parent, Type _type, const rp::point& _point, double _norm, const rp::string& type = "" ): RPConnectorDock( _parent, _type, _point, _norm, type ) {};
	virtual ~RPConnectorDockResourceOut() {};

	virtual RPConnector* make_connector( RPObject* _parent ) {
		RPConnector* conn = RPConnectorDock::make_connector( _parent );
		conn->setPen( CPen(PS_DOT, 1, RGB(0x00, 0x00, 0x80)) );
		return conn;
	}
};

// --------------------------------------------------------------------------------
// -------------------- RPShapeResource_MJ
// --------------------------------------------------------------------------------
RPShapeResource_MJ::RPShapeResource_MJ( RPObject* _parent ):
	RPShape_MJ( _parent, _T("Resource") )
{
	
	gamount=1;

	// инициализация типа блока 

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
	docks.push_back( new RPConnectorDockResourceOut( this, RPConnectorDock::out,  rp::point( 0, -25 ), 90, "resource" ) );
}

RPShapeResource_MJ::~RPShapeResource_MJ()
{
}

RPObject* RPShapeResource_MJ::newObject( RPObject* parent )
{
	return new RPShapeResource_MJ( parent );
}

void RPShapeResource_MJ::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	UNUSED(nFlags          );
	UNUSED(global_chart_pos);

	RPShapeResourceDLG1_MJ dlg( AfxGetMainWnd(), this );
	dlg.DoModal();
}

void RPShapeResource_MJ::generate()
{
	/*RDOfiles->pattern <<std::endl<<std::endl<<"имя следующего блока - "<<id_next
	<<std::endl<<"имя - "<<getName().c_str()
	<<std::endl<<"всего может обслуживать одновременно - "<<gamount;
*/

	RPCreationRDOFilesMJ* RDOfiles = proc2rdo->RDOfiles;

	// ГЕНЕРАЦИЯ статистики РДО ФАЙЛ *.pmd
	RDOfiles->statistic

	<<std::endl<<"Занятость_Ресурса_" <<getName().c_str()<<
	  ":  watch_state "<<"Resource_"<<getName().c_str()<<".Состояние = занят";




	
// ГЕНЕРАЦИЯ ФУНКЦИЙ РДО ФАЙЛ *.fun
RDOfiles->function<<std::endl<<"{-------ресурс ------" <<getName().c_str()<<"-------------------}" <<std::endl
	
	
	
	
<<std::endl<<"$Function  fun_resource_" <<getName().c_str()<<" : such_as Resource.Состояние {освобождение или занятие ресурса}"
<<std::endl<<"	$Type = algorithmic"
<<std::endl<<"	$Parameters"
<<std::endl<<"		 _amount        : integer {сколько транзактов уже обслуживается}"
<<std::endl<<"		 _state         : such_as Resource.Состояние {состяние ресурса}"
<<std::endl<<"		 _max           : integer      { макс колличество которое может обслуживаться"
<<std::endl<<"после которого рессурс переходит в состяоние занят}"
<<std::endl
<<std::endl<<"$Body"
<<std::endl<<"	Calculate_if _amount >= _max and _state = свободен"
<<std::endl<<"		fun_resource_" <<getName().c_str()<<" = занят"
<<std::endl<<"	{занимает если кол-во превысило максимум}"
<<std::endl 
<<std::endl<<"	Calculate_if _amount < _max and _state = занят"
<<std::endl<<"		fun_resource_" <<getName().c_str()<<" = свободен"
<<std::endl    
<<std::endl<<"	Calculate_if 1=1"
<<std::endl<<"		fun_resource_" <<getName().c_str()<<" = _state"
<<std::endl
<<std::endl<<"$End";



// ГЕНЕРАЦИЯ ресурсов РДО ФАЙЛ *.res
RDOfiles->resourse<<std::endl<<std::endl<<"{-------ресурс ------" <<getName().c_str()<<"-------------------}" <<std::endl

<<"Resource_"<<getName().c_str()<<" : Resource свободен 0 1";

}

void RPShapeResource_MJ::saveToXML(REF(pugi::xml_node) parentNode) const
{
	// Записываем узел <RPShapeResource_MJ/>:
	pugi::xml_node node = parentNode.append_child(getClassName().c_str());
	// Соxраняем атрибуты объекта:
	// 1) Атрибуты графики
	RPObjectMatrix::saveToXML(node);
	RPShape::       saveToXML(node);
	// 2) Атрибуты симулятора
	node.append_attribute(_T("name"))     .set_value(getName().c_str());
	node.append_attribute(_T("amount"))   .set_value(gamount          );
}

void RPShapeResource_MJ::loadFromXML(CREF(pugi::xml_node) node)
{
	// Считываем атрибуты для загрузки сохраненного блока "Resource":
	for(pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute())
	{
		// Присваиваем сохраненные в xml-файле параметры:
		// Для симулятора (диалоговые окна)
		tstring attrName = attr.name();
		if (attrName == _T("name"))
		{
			setName(attr.value());
		}
		else if (attrName == _T("amount"))
		{
			gamount = attr.as_int();
		}
	}
	// Отображения объекта на Flowchart'е
	RPObjectMatrix::loadFromXML(node);
	RPShape::       loadFromXML(node);
}

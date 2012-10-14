#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_resource.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_resource_dlg.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_method_proc2rdo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

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
// -------------------- RPProcessShapeResource
// --------------------------------------------------------------------------------
RPProcessShapeResource::RPProcessShapeResource( RPObject* _parent ):
	RPProcessShape( _parent, _T("Resource") )
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

RPProcessShapeResource::~RPProcessShapeResource()
{
}

RPObject* RPProcessShapeResource::newObject( RPObject* parent )
{
	return new RPProcessShapeResource( parent );
}

void RPProcessShapeResource::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	UNUSED(nFlags          );
	UNUSED(global_chart_pos);

	RPProcessShapeResourceDlg1 dlg( AfxGetMainWnd(), this );
	dlg.DoModal();
}

void RPProcessShapeResource::generate()
{
	NEVER_REACH_HERE;
}

void RPProcessShapeResource::saveToXML(REF(pugi::xml_node) parentNode) const
{
	// Записываем узел <RPProcessShapeResource/>:
	pugi::xml_node node = parentNode.append_child(getClassName().c_str());
	// Соxраняем атрибуты объекта:
	// 1) Атрибуты графики
	RPObjectMatrix::saveToXML(node);
	RPShape::       saveToXML(node);
	// 2) Атрибуты симулятора
	node.append_attribute(_T("name"))     .set_value(getName().c_str());
	node.append_attribute(_T("amount"))   .set_value(gamount          );
}

void RPProcessShapeResource::loadFromXML(CREF(pugi::xml_node) node)
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

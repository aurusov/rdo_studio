#include "app/rdo_studio_mfc/rdo_process/rp_method/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object_chart.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object_flowchart.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_xml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RPObjectChart
// --------------------------------------------------------------------------------
RPObjectChart::RPObjectChart( RPObject* _parent, const rp::string& _name ):
	RPObject( _parent, _name ),
	main_pen_width( 2 ),
	can_update( false )
{
	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = RGB(0x00, 0x00, 0x00);
	lb.lbHatch = 0;
	main_pen_default.CreatePen( PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE | PS_JOIN_MITER, main_pen_width, &lb );
	setPen( main_pen_default );
	can_update = true;
}

RPObjectChart::~RPObjectChart()
{
}

void RPObjectChart::load( rp::RPXMLNode* node )
{
	RPObject::load( node );
	rp::RPXMLNode* pen_node = node->findFirstChild( "pen" );
	LOGPEN lp;
	if ( main_pen.GetLogPen( &lp ) ) {
		lp.lopnWidth.x = pen_node->getAttributeInt( "width" );
		lp.lopnStyle   = pen_node->getAttributeInt( "style" );
		lp.lopnColor   = RPObjectChart::strToColor( pen_node->getAttribute( "color" ) );
		CPen pen;
		pen.CreatePenIndirect( &lp );
		setPen( pen );
	} else {
		EXTLOGPEN lp;
		if ( main_pen.GetExtLogPen( &lp ) ) {
			lp.elpWidth    = pen_node->getAttributeInt( "width" );
			lp.elpPenStyle = pen_node->getAttributeInt( "style" );
			lp.elpColor    = RPObjectChart::strToColor( pen_node->getAttribute( "color" ) );
			LOGBRUSH lb;
			lb.lbStyle     = lp.elpBrushStyle;
			lb.lbColor     = lp.elpColor;
			lb.lbHatch     = 0;
			CPen pen;
			pen.CreatePen( lp.elpPenStyle, lp.elpWidth, &lb );
			setPen( pen );
		}
	}
}

rp::RPXMLNode* RPObjectChart::save( rp::RPXMLNode* parent_node )
{
	rp::RPXMLNode* obj_node = RPObject::save( parent_node );
	LOGPEN lp;
	if ( main_pen.GetLogPen( &lp ) ) {
		rp::RPXMLNode* pen_node = obj_node->makeChild( "pen" );
		pen_node->insertAttributeInt( "width", lp.lopnWidth.x );
		pen_node->insertAttributeInt( "style", lp.lopnStyle );
		pen_node->insertAttribute( "color", RPObjectChart::colorToStr(lp.lopnColor) );
	} else {
		EXTLOGPEN lp;
		if ( main_pen.GetExtLogPen( &lp ) ) {
			rp::RPXMLNode* pen_node = obj_node->makeChild( "pen" );
			pen_node->insertAttributeInt( "width", lp.elpWidth );
			pen_node->insertAttributeInt( "style", lp.elpPenStyle );
			pen_node->insertAttribute( "color", RPObjectChart::colorToStr(lp.elpColor) );
		}
	}
	return obj_node;
}

void RPObjectChart::setPen( const CPen& pen )
{
	LOGPEN lp;
	if ( const_cast<CPen&>(pen).GetLogPen( &lp ) ) {
		main_pen.DeleteObject();
		main_pen.CreatePenIndirect( &lp );
		modify();
		update();
	} else {
		EXTLOGPEN lp;
		if ( const_cast<CPen&>(pen).GetExtLogPen( &lp ) ) {
			LOGBRUSH lb;
			lb.lbStyle = lp.elpBrushStyle;
			lb.lbColor = lp.elpColor;
			lb.lbHatch = 0;
			main_pen.DeleteObject();
			main_pen.CreatePen( lp.elpPenStyle, lp.elpWidth, &lb );
			modify();
			update();
		}
	}
}

RPObjectFlowChart* RPObjectChart::flowChart() const
{
	RPObjectChart* chart_parent = chartParent();
	RPObjectChart* prev = chart_parent;
	while ( chart_parent ) {
		prev = chart_parent;
		chart_parent = chart_parent->chartParent();
	}
	return static_cast<RPObjectFlowChart*>(prev);
}

void RPObjectChart::modify()
{
	RPObjectChart* flowchart = flowChart();
	if ( flowchart ) {
		flowchart->modify();
	}
}

void RPObjectChart::update()
{
	if ( !can_update ) return;
	RPObjectChart* flowchart = flowChart();
	if ( flowchart ) {
		flowchart->update();
	}
}

RPProject::Cursor RPObjectChart::getCursor( const rp::point& global_chart_pos )
{
	std::list< RPObjectChart* > objects;
	getChartObjects( objects );
	std::list< RPObjectChart* >::iterator it = objects.begin();
	while ( it != objects.end() ) {
		RPProject::Cursor cursor = (*it)->getCursor( global_chart_pos );
		if ( cursor != RPProject::cursor_flow_select ) return cursor;
		it++;
	}
	return RPProject::cursor_flow_select;
}

RPObjectChart* RPObjectChart::find( const rp::point& global_chart_pos )
{
	std::list< RPObjectChart* > objects;
	getChartObjects( objects );
	std::list< RPObjectChart* >::iterator it = objects.begin();
	while ( it != objects.end() ) {
		RPObjectChart* obj = (*it)->find( global_chart_pos );
		if ( obj ) return obj;
		it++;
	}
	return pointInShape( global_chart_pos ) ? this : NULL;
}

void RPObjectChart::draw( CDC& dc )
{
	// Вызов отрисовки потомков
	std::list< RPObjectChart* > objects;
	getChartObjects( objects );
	std::list< RPObjectChart* >::iterator it = objects.end();
	if ( it != objects.begin() ) it--;
	if ( it != objects.end() ) {
		while ( true ) {
			int saved = dc.SaveDC();
			(*it)->draw( dc );
			dc.RestoreDC( saved );
			if ( it == objects.begin() ) break;
			it--;
		}
	}
}

void RPObjectChart::draw_after( CDC& dc )
{
	// Вызов отрисовки потомков
	std::list< RPObjectChart* > objects;
	getChartObjects( objects );
	std::list< RPObjectChart* >::iterator it = objects.begin();
	while ( it != objects.end() ) {
		int saved = dc.SaveDC();
		(*it)->draw_after( dc );
		dc.RestoreDC( saved );
		it++;
	}
	if ( isSelected() ) draw_selected( dc );
}

void RPObjectChart::drawCustom(REF(CDC) dc)
{
	UNUSED(dc);
}

void RPObjectChart::setSelected( rbool value )
{
	rbool _sel = isSelected();
	RPObject::setSelected( value );
	if ( _sel != value ) {
		update();
	}
}

rp::string RPObjectChart::colorToStr( COLORREF color )
{
	switch ( color & 0xFFFFFF ) {
		case 0xFFFFFF: return "white";
		case 0x000000: return "black";
		case 0x0000FF: return "red";
		case 0x00FF00: return "green";
		case 0xFF0000: return "blue";
		case 0x00FFFF: return "yellow";
		case 0xFF00FF: return "magenta";
		case 0xFFFF00: return "cyan";
	}
	return rp::format( "#%02X%02X%02X", GetRValue(color), GetGValue(color), GetBValue(color) );
}

COLORREF RPObjectChart::strToColor( const rp::string& color )
{
	if ( color == "white" ) {
		return 0xFFFFFF;
	} else if ( color == "black" ) {
		return 0x000000;
	} else if ( color == "red" ) {
		return 0x0000FF;
	} else if ( color == "green" ) {
		return 0x00FF00;
	} else if ( color == "blue" ) {
		return 0xFF0000;
	} else if ( color == "yellow" ) {
		return 0x00FFFF;
	} else if ( color == "magenta" ) {
		return 0xFF00FF;
	} else if ( color == "cyan" ) {
		return 0xFFFF00;
	}
	rp::string str = color;
	str.erase( 0, 1 );
	COLORREF clr = str.tohex();
	return RGB( GetBValue(clr), GetGValue(clr), GetRValue(clr) );
}

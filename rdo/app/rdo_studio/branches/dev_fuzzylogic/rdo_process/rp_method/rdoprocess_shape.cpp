#include "app/rdo_studio_mfc/rdo_process/rp_method/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_shape.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object_flowchart.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_math.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_xml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RPShape
// --------------------------------------------------------------------------------
RPShape::RPShape( RPObject* _parent, const rp::string& _name ):
	RPObjectMatrix( _parent, _name ),
	pcmd( pcmd_none ),
	bg_brush( RGB(0xFF, 0xFF, 0xA0) ),
	text_color( RGB(0x00, 0x00, 0x00 ) ),
	text_show( true )
//	snap_to_point( 0, 0 )
{
	text_font.CreateStockObject( DEFAULT_GUI_FONT );
}

RPShape::~RPShape()
{
	std::vector< RPConnectorDock* >::iterator it = docks.begin();
	while ( it != docks.end() ) {
		RPConnectorDock* dock = *it;
		it = docks.erase( it );
		if ( dock ) delete dock;
	}
}

void RPShape::load( rp::RPXMLNode* node )
{
	RPObjectMatrix::load( node );

	rp::RPXMLNode* brush_node = node->findFirstChild( "brush" );
	LOGBRUSH lb;
	lb.lbColor = RPObjectChart::strToColor( brush_node->getAttribute( "color" ) );
	lb.lbStyle = brush_node->getAttributeInt( "style" );
	lb.lbHatch = 0;
	CBrush brush;
	brush.CreateBrushIndirect( &lb );
	setBgBrush( brush );

	rp::RPXMLNode* font_node = node->findFirstChild( "font" );
	rp::string font_name( font_node->getAttribute( "name" ) );
	LOGFONT lf;
	text_font.GetLogFont( &lf );
	memset( lf.lfFaceName, 0, LF_FACESIZE );
	memcpy( lf.lfFaceName, font_name.c_str(), font_name.size() );
	lf.lfHeight = font_node->getAttributeInt( "height" );
	CFont font;
	font.CreateFontIndirect( &lf );
	setTextFont( font, RPObjectChart::strToColor( font_node->getAttribute( "color" ) ), font_node->getAttribute( "show" ).tobool() );
}

rp::RPXMLNode* RPShape::save( rp::RPXMLNode* parent_node )
{
	rp::RPXMLNode* obj_node = RPObjectMatrix::save( parent_node );
	LOGBRUSH lb;
	if ( bg_brush.GetLogBrush( &lb ) ) {
		rp::RPXMLNode* brush_node = obj_node->makeChild( "brush" );
		brush_node->insertAttribute( "color", RPObjectChart::colorToStr(lb.lbColor) );
		brush_node->insertAttributeInt( "style", lb.lbStyle );
	}
	LOGFONT lf;
	if ( text_font.GetLogFont( &lf ) ) {
		rp::RPXMLNode* font_node = obj_node->makeChild( "font" );
		font_node->insertAttribute( "name", lf.lfFaceName );
		font_node->insertAttributeInt( "height", lf.lfHeight );
		font_node->insertAttribute( "color", RPObjectChart::colorToStr(text_color) );
		font_node->insertAttribute( "show", rp::string::frombool(text_show) );
	}
//	rp::RPXMLNode* connectors_node = obj_node->makeChild( "docks" );
//	std::vector< RPConnectorDock* >::const_iterator it = docks.begin();
//	while ( it != docks.end() ) {
//		(*it)->save( connectors_node );
//		it++;
//	}
	return obj_node;
}

void RPShape::saveToXML(REF(pugi::xml_node) parentNode) const
{
	pugi::xml_node node = parentNode.append_child(_T("RPShape"));
	// Создаем физическую кисть (цвет, стиль и узор):
	LOGBRUSH lb;
	if (const_cast<CBrush&>(bg_brush).GetLogBrush(&lb))
	{
		pugi::xml_node brushNode = node.append_child(_T("LOGBRUSH"));
		brushNode.append_attribute(_T("color"))    .set_value(RPObjectChart::colorToStr(lb.lbColor).c_str());
		brushNode.append_attribute(_T("style"))    .set_value(lb.lbStyle                                   );
	}
	// Создаем фон текста:
	LOGFONT lf;
	if (const_cast<CFont&>(text_font).GetLogFont(&lf))
	{
		pugi::xml_node fontNode = node.append_child(_T("LOGFONT"));
		fontNode.append_attribute(_T("name"))     .set_value(lf.lfFaceName                                );
		fontNode.append_attribute(_T("height"))   .set_value(lf.lfHeight                                  );
		fontNode.append_attribute(_T("color"))    .set_value(RPObjectChart::colorToStr(text_color).c_str());
		fontNode.append_attribute(_T("show"))     .set_value(rp::string::   frombool  (text_show) .c_str());
	}
}

void RPShape::loadFromXML(CREF(pugi::xml_node) node)
{
	LOGBRUSH lb;
	LOGFONT  lf;
	CFont    font;
	CBrush   brush;
	
	COLORREF clr;
	rbool show       = false;
	// Переменные для проверки существования структур lb:
	rbool colorExist = false;
	rbool styleExist = false;
	
	// Поиск узла <RPShape/> в списке потомков корня поддерева:
	for (pugi::xml_node next_node = node.first_child(); next_node; next_node = next_node.next_sibling())
	{
		if (strcmp(next_node.name(), "RPShape") == 0)
		{
			for (pugi::xml_node sub_node = next_node.first_child(); sub_node; sub_node = sub_node.next_sibling())
			{
				// Считываем атрибуты кисти
				if (strcmp(sub_node.name(), _T("LOGBRUSH")) == 0)
				{
					for (pugi::xml_attribute attr = sub_node.first_attribute(); attr; attr = attr.next_attribute())
					{
						// Для отслеживания процесса "debug" заводим новую переменную
						tstring attrName = attr.name();
						if (attrName == _T("color"))
						{
							lb.lbColor = RPObjectChart::strToColor(attr.value());
							colorExist = true;
						}
						// Значение атрибута "style" должно лежать в интервале [0; 3):
						else if (attrName == _T("style") && (attr.as_int() < 3 && attr.as_int() > -1))
						{
							lb.lbStyle = attr.as_int();
							styleExist = true;
						}
					}
				}
				// Считываем атрибуты стиля текста
				if (strcmp(sub_node.name(), _T("LOGFONT")) == 0)
				{
					text_font.GetLogFont(&lf);
					for (pugi::xml_attribute attr = sub_node.first_attribute(); attr; attr = attr.next_attribute())
					{
						tstring attrName = attr.name();
						if (attrName == _T("name"))
						{
							tstring font_name = attr.value();
							//text_font.GetLogFont(&lf);				// Удаляет подчеркивания и зачеркивания текста
							memset(lf.lfFaceName, 0, LF_FACESIZE);
							memcpy(lf.lfFaceName, font_name.c_str(), font_name.size());
						}
						else if (attrName == _T("height"))
						{
							lf.lfHeight = attr.as_uint();
						}
						else if (attrName == _T("color"))
						{
							clr = RPObjectChart::strToColor(attr.value());
						}
						else if (attrName == _T("show"))
						{
							show = attr.as_bool();
						}
					}
				}
			}
		}
	}
	// Условие существования структур типа lb:
	if (colorExist && styleExist)
	{
		lb.lbHatch = 0;
		brush.CreateBrushIndirect(&lb);
		setBgBrush(brush);
	}
	// Текстовый стиль:
	font.CreateFontIndirect(&lf);
	setTextFont(font, clr, show);
}

RPObjectChart* RPShape::find( const rp::point& global_chart_pos )
{
	if ( pointInShape(global_chart_pos) ) {
		return this;
	}
	return RPObjectMatrix::find( global_chart_pos );
}

rp::rect RPShape::getMaxRect()
{
	rp::rect rect = RPObjectMatrix::getMaxRect();
	double max_x = rect.getMaxX();
	double max_y = rect.getMaxY();
	double min_x = rect.getMinX();
	double min_y = rect.getMinY();
	transformToGlobal();
	pa_global.extendByPerimetr( static_cast<double>(main_pen_width) / 2.0 );
	if ( pa_global.getMaxX() > max_x ) max_x = pa_global.getMaxX();
	if ( pa_global.getMaxY() > max_y ) max_y = pa_global.getMaxY();
	if ( pa_global.getMinX() < min_x ) min_x = pa_global.getMinX();
	if ( pa_global.getMinY() < min_y ) min_y = pa_global.getMinY();
	return rp::rect( min_x, min_y, max_x, max_y );
}

RPProject::Cursor RPShape::getCursor( const rp::point& global_chart_pos )
{
	if ( rpMethod::project->getFlowState() == RPProject::flow_rotate && isSelected() && isRotateCenter( global_chart_pos ) ) return RPProject::cursor_flow_rotate_center;

	RPProject::Cursor cursor = RPObjectMatrix::getCursor( global_chart_pos );
	if ( cursor != RPProject::cursor_flow_select ) return cursor;

	if ( pointInShape(global_chart_pos) ) {
		RPShape::PossibleCommand pcmd = getPossibleCommand( global_chart_pos, true );
		if ( isSelected() ) {
			switch ( pcmd ) {
				case RPShape::pcmd_move         : return RPProject::cursor_flow_move;
				case RPShape::pcmd_rotate_tl    : return RPProject::cursor_flow_rotate_tl;
				case RPShape::pcmd_rotate_tr    : return RPProject::cursor_flow_rotate_tr;
				case RPShape::pcmd_rotate_bl    : return RPProject::cursor_flow_rotate_bl;
				case RPShape::pcmd_rotate_br    : return RPProject::cursor_flow_rotate_br;
				case RPShape::pcmd_scale_l      :
				case RPShape::pcmd_scale_r      : return RPProject::cursor_flow_scale_lr;
				case RPShape::pcmd_scale_t      :
				case RPShape::pcmd_scale_b      : return RPProject::cursor_flow_scale_tb;
				case RPShape::pcmd_scale_tl     :
				case RPShape::pcmd_scale_br     : return RPProject::cursor_flow_scale_tlbr;
				case RPShape::pcmd_scale_tr     :
				case RPShape::pcmd_scale_bl     : return RPProject::cursor_flow_scale_trbl;
				case RPShape::pcmd_dock_in      : return RPProject::cursor_flow_dock_in;
				case RPShape::pcmd_dock_out     : return RPProject::cursor_flow_dock_out;
				case RPShape::pcmd_dock_inout   : return RPProject::cursor_flow_dock_inout;
				case RPShape::pcmd_dock_fly     : return RPProject::cursor_flow_dock_fly;
				case RPShape::pcmd_dock_not     : return RPProject::cursor_flow_dock_not;
			}
		} else {
			switch ( pcmd ) {
				case RPShape::pcmd_move         :
				case RPShape::pcmd_rotate_tl    :
				case RPShape::pcmd_rotate_tr    :
				case RPShape::pcmd_rotate_bl    :
				case RPShape::pcmd_rotate_br    :
				case RPShape::pcmd_scale_l      :
				case RPShape::pcmd_scale_r      :
				case RPShape::pcmd_scale_t      :
				case RPShape::pcmd_scale_b      :
				case RPShape::pcmd_scale_tl     :
				case RPShape::pcmd_scale_br     :
				case RPShape::pcmd_scale_tr     :
				case RPShape::pcmd_scale_bl     : return RPProject::cursor_flow_move;
				case RPShape::pcmd_dock_in      : return RPProject::cursor_flow_dock_in;
				case RPShape::pcmd_dock_out     : return RPProject::cursor_flow_dock_out;
				case RPShape::pcmd_dock_inout   : return RPProject::cursor_flow_dock_inout;
				case RPShape::pcmd_dock_fly     : return RPProject::cursor_flow_dock_fly;
				case RPShape::pcmd_dock_not     : return RPProject::cursor_flow_dock_not;
			}
		}
	}
	return RPProject::cursor_flow_select;
}

rbool RPShape::pointInNCArea( const rp::point& global_chart_pos )
{
	// Проверяем на попадание в док
	if ( rpMethod::project->getFlowState() == RPProject::flow_connector ) {
		if ( find_dock(global_chart_pos) ) return true;
	}
	// Проверяем на попадание в обрамляющий прямоугольник на растяжение/поворот
	if ( !isSelected() ) return false;
	switch ( getPossibleCommand( global_chart_pos ) ) {
		case RPShape::pcmd_rotate_center:
		case RPShape::pcmd_rotate_tl    :
		case RPShape::pcmd_rotate_tr    :
		case RPShape::pcmd_rotate_bl    :
		case RPShape::pcmd_rotate_br    :
		case RPShape::pcmd_scale_l      :
		case RPShape::pcmd_scale_r      :
		case RPShape::pcmd_scale_t      :
		case RPShape::pcmd_scale_b      :
		case RPShape::pcmd_scale_tl     :
		case RPShape::pcmd_scale_br     :
		case RPShape::pcmd_scale_tr     :
		case RPShape::pcmd_scale_bl     : return true;
	}
	return false;
}

void RPShape::setPosition( double posx, double posy )
{
	RPObjectMatrix::setPosition( posx, posy );
	if (flowChart()) flowChart()->snapToGrid( this );
}

void RPShape::transformToGlobal()
{
	if ( pa_global.size() != pa_src.size() ) {
		pa_global.resize( pa_src.size() );
	}
	rp::matrix m(globalMatrix());
	trans tr(m);
	std::transform( pa_src.begin(), pa_src.end(), pa_global.begin(), tr );
}

rp::rect RPShape::getBoundingRect( rbool global ) const
{
	rp::rect bound_rect;
	if ( !pa_src.empty() ) {
		rp::rect rect = pa_src.getBoundingRect();
		if ( global ) {
			rect.transform( globalMatrix() );
		} else {
			rect.transform( matrix_scale );
		}
		bound_rect = rect;
	}
	return bound_rect;
}

void RPShape::drawPolyline( CDC& dc )
{
	if ( pa_global.size() < 2 ) return;
	dc.SelectObject( const_cast<CPen*>(&getPen()) );
//	dc.Polyline( &pa_global.getWinPolyline()[0], pa_global.size() );
//	return;
	CBrush* old_brush = dc.SelectObject( &bg_brush );
//	dc.BeginPath();
	pa_global.draw( dc );
//	if ( pa_global.isPolygon() ) {
//		dc.Polygon( &pa_global.getWinPolyline()[0], pa_global.size() );
//	} else {
//		dc.Polyline( &pa_global.getWinPolyline()[0], pa_global.size() );
//	}
//	dc.EndPath();
//	dc.StrokePath();
	dc.SelectObject( old_brush );
}

void RPShape::drawDocks( CDC& dc )
{
	if ( rpMethod::project->getFlowState() != RPProject::flow_connector ) return;
	CPen pen( PS_SOLID, 1, RGB(0x00, 0x00, 0x00) );
	CPen* old_pen = dc.SelectObject( &pen );
	int radius = RPObjectFlowChart::getSensitivity();
	rp::matrix gm = globalMatrix();
	std::vector< RPConnectorDock* >::const_iterator it = docks.begin();
	while ( it != docks.end() ) {
		rp::point point = gm * (*it)->getPosition( false );

//		if ( !(*it)->connectors.empty() ) {
//			rp::point norm_point = (*it)->getDeltaPosition();
//			dc.MoveTo( point.x, point.y );
//			dc.LineTo( norm_point.x, norm_point.y );
//		}

		CBrush brush( (*it)->color() );
		CBrush* old_brush = dc.SelectObject( &brush );
		dc.Ellipse( static_cast<int>(point.x - radius + 1), static_cast<int>(point.y - radius + 1), static_cast<int>(point.x + radius), static_cast<int>(point.y + radius) );
		dc.SelectObject( old_brush );

		it++;
	}
	dc.SelectObject( old_pen );
}

/*
void RPShape::drawConnectorsInput( CDC& dc )
{
	if ( flowChart->getShowConnectorPoint() ) {
		painter.setPen( flowChart->getShapeColor() );
		painter.setBrush( flowChart->getConnectorDockColor() );
		for ( unsigned int i = 0; i < conI.size(); i++ ) {
			painter.drawEllipse( conI.point(i).x() - 3, conI.point(i).y() - 3, 7, 7 );
		}
	}
}
*/

void RPShape::draw( CDC& dc )
{
	RPObjectMatrix::draw( dc );

	// Перевод фигуры в глобальные координаты
	transformToGlobal();

	// Отрисовка полигона
	drawPolyline( dc );

	// Отрисовка доков для конекторов
	drawDocks( dc );

	// Вывод имени
	if ( text_show ) {
		LOGFONT lf;
		text_font.GetLogFont( &lf );
		lf.lfEscapement = static_cast<int>(getRotationGlobal() * 10);
		CFont font;
		font.CreateFontIndirect( &lf );
		CFont* old_font = dc.SelectObject( &font );
		CRect calc( 0, 0, 1, 1 );
		dc.DrawText( name.c_str(), &calc, DT_CALCRECT | DT_SINGLELINE );
		rp::point center( -calc.Width()/2, -calc.Height()/2 );
		center = globalMatrix( m_all & ~m_sc, m_all & ~m_sc ) * center;
		dc.SetTextColor( text_color );
		dc.TextOut( static_cast<int>(center.x), static_cast<int>(center.y), name.c_str() );
		dc.SelectObject( old_font );
	}
/*
	rp::rect rect = getBoundingRect().getBoundingRect();
	dc.MoveTo( rect.p0().x, rect.p0().y );
	dc.LineTo( rect.p1().x, rect.p1().y );
	dc.LineTo( rect.p2().x, rect.p2().y );
	dc.LineTo( rect.p3().x, rect.p3().y );
	dc.LineTo( rect.p0().x, rect.p0().y );

	rp::matrix gm = globalMatrix();
	CPen pen1( PS_SOLID, 1, RGB(-1,0,0) );
	dc.SelectObject( pen1 );
	dc.MoveTo( gm * CPoint(-70,0) );
	dc.LineTo( gm * CPoint(70,0) );
	CPen pen2( PS_SOLID, 1, RGB(0,-1,0) );
	dc.SelectObject( pen2 );
	dc.MoveTo( gm * CPoint(0,-70) );
	dc.LineTo( gm * CPoint(0,70) );
*/
//	dc.DrawText( rp::string::format( "alpha = %f", getRotationGlobal() ).c_str(), CRect( gm * CPoint(0,70), CSize(100,100)), DT_SINGLELINE );
//	dc.DrawText( rp::string::format( "alpha = %f", rotation_alpha ).c_str(), CRect( gm * CPoint(0,70), CSize(100,100)), DT_SINGLELINE );
/*
	transformToGlobal();
	pa_global.extendByPerimetr( main_pen_width / 2.0 );
	CPen pen( PS_SOLID, 1, RGB(-1,0,0) );
	dc.SelectObject( pen );
	dc.BeginPath();
	if ( pa_global.isPolygon() ) {
		dc.Polygon( &pa_global.getWinPolyline()[0], pa_global.size() );
	} else {
//		dc.Polyline( &pa_global.getWinPolyline()[0], pa_global.size() );
	}
	dc.EndPath();
	dc.StrokePath();
*/
/*
	if ( flowChart->getShowShapeName() ) {
		painter.setPen( flowChart->getShapeColor() );
		int name_x = getX() + flowChart->getPaperBorder();
		int name_y = getY() + flowChart->getPaperBorder();
		painter.drawText( name_x, name_y, size.width(), size.height()-1, Qt::AlignHCenter | Qt::AlignVCenter | Qt::SingleLine, name );
	}
	drawConnectorsInput( painter );
	drawConnectorsOutput( painter );
*/
}

void RPShape::draw_selected( CDC& dc )
{
	RPObjectMatrix::draw_selected( dc );

	// Прямоугольник вокруг фигуры
	rp::rect rect = getBoundingRect();
	int x0 = static_cast<int>(rect.p0().x);
	int x1 = static_cast<int>(rect.p1().x);
	int x2 = static_cast<int>(rect.p2().x);
	int x3 = static_cast<int>(rect.p3().x);
	int y0 = static_cast<int>(rect.p0().y);
	int y1 = static_cast<int>(rect.p1().y);
	int y2 = static_cast<int>(rect.p2().y);
	int y3 = static_cast<int>(rect.p3().y);
	RPObjectFlowChart* flowchart = flowChart();
	dc.SelectObject( flowchart->getPenSelectedLine() );
	dc.MoveTo( x0, y0 );
	dc.LineTo( x1, y1 );
	dc.LineTo( x2, y2 );
	dc.LineTo( x3, y3 );
	dc.LineTo( x0, y0 );
	dc.SelectObject( flowchart->getPenSelectedBox() );
	dc.SelectObject( flowchart->getBrushSelectedBox() );
	int box_size = flowchart->getSelectBoxSize2();
	if ( rpMethod::project->getFlowState() == RPProject::flow_rotate ) {
		int radius = RPObjectFlowChart::getSensitivity();
		dc.Ellipse( x0 - radius, y0 - radius, x0 + radius, y0 + radius );
		dc.Ellipse( x1 - radius, y1 - radius, x1 + radius, y1 + radius );
		dc.Ellipse( x2 - radius, y2 - radius, x2 + radius, y2 + radius );
		dc.Ellipse( x3 - radius, y3 - radius, x3 + radius, y3 + radius );
	} else {
		dc.Rectangle( x0 - box_size, y0 - box_size, x0 + box_size, y0 + box_size );
		dc.Rectangle( x1 - box_size, y1 - box_size, x1 + box_size, y1 + box_size );
		dc.Rectangle( x2 - box_size, y2 - box_size, x2 + box_size, y2 + box_size );
		dc.Rectangle( x3 - box_size, y3 - box_size, x3 + box_size, y3 + box_size );
	}
	dc.Rectangle( (x0 + x1)/2 - box_size, (y0 + y1)/2 - box_size, (x0 + x1)/2 + box_size, (y0 + y1)/2 + box_size );
	dc.Rectangle( (x1 + x2)/2 - box_size, (y1 + y2)/2 - box_size, (x1 + x2)/2 + box_size, (y1 + y2)/2 + box_size );
	dc.Rectangle( (x2 + x3)/2 - box_size, (y2 + y3)/2 - box_size, (x2 + x3)/2 + box_size, (y2 + y3)/2 + box_size );
	dc.Rectangle( (x3 + x0)/2 - box_size, (y3 + y0)/2 - box_size, (x3 + x0)/2 + box_size, (y3 + y0)/2 + box_size );
	if ( rpMethod::project->getFlowState() == RPProject::flow_rotate ) {
		// Центр вращения
		rp::point center = getRotateCenter();
		CPen pen_red( PS_SOLID, 1, RGB(0,0,0) );
		CBrush brush_white( RGB(-1,-1,0) );
		int radius = RPObjectFlowChart::getSensitivity();
		dc.SelectObject( pen_red );
		dc.SelectObject( brush_white );
		dc.Ellipse( static_cast<int>(center.x - radius), static_cast<int>(center.y - radius), static_cast<int>(center.x + radius), static_cast<int>(center.y + radius) );
	}
}

void RPShape::command_before( const rp::point& global_chart_pos, rbool first_click )
{
	RPObjectMatrix::command_before( global_chart_pos, first_click );
	pcmd = ((rpMethod::project->getFlowState() == RPProject::flow_select || rpMethod::project->getFlowState() == RPProject::flow_rotate) && first_click) ? RPShape::pcmd_move : getPossibleCommand( global_chart_pos );
	if ( pcmd == pcmd_dock_in || pcmd == pcmd_dock_out || pcmd == pcmd_dock_inout ) {
		RPObjectFlowChart* flowchart = flowChart();
		RPConnectorDock* dock = find_dock( global_chart_pos );
		if ( flowchart ) {
			flowchart->insert_connector( dock );
		}
	}
}

void RPShape::command_make( const rp::point& global_chart_pos )
{
	RPObjectMatrix::command_make( global_chart_pos );

	RPObjectFlowChart* flowchart = flowChart();
	RPShape::angle90 a90 = getAngle90();
	rp::point mouse_delta = flowchart->mouse_delta();
	switch ( pcmd ) {
		case RPShape::pcmd_move: {
			mouse_delta = parentRotate().obr() * mouse_delta;
			matrix_transform.dx() += mouse_delta.x;
			matrix_transform.dy() += mouse_delta.y;
			modify();
			update();
			break;
		}
		case RPShape::pcmd_rotate_center: {
			setRotateCenter( global_chart_pos );
			update();
			break;
		}
		case RPShape::pcmd_dock_out     : {
			break;
		}
		case RPShape::pcmd_rotate_tl    :
		case RPShape::pcmd_rotate_tr    :
		case RPShape::pcmd_rotate_bl    :
		case RPShape::pcmd_rotate_br    : {
			setRotation( getRotation() + rp::math::getAlpha( flowchart->mouse_prev(), getRotateCenter(), flowchart->mouse_current() ) );
			update();
			break;
		}
		case RPShape::pcmd_scale_t      :
		case RPShape::pcmd_scale_b      :
		case RPShape::pcmd_scale_l      :
		case RPShape::pcmd_scale_r      :
		case RPShape::pcmd_scale_tl     :
		case RPShape::pcmd_scale_tr     :
		case RPShape::pcmd_scale_bl     :
		case RPShape::pcmd_scale_br     : {
			rp::rect  rect = getBoundingRect();
			rp::point point_delta( globalRotate().obr() * mouse_delta );
			rp::point len( rp::math::getLength( rect.p_tl(), rect.p_tr() ), rp::math::getLength( rect.p_tl(), rect.p_bl() ) );
			RPShape::getScaleDelta( point_delta, a90, pcmd );
			if ( len.x + point_delta.x < 0 ) {
				point_delta.x = -len.x;
			}
			if ( len.y + point_delta.y < 0 ) {
				point_delta.y = -len.y;
			}
			setScaleX( getScaleX() * (len.x + point_delta.x ) / len.x );
			setScaleY( getScaleY() * (len.y + point_delta.y ) / len.y );
			rp::rect r(getBoundingRect());
			RPShape::getRectDelta( rect, r, point_delta, a90, pcmd );
			point_delta = globalRotate().obr() * point_delta;
			setPostX( getPostX() + point_delta.x );
			setPostY( getPostY() + point_delta.y );
			setRotateCenterLocalDelta( point_delta.x, point_delta.y );
			update();
			break;
		}
	}
}

RPConnectorDock* RPShape::find_dock( const rp::point& global_chart_pos )
{
	rp::matrix gm = globalMatrix();
	std::vector< RPConnectorDock* >::iterator it = docks.begin();
	while ( it != docks.end() ) {
		if ( rp::math::getLength( gm * (*it)->getPosition( false ), global_chart_pos ) <= RPObjectFlowChart::getSensitivity() ) {
			return *it;
		}
		it++;
	}
	return NULL;
}

int RPShape::getDockIndex( const RPConnectorDock* dock ) const
{
	int index = -1;
	std::vector< RPConnectorDock* >::const_iterator it = docks.begin();
	while ( it != docks.end() ) {
		index++;
		if ( *it == dock ) break;
		it++;
	}
	return index;
}

RPShape::PossibleCommand RPShape::getPossibleCommand( const rp::point& global_chart_pos, rbool for_cursor )
{
	// Отдельно проверим на перемещение центра вращения. Он отрисовывается поверх выделения, значит и проверяться должен первым.
	if ( rpMethod::project->getFlowState() == RPProject::flow_rotate && isSelected() && isRotateCenter( global_chart_pos ) ) return RPShape::pcmd_rotate_center;

	int sensitivity = RPObjectFlowChart::getSensitivity();
	angle90 a90 = getAngle90();
	angle45 a45 = getAngle45();

	// Проверим на попадание в dock для коннекторов
	if ( rpMethod::project->getFlowState() == RPProject::flow_connector ) {
		RPConnectorDock* dock = find_dock( global_chart_pos );
		if ( dock ) {
			if ( !dock->can_connect( flowChart()->getDockBegin() ) ) return pcmd_dock_not;
			if ( dock->isType( RPConnectorDock::fly ) ) return pcmd_dock_fly;
			if ( dock->isType( RPConnectorDock::inout )  ) return pcmd_dock_inout;
			if ( dock->isType( RPConnectorDock::in ) ) {
				return (flowChart()->getConnectorTypeWanted() == RPObjectFlowChart::ctw_end) ? pcmd_dock_in : pcmd_dock_not;
			}
			if ( dock->isType( RPConnectorDock::out ) ) {
				return (flowChart()->getConnectorTypeWanted() == RPObjectFlowChart::ctw_begin) ? pcmd_dock_out : pcmd_dock_not;
			}
			return pcmd_dock_not;
		}
	}

	// Отдельно проверим на растяжение за правый нижний угол, т.к. фигуру, сжатую в ноль, лучше растягивать из него
	rp::rect rect = getBoundingRect();
	if ( rpMethod::project->getFlowState() == RPProject::flow_select ) {
		if ( rp::math::getLength( rect.p_tl(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_180: return RPShape::pcmd_scale_br;
				}
			} else {
				switch ( a90 ) {
					case angle90_180: return RPShape::pcmd_scale_br;
				}
			}
		}
		if ( rp::math::getLength( rect.p_tr(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_270: return RPShape::pcmd_scale_br;
				}
			} else {
				switch ( a90 ) {
					case angle90_270: return RPShape::pcmd_scale_br;
				}
			}
		}
		if ( rp::math::getLength( rect.p_bl(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_90 : return RPShape::pcmd_scale_br;
				}
			} else {
				switch ( a90 ) {
					case angle90_90 : return RPShape::pcmd_scale_br;
				}
			}
		}
		if ( rp::math::getLength( rect.p_br(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_br;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_br;
				}
			}
		}
		// Отдельно проверим на растяжение за левый нижний угол, т.к. фигуру, сжатую в горизонтальную линию, лучше растягивать именно из него, а не в лево-вверх
		if ( rp::math::getLength( rect.p_tl(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_90 : return RPShape::pcmd_scale_bl;
				}
			} else {
				switch ( a90 ) {
					case angle90_90 : return RPShape::pcmd_scale_bl;
				}
			}
		}
		if ( rp::math::getLength( rect.p_tr(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_180: return RPShape::pcmd_scale_bl;
				}
			} else {
				switch ( a90 ) {
					case angle90_180: return RPShape::pcmd_scale_bl;
				}
			}
		}
		if ( rp::math::getLength( rect.p_bl(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_bl;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_bl;
				}
			}
		}
		if ( rp::math::getLength( rect.p_br(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_270: return RPShape::pcmd_scale_bl;
				}
			} else {
				switch ( a90 ) {
					case angle90_270: return RPShape::pcmd_scale_bl;
				}
			}
		}
	}
	rbool select_or_rotate = rpMethod::project->getFlowState() == RPProject::flow_select || rpMethod::project->getFlowState() == RPProject::flow_rotate;
	// Общая часть и для перемещения и для вращения
	if ( select_or_rotate ) {
		// Отдельно проверим на растяжение за нижний центр, т.к. фигуру, сжатую в горизонтальную линию, лучше растягивать вниз за него, а не вверх
		if ( rp::math::getLength( rect.p_r(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_270: return RPShape::pcmd_scale_b;
				}
			} else {
				switch ( a90 ) {
					case angle90_270: return RPShape::pcmd_scale_b;
				}
			}
		}
		if ( rp::math::getLength( rect.p_l(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_90 : return RPShape::pcmd_scale_b;
				}
			} else {
				switch ( a90 ) {
					case angle90_90 : return RPShape::pcmd_scale_b;
				}
			}
		}
		if ( rp::math::getLength( rect.p_t(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_180: return RPShape::pcmd_scale_b;
				}
			} else {
				switch ( a90 ) {
					case angle90_180: return RPShape::pcmd_scale_b;
				}
			}
		}
		if ( rp::math::getLength( rect.p_b(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_b;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_b;
				}
			}
		}
		// Отдельно проверим на растяжение за правый центр, т.к. фигуру, сжатую в вертикальную линию, лучше растягивать вправо за него, а не влево
		if ( rp::math::getLength( rect.p_r(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_r;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_r;
				}
			}
		}
		if ( rp::math::getLength( rect.p_l(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_180: return RPShape::pcmd_scale_r;
				}
			} else {
				switch ( a90 ) {
					case angle90_180: return RPShape::pcmd_scale_r;
				}
			}
		}
		if ( rp::math::getLength( rect.p_t(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_270: return RPShape::pcmd_scale_r;
				}
			} else {
				switch ( a90 ) {
					case angle90_270: return RPShape::pcmd_scale_r;
				}
			}
		}
		if ( rp::math::getLength( rect.p_b(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_90 : return RPShape::pcmd_scale_r;
				}
			} else {
				switch ( a90 ) {
					case angle90_90 : return RPShape::pcmd_scale_r;
				}
			}
		}
		// Для остальных (нижний центр тоже проверяется)
		if ( rp::math::getLength( rect.p_r(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_r;
					case angle45_90 : return RPShape::pcmd_scale_t;
					case angle45_180: return RPShape::pcmd_scale_l;
					case angle45_270: return RPShape::pcmd_scale_b;
					case angle45_45 : return RPShape::pcmd_scale_tr;
					case angle45_135: return RPShape::pcmd_scale_tl;
					case angle45_225: return RPShape::pcmd_scale_bl;
					case angle45_315: return RPShape::pcmd_scale_br;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_r;
					case angle90_90 : return RPShape::pcmd_scale_t;
					case angle90_180: return RPShape::pcmd_scale_l;
					case angle90_270: return RPShape::pcmd_scale_b;
				}
			}
		}
		if ( rp::math::getLength( rect.p_l(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_l;
					case angle45_90 : return RPShape::pcmd_scale_b;
					case angle45_180: return RPShape::pcmd_scale_r;
					case angle45_270: return RPShape::pcmd_scale_t;
					case angle45_45 : return RPShape::pcmd_scale_bl;
					case angle45_135: return RPShape::pcmd_scale_br;
					case angle45_225: return RPShape::pcmd_scale_tr;
					case angle45_315: return RPShape::pcmd_scale_tl;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_l;
					case angle90_90 : return RPShape::pcmd_scale_b;
					case angle90_180: return RPShape::pcmd_scale_r;
					case angle90_270: return RPShape::pcmd_scale_t;
				}
			}
		}
		if ( rp::math::getLength( rect.p_t(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_t;
					case angle45_90 : return RPShape::pcmd_scale_l;
					case angle45_180: return RPShape::pcmd_scale_b;
					case angle45_270: return RPShape::pcmd_scale_r;
					case angle45_45 : return RPShape::pcmd_scale_tl;
					case angle45_135: return RPShape::pcmd_scale_bl;
					case angle45_225: return RPShape::pcmd_scale_br;
					case angle45_315: return RPShape::pcmd_scale_tr;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_t;
					case angle90_90 : return RPShape::pcmd_scale_l;
					case angle90_180: return RPShape::pcmd_scale_b;
					case angle90_270: return RPShape::pcmd_scale_r;
				}
			}
		}
		if ( rp::math::getLength( rect.p_b(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_b;
					case angle45_90 : return RPShape::pcmd_scale_r;
					case angle45_180: return RPShape::pcmd_scale_t;
					case angle45_270: return RPShape::pcmd_scale_l;
					case angle45_45 : return RPShape::pcmd_scale_br;
					case angle45_135: return RPShape::pcmd_scale_tr;
					case angle45_225: return RPShape::pcmd_scale_tl;
					case angle45_315: return RPShape::pcmd_scale_bl;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_b;
					case angle90_90 : return RPShape::pcmd_scale_r;
					case angle90_180: return RPShape::pcmd_scale_t;
					case angle90_270: return RPShape::pcmd_scale_l;
				}
			}
		}
	}
	if ( rpMethod::project->getFlowState() == RPProject::flow_select ) {
		// Только при перемещении
		if ( rp::math::getLength( rect.p_tl(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_tl;
					case angle45_90 : return RPShape::pcmd_scale_bl;
					case angle45_180: return RPShape::pcmd_scale_br;
					case angle45_270: return RPShape::pcmd_scale_tr;
					case angle45_45 : return RPShape::pcmd_scale_l;
					case angle45_135: return RPShape::pcmd_scale_b;
					case angle45_225: return RPShape::pcmd_scale_r;
					case angle45_315: return RPShape::pcmd_scale_t;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_tl;
					case angle90_90 : return RPShape::pcmd_scale_bl;
					case angle90_180: return RPShape::pcmd_scale_br;
					case angle90_270: return RPShape::pcmd_scale_tr;
				}
			}
		}
		if ( rp::math::getLength( rect.p_tr(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_tr;
					case angle45_90 : return RPShape::pcmd_scale_tl;
					case angle45_180: return RPShape::pcmd_scale_bl;
					case angle45_270: return RPShape::pcmd_scale_br;
					case angle45_45 : return RPShape::pcmd_scale_t;
					case angle45_135: return RPShape::pcmd_scale_l;
					case angle45_225: return RPShape::pcmd_scale_b;
					case angle45_315: return RPShape::pcmd_scale_r;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_tr;
					case angle90_90 : return RPShape::pcmd_scale_tl;
					case angle90_180: return RPShape::pcmd_scale_bl;
					case angle90_270: return RPShape::pcmd_scale_br;
				}
			}
		}
		if ( rp::math::getLength( rect.p_bl(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_bl;
					case angle45_90 : return RPShape::pcmd_scale_br;
					case angle45_180: return RPShape::pcmd_scale_tr;
					case angle45_270: return RPShape::pcmd_scale_tl;
					case angle45_45 : return RPShape::pcmd_scale_b;
					case angle45_135: return RPShape::pcmd_scale_r;
					case angle45_225: return RPShape::pcmd_scale_t;
					case angle45_315: return RPShape::pcmd_scale_l;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_bl;
					case angle90_90 : return RPShape::pcmd_scale_br;
					case angle90_180: return RPShape::pcmd_scale_tr;
					case angle90_270: return RPShape::pcmd_scale_tl;
				}
			}
		}
		if ( rp::math::getLength( rect.p_br(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_br;
					case angle45_90 : return RPShape::pcmd_scale_tr;
					case angle45_180: return RPShape::pcmd_scale_tl;
					case angle45_270: return RPShape::pcmd_scale_bl;
					case angle45_45 : return RPShape::pcmd_scale_r;
					case angle45_135: return RPShape::pcmd_scale_t;
					case angle45_225: return RPShape::pcmd_scale_l;
					case angle45_315: return RPShape::pcmd_scale_b;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_br;
					case angle90_90 : return RPShape::pcmd_scale_tr;
					case angle90_180: return RPShape::pcmd_scale_tl;
					case angle90_270: return RPShape::pcmd_scale_bl;
				}
			}
		}
	} else if ( rpMethod::project->getFlowState() == RPProject::flow_rotate ) {
		// Только при вращении
		if ( rp::math::getLength( rect.p_tl(), global_chart_pos ) <= sensitivity ) {
			switch ( a90 ) {
				case angle90_0  : return RPShape::pcmd_rotate_tl;
				case angle90_90 : return RPShape::pcmd_rotate_bl;
				case angle90_180: return RPShape::pcmd_rotate_br;
				case angle90_270: return RPShape::pcmd_rotate_tr;
			}
		}
		if ( rp::math::getLength( rect.p_tr(), global_chart_pos ) <= sensitivity ) {
			switch ( a90 ) {
				case angle90_0  : return RPShape::pcmd_rotate_tr;
				case angle90_90 : return RPShape::pcmd_rotate_tl;
				case angle90_180: return RPShape::pcmd_rotate_bl;
				case angle90_270: return RPShape::pcmd_rotate_br;
			}
		}
		if ( rp::math::getLength( rect.p_bl(), global_chart_pos ) <= sensitivity ) {
			switch ( a90 ) {
				case angle90_0  : return RPShape::pcmd_rotate_bl;
				case angle90_90 : return RPShape::pcmd_rotate_br;
				case angle90_180: return RPShape::pcmd_rotate_tr;
				case angle90_270: return RPShape::pcmd_rotate_tl;
			}
		}
		if ( rp::math::getLength( rect.p_br(), global_chart_pos ) <= sensitivity ) {
			switch ( a90 ) {
				case angle90_0  : return RPShape::pcmd_rotate_br;
				case angle90_90 : return RPShape::pcmd_rotate_tr;
				case angle90_180: return RPShape::pcmd_rotate_tl;
				case angle90_270: return RPShape::pcmd_rotate_bl;
			}
		}
	}
	// Общая часть для перемещения, вращения и коннектора
	if ( select_or_rotate || rpMethod::project->getFlowState() == RPProject::flow_connector ) {
		if ( pointInPolygon( global_chart_pos ) ) {
			return RPShape::pcmd_move;
		}
	}
	return RPShape::pcmd_none;
}

void RPShape::getScaleDelta( rp::point& delta, RPShape::angle90 a90, RPShape::PossibleCommand pcmd )
{
	switch ( pcmd ) {
		case RPShape::pcmd_scale_tl: {
			switch ( a90 ) {
				case RPShape::angle90_0  : delta.x = -delta.x; delta.y = -delta.y; break;
				case RPShape::angle90_90 : delta.y = -delta.y; break;
				case RPShape::angle90_180: break;
				case RPShape::angle90_270: delta.x = -delta.x; break;
			}
			break;
		}
		case RPShape::pcmd_scale_tr: {
			switch ( a90 ) {
				case RPShape::angle90_0  : delta.y = -delta.y; break;
				case RPShape::angle90_90 : break;
				case RPShape::angle90_180: delta.x = -delta.x; break;
				case RPShape::angle90_270: delta.x = -delta.x; delta.y = -delta.y; break;
			}
			break;
		}
		case RPShape::pcmd_scale_br: {
			switch ( a90 ) {
				case RPShape::angle90_0  : break;
				case RPShape::angle90_90 : delta.x = -delta.x; break;
				case RPShape::angle90_180: delta.x = -delta.x; delta.y = -delta.y; break;
				case RPShape::angle90_270: delta.y = -delta.y; break;
			}
			break;
		}
		case RPShape::pcmd_scale_bl: {
			switch ( a90 ) {
				case RPShape::angle90_0  : delta.x = -delta.x; break;
				case RPShape::angle90_90 : delta.x = -delta.x; delta.y = -delta.y; break;
				case RPShape::angle90_180: delta.y = -delta.y; break;
				case RPShape::angle90_270: break;
			}
			break;
		}
		case RPShape::pcmd_scale_r: {
			switch ( a90 ) {
				case RPShape::angle90_0  : delta.x = delta.x ; delta.y = 0; break;
				case RPShape::angle90_90 : delta.y = delta.y ; delta.x = 0; break;
				case RPShape::angle90_180: delta.x = -delta.x; delta.y = 0; break;
				case RPShape::angle90_270: delta.y = -delta.y; delta.x = 0; break;
			}
			break;
		}
		case RPShape::pcmd_scale_l: {
			switch ( a90 ) {
				case RPShape::angle90_0  : delta.x = -delta.x; delta.y = 0; break;
				case RPShape::angle90_90 : delta.y = -delta.y; delta.x = 0; break;
				case RPShape::angle90_180: delta.x = delta.x ; delta.y = 0; break;
				case RPShape::angle90_270: delta.y = delta.y ; delta.x = 0; break;
			}
			break;
		}
		case RPShape::pcmd_scale_t: {
			switch ( a90 ) {
				case RPShape::angle90_0  : delta.y = -delta.y; delta.x = 0; break;
				case RPShape::angle90_90 : delta.x = delta.x ; delta.y = 0; break;
				case RPShape::angle90_180: delta.y = delta.y ; delta.x = 0; break;
				case RPShape::angle90_270: delta.x = -delta.x; delta.y = 0; break;
			}
			break;
		}
		case RPShape::pcmd_scale_b: {
			switch ( a90 ) {
				case RPShape::angle90_0  : delta.y = delta.y ; delta.x = 0; break;
				case RPShape::angle90_90 : delta.x = -delta.x; delta.y = 0; break;
				case RPShape::angle90_180: delta.y = -delta.y; delta.x = 0; break;
				case RPShape::angle90_270: delta.x = delta.x ; delta.y = 0; break;
			}
			break;
		}
	}
}

void RPShape::getRectDelta( rp::rect& rect_old, rp::rect& rect_new, rp::point& delta, RPShape::angle90 a90, RPShape::PossibleCommand pcmd )
{
	rp::point c_old = rect_old.getCenter();
	rp::point c_new = rect_new.getCenter();
	switch ( pcmd ) {
		case RPShape::pcmd_scale_tl: {
			switch ( a90 ) {
				case RPShape::angle90_0  : {
					delta.x = rect_old.p_br().x - rect_new.p_br().x;
					delta.y = rect_old.p_br().y - rect_new.p_br().y;
					break;
				}
				case RPShape::angle90_90 : {
					delta.x = rect_old.p_bl().x - rect_new.p_bl().x;
					delta.y = rect_old.p_bl().y - rect_new.p_bl().y;
					break;
				}
				case RPShape::angle90_180: {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
				case RPShape::angle90_270: {
					delta.x = rect_old.p_tr().x - rect_new.p_tr().x;
					delta.y = rect_old.p_tr().y - rect_new.p_tr().y;
					break;
				}
			}
			break;
		}
		case RPShape::pcmd_scale_tr: {
			switch ( a90 ) {
				case RPShape::angle90_0  : {
					delta.x = rect_old.p_bl().x - rect_new.p_bl().x;
					delta.y = rect_old.p_bl().y - rect_new.p_bl().y;
					break;
				}
				case RPShape::angle90_90 : {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
				case RPShape::angle90_180: {
					delta.x = rect_old.p_tr().x - rect_new.p_tr().x;
					delta.y = rect_old.p_tr().y - rect_new.p_tr().y;
					break;
				}
				case RPShape::angle90_270: {
					delta.x = rect_old.p_br().x - rect_new.p_br().x;
					delta.y = rect_old.p_br().y - rect_new.p_br().y;
					break;
				}
			}
			break;
		}
		case RPShape::pcmd_scale_br: {
			switch ( a90 ) {
				case RPShape::angle90_0  : {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
				case RPShape::angle90_90 : {
					delta.x = rect_old.p_tr().x - rect_new.p_tr().x;
					delta.y = rect_old.p_tr().y - rect_new.p_tr().y;
					break;
				}
				case RPShape::angle90_180: {
					delta.x = rect_old.p_br().x - rect_new.p_br().x;
					delta.y = rect_old.p_br().y - rect_new.p_br().y;
					break;
				}
				case RPShape::angle90_270: {
					delta.x = rect_old.p_bl().x - rect_new.p_bl().x;
					delta.y = rect_old.p_bl().y - rect_new.p_bl().y;
					break;
				}
			}
			break;
		}
		case RPShape::pcmd_scale_bl: {
			switch ( a90 ) {
				case RPShape::angle90_0  : {
					delta.x = rect_old.p_tr().x - rect_new.p_tr().x;
					delta.y = rect_old.p_tr().y - rect_new.p_tr().y;
					break;
				}
				case RPShape::angle90_90 : {
					delta.x = rect_old.p_br().x - rect_new.p_br().x;
					delta.y = rect_old.p_br().y - rect_new.p_br().y;
					break;
				}
				case RPShape::angle90_180: {
					delta.x = rect_old.p_bl().x - rect_new.p_bl().x;
					delta.y = rect_old.p_bl().y - rect_new.p_bl().y;
					break;
				}
				case RPShape::angle90_270: {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
			}
			break;
		}
		case RPShape::pcmd_scale_r: {
			switch ( a90 ) {
				case RPShape::angle90_0  :
				case RPShape::angle90_90 : {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
				case RPShape::angle90_180:
				case RPShape::angle90_270: {
					delta.x = rect_old.p_br().x - rect_new.p_br().x;
					delta.y = rect_old.p_br().y - rect_new.p_br().y;
					break;
				}
			}
			break;
		}
		case RPShape::pcmd_scale_l: {
			switch ( a90 ) {
				case RPShape::angle90_0  :
				case RPShape::angle90_90 : {
					delta.x = rect_old.p_br().x - rect_new.p_br().x;
					delta.y = rect_old.p_br().y - rect_new.p_br().y;
					break;
				}
				case RPShape::angle90_180:
				case RPShape::angle90_270: {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
			}
			break;
		}
		case RPShape::pcmd_scale_t: {
			switch ( a90 ) {
				case RPShape::angle90_0  :
				case RPShape::angle90_270: {
					delta.x = rect_old.p_br().x - rect_new.p_br().x;
					delta.y = rect_old.p_br().y - rect_new.p_br().y;
					break;
				}
				case RPShape::angle90_90 :
				case RPShape::angle90_180: {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
			}
			break;
		}
		case RPShape::pcmd_scale_b: {
			switch ( a90 ) {
				case RPShape::angle90_0  :
				case RPShape::angle90_270: {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
				case RPShape::angle90_90 :
				case RPShape::angle90_180: {
					delta.x = rect_old.p_br().x - rect_new.p_br().x;
					delta.y = rect_old.p_br().y - rect_new.p_br().y;
					break;
				}
			}
			break;
		}
	}
}

void RPShape::onRButtonDown( UINT nFlags, CPoint global_chart_pos )
{
	UNUSED(nFlags          );
	UNUSED(global_chart_pos);

	std::vector< RPConnectorDock* >::const_iterator it = docks.begin();
	while ( it != docks.end() ) {
		std::list< RPConnector* >::const_iterator conn_it = (*it)->connectors.begin();
		while ( conn_it != (*it)->connectors.end() ) {
			RPConnectorDock* dock = (*conn_it)->getConnectedDock( **it );
			if ( dock ) {
				TRACE1( "%s", getName().c_str() );
				const RPObjectChart& obj = dock->object();
				if ( dock->isType( RPConnectorDock::inout ) ) {
					TRACE( " <--> " );
				} else if ( dock->isType( RPConnectorDock::in ) ) {
					TRACE( " ---> " );
				} else if ( dock->isType( RPConnectorDock::out ) ) {
					TRACE( " <--- " );
				}
				TRACE1( "%s\n", obj.getName().c_str() );
			}
			conn_it++;
		}
		it++;
	}
}

std::list< RPShape* > RPShape::getNextBlock()
{
	std::list< RPShape* > result;
	std::vector< RPConnectorDock* >::const_iterator it = docks.begin();
	while ( it != docks.end() ) {
		std::list< RPConnector* >::const_iterator conn_it = (*it)->connectors.begin();
		while ( conn_it != (*it)->connectors.end() ) {
			RPConnectorDock* dock = (*conn_it)->getConnectedDock( **it );
			if ( dock ) {
				const RPObjectChart& obj = dock->object();
				if ( dock->isType( RPConnectorDock::in ) && obj.isShape() ) {
					result.push_back( static_cast<RPShape*>(const_cast<RPObjectChart*>(&obj)) );
				} 
			}
			conn_it++;
		}
		it++;
	}
	return result;
}

void RPShape::setBgBrush( const CBrush& brush )
{
	LOGBRUSH lb;
	if ( const_cast<CBrush&>(brush).GetLogBrush( &lb ) ) {
		bg_brush.DeleteObject();
		bg_brush.CreateBrushIndirect( &lb );
		modify();
		update();
	}
}

void RPShape::setTextFont( const CFont& font, COLORREF color, rbool show )
{
	if ( show ) {
		LOGFONT lf;
		if ( const_cast<CFont&>(font).GetLogFont( &lf ) ) {
			text_font.DeleteObject();
			text_font.CreateFontIndirect( &lf );
			text_color = color;
			text_show  = show;
			modify();
			update();
		}
	} else {
		text_show = show;
		modify();
		update();
	}
}

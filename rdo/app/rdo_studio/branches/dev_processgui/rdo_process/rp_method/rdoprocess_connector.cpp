#include "app/rdo_studio_mfc/rdo_process/rp_method/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_connector.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object_flowchart.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_shape.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_xml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RPConnector
// --------------------------------------------------------------------------------
RPConnector::RPConnector( RPObject* _parent, const rp::string& _name ):
	RPObjectChart( _parent, _name ),
	dock_begin( NULL ),
	dock_end( NULL ),
	recursive( 0 )
{
	main_pen_width = 1;
	main_pen_default.DeleteObject();
	main_pen_default.CreatePen( PS_SOLID, 1, RGB(0x00, 0x00, 0x00) );
	can_update = false;
	setPen( main_pen_default );
	can_update = true;
}

RPConnector::~RPConnector()
{
	if ( dock_begin ) dock_begin->connectors.remove( this );
	if ( dock_end )   dock_end->connectors.remove( this );
}

void RPConnector::load( rp::RPXMLNode* node )
{
	RPObjectChart::load( node );
	RPObject* obj_from = rpMethod::project->findObject( node->getAttribute( "obj_from" ) );
	RPObject* obj_to   = rpMethod::project->findObject( node->getAttribute( "obj_to" ) );
	if ( obj_from && obj_to && obj_from->getClassInfo()->isKindOf( "RPShape" ) && obj_to->getClassInfo()->isKindOf( "RPShape" ) ) {
		dock_begin = static_cast<RPShape*>(obj_from)->getDock( node->getAttributeInt( "dock_index_from" ) );
		dock_end   = static_cast<RPShape*>(obj_to)->getDock( node->getAttributeInt( "dock_index_to" ) );
	}
}

rp::RPXMLNode* RPConnector::save( rp::RPXMLNode* parent_node )
{
	rp::RPXMLNode* obj_node = RPObjectChart::save( parent_node );
	obj_node->insertAttribute( "obj_from", dock_begin->object().getFullName() );
	obj_node->insertAttribute( "obj_to", dock_end->object().getFullName() );
	obj_node->insertAttribute( "dock_index_from", dock_begin->getIndex() );
	obj_node->insertAttribute( "dock_index_to", dock_end->getIndex() );
	return obj_node;
}

void RPConnector::saveToXML(REF(pugi::xml_node) parentNode) const
{
	// Записываем узел <RPConnector/>:
	pugi::xml_node node = parentNode.append_child(getClassName().c_str());
	// Соxраняем атрибуты объекта:
	node.append_attribute("obj_from")        .set_value(dock_begin->object().getFullName().c_str());
	node.append_attribute("obj_to")          .set_value(dock_end->  object().getFullName().c_str());
	node.append_attribute("index_from")      .set_value(dock_begin->getIndex()                    );
	node.append_attribute("index_to")        .set_value(dock_end->  getIndex()                    );
}

void RPConnector::loadFromXML(CREF(pugi::xml_node) node)
{
	PTR(RPObject) pObjFrom = NULL;
	PTR(RPObject) pObjTo   = NULL;
	unsigned int iFrom     = 0;
	unsigned int iTo       = 0;
	
	// Считываем атрибуты для загрузки сохраненного блока "Connector":
	for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute())
	{
		// Присваиваем сохраненные в xml-файле параметры:
		// Для отображения объекта на Flowchart'е
		tstring attrName = attr.name();
		if (attrName == _T("obj_from"))
		{
			pObjFrom = rpMethod::project->findObject(attr.value());
			// Для блока Resource восстанавливаем его коннектор (отличен от других):
			if (pObjFrom->getClassInfo()->isKindOf("RPShapeResource_MJ"))
			{
				main_pen_width = 1;
				main_pen_default.DeleteObject();
				main_pen_default.CreatePen( PS_DASHDOTDOT, 1, RGB(0x00, 0x00, 0xFF) );
				can_update = false;
				setPen( main_pen_default );
				can_update = true;
			}
		}
		else if (attrName == _T("obj_to"))
		{
			pObjTo = rpMethod::project->findObject(attr.value());
		}
		else if (attrName == _T("index_from"))
		{
			iFrom = attr.as_uint();
		}
		else if (attrName == _T("index_to"))
		{
			iTo = attr.as_uint();
		}
	}
	if (pObjFrom && pObjTo && pObjFrom->getClassInfo()->isKindOf("RPShape") && pObjTo->getClassInfo()->isKindOf("RPShape"))
	{
		dock_begin = static_cast<RPShape*>(pObjFrom)->getDock(iFrom);
		dock_end   = static_cast<RPShape*>(pObjTo)  ->getDock(iTo);
		dock_begin->connectors.push_back(this);
		dock_end  ->connectors.push_back(this);
	}
}

void RPConnector::registerObject()
{
	rpMethod::factory->insertFactory( new RPObjectClassInfo( "RPConnector", "RPObjectChart", RPConnector::newObject ) );
}

RPObject* RPConnector::newObject( RPObject* parent )
{
	return new RPConnector( parent );
}

void RPConnector::next_step( CDC& dc, const rp::point& p1, const rp::point& p2, double norm1, double norm2, rp::polyline& pa, rp::polyline& pa_post )
{
	recursive++;
	if ( recursive > 100 ) {
		TRACE( "recursive > 100\n" );
		return;
	}
	if ( p1 == p2 ) {
		TRACE( "found !!!\n" );
		return;
	}
	double k_cos1, k_cos2, k_sin1, k_sin2;
	rp::math::getCosSin( norm1, k_cos1, k_sin1 );
	rp::math::getCosSin( norm2, k_cos2, k_sin2 );

	// Создали отрезки p1->p12 и p2->21 в направлении нормалей
	double len = 1e10;
	rp::point p12( p1.x + len * k_cos1, p1.y - len * k_sin1 );
	rp::point p22( p2.x + len * k_cos2, p2.y - len * k_sin2 );
/*
	rp::point _p12( p1.x + 200 * k_cos1, p1.y - 200 * k_sin1 );
	rp::point _p22( p2.x + 200 * k_cos2, p2.y - 200 * k_sin2 );
	dc.TextOut( p1.x, p1.y, rp::string::fromdouble( norm1 ).c_str() );
	dc.TextOut( p2.x, p2.y, rp::string::fromdouble( norm2 ).c_str() );

	dc.SaveDC();
	CPen pen_green( PS_SOLID, 1, RGB(0x00, 0xFF, 0x00) );
	dc.SelectObject( &pen_green );
	dc.MoveTo( p1.x, p1.y );
	dc.LineTo( _p12.x, _p12.y );
	dc.MoveTo( p2.x, p2.y );
	dc.LineTo( _p22.x, _p22.y );
	dc.RestoreDC( -1 );

	RPShape* shape = (RPShape*)(&dock_begin->object());
	rp::rect rect = shape->getBoundingRectNoRotateOuter().extendByPerimetr( RPConnectorDock::delta );
	dc.SaveDC();
	CPen pen_red( PS_DASH, 1, RGB(0xFF, 0x00, 0x00) );
	dc.SelectObject( &pen_red );
	dc.MoveTo( rect.p0().x, rect.p0().y );
	dc.LineTo( rect.p1().x, rect.p1().y );
	dc.LineTo( rect.p2().x, rect.p2().y );
	dc.LineTo( rect.p3().x, rect.p3().y );
	dc.LineTo( rect.p0().x, rect.p0().y );
	shape = (RPShape*)(&dock_end->object());
	rect = shape->getBoundingRectNoRotateOuter().extendByPerimetr( RPConnectorDock::delta );
	dc.MoveTo( rect.p0().x, rect.p0().y );
	dc.LineTo( rect.p1().x, rect.p1().y );
	dc.LineTo( rect.p2().x, rect.p2().y );
	dc.LineTo( rect.p3().x, rect.p3().y );
	dc.LineTo( rect.p0().x, rect.p0().y );
	dc.RestoreDC( -1 );
*/
	double Ka, Kb, K, Ua, Ub;
	rp::point inter = rp::math::getIntersection( p1, p12, p2, p22, Ka, Kb, K, Ua, Ub );
	rbool intersect  = Ua >= 0 && Ua <= 1 && Ub >= 0 && Ub <= 1;
	//rbool parallel   = fabs(K) == 0.0;
	//rbool same       = fabs(K) == 0.0 && fabs(Ka) == 0.0 && fabs(Kb) == 0.0;
	if ( intersect ) {
		// Всё нормально, фигуры пересекаются своими прямыми под углом в 90-то градусов
		pa.push_back( inter );
		return;
	} else {
		RPShape* shape1 = (RPShape*)(&dock_begin->object());
		RPShape* shape2 = (RPShape*)(&dock_end->object());
		rp::rect rect1_small = shape1->getBoundingRectNoRotateOuter();
		rp::rect rect2_small = shape2->getBoundingRectNoRotateOuter();
		rp::rect rect1_big   = rect1_small.extendByPerimetr( RPConnectorDock::delta );
		rp::rect rect2_big   = rect2_small.extendByPerimetr( RPConnectorDock::delta );
		std::list< rp::point > inter1_big_point, inter2_big_point, inter1_small_point, inter2_small_point;
		rbool inter1_flag = rect1_big.isIntersection( p1, p2, inter1_big_point );
		rbool inter2_flag = rect2_big.isIntersection( p1, p2, inter2_big_point );

		double a  = rp::math::getAlpha( p1, p2 );
		double a1 = a - norm1;
		double a2 = 180 - a + norm2;
		if ( a1 >= 180  ) a1 -= 360;
		if ( a1 <= -180 ) a1 += 360;
		if ( a2 >= 180  ) a2 -= 360;
		if ( a2 <= -180 ) a2 += 360;
//		dc.TextOut( 10, 20 * recursive, rp::string::fromdouble( a1 ).c_str() );
/*
		dc.SaveDC();
		CPen pen( PS_SOLID, 1, RGB(0x00, 0x00, 0x00) );
		dc.SelectObject( &pen );
		CBrush brush1( inter1_flag && inter2_flag ? RGB(0xFF, 0x00, 0x00) : RGB(0x00, 0xFF, 0x00) );
		dc.SelectObject( &brush1 );
		std::list< rp::point >::const_iterator it = inter1_big_point.begin();
		while ( it != inter1_big_point.end() ) {
			dc.Ellipse( it->x - 3, it->y - 3, it->x + 3, it->y + 3 );
			it++;
		}
		CBrush brush2( inter1_flag && inter2_flag ? RGB(0xFF, 0x00, 0x00) : RGB(0xFF, 0xFF, 0x00) );
		dc.SelectObject( &brush2 );
		it = inter2_big_point.begin();
		while ( it != inter2_big_point.end() ) {
			dc.Ellipse( it->x - 3, it->y - 3, it->x + 3, it->y + 3 );
			it++;
		}
		dc.RestoreDC( -1 );
*/
		if ( !inter1_flag && !inter2_flag ) {
			// Всё нормально, можем соединять фигуры, направляющие которых параллельны и смотрят друг на друга.
			// Нужно сделать зиг-заг. Продливаем отрезок первой фигуры на половину расстояния
			double dx = fabs(p2.x - p1.x) / 2;
			double dy = fabs(p2.y - p1.y) / 2;
			rp::point p( p1.x + dx * k_cos1, p1.y - dy * k_sin1 );
			if ( a1 != 0 ) {
				// Повернуть и найти простую точку пересечения лучей
				pa.push_back( p );
				next_step( dc, p, p2, a1 < 0 ? norm1 - 90 : norm1 + 90, norm2, pa, pa_post );
			} else {
				// Если a1 == 0, то получается прямая линия, т.е. точку p в список тоек заносить не надо
			}
			return;
		} else if ( inter1_flag && !inter2_flag ) {
			// Пересекли первую фигуры
			norm1 = a1 < 0 ? norm1 - 90 : norm1 + 90;
			rp::math::getCosSin( norm1, k_cos1, k_sin1 );
			p12.x = p1.x + len * k_cos1;
			p12.y = p1.y - len * k_sin1;
			inter1_big_point.clear();
			if ( rect1_big.isIntersection( p1, p12, inter1_big_point ) ) {
/*
				dc.SaveDC();
				CPen pen( PS_SOLID, 1, RGB(0x00, 0x00, 0x00) );
				dc.SelectObject( &pen );
				CBrush brush1( RGB(0xFF, 0xF0, 0xFF) );
				dc.SelectObject( &brush1 );
				dc.Ellipse( inter1_big_point.front().x - 3, inter1_big_point.front().y - 3, inter1_big_point.front().x + 3, inter1_big_point.front().y + 3 );
				dc.RestoreDC( -1 );
*/
				pa.push_back( inter1_big_point.front() );
				next_step( dc, inter1_big_point.front(), p2, norm1, norm2, pa, pa_post );
			}
			return;

		} else if ( !inter1_flag && inter2_flag ) {
			// Пересекли вторую фигуры
			norm2 = a2 < 0 ? norm2 + 90 : norm2 - 90;
			rp::math::getCosSin( norm2, k_cos2, k_sin2 );
			p22.x = p2.x + len * k_cos2;
			p22.y = p2.y - len * k_sin2;
			inter2_big_point.clear();
			if ( rect2_big.isIntersection( p2, p22, inter2_big_point ) ) {
/*
				dc.SaveDC();
				CPen pen( PS_SOLID, 1, RGB(0x00, 0x00, 0x00) );
				dc.SelectObject( &pen );
				CBrush brush1( RGB(0xFF, 0x00, 0xFF) );
				dc.SelectObject( &brush1 );
				dc.Ellipse( inter2_big_point.front().x - 3, inter2_big_point.front().y - 3, inter2_big_point.front().x + 3, inter2_big_point.front().y + 3 );
				dc.RestoreDC( -1 );
*/
				pa_post.push_back( inter2_big_point.front() );
				next_step( dc, p1, inter2_big_point.front(), norm1, norm2, pa, pa_post );
			}
			return;
		} else if ( inter1_flag && inter2_flag ) {
			// Пересекли обе фигуры
			// Пересекли первую фигуры
			norm1 = a1 < 0 ? norm1 - 90 : norm1 + 90;
			rp::math::getCosSin( norm1, k_cos1, k_sin1 );
			p12.x = p1.x + len * k_cos1;
			p12.y = p1.y - len * k_sin1;
			inter1_big_point.clear();
			rbool flag1 = rect1_big.isIntersection( p1, p12, inter1_big_point );
			if ( flag1 ) {
				pa.push_back( inter1_big_point.front() );
			}
			// Пересекли вторую фигуры
			norm2 = a2 < 0 ? norm2 + 90 : norm2 - 90;
			rp::math::getCosSin( norm2, k_cos2, k_sin2 );
			p22.x = p2.x + len * k_cos2;
			p22.y = p2.y - len * k_sin2;
			inter2_big_point.clear();
			rbool flag2 = rect2_big.isIntersection( p2, p22, inter2_big_point );
			if ( flag2 ) {
				pa_post.push_back( inter2_big_point.front() );
			}
			next_step( dc, flag1 ? inter1_big_point.front() : p1, flag2 ? inter2_big_point.front() : p2, norm1, norm2, pa, pa_post );
			return;
		}
	}
}

void RPConnector::draw(REF(CDC) dc)
{
	if ( dock_begin && !dock_end ) {
		rp::polyline pa;
		pa.push_back( dock_begin->getPosition() );
		pa.push_back( dock_begin->getDeltaPosition() );
		pa.push_back( flowChart()->mouse_current() );
		CPen* old_pen = dc.SelectObject( const_cast<CPen*>(&getPen()) );
		pa.draw( dc );
//		dc.Polyline( &pa.getWinPolyline()[0], pa.size() );
		dc.SelectObject( old_pen );
	} else if ( dock_begin && dock_end ) {
		rp::point p1 = dock_begin->getPosition(); // центр дока
		rp::point p2 = dock_begin->getOutpoint(); // ближайшая к доку точка
		rp::point p3 = dock_end->getOutpoint();
		rp::point p4 = dock_end->getPosition();
		rp::polyline pa, pa_post;
		pa.push_back( p1 );
		pa.push_back( p2 );

		recursive = 0;
		double norm1 = rp::math::getAlpha(p1, p2);
		if ( norm1 > 45   && norm1 < 135 ) {
			norm1 = 90;
		} else if ( norm1 >= 135 && norm1 < 225 ) {
			norm1 = 180;
		} else if ( norm1 >= 225 && norm1 < 315 ) {
			norm1 = 270;
		} else {
			norm1 = 0;
		}
		double norm2 = rp::math::getAlpha(p4, p3);
		if ( norm2 > 45   && norm2 < 135 ) {
			norm2 = 90;
		} else if ( norm2 >= 135 && norm2 < 225 ) {
			norm2 = 180;
		} else if ( norm2 >= 225 && norm2 < 315 ) {
			norm2 = 270;
		} else {
			norm2 = 0;
		}
		next_step( dc, p2, p3, norm1, norm2, pa, pa_post );
		if ( !pa_post.empty() ) {
			rp::polyline::iterator it = pa_post.end() - 1;
			while ( true ) {
				pa.push_back( *it );
				if ( it == pa_post.begin() ) break;
				it--;
			}
		}

		pa.push_back( p3 );
		pa.push_back( p4 );

		CPen* old_pen = dc.SelectObject( const_cast<CPen*>(&getPen()) );
		dc.Polyline( &pa.getWinPolyline()[0], pa.size() );
		pa.draw( dc );
		dc.Ellipse( int(p3.x)-3, int(p3.y)-3, int(p3.x)+3, int(p3.y)+3 );
		dc.SelectObject( old_pen );
/*
		rp::point p1 = dock_begin->getDeltaPosition();
		rp::point p2 = dock_end->getDeltaPosition();
		CPen* old_pen = dc.SelectObject( &main_pen );
		dc.MoveTo( p1.x, p1.y );
		dc.LineTo( p2.x, p2.y );
		dc.SelectObject( old_pen );
*/

/*
		double norm1 = dock_begin->getNorm();
		double norm2 = dock_end->getNorm();
		double k_cos1, k_cos2, k_sin1, k_sin2;
		rp::math::getCosSin( norm1, k_cos1, k_sin1 );
		rp::math::getCosSin( norm2, k_cos2, k_sin2 );
		double len = RPConnectorDock::delta;
		rp::point p11 = dock_begin->getPosition();
		rp::point p12 = dock_begin->getDeltaPosition();
		rp::point p21 = dock_end->getDeltaPosition();
		rp::point p22 = dock_end->getPosition();

		double short_len = 0.0;
//		if ( flowChart->getShowConnectorPoint() ) {
//			short_len = 5.0;
//		} else {
//			short_len = 3.0;
//		}

		rp::polyline p1;
		p1.push_back( rp::point(p11.x + short_len * k_cos1, p11.y - short_len * k_sin1) );
		p1.push_back( p12 );
		recursive = 0;
		borders.clear();

#ifdef CON_DEBUG
		makeConnector( dc, p12, p21, norm1, norm2, p1 );
#else if
		makeConnector( p12, p21, norm1, norm2, p1 );
#endif

		borders.clear();
		recursive = 0;
		p1.push_back( p21 );
		p1.push_back( rp::point(p22.x + short_len * k_cos2, p22.y - short_len * k_sin2) );

		CPen* old_pen = dc.SelectObject( &main_pen );
		dc.Polyline( &p1.getWinPolyline()[0], p1.size() );
		dc.SelectObject( old_pen );
*/
	}
}

#ifdef CON_DEBUG
void RPConnector::makeConnector( CDC& dc, const rp::point& p1, const rp::point& p2, double norm1, double norm2, rp::polyline& pa )
#else if
void RPConnector::makeConnector( const rp::point& p1, const rp::point& p2, double norm1, double norm2, rp::polyline& pa )
#endif
{
	if ( recursive > 100 ) {
		TRACE( "recursive > 100\n" );
		return;
	}
	recursive++;
	if ( p1 == p2 ) {
		TRACE( "found !!!\n" );
		return;
	}
	double k_cos1, k_cos2, k_sin1, k_sin2;
	rp::math::getCosSin( norm1, k_cos1, k_sin1 );
	rp::math::getCosSin( norm2, k_cos2, k_sin2 );
	double len = 1e10;
	rp::point p12( p1.x + len * k_cos1, p1.y - len * k_sin1 );
	rp::point p21( p2.x + len * k_cos2, p2.y - len * k_sin2 );

	double Ka, Kb, K, Ua, Ub;
	rp::point inter = rp::math::getIntersection( p1, p12, p21, p2, Ka, Kb, K, Ua, Ub );
	// K = 0                      : the two lines are parallel
	// K = 0 and Ka = 0 and Kb = 0: the two lines are coincident
	// Ua and Ub = [0..1]         : the intersection point is within both line segments
	rbool parallel   = fabs(K) == 0.0;
	rbool same       = fabs(K) == 0.0 && fabs(Ka) == 0.0 && fabs(Kb) == 0.0;
//	rbool inetrsect  = Ua >= 0 && Ua <= 1 && Ub >= 0 && Ub <= 1;
	rbool intersectA = Ua >= 0 && Ua <= 1;
#ifdef CON_DEBUG
	TRACE( "from A(%f,%f) to B(%f,%f)\n", p1.x, p1.y, p2.x, p2.y );
	if ( same ) {
		TRACE( "same, %g, %g, %g, %g, %g, %f, %f\n", K, Ka, Kb, Ua, Ub, norm1, norm2 );
	} else if ( parallel ) {
		TRACE( "parallel, %g, %g, %g, %g, %g, %f, %f\n", K, Ka, Kb, Ua, Ub, norm1, norm2 );
	} else if ( intersectA ) {
		TRACE( "intersectA, %g, %g, %g, %g, %g, %f, %f, x = %f, y = %f\n", K, Ka, Kb, Ua, Ub, norm1, norm2, inter.x, inter.y );
	} else {
		TRACE( "no, %g, %g, %g, %g, %g, %f, %f\n", K, Ka, Kb, Ua, Ub, norm1, norm2 );
	}
#endif
	if ( parallel && !same ) {
		norm1 -= 90;
		TRACE( "-90\n" );
#ifdef CON_DEBUG
		makeConnector( dc, p1, p2, norm1, norm2, pa );
#else if
		makeConnector( p1, p2, norm1, norm2, pa );
#endif
	}
	if ( !parallel && !intersectA ) {
		norm1 -= 180;
		TRACE( "-180\n" );
#ifdef CON_DEBUG
		makeConnector( dc, p1, p2, norm1, norm2, pa );
#else if
		makeConnector( p1, p2, norm1, norm2, pa );
#endif
	}
	if ( (!parallel && intersectA) || same ) {
#ifdef CON_DEBUG
		rp::point _p12( p1.x + 100 * k_cos1, p1.y - 100 * k_sin1 );
		rp::point _p21( p2.x + 100 * k_cos2, p2.y - 100 * k_sin2 );
		dc.SaveDC();
		CPen pen_green( PS_SOLID, 1, RGB(0x00, 0xFF, 0x00) );
		dc.SelectObject( &pen_green );
		dc.MoveTo( p1.x, p1.y );
		dc.LineTo( _p12.x, _p12.y );
		dc.MoveTo( p2.x, p2.y );
		dc.LineTo( _p21.x, _p21.y );
		dc.RestoreDC( -1 );
#endif
		if ( same ) {
			inter = p2;
		} else if ( parallel ) {
			inter = p12;
		}
		double lengthA1 = rp::math::getLength( p1, inter );
		double lengthA2 = lengthA1;
		rp::point B1, B2, interborder;
#ifdef CON_DEBUG
		rbool truncate = getShortLine( dc, pa, p1, inter, lengthA2, B1, B2, interborder );
#else if
		rbool truncate = getShortLine( pa, p1, inter, lengthA2, B1, B2, interborder );
#endif
		if ( p2 == interborder ) {
			TRACE( "found !!! p2 == interborder\n" );
			return;
		}
		if ( truncate ) {
			rbool null;
			rp::point B3 = rp::math::getPerpendicular( B1, B2, p2, null );
//			CBDFlowChartScrollView::correctPoint( pa, B3 );
			double len1 = rp::math::getLength( B1, B3 );
			double len2 = rp::math::getLength( B2, B3 );
			rp::point& dir = len1 < len2 ? B1 : B2;
			if ( std::find( borders.begin(), borders.end(), dir ) == borders.end() ) {
				pa.push_back( interborder );
				pa.push_back( dir );
//				pa.putPoints( pa.size(), 1, interborder.x(), interborder.y() );
//				pa.putPoints( pa.size(), 1, dir.x(), dir.y() );
				borders.push_back( interborder );
				borders.push_back( dir );
				TRACE4( "around border: x1 = %f, y1 = %f, x2 = %f, y2 = %f\n", interborder.x, interborder.y, dir.x, dir.y );
				TRACE( "around border: next\n" );
#ifdef CON_DEBUG
				makeConnector( dc, dir, p2, norm1, norm2, pa );
#else if
				makeConnector( dir, p2, norm1, norm2, pa );
#endif
			} else {
				TRACE2( "border: loop found for (%f,%f)\n", dir.x, dir.y );
			}
		} else {
#ifdef CON_DEBUG
			dc.SaveDC();
			CPen pen_black( PS_SOLID, 1, RGB(0x00, 0x00, 0x00) );
			CBrush brush_yellow( RGB(0xFF, 0xFF, 0x00) );
			dc.SelectObject( &pen_black );
			dc.SelectObject( &brush_yellow );
			dc.Ellipse( inter.x - 3, inter.y - 3, inter.x + 3, inter.y + 3 );
			dc.RestoreDC( -1 );
#endif
			TRACE2( "red, x = %f, y = %f\n", inter.x, inter.y );
			pa.push_back( inter );
//			pa.putPoints( pa.size(), 1, inter.x(), inter.y() );
			norm1 += 90;
			TRACE( "normal: next, rotate normal (+90)\n" );
#ifdef CON_DEBUG
			makeConnector( dc, inter, p2, norm1, norm2, pa );
#else if
			makeConnector( inter, p2, norm1, norm2, pa );
#endif
		}
	}
}

#ifdef CON_DEBUG
rbool RPConnector::getShortLine( CDC& dc, const rp::polyline& pa, const rp::point& from, const rp::point& to, double& lengthA2, rp::point& B1, rp::point& B2, rp::point& interborder )
#else if
rbool RPConnector::getShortLine( const rp::polyline& pa, const rp::point& from, const rp::point& to, double& lengthA2, rp::point& B1, rp::point& B2, rp::point& interborder )
#endif
{
	UNUSED(pa);

	rbool flag = false;
	double lengthBorder = 1e10;
	RPObjectFlowChart* flowchart = flowChart();
	std::list< RPObjectChart* > objects;
	flowchart->getChartObjects( objects );
	std::list< RPObjectChart* >::iterator it = objects.begin();
	while ( it != objects.end() ) {
		if ( (*it)->isShape() ) {
			RPShape* shape = static_cast<RPShape*>(*it);
			rp::rect rect        = shape->getBoundingRect();
			rp::rect border_rect = shape->getBoundingRectNoRotateOuter().extendByPerimetr( RPConnectorDock::delta );
#ifdef CON_DEBUG
			dc.SaveDC();
			CPen pen_red( PS_DASH, 1, RGB(0xFF, 0x00, 0x00) );
			dc.SelectObject( &pen_red );
			dc.MoveTo( border_rect.p0().x, border_rect.p0().y );
			dc.LineTo( border_rect.p1().x, border_rect.p1().y );
			dc.LineTo( border_rect.p2().x, border_rect.p2().y );
			dc.LineTo( border_rect.p3().x, border_rect.p3().y );
			dc.LineTo( border_rect.p0().x, border_rect.p0().y );
			dc.RestoreDC( -1 );
#endif
			for ( int i = 0; i < 4; i++ ) {
				double x3, y3, x4, y4;
				if ( i == 0 ) {
					x3 = border_rect.p0().x;
					y3 = border_rect.p0().y;
					x4 = border_rect.p2().x;
					y4 = y3;
				} else if ( i == 1 ) {
					x3 = border_rect.p2().x;
					y3 = border_rect.p0().y;
					x4 = x3;
					y4 = border_rect.p2().y;
				} else if ( i == 2 ) {
					x3 = border_rect.p2().x;
					y3 = border_rect.p2().y;
					x4 = border_rect.p0().x;
					y4 = y3;
				} else { // if ( i == 3 )
					x3 = border_rect.p0().x;
					y3 = border_rect.p2().y;
					x4 = x3;
					y4 = border_rect.p0().y;
				}
				double Ka, Kb, K, Ua, Ub;
				rp::point _inter = rp::math::getIntersection( from, to, rp::point(x3,y3), rp::point(x4,y4), Ka, Kb, K, Ua, Ub );
				if ( fabs(K) > 0 ) {
					if ( !rect.pointInRect( from ) ) {
						if ( Ua >= 0.0 && Ua <= 1.0 && Ub >= 0.0 && Ub <= 1.0 ) {
							TRACE3( "K = %g, Ua = %g, Ub = %g\n", K, Ua, Ub );
							double len = rp::math::getLength( from, _inter );
							if ( lengthA2 >= len ) {
								rbool vect_inter = true;
								if ( border_rect.pointInRect( from ) ) {
#ifdef CON_DEBUG
									vect_inter = isVectoreIntersect( dc, shape, from, to );
#else if
									vect_inter = isVectoreIntersect( shape, from, to );
#endif
								}
								if ( !vect_inter ) {
									if ( std::find( borders.begin(), borders.end(), to ) != borders.end() ) {
										TRACE( "vect_inter found\n" );
										vect_inter = true;
									}
								}
								if ( vect_inter ) {
#ifdef CON_DEBUG
									dc.SaveDC();
									CPen pen_black( PS_SOLID, 1, RGB(0x00, 0x00, 0x00) );
									CBrush brush_blue( RGB(0x00, 0x00, 0xFF) );
									dc.SelectObject( &pen_black );
									dc.SelectObject( &brush_blue );
									dc.Ellipse( _inter.x - 3, _inter.y - 3, _inter.x + 3, _inter.y + 3 );
									dc.RestoreDC( -1 );
#endif
									lengthA2 = len;
									B1.x = x3;
									B1.y = y3;
									B2.x = x4;
									B2.y = y4;
//									CBDFlowChartScrollView::correctPoint( pa, B1 );
//									CBDFlowChartScrollView::correctPoint( pa, B2 );
									interborder = _inter;
									flag = true;
									TRACE3( "new lengthA2 = %g, (%f,%f)\n", lengthA2, interborder.x, interborder.y );
								} else {
									TRACE3( "skip lengthA2 (no intersection of shape) = %g, (%f,%f)\n", lengthA2, interborder.x, interborder.y );
								}
							} else {
								TRACE4( "no new lengthA2: %g < %g, x= %f, y = %f\n", lengthA2, len, _inter.x, _inter.y );
							}
						} else {
							TRACE3( "_K = %g, Ua = %g, Ub = %g\n", K, Ua, Ub );
						}
					} else {
						TRACE( "rect.contains( from )\n" );
						if ( Ub >= 0.0 && Ub <= 1.0 ) {
							TRACE3( "K = %g, Ua = %g, Ub = %g\n", K, Ua, Ub );
							double len = rp::math::getLength( from, _inter );
							if ( lengthBorder >= len ) {
#ifdef CON_DEBUG
								dc.SaveDC();
								CPen pen_black( PS_SOLID, 1, RGB(0x00, 0x00, 0x00) );
								CBrush brush_green( RGB(0x00, 0xFF, 0x00) );
								dc.SelectObject( &pen_black );
								dc.SelectObject( &brush_green );
								dc.Ellipse( _inter.x - 3, _inter.y - 3, _inter.x + 3, _inter.y + 3 );
								dc.RestoreDC( -1 );
#endif
								lengthBorder = len;
								lengthA2 = len;
								B1.x = x3;
								B1.y = y3;
								B2.x = x4;
								B2.y = y4;
//								CBDFlowChartScrollView::correctPoint( pa, B1 );
//								CBDFlowChartScrollView::correctPoint( pa, B2 );
								interborder = _inter;
								flag = true;
								TRACE3( "new lengthA2 = %g, (%f,%f)\n", lengthA2, interborder.x, interborder.y );
							} else {
								TRACE3( "skip lengthA2 (no intersection of shape) = %g, (%f,%f)\n", lengthA2, interborder.x, interborder.y );
							}
						} else {
							TRACE3( "_K = %g, Ua = %g, Ub = %g\n", K, Ua, Ub );
						}
					}
				} else {
					TRACE1( "K = %g\n", K );
				}
			}
		}
		it++;
	}
	return flag;
}

#ifdef CON_DEBUG
rbool RPConnector::isVectoreIntersect( CDC& dc, RPShape* shape, const rp::point& vect_begin, const rp::point& vect_end ) const
#else if
rbool RPConnector::isVectoreIntersect( RPShape* shape, const rp::point& vect_begin, const rp::point& vect_end ) const
#endif
{
	rp::rect rect = shape->getBoundingRectNoRotateOuter();
#ifdef CON_DEBUG
	dc.SaveDC();
	CPen pen_red( PS_DASH, 1, RGB(0xFF, 0x00, 0x00) );
	dc.SelectObject( &pen_red );
	dc.MoveTo( rect.p0().x, rect.p0().y );
	dc.LineTo( rect.p1().x, rect.p1().y );
	dc.LineTo( rect.p2().x, rect.p2().y );
	dc.LineTo( rect.p3().x, rect.p3().y );
	dc.LineTo( rect.p0().x, rect.p0().y );
	dc.RestoreDC( -1 );
#endif
	for ( int i = 0; i < 4; i++ ) {
		double x3, y3, x4, y4;
		if ( i == 0 ) {
			x3 = rect.p0().x;
			y3 = rect.p0().y;
			x4 = rect.p2().x;
			y4 = y3;
		} else if ( i == 1 ) {
			x3 = rect.p2().x;
			y3 = rect.p0().y;
			x4 = x3;
			y4 = rect.p2().y;
		} else if ( i == 2 ) {
			x3 = rect.p2().x;
			y3 = rect.p2().y;
			x4 = rect.p0().x;
			y4 = y3;
		} else { // if ( i == 3 )
			x3 = rect.p0().x;
			y3 = rect.p2().y;
			x4 = x3;
			y4 = rect.p0().y;
		}
		double Ka, Kb, K, Ua, Ub;
		rp::math::getIntersection( vect_begin, vect_end, rp::point(x3,y3), rp::point(x4,y4), Ka, Kb, K, Ua, Ub );
		if ( fabs(K) > 0 ) {
			if ( Ua >= 0.0 && Ua <= 1.0 && Ub >= 0.0 && Ub <= 1.0 ) {
				return true;
			}
		} else if ( fabs(K) == 0.0 && fabs(Ka) == 0.0 && fabs(Kb) == 0.0 && Ua >= 0.0 && Ua <= 1.0 && Ub >= 0.0 && Ub <= 1.0 ) {
			return true;
		}
	}
	return false;
}

RPConnectorDock* RPConnector::getConnectedDock( const RPConnectorDock& dock ) const
{
	if ( &dock == dock_begin ) return dock_end;
	if ( &dock == dock_end ) return dock_begin;
	return NULL;
}

// --------------------------------------------------------------------------------
// -------------------- RPConnectorDock
// --------------------------------------------------------------------------------
const int RPConnectorDock::delta = 10;

RPConnectorDock::~RPConnectorDock()
{
	std::list< RPConnector* >::const_iterator it = connectors.begin();
	while ( it != connectors.end() ) {
		if ( (*it)->dock_begin == this ) (*it)->dock_begin = NULL;
		if ( (*it)->dock_end   == this ) (*it)->dock_end   = NULL;
		it++;
	}
}

rp::RPXMLNode* RPConnectorDock::save( rp::RPXMLNode* parent_node )
{
	rp::RPXMLNode* obj_node = parent_node->makeChild( "dock" );
	obj_node->insertAttributeInt( "type", type );
	obj_node->insertAttribute( "x", point.x );
	obj_node->insertAttribute( "y", point.y );
	obj_node->insertAttribute( "norm", norm );
	obj_node->insertAttribute( "color", RPObjectChart::colorToStr(color()) );
	return obj_node;
}

#include "app/rdo_studio_mfc/rdo_process/rp_misc/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_polyline.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_rect.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rp {

// --------------------------------------------------------------------------------
// -------------------- polyline
// --------------------------------------------------------------------------------
rbool polyline::pointInPolygon( const rp::point& point ) const
{
	if ( size() > 2 ) {
		int counter  = 0;
		ruint i, j;
		rbool polygon     = isPolygon();
		ruint cnt = polygon ? size() - 1 : size();
		for ( i = 0, j = 1; i < cnt; i++, j++ ) {
			if ( !polygon && j == cnt ) {
				j = 0;
			}
			const rp::point& p1 = (*this)[i];
			const rp::point& p2 = (*this)[j];
			double y_min = p1.y < p2.y ? p1.y : p2.y;
			if ( point.y >= y_min ) {
				double y_max = p1.y > p2.y ? p1.y : p2.y;
				if ( point.y <= y_max ) {
					double x_max = p1.x > p2.x ? p1.x : p2.x;
					if ( point.x <= x_max ) {
						if ( fabs(p1.y - p2.y) > 1 ) {
							double xinters = (point.y-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
							if ( fabs(p1.x - p2.x) <= 1 || point.x <= xinters ) counter++;
						}
					}
				}
			}
		}
		return counter % 2 ? true : false;
	}
	return false;
}

/*
rbool polyline::pointInPolygon( const rp::point& point ) const
{
	if ( size() > 2 ) {
		ruint i;
		ruint j;
		double k0 = 0;
		rbool flag = true;
		rbool polygon = isPolygon();
		int  cnt     = polygon ? size() - 1 : size();
		for ( i = 0, j = 1; i < cnt; i++, j++ ) {
			if ( !polygon && j == cnt ) {
				j = 0;
			}
			const rp::point& p1 = (*this)[i];
			const rp::point& p2 = (*this)[j];
			double k = (point.y - p1.y)*(p2.x - p1.x) - (point.x - p1.x)*(p2.y - p1.y);
			if ( fabs(k) < 1 ) {
				// Попали в линию (прямую), необходимо проверить на попадание в отрезок фигуры
				double x_min = p1.x < p2.x ? p1.x : p2.x;
				double x_max = p1.x > p2.x ? p1.x : p2.x;
				double y_min = p1.y < p2.y ? p1.y : p2.y;
				double y_max = p1.y > p2.y ? p1.y : p2.y;
				if ( point.x >= x_min && point.x <= x_max && point.y >= y_min && point.y <= y_max ) return true;
			} else {
				if ( fabs(k0) < 1 ) {
					k0 = k;
				} else {
					if ( k0 * k < 0 ) {
						flag = false;
						break;
					}
				}
			}
		}
		return flag;
	}
	return false;
}


rbool polyline::pointInPolygon( const rp::point& point ) const
{
	if ( size() > 2 ) {
		ruint i;
		ruint j;
		rbool flag = false;
		rbool polygon = isPolygon();
		int  cnt     = polygon ? size() - 1 : size();
		for ( i = 0, j = 1; i < cnt; i++, j++ ) {
			if ( !polygon && j == cnt ) {
				j = cnt-1;
			}
			const rp::point& p1 = (*this)[i];
			const rp::point& p2 = (*this)[j];
			if ( (((p1.y <= point.y) && (point.y < p2.y)) || ((p2.y <= point.y) && (point.y < p1.y))) &&
			    (point.x < (p2.x - p1.x) * (point.y - p1.y) / (p2.y - p1.y) + p1.x))
			{
				flag = !flag;
			}
		}
		return flag;
	}
	return false;
}

void polyline::extendFromCenter( double delta )
{
	if ( size() < 2 ) return;
	double len, cos_a, sin_a;
	rp::point center = getCenter();
	std::vector< rp::point >::iterator it = begin();
	while ( it != end() ) {
		if ( rp::math::getPlanarData( center, *it, len, cos_a, sin_a ) ) {
			len += delta;
			it->x = cos_a * len + center.x;
			it->y = center.y - sin_a * len;
		}
		it++;
	}
}
*/

polyline& polyline::extendByPerimetr( double delta )
{
	if ( size() < 3 ) return *this;
	rbool polygon = isPolygon();
	std::vector< rp::point >::iterator it = begin();          // текущая точка
	rp::point p_left        = *(end() - (polygon ? 2 : 1));   // левая точка
	rp::point p_right       = *(begin() + 1);                 // правая точка
	rp::point p_right_begin = *(begin() + (polygon ? 1 : 0)); // начальная для правой в конце
	double cos_b, sin_b, koef;
	while ( it != end() ) {
		if ( rp::math::getPlanarData( p_left, *it, p_right, cos_b, sin_b, koef ) ) {
			p_left = *it;
			if ( koef < 1 ) koef = 1.0;
			it->x += cos_b * delta * koef;
			it->y -= sin_b * delta * koef;
		} else {
			p_left = *it;
		}
		it++;
		std::vector< rp::point >::iterator it_next = it;
		if ( it_next != end() ) {
			it_next++;
		}
		if ( it_next != end() ) {
			p_right = *it_next;
		} else {
			p_right = p_right_begin;
		}
	}
	return *this;
}

rp::rect polyline::getBoundingRect() const
{
	if ( empty() ) return rp::rect();
	std::vector< rp::point >::const_iterator it = begin();
	double max_x = it->x;
	double max_y = it->y;
	double min_x = it->x;
	double min_y = it->y;
	it++;
	while ( it != end() ) {
		if ( it->x < min_x ) min_x = it->x;
		if ( it->y < min_y ) min_y = it->y;
		if ( it->x > max_x ) max_x = it->x;
		if ( it->y > max_y ) max_y = it->y;
		it++;
	}
	return rp::rect( min_x, min_y, max_x, max_y );
}

}

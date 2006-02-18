#include "stdafx.h"
#include "rdoprocess_polyline.h"
#include "rdoprocess_math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rp {

// ----------------------------------------------------------------------------
// ---------- polyline
// ----------------------------------------------------------------------------
bool polyline::pointInPolygon( const rp::point& point ) const
{
	if ( size() > 2 ) {
		unsigned int i;
		unsigned int j;
		int k0 = 0;
		bool flag = true;
		bool polygon = isPolygon();
		int cnt = polygon ? size() - 1 : size();
		if ( !polygon ) {
			TRACE( "================================= !polygon\n" );
		} else {
			TRACE( "polygon\n" );
		}
		for ( i = 0, j = 1; i < cnt; i++, j++ ) {
			if ( !polygon && j == cnt ) j = 0;
			const rp::point& p1 = (*this)[i];
			const rp::point& p2 = (*this)[j];
			int k = (point.y - p1.y)*(p2.x - p1.x) - (point.x - p1.x)*(p2.y - p1.y);
			if ( k == 0 ) {
				break;
			} else {
				if ( k0 == 0 ) {
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

/*
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
	bool polygon = isPolygon();
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
		if ( (it + 1) != end() ) {
			p_right = *(it + 1);
		} else {
			p_right = p_right_begin;
		}
	}
	return *this;
}

}

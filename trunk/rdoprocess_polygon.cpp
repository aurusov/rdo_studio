#include "stdafx.h"
#include "rdoprocess_polygon.h"
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
bool polyline::pointInPolygon( int x, int y ) const
{
	if ( size() > 2 ) {
		unsigned int i;
		unsigned int j;
		int k0 = 0;
		bool flag = true;
		for ( i = 0, j = 1; i < size()-1; i++, j++ ) {
			const CPoint& p1 = (*this)[i];
			const CPoint& p2 = (*this)[j];
			int k = (y - p1.y)*(p2.x - p1.x) - (x - p1.x)*(p2.y - p1.y);
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

void polyline::extendFromCenter( int delta )
{
	if ( size() < 2 ) return;
	double len, cos_a, sin_a;
	CPoint center = getCenter();
	std::vector< CPoint >::iterator it = begin();
	while ( it != end() ) {
		if ( rp::math::getPlanarData( center, *it, len, cos_a, sin_a ) ) {
			len += delta;
			it->x = cos_a * len + center.x;
			it->y = center.y - sin_a * len;
		}
		it++;
	}
}

polyline& polyline::extendByPerimetr( int delta )
{
	if ( size() < 3 ) return *this;
	bool polygon = isPolygon();
	std::vector< CPoint >::iterator it1 = end() - (polygon ? 2 : 1);
	std::vector< CPoint >::iterator it2 = begin();
	std::vector< CPoint >::iterator it3 = it2;
	it3++;
	double cos_b, sin_b;
	while ( it2 != end() ) {
		if ( rp::math::getPlanarData( *it1, *it2, *it3, cos_b, sin_b ) ) {
			it2->x = cos_b * static_cast<double>(delta) + it2->x;
			it2->y = it2->y - sin_b * static_cast<double>(delta);
		}
		it1++;
		it2++;
		it3++;
		if ( it1 == end() ) polygon ? it1 = begin() + 1 : it1 = begin();
		if ( it3 == end() ) polygon ? it3 = begin() + 1 : it3 = begin();
	}
	return *this;
}

}

#include "stdafx.h"
#include "rdoprocess_rect.h"
#include "rdoprocess_math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rp {

// ----------------------------------------------------------------------------
// ---------- RPRect
// ----------------------------------------------------------------------------
bool RPRect::pointInRect( int x, int y ) const
{
	unsigned int i;
	unsigned int j;
	int k0 = 0;
	bool flag = true;
	for ( i = 0, j = 1; i < 4; i++, j++ ) {
		if ( j == 4 ) j = 0;
		const CPoint& p1 = pa[i];
		const CPoint& p2 = pa[j];
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

void RPRect::extendFromCenter( int delta )
{
	double len, cos_a, sin_a;
	CPoint center = getCenter();
	if ( rp::math::getPlanarData( center, pa[0], len, cos_a, sin_a ) ) {
		len += delta;
		pa[0].x = cos_a * len + center.x;
		pa[0].y = center.y - sin_a * len;
	}
	if ( rp::math::getPlanarData( center, pa[1], len, cos_a, sin_a ) ) {
		len += delta;
		pa[1].x = cos_a * len + center.x;
		pa[1].y = center.y - sin_a * len;
	}
	if ( rp::math::getPlanarData( center, pa[2], len, cos_a, sin_a ) ) {
		len += delta;
		pa[2].x = cos_a * len + center.x;
		pa[2].y = center.y - sin_a * len;
	}
	if ( rp::math::getPlanarData( center, pa[3], len, cos_a, sin_a ) ) {
		len += delta;
		pa[3].x = cos_a * len + center.x;
		pa[3].y = center.y - sin_a * len;
	}
}

RPRect& RPRect::extendByPerimetr( int delta )
{
	double cos_b, sin_b;
	if ( rp::math::getPlanarData( pa[3], pa[0], pa[1], cos_b, sin_b ) ) {
		pa[0].x += cos_b * static_cast<double>(delta);
		pa[0].y -= sin_b * static_cast<double>(delta);
	}
	if ( rp::math::getPlanarData( pa[0], pa[1], pa[2], cos_b, sin_b ) ) {
		pa[1].x += cos_b * static_cast<double>(delta);
		pa[1].y -= sin_b * static_cast<double>(delta);
	}
	if ( rp::math::getPlanarData( pa[1], pa[2], pa[3], cos_b, sin_b ) ) {
		pa[2].x += cos_b * static_cast<double>(delta);
		pa[2].y -= sin_b * static_cast<double>(delta);
	}
	if ( rp::math::getPlanarData( pa[2], pa[3], pa[0], cos_b, sin_b ) ) {
		pa[3].x += cos_b * static_cast<double>(delta);
		pa[3].y -= sin_b * static_cast<double>(delta);
	}
	return *this;
}

}

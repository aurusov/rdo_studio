#ifndef RDO_PROCESS_RECT_H
#define RDO_PROCESS_RECT_H

#include <list>
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_polyline.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_matrix.h"

namespace rp {

// --------------------------------------------------------------------------------
// -------------------- rect
// --------------------------------------------------------------------------------
class rect {
private:
	polyline pa;
public:
	rect() {
		pa.reserve( 4 );
		pa.push_back( rp::point(0, 0) );
		pa.push_back( rp::point(0, 0) );
		pa.push_back( rp::point(0, 0) );
		pa.push_back( rp::point(0, 0) );
	}
	rect( double x_min, double y_min, double x_max, double y_max  ) {
		pa.reserve( 4 );
		pa.push_back( rp::point(x_min, y_min) );
		pa.push_back( rp::point(x_max, y_min) );
		pa.push_back( rp::point(x_max, y_max) );
		pa.push_back( rp::point(x_min, y_max) );
	}
	rect( const CRect& rect ) {
		pa.reserve( 4 );
		pa.push_back( rp::point(rect.left, rect.top) );
		pa.push_back( rp::point(rect.right, rect.top) );
		pa.push_back( rp::point(rect.right, rect.bottom) );
		pa.push_back( rp::point(rect.left, rect.bottom) );
	}
	rect( const rect& rect ) {
		pa = rect.pa;
	}
	rect& operator= ( const rect& rect ) {
		pa = rect.pa;
		return *this;
	}
	double getMinX() const {
		double min_x = pa[0].x;
		if ( min_x > pa[1].x ) min_x = pa[1].x;
		if ( min_x > pa[2].x ) min_x = pa[2].x;
		if ( min_x > pa[3].x ) min_x = pa[3].x;
		return min_x;
	}
	double getMinY() const {
		double min_y = pa[0].y;
		if ( min_y > pa[1].y ) min_y = pa[1].y;
		if ( min_y > pa[2].y ) min_y = pa[2].y;
		if ( min_y > pa[3].y ) min_y = pa[3].y;
		return min_y;
	}
	double getMaxX() const {
		double max_x = pa[0].x;
		if ( max_x < pa[1].x ) max_x = pa[1].x;
		if ( max_x < pa[2].x ) max_x = pa[2].x;
		if ( max_x < pa[3].x ) max_x = pa[3].x;
		return max_x;
	}
	double getMaxY() const {
		double max_y = pa[0].y;
		if ( max_y < pa[1].y ) max_y = pa[1].y;
		if ( max_y < pa[2].y ) max_y = pa[2].y;
		if ( max_y < pa[3].y ) max_y = pa[3].y;
		return max_y;
	}
	rp::point getCenter() const {
		return rp::point( (pa[0].x + pa[2].x)/2, (pa[0].y + pa[2].y)/2 );
	}
	rp::rect getBoundingRect() const {
		return pa.getBoundingRect();
	}
	void transform( const rp::matrix& matrix ) {
		pa[0] = matrix * pa[0];
		pa[1] = matrix * pa[1];
		pa[2] = matrix * pa[2];
		pa[3] = matrix * pa[3];
	}
	rp::point& p0()   { return pa[0]; }
	rp::point& p1()   { return pa[1]; }
	rp::point& p2()   { return pa[2]; }
	rp::point& p3()   { return pa[3]; }
	rp::point& p_tl() { return pa[0]; }
	rp::point& p_tr() { return pa[1]; }
	rp::point& p_br() { return pa[2]; }
	rp::point& p_bl() { return pa[3]; }
	const rp::point& p_tl() const { return pa[0]; }
	const rp::point& p_tr() const { return pa[1]; }
	const rp::point& p_br() const { return pa[2]; }
	const rp::point& p_bl() const { return pa[3]; }
	rp::point  p_t() const { return rp::point( (pa[0].x + pa[1].x) / 2, (pa[0].y + pa[1].y) / 2 ); }
	rp::point  p_b() const { return rp::point( (pa[3].x + pa[2].x) / 2, (pa[3].y + pa[2].y) / 2 ); }
	rp::point  p_l() const { return rp::point( (pa[0].x + pa[3].x) / 2, (pa[0].y + pa[3].y) / 2 ); }
	rp::point  p_r() const { return rp::point( (pa[1].x + pa[2].x) / 2, (pa[1].y + pa[2].y) / 2 ); }

	rbool pointInRect( const rp::point& point ) const;
//	void extendFromCenter( double delta );
	rect& extendByPerimetr( double delta );
	rbool isIntersection( const rp::point& p1, const rp::point& p2, std::list< rp::point >& inter );

	void draw(REF(CDC) dc) const {
		dc.MoveTo( static_cast<int>(pa[0].x), static_cast<int>(pa[0].y) );
		dc.LineTo( static_cast<int>(pa[1].x), static_cast<int>(pa[1].y) );
		dc.LineTo( static_cast<int>(pa[2].x), static_cast<int>(pa[2].y) );
		dc.LineTo( static_cast<int>(pa[3].x), static_cast<int>(pa[3].y) );
		dc.LineTo( static_cast<int>(pa[0].x), static_cast<int>(pa[0].y) );
	}

#ifdef _DEBUG
	void trace() const {
		TRACE2( "p0.x = %f, p0.y = %f\n", pa[0].x, pa[0].y );
		TRACE2( "p1.x = %f, p1.y = %f\n", pa[1].x, pa[1].y );
		TRACE2( "p2.x = %f, p2.y = %f\n", pa[2].x, pa[2].y );
		TRACE2( "p3.x = %f, p3.y = %f\n", pa[3].x, pa[3].y );
	}
#endif
};

} // namespace rp

#endif // RDO_PROCESS_RECT_H

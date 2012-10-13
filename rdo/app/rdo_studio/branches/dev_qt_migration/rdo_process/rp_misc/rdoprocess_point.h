#ifndef RDO_PROCESS_POINT_H
#define RDO_PROCESS_POINT_H
#include <math.h>
namespace rp {

// --------------------------------------------------------------------------------
// -------------------- point
// --------------------------------------------------------------------------------
class point {
public:
	double x;
	double y;

	point()                        : x( 0 )      , y( 0 )       {};
	point( const rp::point& point ): x( point.x ), y( point.y ) {};
	point( const CPoint& point )   : x( point.x ), y( point.y ) {};
	point( double _x, double _y )  : x( _x )     , y( _y )      {};

	rp::point& operator= ( const rp::point& point ) {
		x = point.x;
		y = point.y;
		return *this;
	}
	rbool operator== ( const rp::point& point ) const {
		return fabs(x - point.x) < 1.0 && fabs(y - point.y) < 1.0;
	}
	rbool softCompare( const rp::point& point ) const {
		return fabs(x - point.x) < 2.0 && fabs(y - point.y) < 2.0;
	}
//	operator CPoint() { return CPoint( x, y ); }

#ifdef _DEBUG
	void trace() {
		TRACE2( "x = %f, y = %f\n", x, y );
	}
#endif
};

} // namespace rp

#endif // RDO_PROCESS_POINT_H

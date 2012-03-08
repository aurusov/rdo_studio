#ifndef RDO_PROCESS_MATH_H
#define RDO_PROCESS_MATH_H

#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_point.h"

namespace rp {

class math
{
public:
	static double pi;

	static double getLength( const rp::point& point1, const rp::point& point2 ) {
		double res = sqrt( (point1.x - point2.x)*(point1.x - point2.x) + (point1.y - point2.y)*(point1.y - point2.y) );
		return res < 1e-10 ? -1 : res;
	}
	static double getDistance( const rp::point& line_point1, const rp::point& line_point2, const rp::point& point, rbool* inside = NULL );
	static double getAlpha( const rp::point& p1, const rp::point& p2 );
	static double getAlpha( const rp::point& p1, const rp::point& p2_center, const rp::point& p3 );
	static rbool getPlanarData( const rp::point& p1, const rp::point& p2, double& len, double& cos_a, double& sin_a );
	static rbool getPlanarData( const rp::point& p1, const rp::point& p2, const rp::point& p3, double& cos_b, double& sin_b, double& koef );
	static rp::point getPerpendicular( const rp::point& line_point1, const rp::point& line_point2, const rp::point& point, rbool& null, rbool* inside = NULL );
	static rp::point getIntersection( const rp::point& p1, const rp::point& p2, const rp::point& p3, const rp::point p4, double& Ka, double& Kb, double& K, double& Ua, double& Ub );
	static void getCosSin( double alpha, double& cos_a, double& sin_a );
};

} // namespace rp

#endif // RDO_PROCESS_MATH_H

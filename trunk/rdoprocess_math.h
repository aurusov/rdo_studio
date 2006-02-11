#ifndef RDO_PROCESS_MATH_H
#define RDO_PROCESS_MATH_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace rp {

class math
{
public:
	static double pi;

	static double getLength( const CPoint& point1, const CPoint& point2 );
	static double getDistance( const CPoint& line_point1, const CPoint& line_point2, const CPoint& point, bool* inside = NULL );
	static double getAlpha( int x1, int y1, int x2, int y2 );
	static bool getPlanarData( const CPoint& p1, const CPoint& p2, double& len, double& cos_a, double& sin_a );
	static bool getPlanarData( const CPoint& p1, const CPoint& p2, const CPoint& p3, double& cos_b, double& sin_b );
	static CPoint getPerpendicular( const CPoint& line_point1, const CPoint& line_point2, const CPoint& point, bool& null, bool* inside = NULL );
	static CPoint getIntersection( const std::vector< CPoint >& pa, const double x1, const double y1, const double x2, const double y2, const double x3, const double y3, const double x4, const double y4, double& Ka, double& Kb, double& K, double& Ua, double& Ub );
};

}

#endif // RDO_PROCESS_MATH_H

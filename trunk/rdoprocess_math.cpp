#include "stdafx.h"
#include "rdoprocess_math.h"

#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rp {

double math::pi = 3.14159265358979323846;

bool math::getPlanarData( const CPoint& p1, const CPoint& p2, double& len, double& cos_a, double& sin_a )
{
	len = math::getLength( p1, p2 );
	if ( len != -1 ) {
		cos_a = static_cast<double>((p2.x-p1.x))/len;
		sin_a = static_cast<double>((p1.y-p2.y))/len;
		return true;
	} else {
		len   = 0;
		cos_a = 1;
		sin_a = 0;
		return false;
	}
}

bool math::getPlanarData( const CPoint& p1, const CPoint& p2, const CPoint& p3, double& cos_b, double& sin_b )
{
	double len1 = math::getLength( p1, p2 );
	double len2 = math::getLength( p2, p3 );
	if ( len1 != -1 && len2 != -1 ) {
		double cos_a1 = static_cast<double>((p1.x-p2.x))/len1;
		double sin_a1 = static_cast<double>((p2.y-p1.y))/len1;
		double cos_a2 = static_cast<double>((p3.x-p2.x))/len2;
		double sin_a2 = static_cast<double>((p2.y-p3.y))/len2;
		double a1 = acos( cos_a1 ) * 180.0 / pi;
		if ( sin_a1 < 0.0 ) {
			a1 = 360.0 - a1;
		}
		double a2 = acos( cos_a2 ) * 180.0 / pi;
		if ( sin_a2 < 0.0 ) {
			a2 = 360.0 - a2;
		}
		double a = a2 - a1;
		if ( a < 0 ) a += 360;
		double b = 180 + a1 + a / 2;
		cos_b = cos( b * pi / 180.0 );
		sin_b = sin( b * pi / 180.0 );
		return true;
	} else {
		cos_b = 1;
		sin_b = 0;
		return false;
	}
}

double math::getDistance( const CPoint& line_point1, const CPoint& line_point2, const CPoint& point, bool* inside )
{
	bool null;
	CPoint p = math::getPerpendicular( line_point1, line_point2, point, null, inside );
	if ( !null ) {
		double l1 = p.x - point.x;
		double l2 = p.y - point.y;
		return sqrt( l1*l1 + l2*l2 );
	}
	return -1.0;
}

double math::getAlpha( const CPoint& p1, const CPoint& p2 )
{
	double len = math::getLength( p1, p2 );
	if ( len != -1 ) {
		double sin_a = static_cast<double>((p1.y-p2.y))/len;
		double cos_a = static_cast<double>((p2.x-p1.x))/len;
		double alpha = acos( cos_a ) * 180.0 / pi;
		if ( sin_a < 0.0 ) {
			alpha = 360.0 - alpha;
		}
		return alpha;
	}
	return 0;
}

double math::getAlpha( const CPoint& p1, const CPoint& p2_center, const CPoint& p3 )
{
	double len1 = math::getLength( p1, p2_center );
	double len2 = math::getLength( p2_center, p3 );
	if ( len1 != -1 && len2 != -1 ) {
		double cos_a1 = static_cast<double>((p1.x-p2_center.x))/len1;
		double sin_a1 = static_cast<double>((p2_center.y-p1.y))/len1;
		double cos_a2 = static_cast<double>((p3.x-p2_center.x))/len2;
		double sin_a2 = static_cast<double>((p2_center.y-p3.y))/len2;
		double a1 = acos( cos_a1 ) * 180.0 / pi;
		if ( sin_a1 < 0.0 ) {
			a1 = 360.0 - a1;
		}
		double a2 = acos( cos_a2 ) * 180.0 / pi;
		if ( sin_a2 < 0.0 ) {
			a2 = 360.0 - a2;
		}
		return a2 - a1;
	}
	return 0;
}

CPoint math::getPerpendicular( const CPoint& line_point1, const CPoint& line_point2, const CPoint& point, bool& null, bool* inside )
{
	double l1 = line_point2.x - line_point1.x;
	double l2 = line_point2.y - line_point1.y;
	double k = l1*l1 + l2*l2;
	if ( k > 1e-20 ) {
		double u = ((point.x - line_point1.x)*(line_point2.x - line_point1.x) + (point.y - line_point1.y)*(line_point2.y - line_point1.y))/k;
		if ( inside ) {
			*inside = u >= 0 && u <= 1;
		}
		null = false;
		return CPoint( static_cast<int>(line_point1.x + u*(line_point2.x - line_point1.x)), static_cast<int>(line_point1.y + u*(line_point2.y - line_point1.y)) );
	} else {
		if ( inside ) {
			*inside = false;
		}
	}
	null = true;
	return CPoint();
}

CPoint math::getIntersection( const std::vector< CPoint >& pa, const double x1, const double y1, const double x2, const double y2, const double x3, const double y3, const double x4, const double y4, double& Ka, double& Kb, double& K, double& Ua, double& Ub )
{
	K  = (y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1);
	Ka = (x4 - x3)*(y1 - y3) - (y4 - y3)*(x1 - x3);
	Kb = (x2 - x1)*(y1 - y3) - (y2 - y1)*(x1 - x3);
	try {
		Ua = Ka/K;
		Ub = Kb/K;
		double x = x1 + Ua*(x2 - x1);
		double y = y1 + Ua*(y2 - y1);
		CPoint point( static_cast<int>(x), static_cast<int>(y) );
		if ( point.x >= x1 - 1 && point.x <= x1 + 1 ) {
			point.x = static_cast<int>(x1);
		}
		if ( point.y >= y1 - 1 && point.y <= y1 + 1 ) {
			point.y = static_cast<int>(y1);
		}
		if ( point.x >= x2 - 1 && point.x <= x2 + 1 ) {
			point.x = static_cast<int>(x2);
		}
		if ( point.y >= y2 - 1 && point.y <= y2 + 1 ) {
			point.y = static_cast<int>(y2);
		}
		if ( point.x >= x3 - 1 && point.x <= x3 + 1 ) {
			point.x = static_cast<int>(x3);
		}
		if ( point.y >= y3 - 1 && point.y <= y3 + 1 ) {
			point.y = static_cast<int>(y3);
		}
		if ( point.x >= x4 - 1 && point.x <= x4 + 1 ) {
			point.x = static_cast<int>(x4);
		}
		if ( point.y >= y4 - 1 && point.y <= y4 + 1 ) {
			point.y = static_cast<int>(y4);
		}
		return point;
	} catch ( ... ) {
		return CPoint( 0, 0 );
	}
}

}
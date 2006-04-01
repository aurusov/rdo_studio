#include "stdafx.h"
#include "rdoprocess_math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rp {

double math::pi = 3.14159265358979323846;

bool math::getPlanarData( const rp::point& p1, const rp::point& p2, double& len, double& cos_a, double& sin_a )
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

bool math::getPlanarData( const rp::point& p1, const rp::point& p2, const rp::point& p3, double& cos_b, double& sin_b, double& koef )
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
		koef  = sin( a * pi / 180.0 / 2.0 );
		if ( fabs(koef) > 1e-10 ) {
			koef = 1 / koef;
		} else {
			koef = 1e+10;
		}
		return true;
	} else {
		cos_b = 1;
		sin_b = 0;
		koef  = 1;
		return false;
	}
}

double math::getDistance( const rp::point& line_point1, const rp::point& line_point2, const rp::point& point, bool* inside )
{
	bool null;
	rp::point p = math::getPerpendicular( line_point1, line_point2, point, null, inside );
	if ( !null ) return math::getLength( p, point );
	return -1.0;
}

double math::getAlpha( const rp::point& p1, const rp::point& p2 )
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

double math::getAlpha( const rp::point& p1, const rp::point& p2_center, const rp::point& p3 )
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

rp::point math::getPerpendicular( const rp::point& line_point1, const rp::point& line_point2, const rp::point& point, bool& null, bool* inside )
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
		return rp::point( static_cast<int>(line_point1.x + u*(line_point2.x - line_point1.x)), static_cast<int>(line_point1.y + u*(line_point2.y - line_point1.y)) );
	} else {
		if ( inside ) {
			*inside = false;
		}
	}
	null = true;
	return rp::point();
}

rp::point math::getIntersection( const rp::point& p1, const rp::point& p2, const rp::point& p3, const rp::point p4, double& Ka, double& Kb, double& K, double& Ua, double& Ub )
{
	K  = (p4.y - p3.y)*(p2.x - p1.x) - (p4.x - p3.x)*(p2.y - p1.y);
	Ka = (p4.x - p3.x)*(p1.y - p3.y) - (p4.y - p3.y)*(p1.x - p3.x);
	Kb = (p2.x - p1.x)*(p1.y - p3.y) - (p2.y - p1.y)*(p1.x - p3.x);
	try {
		Ua = Ka/K;
		Ub = Kb/K;
		double x = p1.x + Ua*(p2.x - p1.x);
		double y = p1.y + Ua*(p2.y - p1.y);
		rp::point point( x, y );
		if ( point.x >= p1.x - 1 && point.x <= p1.x + 1 ) {
			point.x = p1.x;
		}
		if ( point.y >= p1.y - 1 && point.y <= p1.y + 1 ) {
			point.y = p1.y;
		}
		if ( point.x >= p2.x - 1 && point.x <= p2.x + 1 ) {
			point.x = p2.x;
		}
		if ( point.y >= p2.y - 1 && point.y <= p2.y + 1 ) {
			point.y = p2.y;
		}
		if ( point.x >= p3.x - 1 && point.x <= p3.x + 1 ) {
			point.x = p3.x;
		}
		if ( point.y >= p3.y - 1 && point.y <= p3.y + 1 ) {
			point.y = p3.y;
		}
		if ( point.x >= p4.x - 1 && point.x <= p4.x + 1 ) {
			point.x = p4.x;
		}
		if ( point.y >= p4.y - 1 && point.y <= p4.y + 1 ) {
			point.y = p4.y;
		}
		return point;
	} catch ( ... ) {
		return rp::point();
	}
}

}
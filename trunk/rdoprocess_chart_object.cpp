#include "stdafx.h"
#include "rdoprocess_chart_object.h"
#include "rdoprocess_flowchart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const double pi = 3.14159265358979323846;

namespace rp {
// ----------------------------------------------------------------------------
// ---------- RPPolygon
// ----------------------------------------------------------------------------
bool RPPolygon::pointInPolygon( int x, int y ) const
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

void RPPolygon::extendFromCenter( int delta )
{
	if ( size() < 2 ) return;
	double len, cos_a, sin_a;
	CPoint center = getCenter();
	std::vector< CPoint >::iterator it = begin();
	while ( it != end() ) {
		if ( RDOPROCChartObject::getPlanarData( center, *it, len, cos_a, sin_a ) ) {
			len += delta;
			it->x = cos_a * len + center.x;
			it->y = center.y - sin_a * len;
		}
		it++;
	}
}

RPPolygon& RPPolygon::extendByPerimetr( int delta )
{
	if ( size() < 3 ) return *this;
	bool polygon = isPolygon();
	std::vector< CPoint >::iterator it1 = end() - (polygon ? 2 : 1);
	std::vector< CPoint >::iterator it2 = begin();
	std::vector< CPoint >::iterator it3 = it2;
	it3++;
	double cos_b, sin_b;
	while ( it2 != end() ) {
		if ( RDOPROCChartObject::getPlanarData( *it1, *it2, *it3, cos_b, sin_b ) ) {
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
	if ( RDOPROCChartObject::getPlanarData( center, pa[0], len, cos_a, sin_a ) ) {
		len += delta;
		pa[0].x = cos_a * len + center.x;
		pa[0].y = center.y - sin_a * len;
	}
	if ( RDOPROCChartObject::getPlanarData( center, pa[1], len, cos_a, sin_a ) ) {
		len += delta;
		pa[1].x = cos_a * len + center.x;
		pa[1].y = center.y - sin_a * len;
	}
	if ( RDOPROCChartObject::getPlanarData( center, pa[2], len, cos_a, sin_a ) ) {
		len += delta;
		pa[2].x = cos_a * len + center.x;
		pa[2].y = center.y - sin_a * len;
	}
	if ( RDOPROCChartObject::getPlanarData( center, pa[3], len, cos_a, sin_a ) ) {
		len += delta;
		pa[3].x = cos_a * len + center.x;
		pa[3].y = center.y - sin_a * len;
	}
}

RPRect& RPRect::extendByPerimetr( int delta )
{
	double cos_b, sin_b;
	if ( RDOPROCChartObject::getPlanarData( pa[3], pa[0], pa[1], cos_b, sin_b ) ) {
		pa[0].x += cos_b * static_cast<double>(delta);
		pa[0].y -= sin_b * static_cast<double>(delta);
	}
	if ( RDOPROCChartObject::getPlanarData( pa[0], pa[1], pa[2], cos_b, sin_b ) ) {
		pa[1].x += cos_b * static_cast<double>(delta);
		pa[1].y -= sin_b * static_cast<double>(delta);
	}
	if ( RDOPROCChartObject::getPlanarData( pa[1], pa[2], pa[3], cos_b, sin_b ) ) {
		pa[2].x += cos_b * static_cast<double>(delta);
		pa[2].y -= sin_b * static_cast<double>(delta);
	}
	if ( RDOPROCChartObject::getPlanarData( pa[2], pa[3], pa[0], cos_b, sin_b ) ) {
		pa[3].x += cos_b * static_cast<double>(delta);
		pa[3].y -= sin_b * static_cast<double>(delta);
	}
	return *this;
}
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCChartObject
// ----------------------------------------------------------------------------
RDOPROCChartObject::RDOPROCChartObject( RDOPROCObject* _parent, RDOPROCChartObject* _chart_parent, RDOPROCFlowChart* _flowchart ):
	RDOPROCObject( _parent ),
	rotate_center_inited( false ),
	chart_parent( _chart_parent ),
	flowchart( _flowchart ),
	main_pen_width( 20 )
{
	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = RGB(0x00, 0x00, 0x00);
	lb.lbHatch = 0;
//	main_pen.CreatePen( PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE | PS_JOIN_MITER, main_pen_width, &lb );
	main_pen.CreatePen( PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE | PS_JOIN_MITER, main_pen_width, RGB(0,0,0) );
}

RDOPROCChartObject::~RDOPROCChartObject()
{
}

RDOPROCMatrix RDOPROCChartObject::globalMatrix() const
{
	return chart_parent ? chart_parent->globalMatrix() * matrix_transform * matrix_rotate * matrix_scale : matrix_transform * matrix_rotate * matrix_scale;
}

RDOPROCMatrix RDOPROCChartObject::parentMatrix() const
{
	return chart_parent ? chart_parent->matrix_transform : RDOPROCMatrix();
}

void RDOPROCChartObject::meshToGlobal()
{
	if ( pa_global.size() != pa_src.size() ) {
		pa_global.resize( pa_src.size() );
	}
	trans tr( globalMatrix() );
	std::transform( pa_src.begin(), pa_src.end(), pa_global.begin(), tr );
}

void RDOPROCChartObject::transformToGlobal()
{
	meshToGlobal();
}

void RDOPROCChartObject::moving( int dx, int dy )
{
	matrix_transform.dx() += dx;
	matrix_transform.dy() += dy;
	flowchart->modify();
}

void RDOPROCChartObject::setRotation( double alpha )
{
	alpha = alpha * pi / 180.0;
	double cosa = cos( alpha );
	double sina = sin( alpha );
//	if ( fabs(cosa) < 1e-10 ) cosa = 0;
//	if ( fabs(sina) < 1e-10 ) sina = 0;
	matrix_rotate.data[0][0] = cosa;
	matrix_rotate.data[1][1] = cosa;
	matrix_rotate.data[0][1] = -sina;
	matrix_rotate.data[1][0] = sina;
}

rp::RPRect RDOPROCChartObject::getBoundingRect( bool global ) const
{
	rp::RPRect bound_rect;
	if ( !pa_src.empty() ) {
		int x_min = pa_src[0].x;
		int y_min = pa_src[0].y;
		int x_max = pa_src[0].x;
		int y_max = pa_src[0].y;
		std::vector< CPoint >::const_iterator it = pa_src.begin() + 1;
		while ( it != pa_src.end() ) {
			if ( x_min > it->x ) x_min = it->x;
			if ( y_min > it->y ) y_min = it->y;
			if ( x_max < it->x ) x_max = it->x;
			if ( y_max < it->y ) y_max = it->y;
			it++;
		}
		rp::RPRect rect( x_min, y_min, x_max, y_max );
		if ( global ) rect.transform( globalMatrix() );
		bound_rect = rect;
	}
	return bound_rect;
}

double RDOPROCChartObject::getLength( const CPoint& point1, const CPoint& point2 )
{
	try {
		return sqrt( (point1.x - point2.x)*(point1.x - point2.x) + (point1.y - point2.y)*(point1.y - point2.y) );
	} catch (...) {
		return -1;
	}
}

bool RDOPROCChartObject::getPlanarData( const CPoint& p1, const CPoint& p2, double& len, double& cos_a, double& sin_a )
{
	len = RDOPROCChartObject::getLength( p1, p2 );
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

bool RDOPROCChartObject::getPlanarData( const CPoint& p1, const CPoint& p2, const CPoint& p3, double& cos_b, double& sin_b )
{
	double len1 = RDOPROCChartObject::getLength( p1, p2 );
	double len2 = RDOPROCChartObject::getLength( p2, p3 );
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
		TRACE( "a1 = %f, a2 = %f, a2 - a1 = %f, b = %f\n", a1, a2, a2 - a1, b );
		cos_b = cos( b * pi / 180.0 );
		sin_b = sin( b * pi / 180.0 );
		return true;
	} else {
		cos_b = 1;
		sin_b = 0;
		return false;
	}
}

double RDOPROCChartObject::getDistance( const CPoint& line_point1, const CPoint& line_point2, const CPoint& point, bool* inside )
{
	bool null;
	CPoint p = getPerpendicular( line_point1, line_point2, point, null, inside );
	if ( !null ) {
		double l1 = p.x - point.x;
		double l2 = p.y - point.y;
		return sqrt( l1*l1 + l2*l2 );
	}
	return -1.0;
}

double RDOPROCChartObject::getAlpha( int x1, int y1, int x2, int y2 )
{
	double a_len = sqrt(static_cast<double>((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
	double a_sin = static_cast<double>((y1-y2))/a_len;
	double a_cos = static_cast<double>((x2-x1))/a_len;
	double alpha = acos( a_cos ) * 180.0 / pi;
	if ( a_sin < 0.0 ) {
		alpha = 360.0 - alpha;
	}
	return alpha;
}

CPoint RDOPROCChartObject::getPerpendicular( const CPoint& line_point1, const CPoint& line_point2, const CPoint& point, bool& null, bool* inside )
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

CPoint RDOPROCChartObject::getIntersection( const std::vector< CPoint >& pa, const double x1, const double y1, const double x2, const double y2, const double x3, const double y3, const double x4, const double y4, double& Ka, double& Kb, double& K, double& Ua, double& Ub )
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

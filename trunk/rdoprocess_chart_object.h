#ifndef RDO_PROCESS_CHART_OBJECT_H
#define RDO_PROCESS_CHART_OBJECT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_object.h"
#include "rdoprocess_matrix.h"

namespace rp {

// ----------------------------------------------------------------------------
// ---------- RPPolygon
// ----------------------------------------------------------------------------
class RPPolygon: public std::vector< CPoint > {
public:
	int getMinX() const {
		if ( empty() ) return 0;
		std::vector< CPoint >::const_iterator it = begin();
		int value = it->x;
		it++;
		while ( it != end() ) {
			if ( value > it->x ) value = it->x;
			it++;
		}
		return value;
	}
	int getMinY() const {
		if ( empty() ) return 0;
		std::vector< CPoint >::const_iterator it = begin();
		int value = it->y;
		it++;
		while ( it != end() ) {
			if ( value > it->y ) value = it->y;
			it++;
		}
		return value;
	}
	int getMaxX() const {
		if ( empty() ) return 0;
		std::vector< CPoint >::const_iterator it = begin();
		int value = it->x;
		it++;
		while ( it != end() ) {
			if ( value < it->x ) value = it->x;
			it++;
		}
		return value;
	}
	int getMaxY() const {
		if ( empty() ) return 0;
		std::vector< CPoint >::const_iterator it = begin();
		int value = it->y;
		it++;
		while ( it != end() ) {
			if ( value < it->y ) value = it->y;
			it++;
		}
		return value;
	}
	CPoint getCenter() const {
		return CPoint( (getMaxX() + getMinX()) / 2, (getMaxY() + getMinY()) / 2 );
	}
	bool isPolygon() const {
		if ( size() < 3 ) return false;
		return (*begin() == *(end() -1)) ? true : false;
	}
	void extendFromCenter( int delta );
	RPPolygon& extendByPerimetr( int delta );
	bool pointInPolygon( int x, int y ) const;
};

// ----------------------------------------------------------------------------
// ---------- RPRect
// ----------------------------------------------------------------------------
class RPRect {
private:
	RPPolygon pa;
public:
	RPRect() {
		pa.reserve( 4 );
		pa.push_back( CPoint(0, 0) );
		pa.push_back( CPoint(0, 0) );
		pa.push_back( CPoint(0, 0) );
		pa.push_back( CPoint(0, 0) );
	}
	RPRect( int x_min, int y_min, int x_max, int y_max  ) {
		pa.reserve( 4 );
		pa.push_back( CPoint(x_min, y_min) );
		pa.push_back( CPoint(x_max, y_min) );
		pa.push_back( CPoint(x_max, y_max) );
		pa.push_back( CPoint(x_min, y_max) );
	}
	RPRect( const CRect& rect ) {
		pa.reserve( 4 );
		pa.push_back( CPoint(rect.left, rect.top) );
		pa.push_back( CPoint(rect.right, rect.top) );
		pa.push_back( CPoint(rect.right, rect.bottom) );
		pa.push_back( CPoint(rect.left, rect.bottom) );
	}
	RPRect( const RPRect& rect ) {
		pa = rect.pa;
	}
	RPRect& operator= ( const RPRect& rect ) {
		pa = rect.pa;
		return *this;
	}
/*
	int getMinX() const {
		int min_x = pa[0].x;
		if ( min_x > pa[1].x ) min_x = pa[1].x;
		if ( min_x > pa[2].x ) min_x = pa[2].x;
		if ( min_x > pa[3].x ) min_x = pa[3].x;
		return min_x;
	}
	int getMinY() const {
		int min_y = pa[0].y;
		if ( min_y > pa[1].y ) min_y = pa[1].y;
		if ( min_y > pa[2].y ) min_y = pa[2].y;
		if ( min_y > pa[3].y ) min_y = pa[3].y;
		return min_y;
	}
*/
	int getMaxX() const {
		int max_x = pa[0].x;
		if ( max_x < pa[1].x ) max_x = pa[1].x;
		if ( max_x < pa[2].x ) max_x = pa[2].x;
		if ( max_x < pa[3].x ) max_x = pa[3].x;
		return max_x;
	}
	int getMaxY() const {
		int max_y = pa[0].y;
		if ( max_y < pa[1].y ) max_y = pa[1].y;
		if ( max_y < pa[2].y ) max_y = pa[2].y;
		if ( max_y < pa[3].y ) max_y = pa[3].y;
		return max_y;
	}
	CPoint getCenter() const {
		return CPoint( (pa[0].x + pa[2].x)/2, (pa[0].y + pa[2].y)/2 );
	}
	void transform( const RDOPROCMatrix& matrix ) {
		pa[0] = matrix * pa[0];
		pa[1] = matrix * pa[1];
		pa[2] = matrix * pa[2];
		pa[3] = matrix * pa[3];
	}
	CPoint& p0() { return pa[0]; }
	CPoint& p1() { return pa[1]; }
	CPoint& p2() { return pa[2]; }
	CPoint& p3() { return pa[3]; }

	void extendFromCenter( int delta );
	RPRect& extendByPerimetr( int delta );
	bool pointInRect( int x, int y ) const;

#ifdef _DEBUG
	void trace() const {
		TRACE( "p0.x = %d, p0.y = %d\n", pa[0].x, pa[0].y );
		TRACE( "p1.x = %d, p1.y = %d\n", pa[1].x, pa[1].y );
		TRACE( "p2.x = %d, p2.y = %d\n", pa[2].x, pa[2].y );
		TRACE( "p3.x = %d, p3.y = %d\n", pa[3].x, pa[3].y );
	}
#endif
};
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCChartObject
// ----------------------------------------------------------------------------
class RDOPROCFlowChart;

class RDOPROCChartObject: public RDOPROCObject
{
private:
	CPoint rotate_center;
	bool   rotate_center_inited;

protected:
	RDOPROCChartObject* chart_parent;
	RDOPROCFlowChart* flowchart;
	int  main_pen_width;
	CPen main_pen;

	struct trans {
		trans( RDOPROCMatrix& _matrix ): matrix( _matrix ) {};
		CPoint operator()( const CPoint& point ) {
			return matrix * point;
		}
		RDOPROCMatrix& matrix;
	};
	rp::RPPolygon pa_src;
	rp::RPPolygon pa_global;

	RDOPROCMatrix matrix_transform;
	RDOPROCMatrix matrix_rotate;
	RDOPROCMatrix matrix_scale;

	RDOPROCMatrix globalMatrix() const;
	RDOPROCMatrix parentMatrix() const;

	virtual void moving( int dx, int dy );

public:
	RDOPROCChartObject( RDOPROCObject* parent, RDOPROCChartObject* chart_parent, RDOPROCFlowChart* flowchart );
	virtual ~RDOPROCChartObject();

	// Позиция
	virtual void setPosition( double posx, double posy ) {
		matrix_transform.dx() = posx;
		matrix_transform.dy() = posy;
	}
	double getX()               { return matrix_transform.dx();                }
	double getY()               { return matrix_transform.dy();                }
	void setX( double value )   { setPosition( value, matrix_transform.dy() ); }
	void setY( double value )   { setPosition( matrix_transform.dx(), value ); }

	// Масштаб
	virtual void setScale( double sx, double sy ) {
		matrix_scale.sx() = sx;
		matrix_scale.sy() = sy;
	}
	int getScaleX()                { return matrix_scale.sx();             }
	int getScaleY()                { return matrix_scale.sy();             }
	void setScaleX( double value ) { setScale( value, matrix_scale.sy() ); }
	void setScaleY( double value ) { setScale( matrix_scale.sx(), value ); }

	// Поворот
	virtual void setRotation( double alpha );

	virtual void setSelected( bool value ) = 0;
	virtual void draw( CDC& dc ) = 0;

	virtual rp::RPRect getBoundingRect( bool global = true ) const;

	CPoint getCenter() const {
		return globalMatrix() * getBoundingRect( false ).getCenter();
	}
	CPoint getRotateCenter() {
		if ( !rotate_center_inited ) {
			rotate_center = getBoundingRect( false ).getCenter();
			rotate_center_inited = true;
		}
		return globalMatrix() * rotate_center;
	}

	void meshToGlobal();
	virtual void transformToGlobal();
	virtual bool pointInPolygon( int x, int y, bool byperimetr = true ) {
		meshToGlobal();
		if ( byperimetr ) {
			return pa_global.extendByPerimetr( main_pen_width * sqrt(2) / 2.0 ).pointInPolygon( x, y );
		} else {
			return pa_global.pointInPolygon( x, y );
		}
	}

	static double getLength( const CPoint& point1, const CPoint& point2 );
	static double getDistance( const CPoint& line_point1, const CPoint& line_point2, const CPoint& point, bool* inside = NULL );
	static double getAlpha( int x1, int y1, int x2, int y2 );
	static bool getPlanarData( const CPoint& p1, const CPoint& p2, double& len, double& cos_a, double& sin_a );
	static bool getPlanarData( const CPoint& p1, const CPoint& p2, const CPoint& p3, double& cos_b, double& sin_b );
	static CPoint getPerpendicular( const CPoint& line_point1, const CPoint& line_point2, const CPoint& point, bool& null, bool* inside = NULL );
	static CPoint getIntersection( const std::vector< CPoint >& pa, const double x1, const double y1, const double x2, const double y2, const double x3, const double y3, const double x4, const double y4, double& Ka, double& Kb, double& K, double& Ua, double& Ub );
};

#endif // RDO_PROCESS_CHART_OBJECT_H

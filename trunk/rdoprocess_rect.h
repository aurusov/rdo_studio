#ifndef RDO_PROCESS_RECT_H
#define RDO_PROCESS_RECT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_polyline.h"
#include "rdoprocess_matrix.h"

namespace rp {

// ----------------------------------------------------------------------------
// ---------- rect
// ----------------------------------------------------------------------------
class rect {
private:
	polyline pa;
public:
	rect() {
		pa.reserve( 4 );
		pa.push_back( CPoint(0, 0) );
		pa.push_back( CPoint(0, 0) );
		pa.push_back( CPoint(0, 0) );
		pa.push_back( CPoint(0, 0) );
	}
	rect( int x_min, int y_min, int x_max, int y_max  ) {
		pa.reserve( 4 );
		pa.push_back( CPoint(x_min, y_min) );
		pa.push_back( CPoint(x_max, y_min) );
		pa.push_back( CPoint(x_max, y_max) );
		pa.push_back( CPoint(x_min, y_max) );
	}
	rect( const CRect& rect ) {
		pa.reserve( 4 );
		pa.push_back( CPoint(rect.left, rect.top) );
		pa.push_back( CPoint(rect.right, rect.top) );
		pa.push_back( CPoint(rect.right, rect.bottom) );
		pa.push_back( CPoint(rect.left, rect.bottom) );
	}
	rect( const rect& rect ) {
		pa = rect.pa;
	}
	rect& operator= ( const rect& rect ) {
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
	void transform( const rp::matrix& matrix ) {
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
	rect& extendByPerimetr( int delta );
	bool pointInRect( const CPoint& point ) const;

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

#endif // RDO_PROCESS_RECT_H

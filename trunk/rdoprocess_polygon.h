#ifndef RDO_PROCESS_POLYGON_H
#define RDO_PROCESS_POLYGON_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

}

#endif // RDO_PROCESS_POLYGON_H

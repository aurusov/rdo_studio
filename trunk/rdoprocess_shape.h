#ifndef RDO_PROCESS_SHAPE_H
#define RDO_PROCESS_SHAPE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_object.h"
#include <vector>

// ----------------------------------------------------------------------------
// ---------- RDOPROCShape
// ----------------------------------------------------------------------------
class RDOPROCShape: public RDOPROCObject
{
friend class RDOPROCFlowChart;

protected:
	RDOPROCFlowChart* flowchart;

	struct trans {
		trans( int _dx, int _dy ): dx(_dx), dy(_dy) {};
		CPoint& operator()( CPoint& point ) {
			point.Offset( dx, dy );
			return point;
		}
		int dx, dy;
	};
	std::vector< CPoint > pa;

	int x;
	int y;
	CRect bound_rect;

	CPoint snap_to_point;

	virtual void translate( const int dx, const int dy );
	virtual bool pointInPolygon( const int x, const int y ) const;

	virtual void move( const int x, const int y );

	virtual void drawPolyline( CDC& dc );
//	virtual void drawConnectorsInput( CDC& dc );
//	virtual void drawConnectorsOutput( CDC& dc );

	virtual void moving( const int dx, const int dy );

public:
	RDOPROCShape( RDOPROCFlowChart* flowchart, RDOPROCObject* parent );
	virtual ~RDOPROCShape();

	virtual void setSelected( const bool value );

	int getX() const                 { return x;    }
	void setX( const int value );

	int getY() const                 { return y;    }
	void setY( const int value );

	virtual void moveTo( const int x, const int y );

	const CSize getSize() { return getBoundingRect().Size(); }
	CPoint getCenter()    { return CPoint( getSize().cx / 2, getSize().cy / 2 ); }
	CRect getBoundingRect();

	const CPoint& getSnapToPoint() const { return snap_to_point; }

	virtual void draw( CDC& dc );
};

#endif // RDO_PROCESS_SHAPE_H

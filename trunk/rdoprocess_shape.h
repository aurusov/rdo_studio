#ifndef RDO_PROCESS_SHAPE_H
#define RDO_PROCESS_SHAPE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_chart_object.h"

#include <vector>

// ----------------------------------------------------------------------------
// ---------- RPShape
// ----------------------------------------------------------------------------
class RPShape: public RPChartObject
{
friend class RPFlowChartObject;

protected:
//	CPoint snap_to_point;

	struct trans {
		trans( rp::matrix& _matrix ): matrix( _matrix ) {};
		rp::point operator()( const rp::point& point ) {
			return matrix * point;
		}
		rp::matrix& matrix;
	};
	rp::polyline pa_src;
	rp::polyline pa_global;

	virtual void drawPolyline( CDC& dc );
//	virtual void drawConnectorsInput( CDC& dc );
//	virtual void drawConnectorsOutput( CDC& dc );

	virtual void onLButtonDown( UINT nFlags, CPoint flowchart_mouse_pos );
	virtual void onLButtonUp( UINT nFlags, CPoint flowchart_mouse_pos );
	virtual void onLButtonDblClk( UINT nFlags, CPoint flowchart_mouse_pos );
	virtual void onRButtonDown( UINT nFlags, CPoint flowchart_mouse_pos );
	virtual void onRButtonUp( UINT nFlags, CPoint flowchart_mouse_pos );
	virtual void onMouseMove( UINT nFlags, CPoint flowchart_mouse_pos );

public:
	RPShape( RPObject* parent, RPChartObject* chart_parent, RPFlowChart* flowchart, const rp::string& name = "object" );
	virtual ~RPShape();

	virtual void setPosition( int x, int y );

//	const CPoint& getSnapToPoint() const { return snap_to_point; }

	virtual void draw( CDC& dc );
	virtual void draw1( CDC& dc );

	// Габориты фигуры
	virtual rp::rect getBoundingRect( bool global = true ) const;
	virtual rp::rect getMaxRect() {
		transformToGlobal();
		pa_global.extendByPerimetr( static_cast<double>(main_pen_width) / 2.0 );
		return rp::rect( pa_global.getMinX(), pa_global.getMinY(), pa_global.getMaxX(), pa_global.getMaxY() );
	}

	// Перевод всех элементов фигуры в глобальные координаты
	virtual void transformToGlobal();
	// Находится ли точка внутри фигуры
	virtual bool pointInPolygon( const rp::point& point, bool byperimetr = true ) {
		transformToGlobal();
		if ( byperimetr ) {
			return pa_global.extendByPerimetr( static_cast<double>(main_pen_width) / 2.0 ).pointInPolygon( point );
		} else {
			return pa_global.pointInPolygon( point );
		}
	}
	virtual PossibleCommand getPossibleCommand( const rp::point& global_pos, bool for_cursor = false );
};

#endif // RDO_PROCESS_SHAPE_H

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

	virtual RPProject::Cursor getCursor( const rp::point& global_chart_pos );

	virtual void onLButtonDown( UINT nFlags, CPoint global_chart_pos );
	virtual void onLButtonUp( UINT nFlags, CPoint global_chart_pos );
	virtual void onLButtonDblClk( UINT nFlags, CPoint global_chart_pos );
	virtual void onRButtonDown( UINT nFlags, CPoint global_chart_pos );
	virtual void onRButtonUp( UINT nFlags, CPoint global_chart_pos );
	virtual void onMouseMove( UINT nFlags, CPoint global_chart_pos );

public:
	RPShape( RPObject* parent, const rp::string& name = "object" );
	virtual ~RPShape();

	virtual void setPosition( int x, int y );

//	const CPoint& getSnapToPoint() const { return snap_to_point; }

	virtual void draw( CDC& dc );
	virtual void draw_selected( CDC& dc );
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
	virtual bool pointInPolygon( const rp::point& global_chart_pos ) {
		transformToGlobal();
		return pa_global.extendByPerimetr( static_cast<double>(main_pen_width) / 2.0 ).pointInPolygon( global_chart_pos );
	}
	// Находится ли точка в служебной (неклиентской) части фигуры (прямоугольник выделения, к примеру)
	virtual bool pointInNCArea( const rp::point& global_chart_pos );
	// Возможная команда над объектом
	virtual PossibleCommand getPossibleCommand( const rp::point& global_chart_pos, bool for_cursor = false );
};

#endif // RDO_PROCESS_SHAPE_H

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
friend class RPFlowChart;

protected:
//	CPoint snap_to_point;

	struct trans {
		trans( rp::matrix& _matrix ): matrix( _matrix ) {};
		CPoint operator()( const CPoint& point ) {
			return matrix * point;
		}
		rp::matrix& matrix;
	};
	rp::polyline pa_src;
	rp::polyline pa_global;

	virtual void drawPolyline( CDC& dc );
//	virtual void drawConnectorsInput( CDC& dc );
//	virtual void drawConnectorsOutput( CDC& dc );

public:
	RPShape( RPObject* parent, RPFlowChart* flowchart, const rp::string& name = "object" );
	virtual ~RPShape();

	virtual void setPosition( int x, int y );

//	const CPoint& getSnapToPoint() const { return snap_to_point; }

	virtual void draw( CDC& dc );

	// Габориты фигуры
	virtual rp::rect getBoundingRect( bool global = true ) const;
	// Перевод всех элементов фигуры в глобальные координаты
	virtual void transformToGlobal();
	// Находится ли точка внутри фигуры
	virtual bool pointInPolygon( const CPoint& point, bool byperimetr = true ) {
		transformToGlobal();
		if ( byperimetr ) {
			return pa_global.extendByPerimetr( main_pen_width * sqrt(2) / 2.0 ).pointInPolygon( point );
		} else {
			return pa_global.pointInPolygon( point );
		}
	}
	virtual PossibleCommand getPossibleCommand( const CPoint& global_pos ) const;
};

#endif // RDO_PROCESS_SHAPE_H

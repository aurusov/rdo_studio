#ifndef RDO_PROCESS_CHART_OBJECT_H
#define RDO_PROCESS_CHART_OBJECT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_object.h"
#include "rdoprocess_matrix.h"

// ----------------------------------------------------------------------------
// ---------- RDOPROCChartObject
// ----------------------------------------------------------------------------
class RDOPROCFlowChart;

class RDOPROCChartObject: public RDOPROCObject
{
protected:
	RDOPROCChartObject* chart_parent;
	RDOPROCFlowChart* flowchart;

	struct trans {
		trans( RDOPROCMatrix& _matrix ): matrix( _matrix ) {};
		CPoint operator()( const CPoint& point ) {
			return matrix * point;
		}
		RDOPROCMatrix& matrix;
	};
	std::vector< CPoint > pa_src;
	std::vector< CPoint > pa_global;

	RDOPROCMatrix matrix_transform;
	RDOPROCMatrix matrix_rotate;
	RDOPROCMatrix matrix_scale;

	RDOPROCMatrix globalMatrix() const;
	RDOPROCMatrix parentMatrix() const;

	virtual void moving( int dx, int dy );
	virtual void transformToGlobal();
	virtual bool pointInPolygon( int x, int y ) const;

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
};

#endif // RDO_PROCESS_CHART_OBJECT_H

#ifndef RDO_PROCESS_CHART_OBJECT_H
#define RDO_PROCESS_CHART_OBJECT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_object.h"
#include "rdoprocess_matrix.h"
#include "rdoprocess_polygon.h"
#include "rdoprocess_rect.h"

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
	double rotation_alpha;

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
	double RDOPROCChartObject::getRotation() const { return rotation_alpha; }
	virtual void setRotation( double alpha );

	// Выделить/снять выделение с фигуры
	virtual void setSelected( bool value );
	// Отрисовка фигуры
	virtual void draw( CDC& dc ) = 0;

	// Габориты фигуры
	virtual rp::RPRect getBoundingRect( bool global = true ) const;

	// Центр в глобальных координатах
	CPoint getCenter() const {
		return globalMatrix() * getBoundingRect( false ).getCenter();
	}
	// Центр поворота в глобальных координатах
	CPoint getRotateCenter() {
		if ( !rotate_center_inited ) {
			rotate_center = getBoundingRect( false ).getCenter();
			rotate_center_inited = true;
		}
		return globalMatrix() * rotate_center;
	}

	// Перевод pa_src в pa_global для дальнейшей отрисовки
	void meshToGlobal();
	// Перевод всех элементов фигуры в глобальные координаты (включает выход meshToGlobal)
	virtual void transformToGlobal();
	// Находится ли точка внутри фигуры
	virtual bool pointInPolygon( int x, int y, bool byperimetr = true ) {
		meshToGlobal();
		if ( byperimetr ) {
			return pa_global.extendByPerimetr( main_pen_width * sqrt(2) / 2.0 ).pointInPolygon( x, y );
		} else {
			return pa_global.pointInPolygon( x, y );
		}
	}

	enum PossibleCommand { pcmd_none, pcmd_move, pcmd_rotate };
	PossibleCommand getPossibleCommand( int global_x, int global_y ) const;
};

#endif // RDO_PROCESS_CHART_OBJECT_H

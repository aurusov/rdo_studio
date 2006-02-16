#ifndef RDO_PROCESS_CHART_OBJECT_H
#define RDO_PROCESS_CHART_OBJECT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_object.h"
#include "rdoprocess_matrix.h"
#include "rdoprocess_polyline.h"
#include "rdoprocess_rect.h"

// ----------------------------------------------------------------------------
// ---------- RPChartObject
// ----------------------------------------------------------------------------
class RPChartObject: public RPObject
{
friend class RPFlowChart;

private:
	mutable CPoint rotate_center;
	mutable bool   rotate_center_inited;

protected:
	RPChartObject* chart_parent;
	RPFlowChart* flowchart;
	int  main_pen_width;
	CPen main_pen;

	rp::matrix matrix_transform;
	rp::matrix matrix_rotate;
	rp::matrix matrix_scale;
	double rotation_alpha;

	rp::matrix selfMatrix() const {
		return matrix_transform * matrix_rotate * matrix_scale;
	}
	rp::matrix globalMatrix() const {
		return chart_parent ? chart_parent->globalMatrix() * selfMatrix() : selfMatrix();
	}
	rp::matrix parentMatrix( bool self = true ) const {
		if ( self ) {
			return chart_parent ? chart_parent->parentMatrix( false ) : rp::matrix();
		} else {
			return chart_parent ? chart_parent->parentMatrix( false ) * selfMatrix() : selfMatrix();
		}
	}
	rp::matrix rotateCenterMatrix( bool self = true ) const {
		return parentMatrix() * matrix_transform;
	}

	virtual void moving( int dx, int dy );

public:
	RPChartObject( RPObject* parent, RPChartObject* chart_parent, RPFlowChart* flowchart, const rp::string& name = "object" );
	virtual ~RPChartObject();

	int getPenWidth() const { return main_pen_width; }

	// Позиция
	virtual void setPosition( double posx, double posy );
	double getX()               { return matrix_transform.dx();                }
	double getY()               { return matrix_transform.dy();                }
	void setX( double value )   { setPosition( value, matrix_transform.dy() ); }
	void setY( double value )   { setPosition( matrix_transform.dx(), value ); }

	// Масштаб
	virtual void setScale( double sx, double sy );
	double getScaleX()             { return matrix_scale.sx();             }
	double getScaleY()             { return matrix_scale.sy();             }
	void setScaleX( double value ) { setScale( value, matrix_scale.sy() ); }
	void setScaleY( double value ) { setScale( matrix_scale.sx(), value ); }

	// Поворот
	double RPChartObject::getRotation() const { return rotation_alpha; }
	virtual void setRotation( double alpha );
	// Центр поворота в глобальных координатах
	CPoint getRotateCenter() const {
		if ( !rotate_center_inited ) {
			rotate_center = getBoundingRect( false ).getCenter();
			rotate_center_inited = true;
		}
		return rotateCenterMatrix() * rotate_center;
	}
	void setRotateCenter( const CPoint& point ) { rotate_center = rotateCenterMatrix().obr() * point; }
	// Совпадает ли точка на центре вращения фигуры
	bool isRotateCenter( const CPoint& point ) const;

	// Выделить/снять выделение с фигуры
	virtual void setSelected( bool value );
	// Отрисовка фигуры
	virtual void draw( CDC& dc ) = 0;

	// Габориты фигуры
	virtual rp::rect getBoundingRect( bool global = true ) const = 0;

	// Центр в глобальных координатах
	CPoint getCenter() const {
		return globalMatrix() * getBoundingRect( false ).getCenter();
	}

	// Перевод всех элементов фигуры в глобальные координаты
	virtual void transformToGlobal() = 0;
	// Находится ли точка внутри фигуры
	virtual bool pointInPolygon( const CPoint& point, bool byperimetr = true ) = 0;

	enum PossibleCommand {
		pcmd_none = 0,      //!< Над объектом не может быть произведено никакое действие
		pcmd_move,          //!< Объект может быть перемещен
		pcmd_rotate,        //!< Объект можно повернуть
		pcmd_rotate_tl,     //!< Объект можно повернуть за левый верхний угол
		pcmd_rotate_tr,     //!< Объект можно повернуть за правый верхний угол
		pcmd_rotate_bl,     //!< Объект можно повернуть за левый нижний угол
		pcmd_rotate_br,     //!< Объект можно повернуть за левый нижний угол
		pcmd_rotate_center, //!< Может быть изменен центр врещения объекта
		pcmd_scale,         //!< Объект может быть масштабирован
		pcmd_scale_l,       //!< Объект может быть масштабирован за левую сторону
		pcmd_scale_r,       //!< Объект может быть масштабирован за правую сторону
		pcmd_scale_t,       //!< Объект может быть масштабирован за верх
		pcmd_scale_b,       //!< Объект может быть масштабирован за низ
		pcmd_scale_tl,      //!< Объект может быть масштабирован за левый верхний угол
		pcmd_scale_tr,      //!< Объект может быть масштабирован за правый верхний угол
		pcmd_scale_bl,      //!< Объект может быть масштабирован за левый нижний угол
		pcmd_scale_br       //!< Объект может быть масштабирован за правый нижний угол
	};
	virtual PossibleCommand getPossibleCommand( const CPoint& global_pos ) const { return pcmd_none; }
};

#endif // RDO_PROCESS_CHART_OBJECT_H

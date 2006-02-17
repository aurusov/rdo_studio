#ifndef RDO_PROCESS_CHART_OBJECT_H
#define RDO_PROCESS_CHART_OBJECT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_object.h"
#include "rdoprocess_matrix.h"
#include "rdoprocess_polyline.h"
#include "rdoprocess_rect.h"
#include "rdoprocess_point.h"
#include "rdoprocess_math.h"

// ----------------------------------------------------------------------------
// ---------- RPChartObject
// ----------------------------------------------------------------------------
class RPChartObject: public RPObject
{
friend class RPFlowChart;

private:
	mutable rp::point rotate_center;
	mutable bool      rotate_center_inited;

protected:
	RPChartObject* chart_parent;
	RPFlowChart* flowchart;
	int  main_pen_width;
	CPen main_pen;

	rp::matrix matrix_transform;
	rp::matrix matrix_rotate;
	rp::matrix matrix_transform_post;
	rp::matrix matrix_scale;
	double rotation_alpha;

	rp::matrix selfMatrix() const {
		return matrix_transform * matrix_rotate * matrix_transform_post * matrix_scale;
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
	rp::matrix rotateCenterMatrix() const {
		return parentMatrix() * matrix_transform;
	}
	static void fillRotateMatrix( rp::matrix& m_rotate, double alpha ) {
		alpha *= rp::math::pi / 180.0;
		double cos_a = cos( alpha );
		double sin_a = sin( alpha );
//		if ( fabs(cos_a) < 1e-10 ) cos_d = 0;
//		if ( fabs(sin_a) < 1e-10 ) sin_d = 0;
		m_rotate.data[0][0] = cos_a;
		m_rotate.data[1][1] = cos_a;
		m_rotate.data[0][1] = sin_a;
		m_rotate.data[1][0] = -sin_a;
	}

	virtual void moving( int dx, int dy );

public:
	RPChartObject( RPObject* parent, RPChartObject* chart_parent, RPFlowChart* flowchart, const rp::string& name = "object" );
	virtual ~RPChartObject();

	int getPenWidth() const { return main_pen_width; }

	// Позиция
	virtual void setPosition( double posx, double posy );
	double getX() const         { return matrix_transform.dx_const();          }
	double getY() const         { return matrix_transform.dy_const();          }
	void setX( double value )   { setPosition( value, matrix_transform.dy() ); }
	void setY( double value )   { setPosition( matrix_transform.dx(), value ); }

	// Позиция
	virtual void setPositionPost( double posx, double posy );
	double getPostX() const       { return matrix_transform_post.dx_const();              }
	double getPostY() const       { return matrix_transform_post.dy_const();              }
	void setPostX( double value ) { setPositionPost( value, matrix_transform_post.dy() ); }
	void setPostY( double value ) { setPositionPost( matrix_transform_post.dx(), value ); }

	// Масштаб
	virtual void setScale( double sx, double sy );
	double getScaleX() const       { return matrix_scale.sx_const();       }
	double getScaleY() const       { return matrix_scale.sy_const();       }
	void setScaleX( double value ) { setScale( value, matrix_scale.sy() ); }
	void setScaleY( double value ) { setScale( matrix_scale.sx(), value ); }

	// Поворот
	double RPChartObject::getRotation() const { return rotation_alpha; }
	virtual void setRotation( double alpha );
	// Вернуть центр поворота в глобальных координатах
	rp::point getRotateCenter() const {
		if ( !rotate_center_inited ) {
			rotate_center = getBoundingRect( false ).getCenter();
			rotate_center_inited = true;
		}
		return rotateCenterMatrix() * rotate_center;
	}
	// Установить центр поворота в глобальных координатах
	void setRotateCenter( const rp::point& point ) { rotate_center = rotateCenterMatrix().obr() * point; }
	// Изменить центр поворота в локальных координатах
	void setRotateCenterLocalDelta( double dx, double dy );
	// Совпадает ли точка на центре вращения фигуры
	bool isRotateCenter( const CPoint& point ) const;

	enum angle90 {
		angle90_0 = 0,  //!< Угол поворота объекта alpha > 270 + 45 || alpha <= 45
		angle90_90,     //!< Угол поворота объекта alpha > 45       && alpha <= 90 + 45
		angle90_180,    //!< Угол поворота объекта alpha > 90 + 45  && alpha <= 180 + 45
		angle90_270     //!< Угол поворота объекта alpha > 180 + 45 && alpha <= 270 + 45
	};
	// Вернуть дискретный угол поворота (дискрета 90 градусов)
	angle90 getAngle90() const {
		double alpha = getRotation();
		if ( alpha > 270 + 45 || alpha <= 45       ) return RPChartObject::angle90_0;
		if ( alpha > 45       && alpha <= 90 + 45  ) return RPChartObject::angle90_90;
		if ( alpha > 90 + 45  && alpha <= 180 + 45 ) return RPChartObject::angle90_180;
		if ( alpha > 180 + 45 && alpha <= 270 + 45 ) return RPChartObject::angle90_270;
		return RPChartObject::angle90_0;
	}

	enum angle45 {
		angle45_0 = 0,  //!< Угол поворота объекта alpha > 360 - 22 || alpha <= 22
		angle45_90,     //!< Угол поворота объекта alpha > 90 - 22  && alpha <= 90 + 22
		angle45_180,    //!< Угол поворота объекта alpha > 180 - 22 && alpha <= 180 + 22
		angle45_270,    //!< Угол поворота объекта alpha > 270 - 22 && alpha <= 270 + 22
		angle45_45,     //!< Угол поворота объекта alpha > 45 -  22 && alpha <= 45 + 22
		angle45_135,    //!< Угол поворота объекта alpha > 135 - 22 && alpha <= 135 + 22
		angle45_225,    //!< Угол поворота объекта alpha > 225 - 22 && alpha <= 225 + 22
		angle45_315,    //!< Угол поворота объекта alpha > 315 - 22 && alpha <= 315 + 22
	};
	// Вернуть дискретный угол поворота (дискрета 45 градусов)
	angle45 getAngle45() const {
		double alpha = getRotation();
		if ( alpha > 360 - 22 || alpha <= 22       ) return RPChartObject::angle45_0;
		if ( alpha > 90 - 22  && alpha <= 90 + 22  ) return RPChartObject::angle45_90;
		if ( alpha > 180 - 22 && alpha <= 180 + 22 ) return RPChartObject::angle45_180;
		if ( alpha > 270 - 22 && alpha <= 270 + 22 ) return RPChartObject::angle45_270;
		if ( alpha > 45 -  22 && alpha <= 45 + 22  ) return RPChartObject::angle45_45;
		if ( alpha > 135 - 22 && alpha <= 135 + 22 ) return RPChartObject::angle45_135;
		if ( alpha > 225 - 22 && alpha <= 225 + 22 ) return RPChartObject::angle45_225;
		if ( alpha > 315 - 22 && alpha <= 315 + 22 ) return RPChartObject::angle45_315;
		return RPChartObject::angle45_0;
	}

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
	virtual PossibleCommand getPossibleCommand( const CPoint& global_pos, bool for_cursor = false ) const { return pcmd_none; }
};

#endif // RDO_PROCESS_CHART_OBJECT_H

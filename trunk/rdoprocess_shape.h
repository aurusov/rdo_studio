#ifndef RDO_PROCESS_SHAPE_H
#define RDO_PROCESS_SHAPE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_object_matrix.h"

#include <vector>

// ----------------------------------------------------------------------------
// ---------- RPShape
// ----------------------------------------------------------------------------
class RPShape: public RPObjectMatrix
{
private:
	mutable rp::point rotate_center;
	mutable bool      rotate_center_inited;

protected:
//	CPoint snap_to_point;

	// Дискретный угол поворота (дискрета 90 градусов)
	enum angle90 {
		angle90_0 = 0,  //!< Угол поворота объекта alpha > 270 + 45 || alpha <= 45
		angle90_90,     //!< Угол поворота объекта alpha > 45       && alpha <= 90 + 45
		angle90_180,    //!< Угол поворота объекта alpha > 90 + 45  && alpha <= 180 + 45
		angle90_270     //!< Угол поворота объекта alpha > 180 + 45 && alpha <= 270 + 45
	};
	// Дискретный угол поворота (дискрета 45 градусов)
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
	// Возможная команда над объектом
	enum PossibleCommand {
		pcmd_none = 0,      //!< Над объектом не может быть произведено никакое действие
		pcmd_move,          //!< Объект может быть перемещен
		pcmd_rotate_tl,     //!< Объект можно повернуть за левый верхний угол
		pcmd_rotate_tr,     //!< Объект можно повернуть за правый верхний угол
		pcmd_rotate_bl,     //!< Объект можно повернуть за левый нижний угол
		pcmd_rotate_br,     //!< Объект можно повернуть за левый нижний угол
		pcmd_rotate_center, //!< Может быть изменен центр врещения объекта
		pcmd_scale_l,       //!< Объект может быть масштабирован за левую сторону
		pcmd_scale_r,       //!< Объект может быть масштабирован за правую сторону
		pcmd_scale_t,       //!< Объект может быть масштабирован за верх
		pcmd_scale_b,       //!< Объект может быть масштабирован за низ
		pcmd_scale_tl,      //!< Объект может быть масштабирован за левый верхний угол
		pcmd_scale_tr,      //!< Объект может быть масштабирован за правый верхний угол
		pcmd_scale_bl,      //!< Объект может быть масштабирован за левый нижний угол
		pcmd_scale_br       //!< Объект может быть масштабирован за правый нижний угол
	};
	PossibleCommand pcmd;

	// Возможная команда над объектом
	PossibleCommand getPossibleCommand( const rp::point& global_chart_pos, bool for_cursor = false );

	// Вернуть дискретный угол поворота (дискрета 90 градусов)
	angle90 getAngle90() const {
		double alpha = getRotationGlobal();
		if ( alpha > 270 + 45 || alpha <= 45       ) return RPShape::angle90_0;
		if ( alpha > 45       && alpha <= 90 + 45  ) return RPShape::angle90_90;
		if ( alpha > 90 + 45  && alpha <= 180 + 45 ) return RPShape::angle90_180;
		if ( alpha > 180 + 45 && alpha <= 270 + 45 ) return RPShape::angle90_270;
		return RPShape::angle90_0;
	}

	// Вернуть дискретный угол поворота (дискрета 45 градусов)
	angle45 getAngle45() const {
		double alpha = getRotationGlobal();
		if ( alpha > 360 - 22 || alpha <= 22       ) return RPShape::angle45_0;
		if ( alpha > 90 - 22  && alpha <= 90 + 22  ) return RPShape::angle45_90;
		if ( alpha > 180 - 22 && alpha <= 180 + 22 ) return RPShape::angle45_180;
		if ( alpha > 270 - 22 && alpha <= 270 + 22 ) return RPShape::angle45_270;
		if ( alpha > 45 -  22 && alpha <= 45 + 22  ) return RPShape::angle45_45;
		if ( alpha > 135 - 22 && alpha <= 135 + 22 ) return RPShape::angle45_135;
		if ( alpha > 225 - 22 && alpha <= 225 + 22 ) return RPShape::angle45_225;
		if ( alpha > 315 - 22 && alpha <= 315 + 22 ) return RPShape::angle45_315;
		return RPShape::angle45_0;
	}

	static void getScaleDelta( rp::point& delta, RPShape::angle90 a90, RPShape::PossibleCommand pcmd );
	static void getRectDelta( rp::rect& rect_old, rp::rect& rect_new, rp::point& delta, RPShape::angle90 a90, RPShape::PossibleCommand pcmd );

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

	void setPositionPostDelta( double posx, double posy );
	virtual void setPositionPost( double posx, double posy );

	virtual bool isShape() const { return true; }

public:
	RPShape( RPObject* parent, const rp::string& name = "object" );
	virtual ~RPShape();

	virtual void setPosition( int x, int y );

//	const CPoint& getSnapToPoint() const { return snap_to_point; }

	virtual void draw( CDC& dc );
	virtual void draw_selected( CDC& dc );

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

	// Перед выполнение команды (объект должен подготовить команду)
	virtual void command_before( const rp::point& global_chart_pos );
	// Выполнить команду над объектом
	virtual void command_make( const rp::point& global_chart_pos );
};

#endif // RDO_PROCESS_SHAPE_H

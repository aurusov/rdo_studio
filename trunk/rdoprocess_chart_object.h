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
#include "rdoprocess_project.h"

// ----------------------------------------------------------------------------
// ---------- RPChartObject
// ----------------------------------------------------------------------------
class RPChartObject: public RPObject
{
friend class RPFlowChartObject;

private:
	mutable rp::point rotate_center;
	mutable bool      rotate_center_inited;
/*
	class Backup {
	public:
		rp::matrix matrix_transform;
		rp::matrix matrix_rotate;
		rp::matrix matrix_transform_post;
		rp::matrix matrix_scale;
		int        rotation_alpha;
	};
*/
protected:
	RPChartObject*     chartParent() const { return !isFlowChart() && parent && parent->isChartObject() ? static_cast<RPChartObject*>(parent) : NULL; }
	RPFlowChartObject* flowChart() const;
	virtual bool isFlowChart() const { return false; }
	int  main_pen_width;
	CPen main_pen;

	void getChartObjects( std::list< RPChartObject* >& objects ) const {
		std::list< RPObject* >::const_iterator it = begin();
		while ( it != end() ) {
			if ( (*it)->isChartObject() ) objects.push_back( static_cast<RPChartObject*>(*it) );
			it++;
		}
	}

	virtual void modify();
	virtual void update();

	virtual RPProject::Cursor getCursor( const rp::point& global_chart_pos );
	virtual RPChartObject* find( const rp::point& global_chart_pos );

	rp::matrix matrix_transform;
	rp::matrix matrix_rotate;
	rp::matrix matrix_transform_post;
	rp::matrix matrix_scale;
	double rotation_alpha;

//	std::list< Backup > backup;

	rp::matrix selfMatrix() const {
		return matrix_transform * matrix_rotate * matrix_transform_post * matrix_scale;
	}
	rp::matrix selfMatrix_noScale() const {
		return matrix_transform * matrix_rotate * matrix_transform_post;
	}
	rp::matrix globalMatrix( bool first = true ) const {
		if ( first ) {
			return chartParent() ? chartParent()->globalMatrix( false ) * selfMatrix() : selfMatrix();
		} else {
			return chartParent() ? chartParent()->globalMatrix( false ) * selfMatrix_noScale() : selfMatrix_noScale();
		}
	}
	rp::matrix flowchartMatrix() const {
		// занимаемся приведением типов только потому, что
		// в этом хедере не определен класс RPFlowChartObject,
		// а инклюдить "rdoprocess_flowchart.h" не хочется
		RPChartObject* flowchart = reinterpret_cast<RPChartObject*>(flowChart());
		return flowchart ? flowchart->selfMatrix() : rp::matrix();
	}
	rp::matrix parentMatrix_noScale( bool first = true ) const {
		if ( first ) {
			return chartParent() ? chartParent()->parentMatrix_noScale( false ) : rp::matrix();
		} else {
			return chartParent() ? chartParent()->parentMatrix_noScale( false ) * selfMatrix_noScale() : selfMatrix_noScale();
		}
	}
	rp::matrix rotateCenterMatrix() const {
		return parentMatrix_noScale() * matrix_transform;
	}
	rp::matrix globalRotate() const {
		rp::matrix m_rotate;
		RPChartObject::fillRotateMatrix( m_rotate, rotation_alpha );
		return chartParent() ? chartParent()->globalRotate() * m_rotate : m_rotate;
	}
	rp::matrix parentRotate( bool first = true ) const {
		if ( first ) {
			return chartParent() ? chartParent()->parentRotate( false ) : rp::matrix();
		} else {
			rp::matrix m_rotate;
			RPChartObject::fillRotateMatrix( m_rotate, rotation_alpha );
			return chartParent() ? chartParent()->parentRotate( false ) * m_rotate : m_rotate;
		}
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

	virtual void onLButtonDown( UINT nFlags, CPoint global_chart_pos ) {
		if ( !isSelected() ) setSelected( true );
	};
	virtual void onLButtonUp( UINT nFlags, CPoint global_chart_pos ) {};
	virtual void onLButtonDblClk( UINT nFlags, CPoint global_chart_pos ) {};
	virtual void onRButtonDown( UINT nFlags, CPoint global_chart_pos ) {};
	virtual void onRButtonUp( UINT nFlags, CPoint global_chart_pos ) {};
	virtual void onMouseMove( UINT nFlags, CPoint global_chart_pos ) {};

public:
	RPChartObject( RPObject* parent, const rp::string& name = "object" );
	virtual ~RPChartObject();

	virtual bool isChartObject() const { return true; }

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
	void setRotateCenter( const rp::point& global_chart_pos ) { rotate_center = rotateCenterMatrix().obr() * global_chart_pos; }
	// Изменить центр поворота в локальных координатах
	void setRotateCenterLocalDelta( double dx, double dy );
	// Совпадает ли точка на центре вращения фигуры
	bool isRotateCenter( const rp::point& global_chart_pos ) const;

/*
	// Стек для бекапа матриц
	void backup_push() {
		Backup bkp;
		bkp.matrix_transform      = matrix_transform;
		bkp.matrix_rotate         = matrix_rotate;
		bkp.matrix_transform_post = matrix_transform_post;
		bkp.matrix_scale          = matrix_scale;
		bkp.rotation_alpha        = rotation_alpha;
		backup.push_back( bkp );
	}
	void backup_pop() {
		Backup bkp = backup.back();
		backup.pop_back();
		matrix_transform      = bkp.matrix_transform;
		matrix_rotate         = bkp.matrix_rotate;
		matrix_transform_post = bkp.matrix_transform_post;
		matrix_scale          = bkp.matrix_scale;
		rotation_alpha        = bkp.rotation_alpha;
	}
	void backup_clear() { backup.clear(); }
*/
	// Выделить/снять выделение с фигуры
	virtual void setSelected( bool value );
	// Отрисовка фигуры
	virtual void draw( CDC& dc );
	virtual void draw_after( CDC& dc );
	virtual void draw_selected( CDC& dc ) {};
	virtual void draw1( CDC& dc ) {};

	// Габориты фигуры
	virtual rp::rect getBoundingRect( bool global = true ) const = 0;
	virtual rp::rect getMaxRect()  { return getBoundingRect(); }

	// Центр в глобальных координатах
	rp::point getCenter() const {
		return globalMatrix() * getBoundingRect( false ).getCenter();
	}

	// Перевод всех элементов фигуры в глобальные координаты
	virtual void transformToGlobal() = 0;

	// Находится ли точка внутри фигуры
	virtual bool pointInPolygon( const rp::point& global_chart_pos ) = 0;
	// Находится ли точка в служебной (неклиентской) части фигуры (прямоугольник выделения, к примеру)
	virtual bool pointInNCArea( const rp::point& global_chart_pos ) = 0;

	// Перед выполнение команды (объект должен подготовить команду)
	virtual void command_before( const rp::point& global_chart_pos ) {};
	// Выполнить команду над объектом
	virtual void command_make( const rp::point& global_chart_pos ) {};
};

#endif // RDO_PROCESS_CHART_OBJECT_H

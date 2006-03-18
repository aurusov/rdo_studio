#ifndef RDO_PROCESS_OBJECT_MATRIX_H
#define RDO_PROCESS_OBJECT_MATRIX_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_object_chart.h"
#include "rdoprocess_math.h"

#include <vector>

// ----------------------------------------------------------------------------
// ---------- RPObjectMatrix
// ----------------------------------------------------------------------------
class RPObjectMatrix: public RPObjectChart
{
private:
	mutable rp::point rotate_center;
	mutable bool      rotate_center_inited;

protected:
	virtual bool isMatrix() const { return true; }
	RPObjectMatrix* matrixParent() const {
		RPObjectChart* chart_parent = chartParent();
		return chart_parent && chart_parent->isMatrix() ? static_cast<RPObjectMatrix*>(chart_parent) : NULL;
	}

	rp::matrix matrix_transform;
	rp::matrix matrix_rotate;
	rp::matrix matrix_transform_post;
	rp::matrix matrix_scale;
	double rotation_alpha;

	rp::matrix selfMatrix() const {
		return matrix_transform * matrix_rotate * matrix_transform_post * matrix_scale;
	}
	rp::matrix selfMatrix_noScale() const {
		return matrix_transform * matrix_rotate * matrix_transform_post;
	}
	rp::matrix globalMatrix( bool first = true ) const {
		RPObjectMatrix* matrix_parent = matrixParent();
		if ( first ) {
			return matrix_parent ? matrix_parent->globalMatrix( false ) * selfMatrix() : selfMatrix();
		} else {
			return matrix_parent ? matrix_parent->globalMatrix( false ) * selfMatrix_noScale() : selfMatrix_noScale();
		}
	}
	rp::matrix flowchartMatrix() const {
		// занимаемся приведением типов только потому, что
		// в этом хедере не определен класс RPFlowChartObject,
		// а инклюдить "rdoprocess_object_flowchart.h" не хочется
		RPObjectMatrix* flowchart = reinterpret_cast<RPObjectMatrix*>(flowChart());
		return flowchart ? flowchart->selfMatrix() : rp::matrix();
	}
	rp::matrix parentMatrix_noScale( bool first = true ) const {
		RPObjectMatrix* matrix_parent = matrixParent();
		if ( first ) {
			return matrix_parent ? matrix_parent->parentMatrix_noScale( false ) : rp::matrix();
		} else {
			return matrix_parent ? matrix_parent->parentMatrix_noScale( false ) * selfMatrix_noScale() : selfMatrix_noScale();
		}
	}
	rp::matrix rotateCenterMatrix() const {
		return parentMatrix_noScale() * matrix_transform;
	}
	rp::matrix globalRotate() const {
		rp::matrix m_rotate;
		RPObjectMatrix::fillRotateMatrix( m_rotate, rotation_alpha );
		RPObjectMatrix* matrix_parent = matrixParent();
		return matrix_parent ? matrix_parent->globalRotate() * m_rotate : m_rotate;
	}
	rp::matrix parentRotate( bool first = true ) const {
		RPObjectMatrix* matrix_parent = matrixParent();
		if ( first ) {
			return matrix_parent ? matrix_parent->parentRotate( false ) : rp::matrix();
		} else {
			rp::matrix m_rotate;
			RPObjectMatrix::fillRotateMatrix( m_rotate, rotation_alpha );
			return matrix_parent ? matrix_parent->parentRotate( false ) * m_rotate : m_rotate;
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
	double getRotation() const { return rotation_alpha; }
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

	// Центр в глобальных координатах
	rp::point getCenter() const { return globalMatrix() * getBoundingRect( false ).getCenter(); }

public:
	RPObjectMatrix( RPObject* parent, const rp::string& name = "object" );
	virtual ~RPObjectMatrix();
};

#endif // RDO_PROCESS_OBJECT_MATRIX_H

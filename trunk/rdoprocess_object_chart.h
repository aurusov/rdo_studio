#ifndef RDO_PROCESS_OBJECT_CHART_H
#define RDO_PROCESS_OBJECT_CHART_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_object.h"
#include "rdoprocess_rect.h"
#include "rdoprocess_point.h"
#include "rdoprocess_project.h"

// ----------------------------------------------------------------------------
// ---------- RPObjectChart
// ----------------------------------------------------------------------------
class RPObjectFlowChart;

class RPObjectChart: public RPObject
{
protected:
	RPObjectChart*     chartParent() const { return !isFlowChart() && parent && parent->isChartObject() ? static_cast<RPObjectChart*>(parent) : NULL; }
	RPObjectFlowChart* flowChart() const;

	int  main_pen_width;
	CPen main_pen;

	void getChartObjects( std::list< RPObjectChart* >& objects ) const {
		std::list< RPObject* >::const_iterator it = begin();
		while ( it != end() ) {
			if ( (*it)->isChartObject() ) objects.push_back( static_cast<RPObjectChart*>(*it) );
			it++;
		}
	}

	virtual void modify();
	virtual void update();

	virtual RPProject::Cursor getCursor( const rp::point& global_chart_pos );
	virtual RPObjectChart* find( const rp::point& global_chart_pos );

	int getPenWidth() const { return main_pen_width; }

	// Перевод всех элементов фигуры в глобальные координаты
	virtual void transformToGlobal() = 0;

	// Находится ли точка внутри фигуры
	virtual bool pointInPolygon( const rp::point& global_chart_pos ) = 0;
	// Находится ли точка в служебной (неклиентской) части фигуры (прямоугольник выделения, к примеру)
	virtual bool pointInNCArea( const rp::point& global_chart_pos ) = 0;

	// Выделить/снять выделение с фигуры
	virtual void setSelected( bool value );

	// Отрисовка фигуры
	virtual void draw_selected( CDC& dc ) {};

public:
	RPObjectChart( RPObject* parent, const rp::string& name = "object" );
	virtual ~RPObjectChart();

	virtual bool isChartObject() const { return true;  }
	virtual bool isFlowChart() const   { return false; }
	virtual bool isMatrix() const      { return false; }

	virtual void onLButtonDown( UINT nFlags, CPoint global_chart_pos ) {
		if ( !isSelected() ) setSelected( true );
	};
	virtual void onLButtonUp( UINT nFlags, CPoint global_chart_pos ) {};
	virtual void onLButtonDblClk( UINT nFlags, CPoint global_chart_pos ) {};
	virtual void onRButtonDown( UINT nFlags, CPoint global_chart_pos ) {};
	virtual void onRButtonUp( UINT nFlags, CPoint global_chart_pos ) {};
	virtual void onMouseMove( UINT nFlags, CPoint global_chart_pos ) {};

	// Отрисовка фигуры
	virtual void draw( CDC& dc );
	virtual void draw_after( CDC& dc );
	virtual void draw1( CDC& dc ) {};

	// Габориты фигуры
	virtual rp::rect getBoundingRect( bool global = true ) const = 0;
	virtual rp::rect getMaxRect()  { return getBoundingRect(); }

	// Перед выполнение команды (объект должен подготовить команду)
	virtual void command_before( const rp::point& global_chart_pos ) {};
	// Выполнить команду над объектом
	virtual void command_make( const rp::point& global_chart_pos ) {};

/*
	class Backup {
	public:
		rp::matrix matrix_transform;
		rp::matrix matrix_rotate;
		rp::matrix matrix_transform_post;
		rp::matrix matrix_scale;
		int        rotation_alpha;
	};
	std::list< Backup > backup;

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
};

#endif // RDO_PROCESS_OBJECT_CHART_H

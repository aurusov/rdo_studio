#ifndef RDO_PROCESS_OBJECT_CHART_H
#define RDO_PROCESS_OBJECT_CHART_H

#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_project.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_rect.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_point.h"

// --------------------------------------------------------------------------------
// -------------------- RPObjectChart
// --------------------------------------------------------------------------------
class RPObjectFlowChart;

class RPObjectChart: public RPObject
{
private:
	CPen main_pen;

protected:
	rbool can_update;
	RPObjectChart* chartParent() const { return !isFlowChart() && parent && parent->isChartObject() ? static_cast<RPObjectChart*>(parent) : NULL; }

	int  main_pen_width;
	CPen main_pen_default;

	virtual void modify();

	virtual RPObjectChart* find( const rp::point& global_chart_pos );

	int getPenWidth() const { return main_pen_width; }

	// Перевод всех элементов фигуры в глобальные координаты
	virtual void transformToGlobal() = 0;

	// Находится ли точка внутри фигуры
	virtual rbool pointInPolygon( const rp::point& global_chart_pos ) = 0;
	// Находится ли точка в служебной (неклиентской) части фигуры (прямоугольник выделения, к примеру)
	virtual rbool pointInNCArea( const rp::point& global_chart_pos ) = 0;

	// Отрисовка фигуры
	virtual void draw_selected( CDC& dc )
	{
		UNUSED(dc);
	}

public:
	RPObjectChart( RPObject* parent, const rp::string& name = "chart" );
	virtual ~RPObjectChart();

	virtual rp::string getClassName() const { return "RPObjectChart"; }
	virtual void load( rp::RPXMLNode* node );
	virtual rp::RPXMLNode* save( rp::RPXMLNode* parent_node );

	RPObjectFlowChart* flowChart() const;

	virtual rbool setName( const rp::string& value ) {
		rbool res = RPObject::setName( value );
		if ( res ) update();
		return res;
	}

	const CPen& getDefaultPen() const { return main_pen_default; }
	const CPen& getPen() const        { return main_pen;         }
	void setPen( const CPen& pen );

	virtual void update();

	virtual rbool isChartObject() const { return true;  }
	virtual rbool isFlowChart() const   { return false; }
	virtual rbool isMatrix() const      { return false; }
	virtual rbool isShape() const       { return false; }
	virtual rbool isConnector() const   { return false; }

	void getChartObjects( std::list< RPObjectChart* >& objects ) const {
		std::list< RPObject* >::const_iterator it = begin();
		while ( it != end() ) {
			if ( (*it)->isChartObject() ) objects.push_back( static_cast<RPObjectChart*>(*it) );
			it++;
		}
	}

	virtual void onLButtonDown( UINT nFlags, CPoint global_chart_pos )
	{
		UNUSED(nFlags          );
		UNUSED(global_chart_pos);

		if (!isSelected())
		{
			setSelected(true);
		}
	};
	virtual void onLButtonUp( UINT nFlags, CPoint global_chart_pos )
	{
		UNUSED(nFlags          );
		UNUSED(global_chart_pos);
	}
	virtual void onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
	{
		UNUSED(nFlags          );
		UNUSED(global_chart_pos);
	}
	virtual void onRButtonDown( UINT nFlags, CPoint global_chart_pos )
	{
		UNUSED(nFlags          );
		UNUSED(global_chart_pos);
	}
	virtual void onRButtonUp( UINT nFlags, CPoint global_chart_pos )
	{
		UNUSED(nFlags          );
		UNUSED(global_chart_pos);
	}
	virtual void onMouseMove( UINT nFlags, CPoint global_chart_pos )
	{
		UNUSED(nFlags          );
		UNUSED(global_chart_pos);
	}

	// Выделить/снять выделение с фигуры
	virtual void setSelected( rbool value );

	// Находится ли точка внутри фигуры
	rbool pointInShape( const rp::point& global_chart_pos ) {
		return pointInPolygon( global_chart_pos ) || pointInNCArea( global_chart_pos );
	}

	virtual RPProject::Cursor getCursor( const rp::point& global_chart_pos );

	// Отрисовка фигуры
	virtual void draw       ( CDC& dc );
	virtual void draw_after ( CDC& dc );
	virtual void drawCustom ( REF(CDC) dc );

	// Габориты фигуры
	virtual rp::rect getBoundingRect( rbool global = true ) const = 0;
	virtual rp::rect getMaxRect()                   { return getBoundingRect(); }
	virtual rp::rect getBoundingRectNoRotateOuter() { return getBoundingRect(); }
	virtual rp::rect getBoundingRectNoRotateIner()  { return getBoundingRect(); }

	// Перед выполнение команды (объект должен подготовить команду)
	virtual void command_before( const rp::point& global_chart_pos, rbool first_click )
	{
		UNUSED(global_chart_pos);
		UNUSED(first_click     );
	}
	// Выполнить команду над объектом
	virtual void command_make( const rp::point& global_chart_pos )
	{
		UNUSED(global_chart_pos);
	}

	virtual rbool can_delete() const { return true; }

	static rp::string colorToStr( COLORREF color );
	static COLORREF strToColor( const rp::string& color );
};

#endif // RDO_PROCESS_OBJECT_CHART_H

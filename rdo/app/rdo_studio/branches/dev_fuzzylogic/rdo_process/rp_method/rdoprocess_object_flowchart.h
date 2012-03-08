#ifndef RDO_PROCESS_OBJECT_FLOWCHART_H
#define RDO_PROCESS_OBJECT_FLOWCHART_H

#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object_matrix.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_connector.h"

// --------------------------------------------------------------------------------
// -------------------- RPObjectFlowChart
// --------------------------------------------------------------------------------
class RPObjectPixmap;

class RPObjectFlowChart: public RPObjectMatrix
{
friend class RPFlowChart;

private:
	rbool init_ok;
	int border_w;
	int border_h;
	int paper_border_w;
	int paper_border_h;
	int paper_border;
	int paper_shadow;
	int pixmap_w_real;
	int pixmap_h_real;
	int pixmap_w_show;
	int pixmap_h_show;
	int client_width;
	int client_height;
	static const int select_box_size2;

	CPen     pen_black;
	CPen     pen_shape_color;
	CPen     pen_selected_line;
	CPen     pen_selected_box;
	CBrush   brush_selected_box;
	COLORREF paper_border_color;
	COLORREF paper_shadow_color;
	COLORREF paper_bg_color;

	CDC      mem_dc;
	CBitmap* mem_bmp;
	CBitmap* bmp_first;
	CFont*   font_first;
	int      saved_mem_dc;

	CPoint           global_win_pos_current;
	CPoint           global_win_pos_prev;
	RPObjectChart*   one_object;
	RPObjectChart*   one_selected;
	RPConnector*     one_connector;
	RPConnectorDock* dock_begin;

#ifdef TEST_SPEED
	int makepixmap_cnt;
#endif

public:
	enum ConnectorTypeWanted { ctw_non, ctw_begin, ctw_end };

protected:
	ConnectorTypeWanted ct_wanted;

	RPFlowChart* flowchart;
	void after_constructor();

	virtual void  notify( RPObject* from, UINT message, void* param = NULL );
	virtual void  modify();
	virtual void  update();
	virtual rbool isFlowChart() const { return true; }
	void makeNewPixmap();
	void init();

	virtual RPProject::Cursor getCursor( const rp::point& global_chart_pos );
	virtual RPObjectChart* find( const rp::point& global_chart_pos );

	rp::rect getFlowSize( const std::list< RPObjectChart* >& list ) const;

	virtual void onLButtonDown( UINT nFlags, CPoint local_win_pos );
	virtual void onLButtonUp( UINT nFlags, CPoint local_win_pos );
	virtual void onLButtonDblClk( UINT nFlags, CPoint local_win_pos );
	virtual void onRButtonDown( UINT nFlags, CPoint local_win_pos );
	virtual void onRButtonUp( UINT nFlags, CPoint local_win_pos );
	virtual void onMouseMove( UINT nFlags, CPoint local_win_pos );

	RPShape*     drag_and_drop_shape;
	RPConnector* drag_and_drop_connector;
	void findAutoConnector();

	rbool           trash_show;
	rbool           trash_over;
	CRect           trash_rect;
	RPObjectPixmap* trash_bmp;
	rbool           is_trash_over( const rp::point& global_chart_pos ) const { return trash_rect.PtInRect( CPoint(static_cast<int>(global_chart_pos.x), static_cast<int>(global_chart_pos.y)) ) ? true : false; }

public:
	RPObjectFlowChart( RPObject* parent );
	virtual ~RPObjectFlowChart();

	virtual rp::string getClassName() const { return "RPObjectFlowChart"; }
	virtual void load( rp::RPXMLNode* node );
	virtual rp::RPXMLNode* save( rp::RPXMLNode* parent_node );
	virtual rpMethod::RPMethod* getMethod() = 0;
	virtual rbool setName( const rp::string& value );

	virtual void onDragEnter( const RPObjectClassInfo* classInfo, const rp::point& point );
	virtual void onDragOver( const rp::point& point );
	virtual void onDragLeave();
	virtual void onDrop( const rp::point& point );

	virtual void setActive( rbool value );

	void snapToGrid( RPObjectMatrix* shape );

	ConnectorTypeWanted getConnectorTypeWanted() const { return ct_wanted;     }
	void insert_connector( RPConnectorDock* dock );
	RPConnector*     getOneConnector() const           { return one_connector; }
	RPConnectorDock* getDockBegin() const              { return dock_begin;    }

	static int getSensitivity()               { return select_box_size2 + 1; }
	int getSelectBoxSize2() const             { return select_box_size2;     }
	const CPen& getPenSelectedLine() const    { return pen_selected_line;    }
	const CPen& getPenSelectedBox() const     { return pen_selected_box;     }
	const CBrush& getBrushSelectedBox() const { return brush_selected_box;   }

	void setBgBrush( const CBrush& brush );

	rp::point mouse_current() const;
	rp::point mouse_prev() const;
	rp::point mouse_delta() const             { return rp::point( global_win_pos_current.x - global_win_pos_prev.x, global_win_pos_current.y - global_win_pos_prev.y ); }

	// Отрисовка фигуры
	virtual void draw( CDC& dc );
	// Габориты фигуры
	virtual rp::rect getBoundingRect( rbool global = true ) const;
	virtual rp::rect getMaxRect();
	// Перевод всех элементов фигуры в глобальные координаты
	virtual void transformToGlobal() {};
	// Находится ли точка внутри фигуры
	virtual rbool pointInPolygon( const rp::point& global_chart_pos );
	// Находится ли точка в служебной (неклиентской) части фигуры (прямоугольник выделения, к примеру)
	virtual rbool pointInNCArea( const rp::point& global_chart_pos ) { return !pointInPolygon( global_chart_pos ); }

	void clientToZero( CPoint& point ) const {
		point.x -= border_w + paper_border_w;
		point.y -= border_h + paper_border_h;
	}
	void clientToZero( rp::point& point ) const {
		point.x -= border_w + paper_border_w;
		point.y -= border_h + paper_border_h;
	}
};

#endif // RDO_PROCESS_OBJECT_FLOWCHART_H

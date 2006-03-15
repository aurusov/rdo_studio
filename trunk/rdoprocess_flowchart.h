#ifndef RDO_PROCESS_FLOWCHART_H
#define RDO_PROCESS_FLOWCHART_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_chart_object.h"

#define TEST_SPEED TRUE
#undef TEST_SPEED

// ----------------------------------------------------------------------------
// ---------- RPFlowChartObject
// ----------------------------------------------------------------------------
class RPShape;

class RPFlowChartObject: public RPChartObject
{
friend class RPFlowChart;

private:
	bool init_ok;
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

	CPoint                         global_win_pos_prev;
	std::list< RPChartObject* >    moving_objects;
	RPChartObject*                 one_object;
	RPChartObject::PossibleCommand one_object_pcmd;

#ifdef TEST_SPEED
	int makepixmap_cnt;
#endif

protected:
	RPFlowChart* flowchart;

	virtual void notify( RPObject* from, UINT message, WPARAM wParam, LPARAM lParam );
	virtual void modify();
	virtual void update();
	virtual bool isFlowChart() const { return true; }
	void makeNewPixmap();

	RPChartObject* find( const CPoint& local_win_pos ) {
		rp::point global_chart_pos = local_win_pos;
		clientToZero( global_chart_pos );
		std::list< RPObject* >::const_iterator it = begin();
		while ( it != end() ) {
			if ( (*it)->isChartObject() ) {
				RPChartObject* obj = static_cast<RPChartObject*>(*it);
				if ( obj->pointInPolygon( global_chart_pos ) ) {
					return obj;
				}
			}
			it++;
		}
		return NULL;
	}

	virtual RPProject::Cursor getCursor( const rp::point& global_pos );

	rp::rect getFlowSize( const std::list< RPChartObject* >& list ) const;
	rp::rect getFlowSize() const {
		std::list< RPChartObject* > objects;
		getChartObjects( objects );
		return getFlowSize( objects );
	}

	virtual void onLButtonDown( UINT nFlags, CPoint local_win_pos );
	virtual void onLButtonUp( UINT nFlags, CPoint local_win_pos );
	virtual void onLButtonDblClk( UINT nFlags, CPoint local_win_pos );
	virtual void onRButtonDown( UINT nFlags, CPoint local_win_pos );
	virtual void onRButtonUp( UINT nFlags, CPoint local_win_pos );
	virtual void onMouseMove( UINT nFlags, CPoint local_win_pos );

public:
	RPFlowChartObject( RPObject* parent, RPFlowChart* flowchart );
	virtual ~RPFlowChartObject();

	void snapToGrid( RPShape* shape );

	static int getSensitivity()               { return select_box_size2 + 1; }
	int getSelectBoxSize2() const             { return select_box_size2;     }
	const CPen& getPenSelectedLine() const    { return pen_selected_line;    }
	const CPen& getPenSelectedBox() const     { return pen_selected_box;     }
	const CBrush& getBrushSelectedBox() const { return brush_selected_box;   }

	virtual void moveTo( int x, int y ) {};
	virtual void draw( CDC& dc );
	virtual rp::rect getBoundingRect( bool global = true ) const { return rp::rect(); }
	virtual void transformToGlobal() {};
	virtual bool pointInPolygon( const rp::point& point, bool byperimetr = true ) { return true; }

	void clientToZero( CPoint& point ) const {
		point.x -= border_w + paper_border_w;
		point.y -= border_h + paper_border_h;
	}
	void clientToZero( rp::point& point ) const {
		point.x -= border_w + paper_border_w;
		point.y -= border_h + paper_border_h;
	}
};

// ----------------------------------------------------------------------------
// ---------- RPFlowChart
// ----------------------------------------------------------------------------
class RPFlowChart: public CWnd
{
friend class RPFlowChartObject;

private:
	enum GridMode { gtSnapOff, gtSnapToPoint, gtSnapToCenter };
	enum GridType { gtPoints, gtSolidLines, dtDotLines };

	int   saved_dc;
	CRect scroll_bar_size;

	void updateScrollBars();

/*
	GridMode grid_mode;
	GridType grid_type;
	bool     grid_show;
	int      grid_step;
//	bool     grid_wasMouseMoving;
	UINT     grid_timer;
	int      grid_cnt_x;
	int      grid_cnt_y;
	int      grid_delta;
	COLORREF grid_color;
	CDC      grid_dc;
	CBitmap  grid_bmp;
	int      grid_bmp_width;
	std::vector< CPoint >      grid_pa;
	std::list< RPShape* > grid_shapes;
*/
	void makeGrid();

//	bool showShapeName;
//	bool showConnectorPoint;

	RPFlowChartObject* flowobj;

#ifdef TEST_SPEED
	int sec_cnt;
	int sec_timer;
	int makegrid_cnt;
	int makegridempty_cnt;
#endif

public:
	RPFlowChart();
	virtual ~RPFlowChart();

	//{{AFX_VIRTUAL(RPFlowChart)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RPFlowChart)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDO_PROCESS_FLOWCHART_H

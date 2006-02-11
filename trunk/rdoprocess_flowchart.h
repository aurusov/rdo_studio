#ifndef RDO_PROCESS_FLOWCHART_H
#define RDO_PROCESS_FLOWCHART_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_chart_object.h"

#define TEST_SPEED TRUE
#undef TEST_SPEED

// ----------------------------------------------------------------------------
// ---------- RDOPROCFlowChartObject
// ----------------------------------------------------------------------------
class RDOPROCFlowChartObject: public RDOPROCChartObject
{
friend class RDOPROCFlowChart;

public:
	RDOPROCFlowChartObject( RDOPROCObject* parent, RDOPROCChartObject* chart_parent, RDOPROCFlowChart* flowchart );

	virtual void setSelected( bool value ) {};
	virtual void moveTo( int x, int y ) {};
	virtual void draw( CDC& dc ) {};
	virtual void notify( RDOPROCObject* from, UINT message, WPARAM wParam, LPARAM lParam );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCFlowChart
// ----------------------------------------------------------------------------
class RDOPROCShape;

class RDOPROCFlowChart: public CWnd
{
friend class RDOPROCChartObject;
friend class RDOPROCFlowChartObject;
friend class RDOPROCShape;

private:
	void insertShape( RDOPROCShape* shape );
	void deleteShape( RDOPROCShape* shape );

	enum GridMode { gtSnapOff, gtSnapToPoint, gtSnapToCenter };
	enum GridType { gtPoints, gtSolidLines, dtDotLines };

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

	CDC      mem_dc;
	CBitmap* mem_bmp;

	int      saved_dc;
	int      saved_mem_dc;
	CFont*   font_first;
	CBitmap* bmp_first;

	CPen     pen_black;
	CPen     pen_shape_color;
	CPen     pen_selected_line;
	CPen     pen_selected_box;
	CBrush   brush_selected_box;
	COLORREF paper_border_color;
	COLORREF paper_shadow_color;
	COLORREF paper_bg_color;

	void makeNewPixmap();
	CSize getFlowSize( const std::list< RDOPROCShape* >& list ) const;
	CSize getFlowSize() const { return getFlowSize( shapes ); }
	void updateScrollBars();
	void updateDC();
	virtual void modify();
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
	std::list< RDOPROCShape* > grid_shapes;
*/
	void makeGrid();

//	bool showShapeName;
//	bool showConnectorPoint;

	RDOPROCFlowChartObject* chobj;
	std::list< RDOPROCShape* > shapes;

	std::list< RDOPROCShape* >::iterator find( const RDOPROCShape* shape );

	RDOPROCShape* findObject( const int x, const int y ) const;

	void moving_start( RDOPROCShape* shape, const int global_mouse_x, const int global_mouse_y );
	void moving( const int global_mouse_x, const int global_mouse_y );
	void moving_stop();

	int                        global_old_x;
	int                        global_old_y;
	std::list< RDOPROCShape* > movingShapes;

#ifdef TEST_SPEED
	int sec_cnt;
	int sec_timer;
	int makepixmap_cnt;
	int makegrid_cnt;
	int makegridempty_cnt;
#endif

	void updateFlowState();

public:
	RDOPROCFlowChart();
	virtual ~RDOPROCFlowChart();

	void snapToGrid( RDOPROCShape* shape );
	
	void selectShapeOff();
	void selectShape( RDOPROCShape* shape, const bool value = true );

	//{{AFX_VIRTUAL(RDOPROCFlowChart)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOPROCFlowChart)
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDO_PROCESS_FLOWCHART_H

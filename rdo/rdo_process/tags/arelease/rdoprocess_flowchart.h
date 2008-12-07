#ifndef RDO_PROCESS_FLOWCHART_H
#define RDO_PROCESS_FLOWCHART_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <vector>

// ----------------------------------------------------------------------------
// ---------- RDOPROCFlowChart
// ----------------------------------------------------------------------------
class RDOPROCObject;

class RDOPROCFlowChart: public CWnd
{
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
	int pixmap_w_real;
	int pixmap_h_real;
	int pixmap_w_show;
	int pixmap_h_show;
	int client_width;
	int client_height;
	int scroll_x_pos;
	int scroll_y_pos;
	int shape_pen_width;

	CDC      mem_dc;
	CBitmap* mem_bmp;

	int      saved_dc;
	int      saved_mem_dc;
	CFont*   font_first;
	CBitmap* bmp_first;

	CPen     pen_border;
	CPen     pen_shadow;
	CPen     pen_black;
	CPen     pen_shape_default;
	CPen     pen_shape_color;
	CBrush   brush_border;
	CBrush   brush_shadow;
	CBrush   brush_select_box;
	CPoint   border_points[5];
	COLORREF paper_bg_color;

	void makeNewPixmap();
//	void updateScrollBars();
	void updateDC();
	virtual void modify();

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
	void makeGrid();

//	bool showShapeName;
//	bool showConnectorPoint;

	RDOPROCObject* rpobj;
	std::vector< RDOPROCShape* > shapes;

	std::vector< RDOPROCShape* >::iterator find( const RDOPROCShape* shape );

	CRect getBoundingRect( RDOPROCShape* shape ) const;
	RDOPROCShape* findObject( const int x, const int y ) const;

	void moving_start( RDOPROCShape* shape, const int global_mouse_x, const int global_mouse_y );
	void moving( const int global_mouse_x, const int global_mouse_y );
	void moving_stop();

	int                        global_old_x;
	int                        global_old_y;
//	RDOPROCShape*              movingShape;
	std::list< RDOPROCShape* > movingShapes;

	static void correctPoint( const std::vector< CPoint >& pa, CPoint& point );

public:
	RDOPROCFlowChart();
	virtual ~RDOPROCFlowChart();

	void snapToGrid( RDOPROCShape* shape );
//	void snapToGridAllShapes();

	int getGridStep() const                  { return grid_step;   }
//	void setGridStep( const int value );

	const CPen&   getPenShape() const       { return pen_shape_default; }
	const CPen&   getPenShapeColor() const  { return pen_shape_color;   }
	const CBrush& getBrushSelectBox() const { return brush_select_box;  }
	
	int getPenShapeWidth() const             { return shape_pen_width; }
	int getPaperBorderWidth() const          { return paper_border_w;  }
//	void setPaperBorderWidth( const int value );

	int getPaperBorderHeight() const         { return paper_border_h; }
//	void setPaperBorderHeight( const int value );

	void selectShapeOff();
	void selectShape( RDOPROCShape* shape, const bool value = true );

/*
	GridMode getGridMode() const             { return grid_mode;   }
	void setGridMode( const GridMode value );

	GridType getGridType() const             { return grid_type;   }
	void setGridType( const GridType value );

	bool getGridShow() const                 { return grid_show;   }
	void setGridShow( const bool value );

	int getGridDelta() const                 { return grid_delta;  }
	void setGridDelta( const int value );

	const COLORREF& getGridColor() const     { return grid_color;  }
	void setGridColor( const COLORREF& value );

	const COLORREF& getPaperColor() const    { return paper_bg_color; }
	void setPaperColor( const COLORREF& value );

	const COLORREF& getShapeColor() const    { return shape_color; }
	void setShapeColor( const COLORREF& value );

	bool getShowShapeName() const                { return showShapeName;                     }
	void setShowShapeName( const bool value );

	bool getShowConnectorPoint() const           { return showConnectorPoint;                }
	void setShowConnectorPoint( const bool value );
*/
	static double getLength( const CPoint& point1, const CPoint& point2 );
	static CPoint getPerpendicular( const CPoint& line_point1, const CPoint& line_point2, const CPoint& point, bool& null, bool* inside = NULL );
	static double getDistance( const CPoint& line_point1, const CPoint& line_point2, const CPoint& point, bool* inside = NULL );
	static CPoint getIntersection( const std::vector< CPoint >& pa, const double x1, const double y1, const double x2, const double y2, const double x3, const double y3, const double x4, const double y4, double& Ka, double& Kb, double& K, double& Ua, double& Ub );
	static double getCorner( int x1, int y1, int x2, int y2 );

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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDO_PROCESS_FLOWCHART_H

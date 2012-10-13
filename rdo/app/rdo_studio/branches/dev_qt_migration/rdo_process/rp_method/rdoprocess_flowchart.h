#ifndef RDO_PROCESS_FLOWCHART_H
#define RDO_PROCESS_FLOWCHART_H

#define TEST_SPEED TRUE
#undef TEST_SPEED

// --------------------------------------------------------------------------------
// -------------------- RPFlowChart
// --------------------------------------------------------------------------------
class RPView;

class RPFlowChart: public CWnd
{
friend class RPObjectFlowChart;

private:
	enum GridMode { gtSnapOff, gtSnapToPoint, gtSnapToCenter };
	enum GridType { gtPoints, gtSolidLines, dtDotLines };

	int    saved_dc;
	rbool  scroll_inited;
	CPoint scroll_delta;
	CPoint scroll_size;

	void updateScrollBars();

/*
	GridMode grid_mode;
	GridType grid_type;
	rbool    grid_show;
	int      grid_step;
//	rbool    grid_wasMouseMoving;
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

//	rbool showShapeName;
//	rbool showConnectorPoint;

	RPObjectFlowChart* flowobj;

#ifdef TEST_SPEED
	int sec_cnt;
	int sec_timer;
	int makegrid_cnt;
	int makegridempty_cnt;
#endif

public:
	RPFlowChart( RPObjectFlowChart* _flowobj, RPView* pView );
	virtual ~RPFlowChart();

	void init();
	RPObjectFlowChart& getObjectFlowChart() const { return *flowobj; }
	void setName( const rp::string& value );

	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

private:
	RPView* m_pView;

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

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
	DECLARE_MESSAGE_MAP()
};

#endif // RDO_PROCESS_FLOWCHART_H

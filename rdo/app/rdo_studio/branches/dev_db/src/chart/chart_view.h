/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_view.h
  \author    Захаров Павел
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_CHART_VIEW_H_
#define _RDO_STUDIO_CHART_VIEW_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtWidgets/qwidget.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
#include "app/rdo_studio/rdo_tracer/rdotracervalues.h"
#include "app/rdo_studio/src/chart/chart_doc.h"
#include "thirdparty/qt-solutions/qtwinmigrate/src/qwinhost.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- ChartView
// --------------------------------------------------------------------------------
class RDOStudioChartViewStyle;
class TracerSerie;
class ChartSerie;

class ChartView: public CWnd
{
friend class TracerSerieFindValue;
friend class RDOStudioChartOptionsChart;
friend class ChartViewMainWnd;
friend class TracerSerie;
friend class RDOStudioChartDoc;

public:
	RDOStudioChartDoc* getDocument();
	void attachToDoc();

	QWidget* getQtParent();

	const RDOStudioChartViewStyle& getStyle() const;
	void setStyle(RDOStudioChartViewStyle* _style, const rbool needRedraw = true);

	void setPreviwMode(rbool value);

protected:
	CMutex m_mutex;

	COleDropTarget m_ddTarget;
	TracerSerie*   m_pddSerie;

	CRect m_bmpRect;
	CRect m_newClientRect;

	int m_valueCountX;
	std::vector<tstring> m_captionList;
	int m_valueCountY;

	rbool m_timeWrapFlag;
	rbool canUnwrapTime() const
	{
		return m_scaleKoeff >= 1 && !m_zoomAutoFlag;
	}

	rbool doUnwrapTime() const
	{
		return canUnwrapTime() && !m_timeWrapFlag;
	}

	CRect m_chartRect;
	void recalcLayout();

	int m_xMax;
	int m_xPos;
	rbool minXVisible() const
	{
		return m_xPos == 0;
	}

	rbool maxXVisible() const
	{
		return m_xPos == m_xMax;
	}

	void setScrollPos(UINT nSBCode, UINT nPos, const rbool need_update = true);
	void updateScrollBars(const rbool need_update = true);

	long double   m_timeScale;
	TracerTimeNow m_drawFromX;
	int           m_drawFromEventIndex;
	TracerTimeNow m_drawToX;
	int           m_drawToEventCount;
	int           m_chartShift;
	TimesList     m_unwrapTimesList;

	rbool setTo(const int from_max_pos);
	void  setFromTo();

	void drawTitle(CRect& chartRect);

	CRect m_legendRect;
	void drawLegend(CRect& chartRect);
	void drawYAxis(CRect& chartRect, const ChartSerie* axisValues);
	void drawXAxis(CRect& chartRect);
	void drawGrid(CRect& chartRect);

	CMenu m_popupMenu;
	void copyToClipboard();

	double m_zoom;
	double m_zoomOld;
	double m_zoomAuto;
	double m_scaleKoeff;
	rbool  m_zoomAutoFlag;
	void setZoom(double new_zoom, const rbool force_update = false);

	rbool                    m_previewMode;
	RDOStudioChartViewStyle* m_pStyle;

	ChartSerie* m_pYAxis;
	rbool       m_needDrawLegend;

	void updateWindow();
	void updateView();

	HBITMAP m_hbmp;
	HBITMAP m_hbmpInit;
	HFONT   m_hfontTitle;
	HFONT   m_hfontLegend;
	HFONT   m_hfontAxis;
	HFONT   m_hfontInit;
	HDC     m_hdc;
	int     m_savedHdc;
	HDC     m_hmemdc;
	int     m_savedHmemdc;
	HWND    m_hwnd;
	void setFonts(const rbool needRedraw = true);

	QWidget* m_pParent;

	void onDraw();

private:
	ChartView(QWidget* pParent, RDOStudioChartDoc* pDocument, const rbool preview /* = false*/); //! @todo qt
	virtual ~ChartView();

	RDOStudioChartDoc* m_pDocument;

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);

protected:
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChartTimewrap();
	afx_msg void OnUpdateChartTimewrap(CCmdUI* pCmdUI);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditCopy();
	afx_msg void OnInitMenuPopup( CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu );
	afx_msg void OnContextMenu( CWnd* pWnd, CPoint pos );
	afx_msg void OnChartZoomZoomin();
	afx_msg void OnChartZoomZoomout();
	afx_msg void OnChartZoomResetzoom();
	afx_msg void OnUpdateChartZoomZoomin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateChartZoomZoomout(CCmdUI* pCmdUI);
	afx_msg void OnUpdateChartZoomResetzoom(CCmdUI* pCmdUI);
	afx_msg int OnMouseActivate( CWnd* pDesktopWnd, UINT nHitTest, UINT message );
	afx_msg void OnChartOptions();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnViewZoomauto();
	afx_msg void OnUpdateViewZoomauto(CCmdUI* pCmdUI);
	afx_msg void OnHelpKeyword();
	afx_msg LRESULT OnUserUpdateChartView(WPARAM wParam = 0, LPARAM lParam = 0);
	DECLARE_MESSAGE_MAP()
};

class ChartViewMainWnd:
	public QWidget,
	public IInit
{
public:
	ChartViewMainWnd(RDOStudioChartDoc* pDocument, const rbool preview);
	virtual ~ChartViewMainWnd();

	ChartView* getContext();

private:
	typedef  QWidget  parent_type;

	ChartView*          m_pContext;
	RDOStudioChartDoc*  m_pDocument;
	rbool               m_preview;
	CWnd                m_thisCWnd;

	virtual void resizeEvent(PTR(QResizeEvent) event);
	virtual void paintEvent (PTR(QPaintEvent ) event);
	virtual void closeEvent (PTR(QCloseEvent ) event);

	DECLARE_IInit;
};

#endif // _RDO_STUDIO_CHART_VIEW_H_

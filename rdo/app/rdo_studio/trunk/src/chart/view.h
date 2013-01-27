/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/chart/view.h
  \author    Захаров Павел
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_CHART_VIEW_H_
#define _RDO_STUDIO_MFC_CHART_VIEW_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qwidget.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracervalues.h"
#include "app/rdo_studio_mfc/src/chart/document.h"
#include "thirdparty/qt-solutions/qtwinmigrate/src/qwinhost.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartView
// --------------------------------------------------------------------------------
class RDOStudioChartViewStyle;
class TracerSerie;
class ChartSerie;

class RDOStudioChartView : public CWnd
{
friend class TracerSerieFindValue;
friend class RDOStudioChartOptionsChart;
friend class RDOStudioChartViewQt;
friend class TracerSerie;
friend class RDOStudioChartDoc;

protected:
	CMutex mutex;

	COleDropTarget target;
	TracerSerie* dragedSerie;

	CRect bmpRect;
	CRect newClientRect;

	int valueCountX;
	std::vector<tstring> captions;
	int valueCountY;
	
	rbool timeWrap;
	rbool canUnwrapTime() const { return scale_koeff >= 1 && !zoomAuto; };
	rbool doUnwrapTime() const { return canUnwrapTime() && !timeWrap; };

	CRect chartRect;
	void recalcLayout();

	int xMax;
	int xPos;
	rbool minXVisible() const { return xPos == 0; };
	rbool maxXVisible() const { return xPos == xMax; };
	void  setScrollPos( UINT nSBCode, UINT nPos, const rbool need_update = true );
	void  updateScrollBars( const rbool need_update = true );

	long double timeScale;
	TracerTimeNow drawFromX;
	int drawFromEventIndex;
	TracerTimeNow drawToX;
	int drawToEventCount;
	int chartShift;
	rbool setTo( const int from_max_pos );
	void setFromTo();
	timesList unwrapTimesList;
	
	void drawTitle( CRect& chartRect );
	CRect legendRect;
	void drawLegend( CRect& chartRect );
	void drawYAxis( CRect& chartRect, const ChartSerie* axisValues );
	void drawXAxis( CRect& chartRect );
	void drawGrid( CRect& chartRect );

	CMenu popupMenu;
	void copyToClipboard();

	double zoom;
	double old_zoom;
	double auto_zoom;
	double scale_koeff;
	void  setZoom( double new_zoom, const rbool force_update = false );
	rbool zoomAuto;

	rbool previewMode;
	RDOStudioChartViewStyle* style;

	ChartSerie* yAxis;
	rbool needDrawLegend;

	void updateWindow();
	void updateView();

	HBITMAP hbmp;
	HBITMAP hbmpInit;
	HFONT   hfontTitle;
	HFONT   hfontLegend;
	HFONT   hfontAxis;
	HFONT   hfontInit;
	HDC     hdc;
	int     saved_hdc;
	HDC     hmemdc;
	int     saved_hmemdc;
	HWND    hwnd;
	void setFonts( const rbool needRedraw = true );

	QWidget* m_pParent;

	void onDraw();

private:
	RDOStudioChartView(QWidget* pParent, RDOStudioChartDoc* pDocument, const rbool preview /* = false*/); //! @todo qt
	virtual ~RDOStudioChartView();

public:
	RDOStudioChartDoc* GetDocument();
	void attachToDoc();

	QWidget* getQtParent();

	const RDOStudioChartViewStyle& getStyle() const;
	void setStyle( RDOStudioChartViewStyle* _style, const rbool needRedraw = true );

	void setPreviwMode(rbool value);

private:
	RDOStudioChartDoc* m_pDocument;

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual DROPEFFECT OnDragEnter( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
	virtual void OnDragLeave();
	virtual DROPEFFECT OnDragOver( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
	virtual BOOL OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point );

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

class RDOStudioChartViewQt:
	public QWidget,
	public IInit
{
public:
	RDOStudioChartViewQt(RDOStudioChartDoc* pDocument, const rbool preview);
	virtual ~RDOStudioChartViewQt();

	RDOStudioChartView* getContext();

private:
	typedef  QWidget  parent_type;

	RDOStudioChartView* m_pContext;
	RDOStudioChartDoc*  m_pDocument;
	rbool               m_preview;
	CWnd                m_thisCWnd;

	virtual void resizeEvent(PTR(QResizeEvent) event);
	virtual void paintEvent (PTR(QPaintEvent ) event);
	virtual void closeEvent (PTR(QCloseEvent ) event);

	DECLARE_IInit;
};

#endif // _RDO_STUDIO_MFC_CHART_VIEW_H_

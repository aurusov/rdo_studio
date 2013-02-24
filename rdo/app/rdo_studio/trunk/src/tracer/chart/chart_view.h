/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_view.h
  \author    Захаров Павел
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_CHART_VIEW_H_
#define _RDO_STUDIO_TRACER_CHART_VIEW_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qabstractscrollarea.h>
#include <QtWidgets/qscrollbar.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/tracer_values.h"
#include "app/rdo_studio/src/tracer/chart/chart_doc.h"
#include "app/rdo_studio/src/action_activator/action_activator_widget.h"
#include "ui/qt/headers/scroll_bar/scroll_bar.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

// --------------------------------------------------------------------------------
// -------------------- ChartView
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(Serie);
class ChartViewStyle;
class ChartSerie;

class ChartView: public ActionActivatorWidget
{
Q_OBJECT

public:
	ChartView(QAbstractScrollArea* pParent, ChartDoc* pDocument, const rbool preview);
	virtual ~ChartView();

	ChartDoc* getDocument();
	void      attachToDoc();

	const ChartViewStyle& getStyle() const;
	void setStyle(ChartViewStyle* pStyle, const rbool needRedraw = true);

	void setPreviwMode(rbool value);

	int  getValueCountX() const;
	void setValueCountX(int value);
	int  getValueCountY() const;
	void setValueCountY(int value);

	void updateView();
	void onUserUpdateChartView(ruint updateType);

	void setYAxis(ChartSerie* pSerie);

	rbool isDrawLegend () const;
	void  setDrawLegend(rbool value);

	long double  timeScale       () const;
	int          chartShift      () const;
	CREF(Time)   drawFromX       () const;
	CREF(Time)   drawToX         () const;
	int          drawFromEventID () const;
	int          drawToEventCount() const;

	CREF(ChartDoc::TimesList)            unwrapTimesList() const;
	const ChartViewStyle* const style          () const;

	rbool doUnwrapTime () const;

private:
	COleDropTarget m_ddTarget;
	LPSerie  m_pddSerie;

	QRect m_bmpRect;
	QRect m_clientRect;

	int m_valueCountX;
	std::vector<tstring> m_captionList;
	int m_valueCountY;

	rbool m_timeWrapFlag;
	rbool canUnwrapTime() const;

	QRect m_chartRect;
	void recalcLayout();

	rdo::gui::ScrollMetric m_SM_X;
	rbool       minXVisible       () const;
	rbool       maxXVisible       () const;
	QScrollBar& getHorzScrollBar  ();
	void        updateScrollBars  ();
	rbool       scrollHorizontally(rsint inc);

	long double         m_timeScale;
	int                 m_chartShift;
	Time                m_drawFromX;
	Time                m_drawToX;
	int                 m_drawFromEventID;
	int                 m_drawToEventCount;
	ChartDoc::TimesList m_unwrapTimesList;

	rbool setTo(const int fromMaxPos);
	void  setFromTo();

	void drawTitle(QPainter& painter, const QRect& chartRect);

	QRect m_legendRect;
	void drawLegend(QPainter& painter, const QRect& chartRect);
	void drawYAxis (QPainter& painter, const QRect& chartRect, const ChartSerie* axisValues);
	void drawXAxis (QPainter& painter, const QRect& chartRect);
	void drawGrid  (QPainter& painter, const QRect& chartRect);

	QMenu* m_pPopupMenu;

	double m_zoom;
	double m_zoomOld;
	double m_zoomAuto;
	double m_scaleKoeff;
	rbool  m_zoomAutoFlag;
	void setZoom(double new_zoom, const rbool force_update = false);

	rbool           m_previewMode;
	ChartViewStyle* m_pStyle;

	ChartSerie* m_pYAxis;
	rbool       m_needDrawLegend;

	QFont   m_fontTitle;
	QFont   m_fontLegend;
	QFont   m_fontAxis;
	void setFonts(const rbool needRedraw = true);

	void onDraw();

	typedef  ActionActivatorWidget  super;

	ChartDoc* m_pDocument;

	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);

	virtual void resizeEvent    (QResizeEvent* pEvent);
	virtual void paintEvent     (QPaintEvent*  pEvent);
	virtual void mousePressEvent(QMouseEvent*  pEvent);
	virtual void keyPressEvent  (QKeyEvent*    pEvent);
	virtual void wheelEvent     (QWheelEvent*  pEvent);

	virtual void onUpdateActions(rbool activated);

private slots:
	void onHorzScrollBarValueChanged(int value);

	void onEditCopy();

	void onViewZoomIn();
	void onViewZoomOut();
	void onViewZoomReset();
	void onViewZoomAuto();
	
	void onChartTimeWrap();
	void onChartOptions();

	void onHelpKeyword();
};

// --------------------------------------------------------------------------------
// -------------------- ChartViewMainWnd
// --------------------------------------------------------------------------------
class ChartViewMainWnd: public QAbstractScrollArea
{
public:
	ChartViewMainWnd(PTR(QWidget) pParent, PTR(ChartDoc) pDocument, rbool preview);
	virtual ~ChartViewMainWnd();

	ChartView& view();

private:
	typedef  QAbstractScrollArea  super;

	virtual rbool viewportEvent(PTR(QEvent)      pEvent);
	virtual void  focusInEvent (PTR(QFocusEvent) pEvent);
	virtual void  focusOutEvent(PTR(QFocusEvent) pEvent);
	virtual void  keyPressEvent(PTR(QKeyEvent)   pEvent);
};

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_CHART_VIEW_H_

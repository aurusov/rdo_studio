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
#include <QtWidgets/qabstractscrollarea.h>
#include <QtWidgets/qscrollbar.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/rdo_tracer/rdotracervalues.h"
#include "app/rdo_studio/src/chart/chart_doc.h"
#include "app/rdo_studio/src/action_activator/action_activator_widget.h"
#include "ui/qt/headers/scroll_bar/scroll_bar.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- ChartView
// --------------------------------------------------------------------------------
class RDOStudioChartViewStyle;
class TracerSerie;
class ChartSerie;

class ChartView: public ActionActivatorWidget
{
Q_OBJECT

friend class TracerSerieFindValue;
friend class ChartPreferences;
friend class ChartViewMainWnd;
friend class TracerSerie;
friend class RDOStudioChartDoc;

public:
	RDOStudioChartDoc* getDocument();
	void attachToDoc();

	const RDOStudioChartViewStyle& getStyle() const;
	void setStyle(RDOStudioChartViewStyle* pStyle, const rbool needRedraw = true);

	void setPreviwMode(rbool value);

private:
	CMutex m_mutex;

	COleDropTarget m_ddTarget;
	TracerSerie*   m_pddSerie;

	QRect m_bmpRect;
	QRect m_clientRect;

	int m_valueCountX;
	std::vector<tstring> m_captionList;
	int m_valueCountY;

	rbool m_timeWrapFlag;
	rbool canUnwrapTime() const;
	rbool doUnwrapTime () const;

	QRect m_chartRect;
	void recalcLayout();

	rdo::gui::ScrollMetric m_SM_X;
	rbool       minXVisible       () const;
	rbool       maxXVisible       () const;
	QScrollBar& getHorzScrollBar  ();
	void        updateScrollBars  ();
	rbool       scrollHorizontally(rsint inc);

	long double   m_timeScale;
	TracerTimeNow m_drawFromX;
	int           m_drawFromEventIndex;
	TracerTimeNow m_drawToX;
	int           m_drawToEventCount;
	int           m_chartShift;
	TimesList     m_unwrapTimesList;

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

	rbool                    m_previewMode;
	RDOStudioChartViewStyle* m_pStyle;

	ChartSerie* m_pYAxis;
	rbool       m_needDrawLegend;

	void updateView();

	QFont   m_fontTitle;
	QFont   m_fontLegend;
	QFont   m_fontAxis;
	void setFonts(const rbool needRedraw = true);

	void onDraw();

	typedef  ActionActivatorWidget  super;

	ChartView(QAbstractScrollArea* pParent, RDOStudioChartDoc* pDocument, const rbool preview /* = false*/); //! @todo qt
	virtual ~ChartView();

	RDOStudioChartDoc* m_pDocument;

	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);

	virtual void resizeEvent    (QResizeEvent* pEvent);
	virtual void paintEvent     (QPaintEvent*  pEvent);
	virtual void mousePressEvent(QMouseEvent*  pEvent);
	virtual void keyPressEvent  (QKeyEvent*    pEvent);
	virtual void wheelEvent     (QWheelEvent*  pEvent);

	void onUserUpdateChartView(ruint updateType);

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
	ChartViewMainWnd(PTR(QWidget) pParent, PTR(RDOStudioChartDoc) pDocument, rbool preview);
	virtual ~ChartViewMainWnd();

	ChartView& view();

private:
	typedef  QAbstractScrollArea  super;

	virtual rbool viewportEvent  (PTR(QEvent)      pEvent);
	virtual void  focusInEvent   (PTR(QFocusEvent) pEvent);
	virtual void  focusOutEvent  (PTR(QFocusEvent) pEvent);
	virtual void  keyPressEvent  (PTR(QKeyEvent)   pEvent);
};

#endif // _RDO_STUDIO_CHART_VIEW_H_

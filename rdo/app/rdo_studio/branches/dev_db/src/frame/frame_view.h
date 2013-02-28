/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      frame_view.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_FRAME_VIEW_H_
#define _RDO_STUDIO_FRAME_VIEW_H_

// ----------------------------------------------------------------------- INCLUDES
#include <map>
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qscrollarea.h>
#include <QtGui/qevent.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdoanimation.h"
#include "ui/qt/headers/memdc/memdc.h"
#include "ui/qt/headers/animation/area.h"
#include "ui/qt/headers/bitmap/bitmap.h"
// --------------------------------------------------------------------------------

class FrameAnimationContent: public QWidget
{
private:
	Q_OBJECT
	typedef  QWidget  parent_type;

public:
	FrameAnimationContent(PTR(QWidget) pParent);
	virtual ~FrameAnimationContent();

	void update    (CPTRC(rdo::animation::Frame)         pFrame,
	                 CREF(rdo::gui::BitmapList)          bitmapList,
	                  REF(rdo::gui::BitmapList)          bitmapGeneratedList,
	                  REF(rdo::gui::animation::AreaList) areaList);
	void updateFont();

private:
	rdo::gui::QtMemDC  m_memDC;
	QColor             m_bgColor;
	QSize              m_size;
	QPoint             m_pos;
	QFont              m_font;

	rbool valid           () const;
	void  init            (CPTRC(rdo::animation::Frame) pFrame, CREF(rdo::gui::BitmapList) bitmapList);
	void  init            (CREF(QSize)   size );
	void  setBGColor      (CREF(QColor)  color);
	void  onDraw          (REF(QPainter) painter);

	void  drawBackground    (CPTRC(rdo::animation::Frame) pFrame, CREF(rdo::gui::BitmapList) bitmapList);
	template <class F>
	void  drawColoredElement(CPTR(rdo::animation::ColoredElement)   pColor, F drawMethod);
	void  setColors         (CPTR(rdo::animation::ColoredElement)   pColor);
	void  elementText       ( PTR(rdo::animation::TextElement)      pElement);
	void  elementRect       ( PTR(rdo::animation::RectElement)      pElement);
	void  elementRoundRect  ( PTR(rdo::animation::RoundRectElement) pElement);
	void  elementLine       ( PTR(rdo::animation::LineElement)      pElement);
	void  elementTriang     ( PTR(rdo::animation::TriangElement)    pElement);
	void  elementCircle     ( PTR(rdo::animation::CircleElement)    pElement);
	void  elementEllipse    ( PTR(rdo::animation::EllipseElement)   pElement);
	void  elementBMP        ( PTR(rdo::animation::BmpElement)       pElement, CREF(rdo::gui::BitmapList) bitmapList, REF(rdo::gui::BitmapList) bitmapGeneratedList);
	void  elementSBMP       ( PTR(rdo::animation::ScaledBmpElement) pElement, CREF(rdo::gui::BitmapList) bitmapList, REF(rdo::gui::BitmapList) bitmapGeneratedList);
	void  elementActive     ( PTR(rdo::animation::ActiveElement)    pElement, REF(rdo::gui::animation::AreaList) areaList);

	static QPixmap getBitmap(CREF(QString)              bitmapName,
	                         CREF(QString)              maskName,
	                         CREF(rdo::gui::BitmapList) bitmapList,
	                          REF(rdo::gui::BitmapList) bitmapGeneratedList);

private:
	virtual void resizeEvent    (QResizeEvent* pEvent);
	virtual void paintEvent     (QPaintEvent*  pEvent);
	virtual void mousePressEvent(QMouseEvent*  pEvent);
};

// --------------------------------------------------------------------------------
// -------------------- FrameAnimationWnd
// --------------------------------------------------------------------------------
class FrameAnimationWnd: public QScrollArea
{
private:
	Q_OBJECT
	typedef  QScrollArea  parent_type;

public:
	FrameAnimationWnd(PTR(QWidget) pParent);
	virtual ~FrameAnimationWnd();

	PTR(FrameAnimationContent) getContent();

	using QWidget::update;

	void updateFont();

	void update(CPTRC(rdo::animation::Frame)         pFrame,
	             CREF(rdo::gui::BitmapList)          bitmapList,
	              REF(rdo::gui::BitmapList)          bitmapGeneratedList,
	              REF(rdo::gui::animation::AreaList) areaList);

private:
	PTR(QWidget) m_pContent;

	virtual rbool event(QEvent* pEvent);
};

#endif // _RDO_STUDIO_FRAME_VIEW_H_

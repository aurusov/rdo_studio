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
#include "utils/src/common/warning_disable.h"
#include <map>
#include <QWidget>
#include <QScrollArea>
#include <QEvent>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/animation/rdoanimation.h"
#include "ui/qt/headers/memdc/memdc.h"
#include "ui/qt/headers/animation/area.h"
#include "ui/qt/headers/bitmap/bitmap.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace frame {

// --------------------------------------------------------------------------------
// -------------------- Content
// --------------------------------------------------------------------------------
class Content: public QWidget
{
private:
	Q_OBJECT
	typedef  QWidget  parent_type;

public:
	Content(QWidget* pParent);
	virtual ~Content();

	void update(const rdo::animation::Frame* const pFrame,
		CREF(rdo::gui::BitmapList) bitmapList,
		REF(rdo::gui::BitmapList) bitmapGeneratedList,
		REF(rdo::gui::animation::AreaList) areaList);
	void updateFont();

private:
	rdo::gui::QtMemDC  m_memDC;
	QColor             m_bgColor;
	QSize              m_size;
	QPoint             m_pos;
	QFont              m_font;

	bool  valid           () const;
	void  init            (const rdo::animation::Frame* const pFrame, CREF(rdo::gui::BitmapList) bitmapList);
	void  init            (CREF(QSize)   size );
	void  setBGColor      (CREF(QColor)  color);
	void  onDraw          (REF(QPainter) painter);

	void  drawBackground    (const rdo::animation::Frame* const pFrame, CREF(rdo::gui::BitmapList) bitmapList);
	template <class F>
	void  drawColoredElement(const rdo::animation::ColoredElement* pColor, F drawMethod);
	void  setColors         (const rdo::animation::ColoredElement* pColor);
	void  elementText       (rdo::animation::TextElement*      pElement);
	void  elementRect       (rdo::animation::RectElement*      pElement);
	void  elementRoundRect  (rdo::animation::RoundRectElement* pElement);
	void  elementLine       (rdo::animation::LineElement*      pElement);
	void  elementTriang     (rdo::animation::TriangElement*    pElement);
	void  elementCircle     (rdo::animation::CircleElement*    pElement);
	void  elementEllipse    (rdo::animation::EllipseElement*   pElement);
	void  elementBMP        (rdo::animation::BmpElement*       pElement, CREF(rdo::gui::BitmapList) bitmapList, REF(rdo::gui::BitmapList) bitmapGeneratedList);
	void  elementSBMP       (rdo::animation::ScaledBmpElement* pElement, CREF(rdo::gui::BitmapList) bitmapList, REF(rdo::gui::BitmapList) bitmapGeneratedList);
	void  elementActive     (rdo::animation::ActiveElement*    pElement, REF(rdo::gui::animation::AreaList) areaList);

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
// -------------------- View
// --------------------------------------------------------------------------------
class View: public QScrollArea
{
private:
	Q_OBJECT
	typedef  QScrollArea  parent_type;

public:
	View(QWidget* pParent);
	virtual ~View();

	Content* getContent();

	using QWidget::update;

	void updateFont();

	void update(const rdo::animation::Frame* const pFrame,
		CREF(rdo::gui::BitmapList) bitmapList,
		REF(rdo::gui::BitmapList) bitmapGeneratedList,
		REF(rdo::gui::animation::AreaList) areaList);

private:
	QWidget* m_pContent;

	virtual bool event(QEvent* pEvent);
};

}}} // namespace rdo::gui::frame

#endif // _RDO_STUDIO_FRAME_VIEW_H_

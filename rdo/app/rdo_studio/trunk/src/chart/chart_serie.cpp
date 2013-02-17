/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_serie.cpp
  \author    Захаров Павел
  \date      31.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/chart/chart_serie.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- ChartSerie::Options
// --------------------------------------------------------------------------------
ChartSerie::Options::Options()
	: color            (QColor(0x00, 0x00, 0x00))
	, markerType       (RDOSM_NONE)
	, markerSize       (3)
	, markerNeedDraw   (true)
	, markerTransparent(true)
	, showInLegend     (true)
{}

rbool ChartSerie::Options::operator== (CREF(Options) options) const
{
	return title             == options.title
		&& color             == options.color
		&& markerType        == options.markerType
		&& markerSize        == options.markerSize
		&& markerNeedDraw    == options.markerNeedDraw
		&& markerTransparent == options.markerTransparent
		&& showInLegend      == options.showInLegend;
}

// --------------------------------------------------------------------------------
// -------------------- ChartSerie
// --------------------------------------------------------------------------------
ChartSerie::ChartSerie(TracerSerie* pSerie)
	: m_pSerie(pSerie)
{
	if (m_pSerie)
	{
		m_options.title = m_pSerie->getTitle();
	}
}

ChartSerie::~ChartSerie()
{}

TracerSerie* ChartSerie::getSerie() const
{
	return m_pSerie;
}

CREF(ChartSerie::Options) ChartSerie::options() const
{
	return m_options;
}

void ChartSerie::setOptions(CREF(Options) options)
{
	m_options = options;
}

rbool ChartSerie::isTracerSerie(const TracerSerie* pSerie) const
{
	return m_pSerie == pSerie;
}

void ChartSerie::drawSerie(ChartView* const pView, QPainter& painter, const QRect& rect) const
{
	m_pSerie->drawSerie(pView, painter, rect, m_options.color, m_options.markerType, m_options.markerSize, m_options.markerNeedDraw, m_options.markerTransparent);
}

void ChartSerie::getCaptions(std::vector<tstring> &captions, const int val_count) const
{
	m_pSerie->getCaptions(captions, val_count);
}

void ChartSerie::lock()
{
	m_pSerie->mutex.Lock();
}

void ChartSerie::unlock()
{
	m_pSerie->mutex.Unlock();
}

rbool ChartSerie::empty() const
{
	return m_pSerie->empty();
}

void ChartSerie::getLegendExtent(const QFontMetrics& fm, QRect& rect, QSize& size) const
{
	size = QSize(0, 0);
	if (!m_options.showInLegend)
		return;

	QRect tmprect;
	tmprect.setLeft(rect.left() + 10 + m_options.markerSize * 2 + 5);
	tmprect.setRight(rect.right());
	tmprect.setTop(rect.top());
	tmprect.setBottom(rect.bottom());
	tmprect = fm.boundingRect(tmprect, Qt::AlignLeft | Qt::TextSingleLine, QString::fromLocal8Bit(m_options.title.c_str()));
	size.setHeight(tmprect.height());
	if (size.height() < m_options.markerSize * 2)
	{
		size.setHeight(m_options.markerSize * 2);
	}
	size.setWidth(tmprect.right() - rect.left());
	size.setHeight(size.height() + 2);
}

void ChartSerie::drawInLegend(QPainter& painter, QRect& rect, const QColor& textColor, QSize& size) const
{
	getLegendExtent(painter.fontMetrics(), rect, size);
	if (!m_options.showInLegend)
		return;

	int middle = rect.top() + (size.height() - 2) / 2;
	if (m_options.markerNeedDraw)
	{
		QPen   pen(m_options.color);
		QBrush brush(m_options.color, m_options.markerTransparent ? Qt::NoBrush : Qt::SolidPattern);
		painter.setPen(pen);
		painter.setBrush(brush);
		m_pSerie->drawMarker(painter, rect.left() + 5 + m_options.markerSize, middle, m_options.markerType, m_options.markerSize);
	}

	painter.drawLine(rect.left(), middle, rect.left() + 10 + m_options.markerSize * 2, middle);

	QRect tmprect(rect);
	tmprect.setLeft(tmprect.left() + m_options.markerSize * 2 + 5);

	//! @todo qt
	//::SetTextColor(dc, textColor);
	painter.setPen(textColor);
	//! @todo qt +DT_END_ELLIPSIS
	painter.drawText(tmprect, Qt::AlignLeft | Qt::TextSingleLine, QString::fromLocal8Bit(m_options.title.c_str()));
}
